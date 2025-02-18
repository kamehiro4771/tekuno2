
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

const unsigned char E_NEIRO[]		= {"\x1b[2J\x1b[0;0H\x1b[2J\x1b[13A******************** e-NeIRO *********************\n"
										"１.電子オルガンモード\n"
										"２.自動演奏モード\n"
										"３.ゲームモード\n"
										"４.設定\n\x1b[49m"
										};
const unsigned char DUTY_VALUE[]			= {"デューティー比："};
const unsigned char WAVE_TYPE[]				= {"波形："};
const unsigned char playlists_select[]		= {"\x1b[2J\x1b[13A曲を選択して下さい\n"
										};
const unsigned char wavetype_select[]		= {"音の波形を選択してください\n"};
const unsigned char setting_item_select[] 	= {"設定する項目を選択してください\n"};
const unsigned char end_method[]				= {"メニューに戻る e + エンター\n"};
const unsigned char title_name[SONG_NUM][64]	= {{"1.アヴェ・マリア\n"},
										{"2.聖者の行進\n"},
										{"3.メヌエット\n"},
										{"4.主よ、人の望みの喜びよ\n"},
										{"5.オーラ・リー\n"},
										{"6.さくら（独唱）\n"},
										{"7.情熱大陸\n"},
										{"8.Let it Be\n"},
										{"9.NHKのど自慢のテーマ曲\n"},
										{"10ドラゴンクエスト序曲\n"},};									//9.少年時代　10．戦場のメリークリスマス
const unsigned char wave_type_name[WAVE_NUM][64] 	= {{"1.矩形波\n"},
											{"2.のこぎり波\n"},
											{"3.三角波\n"},
											{"4.サイン波\n",}};

const unsigned char setting_item_name[SETTING_ITEM_NUM][64] = {{"1.デューティー比\n"},
													{"2.波形\n"},
													{"3.キーの高さ\n"}};

const unsigned char duty_setting_display[] 	= {"デューティ比を入力してください（1~99％）\n"
										"SW1:1　～　SW9:9 SW10:0\n"};
const unsigned char setting_comp[]			= {"に設定されました。\n"};
const unsigned char error_message[]			= {"入力が正しくありません\n"};
/*
 * ワークエリア定義
 */
struct SPEAKER speaker[3]			= {{NULL,NULL,0,0,50,SQUARE,0,1,OFF,OFF},
										{NULL,NULL,0,0,50,SQUARE,0,2,OFF,OFF},
										{NULL,NULL,0,0,50,SQUARE,0,3,OFF,OFF}};
unsigned char g_output_string[512];
unsigned char g_select;
/*
 * プロトタイプ宣言
 */
void main(void);
void main_sequence_process(void);
signed short item_select_sequence(const unsigned char *item_select,const unsigned char (*item_name)[64],unsigned char item_num);
void selected_mode_transition(unsigned char select);
void electronic_organ_mode(void);
void autplay_mode(void);
void setting_mode(void);
void game_mode(void);
void duty_setting(unsigned char speaker_num);
int semibreve_value_setting(int semibreve);
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
void main(void)
{
	eneiro_init();
	sci0_init(BAUD_RATE);			//シリアル通信モージュールの初期化
	automatic_playing(INITIAL_CHECK,SQUARE,0,0,0);
	while(1)
	{
		main_sequence_process();
	}
}

/****************************************************************************/
/*メインシーケンス															*/
/*void main_sequence_process(void)											*/
/****************************************************************************/
void main_sequence_process(void)
{
	static unsigned char main_sequence_num = 0;
	signed char ret;
	switch(main_sequence_num){
		case 0:
			sci0_receive_start();//受信開始
			sprintf((char *)g_output_string,"%s%s%lf％ %s%s＊SW1~SW3又はキーボード1~3+エンター\n**************************************************\n"
					,E_NEIRO,DUTY_VALUE,speaker[0].duty_value,WAVE_TYPE,wave_type_name[speaker[0].wave_type - 1]);
			send_serial(g_output_string,strlen(g_output_string));
			main_sequence_num++;
			break;
		case 1:
			ret = input_check();
			if(ret == ON){
				ret			= a_to_i();
				main_sequence_num++;
			}else if(ret != OFF)
				main_sequence_num++;
			break;
		case 2:
			selected_mode_transition(ret);
			main_sequence_num = 0;
			break;
	}
}
/****************************************************************************/
/*選択項目表示																*/
/*void selection_screen_display(char *item,char *item_name,unsigned char item_num)	*/
/*			char *item:選択する内容											*/
/*			char *item_name:選択する項目名									*/
/*			char item_num:項目の数											*/
/****************************************************************************/
static void selection_screen_display(const unsigned char *item,const unsigned char (*item_name)[64],unsigned char item_num)
{
	int i;
	send_serial(RESET,10);
	send_serial(item,strlen((const char*)item));
	for(i = 0;i < item_num;i++){
		send_serial(item_name[i],sizeof(item_name[i]));
	}
	send_serial(end_method,sizeof(end_method));
}

