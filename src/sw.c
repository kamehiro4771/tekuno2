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


/*********************************************************/
//スイッチの状態を確認する。チャタリング対策のループ処理
/*int sw_check(void)
/*	戻り値：スイッチ番号1～13又はOFF
/*********************************************************/
//チャタリング対策の１００カウントの間スイッチの状態が変わらなければ押されているスイッチ番号を返す
//スイッチが押されている状態から離されたらOFFを返す
unsigned char sw_check(void)
{
	static unsigned char sw_cnt				= 0;
	static unsigned short last_switch_state	= 0;
	unsigned short switch_state;
	switch_state					= (PORT5.PORT.BYTE << 8) + PORTC.PORT.BYTE;
	if(switch_state == last_switch_state)
		sw_cnt++;
	else{
		last_switch_state			= switch_state;
		sw_cnt						= 0;
	}
	if(sw_cnt == 100){//スイッチの情報を確定、スイッチ番号を返す
		sw_cnt						= 99;
		switch(switch_state){
		case 0x00fffe://sw1
			return SW1;
		case 0x00fffd://sw2
			return SW2;
		case 0x00fffb://sw3
			return SW3;
		case 0x00fff7://sw4
			return SW4;
		case 0x00ffef://sw5
			return SW5;
		case 0x00ffdf://sw6
			return SW6;
		case 0x00ffbf://sw7
			return SW7;
		case 0x00ff7f://sw8
			return SW8;
		case 0x00feff://sw9
			return SW9;
		case 0x00fdff://sw10
			return SW10;
		case 0x00fbff://sw11
			return SW11;
		case 0x00f7ff://sw12
			return SW12;
		case 0x00efff://sw13
			return SW13;
		case 0xffff:
			sw_cnt					= 0;
		}
	}
	return OFF;
}
