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
unsigned char *e2_FLASH;
volatile unsigned char FCU_RAM[8192];
volatile unsigned char FCU_FIRM_WARE[8192];

unsigned long e2_timeout_check_area;
unsigned short offset 				= 0;//アドレスを2Kバイトづつオフセットさせる変数

unsigned char fcu_wait(unsigned short wait_time);
//ROMに格納されているFCUファームをFCURAM領域に格納する
//#pragma address FCU_RAM = 0x007f8000
//#pragma address FCU_FIRM_WARE = 0xfeffe000
void fcu_firmware_transfer(void)
{
	FLASH.FENTRYR.WORD = 0;//FCUを停止
	FLASH.FCURAME.WORD = 0xc401;//FCURAMアクセス許可状態にする
	memcpy(&FCU_RAM,&FCU_FIRM_WARE,8192);
}

unsigned char fcu_initialize(void)
{
	volatile unsigned int *wptr = (volatile unsigned int *)e2_FLASH;
	fcu_firmware_transfer();
	FLASH.FRESETR.WORD 			= 0xcc01;	//FCUをリセットする
	cmt2_wait(210,0);						//35μs待機
	FLASH.FRESETR.BIT.FRESET 	= 0xcc00;	//リセット終了FCUコマンドを使用可能にする
	FLASH.PCKAR.WORD				= 0x30;		//周辺クロックを48MHｚに設定
	//周辺クロック通知コマンド使用
	e2_FLASH						= 0xe9;
	e2_FLASH						= 0x03;
	wptr[0]						= 0x0f0f;
	wptr[1]						= 0x0f0f;
	wptr[2]						= 0x0f0f;
	return fcu_wait(1);
}

/*
 * FCUが処理を完了するまで待機
 */
//待機時間を引数に貰う仕様にする
unsigned char fcu_wait(unsigned short wait_time)
{
	e2_timeout_check_area			= wait_time;	//1msでタイムアウト
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
/*ブランクチェック											   */
/*unsigned char e2_blank_check(void)				   */
/*　戻り値：unsigned short ブランクだったアドレス。0ｘ100000からのオフセット値 */
/*******************************************************/
unsigned short e2_blank_check(void)
{
	FLASH.FMODR.BIT.FRDMD = 1;						//レジスタリード方式に設定　ブランクチェックコマンドを使用する場合に設定
	FLASH.DFLBCCNT.BIT.BCSIZE = 1;					//ブランクチェックのサイズを2Kバイトに指定
	FLASH.FENTRYR.WORD					= 0xaa80;	//データフラッシュをP/EノーマルモードにするFCUコマンドを使用するため
	timer_area_registration(e2_timeout_check_area);
	while(offset < 32768){							//最大32Kバイトブランクチェックするまで繰り返す
		e2_FLASH							= 0x71;		//ブランクチェック第一サイクルロックビットリードモードに移行
		*(&e2_FLASH + offset)			= 0xd0;		//ブランクチェック第二サイクルブランクチェックしたいアドレスにD0h書き込み
		;
		if (fcu_wait(1) == ERROR) {					//処理待ち、タイムアウト、エラー判定
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
unsigned char e2data_erase(unsigned short erase_address)
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
	return fcu_wait(250);
}
/*
 * データフラッシュのデータを全部消去する
 *
 */
/*
void e2data_all_erase(void)
{
	int i;
	for(i = 0;i < ;i += DBWE){
		e2data_erase();
	}
}

*/
/*
 * unsigned short offset 読み出したいアドレスへのオフセット値e2_FLASH + offset
 * void *read_data_buff 読みだしたデータを格納するバッファへのポインタ
 * unsigned int byte_count 読み出すバイト数
 */
unsigned char e2_read(unsigned short offset,void *read_data_buff,unsigned int byte_count)
{
	unsigned int i;
	unsigned char *buff			= (unsigned char *)read_data_buff;
	FLASH.FENTRYR.WORD			= 0xaa00;//ROM/データフラッシュリードモードに移行
	while(FLASH.FENTRYR.WORD == 0x0000){

	}
	for(i = 0;i < byte_count;i++){
		*(buff + i)					= e2_FLASH[offset + i];
	}
	return 0;
}

/***********************************************
 *
 */
unsigned char e2_writing(unsigned short offset,void *write_data,unsigned int word_count)
{
	unsigned int i,j = 0;
	unsigned short *flash_ptr 	= (unsigned short*)(e2_FLASH + offset);
	unsigned short *word_data 	= (unsigned short*)write_data;
	FLASH.FENTRYR.WORD			= 0xaa80;	//データフラッシュP/Eノーマルモードにする
	FLASH.FWEPROR.BYTE			= 0x01;		//書き込み消去可能にする
	FLASH.DFLWE0.WORD			= 0x1eff;	//DB00~DB07ブロックまで書き込み許可
	FLASH.DFLWE1.WORD			= 0x1eff;	//DB08~DB15ブロックまで書き込み許可
	//エラーの確認
	while(word_count){
		e2_FLASH					= 0xe8;
		e2_FLASH					= 0x40;//ワード数を128バイトに設定
		for(i = 0;i < 0x3f;i++){
			if(word_count)
				flash_ptr[j]			= word_data[j];
			else
				flash_ptr[j]			= 0xffff;
			word_count--;
			j++;
		}
		e2_FLASH					= 0xd0;
	//DB08~DB15ブロックまで書き込み許可
		if(fcu_wait(5) == ERROR){
			break;
		}
	}
	FLASH.DFLWE0.WORD			= 0x1e00;	//DB00~DB07ブロックまで書き込み禁止
	FLASH.DFLWE1.WORD			= 0x1e00;
	if(word_count)
		return ERROR;
	return SUCCESS;
}

