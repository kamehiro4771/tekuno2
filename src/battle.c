

/********************************************************************/
/*ヘッダーファイル													*/
/********************************************************************/
#include "main.h"
#if 1
/*********************************************************************/
/*定数定義															 */
/*********************************************************************/
const unsigned char ATTACK[]			= {"攻撃！\n"};
const unsigned char COMBO[]				= {"コンボ！\n"};
const unsigned char DAMAGE[]			= {"のダメージ！\n"};
const unsigned char RECOVERY[]			= {"回復！\n"};
const unsigned char APPEAR[]			= {"が現れた！\n"};
const unsigned char OPERATION_METHOD[]	= {"一文字目動かす宝石の現在地、2文字目動かし先"};
const unsigned char INPUT_ERROR[]		= {"入力が正しくありません"};

/*********************************************************************/
/*ワークエリア定義													 */
/*********************************************************************/
struct playing_resume_point resume_point[3] = {0};//戦闘のテーマを再開する位置
struct SPEAKER *battle_speaker;
unsigned char output_string[128];
unsigned char first_turn_flg;
/********************************************************************/
/*プロトタイプ宣言													*/
/********************************************************************/
void player_turn(struct Enemy* enemy);
void enemy_turn(struct Enemy* enemy);
void i_to_a(unsigned short i);
void learn_resume_point(void);
/********************************************************************/
/*バトルメイン関数													*/
/*unsigned char battle_main(struct Enemy* enemy)					*/
/*  引数：struct Enemy* enemy 対戦している敵情報					*/
/*	戻り値：unsigned char ret 1勝利									*/
/*							  0敗北									*/
/********************************************************************/
unsigned char battle_main(Player *player, Enemy *enemy)
{
	battle_speaker = get_speaker();
	first_turn_flg = ON;
	send_serial(enemy->name,sizeof(enemy->name));//敵の名前表示
	send_serial(APPEAR,sizeof(APPEAR));//「現れた！」表示
	while(1){
		player_turn(enemy);
		if(enemy->hp == 0)
			return 1;
		enemy_turn(enemy);
		if(player->hp <= 0)
			return 0;
		first_turn_flg = OFF;
	}
}


