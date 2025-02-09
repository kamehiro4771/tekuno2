/********************************************************************************************/
/*ヘッダーファイル																			*/
/********************************************************************************************/
#include "main.h"
#if 1
/********************************************************************************************/
/*定数定義																					*/
/********************************************************************************************/
const unsigned char KEY_DISPLAY[] = "A	B	C	D	E	F	G	H	I	J	K	L	M\r\n";
/********************************************************************************************/
/*ワークエリア定義																			*/
/********************************************************************************************/
unsigned char g_matched_count;//一致カウント
unsigned char battle_field[FIELD_NUM];//バトルフィールド（数字が入っている）
unsigned char battle_field_display[128];//シリアル通信で表示する配列

/********************************************************************************************/
/*プロトタイプ宣言																			*/
/********************************************************************************************/
void create_send_data(void);


/********************************************************************************************/
/*入力された宝石を入力された位置に動かす													*/
/*void move_jewel(unsigned char input1,unsigned char input2)								*/
/*	引数：unsigned char input1　1番目に入力した値											*/
/*		　unsigned char input2　2番目に入力した値											*/
/********************************************************************************************/
void move_jewel(unsigned char input1,unsigned char input2)
{
	signed char difference;
	unsigned char absolute_value,temp,i;
	if(input1 <= 'Z')
		i					= input1 - 'A';//動かす宝石の位置
	else
		i					= input1 - 'a';
	difference				= input1 - input2;//一番目に入力した値と二番目に入力した値の差を求める
	if(difference > 0){
		absolute_value		= input1 - input2;
		//宝石を左にづらしていく
		while(absolute_value--){
			temp				= battle_field[i - 1];
			battle_field[i - 1]	= battle_field[i];
			battle_field[i]		= temp;
			i--;
			output_battle_field(UPDATE_FIELD);
			cmt2_wait(18750,CKS512);//200ミリ秒間隔をあける
		}
	}else{
		absolute_value = input2 - input1;
		//宝石を右にづらしていく
		while(absolute_value--){
			temp				= battle_field[i + 1];
			battle_field[i + 1]	= battle_field[i];
			battle_field[i]	= temp;
			i++;
			output_battle_field(UPDATE_FIELD);
			cmt2_wait(18750,CKS512);//200ミリ秒間隔をあける
		}
	}
}

/********************************************************************************************/
/*3つ以上揃っている宝石の数を数える															*/
/*unsigned char* count_jewel(void)															*/
/*	戻り値：unsigned char* 3つ以上一致した先頭のアドレス									*/
/********************************************************************************************/
//最初に三つ以上一致した先頭のアドレスを返す
unsigned char* count_jewel(void)
{
	unsigned char i = 0;
	while(i <= FIELD_NUM - 2){
		if(battle_field[i] == battle_field[i + 1])
			g_matched_count++;
		else{
			if(g_matched_count >= 2)
				return &battle_field[i - g_matched_count];
			else
				g_matched_count = 0;
		}
		i++;
	}
	if(g_matched_count >= 2)
		return &battle_field[i - g_matched_count];
	else
		return NULL;
}

/********************************************************************************************/
/*宝石を消す																				*/
/*void delete_jewel(unsigned char* first_address)											*/
/*	引数：unsigned char *first_address 3つ以上一致した最初のアドレス						*
/*	戻り値：unsigned char ret 消した宝石の数												*/
/********************************************************************************************/
unsigned char delete_jewel(unsigned char *first_address)
{
	unsigned char i;
	for(i = 0;i <= g_matched_count;i++){
		first_address[i]	= 5;
	}
	output_battle_field(UPDATE_FIELD);
	cmt2_wait(18750,CKS512);//200ミリ秒間隔をあける
	return g_matched_count + 1;
}

