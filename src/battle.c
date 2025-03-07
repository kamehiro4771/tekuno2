

/********************************************************************/
/*ヘッダーファイル													*/
/********************************************************************/
#include "main.h"
/*********************************************************************/
/*定数定義															 */
/*********************************************************************/
const T_DISPLAY APPEAR_DISPLAY[]			= {"が現れた！\n"};
const T_DISPLAY ATTACK_DISPLAY[]			= {"の攻撃！\n"};
const T_DISPLAY COMBO_DISPLAY[]				= {"コンボ！\n"};
const T_DISPLAY TO_DISPLAY[]				= {"に"};
const T_DISPLAY DAMAGE_DISPLAY[]			= {"のダメージ"};
const T_DISPLAY TAKE_DISPLAY[]				= {"をうけた！\n"};
const T_DISPLAY LIFE_JEWEL_DISPLAY[]		= {"は命の宝石を使った"};
const T_DISPLAY RECOVERY_DISPLAY[]			= {"回復！\n"};
const T_DISPLAY KILL_DISPLAY[]				= ("を倒した！\n");
const T_DISPLAY REQUEST_COMMAND_DISPLAY[]	= {"コマンド?>>"};
const T_DISPLAY OPERATION_METHOD_DISPLAY[]	= {"一文字目動かす宝石の現在地、2文字目動かし先"};
const T_DISPLAY INPUT_ERROR_DISPLAY[]		= {"入力が正しくありません"};
const T_DISPLAY HP_DISPLAY[]				= {"HP="};
//表示する文字列へのポインタ二次元配列の行は行動、列は表示する順番
/*const T_DISPLAY *DISPLAY_POINTER_ARRAY[][]		= {{APPEAR,,},//モンスター登場
													{,ATTACK,,TO,DAMAGE,},//モンスターに攻撃を与える
													{ATTACK,DAMAGE,TAKE},//モンスターから攻撃をもらう
													{KILL},//モンスターを倒した
													{HP,},//ステータス表示
													{},
};*/
const T_DISPLAY COLOR_CHAR_ARRAY[COLOR_NUM][6] 	= {RED_CHAR,BLUE_CHAR,GREEN_CHAR,YELLOW_CHAR,PURPLE_CHAR};
enum activity{APPEARANCE,ADD_ATTACK,TAKE_ATTACK,KILLED_ENEMY,RECOVERY,STATUS,COMBO};
/*********************************************************************/
/*ワークエリア定義													 */
/*********************************************************************/
AUTOPLAYER *resume_data;
T_ENEMY *penemy;//戦闘中のモンスターへのポインタ
T_PLAYER *pplayer;//プレイヤーへのポインタ
T_ALLY attack_ally;//攻撃したモンスター
unsigned char output_string[7][128];//ダメージ計算して文字列に変換したやつが入る
unsigned char first_turn_flg;
/********************************************************************/
/*プロトタイプ宣言													*/
/********************************************************************/
void player_turn(void);
void enemy_turn(void);
void i_to_a(unsigned short i,unsigned char *output_string);
static void battle_display(unsigned char activity,unsigned char *param);
/********************************************************************/
/*バトルメイン関数													*/
/*unsigned char battle_main(struct T_ENEMY* enemy)					*/
/*  引数：struct T_ENEMY* enemy 対戦している敵情報					*/
/*	戻り値：unsigned char ret 1勝利									*/
/*							  0敗北									*/
/********************************************************************/
unsigned char battle_main(T_PLAYER *player, T_ENEMY *enemy)
{
	//モンスターを倒した時おかしくなる
	first_turn_flg 					= ON;
	penemy					= enemy;
	pplayer					= player;
	battle_display(APPEARANCE,NULL);
	while(1){
		player_turn();
		if(penemy->hp == 0)
			return 1;
		enemy_turn();
		if(pplayer->hp <= 0)
			return 0;
		first_turn_flg 			= OFF;
	}
}


