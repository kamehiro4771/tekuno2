/*
 * output_led.c
 *
 *  Created on: 2023/10/22
 *      Author: kameyamahiroki
 */

#include "main.h"
/************************************************************/
/* θθ`													*/
/************************************************************/
const unsigned char string[]	= {NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE ,
								   NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE ,
								   FON_SP,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,FON_MI,FON_DP,NONE ,
								   FON_0 ,FON_1 ,FON_2 ,FON_3 ,FON_4 ,FON_5 ,FON_6 ,FON_7 ,FON_8 ,FON_9 ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE ,
								   FON_A ,FON_B ,FON_C ,FON_D ,FON_E ,FON_F ,NONE  ,FON_H ,NONE  ,FON_J ,NONE  ,FON_L ,NONE  ,NONE  ,FON_O ,FON_P,
								   FON_Q ,FON_R ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,FON_Y ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,FON__ .NONE  ,
								   FON_A ,FON_B ,FON_C ,FON_D ,FON_E ,FON_F ,NONE  ,FON_H ,NONE  ,FON_J ,NONE  ,FON_L ,NONE  ,NONE  ,FON_O ,FON_P,
								   FON_Q ,FON_R ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,FON_Y ,NONE  ,};

//PIN^[uΪΤ@2IN^[uΪΞ@RIN^[uΪΒ@SIN^[uΪ 5IN^[uΪ 6IN^[uΪ©F
void output_led(int scale,long interval)
{
	switch(scale){
	case DO_1:
	case DO_SH_1:
		PORTD.DR.BIT.B0	= 1;//LED1Τ_
		break;
	case RE_1:
		PORTD.DR.BIT.B3	= 1;//LED2Τ_
		break;
	case RE_SH_1:
	case MI_1:
		PORTD.DR.BIT.B6 = 1;//LED3Τ_
		break;
	case FA_1:
		PORTE.DR.BIT.B1 = 1;//LED4Τ_
		break;
	case FA_SH_1:
	case SO_1:
		PORTE.DR.BIT.B4	= 1;//LED5Τ_
		break;
	case SO_SH_1:
	case RA_1:
		PORTE.DR.BIT.B7 = 1;//LED6Τ_
		break;
	case RA_SH_1:
	case SI_1:
		PORTB.DR.BIT.B2 = 1;//LED7Τ_
		break;
	case DO_2:
	case DO_SH_2:
		PORTD.DR.BIT.B1 = 1;//LED1Ξ_
		break;
	case RE_2:
		PORTD.DR.BIT.B4	= 1;//LED2Ξ_
		break;
	case RE_SH_2:
	case MI_2:
		PORTD.DR.BIT.B7	= 1;//LED3Ξ_
		break;
	case FA_2:
		PORTE.DR.BIT.B2	= 1;//LED4Ξ_
		break;
	case FA_SH_2:
	case SO_2:
		PORTE.DR.BIT.B5 = 1;//LED5Ξ_
		break;
	case SO_SH_2:
	case RA_2:
		PORTB.DR.BIT.B0 = 1;//LED6Ξ_
		break;
	case RA_SH_2:
	case SI_2:
		PORTB.DR.BIT.B3	= 1;//LED7Ξ_
		break;
	case DO_3:
		PORTB.DR.BIT.B7 = 1;//LED8Β_
		break;
	case DO_SH_3:
		PORTB.DR.BIT.B2 = 1;//LED1Β_
		break;
	case RE_3:
		PORTD.DR.BIT.B5 = 1;//LED2Β_
		break;
	case RE_SH_3:
	case MI_3:
		PORTE.DR.BIT.B0	= 1;//LED3Β_
		break;
	case FA_3:
		PORTE.DR.BIT.B3 = 1;//LED4Β_
		break;
	case FA_SH_3:
	case SO_3:
		PORTE.DR.BIT.B6 = 1;//LED5Β_
		break;
	case SO_SH_3:
	case RA_3:
		PORTB.DR.BIT.B1	= 1;//LED6Β_
		break;
	case RA_SH_3:
	case SI_3:
		PORTB.DR.BIT.B4	= 1;//LED7Β_
		break;
	case DO_4:
		PORTB.DR.BYTE	= 0xe0;//LED8_
		break;
	case DO_SH_4:
		PORTD.DR.BYTE	= 0x07;//LED1_
		break;
	case RE_4:
		PORTD.DR.BYTE	= 0x38;//LEDQ_
		break;
	case RE_SH_4:
	case MI_4:
		PORTE.DR.BIT.B0	= 1;//LED3_
		PORTD.DR.BYTE	= 0xc0;
		break;
	case FA_4:
		PORTE.DR.BYTE = 0x0e;//LED4_
		break;
	case FA_SH_4:
	case SO_4:
		PORTE.DR.BYTE	= 0x70;//LED5_
		break;
	case SO_SH_4:
	case RA_4:
		PORTB.DR.BYTE	= 2;//LED6_
		PORTE.DR.BIT.B7	= 1;
		break;
	case RA_SH_4:
	case SI_4:
		PORTB.DR.BYTE	= 0x1c;//LED7_
		break;
	case DO_5:
		PORTB.DR.BYTE	= 0xa0;//LED8_
		break;
	case DO_SH_5:
		PORTD.DR.BYTE	= 0x05;//LED1_
		break;
	case RE_5:
		PORTD.DR.BYTE	= 0x28;//LED2_
		break;
	case RE_SH_5:
	case MI_5:
		PORTD.DR.BIT.B6	= 1;//LED3_
		PORTE.DR.BIT.B0	= 1;
		break;
	case FA_5:
		PORTE.DR.BYTE	= 0x0a;//LED4_
		break;
	case FA_SH_5:
	case SO_5:
		PORTE.DR.BYTE	= 0x50;//LED5_
		break;
	case SO_SH_5:
	case RA_5:
		PORTB.DR.BYTE	= 2;//LED6_
		break;
	case RA_SH_5:
	case SI_5:
		PORTB.DR.BYTE	= 0x14;//LED7_
		break;
	case DO_6:
	case DO_7:
		PORTB.DR.BYTE	= 0x60;//LED8©F_
		break;
	case DO_SH_6:
		PORTD.DR.BYTE	= 0x03;//LED1©F_
		break;
	case RE_6:
		PORTD.DR.BYTE	= 0x30;//LED2©F_
		break;
	case RE_SH_6:
	case MI_6:
		PORTD.DR.BYTE	= 0xc0;//LED3©F_
		break;
	case FA_6:
		PORTE.DR.BYTE	= 0x06;//LED4©F_
		break;
	case FA_SH_6:
	case SO_6:
		PORTE.DR.BYTE	= 0x30;//LED5©F_
		break;
	case SO_SH_6:
	case RA_6:
		PORTE.DR.BYTE	= 0x80;//LED6©F_
		PORTB.DR.BYTE	= 0x01;
		break;
	case RA_SH_6:
	case SI_6:
		PORTB.DR.BYTE	= 0x0c;//LED7©F_
		break;
	default:
		PORTD.DR.BYTE = 0;
		PORTE.DR.BYTE = 0;
		PORTB.DR.BYTE = 0;
		break;
	}
}
/******************************************************************/
/*7ZOLEDΙ\¦·ι											  */
/*void out_put_segled(unsigned char *display,long interval)		@*/
/*		unsigned char *value
/*		long interval:_ΕΤu
/****************************************************/
void out_put_segled(unsigned char *display,long interval)
{
	
}