/*
 * output_led.c
 *
 *  Created on: 2023/10/22
 *      Author: kameyamahiroki
 */

#include "main.h"
/*
 * �萔��`
 */
//�I�N�^�[�u��
#define DO_1 (0)/*1�I�N�^�[�u�ڃ\*/
#define DO_2 (12)//2�I�N�^�[�u�ڃh
#define DO_3 (24)//3�I�N�^�[�u�ڃh
#define DO_4 (36)//4�I�N�^�[�u�ڃh
#define RE_1 (2)//��
#define RE_2 (14)//
#define RE_3 (26)//
#define RE_4 (38)//
#define RE_5 (50)//
#define RE_6 (62)//
#define DO_SH_1 (1)//�h��//�I�N�^�[�u��
#define DO_SH_2 (13)//�I�N�^�[�u��
#define DO_SH_3 (25)//�I�N�^�[�u��
#define DO_SH_4 (37)//�I�N�^�[�u��
#define DO_SH_5 (49)
#define DO_SH_6 (61)
#define RE_SH_1 (3)//�I�N�^�[�u�ڃ���
#define RE_SH_2 (15)//�I�N�^�[�u��
#define RE_SH_3 (27)//�I�N�^�[�u��
#define RE_SH_4 (39)//�I�N�^�[�u��
#define RE_SH_5 (51)//�I�N�^�[�u��
#define RE_SH_6 (63)//�I�N�^�[�u��
#define MI_1 (4)//�I�N�^�[�u�ڃ~
#define MI_2 (16)//�I�N�^�[�u��
#define MI_3 (28)//�I�N�^�[�u��
#define MI_4 (40)//�I�N�^�[�u��
#define MI_5 (52)//�I�N�^�[�u��
#define MI_6 (64)//�I�N�^�[�u��
#define FA_1 (5)//�I�N�^�[�u�ڃt�@
#define FA_2 (17)//�I�N�^�[�u��
#define FA_3 (29)//�I�N�^�[�u��
#define FA_4 (41)//�I�N�^�[�u��
#define FA_5 (53)//�I�N�^�[�u��
#define FA_6 (65)//�I�N�^�[�u��
#define FA_SH_1 (6)//�I�N�^�[�u�ڃt�@��
#define FA_SH_2 (18)//�I�N�^�[�u��
#define FA_SH_3 (30)//�I�N�^�[�u��
#define FA_SH_4 (42)//�I�N�^�[�u��
#define FA_SH_5 (54)//�I�N�^�[�u��
#define FA_SH_6 (66)//�I�N�^�[�u��
#define SO_1 (7)//�I�N�^�[�u��
#define SO_2 (19)//
#define SO_3 (31)//�I�N�^�[�u��
#define SO_4 (43)//�I�N�^�[�u��
#define SO_5 (55)//�I�N�^�[�u��
#define SO_6 (67)//�I�N�^�[�u��
#define SO_SH_1 (8)//�I�N�^�[�u�ڃ\��
#define SO_SH_2 (20)//�I�N�^�[�u��
#define SO_SH_3 (32)//�I�N�^�[�u��
#define SO_SH_4 (44)//�I�N�^�[�u��
#define SO_SH_5 (56)//�I�N�^�[�u��
#define SO_SH_6 (68)//�I�N�^�[�u��
#define RA_1 (9)//�I�N�^�[�u�ڃ�
#define RA_2 (21)//�I�N�^�[�u��
#define RA_3 (33)//�I�N�^�[�u��
#define RA_4 (45)//�I�N�^�[�u��
#define RA_5 (57)//�I�N�^�[�u��
#define RA_6 (69)//�I�N�^�[�u��
#define RA_SH_1 (10)//�I�N�^�[�u�ڃ���
#define RA_SH_2 (22)//�I�N�^�[�u��
#define RA_SH_3 (34)//�I�N�^�[�u��
#define RA_SH_4 (46)//�I�N�^�[�u��
#define RA_SH_5 (58)//�I�N�^�[�u��
#define RA_SH_6 (70)//�I�N�^�[�u��
#define SI_1 (11)//�I�N�^�[�u�ڃV
#define SI_2 (23)//�I�N�^�[�u��
#define SI_3 (35)//�I�N�^�[�u��
#define SI_4 (47)//�I�N�^�[�u��
#define SI_5 (59)//�I�N�^�[�u��
#define SI_6 (71)//�I�N�^�[�u��
#define DO_5 (48)//�I�N�^�[�u��
#define DO_6 (60)//�I�N�^�[�u��
#define DO_7 (72)//�I�N�^�[�u��

