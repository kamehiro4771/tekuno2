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
void forward_score(void);
void output_function_call(void);
void end_flg_check(void);
/*
 * ���[�N�G���A��`
 */
AUTOPLAYER autoplayer[SPEAKER_NUM] = {{SQUARE,NULL,NULL,0,0,0,OFF,ON},{SQUARE,NULL,NULL,0,0,0,OFF,ON},{SQUARE,NULL,NULL,0,0,0,OFF,ON},};
AUTOPLAYER interrupt_data[3];//�G���^�[��X�C�b�`�ŉ��t�I�������Ƃ��̃X�s�[�J�̏���ۑ����Ă���
unsigned char g_use_speaker_num;		//�g�p����X�s�[�J�[�̌�
unsigned char playing_flg = OFF;

/********************************************************************/
/*�������t����@�\���V�X�e���^�C�}�ɓo�^����						*/
/*void autoplay_function_set(void)									*/
/********************************************************************/
void autoplay_function_set(void)
{
	interval_function_set(1, output_function_call);//�o�͊֐��̓o�^
	interval_function_set(1, forward_score);//�o�ߎ��ԊĎ��̃G���A��o�^
	interval_function_set(1, end_flg_check);
}

/********************************************************************/
/*�������t���J�n����ʒu�A�g�`��
/*
/********************************************************************/
void autoplayer_set(unsigned char wave_type,AUTOPLAYER set1,AUTOPLAYER set2,AUTOPLAYER set3)
{
	autoplayer[0] 				= set1;
	autoplayer[1] 				= set2;
	autoplayer[2] 				= set3;
	autoplayer[0].wave_type 	= wave_type;
	switch (g_use_speaker_num) {
	case 1:
		autoplayer[0].set_flg 	= ON;
		autoplayer[0].end_flg 	= OFF;
		break;
	case 2:
		autoplayer[0].set_flg 	= autoplayer[1].set_flg = ON;
		autoplayer[0].end_flg 	= autoplayer[1].end_flg	= OFF;
		break;
	case 3:
		autoplayer[0].set_flg 	= autoplayer[1].set_flg = autoplayer[2].set_flg = ON;
		autoplayer[0].end_flg 	= autoplayer[1].end_flg = autoplayer[2].end_flg	= OFF;
		break;
	}
}

/****************************************************************/
/*���t����Ȃ̊y���Z�b�g����											*/
/*void score_set_speaker(int title)								*/
/*	�����Fint title �^�C�g���ԍ�										*/
/****************************************************************/
void score_set(int title)
{
	autoplayer[0].pscore 		= SCORE_POINTER_ARRAY[title - 1][0];
	autoplayer[1].pscore 		= SCORE_POINTER_ARRAY[title - 1][1];
	autoplayer[2].pscore 		= SCORE_POINTER_ARRAY[title - 1][2];
	autoplayer[0].pnote_value 	= NOTE_POINTER_ARRAY[title - 1][0];
	autoplayer[1].pnote_value 	= NOTE_POINTER_ARRAY[title - 1][1];
	autoplayer[2].pnote_value 	= NOTE_POINTER_ARRAY[title - 1][2];
	autoplayer[0].note_size 	= NOTE_SIZE_ARRAY[title - 1][0];
	autoplayer[1].note_size 	= NOTE_SIZE_ARRAY[title - 1][1];
	autoplayer[2].note_size 	= NOTE_SIZE_ARRAY[title - 1][2];
	g_use_speaker_num 			= USE_SPEAKER_ARRAY[title - 1];
}

/********************************************************************/
/*�C�ӂ̏ꏊ���牉�t���J�n�������Ƃ�
/*
/**/
void interrupt_data_set(AUTOPLAYER set1,AUTOPLAYER set2,AUTOPLAYER set3)
{

}
/********************************************************************/
/*�������t�I������													*/
/*void auto_play_end_processing(void)								*/
/********************************************************************/
void auto_play_end_processing(void)
{
	if (playing_flg == OFF)						//���t���o�Ȃ����ɌĂяo���ꂽ�牽�����Ȃ�
		return;
	led_lights_out();							//LED����
	mute(ALL_SPEAKER);							//�X�s�[�J�[i����
	count_timer_dell(forward_score);
	count_timer_dell(output_function_call);
	count_timer_dell(end_flg_check);
	playing_flg = OFF;							//���t���t���OOFF
}
/******************************************************************************************************************************************************/
/* �������t�J�n�֐�
/*unsigned char automatic_playing_start(unsigned short title,unsigned char wave_type)*/
/*	���� unsigned short title*/
/*		 unsigned char wave_type*/
/******************************************************************************************************************************************************/
void autoplay_start(unsigned short title,unsigned char wave_type)
{
	auto_play_end_processing();
	playing_flg						= ON;	//���t���t���OON
	score_set(title);						//�y�����Z�b�g����
	autoplayer_set(wave_type,,,);
	autoplay_function_set();				//�������t�ɕK�v�Ȋ֐����V�X�e���^�C�}�ɓo�^����
}

/*********************************************/
/*���f�������t���ĊJ								 */
/*void autoplay_start_from_intermediate(void)*/
/*********************************************/
void autoplay_start_from_intermediate(void)
{
	playing_flg						= ON;	//���t���t���OON
	//���f�f�[�^���Z�b�g����
	output_speaker_start(7);				//�o�͊J�n
	autoplay_function_set();
}

/********************************************************************/
/*���Ԍo�߂Ŋy����i�߂�R�[���o�b�N�֐�										*/
/*void forward_score(unsigned char i)								*/
/*	�����Funsigned char i�@�X�s�[�J�[�ԍ�									*/
/********************************************************************/
//�o�ߎ��Ԃ��_�E���J�E���g
//�o�ߎ��Ԃ����ĂO�Ȃ�X�R�A�J�E���g��i�߂�
//�X�R�A�J�E���g���y���z��̃T�C�Y�Ɠ����ɂȂ�����I���t���O��ON�ɂ���
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
		output_speaker_start(output_pattern);
	}
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
}



AUTOPLAYER *get_autoplayer(unsigned char player_num)
{
	return &autoplayer[player_num];
}


AUTOPLAYER get_interrupt_data(unsigned char player_num)
{
	return interrupt_data[player_num];
}

