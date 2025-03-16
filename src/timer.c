/*
 * timer.c
 *
 *  Created on: 2024/01/06
 *      Author: kameyamahiroki
 */
#include "iodefine.h"
#include "main.h"
#define MAX_TIMER_NUM (5)
/*
 * ���[�N�G���A��`
 */
unsigned long *g_count_time[MAX_TIMER_NUM];//�J�E���g�G���A�o�^�ϐ�
typedef void (*func_ptr)(void);//�o�^�֐��ւ̃|�C���^
func_ptr func_array[MAX_TIMER_NUM];//�o�^�z��ւ̃|�C���^�z��
unsigned char g_timer_cnt;//�o�^�����^�C�}��
/*
 *
 */
/********************************************************************/
/*�V�X�e���^�C�}�[�ݒ�								 			*/
/*void  mtu0_initialize(void)										*/
/********************************************************************/
void  mtu0_initialize(void)
{
 	SYSTEM.MSTPCRA.BIT.MSTPA9	= 0;

	MTU0.TCR.BIT.TPSC			= 1;//PCLK�̂S����12MHz�ŃJ�E���g
	MTU0.TCR.BIT.CCLR			= 1;//TGRA�̃R���y�A�}�b�`��TCNT�N���A
	MTU0.TIER.BIT.TGIEA			= 1;//TGIA
	MTU0.TGRA					= 12000;//1ms�ŃR���y�A�}�b�`
	//���荞�݃R���g���[���̐ݒ�
	IEN(MTU0,TGIA0)				= 1;
	IPR(MTU0,TGIA0)				= 2;
	IR(MTU0,TGIA0)				= 0;
	MTUA.TSTR.BIT.CST0			= 1;//MTU0.TCNT�̃J�E���g�X�^�[�g
}

/********************************************************************/
/*DA�o�͗p�^�C�}�ݒ�								 	*/
/*void  mtu1_initialize(void)										*/
/********************************************************************/
void mtu1_initialize(void)
{
	SYSTEM.MSTPCRA.BIT.MSTPA9	= 0;//?}???`?t?@???N?V?????p???X???j?b?g?i???j?b?g?O?j?X?g?b?v??????
	//?}???`?t?@???N?V?????^?C?}?p???X???j?b?g?O?`?????l??1????
	MTU1.TCR.BIT.TPSC			= 0;//48MHz
	MTU1.TCR.BIT.CCLR			= 1;//?R???y?A?}?b?`A??N???A
	MTU1.TIER.BIT.TGIEA			= 1;//TGIA?????????
	MTU1.TGRA					= 180;
	//���荞�݃R���g���[���̐ݒ�
	IEN(MTU1,TGIA1)				= 1;
	IPR(MTU1,TGIA1)				= 1;
	IR(MTU1,TGIA1)				= 0;
}

/****************************************************************************/
/*��莞�ԑҋ@																*/
/*void cmt0_wait(unsigned short cnt)										*/
/*	�����Funsigned short cnt 												*/
/*		unsigned char cks �����ݒ�F00:8���� 01:32���� 10:128���� 11:512����*/
/****************************************************************************/
//Excep_UndefinedInst���荞�݂ɓ����ē����Ȃ��Ȃ�
//�R���{�\�������čU�����炷�܂ł͂����A�����X�^�[���͕\������Ȃ�
//��΂������Ȃ����͎~�܂�Ȃ�
void cmt2_wait(unsigned long cnt,unsigned char cks)
{
	MSTP(CMT2)			= 0;//���W���[���X�g�b�v����
	CMT.CMSTR1.BIT.STR2 = 0;//�^�C�}��~
	CMT2.CMCR.BIT.CMIE	= 1;//?R???y?A?}?b?`?????????
	CMT2.CMCR.BIT.CKS	= cks;//�N���b�N�̕����ݒ�
	CMT2.CMCNT			= 0;
	CMT2.CMCOR			= cnt;
	IR(CMT2,CMI2)		= 0;
	IPR(CMT2,CMI2)		= 1;
	CMT.CMSTR1.BIT.STR2 = 1;//�J�E���g�_�E������J�n
	while(IR(CMT2,CMI2) == 0);
	CMT.CMSTR1.BIT.STR2 = 0;//�J�E���g�_�E�������~
	IR(CMT2,CMI2) 		= 0;
	MSTP(CMT2)			= 1;//���W���[���X�g�b�v
}
/********************************************************************/
/*�}���`�t�@���N�V�����^�C�}�̃R���y�A�}�b�`A���荞�݂ŌĂ΂��		*/
/*void count_time(void)												*/
/********************************************************************/
//�֐��̃|�C���^���g����
void count_time(void)
{
	unsigned char i;
	IEN(MTU0,TGIA0)				= 0;//���荞�݋֎~
	for(i = 0;i < g_timer_cnt;i++){
		(*(g_count_time[i]))--;//�J�E���g�G���A���_�E���J�E���g
		if((*(g_count_time[i])) == 0)
			func_array[i]();
	}
	IEN(MTU0,TGIA0)				= 1;//���荞�݋֎~����
}

/********************************************************************/
/*�J�E���g����G���A�ƃR�[���o�b�N�֐���o�^����										*/
/*void count_timer_set(unsigned long *timer)						*/
/*	�����Funsigned long *timer �o�^����G���A						*/
/*		�Fvoid func(unsigned char)  �o�^����R�[���o�b�N�֐��|�C���^*/
/*	�߂�l�FSUCCESS����ERROR										*/
/********************************************************************/
unsigned char count_timer_set(unsigned long *timer,void func(void))
{
	unsigned char ret;
	IEN(MTU0,TGIA0)				= 0;//���荞�݋֎~
	if(g_timer_cnt < MAX_TIMER_NUM){
		g_count_time[g_timer_cnt]  	= timer;
		func_array[g_timer_cnt++]	= func;
		ret							= SUCCESS;
	}else
		ret							= ERROR;
	IEN(MTU0,TGIA0)				= 1;//���荞�݋֎~����
	return ret;
}


/********************************************************************/
/*�����N���֐�����												*/
/*void count_timer_dell(void func(unsigned char))										*/
/********************************************************************/
void count_timer_dell(void func(void))
{
	unsigned char i;
	IEN(MTU0,TGIA0)				= 0;//���荞�݋֎~
	for(i = 0;i < MAX_TIMER_NUM;i++){
		if(func == func_array[i]){
			g_count_time[i]	= NULL;
//			g_count_time[i]		= 0;
			func_array[i]		= NULL;
			g_timer_cnt--;
		}
	}
	IEN(MTU0,TGIA0)				= 1;//���荞�݋֎~����
}
/********************************************************************/
/*���������p�^�C�}CMT1�̐ݒ�										*/
/*void cmt1_initiralize(void)										*/
/********************************************************************/
void cmt1_initiralize(void)
{
	MSTP(CMT1)			= 0;//���W���[���X�g�b�v��ԉ���
	CMT1.CMCR.BIT.CKS	= 1;//
	CMT1.CMCR.BIT.CMIE	= 0;//���荞�݋֎~
	CMT.CMSTR0.BIT.STR1 = 1;//�J�E���g�_�E������J�n
}

/********************************************************************/
/*�����l�擾															*/
/*unsigned short random_number_acquisition(void)					*/
/*	�߂�l�Funsigned char ����											*/
/********************************************************************/
unsigned short random_number_acquisition(void)
{
	return  CMT1.CMCNT;
}
