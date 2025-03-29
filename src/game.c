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
#define GAME_END (11)
/********************************************************************************************/
/*プロトタイプ宣言																				*/
/********************************************************************************************/
void game_sequence(AUTOPLAYER *pautoplayer);
/*
 * 定数定義
 */
const T_DISPLAY GAME_TITLE[] = {"\x1b[2J\x1b[47A\x1b[44m\x1b[31m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
						   "~                                          ~\n"
						   "~               \x1b[37meNeiro QUEST\x1b[31m               ~\n"
						   "~                                          ~\n"
		                   "~                 \x1b[37mSTART:SW1\x1b[31m                ~\n"
						   "~                 \x1b[37mCONTINUE:SW2\x1b[31m             ~\n"
						   "~                                          ~\n"
		                   "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\x1b[m\n"
							};
const T_DISPLAY SAVE_DATA_CREATION[]	= {"1:ぼうけんのしょをつくる\n"};
const T_DISPLAY LOAD_SAVE_DATA[]		= {"2:ぼうけんをする\n"};
const T_DISPLAY INPUT_NAME[] 			= {"\x1b[2J\x1b[9A　なまえ \r\n>"};
const T_DISPLAY ARRIVAL[]				= {"はダンジョンに到着した\n"};
const T_DISPLAY GAME_CLEAR[] 			= {"はダンジョンを制覇した！\r\n~~~~GAME CLEAR!~~~~\r\n"};
const T_DISPLAY GAME_OVER[] 			= {"~~~~GAME_OVER~~~~"};
AUTOPLAYER REPEATING_FROM_INTERMEDIATE[3] = {{SQUARE,DORAGONQUEST_SCORE1,DORAGONQUEST_NOTE_VALUE1,32,375,96,OFF,OFF},//オープニング曲を途中から
													{SQUARE,DORAGONQUEST_SCORE2,DORAGONQUEST_NOTE_VALUE2,32,375,93,OFF,OFF},
													{SQUARE,DORAGONQUEST_SCORE3,DORAGONQUEST_NOTE_VALUE3,1,500,62,OFF,OFF},
													};
/*const char map1[][]		= {""}
const char map2[][]		= {""}
const char map3[][]		= {""}*/
/********************************************************************************************/
/*ワークエリア定義																				*/
/********************************************************************************************/
T_ENEMY enemy[ENEMY_NUM] = {{"~スライム~",100,100,WATER,10,5},{"#ゴブリン#",200,200,SOIL,20,15},
						{"@オオコウモリ@",300,300,WIND,30,25},{"@ウェアウルフ@",400,400,WIND,40,30},
						{"$ドラゴン$",800,800,FIRE,50,40},};

T_ALLY ally[ALLY_NUM] = {{"$朱雀$",150,FIRE,25,10},
						{"~玄武~",150,WATER,20,15},
						{"@青龍@",150,WIND,15,10},
						{"#白虎#",150,SOIL,20,5},};
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
	AUTOPLAYER *pautoplayer = get_autoplayer();
	while(g_sequence != GAME_END){
		game_sequence(pautoplayer);
	}
	send_serial(RESET,4);
	g_sequence = 0;
}

/****************************************************************************/
/*ゲームシーケンス																*/
/*void game_sequence(void)													*/
/****************************************************************************/
void game_sequence(AUTOPLAYER *pautoplayer)
{
	unsigned char i,ret;
	switch(g_sequence){
	case 0://タイトル表示、プレイヤーのパラメータ初期化
		for(i = 0;i < ALLY_NUM;i++){
			player.mhp = player.hp += ally[i].hp;//味方モンスターのHPの合計がプレイヤーHP
			player.gp = player.gp + ally[i].gp;
		}
//		player.hp = 1;
		player.gp = player.gp / ALLY_NUM;//味方防御力の平均を防御力に設定
		send_serial(GAME_TITLE,sizeof(GAME_TITLE));//タイトル表示
		g_sequence++;//シーケンス番号＋１
		pautoplayer[0].score_count = pautoplayer[1].score_count = pautoplayer[2].score_count = 0;
		break;
	case 1:
		automatic_playing_start(DORAGON_QUEST,SQUARE,pautoplayer[0].score_count,pautoplayer[1].score_count,pautoplayer[2].score_count);//オープニング曲を自動演奏
		g_sequence++;
	case 2:
		ret	= input_check();
		if(ret != OFF){
			pautoplayer[0].end_flg = pautoplayer[1].end_flg = pautoplayer[2].end_flg = ON;
			g_sequence++;//スイッチ又はエンターが押された
		}else if(playing_flg == OFF){//最後まで演奏された時は途中から演奏
			pautoplayer[0]	= REPEATING_FROM_INTERMEDIATE[0];
			pautoplayer[1]	= REPEATING_FROM_INTERMEDIATE[1];
			pautoplayer[2]	= REPEATING_FROM_INTERMEDIATE[2];
			g_sequence					= 1;
		}
		break;
	case 3://画面表示切り替え
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
	case 4://冒険の書自動演奏開始
		ret = automatic_playing_start(BOUKENNNOSYO,SQUARE,0,0,0);
		if(ret == ON)
			g_sequence++;
		break;
	case 5:
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
	case 6://名前の入力促す表示
		sci0_receive_start();//受信開始
		send_serial(RESET,10);
		send_serial(INPUT_NAME,sizeof(INPUT_NAME));
		g_sequence++;
		break;
	case 7:
		ret = input_check();
		if(ret == ON){
			ret = sci0_str_cpy(player.name);//入力をプレイヤーの名前に設定
			if(ret >= 3)
				g_sequence = 8;
			else
				sci0_receive_start();//受信開始
		}else if(playing_flg == OFF)
			automatic_playing_start(BOUKENNNOSYO,SQUARE,0,0,0);
		break;
	case 8:
		auto_play_end_processing();
		send_serial(player.name,strlen((const char*)player.name));
		send_serial(ARRIVAL,sizeof(ARRIVAL));
		g_sequence++;
		break;
	case 9:
		for(i = 0;i < ENEMY_NUM;i++){
			ret = battle_main(&player,&ally,&enemy[i]);
			バトルが終わったら1秒間隔をあけるようにする
			if(ret == LOSE){
				g_sequence =11;
				break;
			}
		}
		g_sequence++;
		break;
	case 10:
		send_serial(player.name,sizeof(player.name));
		send_serial(GAME_CLEAR,sizeof(GAME_CLEAR));
		g_sequence	= GAME_END;
		break;
	default:
		send_serial(GAME_OVER,sizeof(GAME_OVER));
		automatic_playing_start(ZENNMETU,SQUARE,0,0,0);
		while(playing_flg == ON){

		}
		g_sequence = GAME_END;
	}
}

T_ALLY get_ally_data(unsigned char ally_num)
{
	return ally[ally_num];
}
