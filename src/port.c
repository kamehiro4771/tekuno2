

/********************************************************************/
/*ヘッダーファイル													*/
/********************************************************************/
#include "main.h"

/********************************************************************/
/*i/oポートの初期化													*/
/*void io_port_initialize(void)										*/
/********************************************************************/
void io_port_initialize(void)
{
	//LED1~8のポートの設定
	PORTD.DDR.BYTE				= 0xff;
	PORTE.DDR.BYTE				= 0xff;
	PORTB.DDR.BYTE				= 0xff;
	//7SEGLEDa~DPのポートの設定
	PORT4.DDR.BYTE				= 0xff;
	//7SEGLEDのコモン端子の設定
	PORTA.DDR.BIT.B1			= 1;
	PORTA.DDR.BIT.B2			= 1;
	PORTA.DDR.BIT.B3			= 1;
	//スイッチの設定
	PORTC.DDR.BIT.B0			= 0;
	PORTC.DDR.BIT.B1			= 0;
	PORTC.DDR.BIT.B2			= 0;
	PORTC.DDR.BIT.B3			= 0;
	PORTC.DDR.BIT.B4			= 0;
	PORTC.DDR.BIT.B5			= 0;
	PORTC.DDR.BIT.B6			= 0;
	PORTC.DDR.BIT.B7			= 0;
	PORT5.DDR.BIT.B0			= 0;
	PORT5.DDR.BIT.B1			= 0;
	PORT5.DDR.BIT.B2			= 0;
	PORT5.DDR.BIT.B3			= 0;
	PORT5.DDR.BIT.B4			= 0;
}