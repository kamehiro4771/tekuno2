
/***********************************************************************/
/*                                                                     */
/*  FILE        : Main.c                                  			   */
/*  DATE        :Tue, Oct 31, 2006                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/
//#include "typedefine.h"
#ifdef __cplusplus
//#include <ios>                        // Remove the comment when you use ios
//_SINT ios_base::Init::init_cnt;       // Remove the comment when you use ios
#endif

/********************************************************************/
/*ヘッダーファイル													*/
/********************************************************************/
#include "main.h"

/********************************************************************/
/*定数定義															*/
/********************************************************************/
const T_DISPLAY E_NEIRO[]														= {"\x1b[2J\x1b[13A******************** e-NeIRO *********************\n"};
const T_DISPLAY DUTY_VALUE[]													= {"デューティー比："};
const T_DISPLAY WAVE_TYPE[]														= {"波形："};
/*項目の種類*/
const T_DISPLAY PLAYLIST_SELECT[]												= {"\x1b[2J\x1b[13A曲を選択して下さい\n"};
const T_DISPLAY WAVETYPE_SELECT[]												= {"音の波形を選択してください\n"};
const T_DISPLAY SETTING_ITEM_SELECT[] 											= {"設定する項目を選択してください\n"};
const T_DISPLAY SETTING_SPEAKER_SELECT[]										= {"設定するスピーカを選択してください\n"};
const T_DISPLAY OUTPUT_SPEAKER_SELECT[]											= {"電子オルガンモード時に出力するスピーカ数を選択してください\n"};

/*項目名*/
const T_DISPLAY MODE_NAME[MODE_NUM][64]											= {"電子オルガンモード","自動演奏モード","ゲームモード","設定",};//モード名
const T_DISPLAY TITLE_NAME[SONG_NUM][64]										= {"アヴェ・マリア","聖者の行進","メヌエット","主よ、人の望みの喜びよ","オーラ・リー","さくら（独唱","情熱大陸",
																	   	   	   	   "Let it Be","NHKのど自慢のテーマ曲","ドラゴンクエスト序曲","レベルアップ","勝利",
																	   	   	   	   "宿屋","攻撃音","冒険の書","戦闘のテーマ","全滅","イニシャルチェック","パッヘルベルのカノン",};
const T_DISPLAY WAVE_TYPE_NAME[WAVE_NUM][64] 									= {{"矩形波"},
																	   	   	   	   {"のこぎり波"},
																	   	   	   	   {"三角波"},
																	   	   	   	   {"サイン波",}};
const T_DISPLAY SETTING_ITME_NAME[SETTING_ITEM_NUM][64] 						= {{"デューティー比"},
																	   	   	   	   {"波形"},
																				   {"スピーカ数"}};
const T_DISPLAY SETTING_SPEAKER_NAME[SPEAKER_NUM][64]							= {{"スピーカ１"},
																	   	   	   	   {"スピーカ２"},
																				   {"スピーカ３"},
																	  	  	  	  };
const T_DISPLAY OUTPUT_SPEAKER_SELECT_NAME[SPEAKER_NUM][64]						= {{"ひとつ"},{"ふたつ"},{"みっつ"}};
/*操作方法*/
const T_DISPLAY END_METHOD[]													= {"メニューに戻る e + エンター\n"};
const T_DISPLAY DUTY_SETTING_METHOD[] 											= {"デューティ比を入力してください（1~99％）\n"
																					"SW1:1　～　SW9:9 SW10:0\n"};
const T_DISPLAY TIMER_SETTING_METHOD[]											= {"SW1:三桁目設定　SW3:二桁目設定　SW5:一桁目設定\r\n"
																				   "SW2:タイマスタート\r\n"};
