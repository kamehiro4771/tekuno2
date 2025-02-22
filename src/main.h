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
/*定数定義*/
#define ERROR (1)
#define SUCCESS (0)
#define ON (0)
#define SET (1)
#define NOT_FOUND (0)
#define AUTO_PLAY_TITLE_NUM (5)//自動演奏モードで演奏する曲数
#define MODE_NUM (4)//モード数
#define SONG_NUM (19)//全部の曲数
#define WAVE_NUM (4)//波形の種類
#define SUPEAKER_NUM (3)
#define SETTING_ITEM_NUM (3)//設定項目数
#define SPEAKER1 (0)
#define SPEAKER2 (1)
#define SPEAKER3 (2)
//ゲームで使用する
#define FIELD_NUM (13)//バトルフィールドの数
#define ONE_JEWEL (6)//一つの宝石のバイト数
#define ENEMY_NUM (5)//敵モンスターの数
#define ALLY_NUM (4)//味方の数
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
//画面、文字制御
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
//#define RESET "\x1b[2J\x1b[!p\x1b[0;0H"//画面をクリアして設定を初期化カーソルを原点に戻す
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
#define CKS8 (0)//コンペアマッチタイマのPCLKの分周/8
#define CKS32 (1)//コンペアマッチタイマのPCLKの分周/32
#define CKS128 (2)//コンペアマッチタイマのPCLKの分周/128
#define CKS512 (3)//コンペアマッチタイマのPCLKの分周/512
#define KEY_UP (72)//↑キー
#define KEY_DOWN (80)//↓キー
#define KEY_LEFT (75)//←キー
#define KEY_RIGHT (77)//→キー
enum menu{ORGAN = 1,AUTOPLAY,GAME,SETTING};//メインメニューの項目
//曲名
enum title{AVE_MARIA = 1,SAINT_MARCH,MENUETT,JESU_JOY_OF_MAN_S,AURA_LEE,SAKURA,ZYOUNETUTAIRIKU,LET_IT_BE,NHK_NODOZIMAN,DORAGON_QUEST,LEVEL_UP,
			WINNING,BOUKENNNOSYO,YADOYA,ALLY_ATACK,BATTLE1,ZENNMETU,INITIAL_CHECK,CANON,};
//波形
enum wave{SQUARE = 1,SAWTHOOTH,TRIANGLE,SINE};
enum setting{DUTY = 1,WAVE,};
enum{
	SW1 = 1,SW2,SW3,SW4,SW5,SW6,SW7,SW8,SW9,SW10,SW11,SW12,SW13,REST = 73,COMP = 73,NO_SELECT = 73,OFF = 73,
};
//構造体宣言
struct SCI{
	unsigned char receive_data[128];
	unsigned char receive_count;
	const unsigned char *send_data[64]; //送信するデータへのポインタ
	unsigned short send_length[64];		//送信するデータの長さ
	unsigned char send_counter;			//
	unsigned char reg_cnt;				//送信するデータの登録数
	unsigned char send_compflg;			//送信が終了したか確認するフラグ
	unsigned char enter_flg;			//エンターが押されたか確認するためのフラグ
	signed long elapsed_time;			//タイマカウントエリア
};
/*
 *
 */
typedef struct playing_resume_point{
			signed short start;
			long elapsed;
			}RESUME_POINT;
//elapsed_timesとcore_countを構造体にしてまとめて受け渡しできるようにする
//演奏中断ポイントをもらう時に構造体で貰えるようにする
typedef struct SPEAKER{
	const unsigned char *pscore;		//楽譜配列へのポインタ
	const unsigned short *pnote_value;	//音の長さ配列へのポインタ
	unsigned short score_count;			//スコアカウント
	unsigned long elapsed_time;			//経過時間監視エリア
	float duty_value;					//デューティ比
	unsigned char wave_type;			//出力波形（スピーカー１の）
	unsigned short note_size;			//音符の数
	unsigned char speaker_num;			//スピーカー番号0～2
	unsigned char set_flg;				//自動演奏時スコアカウントが進んだのでセットが必要であることを知らせる
	unsigned char end_flg;
}SPEAKER;

extern unsigned char playing_flg;
/****************************************************************************/
/*プロトタイプ宣言																*/
/****************************************************************************/
struct SPEAKER *get_speaker(void);

#endif /* MAIN_H_ */
