/*
 * output_speaker.c
 *
 *  Created on: 2023/10/21
 *      Author: kameyamahiroki
 */
 /*******************************************************************************/
/*�w�b�_�[�t�@�C��																*/
/********************************************************************************/
#include "main.h"
/*
*Createdon:2024/04/06
*Author:kameyamahiroki
*/
/********************************************************************************/
/*�萔��`																		*/
/********************************************************************************/
const unsigned short sine_value_table[]={512,521,530,540,549,559,568,577,587,596,605,614,624,633,642,651,660,
										 669,678,687,696,705,713,722,730,739,747,756,764,772,780,788,796,804,
										 811,819,826,834,841,848,855,862,869,876,882,889,895,901,907,913,919,
										 924,930,935,941,946,950,955,960,964,969,973,977,981,984,988,991,994,
										 997,1000,1003,1005,1008,1010,1012,1014,1016,1017,1018,1020,1021,1021,
										 1022,1023,1023,1023,1023,1023,1022,1022,1021,1020,1019,1018,1016,1015,
										 1013,1011,1009,1007,1004,1002,999,996,993,990,986,982,979,975,971,966,
										 962,958,953,948,943,938,933,927,922,916,910,904,898,892,885,879,872,866,
										 859,852,845,837,830,823,815,808,800,792,784,776,768,760,751,743,735,726,
										 718,709,700,691,683,674,665,656,647,637,628,619,610,601,591,582,573,563,
										 554,544,535,526,516,507,497,488,479,469,460,450,441,432,422,413,404,395,
										 386,376,367,358,349,340,332,323,314,305,297,288,280,272,263,255,247,239,
										 231,223,215,208,200,193,186,178,171,164,157,151,144,138,131,125,119,113,
										 107,101,96,90,85,80,75,70,65,61,57,52,48,44,41,37,33,30,27,24,21,19,16,14,
										 12,10,8,7,5,4,3,2,1,1,0,0,0,0,0,1,2,2,3,5,6,7,9,11,13,15,18,20,23,26,29,32,
										 35,39,42,46,50,54,59,63,68,73,77,82,88,93,99,104,110,116,122,128,134,141,147,
										 154,161,168,175,182,189,197,204,212,219,227,235,243,251,259,267,276,284,293,301,
										 310,318,327,336,345,354,363,372,381,390,399,409,418,427,436,446,455,464,474,483,
										 493,502,511
										};
										
const unsigned char SCALE_LED_NUM[]		= {LED1,LED1,LED2,LED3,LED3,LED4,LED5,LED5,LED6,LED6,LED7,LED7,//1�I�N�^�[�u��
										   LED1,LED1,LED2,LED3,LED3,LED4,LED5,LED5,LED6,LED6,LED7,LED7,//2�I�N�^�[�u��
										   LED8,LED1,LED2,LED3,LED3,LED4,LED5,LED5,LED6,LED6,LED7,LED7,//3�I�N�^�[�u��
										   LED8,LED1,LED2,LED3,LED3,LED4,LED5,LED5,LED6,LED6,LED7,LED7,//4�I�N�^�[�u��
										   LED8,LED1,LED2,LED3,LED3,LED4,LED5,LED5,LED6,LED6,LED7,LED7,//5�I�N�^�[�u��
										   LED8,LED1,LED2,LED3,LED3,LED4,LED5,LED5,LED6,LED6,LED7,LED7,//6�I�N�^�[�u��
										   LED8														   //7�I�N�^�[�u��
										  };														   //���K���Ƃ̓_��LED�ԍ��̃e�[�u��
