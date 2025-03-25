/*
 * flash.c
 *
 *  Created on: 2025/01/05
 *      Author: kameyamahiroki
 */
#include "main.h"

/****************************************
 * ワークエリア定義							*
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
//プレーヤー情報 		= 72バイト
//自動演奏情報 		= 19バイト
//敵情報 = 74 * 5 	= 370バイト
//味方情報　= 72 * 4 	= 288バイト
//合計　				　　749バイト
/*
 * 保存する必要のある情報
 * 	プレーヤー名
 * 	プレーヤーHP
 * 	戦闘中の敵情報
 * 	演奏中の曲
 */
//データROMのアドレスの先頭からブランクチェックして書き込まれているアドレスを求める
//書き込まれていなかったらBLANKを返す
unsigned char e2_blank_check(void)
{
	FLASH.FMODR.BIT.FRDMD				= 1;//FCUリードモードをレジスタリードモードに設定
	FLASH.DFLBCCNT.BIT.BCSIZE			= 1;//ブランクチェックのサイズを2Kバイトに指定
	while(offset < 32768){//最大32Kバイトブランクチェック
		e2_FLASH						= 0x71;//ブランクチェック第一サイクル
		*(&e2_FLASH + offset)			= 0xd0;//ブランクチェック第二サイクルブランクチェックしたいアドレスにD0h書き込み
		e2_timeout_check_area			= 1;//1msでタイムアウト
		while(FLASH.FSTATR0.BIT.FRDY == 0){
			timer_area_registration(e2_timeout_check_area);
			if(e2_timeout_check_area == 0){//タイムアウト判定
				FLASH.FRESETR.BIT.FRESET = 1;
				cmt2_wait(210,0);//35μs待機
				FLASH.FRESETR.BIT.FRESET = 0;
			}
		}
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
	e2_FLASH				= 0x40;
	*(&e2_FLASH + offset)	= ;
}
