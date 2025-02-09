/*
 * game.c
 *
 *  Created on: 2024/05/26
 *      Author: kameyamahiroki
 */
/*
 * ヘッダーファイル
 */

#include "main.h"

/********************************************************************************************/
/*プロトタイプ宣言																				*/
/********************************************************************************************/
void game_sequence(void);
/*
 * 定数定義
 */
const unsigned char GAME_TITLE[] = {"\x1b[2J\x1b[47A\x1b[44m\x1b[31m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
						   "~                                          ~\n"
						   "~               \x1b[37meNeiro QUEST\x1b[31m               ~\n"
						   "~                                          ~\n"
		                   "~                 \x1b[37mSTART:SW1\x1b[31m                ~\n"
						   "~                 \x1b[37mCONTINUE:SW2\x1b[31m             ~\n"
						   "~                                          ~\n"
		                   "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\x1b[m\n"
							};
const unsigned char SAVE_DATA_CREATION[]	= {"0:ぼうけんのしょをつくる\n"};
const unsigned char LOAD_SAVE_DATA[]		= {"1:ぼうけんをする\n"};
const unsigned char INPUT_NAME[] 			= {"\x1b[2J\x1b[9A　なまえ \r\n>"};
const unsigned char ARRIVAL[]				= {"はダンジョンに到着した\n"};
const unsigned char GAME_CLEAR[] 			= {"はダンジョンを制覇した！\r\n~~~~GAME CLEAR!~~~~\r\n"};
const unsigned char GAME_OVER[] 			= {"~~~~GAME_OVER~~~~"};

/*const char map1[][]		= {""}
const char map2[][]		= {""}
const char map3[][]		= {""}*/
/********************************************************************************************/
/*ワークエリア定義																				*/
/********************************************************************************************/
Enemy enemy[ENEMY_NUM] = {{"スライム",100,100,WATER,10,5},{"ゴブリン",200,200,SOIL,20,15},
						{"オオコウモリ",300,300,WIND,30,25},{"ウェアウルフ",400,400,WIND,40,30},
						{"ドラゴン",800,800,FIRE,50,40},};

Ally ally[ALLY_NUM] = {{"\x1b[31m$朱雀$",150,FIRE,25,10},
						{"\x1b[35m~玄武~",150,WATER,20,15},
						{"\x1b[34m@青龍@",150,WIND,15,10},
						{"\x1b[33m#白虎#",150,SOIL,20,5},};
Player player = {"アルス",0,0};
unsigned char sw						= OFF;
unsigned char last_sw					= 'e';
unsigned char output_num				= OFF;
unsigned char last_output_num			= OFF;
unsigned char g_sequence;//シーケンス番号
struct SPEAKER *G_speaker;
signed short party_hp;//パーティー全体のHP
/********************************************************************************************/
/*ゲームモードメイン																		*/
/*void game_mode(void)																		*/
/********************************************************************************************/
void game_mode(void)
{
	unsigned char i;
	G_speaker = get_speaker();
	for(i = 0;i < ALLY_NUM;i++)
		player.mhp = player.hp = ally[i].hp;//味方モンスターのHPの合計がプレイヤーHP
	while(g_sequence != 8){
		game_sequence();
	}
	send_serial(RESET,4);
	g_sequence = 0;
}

/****************************************************************************/
/*ゲームシーケンス																*/
/*void game_sequence(void)													*/
/****************************************************************************/
void game_sequence(void)
{
	unsigned char i,ret;
	switch(g_sequence){
	case 0://タイトル表示
		send_serial(GAME_TITLE,sizeof(GAME_TITLE));//タイトル表示
		g_sequence++;//シーケンス番号＋１
		G_speaker[0].score_count = G_speaker[1].score_count = G_speaker[2].score_count = 0;
		break;
	case 1:
		automatic_playing(DORAGON_QUEST,SQUARE,G_speaker[0].score_count,G_speaker[1].score_count,G_speaker[2].score_count);//オープニング曲を自動演奏
		while(playing_flg == ON){
			//nop
		}
		if(sci0_enter_check() == ON)
			g_sequence++;//スイッチ又はエンターが押された
		else{
			G_speaker[0].score_count 	= G_speaker[1].score_count = 33;//途中から演奏
			G_speaker[2].score_count 	= 1;
			G_speaker[0].set_flg 		= ON;
			G_speaker[1].set_flg		= ON;
			G_speaker[2].elapsed_time = 4500;
		}
		break;
	case 2://セーブデータ確認
#if 0
		if(blank_check())//セーブデータがあれば「ぼうけんをする」「ぼうけんのしょをつくる」両方表示
#endif
			send_serial(RESET,10);
			send_serial(SAVE_DATA_CREATION,sizeof(SAVE_DATA_CREATION));
			send_serial(LOAD_SAVE_DATA,sizeof(LOAD_SAVE_DATA));
/*		else
			send_serial(SAVE_DATA_CREATION);*/
		g_sequence++;
		break;
	case 3://冒険の書自動演奏
		automatic_playing(BOUKENNNOSYO,SQUARE,0,0,0);
		while(playing_flg == ON){
			if(sci0_find_received_data('0') == 1)
				g_sequence++;
			else if(sci0_find_received_data('1') == 1)
				g_sequence = 6;
		}
		if(g_sequence != 3)
			send_serial(RESET,10);
		break;
	case 4://「名前を入力してください」表示
		automatic_playing(BOUKENNNOSYO,SQUARE,0,0,0);
		send_serial(INPUT_NAME,sizeof(INPUT_NAME));
		g_sequence++;
		break;
	case 5:
		automatic_playing(BOUKENNNOSYO,SQUARE,G_speaker[0].score_count,G_speaker[1].score_count,G_speaker[2].score_count);
		if(sci0_enter_check() == ON){
			ret = sci0_str_cpy(player.name);
			if(ret >= 3)
				g_sequence = 8;
		}
		break;
	case 6:
		send_serial(player.name,strlen((const char*)player.name));
		send_serial(ARRIVAL,sizeof(ARRIVAL));
		g_sequence++;
		break;
	case 7:
		for(i = 0;i < ENEMY_NUM;i++){
			ret = battle_main(&player,&enemy[i]);
			if(ret == LOSE){
				g_sequence = 9;
				break;
			}
			automatic_playing(WINNING,SQUARE,0,0,0);
		}
		g_sequence++;
		break;
	case 8:
		send_serial(player.name,sizeof(player.name));
		send_serial(GAME_CLEAR,sizeof(GAME_CLEAR));
		g_sequence++;
		break;
	default:
		send_serial(GAME_OVER,sizeof(GAME_OVER));
		automatic_playing(ZENNMETU,SQUARE,0,0,0);
	}
}

/****************************************************************************/
/*敵情報取得																	*/
/*struct Enemy* get_enemy_data(unsigned char num)							*/
/****************************************************************************/
struct Enemy* get_enemy_data(unsigned char num)
{
	return &enemy[num];
}

/****************************************************************************/
/*味方モンスター情報取得													*/
/*struct Ally* get_ally_data(unsigned char type)							*/
/*	引数：unsigned char type モンスターの属性								*/
/*	戻り値：struct Ally* モンスターへのポインタ								*/
/****************************************************************************/
struct Ally* get_ally_data(unsigned char type)
{
	return &ally[type];
}

