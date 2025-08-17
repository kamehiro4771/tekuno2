#ifndef __AUTO_PLAY
#define __AUTO_PLAY
//スピーカと自動演奏で使う変数を分ける
typedef struct AUTOPLAYER{
	unsigned char wave_type;			//出力波形（スピーカー１の）
	T_SCORE *pscore;		//楽譜配列へのポインタ
	T_NOTE *pnote_value;	//音の長さ配列へのポインタ
	unsigned short score_count;			//スコアカウント
	unsigned long elapsed_time;			//経過時間監視エリア
	unsigned short note_size;			//音符の数
	unsigned char set_flg;				//スコアカウントが進んだのでセットが必要であることを知らせる
	unsigned char end_flg;
}AUTOPLAYER;

void autoplay_start(unsigned short title,unsigned char wave_type);
AUTOPLAYER *get_autoplayer(unsigned char player_num);
AUTOPLAYER get_interrupt_data(unsigned char player_num);
#endif
