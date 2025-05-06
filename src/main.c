
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

/********************************************************************/
/*�萔��`															*/
/********************************************************************/
const T_DISPLAY E_NEIRO[]														= {"\x1b[2J\x1b[13A******************** e-NeIRO *********************\n"};
const T_DISPLAY DUTY_VALUE[]													= {"�f���[�e�B�[��F"};
const T_DISPLAY WAVE_TYPE[]														= {"�g�`�F"};
/*���ڂ̎��*/
const T_DISPLAY PLAYLIST_SELECT[]												= {"\x1b[2J\x1b[13A�Ȃ�I�����ĉ�����\n"};
const T_DISPLAY WAVETYPE_SELECT[]												= {"���̔g�`��I�����Ă�������\n"};
const T_DISPLAY SETTING_ITEM_SELECT[] 											= {"�ݒ肷�鍀�ڂ�I�����Ă�������\n"};
const T_DISPLAY SETTING_SPEAKER_SELECT[]										= {"�ݒ肷��X�s�[�J��I�����Ă�������\n"};
const T_DISPLAY OUTPUT_SPEAKER_SELECT[]											= {"�d�q�I���K�����[�h���ɏo�͂���X�s�[�J����I�����Ă�������\n"};

/*���ږ�*/
const T_DISPLAY MODE_NAME[MODE_NUM][64]											= {"�d�q�I���K�����[�h","�������t���[�h","�Q�[�����[�h","�ݒ�",};//���[�h��
const T_DISPLAY TITLE_NAME[SONG_NUM][64]										= {"�A���F�E�}���A","���҂̍s�i","���k�G�b�g","���A�l�̖]�݂̊�т�","�I�[���E���[","������i�Ə�","��M�嗤",
																	   	   	   	   "Let it Be","NHK�̂ǎ����̃e�[�}��","�h���S���N�G�X�g����","���x���A�b�v","����",
																	   	   	   	   "�h��","�U����","�`���̏�","�퓬�̃e�[�}","�S��","�C�j�V�����`�F�b�N","�p�b�w���x���̃J�m��",};
const T_DISPLAY WAVE_TYPE_NAME[WAVE_NUM][64] 									= {{"��`�g"},
																	   	   	   	   {"�̂�����g"},
																	   	   	   	   {"�O�p�g"},
																	   	   	   	   {"�T�C���g",}};
const T_DISPLAY SETTING_ITME_NAME[SETTING_ITEM_NUM][64] 						= {{"�f���[�e�B�[��"},
																	   	   	   	   {"�g�`"},
																				   {"�X�s�[�J��"}};
const T_DISPLAY SETTING_SPEAKER_NAME[SPEAKER_NUM][64]							= {{"�X�s�[�J�P"},
																	   	   	   	   {"�X�s�[�J�Q"},
																				   {"�X�s�[�J�R"},
																	  	  	  	  };
const T_DISPLAY OUTPUT_SPEAKER_SELECT_NAME[SPEAKER_NUM][64]						= {{"�ЂƂ�"},{"�ӂ���"},{"�݂���"}};
/*������@*/
const T_DISPLAY END_METHOD[]													= {"���j���[�ɖ߂� e + �G���^�[\n"};
const T_DISPLAY DUTY_SETTING_METHOD[] 											= {"�f���[�e�B�����͂��Ă��������i1~99���j\n"
																					"SW1:1�@�`�@SW9:9 SW10:0\n"};
const T_DISPLAY TIMER_SETTING_METHOD[]											= {"SW1:�O���ڐݒ�@SW3:�񌅖ڐݒ�@SW5:�ꌅ�ڐݒ�\r\n"
																				   "SW2:�^�C�}�X�^�[�g\r\n"};
const T_DISPLAY ERROR_MESSAGE[]													= {"���͂�����������܂���\n"};
/*�I���o���鍀�ڂ�ς��������͈ȉ��̔z���ύX���āA��������������#define��SELECT_NUM��ς���*/
const unsigned char SELECTABLE_MODE_ARREY[SELECT_MODE_NUM]						= {ORGAN,AUTOPLAY,GAME,TIMER,SETTING};//�I���ł��郂�[�h
const unsigned char SELECTABLE_TITLE_ARREY[SELECT_PLAY_TITLE_NUM]				= {AVE_MARIA,SAINT_MARCH,JESU_JOY_OF_MAN_S,MENUETT,CANON,DORAGON_QUEST,BATTLE1,WINNING};//���̔z��ɓ����Ă���Ȃ����j���[�ɕ\������I���ł���
const unsigned char SELECTABLE_WAVE_ARREY[SELECT_WAVE_NUM]						= {SQUARE,SAWTHOOTH,TRIANGLE,SINE};//�I���ł���g�`
const unsigned char SELECTABLE_SETTING_ARREY[SELECT_SETTING_ITEM_NUM]			= {DUTY,WAVE,SPEAKER_NUM};
const unsigned char SELECTABLE_SPEAKER_ARREY[SELECT_SPEAKER_NUM]				= {SPEAKER1,SPEAKER2,SPEAKER3,};
const unsigned char SELECTABLE_OUTPUT_SPEAKER_ARREY[SELECT_OUTPUT_SPEAKER_NUM] 	= {SPEAKER1,SPEAKER2,SPEAKER3,};

