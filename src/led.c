/*
 * output_led.c
 *
 *  Created on: 2023/10/22
 *      Author: kameyamahiroki
 */

#include "main.h"
/************************************************************/
/* �萔��`													*/
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
const unsigned long led_port_value_array[LED_NUM][LED_COLOR_NUM] = {{0x000001,0x000002,0x000004,0x000003,0x000006,0x000005,0x000007,0x000000},//LED1�@�ԁ@�΁@�@���@�V�A���@�}�[���^�@���@��
															  		{0x000008,0x000017,0x000027,0x000018,0x000030,0x000028,0x000038,0x000000},//LED2�@�ԁ@�΁@�@���@�V�A���@�}�[���^�@���@��
															  		{0x000040,0x000080,0x000100,0x0000c0,0x000180,0x000140,0x0001c0,0x000000},//LED3�@�ԁ@�΁@�@���@�V�A���@�}�[���^�@���@��
															  		{0x000200,0x000400,0x000800,0x000600,0x000c00,0x000a00,0x000e00,0x000000},//LED4�@�ԁ@�΁@�@���@�V�A���@�}�[���^�@���@��
															  		{0x001000,0x002000,0x004000,0x003000,0x006000,0x005000,0x007000,0x000000},//LED5�@�ԁ@�΁@�@���@�V�A���@�}�[���^�@���@��
															  		{0x008000,0x010000,0x020000,0x018000,0x030000,0x028000,0x038000,0x000000},//LED6�@�ԁ@�΁@�@���@�V�A���@�}�[���^�@���@��
															  		{0x040000,0x080000,0x100000,0x0c0000,0x180000,0x140000,0x1c0000,0x000000},//LED7�@�ԁ@�΁@�@���@�V�A���@�}�[���^�@���@��
															  		{0x200000,0x400000,0x800000,0x600000,0xc00000,0xa00000,0xe00000,0x000000},//LED8�@�ԁ@�΁@�@���@�V�A���@�}�[���^�@���@��
															  		};
/***********************************************************
 * �v���g�^�C�v�錾
 ***********************************************************/
