
/***********************************************************************/
/*                                                                     */
/*  FILE        : Main.c                                  			   */
/*  DATE        :Tue, Oct 31, 2006                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/
//#include "typedefine.h"
#ifdef __cplusplus
//#include <ios>                        // Remove the comment when you use ios
//_SINT ios_base::Init::init_cnt;       // Remove the comment when you use ios
#endif

/********************************************************************/
/*�w�b�_�[�t�@�C��													*/
/********************************************************************/
#include "main.h"

const unsigned char E_NEIRO[]								= {"\x1b[2J\x1b[13A******************** e-NeIRO *********************\n"};
const unsigned char DUTY_VALUE[]							= {"�f���[�e�B�[��F"};
const unsigned char WAVE_TYPE[]								= {"�g�`�F"};
/*���ڂ̎��*/
const unsigned char playlists_select[]						= {"\x1b[2J\x1b[13A�Ȃ�I�����ĉ�����\n"};
const unsigned char wavetype_select[]						= {"���̔g�`��I�����Ă�������\n"};
const unsigned char setting_item_select[] 					= {"�ݒ肷�鍀�ڂ�I�����Ă�������\n"};
const unsigned char SETTING_SPEAKER_SELECT[]				= {"�ݒ肷��X�s�[�J��I�����Ă�������\n"};
const unsigned char OUTPUT_SPEAKER_SELECT[]					= {"�d�q�I���K�����[�h���ɏo�͂���X�s�[�J����I�����Ă�������\n"};
/*���ږ�*/
const unsigned char MODE_NAME[MODE_NUM][64]					= {"�d�q�I���K�����[�h","�������t���[�h","�Q�[�����[�h","�ݒ�",};//���[�h��
const unsigned char TITLE_NAME[SONG_NUM][64]				= {"�A���F�E�}���A","���҂̍s�i","���k�G�b�g","���A�l�̖]�݂̊�т�","�I�[���E���[","������i�Ə�","��M�嗤",
																"Let it Be","NHK�̂ǎ����̃e�[�}��","�h���S���N�G�X�g����","���x���A�b�v","�`���̏�",
																"�h��","�U����","����","�퓬�̃e�[�}","�S��","�C�j�V�����`�F�b�N","�p�b�w���x���̃J�m��",};
const unsigned char WAVE_TYPE_NAME[WAVE_NUM][64] 			= {{"��`�g"},
																{"�̂�����g"},
																{"�O�p�g"},
																{"�T�C���g",}};
const unsigned char SETTING_ITME_NAME[SETTING_ITEM_NUM][64] = {{"�f���[�e�B�[��"},
																{"�g�`"},
																{"�X�s�[�J��"}};
const unsigned char SETTING_SPEAKER_NAME[SPEAKER_NUM][64]	= {{"�X�s�[�J�P"},
																{"�X�s�[�J�Q"},
																{"�X�s�[�J�R"},
																};
const unsigned char OUTPUT_SPEAKER_SELECT_NAME[SPEAKER_NUM][64] = {{"1�F�ЂƂ�"},{"2�F�ӂ���"},{"3�F�݂���"}};

/*�I���o���鍀�ڂ�ς��������͈ȉ��̔z���ύX���āA��������������#define��SELECT_NUM��ς���*/
const unsigned char SELECTABLE_MODE_ARREY[SELECT_MODE_NUM]			= {ORGAN,AUTOPLAY,GAME,SETTING};//�I���ł��郂�[�h
const unsigned char SELECTABLE_TITLE_ARREY[SELECT_PLAY_TITLE_NUM]	= {AVE_MARIA,SAINT_MARCH,JESU_JOY_OF_MAN_S,MENUETT,CANON};//���̔z��ɓ����Ă���Ȃ��������t�����
const unsigned char SELECTABLE_WAVE_ARREY[SELECT_WAVE_NUM]			= {SQUARE,SAWTHOOTH,TRIANGLE,SINE};//�I���ł���g�`
const unsigned char SELECTABLE_SETTING_ARREY[SELECT_SETTING_ITEM_NUM]= {DUTY,WAVE,SPEAKER_NUM};
const unsigned char SELECTABLE_SPEAKER_ARREY[SELECT_SPEAKER_NUM]	= {SPEAKER1,SPEAKER2,SPEAKER3,};
const unsigned char SELECTABLE_OUTPUT_SPEAKER_ARREY[SELECT_OUTPUT_SPEAKER_NUM] = {SPEAKER1,SPEAKER2,SPEAKER3,};
/*�I�����@*/
const unsigned char end_method[]							= {"���j���[�ɖ߂� e + �G���^�[\n"};

