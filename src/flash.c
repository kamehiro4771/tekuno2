/*
 * flash.c
 *
 *  Created on: 2025/01/05
 *      Author: kameyamahiroki
 */
#include "main.h"
//E2�f�[�^�t���b�V���̃u�����N�`�F�b�N
//�u�����N�Ȃ�0���f�[�^��������Ă�����1��Ԃ�
//���b�N�r�b�g���[�h�Q�R�}���h�̓f�[�^�t���b�V���̃u�����N�`�F�b�N�����˂Ă���
//1�T�C�N���ځF�A�h���X�@EA�@�f�[�^�@0x71
//2�T�C�N���ځF�A�h���X�@BA�@�f�[�^�@0xd0
unsigned char blank_check(void)
{
	FLASH.FMODR.BIT.FRDMD		= 1;//FCU���[�h���[�h�����W�X�^���[�h���[�h�ɐݒ�
	FLASH.DFLBCCNT.BIT.BCSIZE	= 1;//�u�����N�`�F�b�N�̃T�C�Y��2�o�C�g�Ɏw��
}