/****************************************************************************/
/* ���[�N�G���A��`															*/
/****************************************************************************/
SPEAKER speaker[3]													= {{50,SQUARE,1},{50,SQUARE,2},{50,SQUARE,3}};
unsigned char electronic_organ_speaker								= 1;//�d�q�I���K�����[�h���ɂ����X�s�[�J��炷��//�ݒ胂�[�h�ŕύX���ēd�q�I���K�����[�h�ŎQ�Ƃ���
unsigned char seg_timer_song										= 0;
unsigned char timer_value[3]										= {"100"};
unsigned long timer_area											= 0;
/****************************************************************************/
/* �v���g�^�C�v�錾															*/
/****************************************************************************/
void main(void);
static void main_sequence_process(void);
static signed short item_select_sequence(const unsigned char *item_select,const unsigned char (*item_name)[64],const unsigned char *select_num,unsigned char item_num,const unsigned char *end_method);
static void selected_mode_transition(unsigned char select);
static void electronic_organ_mode(void);
static void autplay_mode(void);
static void setting_mode(void);
static void game_mode(void);
static void timer_mode(void);
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
//LED1��΂ɓ_��LED�P��ɓ_��
void main(void)
{
	eneiro_initialize();
	timer_area_registration(&timer_area);
	while(1)
	{
		timer_mode();
	}
}
/********************************************************************/
/*7�Z�O�𗘗p�����^�C�}
/*static void timer_mode(void)
/********************************************************************/
/*��肽������
 *	�V�Z�O��_��
 *	�P�Z�O�����g�Â_��������
 *
 */
