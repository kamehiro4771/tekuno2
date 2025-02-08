
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

const unsigned char E_NEIRO[]		= {"\x1b[2J\x1b[0;0H\x1b[2J\x1b[13A******************** e-NeIRO *********************\n"
										"�P.�d�q�I���K�����[�h\n"
										"�Q.�������t���[�h\n"
										"�R.�Q�[�����[�h\n"
										"�S.�ݒ�\n\x1b[49m"
										};
const unsigned char DUTY_VALUE[]			= {"�f���[�e�B�[��F"};
const unsigned char WAVE_TYPE[]				= {"�g�`�F"};
const unsigned char playlists_select[]		= {"\x1b[2J\x1b[13A�Ȃ�I�����ĉ�����\n"
										};
const unsigned char wavetype_select[]		= {"���̔g�`��I�����Ă�������\n"};
const unsigned char setting_item_select[] 	= {"�ݒ肷�鍀�ڂ�I�����Ă�������\n"};
const unsigned char end_method[]				= {"���j���[�ɖ߂� e + �G���^�[\n"};
const unsigned char title_name[SONG_NUM][64]	= {{"1.�A���F�E�}���A\n"},
										{"2.���҂̍s�i\n"},
										{"3.���k�G�b�g\n"},
										{"4.���A�l�̖]�݂̊�т�\n"},
										{"5.�I�[���E���[\n"},
										{"6.������i�Ə��j\n"},
										{"7.��M�嗤\n"},
										{"8.Let it Be\n"},
										{"9.NHK�̂ǎ����̃e�[�}��\n"},
										{"10�h���S���N�G�X�g����\n"},};									//9.���N����@10�D���̃����[�N���X�}�X
const unsigned char wave_type_name[WAVE_NUM][64] 	= {{"1.��`�g\n"},
											{"2.�̂�����g\n"},
											{"3.�O�p�g\n"},
											{"4.�T�C���g\n",}};

const unsigned char setting_item_name[SETTING_ITEM_NUM][64] = {{"1.�f���[�e�B�[��\n"},
													{"2.�g�`\n"},
													{"3.�L�[�̍���\n"}};

const unsigned char duty_setting_display[] 	= {"�f���[�e�B�����͂��Ă��������i1~99���j\n"
										"SW1:1�@�`�@SW9:9 SW10:0\n"};
const unsigned char setting_comp[]			= {"�ɐݒ肳��܂����B\n"};
const unsigned char error_message[]			= {"���͂�����������܂���\n"};
/*
 * ���[�N�G���A��`
 */
struct SPEAKER speaker[3]			= {{NULL,NULL,0,0,50,SQUARE,0,1,OFF,OFF},
										{NULL,NULL,0,0,50,SQUARE,0,2,OFF,OFF},
										{NULL,NULL,0,0,50,SQUARE,0,3,OFF,OFF}};
unsigned char g_output_string[512];
unsigned char g_sequence_num;
unsigned char g_select;
unsigned char play_up_to_last		= ON;//�������t���A���̕ϐ���ON�Ȃ�Ō�܂ŉ��t����
/*
 * �v���g�^�C�v�錾
 */
void main(void);
void main_sequence_process(void);
signed short item_select_sequence(const unsigned char *item_select,const unsigned char (*item_name)[64],unsigned char item_num);
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
	play_up_to_last = OFF;
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
	switch(g_sequence_num){
		case 0:
			sci0_receive_start();//��M�J�n
			sprintf((char *)g_output_string,"%s%s%d�� %s%s��SW1~SW3���̓L�[�{�[�h1~3+�G���^�[\n**************************************************\n"
					,E_NEIRO,DUTY_VALUE,speaker[0].duty_value,WAVE_TYPE,wave_type_name[speaker[0].wave_type - 1]);
			send_serial(g_output_string,strlen(g_output_string));
			g_sequence_num++;
			break;
		case 1:
			if(sci0_enter_check() == ON){//�G���^�[�m�F
				g_select				= a_to_i();//��M�f�[�^�𐔒l�ɂ��Ď󂯎��
				g_sequence_num			= 4;
			}
			else
				g_sequence_num++;
			break;
		case 2:
			g_select					= sw_check();
			if(g_select != OFF)
				g_sequence_num++;
			else
				g_sequence_num			= 1;
			break;
		case 3:
			if(sw_check() == OFF)
				g_sequence_num++;
			break;
		case 4:
			selected_mode_transition(g_select);
			g_sequence_num				= 0;
			break;
	}
}
/****************************************************************************/
/*�I�����ڕ\��																*/
/*void selection_screen_display(char *item,char *item_name,unsigned char item_num)	*/
/*			char *item:�I��������e											*/
/*			char *item_name:�I�����鍀�ږ�									*/
/*			char item_num:���ڂ̐�											*/
/****************************************************************************/
static void selection_screen_display(const unsigned char *item,const unsigned char (*item_name)[64],unsigned char item_num)
{
	int i;
	send_serial(RESET,10);
	send_serial(item,strlen((const char*)item));
	for(i = 0;i < item_num;i++){
		send_serial(item_name[i],sizeof(item_name[i]));
	}
	send_serial(end_method,sizeof(end_method));
}

