/*
 * flash.c
 *
 *  Created on: 2025/01/05
 *      Author: kameyamahiroki
 */
#include "main.h"

/****************************************
 * ワークエリア定義						*
 ***************************************/
unsigned char e2_FLASH;
unsigned long e2_timeout_check_area;
unsigned short offset 				= 0;//アドレスを2Kバイトづつオフセットさせる変数
//E2データフラッシュのブランクチェック
//ブランクなら0をデータが書かれていたら1を返す
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
//128の倍数で
//データROMのアドレスの先頭からブランクチェックして書き込まれているアドレスを求める
//書き込まれていなかったらBLANKを返す
/*
 * アドレス0x0010　0000から書き込み
 * 128バイトずつ書き込む
 * 最初にブランクチェックして消去状態のアドレス保持しておく
 */
/*
P1845データフラッシュ書き込みのフローチャート
P1847データフラッシュのブランクチェックフローチャート

*/
/****************************************/
/*ブランクチェック						*/
/*unsigned char e2_blank_check(void)	*/
/*　戻り値：unsigned char */
/****************************************/
unsigned char e2_blank_check(void)
{
	FLASH.FMODR.BIT.FRDMD = 1;	//レジスタリード方式に設定　ブランクチェックコマンドを使用する場合に設定
	FLASH.DFLBCCNT.BIT.BCSIZE = 1;	//ブランクチェックのサイズを2Kバイトに指定
	FLASH.FENTRYR.FENTRY0				= 0;	//データフラッシュP/EモードはFENTRY.FENTRY0ビットを０かつFENTRYR.FENTRYDビットを１
	FLASH.FENTRYR.FENTRYD				= 1;	//データフラッシュをP/EモードにするFCUコマンドを使用するためにROM　P/Eモードへ移行
	FLASH.FWEPROR.FLWE					= 1;	//書き込み消去プロテクト解除
	timer_area_registration(e2_timeout_check_area);
	while(offset < 32768){						//最大32Kバイトブランクチェックするまで繰り返す
		e2_FLASH						= 0x71;	//ブランクチェック第一サイクル
		*(&e2_FLASH + offset)			= 0xd0;	//ブランクチェック第二サイクルブランクチェックしたいアドレスにD0h書き込み
		e2_timeout_check_area			= 1;	//1msでタイムアウト
		while(FLASH.FSTATR0.BIT.FRDY == 0){
			if(e2_timeout_check_area == 0){		//タイムアウト判定タイムアウトしたらＦＣＵを初期化してエラーを返す
				FLASH.FRESETR.BIT.FRESET = 1;
				cmt2_wait(210,0);				//35μs待機
				FLASH.FRESETR.BIT.FRESET = 0;
				return ERROR;
			}
		}
		if (FLASH.FSTATR0.BIT.ILGLERR == 1) {//FCUが不正なコマンドや、不正なROM/データフラッシュアクセスを検出したかチェック
			return ERROR;
		}ERRORとWRITTEN_STATEが同じ定義だから変更が必要

		if(FLASH.DFLBCSTAT.BIT.BCST == BLANK){
			if(offset == 0)
				return BLANK;
			else
				return WRITTEN_STATE;
		}
		offset						+= 2048;//次の2Kバイトをブランクチェックする
	}
	return WRITTEN_STATE;//32Kバイト書き込まれていた時
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
