/*
 * auto_play.c
/*
 *
 *  Created on: 2023/12/30
 *      Author: kameyamahiroki
 */
/********************************************************************/
/*ヘッダーファイル													*/
/********************************************************************/
#include "main.h"



/********************************************************************/
/*プロトタイプ宣言													*/
/********************************************************************/
void forward_score(void);
void output_function_call(void);
void end_flg_check(void);
/*
 * ワークエリア定義
 */
AUTOPLAYER autoplayer[SPEAKER_NUM] = {{SQUARE,NULL,NULL,0,0,0,OFF,ON},{SQUARE,NULL,NULL,0,0,0,OFF,ON},{SQUARE,NULL,NULL,0,0,0,OFF,ON},};
unsigned char g_use_speaker_num;		//使用するスピーカーの個数
unsigned char playing_flg = OFF;

/********************************************************************/
/*自動演奏する機能をシステムタイマに登録する						*/
/*void autoplay_function_set(void)									*/
/********************************************************************/
void autoplay_function_set(void)
{
	interval_function_set(1, output_function_call);//出力関数の登録
	interval_function_set(1, forward_score);//経過時間監視のエリアを登録
	interval_function_set(1, end_flg_check);
}

/********************************************************************/
/*自動演奏する楽譜、波形を
/*
/********************************************************************/
void autoplayer_set(unsigned short title,unsigned char wave_type)
{
	autoplayer[0].score_count	= 0;
	autoplayer[1].score_count	= 0;
	autoplayer[2].score_count	= 0;
	autoplayer[0].pscore = SCORE_POINTER_ARRAY[title - 1][0];
	autoplayer[1].pscore = SCORE_POINTER_ARRAY[title - 1][1];
	autoplayer[2].pscore = SCORE_POINTER_ARRAY[title - 1][2];
	autoplayer[0].pnote_value = NOTE_POINTER_ARRAY[title - 1][0];
	autoplayer[1].pnote_value = NOTE_POINTER_ARRAY[title - 1][1];
	autoplayer[2].pnote_value = NOTE_POINTER_ARRAY[title - 1][2];
	autoplayer[0].note_size = NOTE_SIZE_ARRAY[title - 1][0];
	autoplayer[1].note_size = NOTE_SIZE_ARRAY[title - 1][1];
	autoplayer[2].note_size = NOTE_SIZE_ARRAY[title - 1][2];
	g_use_speaker_num = USE_SPEAKER_ARRAY[title - 1];
	autoplayer[0].wave_type 	= wave_type;
	switch (g_use_speaker_num) {
	case 1:
		autoplayer[0].set_flg 	= ON;
		autoplayer[0].end_flg 	= OFF;
		autoplayer[1].end_flg = autoplayer[2].end_flg	= ON;
		break;
	case 2:
		autoplayer[0].set_flg 	= autoplayer[1].set_flg = ON;
		autoplayer[0].end_flg 	= autoplayer[1].end_flg	= OFF;
		autoplayer[2].end_flg	= ON;
		break;
	case 3:
		autoplayer[0].set_flg 	= autoplayer[1].set_flg = autoplayer[2].set_flg = ON;
		autoplayer[0].end_flg 	= autoplayer[1].end_flg = autoplayer[2].end_flg	= OFF;
		break;
	}
}

/********************************************************************/
/*自動演奏終了処理													*/
/*void auto_play_end_processing(void)								*/
/********************************************************************/
void auto_play_end_processing(void)
{
	if (playing_flg == OFF)						//演奏中出ない時に呼び出されたら何もしない
		return;
	led_lights_out();							//LED消灯
	mute(ALL_SPEAKER);							//スピーカー消音
	count_timer_dell(forward_score);
	count_timer_dell(output_function_call);
	count_timer_dell(end_flg_check);
	playing_flg = OFF;							//演奏中フラグOFF
}
/******************************************************************************************************************************************************/
/*初めから演奏する
/*unsigned char automatic_playing_start(unsigned short title,unsigned char wave_type)*/
/*	引数 unsigned short title*/
/*		 unsigned char wave_type*/
/******************************************************************************************************************************************************/
void autoplay_start_from_beginning(unsigned short title,unsigned char wave_type)
{
	auto_play_end_processing();
	playing_flg						= ON;	//演奏中フラグON
	autoplayer_set(title,wave_type);
	autoplay_function_set();				//自動演奏に必要な関数をシステムタイマに登録する
}
void autoplay_data_set(AUTOPLAYER set1, AUTOPLAYER set2, AUTOPLAYER set3)
{
	autoplayer[0] = set1;
	autoplayer[1] = set2;
	autoplayer[2] = set3;
}
/******************************************************************************************/
/*途中から演奏する																		  */
/*void autoplay_start_from_beginning_from_intermediate(AUTOPLAYER set1, AUTOPLAYER set2, AUTOPLAYER set3)*/
/**/
/*********************************************/
void autoplay_start_from_intermediate(AUTOPLAYER set1, AUTOPLAYER set2, AUTOPLAYER set3)
{
	playing_flg						= ON;	//演奏中フラグON
	autoplay_data_set(set1, set2, set3);	//中断データをセットする
	output_speaker_start(7);				//出力開始
	autoplay_function_set();
}

/********************************************************************/
/*時間経過で楽譜を進めるコールバック関数										*/
/*void forward_score(void)								*/
/********************************************************************/
//経過時間をダウンカウント
//経過時間を見て０ならスコアカウントを進める
//スコアカウントが楽譜配列のサイズと同じになったら終了フラグをONにする
//スコアカウントが楽譜配列のサイズより小さい場合セットフラグをONにする
void forward_score(void)
{
	unsigned char i;
	for(i = 0;i < g_use_speaker_num;i++){
		autoplayer[i].elapsed_time--;
		if(autoplayer[i].elapsed_time == 0){
			autoplayer[i].score_count++;//音符番号を進める
			if(autoplayer[i].score_count >= autoplayer[i].note_size){//最後まで演奏したら
				autoplayer[i].end_flg	= ON;
			}
			else
				autoplayer[i].set_flg	= ON;
		}
	}
}

/********************************************************************************************/
/*LEDとスピーカ出力																			*/
/*void output_function_call(void)															*/
/********************************************************************************************/
void output_function_call(void)
{
	unsigned char i;
	unsigned char output_pattern = 0;
	for(i = 0;i < g_use_speaker_num;i++){
		if(autoplayer[i].set_flg == ON){//スコアカウントが動いたか判定
			set_output_value(autoplayer[i].pscore[autoplayer[i].score_count],i+1);
			autoplayer[i].set_flg	= OFF;
			output_pattern				+= SET << i;
		}
	}
	if(output_pattern != 0){
		output_speaker_start(output_pattern);
	}
}

/*
 * 演奏終了判定
 *void end_decision(void)
 */
//全てのエンドフラグを見てONなら自動演奏終了関数を呼び出す
void end_flg_check(void)
{
	if(autoplayer[0].end_flg ==  ON && autoplayer[1].end_flg == ON && autoplayer[2].end_flg == ON){//演奏が最後まで終わったか判定
		auto_play_end_processing();//タイマーストップ、変数初期化
	}
}



AUTOPLAYER *get_autoplayer(unsigned char player_num)
{
	return &autoplayer[player_num];
}


AUTOPLAYER get_interrupt_data(unsigned char player_num)
{
	return autoplayer[player_num];
}

