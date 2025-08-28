/*
 * game.c
 *
 *  Created on: 2024/05/26
 *      Author: kameyamahiroki
 */
/*
 * �w�b�_�[�t�@�C��
 */

#include "main.h"
#include "game.h"
/********************************************************************************************/
/*�v���g�^�C�v�錾																				*/
/********************************************************************************************/
void game_sequence(void);
/*
 * �萔��`
 */
const T_DISPLAY GAME_TITLE = {"\x1b[2J\x1b[47A\x1b[44m\x1b[31m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
						   "~                                          ~\n"
						   "~               \x1b[37meNeiro QUEST\x1b[31m               ~\n"
						   "~                                          ~\n"
		                   "~                 \x1b[37mSTART:SW1\x1b[31m                ~\n"
						   "~                 \x1b[37mCONTINUE:SW2\x1b[31m             ~\n"
						   "~                                          ~\n"
		                   "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\x1b[m\n"
							};
const T_DISPLAY SAVE_DATA_CREATION	= {"1:�ڂ�����̂��������\n"};
const T_DISPLAY LOAD_SAVE_DATA		= {"2:�ڂ����������\n"};
const T_DISPLAY INPUT_NAME 			= {"\x1b[2J\x1b[9A�@�Ȃ܂� \r\n>"};
const T_DISPLAY ARRIVAL				= {"�̓_���W�����ɓ�������\n"};
const T_DISPLAY GAME_CLEAR 			= {"�̓_���W�����𐧔e�����I\r\n~~~~GAME CLEAR!~~~~\r\n"};
const T_DISPLAY GAME_OVER 			= {"~~~~GAME_OVER~~~~"};
AUTOPLAYER REPEATING_FROM_INTERMEDIATE[3] = {{SQUARE,DORAGONQUEST_SCORE1,DORAGONQUEST_NOTE_VALUE1,32,375,96,OFF,OFF},//�I�[�v�j���O�Ȃ�r������
													{SQUARE,DORAGONQUEST_SCORE2,DORAGONQUEST_NOTE_VALUE2,32,375,93,OFF,OFF},
													{SQUARE,DORAGONQUEST_SCORE3,DORAGONQUEST_NOTE_VALUE3,1,500,62,OFF,OFF},
													};

enum {GAME_START,BOUKENNNOSYO,SELECT,NAME_SETTING,BATTLE,GAME_CLEAR,GAME_OVER,GAME_END};
/********************************************************************************************/
/*���[�N�G���A��`																			*/
/********************************************************************************************/
T_MONSTER enemy[5] = {{"~�X���C��~",100,100,WATER,10,5},{"#�S�u����#",200,200,SOIL,20,15},
						{"@�I�I�R�E����@",300,300,WIND,30,25},{"@�E�F�A�E���t@",400,400,WIND,40,30},
						{"$�h���S��$",800,800,FIRE,50,40},};

T_MONSTER ally[ALLY_NUM] = {{"$�鐝$",150,150,FIRE,25,10},
						{"~����~",150,150,WATER,20,15},
						{"@��@",150,150,WIND,15,10},
						{"#����#",150,150,SOIL,20,5},};
T_PLAYER player = {"�A���X",0,0};
unsigned char sw						= OFF;
unsigned char last_sw					= 'e';
unsigned char output_num				= OFF;
unsigned char last_output_num			= OFF;
unsigned char g_sequence;//�V�[�P���X�ԍ�
signed short party_hp;//�p�[�e�B�[�S�̂�HP
/********************************************************************************************/
/*�Q�[�����[�h���C��																		*/
/*void game_mode(void)																		*/
/********************************************************************************************/
void game_main(void)
{
	while(g_sequence != GAME_END){
		game_sequence();
	}
	send_serial(RESET,4);
	g_sequence = 0;
}