void led_blink(void);
void segled_flush(void);
void segled_timer_update(void);
void segled_display_update(unsigned char *ascii);
/************************************************************/
/* ���[�N�G���A��`												*/
/************************************************************/
long *led_pointer;                                          //7�Z�O�ȊO��LED�̏�Ԃւ̃|�C���^�A�_�ł��鎞�̓|�C���^��led_state��led_blink_state�Ő؂�ւ���
long led_state;												//7�Z�O�ȊO��LED�̏�ԁA�|�[�gD�ƃ|�[�gE�ƃ|�[�gB�̓_�������Ƃ��̏��
long led_blink_state;										//�|�[�gD�ƃ|�[�gE�ƃ|�[�gB�̏��������Ƃ��̏�ԁi�_�Ŏw�肳�ꂽLED�ȊO�͂��Ă���j
long led_current_interval;
unsigned char segled_state[SEG7_DIGIT_NUM];					//�|�[�gA�̓_�������Ƃ��̏��
unsigned char current_digit = 0;
unsigned char segled_blink_state[SEG7_DIGIT_NUM];					//�|�[�gA�̏��������Ƃ��̏��
unsigned short seg_timer_value;
/***********************************************************************/
/*7�Z�O�ȊO��LED�̓_���Ainterval���w�肷��Γ_�ŊJ�n				   */
/*void output_led(unsigned char led,unsigned char color,long interval) */
/*	�����Funsigned char led		LED�ԍ�1~8							   */
/*		�@unsigned char color	LED�F								   */
/*									1:��							   */
/*									2:��							   */
/*									3:��							   */
/*									4:��							   */
/*									5:�V�A��						   */
/*									6:�}�[���^						   */
/*									7:��							   */
/*									8:��							   */
/*		  long interval			�_�ŊԊu(ms)						   */
/***********************************************************************/
void output_led(unsigned char led,unsigned char color,long interval)
{
	if(interval != 0){
		if(led_current_interval != 0 && led_current_interval != interval)					//���݂̓_�ŊԊu�ƈႤ���o���w�肳�ꂽ��ύX
			count_timer_dell(led_blink);
		interval_function_set(interval,led_blink);
	}else if(led_current_interval != 0)							//�_�ŏI��
		count_timer_dell(led_blink);
	else
		led_blink_state	= led_state;
	led_state			|= led_port_value_array[led - 1][color - 1];
	PORTD.DR.BYTE		|= led_state & 0xff;
	PORTE.DR.BYTE		|= (led_state & 0xff00) >> 8;
	PORTB.DR.BYTE		|= (led_state & 0xff0000) >> 16;
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
/*7�Z�OLED�ɕ\���J�n����										  		�@�@*/
/*void out_put_segled_start(unsigned char *display)				�@�@*/
/*		unsigned char *value ������ւ̃|�C���^						  */
/*		long interval:�_�ŊԊu									  */
/******************************************************************/
void out_put_segled_start(unsigned char *display)
{
    current_digit			= 0;
    segled_display_update(display);
    interval_function_set(1,segled_flush);
}

/******************************************************************/
/*��������o�͂���l�ɕς���
/* void seg_led_update(unsigned char *display)
/******************************************************************/
void segled_display_update(unsigned char *ascii)
{
	unsigned char i;
	for(i = 0;i < 3;i++){
		if(DISPLAY[i] > 0x7a)
    		segled_state[i]	= NONE;
    	else
    		segled_state[i]	= DISPLAY[ascii[i]];
    }
}

/**********************************************************************/
/*7�Z�O�^�C�}�X�^�[�g���͂̔���A�X�^�[�g�l�̃Z�b�g�A�V�X�e���^�C�}�ւ̃Z�b�g				�@�@	  */
/*void segled_timer_start(unsigned char *display)				  	  */
/* 	�����Funsigned char *start									  	  */
/* 	�߂�l�Funsigned char ERROR:���͂����l���s��,�^�C�}�ɋ󂫖����@SUCCESS�F�^�C�}�X�^�[�g*/
/**********************************************************************/
unsigned char segled_timer_start(unsigned char *start_value)
{
	unsigned char i;
	//�����̔���
	seg_timer_value			= atoi(start_value);
	if(seg_timer_value > 999)
		return ERROR;
	if(ERROR == interval_function_set(1000,segled_timer_update))
		return ERROR;
	return interval_function_set(1,segled_flush);
}

/******************************************************************/
/*7�Z�O�^�C�}��~													  */
/* void segled_timer_stop(void)									  */
/******************************************************************/
void segled_timer_stop(void)
{
//	count_timer_dell();
	count_timer_dell(segled_timer_update);
}

/******************************************************************/
/*1�b���ƂɌĂяo����ĕ\�����_�E���J�E���g�O�O�O�ɂȂ����玩�����t�J�n
 *
 */
/******************************************************************/
void segled_timer_update(void)
{
	unsigned char time_to_string[3];
	seg_timer_value--;
	if(seg_timer_value <= 0){
		automatic_playing_start(CANON,SQUARE,0,0,0);
		segled_timer_stop();//
	}else{
		time_to_string[0] = (seg_timer_value / 100) + 0x30;
		time_to_string[1] = ((seg_timer_value / 10) % 10) + 0x30;
		time_to_string[2] = (seg_timer_value % 10) + 0x30;
	}
	segled_display_update(time_to_string);
}

/******************************************************************/
/*
/*
/******************************************************************/
void segled_flush(void)
{
	unsigned char digit_direction[3]	= {0x02,0x04,0x08};
	PORT4.DR.BYTE						= segled_state[current_digit];
	PORTA.DR.BYTE						= digit_direction[current_digit];
	current_digit ++;
	if(current_digit >= SEG7_DIGIT_NUM)
		current_digit					= 0;
}
