

/********************************************************************/
/*�w�b�_�[�t�@�C��															*/
/********************************************************************/
#include "main.h"
/*********************************************************************/
/*�萔��`															 */
/*********************************************************************/
const T_DISPLAY APPEAR_DISPLAY[]			= {"�����ꂽ�I\n"};
const T_DISPLAY ATTACK_DISPLAY[]			= {"�̍U���I\n"};
const T_DISPLAY COMBO_DISPLAY[]				= {"�R���{�I\n"};
const T_DISPLAY TO_DISPLAY[]				= {"��"};
const T_DISPLAY DAMAGE_DISPLAY[]			= {"�̃_���[�W"};
const T_DISPLAY TAKE_DISPLAY[]				= {"���������I\n"};
const T_DISPLAY LIFE_JEWEL_DISPLAY[]		= {"�͖��̕�΂��g����"};
const T_DISPLAY RECOVERY_DISPLAY[]			= {"�񕜁I\n"};
const T_DISPLAY KILL_DISPLAY[]				= ("��|�����I\n");
const T_DISPLAY REQUEST_COMMAND_DISPLAY[]	= {"�R�}���h?>>"};
const T_DISPLAY OPERATION_METHOD_DISPLAY[]	= {"�ꕶ���ړ�������΂̌��ݒn�A2�����ړ�������"};
const T_DISPLAY INPUT_ERROR_DISPLAY[]		= {"���͂�����������܂���"};
const T_DISPLAY HP_DISPLAY[]				= {"HP="};
const T_DISPLAY  LINE_DISPLAY[]				= {"----------------------------------------------------------------------------------------------------"};


const T_DISPLAY COLOR_CHAR_ARRAY[COLOR_NUM][6] 	= {RED_CHAR,BLUE_CHAR,GREEN_CHAR,YELLOW_CHAR,PURPLE_CHAR};
enum activity{APPEARANCE,ADD_ATTACK,TAKE_ATTACK,KILLED_ENEMY,RECOVERY,STATUS,COMBO,PLAYER_TURN,ENEMY_TURN};
/*********************************************************************/
/*���[�N�G���A��`													 */
/*********************************************************************/
AUTOPLAYER *resume_data;
T_ENEMY *penemy;//�퓬���̃����X�^�[�ւ̃|�C���^
T_PLAYER *pplayer;//�v���C���[�ւ̃|�C���^
T_ALLY *pally;//���������X�^�[�B�ւ̃|�C���^
T_ALLY attack_ally;//�U�����������X�^�[
char output_display[9][512];//�퓬���̉�ʕ\��
unsigned char first_turn_flg;//�G������čŏ��̃^�[���Ȃ�V�����o�g���t�B�[���h���쐬����
unsigned char operation[2];//�v���[���[�̓��͂����A���t�@�x�b�g������
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
/*�o�g�����C���֐�													*/
/*unsigned char battle_main(struct T_ENEMY* enemy)					*/
/*  �����Fstruct T_ENEMY* enemy �ΐ킵�Ă���G���					*/
/*	�߂�l�Funsigned char ret 1����									*/
/*							  0�s�k									*/
/********************************************************************/
for (i = 0; i < ENEMY_NUM; i++) {
	ret = battle_main(&player, &ally, &enemy[i]);
	if (ret == LOSE) {
		g_sequence = 11;
		break;
	}
}
unsigned char battle_main(T_PLAYER *player,T_ALLY *ally, T_ENEMY *enemy)
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
			battle_display(KILLED_ENEMY,NULL);
			auto_play_end_processing();
			automatic_playing_start(WINNING,SQUARE,0,0,0);
			while(playing_flg == ON){
				/*nop*/
			}
			kill_cnt++;
		}
		enemy_turn();
		if(pplayer->hp <= 0)
			return 0;
		first_turn_flg 			= OFF;
	}
}


