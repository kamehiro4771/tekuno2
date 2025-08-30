/*
 * main.h
 *
 *  Created on: 2024/05/03
 *      Author: kameyamahiroki
 */

#ifndef MAIN_H_
#define MAIN_H_

//ヘッダーファイル

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
#include "game.h"
#include "ad.h"
#include "battle.h"
#include "flash.h"

#pragma address e2_FLASH = 0x00100000
//#define LCD
#define ERROR (1)
#define SUCCESS (0)
#define ON (0)
#define ENTER_ON (0)
#define SET (1)
#define BLANK (0)
#define WRITTEN_STATE (1)
#define NOT_FOUND (0)
//選択できる項目数
#define SELECT_MODE_NUM (5)					//選択するモード数
#define SELECT_PLAY_TITLE_NUM (9)			//自動演奏モードで演奏する曲数
#define SELECT_WAVE_NUM (4)					//選択する波形の種類
#define SELECT_SETTING_ITEM_NUM (3)
#define SELECT_SPEAKER_NUM (3)
#define SELECT_OUTPUT_SPEAKER_NUM (3)
//全項目数
#define MODE_NUM (5)							//全モード数
#define SONG_NUM (19)							//全部の曲数
#define WAVE_NUM (4)							//全波形の種類
#define SETTING_ITEM_NUM (3)					//設定項目数
#define SPEAKER_NUM (3)

//画面、文字制御
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
#define RESET "\x1b[2J\x1b[0;0H"				//画面をクリアして設定を初期化カーソルを原点に戻す
#define DELETING_LEFT_SIDE "\x1b[K"				//カーソルと行末にある文字を削除
#define CURSOR_5LINE_BUCK "\x1b[5F"
#define CURSOR_4LINE_BUCK "\x1b[4F"
#define CURSOR_3LINE_BUCK "\x1b[3F"
#define CURSOR_2LINE_BUCK "\x1b[2F"
#define CURSOR_1LINE_BUCK "\x1b[1F"
#define CURSOR_2LINE_ADVANCE "\x1b[2E"
#define CURSOR_3LINE_ADVANCE "\x1b[3E"
#define CURSOR_MOVING_LEFT_MOST "\x1b[0`"		//カーソルを左端に移動
#define CURSOL_MOVING_SENTER "\x1b[40G"			//カーソルを画面の中央に移動
//#define CURSOR__BACK_ORIGIN "\x1b[0;0H"
#define PCLK (48000000)
#define BAUD_RATE (115200)
#define CKS8 (0)								//コンペアマッチタイマのPCLKの分周/8
#define CKS32 (1)								//コンペアマッチタイマのPCLKの分周/32
#define CKS128 (2)								//コンペアマッチタイマのPCLKの分周/128
#define CKS512 (3)								//コンペアマッチタイマのPCLKの分周/512
#define KEY_UP (72)								//↑キー
#define KEY_DOWN (80)							//↓キー
#define KEY_LEFT (75)							//←キー
#define KEY_RIGHT (77)							//→キー
typedef char* T_DISPLAY;
typedef const unsigned char T_SCORE;
typedef const unsigned short T_NOTE;
enum menu{ORGAN = 1,AUTOPLAY,GAME,TIMER,SETTING};//メインメニューの項目
//曲名
enum title{AVE_MARIA = 1,SAINT_MARCH,MENUETT,JESU_JOY_OF_MAN_S,AURA_LEE,SAKURA,ZYOUNETUTAIRIKU,LET_IT_BE,NHK_NODOZIMAN,DORAGON_QUEST,LEVEL_UP,
			WINNING,BOUKENNNOSYO,YADOYA,ALLY_ATACK,BATTLE1,ZENNMETU,INITIAL_CHECK,CANON,};
//波形
enum wave{SQUARE = 1,SAWTHOOTH,TRIANGLE,SINE};
enum SPEAKERS{SPEAKER1 = 1,SPEAKER2,SPEAKER3,ALL_SPEAKER};
enum setting{DUTY = 1,WAVE,};
enum{
	SW1 = 1,SW2,SW3,SW4,SW5,SW6,SW7,SW8,SW9,SW10,SW11,SW12,SW13,REST = 73,COMP = 73,NO_SELECT = 73,OFF = 73,
};

#include "score.h"
//構造体宣言
typedef struct SCI{
	unsigned char receive_data[128];
	unsigned char receive_count;
	const unsigned char *send_data[64]; //送信するデータへのポインタ
	unsigned short send_length[64];		//送信するデータの長さ
	unsigned char send_counter;			//
	unsigned char reg_cnt;				//送信するデータの登録数
	unsigned char reg_0_flg;			//登録データ0フラグ
	unsigned char send_compflg;			//送信が終了したか確認するフラグ
	unsigned char enter_flg;			//エンターが押されたか確認するためのフラグ
	unsigned long elapsed_time;			//タイマカウントエリア
}SCI;


typedef struct SPEAKER{
	float duty_value;					//デューティ比
	unsigned char wave_type;			//出力波形（スピーカー１の）
	unsigned char speaker_num;			//スピーカー番号
}SPEAKER;

#include "auto_play.h"
extern unsigned char playing_flg;
extern unsigned char mode;
/****************************************************************************/
/*プロトタイプ宣言																*/
/****************************************************************************/
struct SPEAKER *get_speaker(unsigned char speaker_num);
unsigned char input_check(void);
unsigned char input_wait(void);
#endif /* MAIN_H_ */