const unsigned char SCALE_COLOR_NUM[]	= {RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,
										   GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,
										   BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,
										   YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,
										   CYAN,CYAN,CYAN,CYAN,CYAN,CYAN,CYAN,CYAN,CYAN,CYAN,CYAN,CYAN,
										   MAGENTA,MAGENTA,MAGENTA,MAGENTA,MAGENTA,MAGENTA,MAGENTA,MAGENTA,MAGENTA,MAGENTA,MAGENTA,MAGENTA,
										   WHITE,
										   };
									//0�@�@1	�@2	  �@3	�@4	 �@5	6		7	8	 9	10	  11
									//�h�@�h��	�@��	�����~	�@�t�@	�t�@��	�\	�\����	����  �V
									//12	13	14  �@15  16  17  18   19   20  21  22   23
									//�h   �h���@���@�����@�~�t�@�t�@���\	�\�����@�����V
									//24  	25   26   27   28   29   30   31   32   33   34   35
									//�h   �h���@���@�����@�~�@�t�@�@�t�@���\�@�\�����@�@�����@�V
									//36   37  38   39   40   41    42   43  	44  45  46 47
									//�h�@�h�����@�@�����@�~�@�t�@�@�t�@���\�@�\��	���@�����V
									//48   49  50   51   52   53   54   55  56    57   58  59
									//�h�@�h���@���@�����@�~�@�t�@�t�@���\�\���@ ���@�����V
									//60�@�@61�@�@62�@63�@�@64�@65�@�@66�@�@67�@68�@ 69�@ 70  71  72 73
									//�h�@�@�h���@���@�����@�~�@�t�@�@�t�@���\�@�\���@���@�����V�@�h ����							//0�@�@�@�@1	�@�@2	  �@�@3	�@�@4	 �@�@5	6	  �@�@7	�@8	  �@�@9	10	  �@�@11
//PWM�̃^�C�}�ݒ�l�e�[�u��		//�h�@�@�@�@�h��	�@�@��	�@�@�@����	�@�@�~	�@�@�@�t�@	�t�@��	�@�@�@�@�\	�@�\��	�@�@�@�@��	����	�@�@�@�@�V
const unsigned short pwm_timer_value[] = {45867,43293,40863,38569,36405,34362,32433,30613,28894,27273,25742,24297,
										  //12	13	�@�@14  �@15  �@�@16   17   18   19   20   21   22   23
										  //�h   �@�h���@�@�@�@���@�@�@�����@�@�@�~�@�@�@�t�@�@�@�@�t�@���@�@�\�@�@�@�\���@�@���@�@�@�@�����@�@�V
										  22933,21646,20431,19284,18202,17180,16216,15306,14447,13636,12871,12148,
										  //24  	25   26   27   28   29   30   31   32   33   34   35
										  //�h   �@�h���@�@�@�@���@�@�@�����@�@�@�~�@�@�@�t�@�@�@�@�t�@���@�@�\�@�@�@�\���@�@���@�@�@�@�����@�@�V
										  11466,10823,10216,9642,9101,8590,8108,7653,7223,6818,6435,6074,
										  //36   37  38   39   40   41    42   43  44   45   46   47
										  //�h�@�@�@�h���@�@���@�@�@�����@�@�@�~�@�@�@�t�@�@�@�@�t�@���@�@�\�@�@�@�\���@�@���@�@�@�����@�@�V
										  5733,5411,5108,4821,4550,4295,4054,3826,3611,3409,3217,3037,
											//48   49  50   51   52   53   54   55  56    57   58  59
											//�h�@�@�@�h���@�@���@�@�@�����@�@�@�~�@�@�@�t�@�@�@�t�@���@�@�\�@�@�@�\���@  ���@�@ �����@�@�V
											2866,2705,2554,2410,2275,2147,2027,1913,1806,1704,1609,1518,
											//60�@�@�@61�@�@62�@�@�@63�@�@�@64�@�@�@65�@�@�@66�@�@67�@68�@ 69�@ 70  71  72 73
											//�h�@�@�@�h���@�@���@�@�@�����@�@�@�~�@�@�@�t�@�@�@�t�@���@�@�\�@�@�\���@���@�@�����@�@�V�@�@�h
											1433,1353,1277,1205,1137,1073,1013,956,903,852,804,759,716,0,};
//DA�o�͗p�̎��g���e�[�u��
const float onnkai_freq_value[] = {65.406,69.296,73.416,77.782,82.407,87.307,92.499,97.999,103.826,110.000,116.541,123.471,
								   130.813,138.591,146.832,155.563,164.814,174.614,184.997,195.998,207.652,220.000,233.082,246.942,
								   261.626,277.183,293.665,311.127,329.628,349.228,369.994,391.995,415.305,440.000,466.164,493.883,
								   523.251,554.365,587.330,622.254,659.255,698.456,739.989,783.991,830.609,880.000,932.328,987.767,
								   1046.502,1108.731,1174.659,1244.508,1318.510,1396.913,1479.978,1567.982,1661.219,1760.000,1864.655,1975.533,
								   2093.005,2217.461,2349.318,2489.016,2637.020,2793.826,2959.955,3135.963,3322.438,3520.000,3729.310,3951.066,4186.009,0,
								   };
								   