/*������@*/
const unsigned char DUTY_SETTING_DISPLAY[] 	= {"�f���[�e�B�����͂��Ă��������i1~99���j\n"
										"SW1:1�@�`�@SW9:9 SW10:0\n"};

const unsigned char setting_comp[]			= {"�ɐݒ肳��܂����B\n"};
const unsigned char error_message[]			= {"���͂�����������܂���\n"};
/*
 * ���[�N�G���A��`
 */
SPEAKER speaker[3]							= {{NULL,NULL,0,0,50,SQUARE,0,1,OFF,OFF},
											{NULL,NULL,0,0,50,SQUARE,0,2,OFF,OFF},
											{NULL,NULL,0,0,50,SQUARE,0,3,OFF,OFF}};
unsigned char electronic_organ_speaker		= 1;
unsigned char g_select;
/*
 * �v���g�^�C�v�錾
 */
void main(void);
void main_sequence_process(void);
signed short item_select_sequence(const unsigned char *item_select,const unsigned char (*item_name)[64],const unsigned char *select_num,unsigned char item_num);
void selected_mode_transition(unsigned char select);
void electronic_organ_mode(void);
void autplay_mode(void);
void setting_mode(void);
void game_mode(void);
void duty_setting(void);
int semibreve_value_setting(int semibreve);
//unsigned short item_select(const unsigned char *item_select,const unsigned char (*item_name)[64],unsigned char item_num);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif
/****************************************************************************/
/*																			*/
/*							���C���֐�										*/
/*																			*/
/****************************************************************************/
void main(void)
{
	eneiro_init();
	sci0_init(BAUD_RATE);			//�V���A���ʐM���[�W���[���̏�����
	automatic_playing(INITIAL_CHECK,SQUARE,0,0,0);
	while(playing_flg == ON){
		//nop
	}
	while(1)
	{
		main_sequence_process();
	}
}

/****************************************************************************/
/*���C���V�[�P���X															*/
/*void main_sequence_process(void)											*/
/****************************************************************************/
void main_sequence_process(void)
{
	static unsigned char main_sequence_num = 0;
	signed char ret;
	switch(main_sequence_num){
		case 0://���[�h�I��
			ret = item_select_sequence(E_NEIRO,MODE_NAME,SELECTABLE_MODE_ARREY,SELECT_MODE_NUM);
			if(ret != -1 && ret != 'e')
				main_sequence_num++;
			break;
		case 1://�e���[�h�Ɉڍs
			selected_mode_transition(ret);
			main_sequence_num = 0;
			break;
	}
}
/************************************************************************************/
/*�I�����ڕ\��																		*/
/*void selection_screen_display(char *item,char *item_name,unsigned char item_num)	*/
/*			const unsigned char *item:�I��������e									*/
/*			const unsigned char *item_name:�I�����鍀�ږ��z��ւ̃|�C���^			*/
/*			const unsigned char *select_num�I���ł��鍀�ڂ̔z��ԍ��̔z��ւ̃|�C���^			*/
/*			char item_num:���ڂ̐�													*/
/************************************************************************************/
static void selection_screen_display(const unsigned char *select_item,const unsigned char (*item_name)[64],const unsigned char *select_num,unsigned char item_num)
{
	unsigned char i;
	unsigned char index_num[8];
	send_serial(RESET,10);//��ʂ����Z�b�g
	send_serial(select_item,strlen((const char*)select_item));
	for(i = 1;i <= item_num;i++){
		sprintf((char *)index_num,"%d.",i);
		send_serial(index_num,2);
		send_serial(item_name[select_num[i - 1] - 1],strlen((const char *)item_name[select_num[i - 1] - 1]));
		send_serial("\n",1);
		while(sci0_get_reg_0_flg() != ON){
			//index_num��������������Ƃ��������Ȃ�̂ő��M��������܂ő҂�
		}
	}
	send_serial(end_method,sizeof(end_method));
}

