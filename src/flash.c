/*
 * flash.c
 *
 *  Created on: 2025/01/05
 *      Author: kameyamahiroki
 */
#include "main.h"
//E2データフラッシュのブランクチェック
//ブランクなら0をデータが書かれていたら1を返す
//ロックビットリード２コマンドはデータフラッシュのブランクチェックを兼ねている
//1サイクル目：アドレス　EA　データ　0x71
//2サイクル目：アドレス　BA　データ　0xd0
unsigned char blank_check(void)
{
	FLASH.FMODR.BIT.FRDMD		= 1;//FCUリードモードをレジスタリードモードに設定
	FLASH.DFLBCCNT.BIT.BCSIZE	= 1;//ブランクチェックのサイズを2バイトに指定
}