/*********************************************************************************************************************************/
/*�A�C�e���Z���N�g�V�[�P���X																									 */
/*signed short item_select_sequence(const unsigned char *item_select,const unsigned char (*item_name)[64],unsigned char item_num)*/
/*
/*
/*
/*
/*
/*********************************************************************************************************************************/
signed short item_select_sequence(const unsigned char *item_select,const unsigned char (*item_name)[64],unsigned char item_num)
{
	switch(g_sequence_num){
	case 0:
		sci0_receive_start();//��M�J�n
		selection_screen_display(item_select,item_name,item_num);//�I����ʂ��\�������
		g_sequence_num++;
		break;
	case 1:
		if(sci0_enter_check() ==ON){
			g_select			= a_to_i();//��M�f�[�^�𐔒l�ɂ��Ď󂯎��
			if(sci0_find_received_data('e'))
				return 'e';
			g_sequence_num		= 4;
		}else
			g_sequence_num++;
		break;
	case 2:
		g_select					= sw_check();
		if(g_select != OFF)
			g_sequence_num++;
		else
			g_sequence_num			= 1;
		break;
	case 3:
		if(sw_check() == OFF)
			g_sequence_num++;
		break;
	case 4:
		if(g_select > item_num)
			g_sequence_num		= 0;
		else{
			g_sequence_num			= 0;
			return g_select;
		}
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
	case SETTING:
		setting_mode();
		break;
	case GAME:
		game_mode();
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
	int output_num				= OFF;
	int last_output_num			= OFF;
	send_serial(end_method,sizeof(end_method));
	sci0_receive_start();//��M�J�n
	while(1){
		output_num				= sw_check();
		if(output_num == OFF){//�X�C�b�`��OFF�Ȃ�
			if(sci0_enter_check() == ON){//�G���^�[�������ꂽ���m�F����
				set_output_value(output_num,0);
				break;
			}
		}
		if(last_output_num != output_num){
			last_output_num			= output_num;
			set_output_value(output_num,0);
			output_speaker_start(1);
		}
	}
}

/********************************************************************/
/*�������t���[�h�֐�												*/
/*void autplay_mode(void)											*/
/********************************************************************/
void autplay_mode(void)
{
	signed short title			= -1;
	signed short wave_type		= -1;
	play_up_to_last				= OFF;
	g_sequence_num				= 0;
	while(title == -1){
		title = item_select_sequence(playlists_select,title_name,SONG_NUM);
	}
	if(title == 'e')
		return;//�^�C�g���I����e�����͂��ꂽ�烁�j���[�֖߂�
	while(wave_type == -1){
		wave_type = item_select_sequence(wavetype_select,wave_type_name,WAVE_NUM);
	}
	if(wave_type == 'e')
		return;//�g�`�I����e�����͂��ꂽ�玩�����t���[�h�I��
	send_serial(title_name[title - 1],sizeof(title_name[title - 1]));
	send_serial(wave_type_name[wave_type - 1],sizeof(wave_type_name[wave_type - 1]));
	automatic_playing((unsigned short)title,wave_type,0,0,0);
	while(playing_flg == ON){
		/*nop*/
	}
}

/********************************************************************/
/*�ݒ胂�[�h�֐�													*/
/*void setting_mode(void)											*/
/********************************************************************/
void setting_mode(void)
{
	signed short setting_num			= -1;
	while(1){
		while(setting_num == -1){
			setting_num = item_select_sequence(setting_item_select,setting_item_name,SETTING_ITEM_NUM);
		}
		switch(setting_num){
		case DUTY:
			duty_setting();
			return;
		case WAVE:
			item_select_sequence(wavetype_select,wave_type_name,WAVE_NUM);
			return;
		case 3:
	//		while(semibreve_value_setting(semibreve_value));
			break;
		case 101:
			return;
		}
	}
}
#if 0
void game_mode(void)
{
	game_main();
}
#endif

/*********************************************************************************************************/
/*�f���[�e�B��̐ݒ�֐�
/*void duty_setting(void)
/*********************************************************************************************************/
void duty_setting(void)
{
#if 0
	int select					= OFF;
	int last_select				= NO_SELECT;
	int duty_one_digits			= OFF;
	int duty_two_digits			= OFF;
	while(1){
		if(last_select == NO_SELECT){
			last_select			= OFF;
			send_serial(duty_setting_display,sizeof(duty_setting_display));
			while(sci0_send_comp_check());
		}
		select					= sw_check();
		if(select == OFF){
			if(last_select != select){
				if(last_select >= 0 && last_select <= 9){
					if(duty_two_digits == OFF){
						duty_two_digits = (last_select + 1) % 10;
					}else{
						duty_one_digits = (last_select + 1) % 10;
					}
					last_select			= OFF;
				}else{
					duty_two_digits		= OFF;
					duty_one_digits		= OFF;
					last_select = 'e';
				}
				if(duty_one_digits != OFF){
										= (duty_two_digits * 10) + duty_one_digits;
					break;
				}
			}else if(sci0_find_received_data('\n')){
				speaker[0].duty_value				= atoi(sci0_get_receive_data());
//				sci0_delete_receive_data();
				if(duty_two_digits != OFF){
					duty_two_digits		= OFF;
					last_select = 'e';
				}else if( <= 0 ||  >= 100){
					last_select = NO_SELECT;//�l������������
				}else{
					break;
				}
			}
		}else{
			last_select		= select;
		}
	}
	sprintf(g_output_string,"%d��%s",,setting_comp);
	send_serial(g_output_string,sizeof(g_output_string));
	while(sci0_send_comp_check());
	return;
#endif
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
