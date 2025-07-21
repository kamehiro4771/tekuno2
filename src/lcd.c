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
#define READ_OUT (0X100)				//�C���X�g���N�V�������W�X�^�̒l��ǂ�
#define WRITE_DATA (0x200)				//DDRAM�ɏ�������
#define READ_DATA (0x300)				//DDRAM�̃f�[�^��ǂ�

#define BUSY_FLAG (PORT4.PORT.BIT.B7)
#define DB (PORT4.DR.BYTE)
#define RS (PORTA.DR.BIT.B2)
#define RW (PORTA.DR.BIT.B1)
#define E (PORTA.DR.BIT.B3)
#define FIRST_ROW (0x00)				//1�s�ڂ�1�ԍ��̃A�h���X
#define SECOND_ROW (0x40)				//2�s�ڂ�1�ԍ��̃A�h���X
#define LINE_UP_ADDR ()

#define BS (0x08)
#define TAB (0x09)
#define CR (0x0b)
#define LF (0x0a)

 /****************************************************************************/
 /* ���[�N�G���A��`															*/
 /****************************************************************************/
unsigned char display_data[16][16];//�\���f�[�^(1�s16�����~16�s��)
unsigned char digit;//���ݕ\�����Ă��錅�ʒu
unsigned char line;//���ݕ\�����Ă���s�ԍ�
unsigned char esc_flag = OFF;
unsigned char address_counter;



/*********************************************************/
/*�r�W�[�t���O�ǂݏo��									 */
/*unsigned char read_busy_flag(void)					 */
 /********************************************************/
unsigned char read_bf_ac(void)
{
	unsigned char busy;
    RS				= 0;		//�R�}���h���[�h
    RW				= 1;		//�ǂݎ�胂�[�h
    PORT4.DDR.BYTE	= 0;		//DB����͂�
    __nop();					//�^�C�~���O����
    E				= 1;
    __nop();					//LCD���f�[�^���o�͂���܂ő҂�
    busy			= BUSY_FLAG;//BUSY�t���O�ǂݎ��
	address_counter = DB & 0x7f;
    E				= 0;
	PORT4.DDR.BYTE	= 0xff;		//�|�[�g4���o�̓|�[�g�ɐݒ�
    return busy;
}


 /*********************************************************/
 /*�C���X�g���N�V�����R�[�h�𑗂�	�@�@�@				  */
 /*void instruction_set(unsigned short instruction) �@*/
 /*�����Funsigned short instruction						  */
 /*********************************************************/
void instruction_set(unsigned short instruction)
{
	unsigned char i;
	RS = (instruction & 0x200) ? 1 : 0;
	RW = (instruction & 0x100) ? 1 : 0;
	for (i = 0; i < 10; i++)
		__nop();
	DB = instruction & 0x00ff;
	for(i = 0;i < 10;i++)
		__nop();
	E = 1;
	for(i = 0;i < 10;i++)
		__nop();
	E = 0;
}

/******************************************************************/
/*�r�W�[�t���O��ǂ�ŁA�C���X�g���N�V�����𑗂�				  */
/*void lcd_send_instruction(unsigned short instruction)			  */
/*	�����Funsigned short instruction�@�Z�b�g����C���X�g���N�V����*/
/******************************************************************/
void lcd_send_instruction(unsigned short instruction)
{
	while (read_bf_ac() == 1){
		__nop(); 							// �y���E�F�C�g�Ń|�[�����O�����艻
	}										//�r�W�[�t���O�������Ă���ԑҋ@
	instruction_set(instruction);
	return;
}

/*************************************************************/
/*���s����													 */
/*void lcd_line_feed(unsigned char up_or_down)*/
/*************************************************************/
//��ɉ��s���邩���ɉ��s����
//�X�N���[������ꍇ�́A���݂̕\���f�[�^���X�V����
//display_data�����ăJ�[�\���̈ʒu���ړ�������
//digit�ϐ��̕ύX
//line�ϐ��̕ύX
void lcd_line_feed(unsigned char ch)
{
	digit = 15;
	lcd_send_instruction(READ_OUT);//���݂̃A�h���X���擾
	if (up_or_down == LINE_UP) {
		line--;

	}
	else {
		line++;
	}
	while (display_data[line][digit] == ' ') {
		digit--;
	}
}

/***************************************************************/
/**/
/*void lcd_buck_space(void)									   */
/***************************************************************/
void lcd_buck_space(void)
{
	if(address_counter == )

	else if(address_counter == )

	else {
		lcd_send_instruction(CURSOL_SHIFT_LEFT);
		lcd_send_instruction(WRITE_DATA | ' ');
		lcd_send_instruction(CURSOL_SHIFT_LEFT);
	}

}

