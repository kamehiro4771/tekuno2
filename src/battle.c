

/********************************************************************/
/*ヘッダーファイル													*/
/********************************************************************/
#include "main.h"
#if 1
/*********************************************************************/
/*定数定義															 */
/*********************************************************************/
//「のダメージは」プレイヤーモンスター共通「の攻撃」は今のところモンスターだけだが変更するかもしれないので名前に工夫
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
const T_DISPLAY SLASH_DISPLAY[]				= {" / "};
//表示する文字列へのポインタ二次元配列の行は行動、列は表示する順番
/*const T_DISPLAY *DISPLAY_POINTER_ARRAY[][]		= {{APPEAR,,},//モンスター登場
													{,ATTACK,,TO,DAMAGE,},//モンスターに攻撃を与える
													{ATTACK,DAMAGE,TAKE},//モンスターから攻撃をもらう
													{KILL},//モンスターを倒した
													{HP,},//ステータス表示
													{},
};*/
const T_DISPLAY COLOR_CHAR_ARRAY[COLOR_NUM][6] 	= {RED_CHAR,BLUE_CHAR,GREEN_CHAR,YELLOW_CHAR,PURPLE_CHAR};
/*********************************************************************/
/*ワークエリア定義													 */
/*********************************************************************/
AUTOPLAYER *resume_data;
T_ENEMY *penemy;//戦闘中のモンスターへのポインタ
T_PLAYER *pplayer;//プレイヤーへのポインタ
T_ALLY *pally;
T_DISPLAY damage_num[6];//文字列に変換されたダメージが入る
T_DISPLAY combo_num[6];//文字列に変換されたコンボ数が入る
T_DISPLAY hp_num[6];
T_DISPLAY max_hp_num[6];
//unsigned char output_string[128];//ダメージ計算して文字列に変換したやつが入る
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
	}else
		output_battle_field(CURRENT_FIELD);
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
	ml入力でバグる
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
				pally = get_ally_data(*dladder);
				battle_display(ADD_ATTACK,dladder);
			}else
				battle_display(RECOVERY,dladder);
			pautoplayer[0] = resume_data[0];
			pautoplayer[1] = resume_data[1];
			pautoplayer[2] = resume_data[2];
			automatic_playing(BATTLE1,SQUARE,pautoplayer[0].score_count,pautoplayer[1].score_count,pautoplayer[2].score_count);
//			send_serial(CURSOR_5LINE_BUCK,sizeof(CURSOR_5LINE_BUCK));
			free_padding(dladder);//空いた宝石配列を詰める
		}else
			break;
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
		/*パズル画面操作のある時*/
		combo_value++;//
		deleted_number				= delete_jewel(param);//宝石を消して、消した宝石数をもらう
		if(combo_value >= 2){//コンボカウントが２以上ならコンボ数表示
			i_to_a(combo_value,combo_num);//
			send_serial(combo_num,strlen((const char*)combo_num));
			send_serial(COMBO_DISPLAY,sizeof(COMBO_DISPLAY));//「〇コンボ！」表示
		}
		damage_value						= damage_calculation(penemy,combo_value,*param,deleted_number);//ダメージ計算
		i_to_a(damage_value,damage_num);//文字列に変換
	}
	if(activity == RECOVERY){
		/*回復表示の時*/
		send_serial((const T_DISPLAY*)pplayer->name,strlen((const char*)pplayer->name));
		send_serial(LIFE_JEWEL_DISPLAY,sizeof(LIFE_JEWEL_DISPLAY));
		send_serial(damage_num,strlen((const char*)damage_num));//回復値表示
		send_serial(RECOVERY_DISPLAY,sizeof(RECOVERY_DISPLAY));
		pplayer->hp = pplayer->hp + damage_value;//自分のHPにダメージを足す最大値を超えないように
		if(pplayer->hp > pplayer->mhp)
			pplayer->hp = pplayer->mhp;
	}else{
		/*回復以外の表示の時*/
		if(activity == ADD_ATTACK){//敵を攻撃する時
			send_serial(COLOR_CHAR_ARRAY[pally->el],sizeof(COLOR_CHAR_ARRAY[pally->el]));
			send_serial(pally->name,strlen((const char*)pally->name));
			send_serial(DEFAULT_CHAR,sizeof(DEFAULT_CHAR));
			send_serial(ATTACK_DISPLAY,sizeof(ATTACK_DISPLAY));
			if(penemy->hp >= damage_value)
				penemy->hp 			= penemy->hp - damage_value;//モンスターのHPからダメージを引く
			else
				penemy->hp 			= 0;
		}else if(activity == STATUS){
			send_serial(CURSOL_MOVING_SENTER,sizeof(CURSOL_MOVING_SENTER));//ステータス表示の時は真ん中から表示
		}
		send_serial(COLOR_CHAR_ARRAY[penemy->el],sizeof(COLOR_CHAR_ARRAY[penemy->el]));
		send_serial(penemy->name,strlen((const char*)penemy->name));//モンスター名表示
		send_serial(DEFAULT_CHAR,sizeof(DEFAULT_CHAR));
		switch(activity){
		case APPEARANCE:
			send_serial(APPEAR_DISPLAY,sizeof(APPEAR_DISPLAY));
			break;
		case ADD_ATTACK:
			send_serial(TO_DISPLAY,sizeof(TO_DISPLAY));
			send_serial(damage_num,strlen((const char*)damage_num));//ダメージ値表示
			send_serial(DAMAGE_DISPLAY,sizeof(DAMAGE_DISPLAY));//のダメージ
			send_serial(CRLF,2);
			break;
		case TAKE_ATTACK:
			send_serial(ATTACK_DISPLAY,sizeof(ATTACK_DISPLAY));
			send_serial(damage_num,strlen((const char*)damage_num));//ダメージ値表示
			send_serial(DAMAGE_DISPLAY,sizeof(DAMAGE_DISPLAY));//のダメージ
			send_serial(TAKE_DISPLAY,sizeof(TAKE_DISPLAY));
			break;
		case KILLED_ENEMY:
			send_serial(KILL_DISPLAY,sizeof(KILL_DISPLAY));
			break;
		case STATUS:
			send_serial(CRLF,2);
			send_serial(CURSOL_MOVING_SENTER,sizeof(CURSOL_MOVING_SENTER));//ステータス表示の時は真ん中から表示
			send_serial(HP_DISPLAY,sizeof(HP_DISPLAY));
			i_to_a(penemy->hp,hp_num);
			send_serial(hp_num,strlen((const char*)hp_num));
			send_serial(SLASH_DISPLAY,sizeof(SLASH_DISPLAY));
			i_to_a(penemy->mhp,max_hp_num);
			send_serial(max_hp_num,strlen((const char*)max_hp_num));
			send_serial(CRLF,2);
			break;
		}
	}
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

#endif
