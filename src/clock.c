/*
 * clock.c
 *メインクロック１２メガヘルツ
 *  Created on: 2023/10/15
 *      Author: kameyamahiroki
 */
#include "iodefine.h"

void clock_initialize(void)
{
	SYSTEM.SCKCR.LONG			= 0x00000100;//システムクロック96MHz、周辺モジュールクロック48MHz、外侮バスクロック96MHzに設定
	SYSTEM.SUBOSCCR.BIT.SUBSTOP	= 1;//サブクロック発振器停止（サブクロックは接続されていない）
}

