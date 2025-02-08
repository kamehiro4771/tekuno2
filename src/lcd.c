/*
 * lcd.c
 *
 *  Created on: 2024/03/27
 *      Author: kameyamahiroki
 */
/*
 * �w�b�_�[�t�@�C��
 */
#include "iodefine.h"
#include "main.h"
/*
 * �}�N����`
 */
#define FUNCTION_SET (0x02c)//�C���^�[�t�F�[�X�f�[�^��8�r�b�g�f���[�e�B�[�̐ݒ�1/16�ɐݒ�
#define CLEAR_DISPLAY (0x001)
#define RETURN_HOME (0x002)
#define ENTRY_MODE_RIGHT (0x006)//�\�����E�ɃV�t�g�ɐݒ肷��B�\���S�̂̓V�t�g���Ȃ�
#define ENTRY_MODE_LEFT (0x004)//�\�������V�t�g�ɐݒ肷��B�\���S�̂̓V�t�g���Ȃ�
#define DISPLAY_ON (0x000f)//�f�B�X�v���CON�J�[�\��ON
#define DISPLAY_OFF (0x0008)//�f�B�X�v���COFF�J�[�\��OFF
#define CURSOL_SHIFT_RIGHT (0x018)//�J�[�\�����E�Ɉړ�
#define CURSOL_SHIFT_LEFT (0x010)//�J�[�\�������Ɉړ�
#define SET_DDRAM_ADDRESS (0X080)//�f�B�X�v���C�̃A�h���X���w�肷��
#define BUSYFLAG_LEADOUT (0x100)//�������쒆���ǂ����m�F����
#define WRITE_DATA (0x200)//DDRAM�ɏ�������
#define READ_DATA (0x300)//DDRAM��ǂ�

void lcd_init(void)
{
	PORT4.DDR.BYTE	= 0xff;//�|�[�g4���o�̓|�[�g�ɐݒ�
	PORTA.DDR.BYTE	= 0x0d;//PA1,PA2�CPA3���o�̓|�[�g�ɐݒ�
	cmt2_wait(60000,CKS32);//40ms�ҋ@
	PORT4.DR.BYTE	= FUNCTION_SET;
	cmt2_wait(56,CKS32);//37?�ҋ@
	PORT4.DR.BYTE	= DISPLAY_ON;
	cmt2_wait(56,CKS32);//37?�ҋ@
	PORT4.DR.BYTE	= CLEAR_DISPLAY;
	cmt2_wait(2280,CKS32);//1.52ms�ҋ@
	PORT4.DR.BYTE	= ENTRY_MODE_RIGHT;
}

