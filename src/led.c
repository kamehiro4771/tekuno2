/*
 * output_led.c
 *
 *  Created on: 2023/10/22
 *      Author: kameyamahiroki
 */

#include "main.h"
/************************************************************/
/* 定数定義													*/
/************************************************************/
const unsigned char string[]	= {NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE ,
				 				   NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE ,
				   				   FON_SP,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,FON_MI,FON_DP,NONE ,
				   				   FON_0 ,FON_1 ,FON_2 ,FON_3 ,FON_4 ,FON_5 ,FON_6 ,FON_7 ,FON_8 ,FON_9 ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE ,
				   				   FON_A ,FON_B ,FON_C ,FON_D ,FON_E ,FON_F ,NONE  ,FON_H ,NONE  ,FON_J ,NONE  ,FON_L ,NONE  ,NONE  ,FON_O ,FON_P,
				   				   FON_Q ,FON_R ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,FON_Y ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,FON__ .NONE  ,
				   				   FON_A ,FON_B ,FON_C ,FON_D ,FON_E ,FON_F ,NONE  ,FON_H ,NONE  ,FON_J ,NONE  ,FON_L ,NONE  ,NONE  ,FON_O ,FON_P,
				   				   FON_Q ,FON_R ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,FON_Y ,NONE  ,};
/************************************************************/
/* ワークエリア定義											*/
/************************************************************/
long led_state;												//ポートDとポートEとポートBの点灯したときの状態
long led_blink_state;										//ポートDとポートEとポートBの消灯したときの状態（点滅指定されたLED以外はついている）
char segled_state[SEG7_DIGIT_NUM];							//ポートAの点灯したときの状態
char segled_blink_state[SEG7_DIGIT_NUM];						//ポートAの消灯したときの状態

