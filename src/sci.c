/*
 * sci.c
 *
 *  Created on: 2024/02/20
 *      Author: kameyamahiroki
 */
//sci0チャンネル使用
//調歩同期式モード
#include "main.h"

//ワークエリア定義
SCI sci0 = {{0},0,{NULL},{0},0,0,ON,ON,OFF,10};
/*
 * void sci0_init(void)
 *シリアル通信初期化関数
 *
 */
void send_data_is_exists_confirm(void);
/****************************************************/
/*										*/
/*void sci0_init(int baudrate)						*/
/****************************************************/
void sci0_init(int baudrate)
{
	MSTP(SCI0)			= 0;	//消費電力低減機能解除
	SCI0.SCR.BYTE		= 0;
	PORT2.ICR.BIT.B1	= 1;	//P21端子の入力バッファを有効にする
	SCI0.SCR.BIT.CKE	= 0;	//
	SCI0.SMR.BIT.CKS	= 0;	//クロックの分周なし
	SCI0.SMR.BIT.PM		= 0;	//偶数パリティで送受信
	SCI0.SMR.BIT.PE		= 1;	//パリティビットを付加
	SCI0.BRR			= (PCLK / (32 * baudrate)) - 1;//ボーレートを115200に設定
	SCI0.SCR.BYTE		= 0xc0;	//受信割り込み、送信割り込み許可
	cmt2_wait(600,CKS8);				//100?待機
	//割り込み設定
	IR(SCI0,TXI0)		= 0;
	IPR(SCI0,TXI0)		= 1;
	IEN(SCI0,TXI0)		= 1;
	IR(SCI0,RXI0)		= 0;
	IPR(SCI0,RXI0)		= 1;
	IEN(SCI0,RXI0)		= 1;
	IR(SCI0,TEI0)		= 0;
	IPR(SCI0,TEI0)		= 1;
	IEN(SCI0,TEI0)		= 1;
	IR(SCI0,ERI0)		= 0;
	IPR(SCI0,ERI0)		= 1;
	IEN(SCI0,ERI0)		= 1;
	interval_function_set(1,send_data_is_exists_confirm);
}
/****************************************************/
/*受信データフル割り込み（RXI）							*/
/*void sci0_received(void)							*/
/****************************************************/
void sci0_received(void)
{
	sci0.receive_data[sci0.receive_count]		= SCI0.RDR;
	if(sci0.receive_data[sci0.receive_count] == '\n'){//エンターが押されたら
		sci0.enter_flg							= ENTER_ON;//エンターフラグON
		SCI0.SCR.BIT.RE							= 0;//受信動作禁止
	}
	sci0.receive_count							= (sci0.receive_count + 1) % 128;//バッファが0～127の128文字以内にする
}
/****************************************************/
/*送信データエンプティ割り込み（TXI）				*/
/*void sci0_send_next_data(void)					*/
/****************************************************/
void sci0_send_next_data(void)
{
	sci0.send_length[sci0.send_counter]--;
	SCI0.TDR				= *(sci0.send_data[sci0.send_counter]++);
	if(sci0.send_length[sci0.send_counter] == '\0'){//ヌル文字になったら送信終了
		SCI0.SCR.BIT.TIE	= 0;//送信終了割り込み禁止
		SCI0.SCR.BIT.TEIE	= 1;//送信終了割り込み許可
	}
}

/****************************************************/
/*送信終了割り込み									*/
/*void sci0_send_comp(void)							*/
/****************************************************/
void sci0_send_comp(void)
{
	SCI0.SCR.BIT.TEIE		= 0;//送信終了割り込み禁止
	SCI0.SCR.BIT.TE			= 0;//シリアル送信動作禁止
	sci0.send_counter		= (sci0.send_counter + 1) % 64;
	sci0.send_compflg		= ON;//送信完了フラグON
	if(sci0.send_counter == sci0.reg_cnt){
		sci0.send_counter	= 0;
		sci0.reg_cnt		= 0;
		sci0.reg_0_flg		= ON;//送信登録数０
	}

}

/****************************************************/
/*送信開始											*/
/*void sci0_send_start(void)						*/
/****************************************************/
static void sci0_send_start(void)
{
	SCI0.SCR.BYTE						= 0xc0;
	SCI0.SCR.BYTE						= 0xf0;
	sci0.send_compflg 					= OFF;
}

/****************************************************/
/*受信開始											*/
/*void sci0_receive_start(void)						*/
/****************************************************/
void sci0_receive_start(void)
{
	sci0.receive_count		= 0;//受信カウントの初期化
	sci0.enter_flg			= OFF;//エンターフラグOFF
	SCI0.SCR.BIT.RE			= 1;//シリアル受信動作許可
}

