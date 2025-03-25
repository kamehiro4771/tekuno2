/*
 * flash.c
 *
 *  Created on: 2025/01/05
 *      Author: kameyamahiroki
 */
#include "main.h"

/****************************************
 * ���[�N�G���A��`							*
 ***************************************/
unsigned char e2_FLASH;
unsigned long e2_timeout_check_area;
unsigned short offset 				= 0;//�A�h���X��2K�o�C�g�ÂI�t�Z�b�g������ϐ�
//E2�f�[�^�t���b�V���̃u�����N�`�F�b�N
//�u�����N�Ȃ�0���f�[�^��������Ă�����1��Ԃ�
//���b�N�r�b�g���[�h�Q�R�}���h�̓f�[�^�t���b�V���̃u�����N�`�F�b�N�����˂Ă���
//1�T�C�N���ځF�A�h���X�@EA�@�f�[�^�@0x71
//2�T�C�N���ځF�A�h���X�@BA�@�f�[�^�@0xd0
//FCU�ւ̃R�}���h���s�́A�f�[�^�t���b�V���̈�ɑ΂��郉�C�g�A�N�Z�X�Ŏ�������܂��iP�P�W�S�Q�j
//�f�[�^�t���b�V���A�h���X0x0010 0000~0010 7fff
//FCU�R�}���h�̎g�p���@P1844
/*�u�����N�`�F�b�N�̑��T�C�N���ł́A�V�Ph���f�[�^�t���b�V���̈�̃A�h���X�Ƀo�C�g�������݂��܂��B
 * �R�}���h�̑��T�C�N���ł́A�u�����N�`�F�b�N�Ώۗ̈���܂ޏ����u���b�N���̔C�ӂ̃A�h���X��D0h
 * ���o�C�g�������݂���ƁAFCU���f�[�^�t���b�V���̃u�����N�`�F�b�N�������J�n���܂��B
 * P1847�Ƀt���[�`���[�g
 * 2K�o�C�g�f�[�^�̃u�����N�`�F�b�N�������ݎ���700��s
 * �������݁E�������̃��Z�b�g�p���X��
 * */
//���o�C�g�ۑ����邩���߂�
//�v���[���[��� 		= 72�o�C�g
//�������t��� 		= 19�o�C�g
//�G��� = 74 * 5 	= 370�o�C�g
//�������@= 72 * 4 	= 288�o�C�g
//���v�@				�@�@749�o�C�g
/*
 * �ۑ�����K�v�̂�����
 * 	�v���[���[��
 * 	�v���[���[HP
 * 	�퓬���̓G���
 * 	���t���̋�
 */
//�f�[�^ROM�̃A�h���X�̐擪����u�����N�`�F�b�N���ď������܂�Ă���A�h���X�����߂�
//�������܂�Ă��Ȃ�������BLANK��Ԃ�
unsigned char e2_blank_check(void)
{
	FLASH.FMODR.BIT.FRDMD				= 1;//FCU���[�h���[�h�����W�X�^���[�h���[�h�ɐݒ�
	FLASH.DFLBCCNT.BIT.BCSIZE			= 1;//�u�����N�`�F�b�N�̃T�C�Y��2K�o�C�g�Ɏw��
	while(offset < 32768){//�ő�32K�o�C�g�u�����N�`�F�b�N
		e2_FLASH						= 0x71;//�u�����N�`�F�b�N���T�C�N��
		*(&e2_FLASH + offset)			= 0xd0;//�u�����N�`�F�b�N���T�C�N���u�����N�`�F�b�N�������A�h���X��D0h��������
		e2_timeout_check_area			= 1;//1ms�Ń^�C���A�E�g
		while(FLASH.FSTATR0.BIT.FRDY == 0){
			timer_area_registration(e2_timeout_check_area);
			if(e2_timeout_check_area == 0){//�^�C���A�E�g����
				FLASH.FRESETR.BIT.FRESET = 1;
				cmt2_wait(210,0);//35��s�ҋ@
				FLASH.FRESETR.BIT.FRESET = 0;
			}
		}
		if(FLASH.DFLBCSTAT.BIT.BCST == BLANK){
			if(offset == 0)
				return BLANK;
			else
				return WRITTEN_STATE;
		}
		offset						+= 2048;//����2K�o�C�g���u�����N�`�F�b�N����
	}
	return WRITTEN_STATE;//32K�o�C�g�������܂�Ă�����
}

/***********************************************
 *
 */
unsigned char e2_writing(unsigned short addr)
{
	e2_FLASH				= 0xe8;
	e2_FLASH				= 0x40;
	*(&e2_FLASH + offset)	= ;
}