static void timer_mode(void)
{
	unsigned short ret,last_sw_state			= OFF;
	unsigned short i							= 0;
	unsigned short j							= 0;
	T_DISPLAY timer_value[SEG7_DIGIT_NUM]		= {"000"};
	T_DISPLAY led_color_array[LED_COLOR_NUM]	= {RED,GREEN,BLUE,YELLOW,CYAN,MAGENTA,WHITE};
//	send_serial(TIMER_SETTING_METHOD,sizeof(TIMER_SETTING_METHOD));		//������@�\��
	while(1){
		ret									= sw_check();
		if(ret != OFF && last_sw_state 		!= ret){
			last_sw_state					= ret;
			if(ret == SW2){
				segled_timer_start(timer_value);							//�^�C�}�X�^�[�g
				break;														//���[�v�𔲂���
			}else{
				switch(ret){
				case SW1:
					timer_value[0]		= timer_value[0] + 1;
					if(timer_value[0] == 0x3a)
						timer_value[0]	= 0x30;
					segled_display_update(&timer_value[0]);					//7�Z�O�\�����X�V
					break;
				case SW3:
					timer_value[1]		= timer_value[1] + 1;
					if(timer_value[1] == 0x3a)
						timer_value[1]	= 0x30;
					segled_display_update(&timer_value[0]);					//7�Z�O�\�����X�V
					break;
				case SW5:
					timer_value[2]		= timer_value[2] + 1;
					if(timer_value[2] == 0x3a)
						timer_value[2]	= 0x30;
					segled_display_update(&timer_value[0]);					//7�Z�O�\�����X�V
					break;
				}
			}
		}else
			last_sw_state			= ret;
	}
	while(1){																//�J�E���g�_�E���I���܂őҋ@
		if(timer_value[0] == '0' && timer_value[1] == '0' && timer_value[2] == '0'){
			break;
		}
	}
	automatic_playing_start(CANON,SQUARE,0,0,0);
	while(playing_flg == ON){//���t��
		if(timer_area == 0){
			led_lights_out();
			for(i = 1;i <= LED_NUM;i++){
				output_led(i,led_color_array[j],0);
			}
			j++;
			if(j == 7)
				j = 0;
			timer_area = 1000;
		}
	}
	led_lights_out();
}
/****************************************************************************/
/*���C���V�[�P���X															*/
/*void main_sequence_process(void)											*/
/****************************************************************************/
static void main_sequence_process(void)
{
	static unsigned char main_sequence_num = 0;
	signed char ret;
	switch(main_sequence_num){
		case 0://���[�h�I��
			ret = item_select_sequence(E_NEIRO,MODE_NAME,SELECTABLE_MODE_ARREY,SELECT_MODE_NUM,NULL);
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
//����Ȃɂ�������send_serial���Ȃ��Ƃ����Ȃ���
static void selection_screen_display(const T_DISPLAY *select_item,const T_DISPLAY (*item_name)[64],T_DISPLAY *select_num,unsigned char item_num,T_DISPLAY *end_method)
{
	unsigned char i;
	unsigned char index_num[8];
	send_serial(RESET,10);//��ʂ����Z�b�g
	send_serial(select_item,strlen((const char*)select_item));
	for(i = 1;i <= item_num;i++){
		sprintf((char *)index_num,"%d:",i);
		send_serial(index_num,2);
		send_serial(item_name[select_num[i - 1] - 1],strlen((const char *)item_name[select_num[i - 1] - 1]));
		send_serial("\n",1);
		while(sci0_get_reg_0_flg() != ON){
			//index_num��������������Ƃ��������Ȃ�̂ő��M��������܂ő҂�
		}
	}
	if(end_method != NULL)//�I������Ђ傤�����邩
		send_serial(end_method,strlen((const char*)end_method));
}

/*********************************************************************************************************************************/
/*�A�C�e���Z���N�g�V�[�P���X																									 */
/*signed short item_select_sequence(const unsigned char *item_select,const unsigned char (*item_name)[64],unsigned char item_num)*/
/*		�����Fconst unsigned char *item_select
/*			const unsigned char (*item_name)[64]
 * 			const unsigned char *select_num
 * 			const unsigned char *end_method
/*			unsigned char item_num
/*********************************************************************************************************************************/
static signed short item_select_sequence(const T_DISPLAY *item_select,const T_DISPLAY (*item_name)[64],const T_DISPLAY *select_num,unsigned char item_num,const T_DISPLAY *end_method)
{
	static unsigned char item_select_sequence_num = 0;
	static signed char ret;
	switch(item_select_sequence_num){
	case 0://��ʕ\��
		sci0_receive_start();//��M�J�n
		selection_screen_display(item_select,item_name,select_num,item_num,end_method);//�I����ʂ��\�������
		item_select_sequence_num++;
		break;
	case 1://���͑҂�
		ret = input_check();
		if(ret == ON){
			ret			= a_to_i();//��M�f�[�^�𐔒l�ɂ��Ď󂯎��
			item_select_sequence_num++;
		}else if(ret != OFF)
			item_select_sequence_num++;
		break;
	case 2://���͒l�̔���
		item_select_sequence_num		= 0;
		if(ret <= item_num && ret > 0)
			return select_num[ret - 1];
		if(sci0_find_received_data('e'))
			return 'e';
		break;
	}
	return -1;
}
/*********************************************************************************************************/
/*�f���[�e�B��̐ݒ�																					 */
/*void duty_setting(unsigned char speaker_num)															 */
/*	unsigned char speaker_num �X�s�[�J�ԍ�																 */
/*********************************************************************************************************/
static void duty_setting(void)
{
	signed long ret						= OFF;
	signed short speaker_num			= -1;
	unsigned char duty_one_digits		= NO_SELECT;
	unsigned char duty_two_digits		= NO_SELECT;
	while(speaker_num == -1){
		speaker_num = item_select_sequence(SETTING_SPEAKER_SELECT,SETTING_SPEAKER_NAME,SELECTABLE_SPEAKER_ARREY,SELECT_SPEAKER_NUM,END_METHOD);
	}
	sci0_receive_start();
	send_serial(DUTY_SETTING_METHOD,sizeof(DUTY_SETTING_METHOD));
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
/********************************************************************/
/*�I���������[�h�ɑJ�ڂ���											*/
/*void selected_mode_transition(int *select)						*/
/*	
/********************************************************************/
static void selected_mode_transition(unsigned char select)
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
	case TIMER:
		timer_mode:
		break;
	case SETTING:
		setting_mode();
		break;
	default:
		send_serial(ERROR_MESSAGE,sizeof(ERROR_MESSAGE));
		break;
	}
}

/********************************************************************/
/*�d�q�I���K�����[�h�֐�											*/
/*unsigned char electronic_organ_mode(void)							*/
/********************************************************************/
static void electronic_organ_mode(void)
{
	unsigned char ret				= OFF,i;
	unsigned char last_sw_state		= OFF;
	send_serial(END_METHOD,sizeof(END_METHOD));
	sci0_receive_start();//��M�J�n
	while(1){
		ret							= sci0_enter_check();
		if(ret == ON){
			if(sci0_find_received_data('e') != NOT_FOUND){
				mute(SPEAKER1);
				for(i = 1;i < 9;i++)
					output_led(i,BLACK,0);
				break;
			}else
				sci0_receive_start();//��M�J�n
		}
		ret							= sw_check();
		if(ret != last_sw_state){
			last_sw_state				= ret;
			switch(electronic_organ_speaker){
			case 1:
				set_output_value(last_sw_state,SPEAKER1);
				output_speaker_start(1);
				break;
			case 2:
				set_output_value(last_sw_state,SPEAKER1);
				set_output_value(last_sw_state + 12,SPEAKER2);
				output_speaker_start(3);
				break;
			case 3:
				set_output_value(last_sw_state,SPEAKER1);
				set_output_value(last_sw_state + 12,SPEAKER2);
				set_output_value(last_sw_state + 24,SPEAKER3);
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
static void autplay_mode(void)
{
	unsigned char ret			= 0;
	signed short title			= -1;
	signed short wave_type		= -1;
	while(title == -1){
		title = item_select_sequence(PLAYLIST_SELECT,TITLE_NAME,SELECTABLE_TITLE_ARREY,SELECT_PLAY_TITLE_NUM,END_METHOD);
	}
	if(title == 'e')
		return;//�^�C�g���I����e�����͂��ꂽ�烁�j���[�֖߂�
	while(wave_type == -1){
		wave_type = item_select_sequence(WAVETYPE_SELECT,WAVE_TYPE_NAME,SELECTABLE_WAVE_ARREY,SELECT_WAVE_NUM,END_METHOD);
	}
	if(wave_type == 'e')
		return;//�g�`�I����e�����͂��ꂽ�烁�j���[�֖߂�
	send_serial(TITLE_NAME[title - 1],sizeof(TITLE_NAME[title - 1]));
	send_serial(WAVE_TYPE_NAME[wave_type - 1],sizeof(WAVE_TYPE_NAME[wave_type - 1]));
	automatic_playing_start((unsigned short)title,wave_type,0,0,0);
	while(playing_flg == ON){
		ret = input_check();
		if(ret != OFF)
			auto_play_end_processing();
	}
}

/********************************************************************/
/*�Q�[�����[�h�֐�													*/
/*void game_mode(void)											*/
/********************************************************************/
static void game_mode(void)
{
	game_main();
}


/********************************************************************/
/*�ݒ胂�[�h�֐�													*/
/*void setting_mode(void)											*/
/********************************************************************/
static void setting_mode(void)
{
	signed short setting_num			= -1;
	while(setting_num == -1){
		setting_num = item_select_sequence(SETTING_ITEM_SELECT,SETTING_ITME_NAME,SELECTABLE_SETTING_ARREY,SETTING_ITEM_NUM,END_METHOD);
	}
	switch(setting_num){
	case DUTY:
//		�f���[�e�B��̃X�s�[�J���Ƃ̐ݒ肪�ł��Ȃ�
		duty_setting();
		break;
	case WAVE:
		do{
			setting_num = item_select_sequence(WAVETYPE_SELECT,WAVE_TYPE_NAME,SELECTABLE_WAVE_ARREY,SELECT_WAVE_NUM,END_METHOD);
		}while(setting_num == -1);
		speaker[0].wave_type= setting_num;
		break;
	case SPEAKER_NUM://�d�q�I���K�����[�h���ɖ炷�X�s�[�J�̔ԍ��w��
		do{
			setting_num = item_select_sequence(OUTPUT_SPEAKER_SELECT,OUTPUT_SPEAKER_SELECT_NAME,SELECTABLE_OUTPUT_SPEAKER_ARREY,SELECT_OUTPUT_SPEAKER_NUM,END_METHOD);
		}while(setting_num == -1);
		electronic_organ_speaker = setting_num;
		break;
	case 'e':
		break;
	}
}
/*
 * �G���^�[�L�[�ƃX�C�b�`���͂̔���
 *unsigned char input_check(void)
 *	�߂�l�Funsigned char ON:�X�C�b�`�������Ă��痣���ꂽ�G���^�[�L�[�����͂���Ă����@OFF:���͂Ȃ�
 */
 /*
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
*/
//�����ꂽ��ON��Ԃ��d�l
//�X�C�b�`�ԍ��܂���ENTER�QON��Ԃ��d�l
unsigned char input_check(void)
{
	unsigned char ret						= OFF;
	unsigned char sw_state					= OFF;
	sw_state								= sw_check();
	if(sw_state != OFF)								//�X�C�b�`��������Ă�����
		ret									= sw_state;	//�X�C�b�`�̏�ԋL�^
	else if(sci0_enter_check() == ON)					//�X�C�b�`��������Ă��鎞�̓G���^�[�͌��Ȃ�
		ret									= ENTER_ON;
	return ret;
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
