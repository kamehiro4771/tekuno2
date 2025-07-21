/*
 * lcd.c
 *
 *  Created on: 2024/03/27
 *      Author: kameyamahiroki
 */
/*
 * ヘッダーファイル
 */
#include "iodefine.h"
#include "main.h"
/*
 * マクロ定義
 */
#define FUNCTION_SET (0x03c)			//インターフェースデータ長8ビット,デューティーの設定1/16に設定
#define CLEAR_DISPLAY (0x001)
#define RETURN_HOME (0x002)
#define ENTRY_MODE_RIGHT (0x006)		//表示を右にシフトに設定する。表示全体はシフトしない
#define ENTRY_MODE_LEFT (0x004)			//表示を左シフトに設定する。表示全体はシフトしない
#define DISPLAY_ON (0x000f)				//ディスプレイONカーソルON
#define DISPLAY_OFF (0x0008)			//ディスプレイOFFカーソルOFF
#define CURSOL_SHIFT_RIGHT (0x018)		//カーソルを右に移動
#define CURSOL_SHIFT_LEFT (0x010)		//カーソルを左に移動
#define SET_DDRAM_ADDRESS (0X080)		//ディスプレイのアドレスを指定する
#define READ_OUT (0X100)				//インストラクションレジスタの値を読む
#define WRITE_DATA (0x200)				//DDRAMに書き込む
#define READ_DATA (0x300)				//DDRAMのデータを読む

#define BUSY_FLAG (PORT4.PORT.BIT.B7)
#define DB (PORT4.DR.BYTE)
#define RS (PORTA.DR.BIT.B2)
#define RW (PORTA.DR.BIT.B1)
#define E (PORTA.DR.BIT.B3)
#define FIRST_ROW (0x00)				//1行目の1番左のアドレス
#define SECOND_ROW (0x40)				//2行目の1番左のアドレス
#define LINE_UP_ADDR ()

#define BS (0x08)
#define TAB (0x09)
#define CR (0x0b)
#define LF (0x0a)

 /****************************************************************************/
 /* ワークエリア定義															*/
 /****************************************************************************/
unsigned char display_data[16][16];//表示データ(1行16文字×16行分)
unsigned char digit;//現在表示している桁位置
unsigned char line;//現在表示している行番号
unsigned char esc_flag = OFF;
unsigned char address_counter;



/*********************************************************/
/*ビジーフラグ読み出し									 */
/*unsigned char read_busy_flag(void)					 */
 /********************************************************/
unsigned char read_bf_ac(void)
{
	unsigned char busy;
    RS				= 0;		//コマンドモード
    RW				= 1;		//読み取りモード
    PORT4.DDR.BYTE	= 0;		//DBを入力に
    __nop();					//タイミング調整
    E				= 1;
    __nop();					//LCDがデータを出力するまで待つ
    busy			= BUSY_FLAG;//BUSYフラグ読み取り
	address_counter = DB & 0x7f;
    E				= 0;
	PORT4.DDR.BYTE	= 0xff;		//ポート4を出力ポートに設定
    return busy;
}


 /*********************************************************/
 /*インストラクションコードを送る	　　　				  */
 /*void instruction_set(unsigned short instruction) 　*/
 /*引数：unsigned short instruction						  */
 /*********************************************************/
void instruction_set(unsigned short instruction)
{
	unsigned char i;
	RS = (instruction & 0x200) ? 1 : 0;
	RW = (instruction & 0x100) ? 1 : 0;
	for (i = 0; i < 10; i++)
		__nop();
	DB = instruction & 0x00ff;
	for(i = 0;i < 10;i++)
		__nop();
	E = 1;
	for(i = 0;i < 10;i++)
		__nop();
	E = 0;
}

/******************************************************************/
/*ビジーフラグを読んで、インストラクションを送る				  */
/*void lcd_send_instruction(unsigned short instruction)			  */
/*	引数：unsigned short instruction　セットするインストラクション*/
/******************************************************************/
void lcd_send_instruction(unsigned short instruction)
{
	while (read_bf_ac() == 1){
		__nop(); 							// 軽いウェイトでポーリングを安定化
	}										//ビジーフラグが立っている間待機
	instruction_set(instruction);
	return;
}

/*************************************************************/
/*改行処理													 */
/*void lcd_line_feed(unsigned char up_or_down)*/
/*************************************************************/
//上に改行するか下に改行する
//スクロールする場合は、現在の表示データを更新する
//display_dataを見てカーソルの位置を移動させる
//digit変数の変更
//line変数の変更
void lcd_line_feed(unsigned char ch)
{
	digit = 15;
	lcd_send_instruction(READ_OUT);//現在のアドレスを取得
	if (up_or_down == LINE_UP) {
		line--;

	}
	else {
		line++;
	}
	while (display_data[line][digit] == ' ') {
		digit--;
	}
}