/*********************************************************************************************************************************/
/*�A�C�e���Z���N�g�V�[�P���X																									 */
/*signed short item_select_sequence(const unsigned char *item_select,const unsigned char (*item_name)[64],unsigned char item_num)*/
/*
/*
/*			const unsigned char *�I���ł��鍀�ڂ̔z��ԍ��̔z��ւ̃|�C���^
/*
/*
/*********************************************************************************************************************************/
signed short item_select_sequence(const unsigned char *item_select,const unsigned char (*item_name)[64],const unsigned char *select_num,unsigned char item_num)
{
	static unsigned char item_select_sequence_num = 0;
	unsigned char ret;
	switch(item_select_sequence_num){
	case 0://��ʕ\��
		sci0_receive_start();//��M�J�n
		selection_screen_display(item_select,item_name,select_num,item_num);//�I����ʂ��\�������
		item_select_sequence_num++;
		break;
	case 1://���͑҂�
		ret = input_check();
		if(ret == ON){
			if(sci0_find_received_data('e'))
				return 'e';
			g_select			= a_to_i();//��M�f�[�^�𐔒l�ɂ��Ď󂯎��
			item_select_sequence_num++;
		}else if(ret != OFF){
			g_select			= ret;
			item_select_sequence_num++;
		}
		break;
	case 2:
		item_select_sequence_num		= 0;
		if(g_select <= item_num)
			return select_num[g_select - 1];
	}
	return -1;
}
/********************************************************************/
/*�I���������[�h�ɑJ�ڂ���											*/
/*void selected_mode_transition(int *select)						*/
/*	
/********************************************************************/
void selected_mode_transition(unsigned char select)
{
	switch(select){
	case ORGAN:
		electronic_organ_mode();
		break;
	case AUTOPLAY:
		autplay_mode();
		break;
	case GAME:
		game_mode();
		break;
	case SETTING:
		setting_mode();
		break;
	default:
		send_serial(error_message,sizeof(error_message));
		break;
	}
}

/********************************************************************/
/*�d�q�I���K�����[�h�֐�											*/
/*unsigned char electronic_organ_mode(void)							*/
/********************************************************************/
//�X�C�b�`�������Ȃ���G���^�[���������ƃX�C�b�`�𗣂������Ƀ��j���[�ɖ߂鉹������ςȂ��ɂȂ�
void electronic_organ_mode(void)
{
	unsigned char ret				= OFF;
	unsigned char output_num		= OFF;
	send_serial(end_method,sizeof(end_method));
	sci0_receive_start();//��M�J�n
	while(1){
		ret							= sci0_enter_check();
		if(ret == ON){
			if(sci0_find_received_data('e') != NOT_FOUND){
				mute(SPEAKER1);
				output_led(OFF);
				break;
			}else
				sci0_receive_start();//��M�J�n
		}
		ret							= sw_check();
		if(output_num != ret){
			output_num				= ret;
			switch(electronic_organ_speaker){
			case 1:
				set_output_value(output_num,SPEAKER1);
				output_speaker_start(1);
				break;
			case 2:
				set_output_value(output_num,SPEAKER1);
				set_output_value(output_num + 12,SPEAKER2);
				output_speaker_start(3);
				break;
			case 3:
				set_output_value(output_num,SPEAKER1 - 1);
				set_output_value(output_num + 12,SPEAKER2);
				set_output_value(output_num + 24,SPEAKER3);
				output_speaker_start(7);
				break;
			}
		}
	}
}