const T_DISPLAY ERROR_MESSAGE[]													= {"入力が正しくありません\n"};
/*選択出来る項目を変えたい時は以下の配列を変更して、増減があったら#defineのSELECT_NUMを変える*/
const unsigned char SELECTABLE_MODE_ARREY[SELECT_MODE_NUM]						= {ORGAN,AUTOPLAY,GAME,TIMER,SETTING};//選択できるモード
const unsigned char SELECTABLE_TITLE_ARREY[SELECT_PLAY_TITLE_NUM]				= {AVE_MARIA,SAINT_MARCH,JESU_JOY_OF_MAN_S,MENUETT,CANON,DORAGON_QUEST,BATTLE1,WINNING};//この配列に入っている曲がメニューに表示され選択できる
const unsigned char SELECTABLE_WAVE_ARREY[SELECT_WAVE_NUM]						= {SQUARE,SAWTHOOTH,TRIANGLE,SINE};//選択できる波形
const unsigned char SELECTABLE_SETTING_ARREY[SELECT_SETTING_ITEM_NUM]			= {DUTY,WAVE,SPEAKER_NUM};
const unsigned char SELECTABLE_SPEAKER_ARREY[SELECT_SPEAKER_NUM]				= {SPEAKER1,SPEAKER2,SPEAKER3,};
const unsigned char SELECTABLE_OUTPUT_SPEAKER_ARREY[SELECT_OUTPUT_SPEAKER_NUM] 	= {SPEAKER1,SPEAKER2,SPEAKER3,};

/****************************************************************************/
/* ワークエリア定義															*/
/****************************************************************************/
SPEAKER speaker[3]													= {{50,SQUARE,1},{50,SQUARE,2},{50,SQUARE,3}};
unsigned char electronic_organ_speaker								= 1;//電子オルガンモード時にいくつスピーカを鳴らすか//設定モードで変更して電子オルガンモードで参照する
unsigned char seg_timer_song										= 0;
unsigned char timer_value[3]										= {"100"};
unsigned long timer_area											= 0;
/****************************************************************************/
/* プロトタイプ宣言															*/
/****************************************************************************/
void main(void);
static void main_sequence_process(void);
static signed short item_select_sequence(const unsigned char *item_select,const unsigned char (*item_name)[64],const unsigned char *select_num,unsigned char item_num,const unsigned char *end_method);
static void selected_mode_transition(unsigned char select);
static void electronic_organ_mode(void);
static void autplay_mode(void);
static void setting_mode(void);
static void game_mode(void);
static void timer_mode(void);
//unsigned short item_select(const unsigned char *item_select,const unsigned char (*item_name)[64],unsigned char item_num);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif
/****************************************************************************/
/*																			*/
/*							メイン関数										*/
/*																			*/
/****************************************************************************/
//LED1を緑に点灯LED１を青に点灯
void main(void)
{
	eneiro_initialize();
	timer_area_registration(&timer_area);
	while(1)
	{
		timer_mode();
	}
}
/********************************************************************/
/*7セグを利用したタイマ
/*static void timer_mode(void)
/********************************************************************/
/*やりたいこと
 *	７セグを点滅
 *	１セグメントづつ点灯させる
 *
 */