/****************************************************/
/*送信が終了したか確認する							*/
/*unsigned char sci0_send_comp_check(void)					*/
/****************************************************/
unsigned char sci0_get_reg_0_flg(void)
{
	return sci0.reg_0_flg;
}



/****************************************************/
/*受信したデータを受け取った配列へいれていく		*/
/*unsigned char sci0_data_cpy(unsigned char *data)	*/
/*	引数：char *data　配列へのポインタ				*/
/*	戻り値：unsigned char バイト数					*/
/****************************************************/
unsigned char sci0_data_cpy(unsigned char *data)
{
	unsigned char i;
	for(i = 0;i < sci0.receive_count - 2;i++){//CRとLF以外をコピー
		data[i]	= sci0.receive_data[i];
	}
	return sci0.receive_count;
}
/****************************************************/
/*受信したデータを受け取った配列へいれていく（文字列にする）			*/
/*unsigned char sci0_data_cpy(unsigned char *data)	*/
/*	引数：char *data　配列へのポインタ						*/
/*	戻り値：unsigned char バイト数						*/
/****************************************************/
unsigned char sci0_str_cpy(unsigned char *data)
{
	unsigned char i;
	for(i = 0;i < sci0.receive_count - 2;i++){//CRとLF以外をコピー
		data[i]	= sci0.receive_data[i];
	}
	data[i] = '\0';
	return sci0.receive_count;
}
/****************************************************/
/*文字列へのポインタを渡す							*/
/*unsigned char *sci0_get_receive_data(void)		*/
/****************************************************/
unsigned char *sci0_get_receive_data(void)
{
	return &sci0.receive_data[0];
}
/****************************************************/
/*文字列の長さ										*/
/*unsigned char sci0_get_receive_count(void)		*/
/****************************************************/
unsigned char sci0_get_receive_count(void)
{
	return sci0.receive_count;
}
/****************************************************/
/*受信エラー割り込み								*/
/* void sci0_receive_error(void)					*/
/****************************************************/
void sci0_receive_error(void)
{
	SCI0.SSR.BIT.PER	= 0;
	SCI0.SSR.BIT.ORER	= 0;
}

/****************************************************/
/*エンターまでの文字列を数値に変換する							*/
/*signed long a_to_i(void)							*/
/*　戻り値：signed long 変換後の数値						*/
/****************************************************/
//最大値99999最小値０
//数字以外が入力されたらー１を返す
//最大値を上回る値が入力されていたらー１を返す
signed long a_to_i(void)
{
	unsigned short j = 1;
	unsigned char i;
	signed long result = 0;
	unsigned char num[5];
	if(sci0.receive_count > 7)//5桁＋CR+LFよりも多く入力されていたらー１を返す
		return -1;
	for(i = 0;sci0.receive_data[i] >= '0' && sci0.receive_data[i] <= '9';i++){
		num[i]		= sci0.receive_data[i] - '0';
	}
	if(i != sci0.receive_count - 2 || i == 0)
		return -1;//途中で文字が入力されていた時、エンターだけ押されたとき
	while(i){
		result 		+= num[i - 1] * j;
		j			*= 10;
		i--;
	}
	return result;
}

/****************************************************/
/*エンターフラグを返す								*/
/*unsigned char sci0_enter_check(void)				*/
/****************************************************/
unsigned char sci0_enter_check(void)
{
	return sci0.enter_flg;
}

/****************************************************/
/*
/*
/****************************************************/
unsigned char sci0_find_received_data(unsigned char find_char)
{
	unsigned char i;
	for(i = 0;i < sci0.receive_count - 2;i++){
		if(sci0.receive_data[i] == find_char)
			return i + 1;
	}
	return NOT_FOUND;
}

/****************************************************/
/*送信										*/
/*void send_serial(display *send_data)			*/
/****************************************************/
void send_serial(T_DISPLAY send_data,unsigned short length)
{
	sci0.send_data[sci0.reg_cnt]	= send_data;
	sci0.send_length[sci0.reg_cnt]	= length;
	sci0.reg_cnt					= (sci0.reg_cnt + 1) % 64;
	sci0.reg_0_flg					= OFF;
}

/*******************************************
 * タイマで1msごとに送信データがあるか確認する
 * 送信データが登録されていて送信中ではない場合送信開始
 * void send_data_is_exists_confirm(void)
 *******************************************/
void send_data_is_exists_confirm(void)
{
	if(sci0.reg_cnt != 0 && sci0.send_compflg == ON){
		sci0_send_start();
	}
	sci0.elapsed_time	= 1;
}

/*******************************************/
/*受信開始して入力あるまで待機*/
/**/
/*******************************************/
void sci0_scanf(void)
{
	sci0_receive_start();
	while (sci0.enter_flg == OFF) {

	}
}
