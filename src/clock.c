/*
 * clock.c
 *���C���N���b�N�P�Q���K�w���c
 *  Created on: 2023/10/15
 *      Author: kameyamahiroki
 */
#include "iodefine.h"

void clock_initialize(void)
{
	SYSTEM.SCKCR.LONG			= 0x00000100;//�V�X�e���N���b�N96MHz�A���Ӄ��W���[���N���b�N48MHz�A�O���o�X�N���b�N96MHz�ɐݒ�
	SYSTEM.SUBOSCCR.BIT.SUBSTOP	= 1;//�T�u�N���b�N���U���~�i�T�u�N���b�N�͐ڑ�����Ă��Ȃ��j
}

