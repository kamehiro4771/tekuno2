/*
 * sci.c
 *
 *  Created on: 2024/02/20
 *      Author: kameyamahiroki
 */
//sci0�`�����l���g�p
//�������������[�h
#include "main.h"

//���[�N�G���A��`
SCI sci0 = {{0},0,{NULL},{0},0,0,ON,ON,OFF,10};
/*
 * void sci0_init(void)
 *�V���A���ʐM�������֐�
 *
 */
void send_data_is_exists_confirm(void);
/****************************************************/
/*										*/
/*void sci0_init(int baudrate)						*/
/****************************************************/
void sci0_init(int baudrate)
{
	MSTP(SCI0)			= 0;	//����d�͒ጸ�@�\����
	SCI0.SCR.BYTE		= 0;
	PORT2.ICR.BIT.B1	= 1;	//P21�[�q�̓��̓o�b�t�@��L���ɂ���
	SCI0.SCR.BIT.CKE	= 0;	//
	SCI0.SMR.BIT.CKS	= 0;	//�N���b�N�̕����Ȃ�
	SCI0.SMR.BIT.PM		= 0;	//�����p���e�B�ő���M
	SCI0.SMR.BIT.PE		= 1;	//�p���e�B�r�b�g��t��
	SCI0.BRR			= (PCLK / (32 * baudrate)) - 1;//�{�[���[�g��115200�ɐݒ�
	SCI0.SCR.BYTE		= 0xc0;	//��M���荞�݁A���M���荞�݋���
	cmt2_wait(600,CKS8);				//100?�ҋ@
	//���荞�ݐݒ�
	IR(SCI0,TXI0)		= 0;
	IPR(SCI0,TXI0)		= 1;
	IEN(SCI0,TXI0)		= 1;
	IR(SCI0,RXI0)		= 0;
	IPR(SCI0,RXI0)		= 1;
	IEN(SCI0,RXI0)		= 1;
	IR(SCI0,TEI0)		= 0;
	IPR(SCI0,TEI0)		= 1;
	IEN(SCI0,TEI0)		= 1;
	IR(SCI0,ERI0)		= 0;
	IPR(SCI0,ERI0)		= 1;
	IEN(SCI0,ERI0)		= 1;
	interval_function_set(1,send_data_is_exists_confirm);
}
/****************************************************/
/*��M�f�[�^�t�����荞�݁iRXI�j							*/
/*void sci0_received(void)							*/
/****************************************************/
void sci0_received(void)
{
	sci0.receive_data[sci0.receive_count]		= SCI0.RDR;
	if(sci0.receive_data[sci0.receive_count] == '\n'){//�G���^�[�������ꂽ��
		sci0.enter_flg							= ENTER_ON;//�G���^�[�t���OON
		SCI0.SCR.BIT.RE							= 0;//��M����֎~
	}
	sci0.receive_count							= (sci0.receive_count + 1) % 128;//�o�b�t�@��0�`127��128�����ȓ��ɂ���
}
/****************************************************/
/*���M�f�[�^�G���v�e�B���荞�݁iTXI�j				*/
/*void sci0_send_next_data(void)					*/
/****************************************************/
void sci0_send_next_data(void)
{
	sci0.send_length[sci0.send_counter]--;
	SCI0.TDR				= *(sci0.send_data[sci0.send_counter]++);
	if(sci0.send_length[sci0.send_counter] == '\0'){//�k�������ɂȂ����瑗�M�I��
		SCI0.SCR.BIT.TIE	= 0;//���M�I�����荞�݋֎~
		SCI0.SCR.BIT.TEIE	= 1;//���M�I�����荞�݋���
	}
}

/****************************************************/
/*���M�I�����荞��									*/
/*void sci0_send_comp(void)							*/
/****************************************************/
void sci0_send_comp(void)
{
	SCI0.SCR.BIT.TEIE		= 0;//���M�I�����荞�݋֎~
	SCI0.SCR.BIT.TE			= 0;//�V���A�����M����֎~
	sci0.send_counter		= (sci0.send_counter + 1) % 64;
	sci0.send_compflg		= ON;//���M�����t���OON
	if(sci0.send_counter == sci0.reg_cnt){
		sci0.send_counter	= 0;
		sci0.reg_cnt		= 0;
		sci0.reg_0_flg		= ON;//���M�o�^���O
	}

}

/****************************************************/
/*���M�J�n											*/
/*void sci0_send_start(void)						*/
/****************************************************/
static void sci0_send_start(void)
{
	SCI0.SCR.BYTE						= 0xc0;
	SCI0.SCR.BYTE						= 0xf0;
	sci0.send_compflg 					= OFF;
}

/****************************************************/
/*��M�J�n											*/
/*void sci0_receive_start(void)						*/
/****************************************************/
void sci0_receive_start(void)
{
	sci0.receive_count		= 0;//��M�J�E���g�̏�����
	sci0.enter_flg			= OFF;//�G���^�[�t���OOFF
	SCI0.SCR.BIT.RE			= 1;//�V���A����M���싖��
}