//音階とLED色の関係　１オクターブ目赤　2オクターブ目緑　３オクターブ目青　４オクターブ目白 5オクターブ目紫 6オクターブ目黄色
/************************************************************/
/*
/*void output_led(int scale,long interval)
/*	引数：int scale:
/*		  long interval:
/************************************************************/
void output_led(int scale,long interval)
{
	if(interval != 0)
		led_blink_state	= led_state;
	switch(scale){
	case DO_1:
	case DO_SH_1:
		PORTD.DR.BIT.B0	= 1;//LED1赤点灯
		break;
	case RE_1:
		PORTD.DR.BIT.B3	= 1;//LED2赤点灯
		break;
	case RE_SH_1:
	case MI_1:
		PORTD.DR.BIT.B6 = 1;//LED3赤点灯
		break;
	case FA_1:
		PORTE.DR.BIT.B1 = 1;//LED4赤点灯
		break;
	case FA_SH_1:
	case SO_1:
		PORTE.DR.BIT.B4	= 1;//LED5赤点灯
		break;
	case SO_SH_1:
	case RA_1:
		PORTE.DR.BIT.B7 = 1;//LED6赤点灯
		break;
	case RA_SH_1:
	case SI_1:
		PORTB.DR.BIT.B2 = 1;//LED7赤点灯
		break;
	case DO_2:
	case DO_SH_2:
		PORTD.DR.BIT.B1 = 1;//LED1緑点灯
		break;
	case RE_2:
		PORTD.DR.BIT.B4	= 1;//LED2緑点灯
		break;
	case RE_SH_2:
	case MI_2:
		PORTD.DR.BIT.B7	= 1;//LED3緑点灯
		break;
	case FA_2:
		PORTE.DR.BIT.B2	= 1;//LED4緑点灯
		break;
	case FA_SH_2:
	case SO_2:
		PORTE.DR.BIT.B5 = 1;//LED5緑点灯
		break;
	case SO_SH_2:
	case RA_2:
		PORTB.DR.BIT.B0 = 1;//LED6緑点灯
		break;
	case RA_SH_2:
	case SI_2:
		PORTB.DR.BIT.B3	= 1;//LED7緑点灯
		break;
	case DO_3:
		PORTB.DR.BIT.B7 = 1;//LED8青点灯
		break;
	case DO_SH_3:
		PORTB.DR.BIT.B2 = 1;//LED1青点灯
		break;
	case RE_3:
		PORTD.DR.BIT.B5 = 1;//LED2青点灯
		break;
	case RE_SH_3:
	case MI_3:
		PORTE.DR.BIT.B0	= 1;//LED3青点灯
		break;
	case FA_3:
		PORTE.DR.BIT.B3 = 1;//LED4青点灯
		break;
	case FA_SH_3:
	case SO_3:
		PORTE.DR.BIT.B6 = 1;//LED5青点灯
		break;
	case SO_SH_3:
	case RA_3:
		PORTB.DR.BIT.B1	= 1;//LED6青点灯
		break;
	case RA_SH_3:
	case SI_3:
		PORTB.DR.BIT.B4	= 1;//LED7青点灯
		break;
	case DO_4:
		PORTB.DR.BYTE	= 0xe0;//LED8白点灯
		break;
	case DO_SH_4:
		PORTD.DR.BYTE	= 0x07;//LED1白点灯
		break;
	case RE_4:
		PORTD.DR.BYTE	= 0x38;//LED２白点灯
		break;
	case RE_SH_4:
	case MI_4:
		PORTE.DR.BIT.B0	= 1;//LED3白点灯
		PORTD.DR.BYTE	= 0xc0;
		break;
	case FA_4:
		PORTE.DR.BYTE = 0x0e;//LED4白点灯
		break;
	case FA_SH_4:
	case SO_4:
		PORTE.DR.BYTE	= 0x70;//LED5白点灯
		break;
	case SO_SH_4:
	case RA_4:
		PORTB.DR.BYTE	= 2;//LED6白点灯
		PORTE.DR.BIT.B7	= 1;
		break;
	case RA_SH_4:
	case SI_4:
		PORTB.DR.BYTE	= 0x1c;//LED7白点灯
		break;
	case DO_5:
		PORTB.DR.BYTE	= 0xa0;//LED8紫点灯
		break;
	case DO_SH_5:
		PORTD.DR.BYTE	= 0x05;//LED1紫点灯
		break;
	case RE_5:
		PORTD.DR.BYTE	= 0x28;//LED2紫点灯
		break;
	case RE_SH_5:
	case MI_5:
		PORTD.DR.BIT.B6	= 1;//LED3紫点灯
		PORTE.DR.BIT.B0	= 1;
		break;
	case FA_5:
		PORTE.DR.BYTE	= 0x0a;//LED4紫点灯
		break;
	case FA_SH_5:
	case SO_5:
		PORTE.DR.BYTE	= 0x50;//LED5紫点灯
		break;
	case SO_SH_5:
	case RA_5:
		PORTB.DR.BYTE	= 2;//LED6紫点灯
		break;
	case RA_SH_5:
	case SI_5:
		PORTB.DR.BYTE	= 0x14;//LED7紫点灯
		break;
	case DO_6:
	case DO_7:
		PORTB.DR.BYTE	= 0x60;//LED8黄色点灯
		break;
	case DO_SH_6:
		PORTD.DR.BYTE	= 0x03;//LED1黄色点灯
		break;
	case RE_6:
		PORTD.DR.BYTE	= 0x30;//LED2黄色点灯
		break;
	case RE_SH_6:
	case MI_6:
		PORTD.DR.BYTE	= 0xc0;//LED3黄色点灯
		break;
	case FA_6:
		PORTE.DR.BYTE	= 0x06;//LED4黄色点灯
		break;
	case FA_SH_6:
	case SO_6:
		PORTE.DR.BYTE	= 0x30;//LED5黄色点灯
		break;
	case SO_SH_6:
	case RA_6:
		PORTE.DR.BYTE	= 0x80;//LED6黄色点灯
		PORTB.DR.BYTE	= 0x01;
		break;
	case RA_SH_6:
	case SI_6:
		PORTB.DR.BYTE	= 0x0c;//LED7黄色点灯
		break;
	default:
		PORTD.DR.BYTE = 0;
		PORTE.DR.BYTE = 0;
		PORTB.DR.BYTE = 0;
		break;
	}
}
/******************************************************************/
/*7セグLEDに表示する											  */
/*void out_put_segled(unsigned char *display,long interval)		　*/
/*		unsigned char *value 文字列へのポインタ
/*		long interval:点滅間隔
/****************************************************/
void out_put_segled(unsigned char *display,long interval)
{
    
}

void segled_blink(void)
{
	if(PORT4.DR.BYTE == seled_blink_data)
		
	else
}
