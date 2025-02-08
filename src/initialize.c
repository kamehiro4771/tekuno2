/*
 * initialize.c
 *
 *  Created on: 2023/10/15
 *      Author: kameyamahiroki
 */
#include "main.h"

/********************************************************************/
/*システムの初期化													*/
/*void eneiro_init(void)											*/
/********************************************************************/
void eneiro_init(void)
{
	clock_init();//クロック初期化
	io_port_initialize();
	mtu0_initialize();				//MTU0の設定、システムタイマ
	mtu1_initialize();				//MTU1の設定、DA出力用タイマ
	cmt1_initiralize();				//乱数生成用タイマ
	speaker_initialize();
}
