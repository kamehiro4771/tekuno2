

/********************************************************************/
/*ヘッダーファイル															*/
/********************************************************************/
#include "main.h"
/*********************************************************************/
/*定数定義															 */
/*********************************************************************/
const T_DISPLAY APPEAR_DISPLAY				= {"が現れた！\n"};
const T_DISPLAY ATTACK_DISPLAY				= {"の攻撃！\n"};
const T_DISPLAY COMBO_DISPLAY				= {"コンボ！\n"};
const T_DISPLAY TO_DISPLAY					= {"に"};
const T_DISPLAY DAMAGE_DISPLAY				= {"のダメージ"};
const T_DISPLAY TAKE_DISPLAY				= {"をうけた！\n"};
const T_DISPLAY LIFE_JEWEL_DISPLAY			= {"は命の宝石を使った"};
const T_DISPLAY RECOVERY_DISPLAY			= {"回復！\n"};
const T_DISPLAY KILL_DISPLAY				= ("を倒した！\n");
const T_DISPLAY REQUEST_COMMAND_DISPLAY		= {"コマンド?>>"};
const T_DISPLAY OPERATION_METHOD_DISPLAY	= {"一文字目動かす宝石の現在地、2文字目動かし先"};
const T_DISPLAY INPUT_ERROR_DISPLAY			= {"入力が正しくありません"};
const T_DISPLAY HP_DISPLAY					= {"HP="};
const T_DISPLAY  LINE_DISPLAY				= {"----------------------------------------------------------------------------------------------------"};


const T_DISPLAY COLOR_CHAR_ARRAY[COLOR_NUM] 	= {RED_CHAR,BLUE_CHAR,GREEN_CHAR,YELLOW_CHAR,PURPLE_CHAR};
enum activity{APPEARANCE,ADD_ATTACK,TAKE_ATTACK,KILLED_ENEMY,RECOVERY,STATUS,COMBO,PLAYER_TURN,ENEMY_TURN};
/*********************************************************************/
/*ワークエリア定義													 */
/*********************************************************************/
AUTOPLAYER resume_data[SPEAKER_NUM];
T_MONSTER *penemy;				//戦闘中のモンスターへのポインタ
T_PLAYER *pplayer;				//プレイヤーへのポインタ
T_MONSTER *pally;				//味方モンスター達へのポインタ
T_MONSTER attack_ally;			//攻撃したモンスター
char output_display[9][512];	//戦闘中の画面表示
unsigned char first_turn_flg;	//敵が現れて最初のターンなら新しいバトルフィールドを作成する
unsigned char operation[2];		//プレーヤーの入力したアルファベットが入る
unsigned short combo_value;		//コンボ数
/********************************************************************/
/*プロトタイプ宣言													*/
/********************************************************************/
void player_turn(void);
void enemy_turn(void);
void i_to_a(unsigned short i,unsigned char *output_string);
static void battle_display(unsigned char activity,unsigned char *param);
unsigned char puzzle_operation_check(void);
void motion_after_input(void);

/********************************************************************/
/*戦闘に勝利															*/
/*void win(void)													*/
/********************************************************************/
void win(void)
{
	battle_display(KILLED_ENEMY,NULL);
	auto_play_end_processing();
	autoplay_start_from_beginning(WINNING,SQUARE,0,0,0);
	while(playing_flg == ON){
		/*nop*/
	}
}
/********************************************************************/
/*バトルメイン関数													*/
/*unsigned char battle_main(struct T_MONSTER* enemy)					*/
/*  引数：struct T_MONSTER* enemy 対戦している敵情報					*/
/*	戻り値：unsigned char ret 1勝利									*/
/*							  0敗北									*/
/********************************************************************/
unsigned char battle(T_MONSTER* enemy, T_MONSTER* ally,T_PLAYER* player)
{
	unsigned char kill_cnt = 0;
	first_turn_flg 			= ON;
	penemy					= enemy;
	pplayer					= player;
	pally					= ally;
	battle_display(APPEARANCE,NULL);
	while(kill_cnt < ENEMY_NUM){
		player_turn();
		if(penemy->hp == 0){
			win();
			kill_cnt++;
		}
		enemy_turn();
		if(pplayer->hp <= 0)
			return 0;
		first_turn_flg 			= OFF;
	}
	return 1;
}


