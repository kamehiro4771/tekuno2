/*
 * flash.c
 *
 *  Created on: 2025/01/05
 *      Author: kameyamahiroki
 */
#include "main.h"
unsigned char e2_FLASH;
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
 * をバイト書き込みすると、FCUがデータフラッシュのブランクチェック処理を開始します。*/
unsigned char blank_check(void)
{
	unsigned short offset;//アドレスを2Kバイトづつオフセットさせる変数
	FLASH.FMODR.BIT.FRDMD		= 1;//FCUリードモードをレジスタリードモードに設定
	FLASH.DFLBCCNT.BIT.BCSIZE	= 1;//ブランクチェックのサイズを2Kバイトに指定
	for(offset = 0;offset < 32000;offset += 2000){
		e2_FLASH					= 0x71;//ブランクチェック第一サイクル
		*(&e2_FLASH + offset)		= 0xd0;//ブランクチェック第二サイクル
		if(FLASH.DFLBCSTAT.BIT.BCST == 1){
			//データが書き込まれた状態
		}
	}

}

