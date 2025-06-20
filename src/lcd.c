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
#define FUNCTION_SET (0x03c)			//�C���^�[�t�F�[�X�f�[�^��8�r�b�g,�f���[�e�B�[�̐ݒ�1/16�ɐݒ�
#define CLEAR_DISPLAY (0x001)
#define RETURN_HOME (0x002)
#define ENTRY_MODE_RIGHT (0x006)		//�\�����E�ɃV�t�g�ɐݒ肷��B�\���S�̂̓V�t�g���Ȃ�
#define ENTRY_MODE_LEFT (0x004)			//�\�������V�t�g�ɐݒ肷��B�\���S�̂̓V�t�g���Ȃ�
#define DISPLAY_ON (0x000f)				//�f�B�X�v���CON�J�[�\��ON
#define DISPLAY_OFF (0x0008)			//�f�B�X�v���COFF�J�[�\��OFF
#define CURSOL_SHIFT_RIGHT (0x018)		//�J�[�\�����E�Ɉړ�
#define CURSOL_SHIFT_LEFT (0x010)		//�J�[�\�������Ɉړ�
#define SET_DDRAM_ADDRESS (0X080)		//�f�B�X�v���C�̃A�h���X���w�肷��
#define BUSYFLAG_LEADOUT (0x100)		//�������쒆���ǂ����m�F����
#define WRITE_DATA (0x200)				//DDRAM�ɏ�������
#define READ_DATA (0x300)				//DDRAM��ǂ�
#define BUSY_FLAG (PORT4.PORT.BIT.B7)
#define DB (PORT4.DDR.BYTE)
#define RS (PORTA.DDR.BIT.B2)
#define RW (PORTA.DDR.BIT.B1)
#define E (PORTA.DDR.BIT.B3)

 /*********************************************************/
 /*�C���X�g���N�V�����R�[�h�𑗂�	�@�@�@				  */
 /*void instruction_sending(unsigned short instruction) �@*/
 /*�����Funsigned short instruction						  */
 /*********************************************************/
void instruction_sending(unsigned short instruction)
{
	DB = (instruction & 0x00ff);
	RW = (instruction >> 8) & 1
	RS |= (instruction >> 8) & 2;
	E = 1;
	cmt2_wait(2, CKS8);//��300ns�ҋ@
	E = 0;
}

/*********************************************************/
/*LCD�f�B�X�v���C�������֐�								 */
/*void lcd_init(void)									 */
/*********************************************************/
void lcd_init(void)
{
	PORT4.DDR.BYTE	= 0xff;//�|�[�g4���o�̓|�[�g�ɐݒ�
	PORTA.DDR.BYTE	= 0x0d;//PA1,PA2�CPA3���o�̓|�[�g�ɐݒ�
	instruction_sending(FUNCTION_SET);//�t�@���N�V�����Z�b�g1���
	cmt2_wait(60000,CKS32);//40ms�ҋ@
	instruction_sending(FUNCTION_SET);//�t�@���N�V�����Z�b�g2���
	cmt2_wait(150, CKS32);//100��s�ҋ@
	instruction_sending(FUNCTION_SET);//�t�@���N�V�����Z�b�g3���
	while (BUSY_FLAG == 0) {
	}
	instruction_sending(FUNCTION_SET);//�t�@���N�V�����Z�b�g4���
	while (BUSY_FLAG == 0) {
	}
	instruction_sending(DISPLAY_OFF);
	while (BUSY_FLAG == 0) {
	}
	instruction_sending(CLEAR_DISPLAY);
	while (BUSY_FLAG == 0) {
	}
	instruction_sending(ENTRY_MODE_RIGHT);
	while (BUSY_FLAG == 0) {
	}
	instruction_sending(DISPLAY_ON);
	while (BUSY_FLAG == 0) {
	}
}

/*************************************************************/
/*LCD�Ƀf�[�^��\������										 */
/*void lcd_display(unsigned char *data,unsigned short length)*/
/*�����F*/
/**/
/*************************************************************/
void lcd_display(unsigned char *data,unsigned short length)
{
	unsigned short instruction;
	unsigned short i = 0;
	while (length) {
		instruction = WRITE_DATA + data[i++];
		instruction_sending(instruction);
		length--;
	}
}
