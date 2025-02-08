/*
 * initialize.c
 *
 *  Created on: 2023/10/15
 *      Author: kameyamahiroki
 */
#include "main.h"

/********************************************************************/
/*�V�X�e���̏�����													*/
/*void eneiro_init(void)											*/
/********************************************************************/
void eneiro_init(void)
{
	clock_init();//�N���b�N������
	io_port_initialize();
	mtu0_initialize();				//MTU0�̐ݒ�A�V�X�e���^�C�}
	mtu1_initialize();				//MTU1�̐ݒ�ADA�o�͗p�^�C�}
	cmt1_initiralize();				//���������p�^�C�}
	speaker_initialize();
}