/********************************************************************/
/*プレーヤーのターン関数											*/
/*void player_turn(T_PLAYER *player,T_ENEMY* enemy)							*/
/*	引数：struct T_ENEMY* enemy 戦闘中の敵のデータ					*/
/********************************************************************/
void player_turn(void)
{
	unsigned char i,ret;
	unsigned char input[2] = {0};
	unsigned char *dladder = NULL;
	AUTOPLAYER *pautoplayer = get_autoplayer();
	battle_display(STATUS,NULL);
	if(first_turn_flg == ON){
		automatic_playing(BATTLE1,SQUARE,0,0,0);
		output_battle_field(NEW_FIELD);
	}else{
		output_battle_field(CURRENT_FIELD);
	}
	send_serial(REQUEST_COMMAND_DISPLAY,sizeof(REQUEST_COMMAND_DISPLAY));
	while(1){
		ret = input_check();
		if(ret == ON){
			ret						= sci0_get_receive_count();
			if(ret == 4){//2文字入力された
				sci0_data_cpy(&input[0]);
				if(input[0] >= 'A' && input[0] < 'N'){//1文字目判定
					if(input[1] >= 'A' && input[1] < 'N')//2文字目判定
						break;
					if(input[1] >= 'a' && input[1] < 'n')
						break;
				}
				if(input[0] >= 'a' && input[0] < 'n'){//1文字目a~m以外の入力
					if(input[1] >= 'A' && input[1] < 'N')//2文字目判定
						break;
					if(input[1] >= 'a' && input[1] < 'n')
						break;
				}
			}else{
				//一行上に戻って「コマンド>>」表示
				sci0_receive_start();
			}
		}else if(playing_flg == OFF){//何も入力されずに曲終了
			automatic_playing(BATTLE1,SQUARE,0,0,0);
		}
	}
//	ml入力でバグる
	move_jewel(input[0],input[1]);//宝石を動かす
	while(1){
		dladder							= count_jewel();//3つ以上宝石が一致していたら配列のアドレスを返す。一致してなかったらNULLを返す
		if(dladder != NULL){
			auto_play_end_processing();
			for(i = 0;i < 3;i++)
				resume_data[i]			= get_interrupt_data(i);
			automatic_playing(ALLY_ATACK,SQUARE,0,0,0);//攻撃音演奏
			while(playing_flg == ON){
				//nop
			}
			if(*dladder != LIFE){
				attack_ally = get_ally_data(*dladder);
				battle_display(ADD_ATTACK,dladder);
			}else
				battle_display(RECOVERY,dladder);
			pautoplayer[0] = resume_data[0];
			pautoplayer[1] = resume_data[1];
			pautoplayer[2] = resume_data[2];
			//演奏再開
			automatic_playing(BATTLE1,SQUARE,pautoplayer[0].score_count,pautoplayer[1].score_count,pautoplayer[2].score_count);
			free_padding(dladder);//空いた宝石配列を詰める
		}else{
			sci0_receive_start();//受信が終わっているので開始
			break;
		}
	}
	send_serial(CURSOR_2LINE_ADVANCE,4);
}

/********************************************************************
/*敵のターン関数
/*void enemy_turn(void)
/*	引数：struct Enemy* enemy　攻撃する敵の情報
/*	戻り値：unsigned char
********************************************************************/
void enemy_turn(void)
{
	battle_display(TAKE_ATTACK,NULL);
}
/**************************************************************************
 * バトル中の画面表示
 * static void battle_display(unsigned char activity,unsigned char *param)
 * 	引数：unsigned char activity　APPEARANCE　敵が現れた
 * 							ADD_ATTACK　敵に攻撃を加えた
 * 							TAKE_ATTACK　敵に攻撃された
 * 							KILLED_ENEMY　敵を倒した時の表示
 * 							RECOVERY　回復表示
 * 							STATUS　ステータス表示
 * 		unsigned char *param　消去した宝石の先頭アドレス、又はNULL
 */

