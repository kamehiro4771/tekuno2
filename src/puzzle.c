/********************************************************************************************/
/*�w�b�_�[�t�@�C��																			*/
/********************************************************************************************/
#include "main.h"
#if 1
/********************************************************************************************/
/*�萔��`																					*/
/********************************************************************************************/
const unsigned char KEY_DISPLAY[] = "A	B	C	D	E	F	G	H	I	J	K	L	M\r\n";
/********************************************************************************************/
/*���[�N�G���A��`																			*/
/********************************************************************************************/
unsigned char g_matched_count;//��v�J�E���g
unsigned char battle_field[FIELD_NUM];//�o�g���t�B�[���h�i�����������Ă���j
unsigned char battle_field_display[128];//�V���A���ʐM�ŕ\������z��

/********************************************************************************************/
/*�v���g�^�C�v�錾																			*/
/********************************************************************************************/
void create_send_data(void);


/********************************************************************************************/
/*���͂��ꂽ��΂���͂��ꂽ�ʒu�ɓ�����													*/
/*void move_jewel(unsigned char input1,unsigned char input2)								*/
/*	�����Funsigned char input1�@1�Ԗڂɓ��͂����l											*/
/*		�@unsigned char input2�@2�Ԗڂɓ��͂����l											*/
/********************************************************************************************/
void move_jewel(unsigned char input1,unsigned char input2)
{
	signed char difference;
	unsigned char absolute_value,temp,i;
	if(input1 <= 'Z')
		i					= input1 - 'A';//��������΂̈ʒu
	else
		i					= input1 - 'a';
	difference				= input1 - input2;//��Ԗڂɓ��͂����l�Ɠ�Ԗڂɓ��͂����l�̍������߂�
	if(difference > 0){
		absolute_value		= input1 - input2;
		//��΂����ɂÂ炵�Ă���
		while(absolute_value--){
			temp				= battle_field[i - 1];
			battle_field[i - 1]	= battle_field[i];
			battle_field[i]		= temp;
			i--;
			output_battle_field(UPDATE_FIELD);
			cmt2_wait(18750,CKS512);//200�~���b�Ԋu��������
		}
	}else{
		absolute_value = input2 - input1;
		//��΂��E�ɂÂ炵�Ă���
		while(absolute_value--){
			temp				= battle_field[i + 1];
			battle_field[i + 1]	= battle_field[i];
			battle_field[i]	= temp;
			i++;
			output_battle_field(UPDATE_FIELD);
			cmt2_wait(18750,CKS512);//200�~���b�Ԋu��������
		}
	}
}

/********************************************************************************************/
/*3�ȏ㑵���Ă����΂̐��𐔂���															*/
/*unsigned char* count_jewel(void)															*/
/*	�߂�l�Funsigned char* 3�ȏ��v�����擪�̃A�h���X									*/
/********************************************************************************************/
//�ŏ��ɎO�ȏ��v�����擪�̃A�h���X��Ԃ�
unsigned char* count_jewel(void)
{
	unsigned char i = 0;
	while(i <= FIELD_NUM - 2){
		if(battle_field[i] == battle_field[i + 1])
			g_matched_count++;
		else{
			if(g_matched_count >= 2)
				return &battle_field[i - g_matched_count];
			else
				g_matched_count = 0;
		}
		i++;
	}
	if(g_matched_count >= 2)
		return &battle_field[i - g_matched_count];
	else
		return NULL;
}

/********************************************************************************************/
/*��΂�����																				*/
/*void delete_jewel(unsigned char* first_address)											*/
/*	�����Funsigned char *first_address 3�ȏ��v�����ŏ��̃A�h���X						*
/*	�߂�l�Funsigned char ret ��������΂̐�												*/
/********************************************************************************************/
unsigned char delete_jewel(unsigned char *first_address)
{
	unsigned char i;
	for(i = 0;i <= g_matched_count;i++){
		first_address[i]	= 5;
	}
	output_battle_field(UPDATE_FIELD);
	cmt2_wait(18750,CKS512);//200�~���b�Ԋu��������
	return g_matched_count + 1;
}