/********************************************************************************************/
/*空いた宝石配列を詰める																	*/
/*void free_padding(unsigned char* first_address)											*/
/*	引数：unsigned char *first_address 3つ以上一致した最初のアドレス						*/
/********************************************************************************************/
//タイマだと乱数にならないかも？連続して発生させないといけないため
//battle_field_displayが書き換えられるのでおかしくなる？
void free_padding(unsigned char *first_address)
{
	unsigned char i,j,moving_times;
	unsigned char *padding_address;
	for(i = 0;i <= g_matched_count;i++){//消した宝石の数繰り返す
		padding_address				= &first_address[g_matched_count - i];
		moving_times							= &battle_field[FIELD_NUM - 1] - padding_address;//宝石の移動回数を求める一番右端のアドレスとの差
		for(j = 0;j <= moving_times;j++){
			padding_address[j]					= padding_address[j + 1];
		}
		battle_field[FIELD_NUM - 1]				= random_number_acquisition(5);//右端に新しい宝石発生させる
		output_battle_field(UPDATE_FIELD);//送信データ作成
		cmt2_wait(37500,CKS512);//200ミリ秒間隔をあける
	}
	g_matched_count								= 0;
}

/********************************************************************/
/*新しいバトルフィールドを作る											*/
/*void make_battle_field(void)										*/
/********************************************************************/
void create_new_battle_field(void)
{
	unsigned char i;
	for(i = 0;i < FIELD_NUM;i++){
		battle_field[i]	= random_number_acquisition(5);//ランダムに宝石を発生させてバトルフィールドに入れる
	}
}

/********************************************************************************************/
/*battle_fieldに入っている数値をもとに、シリアル通信で送信するデータを作成する									*/
/*void create_send_data(void)																*/
/********************************************************************************************/
void create_send_data(unsigned char *battle_field_display)
{
	unsigned char i,j = 0;
	for(i = 0;i < FIELD_NUM;i++){
		battle_field_display[j++]	= '\x1b';
		battle_field_display[j++]	= '[';
		switch(battle_field[i]){
		case 0://火属性
			battle_field_display[j++]	= '3';//文字色赤
			battle_field_display[j++]	= '1';
			battle_field_display[j++]	= 'm';
			battle_field_display[j++]	= '$';
			break;
		case 1://水属性
			battle_field_display[j++]	= '3';//文字色水色
			battle_field_display[j++]	= '6';
			battle_field_display[j++]	= 'm';
			battle_field_display[j++]	= '~';
			break;
		case 2://風属性
			battle_field_display[j++]	= '3';//文字色緑
			battle_field_display[j++]	= '2';
			battle_field_display[j++]	= 'm';
			battle_field_display[j++]	= '@';
			break;
		case 3://土属性
			battle_field_display[j++]	= '3';//文字色黄色
			battle_field_display[j++]	= '3';
			battle_field_display[j++]	= 'm';
			battle_field_display[j++]	= '#';
			break;
		case 4://命属性
			battle_field_display[j++]	= '3';//文字色紫
			battle_field_display[j++]	= '5';
			battle_field_display[j++]	= 'm';
			battle_field_display[j++]	= '&';
			break;
		case 5://スペース
			battle_field_display[j++]	= '3';//色を元に戻す
			battle_field_display[j++]	= '9';
			battle_field_display[j++]	= 'm';
			battle_field_display[j++]	= ' ';
			break;
		}
		battle_field_display[j++]	= '	';
	}
	battle_field_display[j++]	= '\x1b';
	battle_field_display[j++]	= '[';
	battle_field_display[j++]	= '3';//色を元に戻す
	battle_field_display[j++]	= '9';
	battle_field_display[j++]	= 'm';
}

/*****************************************************************
 *
 *
 *****************************************************************/
void output_battle_field(unsigned char sw)
{
	send_serial(KEY_DISPLAY,strlen((const char*)KEY_DISPLAY));
	if(sw == NEW_FIELD){
		create_new_battle_field();
		create_send_data(battle_field_display);
		send_serial(battle_field_display,101);
	}else if(sw == UPDATE_FIELD){
		create_send_data(battle_field_display);
		send_serial(battle_field_display,101);
		send_serial(CURSOR_1LINE_BUCK,5);//その場で画面を更新するためにカーソルを一行上の左端に戻す
	}else
		send_serial(battle_field_display,101);
}
#endif