//�P�I�N�^�[�u�ڐԁ@2�I�N�^�[�u�ڗ΁@�R�I�N�^�[�u�ڐ@�S�I�N�^�[�u�ڔ� 5�I�N�^�[�u�ڎ� 6�I�N�^�[�u�ډ��F
void output_led(int scale)
{
	switch(scale){
	case DO_1:
	case DO_SH_1:
		PORTD.DR.BIT.B0	= 1;//LED1�ԓ_��
		break;
	case RE_1:
		PORTD.DR.BIT.B3	= 1;//LED2�ԓ_��
		break;
	case RE_SH_1:
	case MI_1:
		PORTD.DR.BIT.B6 = 1;//LED3�ԓ_��
		break;
	case FA_1:
		PORTE.DR.BIT.B1 = 1;//LED4�ԓ_��
		break;
	case FA_SH_1:
	case SO_1:
		PORTE.DR.BIT.B4	= 1;//LED5�ԓ_��
		break;
	case SO_SH_1:
	case RA_1:
		PORTE.DR.BIT.B7 = 1;//LED6�ԓ_��
		break;
	case RA_SH_1:
	case SI_1:
		PORTB.DR.BIT.B2 = 1;//LED7�ԓ_��
		break;
	case DO_2:
	case DO_SH_2:
		PORTD.DR.BIT.B1 = 1;//LED1�Γ_��
		break;
	case RE_2:
		PORTD.DR.BIT.B4	= 1;//LED2�Γ_��
		break;
	case RE_SH_2:
	case MI_2:
		PORTD.DR.BIT.B7	= 1;//LED3�Γ_��
		break;
	case FA_2:
		PORTE.DR.BIT.B2	= 1;//LED4�Γ_��
		break;
	case FA_SH_2:
	case SO_2:
		PORTE.DR.BIT.B5 = 1;//LED5�Γ_��
		break;
	case SO_SH_2:
	case RA_2:
		PORTB.DR.BIT.B0 = 1;//LED6�Γ_��
		break;
	case RA_SH_2:
	case SI_2:
		PORTB.DR.BIT.B3	= 1;//LED7�Γ_��
		break;
	case DO_3:
		PORTB.DR.BIT.B7 = 1;//LED8�_��
		break;
	case DO_SH_3:
		PORTB.DR.BIT.B2 = 1;//LED1�_��
		break;
	case RE_3:
		PORTD.DR.BIT.B5 = 1;//LED2�_��
		break;
	case RE_SH_3:
	case MI_3:
		PORTE.DR.BIT.B0	= 1;//LED3�_��
		break;
	case FA_3:
		PORTE.DR.BIT.B3 = 1;//LED4�_��
		break;
	case FA_SH_3:
	case SO_3:
		PORTE.DR.BIT.B6 = 1;//LED5�_��
		break;
	case SO_SH_3:
	case RA_3:
		PORTB.DR.BIT.B1	= 1;//LED6�_��
		break;
	case RA_SH_3:
	case SI_3:
		PORTB.DR.BIT.B4	= 1;//LED7�_��
		break;
	case DO_4:
		PORTB.DR.BYTE	= 0xe0;//LED8���_��
		break;
	case DO_SH_4:
		PORTD.DR.BYTE	= 0x07;//LED1���_��
		break;
	case RE_4:
		PORTD.DR.BYTE	= 0x38;//LED�Q���_��
		break;
	case RE_SH_4:
	case MI_4:
		PORTE.DR.BIT.B0	= 1;//LED3���_��
		PORTD.DR.BYTE	= 0xc0;
		break;
	case FA_4:
		PORTE.DR.BYTE = 0x0e;//LED4���_��
		break;
	case FA_SH_4:
	case SO_4:
		PORTE.DR.BYTE	= 0x70;//LED5���_��
		break;
	case SO_SH_4:
	case RA_4:
		PORTB.DR.BYTE	= 2;//LED6���_��
		PORTE.DR.BIT.B7	= 1;
		break;
	case RA_SH_4:
	case SI_4:
		PORTB.DR.BYTE	= 0x1c;//LED7���_��
		break;
	case DO_5:
		PORTB.DR.BYTE	= 0xa0;//LED8���_��
		break;
	case DO_SH_5:
		PORTD.DR.BYTE	= 0x05;//LED1���_��
		break;
	case RE_5:
		PORTD.DR.BYTE	= 0x28;//LED2���_��
		break;
	case RE_SH_5:
	case MI_5:
		PORTE.DR.BIT.B1	= 1;//LED3���_��
		PORTE.DR.BIT.B7	= 1;
		break;
	case FA_5:
		PORTE.DR.BYTE	= 0x0a;//LED4���_��
		break;
	case FA_SH_5:
	case SO_5:
		PORTE.DR.BYTE	= 0x50;//LED5���_��
		break;
	case SO_SH_5:
	case RA_5:
		PORTB.DR.BYTE	= 2;//LED6���_��
		break;
	case RA_SH_5:
	case SI_5:
		PORTB.DR.BYTE	= 0x14;//LED7���_��
		break;
	case DO_6:
	case DO_7:
		PORTB.DR.BYTE	= 0x60;//LED8���F�_��
		break;
	case DO_SH_6:
		PORTD.DR.BYTE	= 0x03;//LED1���F�_��
		break;
	case RE_6:
		PORTD.DR.BYTE	= 0x30;//LED2���F�_��
		break;
	case RE_SH_6:
	case MI_6:
		PORTD.DR.BYTE	= 0xc0;//LED3���F�_��
		break;
	case FA_6:
		PORTE.DR.BYTE	= 0x06;//LED4���F�_��
		break;
	case FA_SH_6:
	case SO_6:
		PORTE.DR.BYTE	= 0x30;//LED5���F�_��
		break;
	case SO_SH_6:
	case RA_6:
		PORTE.DR.BYTE	= 0x80;//LED6���F�_��
		PORTB.DR.BYTE	= 0x01;
		break;
	case RA_SH_6:
	case SI_6:
		PORTB.DR.BYTE	= 0x0c;//LED7���F�_��
		break;
	case OFF:
	case REST://LED�S�ď���
	case COMP:
		PORTD.DR.BYTE = 0;
		PORTE.DR.BYTE = 0;
		PORTB.DR.BYTE = 0;
		break;
	}
}