/********************************************************************************************/
/*�󂢂���Δz����l�߂�																	*/
/*void free_padding(unsigned char* first_address)											*/
/*	�����Funsigned char *first_address 3�ȏ��v�����ŏ��̃A�h���X						*/
/********************************************************************************************/
//�^�C�}���Ɨ����ɂȂ�Ȃ������H�A�����Ĕ��������Ȃ��Ƃ����Ȃ�����
//battle_field_display��������������̂ł��������Ȃ�H
void free_padding(unsigned char *first_address)
{
	unsigned char i,j,moving_times;
	unsigned char *padding_address;
	for(i = 0;i <= g_matched_count;i++){//��������΂̐��J��Ԃ�
		padding_address				= &first_address[g_matched_count - i];
		moving_times							= &battle_field[FIELD_NUM - 1] - padding_address;//��΂̈ړ��񐔂����߂��ԉE�[�̃A�h���X�Ƃ̍�
		for(j = 0;j <= moving_times;j++){
			padding_address[j]					= padding_address[j + 1];
		}
		battle_field[FIELD_NUM - 1]				= random_number_acquisition(5);//�E�[�ɐV������Δ���������
		output_battle_field(UPDATE_FIELD);//���M�f�[�^�쐬
		cmt2_wait(37500,CKS512);//200�~���b�Ԋu��������
	}
	g_matched_count								= 0;
}

/********************************************************************/
/*�V�����o�g���t�B�[���h�����											*/
/*void make_battle_field(void)										*/
/********************************************************************/
void create_new_battle_field(void)
{
	unsigned char i;
	for(i = 0;i < FIELD_NUM;i++){
		battle_field[i]	= random_number_acquisition(5);//�����_���ɕ�΂𔭐������ăo�g���t�B�[���h�ɓ����
	}
}

/********************************************************************************************/
/*battle_field�ɓ����Ă��鐔�l�����ƂɁA�V���A���ʐM�ő��M����f�[�^���쐬����									*/
/*void create_send_data(void)																*/
/********************************************************************************************/
void create_send_data(unsigned char *battle_field_display)
{
	unsigned char i,j = 0;
	for(i = 0;i < FIELD_NUM;i++){
		battle_field_display[j++]	= '\x1b';
		battle_field_display[j++]	= '[';
		switch(battle_field[i]){
		case 0://�Α���
			battle_field_display[j++]	= '3';//�����F��
			battle_field_display[j++]	= '1';
			battle_field_display[j++]	= 'm';
			battle_field_display[j++]	= '$';
			break;
		case 1://������
			battle_field_display[j++]	= '3';//�����F���F
			battle_field_display[j++]	= '6';
			battle_field_display[j++]	= 'm';
			battle_field_display[j++]	= '~';
			break;
		case 2://������
			battle_field_display[j++]	= '3';//�����F��
			battle_field_display[j++]	= '2';
			battle_field_display[j++]	= 'm';
			battle_field_display[j++]	= '@';
			break;
		case 3://�y����
			battle_field_display[j++]	= '3';//�����F���F
			battle_field_display[j++]	= '3';
			battle_field_display[j++]	= 'm';
			battle_field_display[j++]	= '#';
			break;
		case 4://������
			battle_field_display[j++]	= '3';//�����F��
			battle_field_display[j++]	= '5';
			battle_field_display[j++]	= 'm';
			battle_field_display[j++]	= '&';
			break;
		case 5://�X�y�[�X
			battle_field_display[j++]	= '3';//�F�����ɖ߂�
			battle_field_display[j++]	= '9';
			battle_field_display[j++]	= 'm';
			battle_field_display[j++]	= ' ';
			break;
		}
		battle_field_display[j++]	= '	';
	}
	battle_field_display[j++]	= '\x1b';
	battle_field_display[j++]	= '[';
	battle_field_display[j++]	= '3';//�F�����ɖ߂�
	battle_field_display[j++]	= '9';
	battle_field_display[j++]	= 'm';
}

/*****************************************************************
 *
 *
 *****************************************************************/
void output_battle_field(unsigned char sw)
{
	send_serial(KEY_DISPLAY,strlen((const char*)KEY_DISPLAY));
	if(sw == NEW_FIELD){
		create_new_battle_field();
		create_send_data(battle_field_display);
		send_serial(battle_field_display,101);
	}else if(sw == UPDATE_FIELD){
		create_send_data(battle_field_display);
		send_serial(battle_field_display,101);
		send_serial(CURSOR_1LINE_BUCK,5);//���̏�ŉ�ʂ��X�V���邽�߂ɃJ�[�\������s��̍��[�ɖ߂�
	}else
		send_serial(battle_field_display,101);
}
#endif
