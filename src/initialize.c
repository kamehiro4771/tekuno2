/*
 * initialize.c
 *
 *  Created on: 2023/10/15
 *      Author: kameyamahiroki
 */
#include "main.h"

/********************************************************************/
/*システムの初期化													*/
/*void eneiro_initialize(void)										*/
/********************************************************************/
void eneiro_initialize(void)
{
	clock_initialize();				//クロック初期化
	io_port_initialize();			//汎用入出力ポートの初期化
	mtu0_initialize();				//MTU0の設定、システムタイマ
	cmt1_initiralize();				//乱数生成用タイマ
	speaker_initialize();			//スピーカ用PWM,DAコンバータ、DA出力用タイマ初期化
	sci0_init(BAUD_RATE);			//シリアル通信モージュールの初期化
	lcd_init();
	autoplay_start(INITIAL_CHECK, SQUARE);
}
