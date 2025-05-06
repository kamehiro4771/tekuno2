/*
 * output_speaker.c
 *
 *  Created on: 2023/10/21
 *      Author: kameyamahiroki
 */
 /*******************************************************************************/
/*ヘッダーファイル																*/
/********************************************************************************/
#include "main.h"
/*
*Createdon:2024/04/06
*Author:kameyamahiroki
*/
/********************************************************************************/
/*定数定義																		*/
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
										
const unsigned char SCALE_LED_NUM[]		= {LED1,LED1,LED2,LED3,LED3,LED4,LED5,LED5,LED6,LED6,LED7,LED7,//1オクターブ目
										   LED1,LED1,LED2,LED3,LED3,LED4,LED5,LED5,LED6,LED6,LED7,LED7,//2オクターブ目
										   LED8,LED1,LED2,LED3,LED3,LED4,LED5,LED5,LED6,LED6,LED7,LED7,//3オクターブ目
										   LED8,LED1,LED2,LED3,LED3,LED4,LED5,LED5,LED6,LED6,LED7,LED7,//4オクターブ目
										   LED8,LED1,LED2,LED3,LED3,LED4,LED5,LED5,LED6,LED6,LED7,LED7,//5オクターブ目
										   LED8,LED1,LED2,LED3,LED3,LED4,LED5,LED5,LED6,LED6,LED7,LED7,//6オクターブ目
										   LED8														   //7オクターブ目
										  };														   //音階ごとの点灯LED番号のテーブル
const unsigned char SCALE_COLOR_NUM[]	= {RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,
										   GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,
										   BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,
										   YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,
										   CYAN,CYAN,CYAN,CYAN,CYAN,CYAN,CYAN,CYAN,CYAN,CYAN,CYAN,CYAN,
										   MAGENTA,MAGENTA,MAGENTA,MAGENTA,MAGENTA,MAGENTA,MAGENTA,MAGENTA,MAGENTA,MAGENTA,MAGENTA,MAGENTA,
										   WHITE,
										   };
									//0　　1	　2	  　3	　4	 　5	6		7	8	 9	10	  11
									//ド　ド＃	　レ	レ＃ミ	　ファ	ファ＃	ソ	ソ＃ラ	ラ＃  シ
									//12	13	14  　15  16  17  18   19   20  21  22   23
									//ド   ド＃　レ　レ＃　ミファファ＃ソ	ソ＃ラ　ラ＃シ
									//24  	25   26   27   28   29   30   31   32   33   34   35
									//ド   ド＃　レ　レ＃　ミ　ファ　ファ＃ソ　ソ＃ラ　　ラ＃　シ
									//36   37  38   39   40   41    42   43  	44  45  46 47
									//ド　ド＃レ　　レ＃　ミ　ファ　ファ＃ソ　ソ＃	ラ　ラ＃シ
									//48   49  50   51   52   53   54   55  56    57   58  59
									//ド　ド＃　レ　レ＃　ミ　ファファ＃ソソ＃　 ラ　ラ＃シ
									//60　　61　　62　63　　64　65　　66　　67　68　 69　 70  71  72 73
									//ド　　ド＃　レ　レ＃　ミ　ファ　ファ＃ソ　ソ＃　ラ　ラ＃シ　ド 消音							//0　　　　1	　　2	  　　3	　　4	 　　5	6	  　　7	　8	  　　9	10	  　　11
