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
#define FUNCTION_SET (0x02c)//インターフェースデータ長8ビットデューティーの設定1/16に設定
#define CLEAR_DISPLAY (0x001)
#define RETURN_HOME (0x002)
#define ENTRY_MODE_RIGHT (0x006)//表示を右にシフトに設定する。表示全体はシフトしない
#define ENTRY_MODE_LEFT (0x004)//表示を左シフトに設定する。表示全体はシフトしない
#define DISPLAY_ON (0x000f)//ディスプレイONカーソルON
#define DISPLAY_OFF (0x0008)//ディスプレイOFFカーソルOFF
#define CURSOL_SHIFT_RIGHT (0x018)//カーソルを右に移動
#define CURSOL_SHIFT_LEFT (0x010)//カーソルを左に移動
#define SET_DDRAM_ADDRESS (0X080)//ディスプレイのアドレスを指定する
#define BUSYFLAG_LEADOUT (0x100)//内部操作中かどうか確認する
#define WRITE_DATA (0x200)//DDRAMに書き込む
#define READ_DATA (0x300)//DDRAMを読む

void lcd_init(void)
{
	PORT4.DDR.BYTE	= 0xff;//ポート4を出力ポートに設定
	PORTA.DDR.BYTE	= 0x0d;//PA1,PA2，PA3を出力ポートに設定
	cmt2_wait(60000,CKS32);//40ms待機
	PORT4.DR.BYTE	= FUNCTION_SET;
	cmt2_wait(56,CKS32);//37?待機
	PORT4.DR.BYTE	= DISPLAY_ON;
	cmt2_wait(56,CKS32);//37?待機
	PORT4.DR.BYTE	= CLEAR_DISPLAY;
	cmt2_wait(2280,CKS32);//1.52ms待機
	PORT4.DR.BYTE	= ENTRY_MODE_RIGHT;
}

