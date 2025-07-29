/*
 * sw.c
 *
 *  Created on: 2023/10/17
 *      Author: kameyamahiroki
 */
/********************************************/
/*ヘッダーファイル									*/
/********************************************/
#include "main.h"

#define MAX_CNT (100)

/*********************************************************/
/*スイッチの状態を確認する。チャタリング対策のループ処理					 */
/*iunsigned char sw_check(void)							　*/
/*	戻り値：スイッチ番号1～13又はOFF
/*********************************************************/
unsigned char sw_check(void)
{
	static unsigned char sw_cnt				= 0;
	static unsigned short last_switch_state	= 0;
	unsigned short switch_state;
	switch_state					= (PORT5.PORT.BYTE << 8) + PORTC.PORT.BYTE;
	if (switch_state == last_switch_state) {
		sw_cnt++;
		if (sw_cnt == MAX_CNT) {//スイッチの情報を確定、スイッチ番号を返す
			sw_cnt = MAX_CNT - 1;
			switch (switch_state) {
			case 0xfffe://sw1
				return SW1;
			case 0xfffd://sw2
				return SW2;
			case 0xfffb://sw3
				return SW3;
			case 0xfff7://sw4
				return SW4;
			case 0xffef://sw5
				return SW5;
			case 0xffdf://sw6
				return SW6;
			case 0xffbf://sw7
				return SW7;
			case 0xff7f://sw8
				return SW8;
			case 0xfeff://sw9
				return SW9;
			case 0xfdff://sw10
				return SW10;
			case 0xfbff://sw11
				return SW11;
			case 0xf7ff://sw12
				return SW12;
			case 0xefff://sw13
				return SW13;
			case 0xffff:
				return OFF;
			}
		}
	}
	else {
		last_switch_state			= switch_state;
		sw_cnt						= 0;
	}
	return OFF;
}