/*********************************************************************************************************************************/
/*アイテムセレクトシーケンス																									 */
/*signed short item_select_sequence(const unsigned char *item_select,const unsigned char (*item_name)[64],unsigned char item_num)*/
/*
/*
/*
/*
/*
/*********************************************************************************************************************************/
signed short item_select_sequence(const unsigned char *item_select,const unsigned char (*item_name)[64],unsigned char item_num)
{
	static unsigned char item_select_sequence_num = 0;
	unsigned char ret;
	switch(item_select_sequence_num){
	case 0:
		sci0_receive_start();//受信開始
		selection_screen_display(item_select,item_name,item_num);//選択画面が表示される
		item_select_sequence_num++;
		break;
	case 1:
		ret = input_check();
		if(ret == ON){
			if(sci0_find_received_data('e'))
				return 'e';
			g_select			= a_to_i();//受信データを数値にして受け取る
			item_select_sequence_num++;
		}else if(ret != OFF){
			g_select			= ret;
			item_select_sequence_num++;
		}
		break;
	case 2:
		if(g_select > item_num)
			item_select_sequence_num		= 0;
		else{
			item_select_sequence_num			= 0;
			return g_select;
		}
	}
	return -1;
}
/********************************************************************/
/*選択したモードに遷移する											*/
/*void selected_mode_transition(int *select)						*/
/*	
/********************************************************************/
void selected_mode_transition(unsigned char select)
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
	case SETTING:
		setting_mode();
		break;
	default:
		send_serial(error_message,sizeof(error_message));
		break;
	}
}

/********************************************************************/
/*電子オルガンモード関数											*/
/*unsigned char electronic_organ_mode(void)							*/
/********************************************************************/
//スイッチを押しながらエンターが押されるとスイッチを離した時にメニューに戻る音が鳴りっぱなしになる
void electronic_organ_mode(void)
{
	int output_num				= OFF;
	int last_output_num			= OFF;
	send_serial(end_method,sizeof(end_method));
	sci0_receive_start();//受信開始
	while(1){
		output_num				= sw_check();
		if(output_num == OFF){//スイッチがOFFなら
			if(sci0_enter_check() == ON){//エンターが押されたか確認する
				set_output_value(output_num,0);
				break;
			}
		}
		if(last_output_num != output_num){
			last_output_num			= output_num;
			set_output_value(output_num,0);
			output_speaker_start(1);
		}
	}
}

/********************************************************************/
/*自動演奏モード関数												*/
/*void autplay_mode(void)											*/
/********************************************************************/
void autplay_mode(void)
{
	unsigned char ret			= 0;
	signed short title			= -1;
	signed short wave_type		= -1;
	while(title == -1){
		title = item_select_sequence(playlists_select,title_name,SONG_NUM);
	}
	if(title == 'e')
		return;//タイトル選択でeが入力されたらメニューへ戻る
	while(wave_type == -1){
		wave_type = item_select_sequence(wavetype_select,wave_type_name,WAVE_NUM);
	}
	if(wave_type == 'e')
		return;//波形選択でeが入力されたら自動演奏モード終了
	send_serial(title_name[title - 1],sizeof(title_name[title - 1]));
	send_serial(wave_type_name[wave_type - 1],sizeof(wave_type_name[wave_type - 1]));
	automatic_playing((unsigned short)title,wave_type,0,0,0);
	while(playing_flg == ON){
		ret = input_check();
		if(ret != OFF)
			auto_play_end_processing();
	}
}

/********************************************************************/
/*設定モード関数													*/
/*void setting_mode(void)											*/
/********************************************************************/
void setting_mode(void)
{
	signed short setting_num			= -1;
	while(1){
		while(setting_num == -1){
			setting_num = item_select_sequence(setting_item_select,setting_item_name,SETTING_ITEM_NUM);
		}
		switch(setting_num){
		case DUTY:
			duty_setting(0);
			return;
		case WAVE:
			item_select_sequence(wavetype_select,wave_type_name,WAVE_NUM);
			return;
		case 3:
	//		while(semibreve_value_setting(semibreve_value));
			break;
		case 101:
			return;
		}
	}
}
#if 0
void game_mode(void)
{
	game_main();
}
#endif

/*********************************************************************************************************/
/*
/*void duty_setting(void)
/*********************************************************************************************************/

void duty_setting(unsigned char speaker_num)
{
	signed long ret					= OFF;
	unsigned char duty_one_digits		= NO_SELECT;
	unsigned char duty_two_digits		= NO_SELECT;
	send_serial(duty_setting_display,sizeof(duty_setting_display));
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
	speaker[speaker_num].duty_value		= ret;
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