/********************************************************************************/
/*
/*
/********************************************************************************/
/********************************************************************************/
/*���[�N�G���A��`																*/
/********************************************************************************/
double g_1_cycle_interruption_number;
double g_once_voltage_change_quantity;
int g_interrupt_count;
int g_wave_type;
//double g_duty_num;
//�v���g�^�C�v�錾
void sawthooth_wave_calculation(void);
void triangle_wave_calculation(void);
void sine_wave_calculation(double onnkai_freq_value);
void square_wave_calculation(void);
void da_process_each_waveform(int wave_type,int sound_num);
void mute(unsigned char speaker_num);

/*
 * �X�s�[�J�֘A�̏�����
 * void speaker_initialize(void)
 */
void speaker_initialize(void)
{
	//����d�͒񌾋@�\�ݒ�
	SYSTEM.MSTPCRA.BIT.MSTPA8	= 0;	//�}���`�t�@���N�V�����^�C�}�p���X���j�b�g�̃��j�b�g�P���W���[���X�g�b�v�����iPWM�o�́j
	SYSTEM.MSTPCRA.BIT.MSTPA19	= 0;	//DA�R���o�[�^���W���[���X�g�b�v����
	//PWM�X�s�[�J�[
	PORTA.DDR.BIT.B0			= 0;
	//�}���`�t�@���N�V�����^�C�}�p���X���j�b�g1�`�����l���U�̐ݒ�i�X�s�[�J�[�P�j
	MTU6.TCR.BIT.TPSC			= 2;	//16����,3MH��
	MTU6.TCR.BIT.CCLR			= 1;	//TGRA�̃R���y�A�}�b�`�ŃN���A�A�����オ��G�b�W�ŃJ�E���g�A
	MTU6.TMDR.BYTE				= 0x02;	//�ʏ퓮��APWM���[�h�ɐݒ�
	MTU6.TIORH.BIT.IOA			= 0x1;	//�����o�͂�LOW�o�̓R���y�A�}�b�`��LOW�o��
	MTU6.TIORH.BIT.IOB			= 0x2;	//�����o�͂�LOW�o�̓R���y�A�}�b�`��HI�o��
	//�}���`�t�@���N�V�����^�C�}�p���X���j�b�g�P�`�����l��7�̐ݒ�i�X�s�[�J�[2�j
	MTU7.TCR.BIT.TPSC			= 2;	//16����,3MH��
	MTU7.TCR.BIT.CCLR			= 1;	//TGRA�̃R���y�A�}�b�`�ŃN���A�A�����オ��G�b�W�ŃJ�E���g�A
	MTU7.TMDR.BYTE				= 0x02;	//�ʏ퓮��APWM���[�h�ɐݒ�
	MTU7.TIOR.BIT.IOA			= 0x1;	//�����o�͂�LOW�o�̓R���y�A�}�b�`��LOW�o��
	MTU7.TIOR.BIT.IOB			= 0x2;	//�����o�͂�LOW�o�̓R���y�A�}�b�`��HI�o��
	//�}���`�t�@���N�V�����^�C�}�p���X���j�b�g�P�`�����l��8�̐ݒ�i�X�s�[�J�[3�j
	MTU8.TCR.BIT.TPSC			= 2;	//16����,3MH��
	MTU8.TCR.BIT.CCLR			= 1;	//TGRA�̃R���y�A�}�b�`�ŃN���A�A�����オ��G�b�W�ŃJ�E���g�A
	MTU8.TMDR.BYTE				= 0x02;	//�ʏ퓮��APWM���[�h�ɐݒ�
	MTU8.TIOR.BIT.IOA			= 0x1;	//�����o�͂�LOW�o�̓R���y�A�}�b�`��LOW�o��
	MTU8.TIOR.BIT.IOB			= 0x2;	//�����o�͂�LOW�o�̓R���y�A�}�b�`��HI�o��
	//DA�R���o�[�^�[�̐ݒ�
	mtu1_initialize();					//MTU1�̐ݒ�ADA�o�͗p�^�C�}�ݒ�
	DA.DACR.BYTE				= 0xff;	//�`�����l���P�̃A�i���O�o�͋���
}
/********************************************************************************************/
/*�X�s�[�J��LED�̏o�͂��Z�b�g����								  							*/
/*void set_output_value(unsigned char scale,unsigned char speaker_num)		  				*/
/*			�����Fconst char scale �o�͂��鉹�̔ԍ��Apwm_timer_value�̓Y�����ԍ�			*/
/*				  unsigned char speaker_num �X�s�[�J�[�ԍ�									*/
/********************************************************************************************/
void set_output_value(unsigned char scale,unsigned char speaker_num)
{
	SPEAKER *speaker						= get_speaker();
	switch(speaker_num){
	case SPEAKER1:
//		led_lights_out();											//LED����
		MTUB.TSTR.BIT.CST0					= 0;					//PWM�o�̓^�C�}�[��~
		MTUA.TSTR.BIT.CST1					= 0;					//DA�o�̓^�C�}��~
		DA.DACR.BIT.DAOE1					= 1;					//DA�o�͋���
		MTU6.TGRA 							= pwm_timer_value[scale];
		MTU6.TGRB							= MTU6.TGRA * (speaker[0].duty_value / 100);
		da_process_each_waveform(speaker[0].wave_type,scale);		//DA�o�͂ɕK�v�ȏ����A�g�`���Ƃ̏���
//		output_led(SCALE_LED_NUM[scale],SCALE_COLOR_NUM[scale],0);
		break;
	case SPEAKER2:
		MTUB.TSTR.BIT.CST1					= 0;
		MTU7.TGRA 							= pwm_timer_value[scale];
		MTU7.TGRB							= MTU7.TGRA * (speaker[1].duty_value / 100);
		break;
	case SPEAKER3:
		MTUB.TSTR.BIT.CST2					= 0;
		MTU8.TGRA 							= pwm_timer_value[scale];
		MTU8.TGRB							= MTU8.TGRA * (speaker[2].duty_value / 100);
		break;
	}
}