/********************************************************************/
/*�v���[���[�̃^�[���֐�											*/
/*void player_turn(T_PLAYER *player,T_ENEMY* enemy)							*/
/*	�����Fstruct T_ENEMY* enemy �퓬���̓G�̃f�[�^					*/
/********************************************************************/
void player_turn(void)
{
	unsigned char ret;
	battle_display(PLAYER_TURN,&ret);
	battle_display(STATUS,NULL);
	if(first_turn_flg == ON){
		automatic_playing_start(BATTLE1,SQUARE,0,0,0);//�퓬�e�[�}���t�J�n
		output_battle_field(NEW_FIELD);
	}else{
		output_battle_field(CURRENT_FIELD);
	}
	send_serial(REQUEST_COMMAND_DISPLAY,sizeof(REQUEST_COMMAND_DISPLAY));
	while(1){
		ret									= puzzle_operation_check();
		if(ret == ON){
			motion_after_input();
			break;
		}
		if(playing_flg == OFF)//�퓬�̋Ȃ��I�������Ƃ�
			automatic_playing_start(BATTLE1,SQUARE,0,0,0);
	}
	send_serial(CURSOR_2LINE_ADVANCE,4);
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
			if(input[0] >= 'A' && input[0] <= 'M'){//1�����ڔ���
				operation[0] = input[0];
				if(input[1] >= 'A' && input[1] <= 'M'){//2�����ڔ���
					operation[1]	= input[1];
					return ON;
				}
				if(input[1] >= 'a' && input[1] <= 'm'){
					operation[1]	= input[1];
					return ON;
				}
			}
			if(input[0] >= 'a' && input[0] <= 'm'){//
				operation[0] = input[0];
				if(input[1] >= 'A' && input[1] <= 'M'){//2�����ڔ���
					operation[1]	= input[1];
					return ON;
				}
				if(input[1] >= 'a' && input[1] <= 'm'){
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
	AUTOPLAYER *pautoplayer = get_autoplayer();
	move_jewel(operation[0],operation[1]);
	while(1){
		dladder							= count_jewel();//3�ȏ��΂���v���Ă�����z��̃A�h���X��Ԃ��B��v���ĂȂ�������NULL��Ԃ�
		if(dladder != NULL){
			if(playing_flg == ON){
				auto_play_end_processing();
				for(i = 0;i < 3;i++){
					resume_data[i]		= get_interrupt_data(i);
				}
			}else
				pautoplayer[0].score_count = pautoplayer[1].score_count = pautoplayer[2].score_count = 0;
			automatic_playing_start(ALLY_ATACK,SQUARE,0,0,0);//�U�������t
			while(playing_flg == ON){
				//nop
			}
			if(*dladder != LIFE){
				attack_ally 			= get_ally_data(*dladder);
				battle_display(ADD_ATTACK,dladder);
			}else
				battle_display(RECOVERY,dladder);
			//���t�ĊJ
			for(i = 0;i < 3;i++){
				pautoplayer[i] 		= resume_data[i];
			}
			automatic_playing_start(BATTLE1,SQUARE,resume_data[0].score_count,resume_data[1].score_count,resume_data[2].score_count);
			free_padding(dladder);//�󂢂���Δz����l�߂�
		}else{//�����̃^�[���I��
			sci0_receive_start();//��M���I����Ă���̂ŊJ�n
			break;
		}
	}
}
/********************************************************************
/*�G�̃^�[���֐�
/*void enemy_turn(void)
/*	�����Fstruct Enemy* enemy�@�U������G�̏��
/*	�߂�l�Funsigned char
********************************************************************/
void enemy_turn(void)
{
	//�G�̍U�������ǉ�
	battle_display(ENEMY_TURN,NULL);
	battle_display(TAKE_ATTACK,NULL);
}
/**************************************************************************
 * �o�g�����̉�ʕ\��
 * static void battle_display(unsigned char activity,unsigned char *param)
 * 	�����Funsigned char activity�@APPEARANCE�@�G�����ꂽ
 * 							ADD_ATTACK�@�G�ɍU����������
 * 							TAKE_ATTACK�@�G�ɍU�����ꂽ
 * 							KILLED_ENEMY�@�G��|�������̕\��
 * 							RECOVERY�@�񕜕\��
 * 							STATUS�@�X�e�[�^�X�\��
 * 		unsigned char *param�@����������΂̐擪�A�h���X�A����NULL
 */

static void battle_display(unsigned char activity,unsigned char *param)
{
	unsigned short damage_value = 0;
	static unsigned short combo_value = 0;//�R���{�J�E���g�̃��Z�b�g��Y��Ȃ��悤��
	unsigned char deleted_number;
	if(activity == ADD_ATTACK || activity == RECOVERY){
		/*�_���[�W�v�Z�ƃR���{���\��*/
		combo_value++;//
		deleted_number						= delete_jewel(param);//��΂������āA��������ΐ������炤
		if(combo_value >= 2){//�R���{�J�E���g���Q�ȏ�Ȃ�R���{���\��
			sprintf(output_display[COMBO],"%d%s",combo_value,COMBO_DISPLAY);
			send_serial((const unsigned char *)output_display[COMBO],strlen(output_display[COMBO]));
		}
	}
	switch(activity){
	case APPEARANCE:
		sprintf(output_display[APPEARANCE],"%s%s%s%s",COLOR_CHAR_ARRAY[penemy->el],penemy->name,DEFAULT_CHAR,APPEAR_DISPLAY);
		break;
	case ADD_ATTACK:
		damage_value			= damage_or_recovery_value_calculate(penemy,combo_value,attack_ally.el,deleted_number);//�_���[�W�v�Z
		sprintf(output_display[ADD_ATTACK],"%s%s%s%s%s%s%s%s%d%s%s",COLOR_CHAR_ARRAY[attack_ally.el],attack_ally.name,DEFAULT_CHAR,ATTACK_DISPLAY,COLOR_CHAR_ARRAY[penemy->el],penemy->name,DEFAULT_CHAR,TO_DISPLAY,damage_value,DAMAGE_DISPLAY,CRLF);
		if(penemy->hp >= damage_value)
			penemy->hp 			= penemy->hp - damage_value;//�����X�^�[��HP����_���[�W������
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
		/*�񕜕\���̎�*/
		damage_value			= damage_or_recovery_value_calculate(penemy,combo_value,LIFE,deleted_number);//�_���[�W�v�Z
		sprintf(output_display[RECOVERY],"%s%s%d%s",pplayer->name,LIFE_JEWEL_DISPLAY,damage_value,RECOVERY_DISPLAY);
		pplayer->hp = pplayer->hp + damage_value;//������HP�Ƀ_���[�W�𑫂��ő�l�𒴂��Ȃ��悤��
		if(pplayer->hp > pplayer->mhp)
			pplayer->hp = pplayer->mhp;
		break;
	case STATUS:
		//�����̃X�e�[�^�X���\������
		sprintf(output_display[STATUS],"%s%s%s%s%s%s%s%s%s%s%d/%d%s%s%s%s%s %s%s %s%s %s%s%s%s%s%s%d/%d%s%s%s%s%s",
																LINE_DISPLAY,CRLF,CRLF,
																CURSOL_MOVING_SENTER,COLOR_CHAR_ARRAY[penemy->el],penemy->name,DEFAULT_CHAR,CRLF,
																CURSOL_MOVING_SENTER,HP_DISPLAY,penemy->hp,penemy->mhp,CRLF,CRLF,CRLF,
																COLOR_CHAR_ARRAY[pally[FIRE].el],pally[FIRE].name,
																COLOR_CHAR_ARRAY[pally[WATER].el],pally[WATER].name,
																COLOR_CHAR_ARRAY[pally[WIND].el],pally[WIND].name,
																COLOR_CHAR_ARRAY[pally[SOIL].el],pally[SOIL].name,DEFAULT_CHAR,CRLF,
																CURSOL_MOVING_SENTER,HP_DISPLAY,pplayer->hp,pplayer->mhp,CRLF,
																CRLF,CRLF,LINE_DISPLAY,CRLF);
		combo_value 			= 0;
		break;
	case ENEMY_TURN:
	case PLAYER_TURN:
		//�^�[���\��
		if(param != NULL)//�v���[���[�̃^�[���Ȃ�v���C���[��
			sprintf(output_display[PLAYER_TURN],"�y%s�̃^�[���z\r\n",pplayer->name);
		else
			sprintf(output_display[ENEMY_TURN],"�y%s%s%s�̃^�[���z\r\n",COLOR_CHAR_ARRAY[penemy->el],penemy->name,DEFAULT_CHAR);
		break;
	}
	send_serial((const unsigned char *)output_display[activity],strlen(output_display[activity]));
}
//�G�Ɩ����̃X�e�[�^�X��\������
/********************************************************************/
/*���l�𕶎���ɕϊ�����											*/
/*void i_to_a(unsigned short i)										*/
/*	�����Funsigned short i ���l										*/
/********************************************************************/
//���͂�65535�܂�
//�o�͂̓��[�N�G���A��`��output_string�ɕϊ������
//�Ō��NULL����������
/*
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
}*/

/********************************************************************/
/*else if(ret == 3){
				if(input[i] < 0x41 || input[i] > 0x4d)//1������A~M�ȊO�̓���
					//nop
				else if(input[i] < 0x61 || input[i] > 0x6d)//1������a~m�ȊO�̓���
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
		}else if(ret >= SW1 && ret <= SW13){//�X�C�b�`�������ꂽ�Ƃ�
			g_start[0]	= speaker[0].score_count;
			g_start[1]	= speaker[1].score_count;
			g_start[2]	= speaker[2].score_count;
			input[i]	= ret;
			i++;*/