static void timer_mode(void)
{
	unsigned short ret,last_sw_state			= OFF;
	unsigned short i							= 0;
	unsigned short j							= 0;
	T_DISPLAY timer_value[SEG7_DIGIT_NUM]		= {"000"};
	T_DISPLAY led_color_array[LED_COLOR_NUM]	= {RED,GREEN,BLUE,YELLOW,CYAN,MAGENTA,WHITE};
//	send_serial(TIMER_SETTING_METHOD,sizeof(TIMER_SETTING_METHOD));		//操作方法表示
	while(1){
		ret									= sw_check();
		if(ret != OFF && last_sw_state 		!= ret){
			last_sw_state					= ret;
			if(ret == SW2){
				segled_timer_start(timer_value);							//タイマスタート
				break;														//ループを抜ける
			}else{
				switch(ret){
				case SW1:
					timer_value[0]		= timer_value[0] + 1;
					if(timer_value[0] == 0x3a)
						timer_value[0]	= 0x30;
					segled_display_update(&timer_value[0]);					//7セグ表示を更新
					break;
				case SW3:
					timer_value[1]		= timer_value[1] + 1;
					if(timer_value[1] == 0x3a)
						timer_value[1]	= 0x30;
					segled_display_update(&timer_value[0]);					//7セグ表示を更新
					break;
				case SW5:
					timer_value[2]		= timer_value[2] + 1;
					if(timer_value[2] == 0x3a)
						timer_value[2]	= 0x30;
					segled_display_update(&timer_value[0]);					//7セグ表示を更新
					break;
				}
			}
		}else
			last_sw_state			= ret;
	}
	while(1){																//カウントダウン終了まで待機
		if(timer_value[0] == '0' && timer_value[1] == '0' && timer_value[2] == '0'){
			break;
		}
	}
	automatic_playing_start(CANON,SQUARE,0,0,0);
	while(playing_flg == ON){//演奏中
		if(timer_area == 0){
			led_lights_out();
			for(i = 1;i <= LED_NUM;i++){
				output_led(i,led_color_array[j],0);
			}
			j++;
			if(j == 7)
				j = 0;
			timer_area = 1000;
		}
	}
	led_lights_out();
}
/****************************************************************************/
/*メインシーケンス															*/
/*void main_sequence_process(void)											*/
/****************************************************************************/
static void main_sequence_process(void)
{
	static unsigned char main_sequence_num = 0;
	signed char ret;
	switch(main_sequence_num){
		case 0://モード選択
			ret = item_select_sequence(E_NEIRO,MODE_NAME,SELECTABLE_MODE_ARREY,SELECT_MODE_NUM,NULL);
			if(ret != -1 && ret != 'e')
				main_sequence_num++;
			break;
		case 1://各モードに移行
			selected_mode_transition(ret);
			main_sequence_num = 0;
			break;
	}
}
/************************************************************************************/
/*選択項目表示																		*/
/*void selection_screen_display(char *item,char *item_name,unsigned char item_num)	*/
/*			const unsigned char *item:選択する内容									*/
/*			const unsigned char *item_name:選択する項目名配列へのポインタ			*/
/*			const unsigned char *select_num選択できる項目の配列番号の配列へのポインタ			*/
/*			char item_num:項目の数													*/
/************************************************************************************/
//こんなにたくさんsend_serialしないといけないか
static void selection_screen_display(const T_DISPLAY *select_item,const T_DISPLAY (*item_name)[64],T_DISPLAY *select_num,unsigned char item_num,T_DISPLAY *end_method)
{
	unsigned char i;
	unsigned char index_num[8];
	send_serial(RESET,10);//画面をリセット
	send_serial(select_item,strlen((const char*)select_item));
	for(i = 1;i <= item_num;i++){
		sprintf((char *)index_num,"%d:",i);
		send_serial(index_num,2);
		send_serial(item_name[select_num[i - 1] - 1],strlen((const char *)item_name[select_num[i - 1] - 1]));
		send_serial("\n",1);
		while(sci0_get_reg_0_flg() != ON){
			//index_numが書き換えられるとおかしくなるので送信完了するまで待つ
		}
	}
	if(end_method != NULL)//終了操作ひょうじするか
		send_serial(end_method,strlen((const char*)end_method));
}