void set_output_speaker_length(unsigned char set_pattern)
{
//	unsigned char i;
	AUTOPLAYER *autoplayer						= get_autoplayer();
	//�X���[�̎���for��ʂ�Ȃ��悤�ɂ�����
//	for(i = 0;i < 200;i++){//���Ɖ��̊ԋ󂯂�
//	}
	switch(set_pattern){//�Z�b�g�p�^�[�������Čo�ߎ��ԃJ�E���g�ɉ��̒������Z�b�g����
	case 1://1�̂�
		autoplayer[0].elapsed_time = autoplayer[0].pnote_value[autoplayer[0].score_count];
		break;
	case 2://2�̂�
		autoplayer[1].elapsed_time = autoplayer[1].pnote_value[autoplayer[1].score_count];
		break;
	case 3://1��2
		autoplayer[0].elapsed_time = autoplayer[0].pnote_value[autoplayer[0].score_count];
		autoplayer[1].elapsed_time = autoplayer[1].pnote_value[autoplayer[1].score_count];
		break;
	case 4://3�̂�
		autoplayer[2].elapsed_time = autoplayer[2].pnote_value[autoplayer[2].score_count];
		break;
	case 5://1��3
		autoplayer[0].elapsed_time = autoplayer[0].pnote_value[autoplayer[0].score_count];
		autoplayer[2].elapsed_time = autoplayer[2].pnote_value[autoplayer[2].score_count];
		break;
	case 6://2��3
		autoplayer[1].elapsed_time = autoplayer[1].pnote_value[autoplayer[1].score_count];
		autoplayer[2].elapsed_time = autoplayer[2].pnote_value[autoplayer[2].score_count];
		break;
	case 7://1��2��3
		autoplayer[0].elapsed_time = autoplayer[0].pnote_value[autoplayer[0].score_count];
		autoplayer[1].elapsed_time = autoplayer[1].pnote_value[autoplayer[1].score_count];
		autoplayer[2].elapsed_time = autoplayer[2].pnote_value[autoplayer[2].score_count];
		break;
	}
}
/********************************************************************************************/
/*�w�����ꂽ�X�s�[�J�[�̐��o�͊J�n															*/
/*void output_speaker_start(unsigned char pattern)											*/
/*		���� unsigned char pattern �o�͂���X�s�[�J�[�p�^�[��								*/
/********************************************************************************************/
void output_speaker_start(unsigned char pattern)
{
	switch(pattern){
	case 1://�X�s�[�J�[�P�̂ݏo��
		MTUB.TSTR.BYTE					|= 1;
		MTUA.TSTR.BIT.CST1				= 1;//DA�o�͂̃^�C�}�[�X�^�[�g
		break;
	case 2://�X�s�[�J�[�Q�̂ݏo��
		MTUB.TSTR.BYTE					|= 2;
		break;
	case 3://�X�s�[�J�[�P�ƃX�s�[�J�[2�o��
		MTUB.TSTR.BYTE					|= 3;
		MTUA.TSTR.BIT.CST1				= 1;//DA�o�͂̃^�C�}�[�X�^�[�g
		break;
	case 4://�X�s�[�J�[�R�̂ݏo��
		MTUB.TSTR.BYTE					|= 4;
		break;
	case 5://�X�s�[�J�[�P�ƃX�s�[�J�[�R�o��
		MTUB.TSTR.BYTE					|= 5;
		MTUA.TSTR.BIT.CST1				= 1;//DA�o�͂̃^�C�}�[�X�^�[�g
		break;
	case 6://�X�s�[�J�[�Q�ƃX�s�[�J�[�R�o��
		MTUB.TSTR.BYTE					|= 6;
		break;
	case 7://�X�s�[�J�P�ƃX�s�[�J�[�Q�ƃX�s�[�J�[�R�o��
		MTUB.TSTR.BYTE					|= 7;
		MTUA.TSTR.BIT.CST1				= 1;//DA�o�͂̃^�C�}�[�X�^�[�g
		break;
	}
}

