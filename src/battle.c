

/********************************************************************/
/*�w�b�_�[�t�@�C��													*/
/********************************************************************/
#include "main.h"
/*********************************************************************/
/*�萔��`															 */
/*********************************************************************/
const T_DISPLAY APPEAR_DISPLAY				= {"�����ꂽ�I\n"};
const T_DISPLAY ATTACK_DISPLAY				= {"�̍U���I\n"};
const T_DISPLAY COMBO_DISPLAY				= {"�R���{�I\n"};
const T_DISPLAY TO_DISPLAY					= {"��"};
const T_DISPLAY DAMAGE_DISPLAY				= {"�̃_���[�W"};
const T_DISPLAY TAKE_DISPLAY				= {"���������I\n"};
const T_DISPLAY LIFE_JEWEL_DISPLAY			= {"�͖��̕�΂��g����"};
const T_DISPLAY RECOVERY_DISPLAY			= {"�񕜁I\n"};
const T_DISPLAY KILL_DISPLAY				= ("��|�����I\n");
const T_DISPLAY REQUEST_COMMAND_DISPLAY		= {"�R�}���h?>>"};
const T_DISPLAY OPERATION_METHOD_DISPLAY	= {"�ꕶ���ړ�������΂̌��ݒn�A2�����ړ�������"};
const T_DISPLAY INPUT_ERROR_DISPLAY			= {"���͂�����������܂���"};
const T_DISPLAY HP_DISPLAY					= {"HP="};
const T_DISPLAY  LINE_DISPLAY				= {"----------------------------------------------------------------------------------------------------"};


const T_DISPLAY COLOR_CHAR_ARRAY[COLOR_NUM] 	= {RED_CHAR,BLUE_CHAR,GREEN_CHAR,YELLOW_CHAR,PURPLE_CHAR};
enum activity{APPEARANCE,ADD_ATTACK,KILLED_ENEMY,RECOVERY,COMBO,PLAYER_TURN,ENEMY_TURN};
/*********************************************************************/
/*���[�N�G���A��`													 */
/*********************************************************************/
AUTOPLAYER resume_data[SPEAKER_NUM];
T_MONSTER *penemy;				//�퓬���̃����X�^�[�ւ̃|�C���^
T_PLAYER *pplayer;				//�v���C���[�ւ̃|�C���^
T_MONSTER *pally;				//���������X�^�[�B�ւ̃|�C���^
char output_display[9][512];	//�퓬���̉�ʕ\��
unsigned char first_turn_flg;	//�G������čŏ��̃^�[���Ȃ�V�����o�g���t�B�[���h���쐬����
unsigned char operation[2];		//�v���[���[�̓��͂����A���t�@�x�b�g������
unsigned short combo_value;		//�R���{��
/********************************************************************/
/*�v���g�^�C�v�錾													*/
/********************************************************************/
void player_turn(void);
void enemy_turn(void);
void i_to_a(unsigned short i,unsigned char *output_string);
static void battle_display(unsigned char activity,unsigned char *param);
unsigned char puzzle_operation_check(void);
void motion_after_input(void);

/********************************************************************/
/*�Ȃ̉��t���ĊJ����ʒu��ۑ�����									*/
/*voidresume_data_save(void)										*/
/********************************************************************/
void resume_data_save(void)
{
	unsigned char i;
	for (i = 0; i < 3; i++) {
		resume_data[i] = get_interrupt_data(i);
	}
}

/********************************************************************/
/*�퓬�ɏ���															*/
/*void win(void)													*/
/********************************************************************/
void win(void)
{
	battle_display(KILLED_ENEMY,NULL);
	auto_play_end_processing();
	autoplay_start_from_beginning(WINNING,SQUARE);
	while(playing_flg == ON){
		/*nop*/
	}
}
/**/
/*�o�g���Ɏg���O���[�o���ϐ�������*/
/*void battle_init(T_MONSTER* enemy, T_MONSTER* ally,T_PLAYER* player)*/
/**/
/* */
/**/
/********************************************************************/
void battle_init(T_MONSTER* enemy, T_MONSTER* ally,T_PLAYER* player)
{
	penemy					= enemy;
	pplayer					= player;
	pally					= ally;
	battle_display(APPEARANCE,NULL);
}
/********************************************************************/
/*�o�g�����C���֐�													*/
/*unsigned char battle_main(struct T_MONSTER* enemy)					*/
/*  �����Fstruct T_MONSTER* enemy �ΐ킵�Ă���G���					*/
/*	�߂�l�Funsigned char ret 1����									*/
/*							  0�s�k									*/
/********************************************************************/
unsigned char battle(T_MONSTER* enemy, T_MONSTER* ally,T_PLAYER* player)
{
	unsigned char kill_cnt = 0;
	battle_init(enemy,ally,player);
	while(kill_cnt < ENEMY_NUM){
		player_turn();
		if(penemy->hp.now_hp == 0){
			win();
			kill_cnt++;
			penemy = &enemy[kill_cnt];
			battle_display(APPEARANCE,NULL);//���̃����X�^�[�������
			first_turn_flg 			= ON;
		}else{
			enemy_turn();
			first_turn_flg 			= OFF;
			if(pplayer->hp.now_hp <= 0)
				return 0;
		}
	}
	return 1;
}


