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
#include "auto_play.h"
#include "clock.h"
#include "initialize.h"
#include "lcd.h"
#include "output_led.h"
#include "output_speaker.h"
#include "sci.h"
#include "sw.h"
#include "timer.h"
#include "port.h"
#include "puzzle.h"
#include "game.h"
#include "ad.h"
#include "battle.h"
/*�萔��`*/
#define ERROR (1)
#define SUCCESS (0)
#define ON (0)
#define SET (1)
#define NOT_FOUND (0)
#define AUTO_PLAY_TITLE_NUM (5)//�������t���[�h�ŉ��t����Ȑ�
#define MODE_NUM (4)//���[�h��
#define SONG_NUM (19)//�S���̋Ȑ�
#define WAVE_NUM (4)//�g�`�̎��
#define SUPEAKER_NUM (3)
#define SETTING_ITEM_NUM (3)//�ݒ荀�ڐ�
#define SPEAKER1 (0)
#define SPEAKER2 (1)
#define SPEAKER3 (2)
//�Q�[���Ŏg�p����
#define FIELD_NUM (13)//�o�g���t�B�[���h�̐�
#define ONE_JEWEL (6)//��̕�΂̃o�C�g��
#define ENEMY_NUM (5)//�G�����X�^�[�̐�
#define ALLY_NUM (4)//�����̐�
#define FIRE (0)
#define WATER (1)
#define WIND (2)
#define SOIL (3)
#define LIFE (4)
#define WIN (1)
#define LOSE (0)
#define NEW_FIELD (0)
#define CURRENT_FIELD (1)
#define UPDATE_FIELD (2)
#define ATTRIBUTE_NUM (4)
//��ʁA��������
#define SCREEN_ERACE "\x1b[2J"
#define RED_CHAR "\x1b[31m"
#define YELLOW_CHAR "\x1b[33m"
#define BLUE_CHAR "\x1b[36m"
#define GREEN_CHAR "\x1b[32m"
#define DEFAULT_CHAR "\x1b[39m\r\n"
#define DEFAULT_BACKGROUND "\x1b[49m"
#define CRLF "\r\n"
#define DISPLAY_CLEAR "\x1b[2J"
#define RESET "\x1b[2J\x1b[0;0H"
//#define RESET "\x1b[2J\x1b[!p\x1b[0;0H"//��ʂ��N���A���Đݒ���������J�[�\�������_�ɖ߂�
#define CURSOR_3LINE_BUCK "\x1b[3F"
#define CURSOR_2LINE_BUCK "\x1b[2F"
#define CURSOR_1LINE_BUCK "\x1b[1F"
#define CURSOR_2LINE_ADVANCE "\x1b[2E"
#define CURSOR_3LINE_ADVANCE "\x1b[3E"
//#define CURSOR__BACK_ORIGIN "\x1b[0;0H"
/*
const unsigned char SCREEN_ERACE[]			= {"\x1b[2J"};
const unsigned char RED_CHAR[]				= {"\x1b[31"};
const unsigned char YELLOW_CHAR[]			= {"\x1b[33"};
const unsigned char BLUE_CHAR[]				= {"\x1b[36"};
const unsigned char GREEN_CHAR[]			= {"\x1b[32"};
const unsigned char DEFAULT_CHAR[]			= {"\x1b[39"};
const unsigned char DEFAULT_BACKGROUND[]	= {"\x1b[49"};
*/
#define PCLK (48000000)
#define BAUD_RATE (115200)
#define CKS8 (0)//�R���y�A�}�b�`�^�C�}��PCLK�̕���/8
#define CKS32 (1)//�R���y�A�}�b�`�^�C�}��PCLK�̕���/32
#define CKS128 (2)//�R���y�A�}�b�`�^�C�}��PCLK�̕���/128
#define CKS512 (3)//�R���y�A�}�b�`�^�C�}��PCLK�̕���/512
#define KEY_UP (72)//���L�[
#define KEY_DOWN (80)//���L�[
#define KEY_LEFT (75)//���L�[
#define KEY_RIGHT (77)//���L�[
enum menu{ORGAN = 1,AUTOPLAY,GAME,SETTING};//���C�����j���[�̍���
//�Ȗ�
enum title{AVE_MARIA = 1,SAINT_MARCH,MENUETT,JESU_JOY_OF_MAN_S,AURA_LEE,SAKURA,ZYOUNETUTAIRIKU,LET_IT_BE,NHK_NODOZIMAN,DORAGON_QUEST,LEVEL_UP,
			WINNING,BOUKENNNOSYO,YADOYA,ALLY_ATACK,BATTLE1,ZENNMETU,INITIAL_CHECK,CANON,};
//�g�`
enum wave{SQUARE = 1,SAWTHOOTH,TRIANGLE,SINE};
enum setting{DUTY = 1,WAVE,};
enum{
	SW1 = 1,SW2,SW3,SW4,SW5,SW6,SW7,SW8,SW9,SW10,SW11,SW12,SW13,REST = 73,COMP = 73,NO_SELECT = 73,OFF = 73,
};
//�\���̐錾
struct SCI{
	unsigned char receive_data[128];
	unsigned char receive_count;
	const unsigned char *send_data[64]; //���M����f�[�^�ւ̃|�C���^
	unsigned short send_length[64];		//���M����f�[�^�̒���
	unsigned char send_counter;			//
	unsigned char reg_cnt;				//���M����f�[�^�̓o�^��
	unsigned char send_compflg;			//���M���I���������m�F����t���O
	unsigned char enter_flg;			//�G���^�[�������ꂽ���m�F���邽�߂̃t���O
	signed long elapsed_time;			//�^�C�}�J�E���g�G���A
};
/*
 *
 */
typedef struct playing_resume_point{
			signed short start;
			long elapsed;
			}RESUME_POINT;
//elapsed_times��core_count���\���̂ɂ��Ă܂Ƃ߂Ď󂯓n���ł���悤�ɂ���
//���t���f�|�C���g�����炤���ɍ\���̂ŖႦ��悤�ɂ���
typedef struct SPEAKER{
	const unsigned char *pscore;		//�y���z��ւ̃|�C���^
	const unsigned short *pnote_value;	//���̒����z��ւ̃|�C���^
	unsigned short score_count;			//�X�R�A�J�E���g
	unsigned long elapsed_time;			//�o�ߎ��ԊĎ��G���A
	float duty_value;					//�f���[�e�B��
	unsigned char wave_type;			//�o�͔g�`�i�X�s�[�J�[�P�́j
	unsigned short note_size;			//�����̐�
	unsigned char speaker_num;			//�X�s�[�J�[�ԍ�0�`2
	unsigned char set_flg;				//�������t���X�R�A�J�E���g���i�񂾂̂ŃZ�b�g���K�v�ł��邱�Ƃ�m�点��
	unsigned char end_flg;
}SPEAKER;

extern unsigned char playing_flg;
/****************************************************************************/
/*�v���g�^�C�v�錾																*/
/****************************************************************************/
struct SPEAKER *get_speaker(void);

#endif /* MAIN_H_ */
