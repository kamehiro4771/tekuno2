

/********************************************************************/
/*ヘッダーファイル													*/
/********************************************************************/
#include "main.h"
#if 1
/*********************************************************************/
/*定数定義															 */
/*********************************************************************/
T_DISPLAY ATTACK[]				= {"の攻撃！\n"};
T_DISPLAY COMBO[]				= {"コンボ！\n"};
T_DISPLAY ADD_DAMAGE[]			= {"のダメージ！\n"};
T_DISPLAY TAKE_DAMAGE[]			= {"をうけた！\n"};
T_DISPLAY RECOVERY[]			= {"回復！\n"};
T_DISPLAY APPEAR[]				= {"が現れた！\n"};
T_DISPLAY KILL[]				= ("を倒した！\n");
T_DISPLAY REQUEST_COMMAND[]		= {"コマンド?>>"};
T_DISPLAY OPERATION_METHOD[]	= {"一文字目動かす宝石の現在地、2文字目動かし先"};
T_DISPLAY INPUT_ERROR[]			= {"入力が正しくありません"};
T_DISPLAY HP[]					= {"HP="};
T_DISPLAY COLOR_CHAR_ARRAY[COLOR_NUM][6] = {RED_CHAR,BLUE_CHAR,GREEN_CHAR,YELLOW_CHAR,PURPLE_CHAR};
/*********************************************************************/
/*ワークエリア定義													 */
/*********************************************************************/
AUTOPLAYER *resume_data;
unsigned char output_string[128];
unsigned char first_turn_flg;
/********************************************************************/
/*プロトタイプ宣言													*/
/********************************************************************/
void player_turn(Player *player,Enemy* enemy);
void enemy_turn(Player *player,struct Enemy* enemy);
void i_to_a(unsigned short i);
void display_about_monster(Enemy *enemy,unsigned char activity,unsigned short damage);
/********************************************************************/
/*バトルメイン関数													*/
/*unsigned char battle_main(struct Enemy* enemy)					*/
/*  引数：struct Enemy* enemy 対戦している敵情報					*/
/*	戻り値：unsigned char ret 1勝利									*/
/*							  0敗北									*/
/********************************************************************/
unsigned char battle_main(Player *player, Enemy *enemy)
{
	first_turn_flg 					= ON;
	display_about_monster(enemy,APPEARANCE,0);
	while(1){
		player_turn(player,enemy);
		if(enemy->hp == 0)
			return 1;
		enemy_turn(player,enemy);
		if(player->hp <= 0)
			return 0;
		first_turn_flg 			= OFF;
	}
}