static void battle_display(unsigned char activity,unsigned char *param)
{
	unsigned short damage_value = 0;
	unsigned short combo_value = 0;//コンボカウントのリセットを忘れないように
	unsigned char deleted_number;
	if(activity == ADD_ATTACK || activity == RECOVERY){
		/*ダメージ計算とコンボ数表示*/
		combo_value++;//
		deleted_number						= delete_jewel(param);//宝石を消して、消した宝石数をもらう
		damage_value						= damage_to_enemy_calculation(penemy,combo_value,attack_ally.el,deleted_number);//ダメージ計算
		if(combo_value >= 2){//コンボカウントが２以上ならコンボ数表示
			sprintf(output_string[COMBO],"%d%s",combo_value,COMBO_DISPLAY);
			send_serial(output_string[COMBO],strlen(output_string[COMBO]));
		}
	}
	if(activity == RECOVERY){
		/*回復表示の時*/
		sprintf(output_string[RECOVERY],"%s%s%d%s",pplayer->name,LIFE_JEWEL_DISPLAY,damage_value,RECOVERY_DISPLAY);
		pplayer->hp = pplayer->hp + damage_value;//自分のHPにダメージを足す最大値を超えないように
		if(pplayer->hp > pplayer->mhp)
			pplayer->hp = pplayer->mhp;
	}else{
		switch(activity){
		case APPEARANCE:
			sprintf(output_string[APPEARANCE],"%s%s%s%s",COLOR_CHAR_ARRAY[penemy->el],penemy->name,DEFAULT_CHAR,APPEAR_DISPLAY);
			break;
		case ADD_ATTACK:
			sprintf(output_string[ADD_ATTACK],"%s%s%s%s%s%s%s%s%d%s%s",COLOR_CHAR_ARRAY[attack_ally.el],attack_ally.name,DEFAULT_CHAR,ATTACK_DISPLAY,COLOR_CHAR_ARRAY[penemy->el],penemy->name,DEFAULT_CHAR,TO_DISPLAY,damage_value,DAMAGE_DISPLAY,CRLF);
			if(penemy->hp >= damage_value)
				penemy->hp 			= penemy->hp - damage_value;//モンスターのHPからダメージを引く
			else
				penemy->hp 			= 0;
			break;
		case TAKE_ATTACK:
			damage_value			= damge_from_enemy_calculation(pplayer->gp,penemy);
			pplayer->hp				= pplayer->hp - damage_value;
			sprintf(output_string[TAKE_ATTACK],"%s%s%s%s%d%s%s",COLOR_CHAR_ARRAY[penemy->el],penemy->name,DEFAULT_CHAR,ATTACK_DISPLAY,damage_value,DAMAGE_DISPLAY,TAKE_DISPLAY);
			break;
		case KILLED_ENEMY:
			sprintf(output_string[KILLED_ENEMY],"%s%s%s%s%s%s%s",COLOR_CHAR_ARRAY[penemy->el],penemy->name,DEFAULT_CHAR,COLOR_CHAR_ARRAY[penemy->el],penemy->name,DEFAULT_CHAR,KILL_DISPLAY);
			send_serial(KILL_DISPLAY,sizeof(KILL_DISPLAY));
			break;
		case STATUS:
			sprintf(output_string[STATUS],"%s%s%s%s%s%s%s%d/%d%s",CURSOL_MOVING_SENTER,COLOR_CHAR_ARRAY[penemy->el],penemy->name,DEFAULT_CHAR,CRLF,CURSOL_MOVING_SENTER,HP_DISPLAY,penemy->hp,penemy->mhp,CRLF);
			break;
		}
	}
	send_serial(output_string[activity],strlen(output_string[activity]));
}
//敵と味方のステータスを表示する
/********************************************************************/
/*数値を文字列に変換する											*/
/*void i_to_a(unsigned short i)										*/
/*	引数：unsigned short i 数値										*/
/********************************************************************/
//入力は65535まで
//出力はワークエリア定義のoutput_stringに変換される
//最後にNULL文字を入れる
void i_to_a(unsigned short i,unsigned char *output_string)
{
	if(i > 9999){
		output_string[0]	= (i / 10000) + 0x30;
		output_string[1]	= ((i / 1000) % 10) + 0x30;
		output_string[2]	= ((i / 100) % 10) + 0x30;
		output_string[3]	= ((i / 10) % 10) + 0x30;
		output_string[4]	= (i % 10) + 0x30;
		output_string[5]		= '\0';
	}else if(i > 999){
		output_string[0]	= ((i / 1000) % 10) + 0x30;
		output_string[1]	= ((i / 100) % 10) + 0x30;
		output_string[2]	= ((i / 10) % 10) + 0x30;
		output_string[3]	= (i % 10) + 0x30;
		output_string[4]		= '\0';
	}else if(i > 99){
		output_string[0]	= ((i / 100) % 10) + 0x30;
		output_string[1]	= ((i / 10) % 10) + 0x30;
		output_string[2]	= (i % 10) + 0x30;
		output_string[3]		= '\0';
	}else if(i > 9){
		output_string[0]	= ((i / 10) % 10) + 0x30;
		output_string[1]	= (i % 10) + 0x30;
		output_string[2]		= '\0';
	}else{
		output_string[0]	= (i % 10) + 0x30;
		output_string[1]		= '\0';
	}
}

/********************************************************************/
/*else if(ret == 3){
				if(input[i] < 0x41 || input[i] > 0x4d)//1文字目A~M以外の入力
					//nop
				else if(input[i] < 0x61 || input[i] > 0x6d)//1文字目a~m以外の入力
					//nop
				else{
					i++;
					if(i == 2)
						break;
				}
			}
			if(i == 0){
				send_serial(INPUT_ERROR,sizeof(INPUT_ERROR));
				input[0]	= 0;
				input[1]	= 0;
				i			= 0;
			}
		}else if(ret >= SW1 && ret <= SW13){//スイッチが押されたとき
			g_start[0]	= speaker[0].score_count;
			g_start[1]	= speaker[1].score_count;
			g_start[2]	= speaker[2].score_count;
			input[i]	= ret;
			i++;*/


