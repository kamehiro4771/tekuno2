/*
 * output_led.h
 *
 *  Created on: 2023/10/22
 *      Author: kameyamahiroki
 */

#ifndef __LED_H__
#define __LED_H__

#define NONE (0x00)
#define FON_SP (0x00)
#define FON_MI (0x40)
#define FON_DP (0x80)
#define FON_0 (0x3f)
#define FON_1 (0x06)
#define FON_2 (0x5b)
#define FON_3 (0x4f)
#define FON_4 (0x66)
#define FON_5 (0x6d)
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
#define FON_H (0x76)
#define FON_J (0x3a)
#define FON_L (0x3c)
#define FON_O (0x5c)
#define FON_P (0x73)
#define FON_Q (0x67)
#define FON_R (0x31)
#define FON_Y (0x6e)
#define FON__ (0x08)
#define LED1 (1)
#define LED2 (2)
#define LED3 (3)
#define LED4 (4)
#define LED5 (5)
#define LED6 (6)
#define LED7 (7)
#define LED8 (8)
#define SEG7_DIGIT_NUM (3)
#define LED_NUM (8)
#define LED_COLOR_NUM (8)
#define RED (1)
#define GREEN (2)
#define BLUE (3)
#define YELLOW (4)
#define CYAN (5)
#define MAGENTA (6)
#define WHITE (7)
#define BLACK (8)
void output_led(unsigned char led,unsigned char color,long interval);
void segled_initialize(void);
unsigned char segled_timer_start(unsigned char *start_value);
void segled_timer_stop(void);

#endif /* OUTPUT_LED_H_ */