/***************************************************************/
/*�ꕶ�������o�͂���										   */
/*void lcd_putchar(unsigned char ch)						   */
/*�����Funsigned char ch �o�͂��镶��						   */
/***************************************************************/
void lcd_putchar(unsigned char ch)
{
	switch (ch) {
	case BS:
		lcd_buck_space();
		break;
	case TAB:

		break;
	case CR:
		if (address_counter <= 0x0f)
			lcd_send_instruction(SET_DDRAM_ADDRESS);
		else
			lcd_send_instruction(SET_DDRAM_ADDRESS | 0x40);
		break;
	case LF:
		lcd_line_feed(ch);
		break;
	default:
		lcd_send_instruction(WRITE_DATA | ch);
		if (address_counter == 0x0f || address_counter == 0x4f)
			lcd_line_feed(ch);
		break;
	}
}
/****************************************************************/
/*LCD�ɕ�������o�͂���											*/
/*void lcd_print(const unsigned char* str,unsigned short length)*/
/**/
/**/
/***************************************************************/
void lcd_print(const unsigned char* str,unsigned short length)
{
	unsigned short i;
	lcd_send_instruction(CLEAR_DISPLAY);
	lcd_send_instruction(RETURN_HOME);
	do{
		lcd_putchar(str[i]);
	} while (address_counter != 0x00);
}

void lcd_news_ticker(void)
{

}
/*********************************************************/
/*LCD�f�B�X�v���C�������֐�								 */
/*void lcd_init(void)									 */
/*********************************************************/
void lcd_init(void)
{
	PORT4.DDR.BYTE	= 0xff;				//�|�[�g4���o�̓|�[�g�ɐݒ�
	PORTA.DDR.BYTE	|= 0x0e;				//PA1,PA2�CPA3���o�̓|�[�g�ɐݒ�
	cmt2_wait(22500,CKS32);	//15ms�ҋ@
	instruction_set(FUNCTION_SET);		//�t�@���N�V�����Z�b�g1���
	cmt2_wait(60000,CKS32);				//40ms�ҋ@
	instruction_set(FUNCTION_SET);		//�t�@���N�V�����Z�b�g2���
	cmt2_wait(150, CKS32);				//100��s�ҋ@
	lcd_send_instruction(FUNCTION_SET);			//�t�@���N�V�����Z�b�g3���
	lcd_send_instruction(FUNCTION_SET);			//�t�@���N�V�����Z�b�g4���
	lcd_send_instruction(DISPLAY_OFF);
	lcd_send_instruction(CLEAR_DISPLAY);
	lcd_send_instruction(ENTRY_MODE_RIGHT);
	lcd_send_instruction(DISPLAY_ON);
}



void lcd_page_ud()
{

}
/*************************************************************/
/*LCD�Ƀf�[�^��\������										 */
/*void lcd_display(unsigned char *data,unsigned short length)*/
/*�����F*/
/**/
/*************************************************************/
//���s�������
/*	�G���^�[�L�[����
*	���L�[����
*	���L�[����
*	��ԍ��Ł��L�[����
*	��ԉE�Ł��L�[����
*	��ԉE�ŕ�������
*	��ԍ��Ńo�b�N�X�y�[�X�L�[����
* 
*/
/*
void lcd_display(unsigned char *data,unsigned short length)
{
	unsigned short instruction;
	unsigned short i = 0;
	unsigned char cursol = lcd_send_instruction(READ_OUT);//���݂̃J�[�\���ʒu���擾
	while (length) {
		switch (data[i]) {//���s�̂���Ȃ��𔻒�
		case 0x0d://CR(�s�����A)
			if (cursol == <= 0x0f)
				lcd_send_instruction(SET_DDRAM_ADDRESS);
			else
				lcd_send_instruction(SET_DDRAM_ADDRESS | 0x40);
			break;
		case 0x0a://LF(���s)
			if(line % 2) == 0)
				lcd_line_feed(LINE_DOWN);
			else
				lcd_line_feed(LINE_UP);
			break;
		case 0x08://�o�b�N�X�y�[�X
			if (digit == 0)
				lcd_line_feed(LINE_UP);
			else
				digit--;
			lcd_send_instruction(CURSOL_SHIFT_LEFT);
			break;
		case 0x1b://�G�X�P�[�v�V�[�P���X�J�n

			break;
		case '':
			break;
		case '':
			break;
		case '':
			break;
		default:
			instruction = WRITE_DATA + data[i++];
			lcd_send_instruction(instruction);
			digit++;
			if (digit == 15)
				lcd_line_feed();
			break;
		}
		length--;
	}
}
*/
