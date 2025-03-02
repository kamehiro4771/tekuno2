/*
 * auto_play.c
/*
 *
 *  Created on: 2023/12/30
 *      Author: kameyamahiroki
 */
/********************************************************************/
/*�w�b�_�[�t�@�C��													*/
/********************************************************************/
#include "main.h"



/********************************************************************/
/*�v���g�^�C�v�錾													*/
/********************************************************************/
void auto_play_end_processing(void);
void score_set_speaker(int title,unsigned char wave_type,unsigned short start1,unsigned short start2,unsigned short start3);
void forward_score(void);
void output_function_call(void);

/*
 * ���[�N�G���A��`
 */
AUTOPLAYER autoplayer[SPEAKER_NUM] = {{SQUARE,NULL,NULL,0,0,0,OFF,OFF},{SQUARE,NULL,NULL,0,0,0,OFF,OFF},{SQUARE,NULL,NULL,0,0,0,OFF,OFF},};
AUTOPLAYER interrupt_data[3];//�G���^�[��X�C�b�`�ŉ��t�I�������Ƃ��̃X�s�[�J�̏���ۑ����Ă���
unsigned char g_use_speaker_num;		//�g�p����X�s�[�J�[�̌�
unsigned long common_timer = 1;
unsigned char playing_flg = OFF;


void automatic_playing(unsigned short title,unsigned char wave_type,unsigned short start1,unsigned short start2,unsigned short start3)
{
	sci0_receive_start();//��M�J�n
	playing_flg						= ON;//���t���t���OON
	score_set_speaker(title,wave_type,start1,start2,start3);//�y�����Z�b�g����A�Ȃ̊J�n�ʒu�̃Z�b�g
}

/********************************************************************/
/*���Ԍo�߂Ŋy����i�߂�R�[���o�b�N�֐�										*/
/*void forward_score(unsigned char i)								*/
/*	�����Funsigned char i�@�X�s�[�J�[�ԍ�									*/
/********************************************************************/
//�o�ߎ��Ԃ��_�E���J�E���g
//�o�ߎ��Ԃ����ĂO�Ȃ�X�R�A�J�E���g��i�߂�
//�X�R�A�J�E���g���y���z��̃T�C�Y�Ɠ����ɂȂ�����I���t���O��ON�ɂ��ăX�s�[�J����������
//�X�R�A�J�E���g���y���z��̃T�C�Y��菬�����ꍇ�Z�b�g�t���O��ON�ɂ���
void forward_score(void)
{
	unsigned char i;
	for(i = 0;i < g_use_speaker_num;i++){
		autoplayer[i].elapsed_time--;
		if(autoplayer[i].elapsed_time == 0){
			autoplayer[i].score_count++;//�����ԍ���i�߂�
			if(autoplayer[i].score_count >= autoplayer[i].note_size){//�Ō�܂ŉ��t������
				autoplayer[i].end_flg	= ON;
			}
			else
				autoplayer[i].set_flg	= ON;
		}
	}
	common_timer						= 1;
}

/********************************************************************************************/
/*LED�ƃX�s�[�J�o��																			*/
/*void output_function_call(void)															*/
/********************************************************************************************/
void output_function_call(void)
{
	unsigned char i;
	unsigned char output_pattern = 0;
	for(i = 0;i < g_use_speaker_num;i++){
		if(autoplayer[i].set_flg == ON){//�X�R�A�J�E���g��������������
			set_output_value(autoplayer[i].pscore[autoplayer[i].score_count],i+1);
			autoplayer[i].set_flg	= OFF;
			output_pattern				+= SET << i;
		}
	}
	if(output_pattern != 0){
		set_output_speaker_length(output_pattern);
		output_speaker_start(output_pattern);//�o�͊J�n
	}
	common_timer = 1;
}

/*
 * ���t�I������
 *void end_decision(void)
 */
//�S�ẴG���h�t���O������ON�Ȃ玩�����t�I���֐����Ăяo��
void end_flg_check(void)
{
	if(autoplayer[0].end_flg ==  ON && autoplayer[1].end_flg == ON && autoplayer[2].end_flg == ON){//���t���Ō�܂ŏI�����������
		auto_play_end_processing();//�^�C�}�[�X�g�b�v�A�ϐ�������
	}
	common_timer = 1;
}

/*
 * �G���^�[�L�[�ƃX�C�b�`���͂̔���
 *
 */
