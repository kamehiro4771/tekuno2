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
#define BUSYFLAG_LEADOUT (0x100)		//内部操作中かどうか確認する
#define WRITE_DATA (0x200)				//DDRAMに書き込む
#define READ_DATA (0x300)				//DDRAMを読む
#define BUSY_FLAG (PORT4.PORT.BIT.B7)
#define DB (PORT4.DDR.BYTE)
#define RS (PORTA.DDR.BIT.B2)
#define RW (PORTA.DDR.BIT.B1)
#define E (PORTA.DDR.BIT.B3)

 /*********************************************************/
 /*インストラクションコードを送る	　　　				  */
 /*void instruction_sending(unsigned short instruction) 　*/
 /*引数：unsigned short instruction						  */
 /*********************************************************/
void instruction_sending(unsigned short instruction)
{
	DB = (instruction & 0x00ff);
	RW = (instruction >> 8) & 1
	RS |= (instruction >> 8) & 2;
	E = 1;
	cmt2_wait(2, CKS8);//約300ns待機
	E = 0;
}

/*********************************************************/
/*LCDディスプレイ初期化関数								 */
/*void lcd_init(void)									 */
/*********************************************************/
void lcd_init(void)
{
	PORT4.DDR.BYTE	= 0xff;//ポート4を出力ポートに設定
	PORTA.DDR.BYTE	= 0x0d;//PA1,PA2，PA3を出力ポートに設定
	instruction_sending(FUNCTION_SET);//ファンクションセット1回目
	cmt2_wait(60000,CKS32);//40ms待機
	instruction_sending(FUNCTION_SET);//ファンクションセット2回目
	cmt2_wait(150, CKS32);//100μs待機
	instruction_sending(FUNCTION_SET);//ファンクションセット3回目
	while (BUSY_FLAG == 0) {
	}
	instruction_sending(FUNCTION_SET);//ファンクションセット4回目
	while (BUSY_FLAG == 0) {
	}
	instruction_sending(DISPLAY_OFF);
	while (BUSY_FLAG == 0) {
	}
	instruction_sending(CLEAR_DISPLAY);
	while (BUSY_FLAG == 0) {
	}
	instruction_sending(ENTRY_MODE_RIGHT);
	while (BUSY_FLAG == 0) {
	}
	instruction_sending(DISPLAY_ON);
	while (BUSY_FLAG == 0) {
	}
}

/*************************************************************/
/*LCDにデータを表示する										 */
/*void lcd_display(unsigned char *data,unsigned short length)*/
/*引数：*/
/**/
/*************************************************************/
void lcd_display(unsigned char *data,unsigned short length)
{
	unsigned short instruction;
	unsigned short i = 0;
	while (length) {
		instruction = WRITE_DATA + data[i++];
		instruction_sending(instruction);
		length--;
	}
}