/********************************************************************/
/*プレーヤーのターン関数											*/
/*void player_turn(T_PLAYER *player,T_MONSTER* enemy)							*/
/*	引数：struct T_MONSTER* enemy 戦闘中の敵のデータ					*/
/********************************************************************/
void player_turn(void)
{
	unsigned char ret;
	cmt2_wait（,CKS512）；//表示を変える前の時間待ち
	battle_display(PLAYER_TURN,&ret);
	battle_display(STATUS,NULL);
	if(first_turn_flg == ON){
		autoplay_start_from_beginning(BATTLE1,SQUARE,0,0,0);//戦闘テーマ演奏開始
		output_battle_field(NEW_FIELD);
	}else{
		output_battle_field(CURRENT_FIELD);
	}
	send_serial(REQUEST_COMMAND_DISPLAY,strlen(REQUEST_COMMAND_DISPLAY));
	while(1){
		ret									= puzzle_operation_check();
		if(ret == ON){
			motion_after_input();
			break;
		}
		if(playing_flg == OFF)//戦闘の曲が終了したとき
			autoplay_start_from_beginning(BATTLE1,SQUARE,0,0,0);
	}
	send_serial(CURSOR_2LINE_ADVANCE,4);
}

/**********************************************************
/*入力値の判定 */
/*unsigned char decision_c(unsigned char c)*/
/*
 *
 */
/**/
unsigned char decision_c(unsigned char c)
{
	if(c >= 'A' && c  <= 'M')
		return c;
	else if(c >= 'a' && c <= 'm')
		return c;
	return 0;
}
/**********************************************************
 *　パズル操作の入力を判定する
 * unsigned char puzzle_operation(void)
 * 		戻り値：unsigned char ON：正しい入力があった
 * 						   OFF:入力なし又は正しくない入力
 */
//saveコマンドでセーブできるように
unsigned char puzzle_operation_check(void)
{
	unsigned char ret;
	unsigned char input[2] = {0};
	ret = input_check();
	if(ret == ON){
		ret						= sci0_get_receive_count();
		if(ret == 4){//2文字入力された
			sci0_data_cpy(&input[0]);
			if(decision_c(input[0])){
				operation[0] = input[0];;
				if(decision_c(input[1])){
					operation[1]	= input[1];
					return ON;
				}
			}
		}
		sci0_receive_start();
	}
	return OFF;
}
/******************************************************************************
 *
 *void motion_after_input(unsigned char *input1,unsigned char *input2)
 */
void motion_after_input(void)
{
	unsigned char i;
	unsigned char *dladder = NULL;
	move_jewel(operation[0],operation[1]);
	while(1){
		dladder							= count_jewel();//3つ以上宝石が一致していたら配列のアドレスを返す。一致してなかったらNULLを返す
		if(dladder != NULL){
			if(playing_flg == ON){
				auto_play_end_processing();
				for(i = 0;i < 3;i++){
					resume_data[i]		= get_interrupt_data(i);
				}
			}else
			autoplay_start_from_beginning(ALLY_ATACK,SQUARE);//攻撃音演奏
			while(playing_flg == ON){
				//nop
			}
			if(*dladder != LIFE){
				attack_ally 			= get_ally_data(*dladder);
				battle_display(ADD_ATTACK,dladder);
			}else
				battle_display(RECOVERY,dladder);
			autoplay_start_from_intermediate(resume_data[0],resume_data[1],resume_data[2]);//演奏再開
			free_padding(dladder);//空いた宝石配列を詰める
		}else{//自分のターン終了
			sci0_receive_start();//受信が終わっているので開始
			break;
		}
	}
}
/********************************************************************
/*敵のターン関数
/*void enemy_turn(void)
/*	引数：struct Enemy* enemy　攻撃する敵の情報
/*	戻り値：unsigned char
********************************************************************/
void enemy_turn(void)
{
	//敵の攻撃音も追加
	battle_display(ENEMY_TURN,NULL);
	battle_display(TAKE_ATTACK,NULL);
}
/**************************************************************************
/*コンボ数を数える。コンボならコンボ数表示											　*/
/*
/**************************************************************************/
void combo(void)
{
	combo_value++;
	if(combo_value >= 2){//コンボカウントが２以上ならコンボ数表示
		sprintf(output_display[COMBO],"%d%s",combo_value,COMBO_DISPLAY);
		send_serial((const unsigned char *)output_display[COMBO],strlen(output_display[COMBO]));
	}
}

