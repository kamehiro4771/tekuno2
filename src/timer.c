/*
 * timer.c
 *
 *  Created on: 2024/01/06
 *      Author: kameyamahiroki
 */
#include "iodefine.h"
#include "main.h"
#define MAX_FUNC_NUM (5)
#define MAX_AREA_NUM (5)
/*
 * ���[�N�G���A��`
 */
unsigned long *area_array[MAX_AREA_NUM];//�J�E���g�G���A�o�^�ϐ�
typedef void (*func_ptr)(void);//�o�^�֐��ւ̃|�C���^
unsigned long func_interval_array[MAX_FUNC_NUM];//�֐��Ăяo���̊Ԋu�z��
func_ptr func_array[MAX_FUNC_NUM];//�o�^�z��ւ̃|�C���^�z��
unsigned char function_cnt;//�o�^�����֐��̐�
unsigned char area_cnt;//�o�^�����^�C�}�G���A�̐�
unsigned long timer_cnt_array[MAX_FUNC_NUM];//�^�C�}�[�J�E���g
/*
 *
 */
/********************************************************************/
/*�V�X�e���^�C�}�[�ݒ�								 			*/
/*void  mtu0_initialize(void)										*/
/********************************************************************/
void  mtu0_initialize(void)
{
 	SYSTEM.MSTPCRA.BIT.MSTPA9	= 0;		//�}���`�t�@���N�V�����^�C�}�p���X���j�b�g�i���j�b�g�O�j�̃��W���[���X�g�b�v����
	MTU0.TCR.BIT.TPSC			= 1;		//PCLK�̂S�����ŃJ�E���g
	MTU0.TCR.BIT.CCLR			= 1;		//TGRA�̃R���y�A�}�b�`��TCNT�N���A
	MTU0.TIER.BIT.TGIEA			= 1;		//TGIA���荞�݋���
	MTU0.TGRA					= 12000;	//1ms�ŃR���y�A�}�b�`
	//���荞�݃R���g���[���̐ݒ�
	IEN(MTU0,TGIA0)				= 1;
	IPR(MTU0,TGIA0)				= 1;
	IR(MTU0,TGIA0)				= 0;
	MTUA.TSTR.BIT.CST0			= 1;		//MTU0.TCNT�̃J�E���g�X�^�[�g
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
/*		unsigned char cks �����ݒ�F00:8���� 01:32���� 10:128���� 11:512����		*/
/****************************************************************************/
void cmt2_wait(unsigned long cnt,unsigned char cks)
{
	MSTP(CMT2)			= 0;//���W���[���X�g�b�v����
	CMT.CMSTR1.BIT.STR2 = 0;//�^�C�}��~
	CMT2.CMCR.BIT.CMIE	= 1;//���荞�݋���
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
void count_time(void)
{
	unsigned char i = 0;
	__clrpsw_i();//���荞�݋֎~
	for(i = 0;i < MAX_FUNC_NUM;i++){
		if(func_array[i] == NULL){
			/*nop*/
		}else{
			timer_cnt_array[i]++;
			if(func_interval_array[i] == timer_cnt_array[i]){//�J�E���g���Ăяo���J�E���g�ƈ�v����
				func_array[i]();//�o�^���ꂽ�֐��Ăяo��
				timer_cnt_array[i] = 0;//�^�C�}�[�J�E���g�N���A
			}
		}
	}
	for(i = 0;i < area_cnt;i++){
		if(*(area_array[i]) != 0)
			(*(area_array[i]))--;//�J�E���g�G���A���_�E���J�E���g
	}
	__setpsw_i();//���荞�݋���
}

/********************************************************************************/
/*�^�C�}�J�E���g�ŌĂяo�����֐���o�^����									*/
/*unsigned char interval_function_set(unsigned long interval,void func(void))	*/
/*	�����Funsigned long interval 	�Ăяo������(ms)							*/
/*		�Fvoid func(unsigned char)  �o�^����R�[���o�b�N�֐��|�C���^			*/
/*	�߂�l�FSUCCESS����ERROR													*/
/********************************************************************************/
unsigned char interval_function_set(unsigned long interval,void func(void))
{
	unsigned char ret,i;
	__clrpsw_i();											//���荞�݋֎~
	if(function_cnt < MAX_FUNC_NUM){
		for(i = 0;i < MAX_FUNC_NUM;i++){
			if(func_array[i] == func){
				__setpsw_i();											//���荞�݋���
				return SUCCESS;								//�֐������ɓo�^����Ă���
			}
		}
		func_interval_array[function_cnt]  	= interval;
		func_array[function_cnt++]			= func;
		ret									= SUCCESS;
	}else
		ret									= ERROR;		//�o�^���I�[�o�[
	__setpsw_i();											//���荞�݋���
	return ret;
}

/****************************************************************/
/* �^�C�}�J�E���g�G���A�o�^										*/
/*unsigned char timer_area_registration(unsigned long* area)	*/
/*	�����Funsigned long* area�_�E���J�E���g����G���A			*/
/*	�߂�l�FSUCCESS�o�^�����@ERROR�F���ɓo�^�ς�				*/
/****************************************************************/
unsigned char timer_area_registration(unsigned long* area)
{
	unsigned char ret,i;
	__clrpsw_i();											//���荞�݋֎~
	for(i = 0;i < area_cnt;i++){
		if(area_array[i] == area)							//���ɓo�^�ς݂̃G���A
			break;
	}
	if(i == area_cnt){
		area_array[area_cnt++] = area;
		ret = SUCCESS;
	}else
		ret = ERROR;
	__setpsw_i();//���荞�݋���
	return ret;
}
/********************************************************************/
/*�����N���֐��폜													*/
/*void count_timer_dell(void func(unsigned char))					*/
/*	�����F�폜����֐��̃|�C���^									*/
/********************************************************************/
void count_timer_dell(void func(void))
{
	unsigned char i;
	__clrpsw_i();//���荞�݋֎~
	for(i = 0;i < MAX_FUNC_NUM;i++){
		if(func == func_array[i]){
			function_cnt--;
//			g_count_time[i]		= 0;
			func_array[i]		= NULL;
		}
	}
	__setpsw_i();//���荞�݋���
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
