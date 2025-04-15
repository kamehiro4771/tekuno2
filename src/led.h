/*
 * output_led.h
 *
 *  Created on: 2023/10/22
 *      Author: kameyamahiroki
 */

#ifndef __LED_H__
#define __LED_H__
#define DO_1 (0)					//1オクターブ目ド
#define DO_2 (12)					//2オクターブ目ド
#define DO_3 (24)					//3オクターブ目ド
#define DO_4 (36)					//4オクターブ目ド
#define RE_1 (2)					//1オクターブ目レ
#define RE_2 (14)					//2オクターブ目レ
#define RE_3 (26)					//3オクターブ目レ
#define RE_4 (38)					//4オクターブ目レ
#define RE_5 (50)					//5オクターブ目レ
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
#define NONE (0x00)
#define FON_SP (0x00)
#define FON_MI (0x)
#define FON_0 (0x3f)
#define FON_1 (0x06)
#define FON_2 (0x5b)
#define FON_3 (0x4f)
#define FON_4 (0x66)
#define FON_5 (0x75)
#define FON_6 (0x7c)
#define FON_7 (0x27)
#define FON_8 (0x7f)
#define FON_9 (0x6f)
#define FON_A (0x77)
#define FON_B (0x7c)
#define FON_C (0x39)
#define FON_D (0x5b)
#define FON_E (0x79)
#define FON_F (0x71)
#define FON_H ()
#define FON_J ()
#define FON_L ()
#define FON_O ()
#define FON_P ()
#define FON_Q ()
#define FON_R ()
#define FON_Y ()
#define FON__ ()
#define SEG7_DIGIT_NUM (3)
void output_led(int sw_no);
void out_put_segled(unsigned char *display,long interval);
#endif /* OUTPUT_LED_H_ */
