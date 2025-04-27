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
void auto_play_end_processing(void);
void score_set_speaker(int title,unsigned char wave_type,unsigned short start1,unsigned short start2,unsigned short start3);
void forward_score(void);
void output_function_call(void);

/*
 * ワークエリア定義
 */
AUTOPLAYER autoplayer[SPEAKER_NUM] = {{SQUARE,NULL,NULL,0,0,0,OFF,OFF},{SQUARE,NULL,NULL,0,0,0,OFF,OFF},{SQUARE,NULL,NULL,0,0,0,OFF,OFF},};
AUTOPLAYER interrupt_data[3];//エンターやスイッチで演奏終了したときのスピーカの情報を保存しておく
unsigned char g_use_speaker_num;		//使用するスピーカーの個数
unsigned char playing_flg = OFF;

/*
 * 自動演奏開始関数
 *
 */
//演奏中は演奏開始出来ないようにする
unsigned char automatic_playing_start(unsigned short title,unsigned char wave_type,unsigned short start1,unsigned short start2,unsigned short start3)
{
	if(playing_flg == OFF){
		sci0_receive_start();//受信開始
		playing_flg						= ON;//演奏中フラグON
		score_set_speaker(title,wave_type,start1,start2,start3);//楽譜をセットする、曲の開始位置のセット
		return ON;
	}
	return OFF;
}

/********************************************************************/
/*時間経過で楽譜を進めるコールバック関数										*/
/*void forward_score(unsigned char i)								*/
/*	引数：unsigned char i　スピーカー番号									*/
/********************************************************************/
//経過時間をダウンカウント
//経過時間を見て０ならスコアカウントを進める
//スコアカウントが楽譜配列のサイズと同じになったら終了フラグをONにしてスピーカを消音する
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
		set_output_speaker_length(output_pattern);
		output_speaker_start(output_pattern);//出力開始
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


/********************************************************************/
/*自動演奏終了処理													*/
/*void auto_play_end_processing(void)								*/
/********************************************************************/
void auto_play_end_processing(void)
{
	if(playing_flg == OFF)						//演奏中出ない時に呼び出されたら何もしない
		return;
	g_use_speaker_num	= 0;
	output_led(REST,BLACK,0);					//LED消灯
	mute(ALL_SPEAKER);							//スピーカーi消音
	interrupt_data[0]			= autoplayer[0];//演奏中断データを保存
	interrupt_data[1]			= autoplayer[1];
	interrupt_data[2]			= autoplayer[2];
	count_timer_dell(forward_score);
	count_timer_dell(output_function_call);
	count_timer_dell(end_flg_check);
	autoplayer[0].elapsed_time	= 0;
	autoplayer[1].elapsed_time	= 0;
	autoplayer[2].elapsed_time	= 0;
	autoplayer[0].end_flg		= OFF;
	autoplayer[1].end_flg		= OFF;
	autoplayer[2].end_flg		= OFF;
	playing_flg					= OFF;			//演奏中フラグOFF
}

/********************************************************************************************/
/*楽譜,演奏開始位置,波形をスピーカーにセットする											  				*/
/*void void score_set_speaker(int title,unsigned short start_point,unsigned char )	*/
/*	引数：int title タイトル番号													  				*/
/*		unsigned short start_point 演奏開始する添え字番号							  				*/
/*		unsigned char  出力波形										  				*/
/********************************************************************************************/
void score_set_speaker(int title,unsigned char wave_type,unsigned short start1,unsigned short start2,unsigned short start3)
{
	unsigned char i;
	autoplayer[0].score_count 		= start1;
	autoplayer[1].score_count 		= start2;
	autoplayer[2].score_count 		= start3;//演奏開始位置の設定
	autoplayer[0].wave_type			= wave_type;
	autoplayer[0].pscore			= SCORE_POINTER_ARRAY[title-1][0];
	autoplayer[1].pscore			= SCORE_POINTER_ARRAY[title-1][1];
	autoplayer[2].pscore			= SCORE_POINTER_ARRAY[title-1][2];
	autoplayer[0].pnote_value		= NOTE_POINTER_ARRAY[title-1][0];
	autoplayer[1].pnote_value		= NOTE_POINTER_ARRAY[title-1][1];
	autoplayer[2].pnote_value		= NOTE_POINTER_ARRAY[title-1][2];
	autoplayer[0].note_size			= NOTE_SIZE_ARRAY[title-1][0];
	autoplayer[1].note_size			= NOTE_SIZE_ARRAY[title-1][1];
	autoplayer[2].note_size			= NOTE_SIZE_ARRAY[title-1][2];
	g_use_speaker_num				= USE_SPEAKER_ARRAY[title-1];
	if(start1 == 0 && start2 == 0 && start3 == 0){
		switch(g_use_speaker_num){
		case 1:
			autoplayer[0].set_flg	= ON;
			autoplayer[1].end_flg	= autoplayer[2].end_flg = ON;
			break;
		case 2:
			autoplayer[0].set_flg 	= autoplayer[1].set_flg = ON;
			autoplayer[2].end_flg	= ON;
			break;
		case 3:
			autoplayer[0].set_flg 	= autoplayer[1].set_flg = autoplayer[2].set_flg		= ON;
			break;
		}
	}else{//途中から演奏する時は経過時間の更新をされたくないためset_flgをOFFでスタートする
		for(i = 0;i < g_use_speaker_num;i++){
			set_output_value(autoplayer[i].pscore[autoplayer[i].score_count],i + 1);
		}
		output_speaker_start(7);//出力開始

	}
	interval_function_set(1,output_function_call);//出力関数の登録
	interval_function_set(1,forward_score);//経過時間監視のエリアを登録
	interval_function_set(1,end_flg_check);
}

AUTOPLAYER *get_autoplayer(void)
{
	return &autoplayer;
}
/*
 * 自動演奏を中断したときのデータをもらう
 * struct SPEAKER get_interrupt_data(speaker_num)
 *
 *
 */
AUTOPLAYER get_interrupt_data(unsigned char player_num)
{
	return interrupt_data[player_num];
}