/****************************************************/
/*���M���I���������m�F����							*/
/*unsigned char sci0_send_comp_check(void)					*/
/****************************************************/
unsigned char sci0_get_reg_0_flg(void)
{
	return sci0.reg_0_flg;
}



/****************************************************/
/*��M�����f�[�^���󂯎�����z��ւ���Ă���		*/
/*unsigned char sci0_data_cpy(unsigned char *data)	*/
/*	�����Fchar *data�@�z��ւ̃|�C���^				*/
/*	�߂�l�Funsigned char �o�C�g��					*/
/****************************************************/
unsigned char sci0_data_cpy(unsigned char *data)
{
	unsigned char i;
	for(i = 0;i < sci0.receive_count - 2;i++){//CR��LF�ȊO���R�s�[
		data[i]	= sci0.receive_data[i];
	}
	return sci0.receive_count;
}
/****************************************************/
/*��M�����f�[�^���󂯎�����z��ւ���Ă����i������ɂ���j			*/
/*unsigned char sci0_data_cpy(unsigned char *data)	*/
/*	�����Fchar *data�@�z��ւ̃|�C���^						*/
/*	�߂�l�Funsigned char �o�C�g��						*/
/****************************************************/
unsigned char sci0_str_cpy(unsigned char *data)
{
	unsigned char i;
	for(i = 0;i < sci0.receive_count - 2;i++){//CR��LF�ȊO���R�s�[
		data[i]	= sci0.receive_data[i];
	}
	data[i] = '\0';
	return sci0.receive_count;
}
/****************************************************/
/*������ւ̃|�C���^��n��							*/
/*unsigned char *sci0_get_receive_data(void)		*/
/****************************************************/
unsigned char *sci0_get_receive_data(void)
{
	return &sci0.receive_data[0];
}
/****************************************************/
/*������̒���										*/
/*unsigned char sci0_get_receive_count(void)		*/
/****************************************************/
unsigned char sci0_get_receive_count(void)
{
	return sci0.receive_count;
}
/****************************************************/
/*��M�G���[���荞��								*/
/* void sci0_receive_error(void)					*/
/****************************************************/
void sci0_receive_error(void)
{
	SCI0.SSR.BIT.PER	= 0;
	SCI0.SSR.BIT.ORER	= 0;
}

/****************************************************/
/*�G���^�[�܂ł̕�����𐔒l�ɕϊ�����							*/
/*signed long a_to_i(void)							*/
/*�@�߂�l�Fsigned long �ϊ���̐��l						*/
/****************************************************/
//�ő�l99999�ŏ��l�O
//�����ȊO�����͂��ꂽ��[�P��Ԃ�
//�ő�l������l�����͂���Ă�����[�P��Ԃ�
signed long a_to_i(void)
{
	unsigned short j = 1;
	unsigned char i;
	signed long result = 0;
	unsigned char num[5];
	if(sci0.receive_count > 7)//5���{CR+LF�����������͂���Ă�����[�P��Ԃ�
		return -1;
	for(i = 0;sci0.receive_data[i] >= '0' && sci0.receive_data[i] <= '9';i++){
		num[i]		= sci0.receive_data[i] - '0';
	}
	if(i != sci0.receive_count - 2 || i == 0)
		return -1;//�r���ŕ��������͂���Ă������A�G���^�[���������ꂽ�Ƃ�
	while(i){
		result 		+= num[i - 1] * j;
		j			*= 10;
		i--;
	}
	return result;
}

/****************************************************/
/*�G���^�[�t���O��Ԃ�								*/
/*unsigned char sci0_enter_check(void)				*/
/****************************************************/
unsigned char sci0_enter_check(void)
{
	return sci0.enter_flg;
}

/****************************************************/
/*
/*
/****************************************************/
unsigned char sci0_find_received_data(unsigned char find_char)
{
	unsigned char i;
	for(i = 0;i < sci0.receive_count - 2;i++){
		if(sci0.receive_data[i] == find_char)
			return i + 1;
	}
	return NOT_FOUND;
}

/****************************************************/
/*���M										*/
/*void send_serial(display *send_data)			*/
/****************************************************/
void send_serial(T_DISPLAY send_data,unsigned short length)
{
	sci0.send_data[sci0.reg_cnt]	= send_data;
	sci0.send_length[sci0.reg_cnt]	= length;
	sci0.reg_cnt					= (sci0.reg_cnt + 1) % 64;
	sci0.reg_0_flg					= OFF;
}

/*******************************************
 * �^�C�}��1ms���Ƃɑ��M�f�[�^�����邩�m�F����
 * ���M�f�[�^���o�^����Ă��đ��M���ł͂Ȃ��ꍇ���M�J�n
 * void send_data_is_exists_confirm(void)
 *******************************************/
void send_data_is_exists_confirm(void)
{
	if(sci0.reg_cnt != 0 && sci0.send_compflg == ON){
		sci0_send_start();
	}
	sci0.elapsed_time	= 1;
}

/*******************************************/
/*��M�J�n���ē��͂���܂őҋ@*/
/**/
/*******************************************/
void sci0_scanf(void)
{
	sci0_receive_start();
	while (sci0.enter_flg == OFF) {

	}
}
