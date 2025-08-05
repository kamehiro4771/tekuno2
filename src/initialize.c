/*
 * initialize.c
 *
 *  Created on: 2023/10/15
 *      Author: kameyamahiroki
 */
#include "main.h"

/********************************************************************/
/*�V�X�e���̏�����													*/
/*void eneiro_initialize(void)										*/
/********************************************************************/
void eneiro_initialize(void)
{
	clock_initialize();				//�N���b�N������
	io_port_initialize();			//�ėp���o�̓|�[�g�̏�����
	mtu0_initialize();				//MTU0�̐ݒ�A�V�X�e���^�C�}
	cmt1_initiralize();				//���������p�^�C�}
	speaker_initialize();			//�X�s�[�J�pPWM,DA�R���o�[�^�ADA�o�͗p�^�C�}������
	sci0_init(BAUD_RATE);			//�V���A���ʐM���[�W���[���̏�����
	lcd_init();
	autoplay_start(INITIAL_CHECK, SQUARE);
}
