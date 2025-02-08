/*
 * output_led.c
 *
 *  Created on: 2023/10/22
 *      Author: kameyamahiroki
 */

#include "main.h"
/*
 * 定数定義
 */
//オクターブ目
#define DO_1 (0)/*1オクターブ目ソ*/
#define DO_2 (12)//2オクターブ目ド
#define DO_3 (24)//3オクターブ目ド
#define DO_4 (36)//4オクターブ目ド
#define RE_1 (2)//レ
#define RE_2 (14)//
#define RE_3 (26)//
#define RE_4 (38)//
#define RE_5 (50)//
#define RE_6 (62)//
#define DO_SH_1 (1)//ド＃//オクターブ目
#define DO_SH_2 (13)//オクターブ目
#define DO_SH_3 (25)//オクターブ目
#define DO_SH_4 (37)//オクターブ目
#define DO_SH_5 (49)
#define DO_SH_6 (61)
#define RE_SH_1 (3)//オクターブ目レ＃
#define RE_SH_2 (15)//オクターブ目
#define RE_SH_3 (27)//オクターブ目
#define RE_SH_4 (39)//オクターブ目
#define RE_SH_5 (51)//オクターブ目
#define RE_SH_6 (63)//オクターブ目
#define MI_1 (4)//オクターブ目ミ
#define MI_2 (16)//オクターブ目
#define MI_3 (28)//オクターブ目
#define MI_4 (40)//オクターブ目
#define MI_5 (52)//オクターブ目
#define MI_6 (64)//オクターブ目
#define FA_1 (5)//オクターブ目ファ
#define FA_2 (17)//オクターブ目
#define FA_3 (29)//オクターブ目
#define FA_4 (41)//オクターブ目
#define FA_5 (53)//オクターブ目
#define FA_6 (65)//オクターブ目
#define FA_SH_1 (6)//オクターブ目ファ＃
#define FA_SH_2 (18)//オクターブ目
#define FA_SH_3 (30)//オクターブ目
#define FA_SH_4 (42)//オクターブ目
#define FA_SH_5 (54)//オクターブ目
#define FA_SH_6 (66)//オクターブ目
#define SO_1 (7)//オクターブ目
#define SO_2 (19)//
#define SO_3 (31)//オクターブ目
#define SO_4 (43)//オクターブ目
#define SO_5 (55)//オクターブ目
#define SO_6 (67)//オクターブ目
#define SO_SH_1 (8)//オクターブ目ソ＃
#define SO_SH_2 (20)//オクターブ目
#define SO_SH_3 (32)//オクターブ目
#define SO_SH_4 (44)//オクターブ目
#define SO_SH_5 (56)//オクターブ目
#define SO_SH_6 (68)//オクターブ目
#define RA_1 (9)//オクターブ目ラ
#define RA_2 (21)//オクターブ目
#define RA_3 (33)//オクターブ目
#define RA_4 (45)//オクターブ目
#define RA_5 (57)//オクターブ目
#define RA_6 (69)//オクターブ目
#define RA_SH_1 (10)//オクターブ目ラ＃
#define RA_SH_2 (22)//オクターブ目
#define RA_SH_3 (34)//オクターブ目
#define RA_SH_4 (46)//オクターブ目
#define RA_SH_5 (58)//オクターブ目
#define RA_SH_6 (70)//オクターブ目
#define SI_1 (11)//オクターブ目シ
#define SI_2 (23)//オクターブ目
#define SI_3 (35)//オクターブ目
#define SI_4 (47)//オクターブ目
#define SI_5 (59)//オクターブ目
#define SI_6 (71)//オクターブ目
#define DO_5 (48)//オクターブ目
#define DO_6 (60)//オクターブ目
#define DO_7 (72)//オクターブ目

//１オクターブ目赤　2オクターブ目緑　３オクターブ目青　４オクターブ目白 5オクターブ目紫 6オクターブ目黄色
void output_led(int scale)
{
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
		PORTE.DR.BIT.B1	= 1;//LED3紫点灯
		PORTE.DR.BIT.B7	= 1;
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
	case OFF:
	case REST://LED全て消灯
	case COMP:
		PORTD.DR.BYTE = 0;
		PORTE.DR.BYTE = 0;
		PORTB.DR.BYTE = 0;
		break;
	}
}