//�������t�ȊO�ɂ��g����ėp�I�ȃX�C�b�`�A�G���^�[�|�[�����O�֐��ɂ���
unsigned char input_check(void)
{
	unsigned char ret						= OFF;
	static unsigned char sw_state			= OFF;
	static unsigned char last_sw_state		= OFF;
	sw_state				= sw_check();
	if(sw_state != OFF){//�X�C�b�`��������Ă�����
		last_sw_state		= sw_state;//�X�C�b�`�̏�ԋL�^
	}else if(sci0_enter_check() == ON){//�X�C�b�`��������Ă��鎞�̓G���^�[�͌��Ȃ�
		ret				= ON;
	}else if(last_sw_state != OFF){//�X�C�b�`�������ꂽ
		ret				= last_sw_state;
		last_sw_state	= OFF;
	}
	return ret;
}

/********************************************************************/
/*�������t�I������													*/
/*void auto_play_end_processing(void)								*/
/********************************************************************/
void auto_play_end_processing(void)
{
	g_use_speaker_num	= 0;
	output_led(REST);				//LED����
	mute(ALL_SPEAKER);//�X�s�[�J�[i����
	interrupt_data[0]			= autoplayer[0];
	interrupt_data[1]			= autoplayer[1];
	interrupt_data[2]			= autoplayer[2];
	count_timer_dell(forward_score);
	count_timer_dell(output_function_call);
	count_timer_dell(end_flg_check);
	count_timer_dell(input_check);
	autoplayer[0].elapsed_time	= 0;
	autoplayer[1].elapsed_time	= 0;
	autoplayer[2].elapsed_time	= 0;
	autoplayer[0].end_flg		= OFF;
	autoplayer[1].end_flg		= OFF;
	autoplayer[2].end_flg		= OFF;
	playing_flg					= OFF;//���t���t���OOFF
}

/********************************************************************************************/
/*�y��,���t�J�n�ʒu,�g�`���X�s�[�J�[�ɃZ�b�g����											  				*/
/*void void score_set_speaker(int title,unsigned short start_point,unsigned char )	*/
/*	�����Fint title �^�C�g���ԍ�													  				*/
/*		unsigned short start_point ���t�J�n����Y�����ԍ�							  				*/
/*		unsigned char  �o�͔g�`										  				*/
/********************************************************************************************/
void score_set_speaker(int title,unsigned char wave_type,unsigned short start1,unsigned short start2,unsigned short start3)
{
	unsigned char i;
	autoplayer[0].score_count 		= start1;
	autoplayer[1].score_count 		= start2;
	autoplayer[2].score_count 		= start3;//���t�J�n�ʒu�̐ݒ�
	autoplayer[0].wave_type			= wave_type;
	autoplayer[0].pscore			= SCORE_POINTER_ARRAY[title-1][0];
	autoplayer[1].pscore			= SCORE_POINTER_ARRAY[title-1][1];
	autoplayer[2].pscore			= SCORE_POINTER_ARRAY[title-1][2];
	autoplayer[0].pnote_value		= NOTE_POINTER_ARRAY[title-1][0];
	autoplayer[1].pnote_value		= NOTE_POINTER_ARRAY[title-1][1];
	autoplayer[2].pnote_value		= NOTE_POINTER_ARRAY[title-1][2];
	autoplayer[0].note_size			= NOTE_SIZE_ARRAY[title-1][0];
	autoplayer[1].note_size			= NOTE_SIZE_ARRAY[title-1][1];
	autoplayer[2].note_size			= NOTE_SIZE_ARRAY[title-1][2];
	g_use_speaker_num				= USE_SPEAKER_ARRAY[title-1];
	if(start1 == 0 && start2 == 0 && start3 == 0){
		switch(g_use_speaker_num){
		case 1:
			autoplayer[0].set_flg	= ON;
			autoplayer[1].end_flg	= autoplayer[2].end_flg = ON;
			break;
		case 2:
			autoplayer[0].set_flg 	= autoplayer[1].set_flg = ON;
			autoplayer[2].end_flg	= ON;
			break;
		case 3:
			autoplayer[0].set_flg 	= autoplayer[1].set_flg = autoplayer[2].set_flg		= ON;
			break;
		}
	}else{//�r�����牉�t���鎞�͌o�ߎ��Ԃ̍X�V�����ꂽ���Ȃ�����set_flg��OFF�ŃX�^�[�g����
		for(i = 0;i < g_use_speaker_num;i++){
			set_output_value(autoplayer[i].score_count,i);
		}
		output_speaker_start(3);//�o�͊J�n

	}
	count_timer_set(&common_timer,output_function_call);//�o�͊֐��̓o�^
	count_timer_set(&common_timer,forward_score);//�o�ߎ��ԊĎ��̃G���A��o�^
	count_timer_set(&common_timer,end_flg_check);
}

AUTOPLAYER *get_autoplayer(void)
{
	return &autoplayer;
}
/*
 * �������t�𒆒f�����Ƃ��̃f�[�^�����炤
 * struct SPEAKER get_interrupt_data(speaker_num)
 *
 *
 */
AUTOPLAYER get_interrupt_data(unsigned char player_num)
{
	return interrupt_data[player_num];
}