/********************************************************************************************/
/*�Q�[���Ɏg����p�����[�^�̏�����														*/
/*void game_param_init(void)																*/
/********************************************************************************************/
void game_param_init(void)
{
	unsigned char i;
	unsigned short temp;
	for (i = 0; i < ALLY_NUM; i++) {
		player.max_hp = player.hp += ally[i].hp;	//���������X�^�[��HP�̍��v���v���C���[HP�̍��v�ɐݒ�
		temp += ally[i].gp;		//���������X�^�[�̖h��͂����v����
	}
	player.gp = temp / ALLY_NUM;			//���ϒl��h��͂ɐݒ�
}
/***************************************************************************/
/*���t���Ȃ�����͑҂�*/
/*unsigned char wait_tune_with(unsigned char title,unsigned char wave_type)*/
/*	�����Funsigned char title�@���t������*/
/*		�@unsigned char wave_type�@�g�`									   */
/**/
/***************************************************************************/
unsigned char wait_tune_with(unsigned char title, unsigned char wave_type)
{
	unsigned char ret;
	autoplay_start_from_beginning(title, wave_type);
	sci0_receive_start();//��M�J�n
	while (1) {
		ret = input_check();
		if (ret != OFF) {
			return ret;
			if (playing_flg == OFF)
				autoplay_start_from_beginning(title, wave_type);
		}
	}
}
/********************************************************************************************/
/*�Q�[���J�n����																			*/
/*void game_atart(void)																		*/
/********************************************************************************************/
void game_start(void)
{
	game_param_init();										//�v���C���[�̃p�����[�^������
	send_serial(GAME_TITLE, strlen(GAME_TITLE));			//�^�C�g���\��
	autoplay_start_from_beginning(DORAGON_QUEST, SQUARE);	//�I�[�v�j���O�Ȃ��������t�J�n
	sci0_receive_start();									//��M�J�n
	while (1) {
		ret = input_check();
		if (ret != OFF) {
			auto_play_end_processing();
			break;
		}
		else if (playing_flg == OFF)//�Ō�܂ŉ��t���ꂽ���͓r�����牉�t
			autoplay_start_from_intermediate(REPEATING_FROM_INTERMEDIATE[0], REPEATING_FROM_INTERMEDIATE[1], REPEATING_FROM_INTERMEDIATE[2]);
	}
	g_sequence	= BOUKENNNOSYO;
}

/********************************************************************************************/
/*�Z�[�u�f�[�^�̊m�F
/*void boukennnosyo_check(void)
/********************************************************************************************/
void boukennnosyo_check(void)
{
#if 0
	if (blank_check())//�Z�[�u�f�[�^������΁u�ڂ����������v�u�ڂ�����̂��������v�����\��
#endif
		send_serial(RESET, 10);
		send_serial(SAVE_DATA_CREATION, strlen(SAVE_DATA_CREATION));
		send_serial(LOAD_SAVE_DATA, strlen(LOAD_SAVE_DATA));
/*	else
		send_serial(SAVE_DATA_CREATION);*/
		g_sequence = SELECT;
}


void create_or_road(void)
{
	char* str1 = {"1\r\n"};
	char* str2 = {"2\y\n"};
	while (1) {
		ret = wait_tune_with(BOUKENNNOSYO, SQUARE);
		if (ret == ON) {
			if (sci0_comp(str1) == 0) {
				g_sequence = NAME_SETTING;
				break;
			}
			if (sci0_comp(str2) == 0) {
				g_sequence = NAME_SETTING;
				break;
			}
		}
		else if (ret == SW1 || ret == SW2) {//�f�[�^�t���b�V�������܂łǂ��炪������Ă����O���͂ɂ���
			g_sequence = NAME_SETTING;
			break;
		}
	}
}

void player_name_setting(void)
{
	sci0_receive_start();//��M�J�n
	send_serial(RESET, 10);
	send_serial(INPUT_NAME, strlen(INPUT_NAME));
	ret = input_check();
	if (ret == ON) {
		ret = sci0_str_cpy(player.name);//���͂��v���C���[�̖��O�ɐݒ�
		if (ret >= 3)
			g_sequence++;
		else
			sci0_receive_start();//��M�J�n
	}
	else if (playing_flg == OFF)
		autoplay_start_from_beginning(BOUKENNNOSYO, SQUARE); 7
}
/****************************************************************************/
/*�Q�[���V�[�P���X															*/
/*void game_sequence(void)													*/
/****************************************************************************/
void game_sequence(void)
{
	enum { GAME_START, BOUKENNNOSYO, SELECT, INPUT_NAME, BATTLE, GAME_CLEAR, GAME_OVER };
	unsigned char ret;
	switch(g_sequence){
	case GAME_START:
		game_start();
		break;
	case BOUKENNNOSYO://�Z�[�u�f�[�^���m�F����
		boukennnosyo_check();
		break;
	case SELECT:
		create_or_road
		break;
	case NAME_SETTING://���O�̓��͑����\��
		player_name_setting();
		break;
	case 6:
		auto_play_end_processing();
		send_serial(player.name,strlen((const char*)player.name));
		send_serial(ARRIVAL, strlen(ARRIVAL));
		g_sequence++;
		break;
	case 7://�����X�^�[�Ɛ퓬
		battle(enemy,ally,&player);
		g_sequence++;
		break;
	case 8:
		send_serial(player.name, strlen(player.name));
		send_serial(GAME_CLEAR, strlen(GAME_CLEAR));
		g_sequence	= GAME_END;
		break;
	default:
		send_serial(GAME_OVER, strlen(GAME_OVER));
		autoplay_start_from_beginning(ZENNMETU,SQUARE);
		while(playing_flg == ON){

		}
		g_sequence = GAME_END;
	}
}

T_MONSTER get_ally_data(unsigned char ally_num)
{
	return ally[ally_num];
}