/********************************************************************/
/*�v���[���[�̃^�[���֐�											*/
/*void player_turn(T_PLAYER *player,T_MONSTER* enemy)							*/
/*	�����Fstruct T_MONSTER* enemy �퓬���̓G�̃f�[�^					*/
/********************************************************************/
void player_turn(void)
{
	unsigned char ret;
	second_wait(2);//�\����ς���O�̎��ԑ҂�
	battle_display(PLAYER_TURN,NULL);
	if(first_turn_flg == ON){
		autoplay_start_from_beginning(BATTLE1,SQUARE);//�퓬�e�[�}���t�J�n
		output_battle_field(NEW_FIELD);
	}else{
		output_battle_field(CURRENT_FIELD);
	}
	send_serial(REQUEST_COMMAND_DISPLAY,strlen(REQUEST_COMMAND_DISPLAY));
	sci0_receive_start();
	while(1){
		ret									= puzzle_operation_check();
		if(ret == ON){
			motion_after_input();
			break;
		}
		if(playing_flg == OFF)//�퓬�̋Ȃ��I�������Ƃ�
			autoplay_start_from_beginning(BATTLE1,SQUARE);
	}
	send_serial(CURSOR_2LINE_ADVANCE,4);
}

/******************************************************************************/
/*���͒l�̔���																  */
/*unsigned char decision_c(unsigned char c)									  */
/*	�����Funsigned char c�@���肷��l										  */
/*	�߂�l�Funsigned char ret ���͂��s���Ȃ�O�A��������������������̂܂ܕԂ�*/
/******************************************************************************/
unsigned char decision_c(unsigned char c)
{
	if(c >= 'A' && c  <= 'M')
		return c;
	else if(c >= 'a' && c <= 'm')
		return c;
	return 0;
}
/**********************************************************
 *�@�p�Y������̓��͂𔻒肷��
 * unsigned char puzzle_operation(void)
 * 		�߂�l�Funsigned char ON�F���������͂�������
 * 						   OFF:���͂Ȃ����͐������Ȃ�����
 */