//PWMのタイマ設定値テーブル		//ド　　　　ド＃	　　レ	　　　レ＃	　　ミ	　　　ファ	ファ＃	　　　　ソ	　ソ＃	　　　　ラ	ラ＃	　　　　シ
const unsigned short pwm_timer_value[] = {45867,43293,40863,38569,36405,34362,32433,30613,28894,27273,25742,24297,
										  //12	13	　　14  　15  　　16   17   18   19   20   21   22   23
										  //ド   　ド＃　　　　レ　　　レ＃　　　ミ　　　ファ　　　ファ＃　　ソ　　　ソ＃　　ラ　　　　ラ＃　　シ
										  22933,21646,20431,19284,18202,17180,16216,15306,14447,13636,12871,12148,
										  //24  	25   26   27   28   29   30   31   32   33   34   35
										  //ド   　ド＃　　　　レ　　　レ＃　　　ミ　　　ファ　　　ファ＃　　ソ　　　ソ＃　　ラ　　　　ラ＃　　シ
										  11466,10823,10216,9642,9101,8590,8108,7653,7223,6818,6435,6074,
										  //36   37  38   39   40   41    42   43  44   45   46   47
										  //ド　　　ド＃　　レ　　　レ＃　　　ミ　　　ファ　　　ファ＃　　ソ　　　ソ＃　　ラ　　　ラ＃　　シ
										  5733,5411,5108,4821,4550,4295,4054,3826,3611,3409,3217,3037,
											//48   49  50   51   52   53   54   55  56    57   58  59
											//ド　　　ド＃　　レ　　　レ＃　　　ミ　　　ファ　　ファ＃　　ソ　　　ソ＃　  ラ　　 ラ＃　　シ
											2866,2705,2554,2410,2275,2147,2027,1913,1806,1704,1609,1518,
											//60　　　61　　62　　　63　　　64　　　65　　　66　　67　68　 69　 70  71  72 73
											//ド　　　ド＃　　レ　　　レ＃　　　ミ　　　ファ　　ファ＃　　ソ　　ソ＃　ラ　　ラ＃　　シ　　ド
											1433,1353,1277,1205,1137,1073,1013,956,903,852,804,759,716,0,};
//DA出力用の周波数テーブル
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
/*ワークエリア定義																*/
/********************************************************************************/
double g_1_cycle_interruption_number;
double g_once_voltage_change_quantity;
int g_interrupt_count;
int g_wave_type;
//double g_duty_num;
//プロトタイプ宣言
void sawthooth_wave_calculation(void);
void triangle_wave_calculation(void);
void sine_wave_calculation(double onnkai_freq_value);
void square_wave_calculation(void);
void da_process_each_waveform(int wave_type,int sound_num);
void mute(unsigned char speaker_num);

/*
 * スピーカ関連の初期化
 * void speaker_initialize(void)
 */