/********************************************************************/
/*�������t���[�h�֐�												*/
/*void autplay_mode(void)											*/
/********************************************************************/
void autplay_mode(void)
{
	unsigned char ret			= 0;
	signed short title			= -1;
	signed short wave_type		= -1;
	while(title == -1){
		title = item_select_sequence(playlists_select,TITLE_NAME,SELECTABLE_TITLE_ARREY,SELECT_PLAY_TITLE_NUM);
	}
	if(title == 'e')
		return;//�^�C�g���I����e�����͂��ꂽ�烁�j���[�֖߂�
	while(wave_type == -1){
		wave_type = item_select_sequence(wavetype_select,WAVE_TYPE_NAME,SELECTABLE_WAVE_ARREY,SELECT_WAVE_NUM);
	}
	if(wave_type == 'e')
		return;//�g�`�I����e�����͂��ꂽ�玩�����t���[�h�I��
	send_serial(TITLE_NAME[title - 1],sizeof(TITLE_NAME[title - 1]));
	send_serial(WAVE_TYPE_NAME[wave_type - 1],sizeof(WAVE_TYPE_NAME[wave_type - 1]));
	automatic_playing((unsigned short)title,wave_type,0,0,0);
	while(playing_flg == ON){
		ret = input_check();
		if(ret != OFF)
			auto_play_end_processing();
	}
}

/********************************************************************/
/*�ݒ胂�[�h�֐�													*/
/*void setting_mode(void)											*/
/********************************************************************/
void setting_mode(void)
{
	signed short setting_num			= -1;
	while(setting_num == -1){
		setting_num = item_select_sequence(setting_item_select,SETTING_ITME_NAME,SELECTABLE_SETTING_ARREY,SETTING_ITEM_NUM);
	}
	switch(setting_num){
	case DUTY:
		�f���[�e�B��̃X�s�[�J���Ƃ̐ݒ肪�ł��Ȃ�
		duty_setting();
		return;
	case WAVE:
		do{
			setting_num = item_select_sequence(wavetype_select,WAVE_TYPE_NAME,SELECTABLE_WAVE_ARREY,SELECT_WAVE_NUM);
		}while(setting_num == -1);
		speaker[0].wave_type= setting_num;
		return;
	case SPEAKER_NUM://�d�q�I���K�����[�h���ɖ炷�X�s�[�J�̔ԍ��w��
		do{
			setting_num = item_select_sequence(OUTPUT_SPEAKER_SELECT,OUTPUT_SPEAKER_SELECT_NAME,SELECTABLE_OUTPUT_SPEAKER_ARREY,SELECT_OUTPUT_SPEAKER_NUM);
		}while(setting_num == -1);
		electronic_organ_speaker = setting_num;
		break;
	case 101:
		break;
	}
}


/*********************************************************************************************************/
/*�f���[�e�B��̐ݒ�																					 */
/*void duty_setting(unsigned char speaker_num)															 */
/*	unsigned char speaker_num �X�s�[�J�ԍ�																 */
/*********************************************************************************************************/
void duty_setting(void)
{
	signed long ret						= OFF;
	signed short speaker_num			= -1;
	unsigned char duty_one_digits		= NO_SELECT;
	unsigned char duty_two_digits		= NO_SELECT;

	while(speaker_num == -1){
		speaker_num = item_select_sequence(SETTING_SPEAKER_SELECT,SETTING_SPEAKER_NAME,SELECTABLE_SPEAKER_ARREY,SELECT_SPEAKER_NUM);
	}
	send_serial(DUTY_SETTING_DISPLAY,sizeof(DUTY_SETTING_DISPLAY));
	while(1){
		ret						= input_check();
		if(ret == ON){
			ret					= a_to_i();
			if(ret > 99 || ret < 1){
				sci0_receive_start();
				ret				= OFF;
			}else
				break;
		}else if(ret != OFF){
			//�X�C�b�`���͂̎�
			if(duty_one_digits == NO_SELECT){
				if(ret < SW11)
					duty_one_digits = ret;
			}else if(ret < SW11){
				duty_two_digits = ret;
				ret  = ((duty_one_digits % 10) * 10) + duty_two_digits % 10;
				if(ret < 1)
					duty_one_digits = duty_two_digits = NO_SELECT;
				else
					break;
			}
		}
	}
	speaker[speaker_num - 1].duty_value		= ret;

}


struct SPEAKER *get_speaker(void)
{
	return speaker;
}


#ifdef __cplusplus
void abort(void)
{

}
#endif
