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
const unsigned char DISPLAY[]	= {NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE ,
				 				   NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE ,
				   				   FON_SP,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,FON_MI,FON_DP,NONE ,
				   				   FON_0 ,FON_1 ,FON_2 ,FON_3 ,FON_4 ,FON_5 ,FON_6 ,FON_7 ,FON_8 ,FON_9 ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE ,
				   				   FON_A ,FON_B ,FON_C ,FON_D ,FON_E ,FON_F ,NONE  ,FON_H ,NONE  ,FON_J ,NONE  ,FON_L ,NONE  ,NONE  ,FON_O ,FON_P,
				   				   FON_Q ,FON_R ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,FON_Y ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,FON__ ,NONE ,
				   				   FON_A ,FON_B ,FON_C ,FON_D ,FON_E ,FON_F ,NONE  ,FON_H ,NONE  ,FON_J ,NONE  ,FON_L ,NONE  ,NONE  ,FON_O ,FON_P,
				   				   FON_Q ,FON_R ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,NONE  ,FON_Y ,NONE  ,};
const unsigned char number[]	= {FON_0 ,FON_1 ,FON_2 ,FON_3 ,FON_4 ,FON_5 ,FON_6 ,FON_7 ,FON_8 ,FON_9 };
const unsigned long led_port_value_array[LED_NUM][LED_COLOR_NUM] = {{0x000001,0x000002,0x000004,0x000003,0x000006,0x000005,0x000007,0x000000},//LED1　赤　緑　青　黄　シアン　マゼンタ　白　黒
															  		{0x000008,0x000010,0x000020,0x000018,0x000030,0x000028,0x000038,0x000000},//LED2　赤　緑　青　黄　シアン　マゼンタ　白　黒
															  		{0x000040,0x000080,0x000100,0x0000c0,0x000180,0x000140,0x0001c0,0x000000},//LED3　赤　緑　青　黄　シアン　マゼンタ　白　黒
															  		{0x000200,0x000400,0x000800,0x000600,0x000c00,0x000a00,0x000e00,0x000000},//LED4　赤　緑　青　黄　シアン　マゼンタ　白　黒
															  		{0x001000,0x002000,0x004000,0x003000,0x006000,0x005000,0x007000,0x000000},//LED5　赤　緑　青　黄　シアン　マゼンタ　白　黒
															  		{0x008000,0x010000,0x020000,0x018000,0x030000,0x028000,0x038000,0x000000},//LED6　赤　緑　青　黄　シアン　マゼンタ　白　黒
															  		{0x040000,0x080000,0x100000,0x0c0000,0x180000,0x140000,0x1c0000,0x000000},//LED7　赤　緑　青　黄　シアン　マゼンタ　白　黒
															  		{0x200000,0x400000,0x800000,0x600000,0xc00000,0xa00000,0xe00000,0x000000},//LED8　赤　緑　青　黄　シアン　マゼンタ　白　黒
															  		};//ポートD,E,Bのバイトを一つにまとめ
/***********************************************************
 * プロトタイプ宣言
 ***********************************************************/
void led_blink(void);
void segled_flush(void);
void segled_blink(void);
void segled_timer_update(void);
void segled_display_update(unsigned char *ascii);
/************************************************************/
/* ワークエリア定義												*/
/************************************************************/
long *led_pointer;                                          //7セグ以外のLEDの状態へのポインタ、点滅する時はポインタをled_stateとled_blink_stateで切り替える
long led_state;												//7セグ以外のLEDの状態、ポートDとポートEとポートBの点灯したときの状態
long led_blink_state;										//ポートDとポートEとポートBの消灯したときの状態（点滅指定されたLED以外はついている）
long led_current_interval;
unsigned char segled_state[SEG7_DIGIT_NUM];					//7セグLEDの点灯したときの状態
unsigned char segled_blink_state[SEG7_DIGIT_NUM] = {FON_SP,FON_SP,FON_SP};			//7セグLEDの消灯したときの状態
unsigned char *segled_pointer = &segled_state[0];			//
long segled_current_interval;								//7セグの現在の点灯間隔
unsigned char current_digit = 0;
unsigned char segled_blink_state[SEG7_DIGIT_NUM];			//ポートAの消灯したときの状態
signed short seg_timer_value;
unsigned char *time_to_string;
/***********************************************************************/
/*7セグ以外のLEDの点灯、intervalを指定すれば点滅開始				   */
/*void output_led(unsigned char led,unsigned char color,long interval) */
/*	引数：unsigned char led		LED番号1~8							   */
/*		　unsigned char color	LED色								   */
/*									1:赤							   */
/*									2:緑							   */
/*									3:青							   */
/*									4:黄							   */
/*									5:シアン						   */
/*									6:マゼンタ						   */
/*									7:白							   */
/*									8:黒							   */
/*		  long interval			点滅間隔(ms)						   */
/***********************************************************************/
void output_led(unsigned char led,unsigned char color,long interval)
{
	led_state						|= led_port_value_array[led - 1][color - 1];//
	if(interval == 0)//点滅しないLEDは覚えておく
		led_blink_state					= led_port_value_array[led - 1][color - 1];
	PORTD.DR.BYTE					= led_state & 0xff;
	PORTE.DR.BYTE					= (led_state & 0xff00) >> 8;
	PORTB.DR.BYTE					= (led_state & 0xff0000) >> 16;
	if(interval != 0){
		if(led_current_interval == interval){
			//nop
		}else if(led_current_interval != interval){							//現在の点滅間隔と違う感覚が指定されたら変更
			led_current_interval	= interval;
			count_timer_dell(led_blink);
			interval_function_set(interval,led_blink);
		}else{
			led_current_interval	= interval;
			interval_function_set(interval,led_blink);
		}
	}else if(led_current_interval != 0)										//点滅終了
		count_timer_dell(led_blink);
}

