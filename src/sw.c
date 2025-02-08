/*
 * sw.c
 *
 *  Created on: 2023/10/17
 *      Author: kameyamahiroki
 */
/********************************************/
/*�w�b�_�[�t�@�C��									*/
/********************************************/
#include "main.h"


/*********************************************************/
//�X�C�b�`�̏�Ԃ��m�F����B�`���^�����O�΍�̃��[�v����
/*int sw_check(void)
/*	�߂�l�F�X�C�b�`�ԍ�1�`13����OFF
/*********************************************************/
//�`���^�����O�΍�̂P�O�O�J�E���g�̊ԃX�C�b�`�̏�Ԃ��ς��Ȃ���Ή�����Ă���X�C�b�`�ԍ���Ԃ�
//�X�C�b�`��������Ă����Ԃ��痣���ꂽ��OFF��Ԃ�
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
	if(sw_cnt == 100){//�X�C�b�`�̏����m��A�X�C�b�`�ԍ���Ԃ�
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