void speaker_initialize(void)
{
	//消費電力提言機能設定
	SYSTEM.MSTPCRA.BIT.MSTPA8	= 0;	//マルチファンクションタイマパルスユニットのユニット１モジュールストップ解除（PWM出力）
	SYSTEM.MSTPCRA.BIT.MSTPA19	= 0;	//DAコンバータモジュールストップ解除
	//PWMスピーカー
	PORTA.DDR.BIT.B0			= 0;
	//マルチファンクションタイマパルスユニット1チャンネル６の設定（スピーカー１）
	MTU6.TCR.BIT.TPSC			= 2;	//16分周,3MHｚ
	MTU6.TCR.BIT.CCLR			= 1;	//TGRAのコンペアマッチでクリア、立ち上がりエッジでカウント、
	MTU6.TMDR.BYTE				= 0x02;	//通常動作、PWMモードに設定
	MTU6.TIORH.BIT.IOA			= 0x1;	//初期出力はLOW出力コンペアマッチでLOW出力
	MTU6.TIORH.BIT.IOB			= 0x2;	//初期出力はLOW出力コンペアマッチでHI出力
	//マルチファンクションタイマパルスユニット１チャンネル7の設定（スピーカー2）
	MTU7.TCR.BIT.TPSC			= 2;	//16分周,3MHｚ
	MTU7.TCR.BIT.CCLR			= 1;	//TGRAのコンペアマッチでクリア、立ち上がりエッジでカウント、
	MTU7.TMDR.BYTE				= 0x02;	//通常動作、PWMモードに設定
	MTU7.TIOR.BIT.IOA			= 0x1;	//初期出力はLOW出力コンペアマッチでLOW出力
	MTU7.TIOR.BIT.IOB			= 0x2;	//初期出力はLOW出力コンペアマッチでHI出力
	//マルチファンクションタイマパルスユニット１チャンネル8の設定（スピーカー3）
	MTU8.TCR.BIT.TPSC			= 2;	//16分周,3MHｚ
	MTU8.TCR.BIT.CCLR			= 1;	//TGRAのコンペアマッチでクリア、立ち上がりエッジでカウント、
	MTU8.TMDR.BYTE				= 0x02;	//通常動作、PWMモードに設定
	MTU8.TIOR.BIT.IOA			= 0x1;	//初期出力はLOW出力コンペアマッチでLOW出力
	MTU8.TIOR.BIT.IOB			= 0x2;	//初期出力はLOW出力コンペアマッチでHI出力
	//DAコンバーターの設定
	mtu1_initialize();					//MTU1の設定、DA出力用タイマ設定
	DA.DACR.BYTE				= 0xff;	//チャンネル１のアナログ出力許可
}
/********************************************************************************************/
/*スピーカとLEDの出力をセットする								  							*/
/*void set_output_value(unsigned char scale,unsigned char speaker_num)		  				*/
/*			引数：const char scale 出力する音の番号、pwm_timer_valueの添え字番号			*/
/*				  unsigned char speaker_num スピーカー番号									*/
/********************************************************************************************/
void set_output_value(unsigned char scale,unsigned char speaker_num)
{
	SPEAKER *speaker						= get_speaker();
	switch(speaker_num){
	case SPEAKER1:
//		led_lights_out();											//LED消灯
		MTUB.TSTR.BIT.CST0					= 0;					//PWM出力タイマー停止
		MTUA.TSTR.BIT.CST1					= 0;					//DA出力タイマ停止
		DA.DACR.BIT.DAOE1					= 1;					//DA出力許可
		MTU6.TGRA 							= pwm_timer_value[scale];
		MTU6.TGRB							= MTU6.TGRA * (speaker[0].duty_value / 100);
		da_process_each_waveform(speaker[0].wave_type,scale);		//DA出力に必要な処理、波形ごとの処理
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
	//スラーの時はforを通らないようにしたい
//	for(i = 0;i < 200;i++){//音と音の間空ける
//	}
	switch(set_pattern){//セットパターンを見て経過時間カウントに音の長さをセットする
	case 1://1のみ
		autoplayer[0].elapsed_time = autoplayer[0].pnote_value[autoplayer[0].score_count];
		break;
	case 2://2のみ
		autoplayer[1].elapsed_time = autoplayer[1].pnote_value[autoplayer[1].score_count];
		break;
	case 3://1と2
		autoplayer[0].elapsed_time = autoplayer[0].pnote_value[autoplayer[0].score_count];
		autoplayer[1].elapsed_time = autoplayer[1].pnote_value[autoplayer[1].score_count];
		break;
	case 4://3のみ
		autoplayer[2].elapsed_time = autoplayer[2].pnote_value[autoplayer[2].score_count];
		break;
	case 5://1と3
		autoplayer[0].elapsed_time = autoplayer[0].pnote_value[autoplayer[0].score_count];
		autoplayer[2].elapsed_time = autoplayer[2].pnote_value[autoplayer[2].score_count];
		break;
	case 6://2と3
		autoplayer[1].elapsed_time = autoplayer[1].pnote_value[autoplayer[1].score_count];
		autoplayer[2].elapsed_time = autoplayer[2].pnote_value[autoplayer[2].score_count];
		break;
	case 7://1と2と3
		autoplayer[0].elapsed_time = autoplayer[0].pnote_value[autoplayer[0].score_count];
		autoplayer[1].elapsed_time = autoplayer[1].pnote_value[autoplayer[1].score_count];
		autoplayer[2].elapsed_time = autoplayer[2].pnote_value[autoplayer[2].score_count];
		break;
	}
}
/********************************************************************************************/
/*指示されたスピーカーの数出力開始															*/
/*void output_speaker_start(unsigned char pattern)											*/
/*		引数 unsigned char pattern 出力するスピーカーパターン								*/
/********************************************************************************************/
void output_speaker_start(unsigned char pattern)
{
	switch(pattern){
	case 1://スピーカー１のみ出力
		MTUB.TSTR.BYTE					|= 1;
		MTUA.TSTR.BIT.CST1				= 1;//DA出力のタイマースタート
		break;
	case 2://スピーカー２のみ出力
		MTUB.TSTR.BYTE					|= 2;
		break;
	case 3://スピーカー１とスピーカー2出力
		MTUB.TSTR.BYTE					|= 3;
		MTUA.TSTR.BIT.CST1				= 1;//DA出力のタイマースタート
		break;
	case 4://スピーカー３のみ出力
		MTUB.TSTR.BYTE					|= 4;
		break;
	case 5://スピーカー１とスピーカー３出力
		MTUB.TSTR.BYTE					|= 5;
		MTUA.TSTR.BIT.CST1				= 1;//DA出力のタイマースタート
		break;
	case 6://スピーカー２とスピーカー３出力
		MTUB.TSTR.BYTE					|= 6;
		break;
	case 7://スピーカ１とスピーカー２とスピーカー３出力
		MTUB.TSTR.BYTE					|= 7;
		MTUA.TSTR.BIT.CST1				= 1;//DA出力のタイマースタート
		break;
	}
}

/********************************************************************************************/
/*指示されたスピーカーの出力停止															*/
/*void mute(unsigned char speaker_num)														*/
/*		引数 unsigned char speaker スピーカー番号0~3３は全スピーカー消音					*/
/********************************************************************************************/
void mute(unsigned char speaker_num)
{
	switch(speaker_num){
	case SPEAKER1:						//スピーカー１消音DAも
		MTUA.TSTR.BIT.CST1		= 0;
		MTUB.TSTR.BIT.CST0		= 0;
		MTU6.TGRA = MTU6.TGRB 	= 0;
		DA.DACR.BIT.DAOE1		= 0;	//DA出力禁止
		break;
	case SPEAKER2:						//スピーカー2消音
		MTUB.TSTR.BIT.CST1		= 0;
		MTU7.TGRA = MTU7.TGRB 	= 0;
		break;
	case SPEAKER3:						//スピーカー3消音
		MTUB.TSTR.BIT.CST2		= 0;
		MTU8.TGRA = MTU8.TGRB 	= 0;
		break;
	case ALL_SPEAKER:					//全スピーカー消音
		MTUA.TSTR.BIT.CST1		= 0;
		MTUB.TSTR.BYTE	= 0;
		MTU6.TGRA = MTU6.TGRB 	= 0;
		MTU7.TGRA = MTU7.TGRB 	= 0;
		DA.DACR.BIT.DAOE1		= 0;	//DA出力禁止
		break;
	}
}
/********************************************************************************************/
/*DA出力に必要な処理、波形ごとの処理
/*void da_process_each_waveform(int wave_type,int sound_num)
/*
/*
/********************************************************************************************/
void da_process_each_waveform(int wave_type,int sound_num)
{
	g_interrupt_count				= 0;
	g_1_cycle_interruption_number	= (onnkai_freq_value[sound_num] == 0) ? 0 : (1 / onnkai_freq_value[sound_num]) / 0.00000375;//1周期当たりの割り込み回数を求める
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
//MAX4.6V,分解能10ビットなので0.004492187Vが最小幅
//例：1オクターブ目のド、261.626Hz
//		1 / 261.626 = 0.0038222500821784 :1周期の時間
//		0.0038222500821784秒で4.6Vに達する
//		タイマ割り込みの時間　1　/　48MHｚ　＊　180　=　0.00000375秒で1回割り込み
//		0.0038222500821784 / 0.00000375 = 1,019.266688580907
//		1周期の間に1,019回割り込み
//		1023 / 1019 = 1.003925417075564ずつ変化させて1019回割り込まれたら0にする

//4186.009Hzの時		63.70427456478633回割り込み
//TGIA1割り込みDA出力する値を設定する
/********************************************************************************/
/*のこぎり波のDA出力一回のレジスタの変化量計算									*/
/*void sawtooth_wave_calculation(void)											*/
/********************************************************************************/
void sawthooth_wave_calculation(void)
{
	MTU1.TGRA						= 180;
	g_once_voltage_change_quantity 	= (g_1_cycle_interruption_number == 0) ? 0 : 1023 / g_1_cycle_interruption_number;//1回の変化量
}
/********************************************************************************/
/*三角波の一回のDA出力一回のレジスタの変化量計算
/*void triangle_wave_calculation(void)											*/
/********************************************************************************/
void triangle_wave_calculation(void)
{
	MTU1.TGRA						= 180;
	g_once_voltage_change_quantity 	= (g_1_cycle_interruption_number == 0) ? 0 : (1023 / g_1_cycle_interruption_number) * 2;
}
/********************************************************************************/
/*サイン波のDA出力一回のレジスタの変化量計算									*/
/*void sine_wave_calculation(double onnkai_freq_value)							*/
/********************************************************************************/
void sine_wave_calculation(double onnkai_freq_value)
{
	//出力する音階の1周期を１０２３分割
	//タイマーの割り込み間隔の方を変更する
	//1023回割り込みで出力する音階の1周期になるように
	//例：1オクターブ目のド、261.626Hz
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
/*DA出力関数　割り込み処理で選択された波形を出力する							*/
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