/******************************************************************/
/*
/*void led_blink(void)											  */
/******************************************************************/
void led_blink(void)
{
	if(led_pointer == &led_blink_state)
		led_pointer 		= &led_state;
	else
		led_pointer 		= &led_blink_state;
	PORTD.DR.BYTE			= *led_pointer & 0xff;
	PORTE.DR.BYTE			= (*led_pointer & 0xff00) >> 8;
	PORTB.DR.BYTE			= (*led_pointer & 0xff0000) >> 16;
}

/******************************************************************/
/*LED消灯
 * void led_lights_out(void)
 ******************************************************************/
void led_lights_out(void)
{
	if(led_current_interval != 0){
		count_timer_dell(led_blink);
		led_current_interval	= 0;
	}
	led_state					= 0;
	PORTD.DR.BYTE				= 0;
	PORTE.DR.BYTE				= 0;
	PORTB.DR.BYTE				= 0;
}
/******************************************************************/
/*7セグLEDに表示開始する										  */
/*void segled_initialize(void)								　　  */
/******************************************************************/
void segled_initialize(void)
{
    current_digit			= 0;
    segled_current_interval	= 0;
    interval_function_set(1,segled_flush);
}

/******************************************************************/
/*現在の表示を更新する変える
/* void segled_display_update(unsigned char *ascii,long interval)
/* 	引数：unsigned char *display　表示する数字、文字
/* 		long interval
/******************************************************************/
void segled_display_update(unsigned char *ascii,long interval)
{
	unsigned char i;
	for(i = 0;i < 3;i++){
		if(ascii[i] > 0x7a)
    		segled_state[i]	= NONE;
    	else
    		segled_state[i]	= DISPLAY[ascii[i]];
	}
	if(segled_current_interval == interval){
		//nop
	}else if(segled_current_interval != interval){							//現在の点滅間隔と違う感覚が指定されたら変更
		segled_current_interval	= interval;
		count_timer_dell(segled_blink);
		interval_function_set(interval,segled_blink);
	}else{
		segled_current_interval	= interval;
		interval_function_set(interval,segled_blink);
	}
}

/******************************************************************/
/*
/******************************************************************/
void segled_blink(void)
{
	if(segled_pointer == &segled_state[0])
		segled_pointer = &segled_blink_state[0];
	else
		segled_pointer = &segled_state[0];
}
/**********************************************************************/
/*7セグタイマスタート入力の判定、スタート値のセット、システムタイマへのセット				　　	  */
/*void segled_timer_start(unsigned char *display)				  	  */
/* 	引数：unsigned char *start									  	  */
/* 	戻り値：unsigned char ERROR:入力した値が不正,タイマに空き無し　SUCCESS：タイマスタート*/
/**********************************************************************/
unsigned char segled_timer_start(unsigned char *start_value)
{
	unsigned char i;
	//引数の判定
	seg_timer_value			= atoi(start_value);					//カウントダウン用の数値
	time_to_string			= start_value;
	if(seg_timer_value > 999)
		return ERROR;
	if(ERROR == interval_function_set(1000,segled_timer_update))
		return ERROR;
	return SUCCESS;
}

/******************************************************************/
/*7セグタイマ停止													  */
/* void segled_timer_stop(void)									  */
/******************************************************************/
void segled_timer_stop(void)
{
	count_timer_dell(segled_timer_update);
}

/*************************************************************************/
/*表示をダウンカウント０００になったらタイマストップ										 */
/*void segled_timer_update(void)										 */
/*************************************************************************/
void segled_timer_update(void)
{
	seg_timer_value--;
	if(seg_timer_value <= 0){
		segled_timer_stop();//
		time_to_string[0] = time_to_string[1] = time_to_string[2] = 0x30;
	}else{
		time_to_string[0] = (seg_timer_value / 100) + 0x30;
		time_to_string[1] = ((seg_timer_value / 10) % 10) + 0x30;
		time_to_string[2] = (seg_timer_value % 10) + 0x30;
	}
	segled_display_update(time_to_string,0);
}

/******************************************************************/
/*7セグLEDのダイナミック点灯、1msで桁を切り替えて表示していく	  */
/*void segled_flush(void)										  */
/******************************************************************/
void segled_flush(void)
{
	unsigned char digit_direction[3]	= {0x02,0x04,0x08};
	PORT4.DR.BYTE						= segled_pointer[current_digit];
	PORTA.DR.BYTE						= digit_direction[current_digit];
	current_digit ++;
	if(current_digit >= SEG7_DIGIT_NUM)
		current_digit					= 0;
}

/******************************************************************/
/*７セグLED消灯													  */
/*void segled_lights_out(void)									  */
/******************************************************************/
void segled_lights_out(void)
{
	count_timer_dell(segled_flush);				//ダイナミック点灯停止
	count_timer_dell(segled_blink);
	segled_pointer 						= &segled_state[0];
	PORTA.DR.BYTE						= 0;//桁セレクトを0にして消灯
}