/********************************************************************/
/*プレーヤーのターン関数											*/
/*void player_turn(struct Enemy* enemy)								*/
/*	引数：struct Enemy* enemy 戦闘中の敵のデータ					*/
/********************************************************************/
void player_turn(struct Enemy* enemy)
{
	unsigned char input[2] = {0};
	unsigned char *dladder;
	unsigned char ret,deleted_type,deleted_number,i = 0;
	unsigned short combo_count = 0;
	signed short damage;
	send_serial(enemy->name,strlen((const char*)enemy->name));//敵と自分のパラメーター表示
	i_to_a(enemy->hp);
	send_serial(CRLF,2);
	send_serial(output_string,strlen((const char*)output_string));
	send_serial(CRLF,2);
	if(first_turn_flg == ON)
		output_battle_field(NEW_FIELD);
	else
		output_battle_field(CURRENT_FIELD);
	while(1){
		automatic_playing(BATTLE1,SQUARE,resume_point[0].start,resume_point[1].start,resume_point[2].start);
		while(playing_flg == ON){
			//nop
		}
		ret 								= sci0_enter_check();
		if(ret == ON){//エンターが押されたとき
			resume_point[0].start			= battle_speaker[0].score_count;
			resume_point[1].start			= battle_speaker[1].score_count;
			resume_point[2].start			= battle_speaker[2].score_count;
			battle_speaker[0].elapsed_time	= get_interrupt_data(0).elapsed_time;//経過時間も覚える
			battle_speaker[1].elapsed_time	= get_interrupt_data(1).elapsed_time;
			battle_speaker[2].elapsed_time	= get_interrupt_data(2).elapsed_time;
			ret								= sci0_get_receive_count();
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
			}
		}else//何も入力されずに曲終了
			resume_point[0].start	= resume_point[1].start = resume_point[2].start = 0;//曲の演奏開始位置を最初に設定
	}
	automatic_playing(BATTLE1,SQUARE,resume_point[0].start,resume_point[1].start,resume_point[2].start);
	move_jewel(input[0],input[1]);//宝石を動かす
	while(1){
		dladder							= count_jewel();//3つ以上宝石が一致していたら配列のアドレスを返す。一致してなかったらNULLを返す
		deleted_type					= *dladder;
		if(dladder != NULL){
			play_up_to_last				= ON;//宝石動かし中は、自動演奏エンターやスイッチで終了しない設定
			combo_count++;//
			deleted_number				= delete_jewel(dladder);//宝石を消す消した宝石数をもらう
			send_serial(CURSOR_2LINE_ADVANCE,4);
			i_to_a(combo_count);//
			send_serial(output_string,strlen((const char*)output_string));//「〇コンボ！」表示
			send_serial(COMBO,sizeof(COMBO));
			damage						= damage_calculation(enemy,combo_count,deleted_type,deleted_number);//ダメージ計算
			if(deleted_type != LIFE){//damageがマイナスなら敵へのダメージ、プラスなら回復表示
				auto_play_end_processing();
				resume_point[0].start		= battle_speaker[0].score_count;
				resume_point[1].start		= battle_speaker[1].score_count;
				resume_point[2].start		= battle_speaker[2].score_count;
				resume_point[0].elapsed		= get_interrupt_data(0).elapsed_time;//経過時間も覚える
				resume_point[1].elapsed		= get_interrupt_data(1).elapsed_time;
				resume_point[2].elapsed		= get_interrupt_data(2).elapsed_time;
				automatic_playing(ALLY_ATACK,SQUARE,0,0,0);//攻撃音演奏
				play_up_to_last				= OFF;
				send_serial(enemy->name,strlen((const char*)enemy->name));//モンスター名表示
				i_to_a(damage);
				send_serial(output_string,sizeof(output_string));//ダメージ値表示
				send_serial(DAMAGE,sizeof(DAMAGE));//のダメージ
				if(enemy->hp >= damage)
					enemy->hp 				= enemy->hp - damage;//モンスターのHPからダメージを引く
				else
					enemy->hp 				= 0;
			}else{
				auto_play_end_processing();
				resume_point[0].start		= battle_speaker[0].score_count;
				resume_point[1].start		= battle_speaker[1].score_count;
				resume_point[2].start		= battle_speaker[2].score_count;
				resume_point[0].elapsed		= get_interrupt_data(0).elapsed_time;//経過時間も覚える
				resume_point[1].elapsed		= get_interrupt_data(1).elapsed_time;
				resume_point[2].elapsed		= get_interrupt_data(2).elapsed_time;
				automatic_playing(ALLY_ATACK,SQUARE,0,0,0);
				i_to_a(damage);
				send_serial(output_string,strlen((const char*)output_string));
				send_serial(RECOVERY,sizeof(RECOVERY));//「回復！」
				//自分のHPにダメージを足す
			}
			free_padding(dladder);//空いた宝石配列を詰める
			play_up_to_last					= OFF;
		}else
			break;
	}
	battle_speaker[0].elapsed_time 			= resume_point[0].elapsed;
	battle_speaker[1].elapsed_time 			= resume_point[1].elapsed;
	battle_speaker[2].elapsed_time 			= resume_point[2].elapsed;
	send_serial(CURSOR_2LINE_ADVANCE,4);
}

/********************************************************************/
/*敵のターン関数													*/
/*unsigned char player_turn(struct Enemy* enemy)					*/
/*	引数：struct Enemy* enemy　攻撃する敵の情報
/*	戻り値：unsigned char
/********************************************************************/
void enemy_turn(struct Enemy* enemy)
{
	
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

/*
 * 演奏中断した位置を覚える
 * void learn_resume_point(RESUME_POINT resume_point)
 */
/*
void learn_resume_point(RESUME_POINT resume_point)
{
	resume_point.start[0]		= speaker[0].score_count;
	resume_point.start[1]		= speaker[1].score_count;
	resume_point.start[2]		= speaker[2].score_count;
	resume_point.elapsed[0]		= get_interrupt_data(0).elapsed_time;//経過時間も覚える
	resume_point.elapsed[1]		= get_interrupt_data(1).elapsed_time;
	resume_point.elapsed[2]		= get_interrupt_data(2).elapsed_time;
}*/
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