//save�R�}���h�ŃZ�[�u�ł���悤��
unsigned char puzzle_operation_check(void)
{
	unsigned char ret;
	unsigned char input[2] = {0};
	ret = input_check();
	if(ret == ON){
		ret						= sci0_get_receive_count();
		if(ret == 4){//2�������͂��ꂽ
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
	unsigned char *dladder = NULL;
	move_jewel(operation[0],operation[1],BATTLE1);
	while(1){
		dladder							= count_jewel();//3�ȏ��΂���v���Ă�����z��̃A�h���X��Ԃ��B��v���ĂȂ�������NULL��Ԃ�
		if(dladder == NULL)
			break;//�����̃^�[���I��
		auto_play_end_processing();
		resume_data_save();
		autoplay_start_from_beginning(ALLY_ATACK,SQUARE);//�U�������t
		while(playing_flg == ON){
			//nop
		}
		autoplay_start_from_intermediate(resume_data[0], resume_data[1], resume_data[2]);//���t�ĊJ
		if(*dladder != LIFE)
			battle_display(ADD_ATTACK,dladder);
		else
			battle_display(RECOVERY,dladder);
		free_padding(dladder);//�󂢂���Δz����l�߂�
	}
}

/********************************************************************/
/*�G�̃^�[���֐�													*/
/*void enemy_turn(void)											    */
/********************************************************************/
void enemy_turn(void)
{
	//�G�̍U�������ǉ�
	battle_display(ENEMY_TURN,NULL);
}
/**************************************************************************
/*�R���{���𐔂���B�R���{�Ȃ�R���{���\��											�@*/
/*void combo(void)
/**************************************************************************/
void combo(void)
{
	combo_value++;
	if(combo_value >= 2){//�R���{�J�E���g���Q�ȏ�Ȃ�R���{���\��
		sprintf(output_display[COMBO],"%d%s",combo_value,COMBO_DISPLAY);
		send_serial((const unsigned char *)output_display[COMBO],strlen(output_display[COMBO]));
	}
}

void combo_reset(void)
{
	combo_value = 0;
}

char* hp_color_decision(T_HP hp)
{
	float ratio = hp.now_hp / hp.max_hp;
	if (ratio > 0.5)
		return DEFAULT_CHAR;
	else if(ratio > 0.25)//50%�ȉ�
		return YELLOW_CHAR;
	else//25%�ȉ�
		return RED_CHAR;
}
/**************************************************************************
 * �o�g�����̉�ʕ\��
 * static void battle_display(unsigned char activity,unsigned char *param)
 * 	�����Funsigned char activity�@APPEARANCE�@�G�����ꂽ
 * 							ADD_ATTACK�@�G�ɍU����������
 * 							KILLED_ENEMY�@�G��|�������̕\��
 * 							RECOVERY�@�񕜕\��
 * 		unsigned char *param�@����������΂̐擪�A�h���X�A����NULL
 */

static void battle_display(unsigned char activity,unsigned char *param)
{
	unsigned short damage_value = 0;
	T_MONSTER attack_ally;			//�U�����������X�^�[
	switch (activity) {
	case APPEARANCE:
		sprintf(output_display[APPEARANCE], "%s%s%s%s", COLOR_CHAR_ARRAY[penemy->el], penemy->name, DEFAULT_CHAR, APPEAR_DISPLAY);
		break;
	case ADD_ATTACK:
		combo();
		attack_ally = get_ally_data(*param);
		damage_value = damage_from_ally_calculation(*penemy, attack_ally, combo_value, delete_jewel(param));//�_���[�W�v�Z
		sprintf(output_display[ADD_ATTACK], "%s%s%s%s%s%s%s%s%d%s%s", COLOR_CHAR_ARRAY[attack_ally.el], attack_ally.name, DEFAULT_CHAR, ATTACK_DISPLAY, COLOR_CHAR_ARRAY[penemy->el], penemy->name, DEFAULT_CHAR, TO_DISPLAY, damage_value, DAMAGE_DISPLAY, CRLF);
		if (penemy->hp.now_hp >= damage_value)
			penemy->hp.now_hp = penemy->hp.now_hp - damage_value;//�����X�^�[��HP����_���[�W������
		else
			penemy->hp.now_hp = 0;
		break;
	case KILLED_ENEMY:
		sprintf(output_display[KILLED_ENEMY], "%s%s%s%s%s", COLOR_CHAR_ARRAY[penemy->el], penemy->name, DEFAULT_CHAR, KILL_DISPLAY, CRLF);
		break;
	case RECOVERY:
		combo();
		damage_value = recovery_value_calculate(combo_value, delete_jewel(param));//�_���[�W�v�Z
		sprintf(output_display[RECOVERY], "%s%s%d%s", pplayer->name, LIFE_JEWEL_DISPLAY, damage_value, RECOVERY_DISPLAY);
		pplayer->hp.now_hp = pplayer->hp.now_hp + damage_value;//������HP�Ƀ_���[�W�𑫂��ő�l�𒴂��Ȃ��悤��
		if (pplayer->hp.now_hp > pplayer->hp.max_hp)
			pplayer->hp.now_hp = pplayer->hp.max_hp;
		break;
	case PLAYER_TURN:
		sprintf(output_display[PLAYER_TURN], "%s�y%s�̃^�[���z\r\n%s%s%s%s%s%s%s%s%s%s%s%d%s/%d%s%s%s%s%s %s%s %s%s %s%s%s%s%s%s%s%d%s/%d%s%s%s%s%s",
			RESET, pplayer->name,
			LINE_DISPLAY, CRLF, CRLF,
			CURSOL_MOVING_SENTER, COLOR_CHAR_ARRAY[penemy->el], penemy->name, DEFAULT_CHAR, CRLF,
			CURSOL_MOVING_SENTER, HP_DISPLAY, hp_color_decision(penemy->hp),penemy->hp.now_hp,DEFAULT_CHAR, penemy->hp.max_hp, CRLF, CRLF, CRLF,
			COLOR_CHAR_ARRAY[pally[FIRE].el], pally[FIRE].name,
			COLOR_CHAR_ARRAY[pally[WATER].el], pally[WATER].name,
			COLOR_CHAR_ARRAY[pally[WIND].el], pally[WIND].name,
			COLOR_CHAR_ARRAY[pally[SOIL].el], pally[SOIL].name, DEFAULT_CHAR, CRLF,
			CURSOL_MOVING_SENTER, HP_DISPLAY, hp_color_decision(pplayer->hp),pplayer->hp.now_hp, DEFAULT_CHAR,pplayer->hp.max_hp, CRLF,
			CRLF, CRLF, LINE_DISPLAY, CRLF);
		combo_reset();
		break;
	case ENEMY_TURN:
		damage_value = damge_from_enemy_calculation(pplayer->gp, penemy->ap);
		if(pplayer->hp.now_hp >= damage_value)
			pplayer->hp.now_hp = pplayer->hp.now_hp - damage_value;
		else
			pplayer->hp.now_hp = 0;
		sprintf(output_display[ENEMY_TURN], "%s�y%s%s%s�̃^�[���z\r\n%s%s%s%s%d%s%s",
			RESET,
			COLOR_CHAR_ARRAY[penemy->el], penemy->name, DEFAULT_CHAR,
			COLOR_CHAR_ARRAY[penemy->el], penemy->name, DEFAULT_CHAR, ATTACK_DISPLAY, damage_value, DAMAGE_DISPLAY, TAKE_DISPLAY);
		break;
	}
	send_serial((const unsigned char *)output_display[activity],strlen(output_display[activity]));
}
