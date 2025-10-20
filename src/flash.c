/*
 * flash.c
 *
 *  Created on: 2025/01/05
 *      Author: kameyamahiroki
 */
#include "main.h"
#define DBWE (0x800)
#define DATA_BLOCK0 (0x000)
#define DATA_BLOCK1 (0x800)
#define DATA_BLOCK2 (0x1000)
#define DATA_BLOCK3 (0x1800)
#define DATA_BLOCK4 (0x2000)
#define DATA_BLOCK5 (0x2800)
#define DATA_BLOCK6 (0x3000)
#define DATA_BLOCK7 (0x3800)
#define DATA_BLOCK8 (0x4000)
#define DATA_BLOCK9 (0x4800)
#define DATA_BLOCK10 (0x5000)
#define DATA_BLOCK11 (0x5800)
#define DATA_BLOCK12 (0x6000)
#define DATA_BLOCK13 (0x6800)
#define DATA_BLOCK14 (0x7000)
#define DATA_BLOCK15 (0x7800)
/****************************************
 * ワークエリア定義						*
 ***************************************/
unsigned char e2_FLASH;
volatile unsigned char FCU_RAM[8192];
volatile unsigned char FCU_FIRM_WARE[8192];

unsigned long e2_timeout_check_area;
unsigned short offset 				= 0;//アドレスを2Kバイトづつオフセットさせる変数

//ROMに格納されているFCUファームをFCURAM領域に格納する
//#pragma address FCU_RAM = 0x007f8000
//#pragma address FCU_FIRM_WARE = 0xfeffe000
void fcu_firmware_transfer(void)
{
	FLASH.FENTRYR.WORD = 0;//FCUを停止
	FLASH.FCURAME.WORD = 0xc401;//FCURAMアクセス許可状態にする
	memcpy(&FCU_RAM,&FCU_FIRM_WARE,8192);
}

void fcu_initialize(void)
{
	FLASH.FRESETR.BIT.FRESET 	= 1;
	cmt2_wait(210,0);				//35μs待機
	FLASH.FRESETR.BIT.FRESET 	= 0;
	FLASH.PCKAR.WORD				= 0x30;
	//周辺クロック通知コマンド使用
	return fcu_wait();
}
//FCUコマンド使用
void fcu_command(void)
{

}
/*
 * FCUが処理を完了するまで待機
 */
unsigned char fcu_wait(void)
{
	e2_timeout_check_area			= 1;	//1msでタイムアウト
	while(FLASH.FSTATR0.BIT.FRDY == 0){
		if(e2_timeout_check_area == 0){		//タイムアウト判定タイムアウトしたらＦＣＵを初期化してエラーを返す
			fcu_initialize();
			return ERROR;
		}
	}
	if(FLASH.FSTATR0.BIT.ERSERR || FLASH.FSTATR0.BIT.ILGLERR == 1)//FCUが不正なコマンドや、不正なROM/データフラッシュアクセスを検出したかチェック
		return ERROR;
	return SUCCESS;
}

//ロックビットリード２コマンドはデータフラッシュのブランクチェックを兼ねている
//1サイクル目：アドレス　EA　データ　0x71
//2サイクル目：アドレス　BA　データ　0xd0
//FCUへのコマンド発行は、データフラッシュ領域に対するライトアクセスで実現されます（P１８４２）
//データフラッシュアドレス0x0010 0000~0010 7fff
//FCUコマンドの使用方法P1844
/*ブランクチェックの第一サイクルでは、７１hをデータフラッシュ領域のアドレスにバイト書き込みします。
 * コマンドの第二サイクルでは、ブランクチェック対象領域を含む消去ブロック内の任意のアドレスにD0h
 * をバイト書き込みすると、FCUがデータフラッシュのブランクチェック処理を開始します。
 * P1847にフローチャート
 * 2Kバイトデータのブランクチェック書き込み時間700μs
 * 書き込み・消去中のリセットパルス幅
 * */
//何バイト保存するか決める
//プレーヤー情報 	70 * 1	= 70バイト
//自動演奏情報 		40 * 3	= 120バイト
//敵情報 			74 * 5 	= 370バイト
//味方情報　		74 * 4 	= 296バイト
//合計　				　　  856バイト
/*
 * 保存する必要のある情報
 * 	プレーヤー情報
 * 	敵情報
 *  味方情報
 * 	演奏中の曲情報
 */

/****************************************/
/*ブランクチェック						*/
/*unsigned char e2_blank_check(void)	*/
/*　戻り値：unsigned char */
/****************************************/
unsigned char e2_blank_check(void)
{
	FLASH.FMODR.BIT.FRDMD = 1;						//レジスタリード方式に設定　ブランクチェックコマンドを使用する場合に設定
	FLASH.DFLBCCNT.BIT.BCSIZE = 1;					//ブランクチェックのサイズを2Kバイトに指定
	FLASH.FENTRYR.WORD					= 0xaa80;	//データフラッシュをP/EノーマルモードにするFCUコマンドを使用するため
	timer_area_registration(e2_timeout_check_area);
	while(offset < 32768){							//最大32Kバイトブランクチェックするまで繰り返す
		e2_FLASH							= 0x71;		//ブランクチェック第一サイクルロックビットリードモードに移行
		*(&e2_FLASH + offset)			= 0xd0;		//ブランクチェック第二サイクルブランクチェックしたいアドレスにD0h書き込み
		;
		if (Ffcu_wait() == ERROR) {					//処理待ち、タイムアウト、エラー判定
			return ERROR;
		}
		if(FLASH.DFLBCSTAT.BIT.BCST == BLANK){
			return offset;							//ブランクの先頭アドレスを返す
		}
		offset						+= 2048;			//次の2Kバイトをブランクチェックする
	}
	return WRITTEN_STATE;//32Kバイト書き込まれていた時
}

/***********************************************
 *指定されたバイト数データフラッシュから消去
 *
 */
void e2data_erase(unsigned char erase_address)
{
	unsigned char bit_point				= 0;
	FLASH.FENTRYR.WORD					= 0xaa80;	//データフラッシュP/Eノーマルモードにする
	FLASH.FWEPROR.BYTE					= 0x01;		//書き込み消去可能にする
	bit_point							= erase_address % DBWE;
	if(bit_point <= DATA_BLOCK7)
		FLASH.DFLWE0.WORD				= 0x1e00 + (1 << bit_point);
	else
		FLASH.DFLWE1.WORD				= 0x1e00 + (1 << bit_point);
	e2_FLASH								= 0x20;		//ブロックイレーズ第一サイクル
	*(&e2_FLASH + offset)				= 0xd0;		//ブロックイレーズ第二サイクル消去したいアドレスにD0h書き込み
	return fcu_wait();
}

void e2data_all_erase(void)
{
	int i;
	for(i = 0;i < ;i += DBWE){
		e2data_erase();
	}
}

/***********************************************
 *
 */
unsigned char e2_writing(unsigned short addr)
{
	e2_FLASH				= 0xe8;
	e2_FLASH				= 0x40;//ワード数を64（128バイト）に設定
//	*(&e2_FLASH + offset)	= ;
	cmt2_wait(1875,2);//5ms待機
}

void flash_error(void)
{

}
