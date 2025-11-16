/*
 * main.h
 *
 *  Created on: 2024/05/03
 *      Author: kameyamahiroki
 */

#ifndef MAIN_H_
#define MAIN_H_

//�w�b�_�[�t�@�C��

#include "iodefine.h"
#include <math.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


#include "clock.h"
#include "lcd.h"
#include "led.h"
#include "speaker.h"
#include "sci.h"

#include "sw.h"
#include "timer.h"
#include "port.h"
#include "puzzle.h"

#include "ad.h"
//#include "battle.h"
#include "flash.h"


//#define LCD

#define SET (1)
#define NOT_FOUND (0)
//�I���ł��鍀�ڐ�
#define SELECT_MODE_NUM (6)					//�I�����郂�[�h��
#define SELECT_PLAY_TITLE_NUM (10)			//�������t���[�h�ŉ��t����Ȑ�
#define SELECT_WAVE_NUM (4)					//�I������g�`�̎��
#define SELECT_SETTING_ITEM_NUM (3)
#define SELECT_SPEAKER_NUM (3)
#define SELECT_OUTPUT_SPEAKER_NUM (3)

//�S���ڐ�
#define MODE_NUM (6)							//�S���[�h��
#define SONG_NUM (19)							//�S���̋Ȑ�
#define WAVE_NUM (4)							//�S�g�`�̎��
#define SETTING_ITEM_NUM (3)					//�ݒ荀�ڐ�
#define SPEAKER_NUM (3)
#define E2_TEST_NUM (3)
//��ʁA��������
#define SCREEN_ERACE "\x1b[2J"
#define RED_CHAR "\x1b[31m"
#define YELLOW_CHAR "\x1b[33m"
#define BLUE_CHAR "\x1b[36m"
#define GREEN_CHAR "\x1b[32m"
#define PURPLE_CHAR "\x1b[35m"
#define DEFAULT_CHAR "\x1b[39m"
#define DEFAULT_BACKGROUND "\x1b[49m"
#define CRLF "\r\n"
#define DISPLAY_CLEAR "\x1b[2J"
#define RESET "\x1b[2J\x1b[0;0H"				//��ʂ��N���A���Đݒ���������J�[�\�������_�ɖ߂�
#define DELETING_LEFT_SIDE "\x1b[K"				//�J�[�\���ƍs���ɂ��镶�����폜
#define CURSOR_5LINE_BUCK "\x1b[5F"
#define CURSOR_4LINE_BUCK "\x1b[4F"
#define CURSOR_3LINE_BUCK "\x1b[3F"
#define CURSOR_2LINE_BUCK "\x1b[2F"
#define CURSOR_1LINE_BUCK "\x1b[1F"
#define CURSOR_2LINE_ADVANCE "\x1b[2E"
#define CURSOR_3LINE_ADVANCE "\x1b[3E"
#define CURSOR_MOVING_LEFT_MOST "\x1b[0`"		//�J�[�\�������[�Ɉړ�
#define CURSOL_MOVING_SENTER "\x1b[40G"			//�J�[�\������ʂ̒����Ɉړ�
//#define CURSOR__BACK_ORIGIN "\x1b[0;0H"
#define PCLK (48000000)
#define BAUD_RATE (115200)
#define CKS8 (0)								//�R���y�A�}�b�`�^�C�}��PCLK�̕���/8
#define CKS32 (1)								//�R���y�A�}�b�`�^�C�}��PCLK�̕���/32
#define CKS128 (2)								//�R���y�A�}�b�`�^�C�}��PCLK�̕���/128
#define CKS512 (3)								//�R���y�A�}�b�`�^�C�}��PCLK�̕���/512
#define KEY_UP (72)								//���L�[
#define KEY_DOWN (80)							//���L�[
#define KEY_LEFT (75)							//���L�[
#define KEY_RIGHT (77)							//���L�[
typedef char* T_DISPLAY;

enum menu{ORGAN = 1,AUTOPLAY,GAME,TIMER,SETTING,E2TEST};//���C�����j���[�̍���
//�Ȗ�
enum title{AVE_MARIA = 1,SAINT_MARCH,MENUETT,JESU_JOY_OF_MAN_S,AURA_LEE,SAKURA,ZYOUNETUTAIRIKU,LET_IT_BE,NHK_NODOZIMAN,DORAGON_QUEST,LEVEL_UP,
			WINNING,BOUKENNNOSYO,YADOYA,ALLY_ATACK,BATTLE1,ZENNMETU,INITIAL_CHECK,CANON,};
//�g�`
enum wave{SQUARE = 1,SAWTHOOTH,TRIANGLE,SINE};
enum SPEAKERS{SPEAKER1 = 1,SPEAKER2,SPEAKER3,ALL_SPEAKER};
enum setting{DUTY = 1,WAVE,};


#include "score.h"
//�\���̐錾





#include "auto_play.h"
extern unsigned char playing_flg;
extern unsigned char mode;
/****************************************************************************/
/*�v���g�^�C�v�錾																*/
/****************************************************************************/
struct SPEAKER *get_speaker(unsigned char speaker_num);
unsigned char input_check(void);
unsigned char input_wait(void);
#include "game.h"
#endif /* MAIN_H_ */