/********************************************************************************************/
/*�w�����ꂽ�X�s�[�J�[�̏o�͒�~															*/
/*void mute(unsigned char speaker_num)														*/
/*		���� unsigned char speaker �X�s�[�J�[�ԍ�0~3�R�͑S�X�s�[�J�[����					*/
/********************************************************************************************/
void mute(unsigned char speaker_num)
{
	switch(speaker_num){
	case SPEAKER1:						//�X�s�[�J�[�P����DA��
		MTUA.TSTR.BIT.CST1		= 0;
		MTUB.TSTR.BIT.CST0		= 0;
		MTU6.TGRA = MTU6.TGRB 	= 0;
		DA.DACR.BIT.DAOE1		= 0;	//DA�o�͋֎~
		break;
	case SPEAKER2:						//�X�s�[�J�[2����
		MTUB.TSTR.BIT.CST1		= 0;
		MTU7.TGRA = MTU7.TGRB 	= 0;
		break;
	case SPEAKER3:						//�X�s�[�J�[3����
		MTUB.TSTR.BIT.CST2		= 0;
		MTU8.TGRA = MTU8.TGRB 	= 0;
		break;
	case ALL_SPEAKER:					//�S�X�s�[�J�[����
		MTUA.TSTR.BIT.CST1		= 0;
		MTUB.TSTR.BYTE	= 0;
		MTU6.TGRA = MTU6.TGRB 	= 0;
		MTU7.TGRA = MTU7.TGRB 	= 0;
		DA.DACR.BIT.DAOE1		= 0;	//DA�o�͋֎~
		break;
	}
}
/********************************************************************************************/
/*DA�o�͂ɕK�v�ȏ����A�g�`���Ƃ̏���
/*void da_process_each_waveform(int wave_type,int sound_num)
/*
/*
/********************************************************************************************/
void da_process_each_waveform(int wave_type,int sound_num)
{
	g_interrupt_count				= 0;
	g_1_cycle_interruption_number	= (onnkai_freq_value[sound_num] == 0) ? 0 : (1 / onnkai_freq_value[sound_num]) / 0.00000375;//1����������̊��荞�݉񐔂����߂�
	g_wave_type						= wave_type;
	switch(wave_type){
	case SQUARE:
		square_wave_calculation();
		break;
	case SAWTHOOTH:
		sawthooth_wave_calculation();
		break;
	case TRIANGLE:
		triangle_wave_calculation();
		break;
	case SINE:
		sine_wave_calculation(onnkai_freq_value[sound_num]);
		break;
	}
}
//MAX4.6V,����\10�r�b�g�Ȃ̂�0.004492187V���ŏ���
//��F1�I�N�^�[�u�ڂ̃h�A261.626Hz
//		1 / 261.626 = 0.0038222500821784 :1�����̎���
//		0.0038222500821784�b��4.6V�ɒB����
//		�^�C�}���荞�݂̎��ԁ@1�@/�@48MH���@���@180�@=�@0.00000375�b��1�񊄂荞��
//		0.0038222500821784 / 0.00000375 = 1,019.266688580907
//		1�����̊Ԃ�1,019�񊄂荞��
//		1023 / 1019 = 1.003925417075564���ω�������1019�񊄂荞�܂ꂽ��0�ɂ���

//4186.009Hz�̎�		63.70427456478633�񊄂荞��
//TGIA1���荞��DA�o�͂���l��ݒ肷��
/********************************************************************************/
/*�̂�����g��DA�o�͈��̃��W�X�^�̕ω��ʌv�Z									*/
/*void sawtooth_wave_calculation(void)											*/
/********************************************************************************/
void sawthooth_wave_calculation(void)
{
	MTU1.TGRA						= 180;
	g_once_voltage_change_quantity 	= (g_1_cycle_interruption_number == 0) ? 0 : 1023 / g_1_cycle_interruption_number;//1��̕ω���
}
/********************************************************************************/
/*�O�p�g�̈���DA�o�͈��̃��W�X�^�̕ω��ʌv�Z
/*void triangle_wave_calculation(void)											*/
/********************************************************************************/
void triangle_wave_calculation(void)
{
	MTU1.TGRA						= 180;
	g_once_voltage_change_quantity 	= (g_1_cycle_interruption_number == 0) ? 0 : (1023 / g_1_cycle_interruption_number) * 2;
}
/********************************************************************************/
/*�T�C���g��DA�o�͈��̃��W�X�^�̕ω��ʌv�Z									*/
/*void sine_wave_calculation(double onnkai_freq_value)							*/
/********************************************************************************/
void sine_wave_calculation(double onnkai_freq_value)
{
	//�o�͂��鉹�K��1�������P�O�Q�R����
	//�^�C�}�[�̊��荞�݊Ԋu�̕���ύX����
	//1023�񊄂荞�݂ŏo�͂��鉹�K��1�����ɂȂ�悤��
	//��F1�I�N�^�[�u�ڂ̃h�A261.626Hz
	//(1 / 261.626) / 1023 =
	//(1 / onnkai_freq_value) / 1023 = 0.000003736314840839078
	//0.000003736314840839078 / (1 / 48MHz) = 179
	MTU1.TGRA						= (g_1_cycle_interruption_number == 0) ? 0 : ((1.0 / onnkai_freq_value) / 342.0) / (1.0 / 48000000.0);
	g_1_cycle_interruption_number 	= 342.0;
}

void square_wave_calculation(void)
{
	MTU1.TGRA						= 180;
	g_once_voltage_change_quantity 	= 1023;
}

/********************************************************************************/
/*DA�o�͊֐��@���荞�ݏ����őI�����ꂽ�g�`���o�͂���							*/
/*void da_output(void)															*/
/********************************************************************************/
void da_output(void)
{
	switch(g_wave_type){
	case SQUARE:
		if(++g_interrupt_count <= g_1_cycle_interruption_number / 2)
			DA.DADR1 = g_once_voltage_change_quantity;
		else
			DA.DADR1 = 0;
		break;
	case SAWTHOOTH:
		DA.DADR1 = ++g_interrupt_count * g_once_voltage_change_quantity;
		break;
	case TRIANGLE:
		if(++g_interrupt_count <= g_1_cycle_interruption_number / 2)
			DA.DADR1 = g_once_voltage_change_quantity * g_interrupt_count;
		else if(g_interrupt_count <= g_1_cycle_interruption_number)
			DA.DADR1 = 1023 - ((g_once_voltage_change_quantity * g_interrupt_count) - 1023);
		break;
	case SINE:
		DA.DADR1 = sine_value_table[g_interrupt_count++];
		break;
	}
	if(g_interrupt_count >= g_1_cycle_interruption_number)
		g_interrupt_count = 0;
}
