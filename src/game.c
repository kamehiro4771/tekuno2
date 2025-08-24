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
#include "game.h"
#define GAME_END (11)
/********************************************************************************************/
/*プロトタイプ宣言																				*/
/********************************************************************************************/
void game_sequence(void);
/*
 * 定数定義
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
const T_DISPLAY SAVE_DATA_CREATION	= {"1:ぼうけんのしょをつくる\n"};
const T_DISPLAY LOAD_SAVE_DATA		= {"2:ぼうけんをする\n"};
const T_DISPLAY INPUT_NAME 			= {"\x1b[2J\x1b[9A　なまえ \r\n>"};
const T_DISPLAY ARRIVAL				= {"はダンジョンに到着した\n"};
const T_DISPLAY GAME_CLEAR 			= {"はダンジョンを制覇した！\r\n~~~~GAME CLEAR!~~~~\r\n"};
const T_DISPLAY GAME_OVER 			= {"~~~~GAME_OVER~~~~"};
AUTOPLAYER REPEATING_FROM_INTERMEDIATE[3] = {{SQUARE,DORAGONQUEST_SCORE1,DORAGONQUEST_NOTE_VALUE1,32,375,96,OFF,OFF},//オープニング曲を途中から
													{SQUARE,DORAGONQUEST_SCORE2,DORAGONQUEST_NOTE_VALUE2,32,375,93,OFF,OFF},
													{SQUARE,DORAGONQUEST_SCORE3,DORAGONQUEST_NOTE_VALUE3,1,500,62,OFF,OFF},
													};
/*const char map1[][]		= {""}
const char map2[][]		= {""}//
const char map3[][]		= {""}*/
/********************************************************************************************/
/*ワークエリア定義																				*/
/********************************************************************************************/
T_MONSTER enemy[5] = {{"~スライム~",100,100,WATER,10,5},{"#ゴブリン#",200,200,SOIL,20,15},
						{"@オオコウモリ@",300,300,WIND,30,25},{"@ウェアウルフ@",400,400,WIND,40,30},
						{"$ドラゴン$",800,800,FIRE,50,40},};

T_MONSTER ally[ALLY_NUM] = {{"$朱雀$",150,150,FIRE,25,10},
						{"~玄武~",150,150,WATER,20,15},
						{"@青龍@",150,150,WIND,15,10},
						{"#白虎#",150,150,SOIL,20,5},};
T_PLAYER player = {"アルス",0,0};
unsigned char sw						= OFF;
unsigned char last_sw					= 'e';
unsigned char output_num				= OFF;
unsigned char last_output_num			= OFF;
unsigned char g_sequence;//シーケンス番号
signed short party_hp;//パーティー全体のHP
/********************************************************************************************/
/*ゲームモードメイン																		*/
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
/*ゲームに使われるパラメータの初期化														*/
/*void game_param_init(void)																*/
/********************************************************************************************/
void game_param_init(void)
{
	unsigned char i;
	unsigned short temp;
	for (i = 0; i < ALLY_NUM; i++) {
		player.max_hp = player.hp += ally[i].hp;	//味方モンスターのHPの合計をプレイヤーHPの合計に設定
		temp += ally[i].gp;		//味方モンスターの防御力を合計する
	}
	player.gp = temp / ALLY_NUM;			//平均値を防御力に設定
}

/********************************************************************************************/
/*ゲーム開始処理																			*/
/*void game_atart(void)																		*/
/********************************************************************************************/
void game_start(void)
{
	game_param_init();										//プレイヤーのパラメータ初期化
	send_serial(GAME_TITLE, strlen(GAME_TITLE));			//タイトル表示
	autoplay_start_from_beginning(DORAGON_QUEST, SQUARE);					//オープニング曲を自動演奏開始
	sci0_receive_start();									//受信開始
}

/********************************************************************************************/
/*セーブデータの確認
/*void boukennnosyo_check(void)
/********************************************************************************************/
void boukennnosyo_check(void)
{
	autoplay_start_from_beginning(BOUKENNNOSYO, SQUARE);
#if 0
	if (blank_check())//セーブデータがあれば「ぼうけんをする」「ぼうけんのしょをつくる」両方表示
#endif
		send_serial(RESET, 10);
		send_serial(SAVE_DATA_CREATION, strlen(SAVE_DATA_CREATION));
		send_serial(LOAD_SAVE_DATA, strlen(LOAD_SAVE_DATA));
/*	else
		send_serial(SAVE_DATA_CREATION);*/
}

/****************************************************************************/
/*ゲームシーケンス															*/
/*void game_sequence(void)													*/
/****************************************************************************/
void game_sequence(void)
{
	unsigned char ret;
	switch(g_sequence){
	case 0:
		game_start();
		g_sequence++;
		break;
	case 1://入力待ち
		ret = input_check();
		if (ret != OFF) {
			auto_play_end_processing();
			g_sequence++;			//スイッチ又はエンターが押されたら次へ進む
		}
		else if (playing_flg == OFF)//最後まで演奏された時は途中から演奏
			autoplay_start_from_intermediate(REPEATING_FROM_INTERMEDIATE[0],REPEATING_FROM_INTERMEDIATE[1],REPEATING_FROM_INTERMEDIATE[2]);
		break;
	case 2://セーブデータを確認する
		boukennnosyo_check();
		g_sequence++;
		break;
	case 3:
		ret = input_check();
		switch(ret){
		case ON:
			if(sci0_find_received_data('1') == 1)
				g_sequence++;
			else if(sci0_find_received_data('2') == 1)
				g_sequence = 8;
			else//無効なキーボード入力があった場合再入力
				sci0_receive_start();//受信開始
			break;
		case SW1:
			g_sequence++;
			break;
		case SW2:
			g_sequence = 8;
			break;
		case OFF:
			if(playing_flg == OFF)
				g_sequence = 4;
			break;
		default:
			//無効なスイッチ入力があった場合
			break;
		}
		break;
	case 4://名前の入力促す表示
		sci0_receive_start();//受信開始
		send_serial(RESET,10);
		send_serial(INPUT_NAME, strlen(INPUT_NAME));
		g_sequence++;
		break;
	case 5:
		ret = input_check();
		if(ret == ON){
			ret = sci0_str_cpy(player.name);//入力をプレイヤーの名前に設定
			if(ret >= 3)
				g_sequence++;
			else
				sci0_receive_start();//受信開始
		}else if(playing_flg == OFF)
			autoplay_start_from_beginning(BOUKENNNOSYO,SQUARE);
		break;
	case 6:
		auto_play_end_processing();
		send_serial(player.name,strlen((const char*)player.name));
		send_serial(ARRIVAL, strlen(ARRIVAL));
		g_sequence++;
		break;
	case 7://モンスターと戦闘
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