void combo_reset(void)
{
	combo_value = 0;
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
	switch(activity){
	case APPEARANCE:
		sprintf(output_display[APPEARANCE],"%s%s%s%s",COLOR_CHAR_ARRAY[penemy->el],penemy->name,DEFAULT_CHAR,APPEAR_DISPLAY);
		break;
	case ADD_ATTACK:
		combo();
		damage_value			= damage_or_recovery_value_calculate(penemy, combo_value, attack_ally.el, delete_jewel(param));//ダメージ計算
		sprintf(output_display[ADD_ATTACK],"%s%s%s%s%s%s%s%s%d%s%s",COLOR_CHAR_ARRAY[attack_ally.el],attack_ally.name,DEFAULT_CHAR,ATTACK_DISPLAY,COLOR_CHAR_ARRAY[penemy->el],penemy->name,DEFAULT_CHAR,TO_DISPLAY,damage_value,DAMAGE_DISPLAY,CRLF);
		if(penemy->hp >= damage_value)
			penemy->hp 			= penemy->hp - damage_value;//モンスターのHPからダメージを引く
		else
			penemy->hp 			= 0;
		break;
	case TAKE_ATTACK:
		damage_value			= damge_from_enemy_calculation(pplayer->gp,penemy);
		pplayer->hp				= pplayer->hp - damage_value;
		sprintf(output_display[TAKE_ATTACK],"%s%s%s%s%d%s%s",COLOR_CHAR_ARRAY[penemy->el],penemy->name,DEFAULT_CHAR,ATTACK_DISPLAY,damage_value,DAMAGE_DISPLAY,TAKE_DISPLAY);
		break;
	case KILLED_ENEMY:
		sprintf(output_display[KILLED_ENEMY],"%s%s%s%s%s",COLOR_CHAR_ARRAY[penemy->el],penemy->name,DEFAULT_CHAR,KILL_DISPLAY,CRLF);
		break;
	case RECOVERY:
		/*回復表示の時*/
		combo();
		damage_value			= damage_or_recovery_value_calculate(penemy, combo_value, LIFE, delete_jewel(param));//ダメージ計算
		sprintf(output_display[RECOVERY],"%s%s%d%s",pplayer->name,LIFE_JEWEL_DISPLAY,damage_value,RECOVERY_DISPLAY);
		pplayer->hp = pplayer->hp + damage_value;//自分のHPにダメージを足す最大値を超えないように
		if(pplayer->hp > pplayer->max_hp)
			pplayer->hp = pplayer->max_hp;
		break;
	case STATUS:
		//味方のステータスも表示する
		sprintf(output_display[STATUS],"%s%s%s%s%s%s%s%s%s%s%d/%d%s%s%s%s%s %s%s %s%s %s%s%s%s%s%s%d/%d%s%s%s%s%s",
																LINE_DISPLAY,CRLF,CRLF,
																CURSOL_MOVING_SENTER,COLOR_CHAR_ARRAY[penemy->el],penemy->name,DEFAULT_CHAR,CRLF,
																CURSOL_MOVING_SENTER,HP_DISPLAY,penemy->hp,penemy->max_hp,CRLF,CRLF,CRLF,
																COLOR_CHAR_ARRAY[pally[FIRE].el],pally[FIRE].name,
																COLOR_CHAR_ARRAY[pally[WATER].el],pally[WATER].name,
																COLOR_CHAR_ARRAY[pally[WIND].el],pally[WIND].name,
																COLOR_CHAR_ARRAY[pally[SOIL].el],pally[SOIL].name,DEFAULT_CHAR,CRLF,
																CURSOL_MOVING_SENTER,HP_DISPLAY,pplayer->hp,pplayer->max_hp,CRLF,
																CRLF,CRLF,LINE_DISPLAY,CRLF);
		combo_reset();
		break;
	case ENEMY_TURN:
	case PLAYER_TURN:
		//ターン表示
		if(param != NULL)//プレーヤーのターンならプレイヤー名
			sprintf(output_display[PLAYER_TURN],"%s【%sのターン】\r\n",DISPLAY_CLEAR,pplayer->name);
		else
			sprintf(output_display[ENEMY_TURN],"【%s%s%sのターン】\r\n",COLOR_CHAR_ARRAY[penemy->el],penemy->name,DEFAULT_CHAR);
		break;
	}
	send_serial((const unsigned char *)output_display[activity],strlen(output_display[activity]));
}
