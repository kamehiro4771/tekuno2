/*
 * flash.c
 *
 *  Created on: 2025/01/05
 *      Author: kameyamahiroki
 */
#include "main.h"
unsigned char e2_FLASH;
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

unsigned char blank_check(void)
{
	unsigned short offset = 0;//�A�h���X��2K�o�C�g�ÂI�t�Z�b�g������ϐ�
	FLASH.FMODR.BIT.FRDMD		= 1;//FCU���[�h���[�h�����W�X�^���[�h���[�h�ɐݒ�
	FLASH.DFLBCCNT.BIT.BCSIZE	= 1;//�u�����N�`�F�b�N�̃T�C�Y��2K�o�C�g�Ɏw��
	while(offset < 32000){
		e2_FLASH					= 0x71;//�u�����N�`�F�b�N���T�C�N��
		*(&e2_FLASH + offset)		= 0xd0;//�u�����N�`�F�b�N���T�C�N���u�����N�`�F�b�N�������A�h���X��D0h��������
		while(FLASH.FSTATR0.BIT.FRDY == 0){
			//�u�����N�`�F�b�N���I���܂őҋ@
//			if(){//�^�C���A�E�g����
//				FRASH.FRESETR.BIT.FRESET = 1;
//				while(){//�ҋ@
//					cmt2_wait();
				//}
//				FRASH.FRESETR.BIT.FRESET = 0;
//			}
		}
		if(FLASH.DFLBCSTAT.BIT.BCST == 1){
			//�f�[�^���������܂ꂽ���
		}
		offset						+= 2000;//����2K�o�C�g���u�����N�`�F�b�N����
	}
//	return;
}