/***************************************************************/
/**/
/*void lcd_buck_space(void)									   */
/***************************************************************/
void lcd_buck_space(void)
{
	if(address_counter == )

	else if(address_counter == )

	else {
		lcd_send_instruction(CURSOL_SHIFT_LEFT);
		lcd_send_instruction(WRITE_DATA | ' ');
		lcd_send_instruction(CURSOL_SHIFT_LEFT);
	}

}

/***************************************************************/
/*一文字だけ出力する										   */
/*void lcd_putchar(unsigned char ch)						   */
/*引数：unsigned char ch 出力する文字						   */
/***************************************************************/
void lcd_putchar(unsigned char ch)
{
	switch (ch) {
	case BS:
		lcd_buck_space();
		break;
	case TAB:

		break;
	case CR:
		if (address_counter <= 0x0f)
			lcd_send_instruction(SET_DDRAM_ADDRESS);
		else
			lcd_send_instruction(SET_DDRAM_ADDRESS | 0x40);
		break;
	case LF:
		lcd_line_feed(ch);
		break;
	default:
		lcd_send_instruction(WRITE_DATA | ch);
		if (address_counter == 0x0f || address_counter == 0x4f)
			lcd_line_feed(ch);
		break;
	}
}
/****************************************************************/
/*LCDに文字列を出力する											*/
/*void lcd_print(const unsigned char* str,unsigned short length)*/
/**/
/**/
/***************************************************************/
void lcd_print(const unsigned char* str,unsigned short length)
{
	unsigned short i;
	lcd_send_instruction(CLEAR_DISPLAY);
	lcd_send_instruction(RETURN_HOME);
	do{
		lcd_putchar(str[i]);
	} while (address_counter != 0x00);
}

void lcd_news_ticker(void)
{

}
/*********************************************************/
/*LCDディスプレイ初期化関数								 */
/*void lcd_init(void)									 */
/*********************************************************/
void lcd_init(void)
{
	PORT4.DDR.BYTE	= 0xff;				//ポート4を出力ポートに設定
	PORTA.DDR.BYTE	|= 0x0e;				//PA1,PA2，PA3を出力ポートに設定
	cmt2_wait(22500,CKS32);	//15ms待機
	instruction_set(FUNCTION_SET);		//ファンクションセット1回目
	cmt2_wait(60000,CKS32);				//40ms待機
	instruction_set(FUNCTION_SET);		//ファンクションセット2回目
	cmt2_wait(150, CKS32);				//100μs待機
	lcd_send_instruction(FUNCTION_SET);			//ファンクションセット3回目
	lcd_send_instruction(FUNCTION_SET);			//ファンクションセット4回目
	lcd_send_instruction(DISPLAY_OFF);
	lcd_send_instruction(CLEAR_DISPLAY);
	lcd_send_instruction(ENTRY_MODE_RIGHT);
	lcd_send_instruction(DISPLAY_ON);
}



void lcd_page_ud()
{

}
/*************************************************************/
/*LCDにデータを表示する										 */
/*void lcd_display(unsigned char *data,unsigned short length)*/
/*引数：*/
/**/
/*************************************************************/
//改行する条件
/*	エンターキー入力
*	↓キー入力
*	↑キー入力
*	一番左で←キー入力
*	一番右で→キー入力
*	一番右で文字入力
*	一番左でバックスペースキー入力
* 
*/
/*
void lcd_display(unsigned char *data,unsigned short length)
{
	unsigned short instruction;
	unsigned short i = 0;
	unsigned char cursol = lcd_send_instruction(READ_OUT);//現在のカーソル位置を取得
	while (length) {
		switch (data[i]) {//改行のあるなしを判定
		case 0x0d://CR(行頭復帰)
			if (cursol == <= 0x0f)
				lcd_send_instruction(SET_DDRAM_ADDRESS);
			else
				lcd_send_instruction(SET_DDRAM_ADDRESS | 0x40);
			break;
		case 0x0a://LF(改行)
			if(line % 2) == 0)
				lcd_line_feed(LINE_DOWN);
			else
				lcd_line_feed(LINE_UP);
			break;
		case 0x08://バックスペース
			if (digit == 0)
				lcd_line_feed(LINE_UP);
			else
				digit--;
			lcd_send_instruction(CURSOL_SHIFT_LEFT);
			break;
		case 0x1b://エスケープシーケンス開始

			break;
		case '':
			break;
		case '':
			break;
		case '':
			break;
		default:
			instruction = WRITE_DATA + data[i++];
			lcd_send_instruction(instruction);
			digit++;
			if (digit == 15)
				lcd_line_feed();
			break;
		}
		length--;
	}
}
*/