/*********************************************************************************************************************************/
/*アイテムセレクトシーケンス																									 */
/*signed short item_select_sequence(const unsigned char *item_select,const unsigned char (*item_name)[64],unsigned char item_num)*/
/*		引数：const unsigned char *item_select
/*			const unsigned char (*item_name)[64]
 * 			const unsigned char *select_num
 * 			const unsigned char *end_method
/*			unsigned char item_num
/*********************************************************************************************************************************/
static signed short item_select_sequence(const T_DISPLAY *item_select,const T_DISPLAY (*item_name)[64],const T_DISPLAY *select_num,unsigned char item_num,const T_DISPLAY *end_method)
{
	static unsigned char item_select_sequence_num = 0;
	static signed char ret;
	switch(item_select_sequence_num){
	case 0://画面表示
		sci0_receive_start();//受信開始
		selection_screen_display(item_select,item_name,select_num,item_num,end_method);//選択画面が表示される
		item_select_sequence_num++;
		break;
	case 1://入力待ち
		ret = input_check();
		if(ret == ON){
			ret			= a_to_i();//受信データを数値にして受け取る
			item_select_sequence_num++;
		}else if(ret != OFF)
			item_select_sequence_num++;
		break;
	case 2://入力値の判定
		item_select_sequence_num		= 0;
		if(ret <= item_num && ret > 0)
			return select_num[ret - 1];
		if(sci0_find_received_data('e'))
			return 'e';
		break;
	}
	return -1;
}
/*********************************************************************************************************/
/*デューティ比の設定																					 */
/*void duty_setting(unsigned char speaker_num)															 */
/*	unsigned char speaker_num スピーカ番号																 */
/*********************************************************************************************************/
static void duty_setting(void)
{
	signed long ret						= OFF;
	signed short speaker_num			= -1;
	unsigned char duty_one_digits		= NO_SELECT;
	unsigned char duty_two_digits		= NO_SELECT;
	while(speaker_num == -1){
		speaker_num = item_select_sequence(SETTING_SPEAKER_SELECT,SETTING_SPEAKER_NAME,SELECTABLE_SPEAKER_ARREY,SELECT_SPEAKER_NUM,END_METHOD);
	}
	sci0_receive_start();
	send_serial(DUTY_SETTING_METHOD,sizeof(DUTY_SETTING_METHOD));
	while(1){
		ret						= input_check();
		if(ret == ON){
			ret					= a_to_i();
			if(ret > 99 || ret < 1){
				sci0_receive_start();
				ret				= OFF;
			}else
				break;
		}else if(ret != OFF){
			//スイッチ入力の時
			if(duty_one_digits == NO_SELECT){
				if(ret < SW11)
					duty_one_digits = ret;
			}else if(ret < SW11){
				duty_two_digits = ret;
				ret  = ((duty_one_digits % 10) * 10) + duty_two_digits % 10;
				if(ret < 1)
					duty_one_digits = duty_two_digits = NO_SELECT;
				else
					break;
			}
		}
	}
	speaker[speaker_num - 1].duty_value		= ret;
}
/********************************************************************/
/*選択したモードに遷移する											*/
/*void selected_mode_transition(int *select)						*/
/*	
/********************************************************************/
static void selected_mode_transition(unsigned char select)
{
	switch(select){
	case ORGAN:
		electronic_organ_mode();
		break;
	case AUTOPLAY:
		autplay_mode();
		break;
	case GAME:
		game_mode();
		break;
	case TIMER:
		timer_mode:
		break;
	case SETTING:
		setting_mode();
		break;
	default:
		send_serial(ERROR_MESSAGE,sizeof(ERROR_MESSAGE));
		break;
	}
}

/********************************************************************/
/*電子オルガンモード関数											*/
/*unsigned char electronic_organ_mode(void)							*/
/********************************************************************/
static void electronic_organ_mode(void)
{
	unsigned char ret				= OFF,i;
	unsigned char last_sw_state		= OFF;
	send_serial(END_METHOD,sizeof(END_METHOD));
	sci0_receive_start();//受信開始
	while(1){
		ret							= sci0_enter_check();
		if(ret == ON){
			if(sci0_find_received_data('e') != NOT_FOUND){
				mute(SPEAKER1);
				for(i = 1;i < 9;i++)
					output_led(i,BLACK,0);
				break;
			}else
				sci0_receive_start();//受信開始
		}
		ret							= sw_check();
		if(ret != last_sw_state){
			last_sw_state				= ret;
			switch(electronic_organ_speaker){
			case 1:
				set_output_value(last_sw_state,SPEAKER1);
				output_speaker_start(1);
				break;
			case 2:
				set_output_value(last_sw_state,SPEAKER1);
				set_output_value(last_sw_state + 12,SPEAKER2);
				output_speaker_start(3);
				break;
			case 3:
				set_output_value(last_sw_state,SPEAKER1);
				set_output_value(last_sw_state + 12,SPEAKER2);
				set_output_value(last_sw_state + 24,SPEAKER3);
				output_speaker_start(7);
				break;
			}
		}
	}
}