/********************************************************************/
/*プレーヤーのターン関数											*/
/*void player_turn(Player *player,Enemy* enemy)							*/
/*	引数：struct Enemy* enemy 戦闘中の敵のデータ					*/
/********************************************************************/
void player_turn(Player *player,Enemy* enemy)
{
	unsigned char input[2] = {0};
	unsigned char *dladder;
	AUTOPLAYER *pautoplayer = get_autoplayer();
	unsigned char ret,deleted_type,deleted_number;
	unsigned short combo_count = 0;
	unsigned short damage;
	display_about_monster(enemy,STATUS,enemy->hp);
	if(first_turn_flg == ON){
		automatic_playing(BATTLE1,SQUARE,0,0,0);
		output_battle_field(NEW_FIELD);
	}else{
		pautoplayer[0].elapsed_time = resume_data[0].elapsed_time;
		pautoplayer[1].elapsed_time = resume_data[1].elapsed_time;
		pautoplayer[2].elapsed_time = resume_data[2].elapsed_time;
		automatic_playing(BATTLE1,SQUARE,resume_data[0].score_count,resume_data[1].score_count,resume_data[2].score_count);
		output_battle_field(CURRENT_FIELD);
	}
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
	攻撃した後曲がおかしくなる
//	send_serial(CURSOR_2LINE_BUCK,5);//エンターが押されてカーソルが下がるので1行戻す
	move_jewel(input[0],input[1]);//宝石を動かす
	while(1){
		dladder							= count_jewel();//3つ以上宝石が一致していたら配列のアドレスを返す。一致してなかったらNULLを返す
		deleted_type					= *dladder;
		if(dladder != NULL){
			combo_count++;//
			deleted_number				= delete_jewel(dladder);//宝石を消す消した宝石数をもらう
			send_serial(CURSOR_2LINE_ADVANCE,4);
			i_to_a(combo_count);//
			send_serial(output_string,strlen((const char*)output_string));//「〇コンボ！」表示
			send_serial(COMBO,sizeof(COMBO));
			damage						= damage_calculation(enemy,combo_count,deleted_type,deleted_number);//ダメージ計算
			if(deleted_type != LIFE){
				auto_play_end_processing();
				resume_data			= get_interrupt_data;
				automatic_playing(ALLY_ATACK,SQUARE,0,0,0);//攻撃音演奏
				while(playing_flg == ON){
					//nop
				}
				i_to_a(damage);


				if(enemy->hp >= damage)
					enemy->hp 			= enemy->hp - damage;//モンスターのHPからダメージを引く
				else
					enemy->hp 			= 0;
			}else{
				auto_play_end_processing();
				resume_data			= get_interrupt_data();
				automatic_playing(ALLY_ATACK,SQUARE,0,0,0);
				while(playing_flg == ON){
					//nop
				}
				i_to_a(damage);
				send_serial(output_string,strlen((const char*)output_string));
				send_serial(RECOVERY,sizeof(RECOVERY));//「回復！」
				player->hp = player->hp + damage;//自分のHPにダメージを足す最大値を超えないように
				if(player->hp > player->mhp)
					player->hp = player->mhp;
			}
			pautoplayer[0].elapsed_time = resume_data[0].elapsed_time;
			pautoplayer[1].elapsed_time = resume_data[1].elapsed_time;
			pautoplayer[2].elapsed_time = resume_data[2].elapsed_time;
			automatic_playing(BATTLE1,SQUARE,resume_data[0].score_count,resume_data[1].score_count,resume_data[2].score_count);
			send_serial(CURSOR_5LINE_BUCK,sizeof(CURSOR_5LINE_BUCK));
			free_padding(dladder);//空いた宝石配列を詰める
		}else{
			auto_play_end_processing();
			resume_data			= get_interrupt_data();
			break;
		}
	}
	send_serial(CURSOR_2LINE_ADVANCE,4);
}

/********************************************************************/
/*敵のターン関数													*/
/*void enemy_turn(Player *player,struct Enemy* enemy)					*/
/*	引数：struct Enemy* enemy　攻撃する敵の情報
/*	戻り値：unsigned char
/********************************************************************/
void enemy_turn(Player *player,Enemy* enemy)
{
	unsigned short ret;
	ret	= damge_from_enemy_calculation(player->gp,enemy);
	display_about_monster(enemy,TAKE_ATTACK,ret);
	player->hp = player->hp - ret;
}

/********************************************************************/
/*数値を文字列に変換する											*/
/*void i_to_a(unsigned short i)										*/
/*	引数：unsigned short i 数値										*/
/********************************************************************/
//入力は65535まで
//出力はワークエリア定義のoutput_stringに変換される
//最後にNULL文字を入れる
void i_to_a(unsigned short i)
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

//モンスターが現れた時、モンスターを攻撃したとき、モンスターが攻撃したとき、モンスターを倒した時、パラメータの表示
void display_about_monster(Enemy *enemy,unsigned char activity,unsigned short param)
{
	send_serial(COLOR_CHAR_ARRAY[enemy->el],sizeof(COLOR_CHAR_ARRAY[enemy->el]));
	send_serial(enemy->name,strlen((const char*)enemy->name));//モンスター名表示
	send_serial(DEFAULT_CHAR,sizeof(DEFAULT_CHAR));
	switch(activity){
	case APPEARANCE:
		send_serial(APPEAR,sizeof(APPEAR));
		break;
	case ADD_ATTACK:
		i_to_a(param);
		send_serial(output_string,strlen((const char*)output_string));//ダメージ値表示
		send_serial(ADD_DAMAGE,sizeof(ADD_DAMAGE));//のダメージ
		break;
	case TAKE_ATTACK:
		send_serial(ATTACK,sizeof(ATTACK));
		i_to_a(param);
		send_serial(output_string,strlen((const char*)output_string));//ダメージ値表示
		break;
	case KILLED_ENEMY:

		break;
	case STATUS:
		send_serial("\n",1);
		i_to_a(param);
		send_serial(HP,sizeof(HP));
		send_serial(output_string,strlen((const char*)output_string));//HP表示
		send_serial("\n",1);
		break;
	}
}
//敵と味方のステータスを表示する
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