/********************************************************************/
/*自動演奏モード関数												*/
/*void autplay_mode(void)											*/
/********************************************************************/
static void autplay_mode(void)
{
	unsigned char ret			= 0;
	signed short title			= -1;
	signed short wave_type		= -1;
	while(title == -1){
		title = item_select_sequence(PLAYLIST_SELECT,TITLE_NAME,SELECTABLE_TITLE_ARREY,SELECT_PLAY_TITLE_NUM,END_METHOD);
	}
	if(title == 'e')
		return;//タイトル選択でeが入力されたらメニューへ戻る
	while(wave_type == -1){
		wave_type = item_select_sequence(WAVETYPE_SELECT,WAVE_TYPE_NAME,SELECTABLE_WAVE_ARREY,SELECT_WAVE_NUM,END_METHOD);
	}
	if(wave_type == 'e')
		return;//波形選択でeが入力されたらメニューへ戻る
	send_serial(TITLE_NAME[title - 1],sizeof(TITLE_NAME[title - 1]));
	send_serial(WAVE_TYPE_NAME[wave_type - 1],sizeof(WAVE_TYPE_NAME[wave_type - 1]));
	automatic_playing_start((unsigned short)title,wave_type,0,0,0);
	while(playing_flg == ON){
		ret = input_check();
		if(ret != OFF)
			auto_play_end_processing();
	}
}

/********************************************************************/
/*ゲームモード関数													*/
/*void game_mode(void)											*/
/********************************************************************/
static void game_mode(void)
{
	game_main();
}


/********************************************************************/
/*設定モード関数													*/
/*void setting_mode(void)											*/
/********************************************************************/
static void setting_mode(void)
{
	signed short setting_num			= -1;
	while(setting_num == -1){
		setting_num = item_select_sequence(SETTING_ITEM_SELECT,SETTING_ITME_NAME,SELECTABLE_SETTING_ARREY,SETTING_ITEM_NUM,END_METHOD);
	}
	switch(setting_num){
	case DUTY:
//		デューティ比のスピーカごとの設定ができない
		duty_setting();
		break;
	case WAVE:
		do{
			setting_num = item_select_sequence(WAVETYPE_SELECT,WAVE_TYPE_NAME,SELECTABLE_WAVE_ARREY,SELECT_WAVE_NUM,END_METHOD);
		}while(setting_num == -1);
		speaker[0].wave_type= setting_num;
		break;
	case SPEAKER_NUM://電子オルガンモード時に鳴らすスピーカの番号指定
		do{
			setting_num = item_select_sequence(OUTPUT_SPEAKER_SELECT,OUTPUT_SPEAKER_SELECT_NAME,SELECTABLE_OUTPUT_SPEAKER_ARREY,SELECT_OUTPUT_SPEAKER_NUM,END_METHOD);
		}while(setting_num == -1);
		electronic_organ_speaker = setting_num;
		break;
	case 'e':
		break;
	}
}
/*
 * エンターキーとスイッチ入力の判定
 *unsigned char input_check(void)
 *	戻り値：unsigned char ON:スイッチが押してから離されたエンターキーが入力されていた　OFF:入力なし
 */
 /*
unsigned char input_check(void)
{
	unsigned char ret						= OFF;
	static unsigned char sw_state			= OFF;
	static unsigned char last_sw_state		= OFF;
	sw_state				= sw_check();
	if(sw_state != OFF){//スイッチが押されていたら
		last_sw_state		= sw_state;//スイッチの状態記録
	}else if(sci0_enter_check() == ON){//スイッチが押されている時はエンターは見ない
		ret				= ON;
	}else if(last_sw_state != OFF){//スイッチが離された
		ret				= last_sw_state;
		last_sw_state	= OFF;
	}
	return ret;
}
*/
//押されたらONを返す仕様
//スイッチ番号またはENTER＿ONを返す仕様
unsigned char input_check(void)
{
	unsigned char ret						= OFF;
	unsigned char sw_state					= OFF;
	sw_state								= sw_check();
	if(sw_state != OFF)								//スイッチが押されていたら
		ret									= sw_state;	//スイッチの状態記録
	else if(sci0_enter_check() == ON)					//スイッチが押されている時はエンターは見ない
		ret									= ENTER_ON;
	return ret;
}

struct SPEAKER *get_speaker(void)
{
	return speaker;
}


#ifdef __cplusplus
void abort(void)
{

}
#endif
