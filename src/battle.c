

/********************************************************************/
/*�w�b�_�[�t�@�C��													*/
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
//�\�����镶����ւ̃|�C���^�񎟌��z��̍s�͍s���A��͕\�����鏇��
/*const T_DISPLAY *DISPLAY_POINTER_ARRAY[][]		= {{APPEAR,,},//�����X�^�[�o��
													{,ATTACK,,TO,DAMAGE,},//�����X�^�[�ɍU����^����
													{ATTACK,DAMAGE,TAKE},//�����X�^�[����U�������炤
													{KILL},//�����X�^�[��|����
													{HP,},//�X�e�[�^�X�\��
													{},
};*/
const T_DISPLAY COLOR_CHAR_ARRAY[COLOR_NUM][6] 	= {RED_CHAR,BLUE_CHAR,GREEN_CHAR,YELLOW_CHAR,PURPLE_CHAR};
enum activity{APPEARANCE,ADD_ATTACK,TAKE_ATTACK,KILLED_ENEMY,RECOVERY,STATUS,COMBO};
/*********************************************************************/
/*���[�N�G���A��`													 */
/*********************************************************************/
AUTOPLAYER *resume_data;
T_ENEMY *penemy;//�퓬���̃����X�^�[�ւ̃|�C���^
T_PLAYER *pplayer;//�v���C���[�ւ̃|�C���^
T_ALLY attack_ally;//�U�����������X�^�[
unsigned char output_string[7][128];//�_���[�W�v�Z���ĕ�����ɕϊ������������
unsigned char first_turn_flg;
/********************************************************************/
/*�v���g�^�C�v�錾													*/
/********************************************************************/
void player_turn(void);
void enemy_turn(void);
void i_to_a(unsigned short i,unsigned char *output_string);
static void battle_display(unsigned char activity,unsigned char *param);
/********************************************************************/
/*�o�g�����C���֐�													*/
/*unsigned char battle_main(struct T_ENEMY* enemy)					*/
/*  �����Fstruct T_ENEMY* enemy �ΐ킵�Ă���G���					*/
/*	�߂�l�Funsigned char ret 1����									*/
/*							  0�s�k									*/
/********************************************************************/
unsigned char battle_main(T_PLAYER *player, T_ENEMY *enemy)
{
	//�����X�^�[��|���������������Ȃ�
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
/*�v���[���[�̃^�[���֐�											*/
/*void player_turn(T_PLAYER *player,T_ENEMY* enemy)							*/
/*	�����Fstruct T_ENEMY* enemy �퓬���̓G�̃f�[�^					*/
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
			if(ret == 4){//2�������͂��ꂽ
				sci0_data_cpy(&input[0]);
				if(input[0] >= 'A' && input[0] < 'N'){//1�����ڔ���
					if(input[1] >= 'A' && input[1] < 'N')//2�����ڔ���
						break;
					if(input[1] >= 'a' && input[1] < 'n')
						break;
				}
				if(input[0] >= 'a' && input[0] < 'n'){//1������a~m�ȊO�̓���
					if(input[1] >= 'A' && input[1] < 'N')//2�����ڔ���
						break;
					if(input[1] >= 'a' && input[1] < 'n')
						break;
				}
			}else{
				//��s��ɖ߂��āu�R�}���h>>�v�\��
				sci0_receive_start();
			}
		}else if(playing_flg == OFF){//�������͂��ꂸ�ɋȏI��
			automatic_playing(BATTLE1,SQUARE,0,0,0);
		}
	}
//	ml���͂Ńo�O��
	move_jewel(input[0],input[1]);//��΂𓮂���
	while(1){
		dladder							= count_jewel();//3�ȏ��΂���v���Ă�����z��̃A�h���X��Ԃ��B��v���ĂȂ�������NULL��Ԃ�
		if(dladder != NULL){
			auto_play_end_processing();
			for(i = 0;i < 3;i++)
				resume_data[i]			= get_interrupt_data(i);
			automatic_playing(ALLY_ATACK,SQUARE,0,0,0);//�U�������t
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
			//���t�ĊJ
			automatic_playing(BATTLE1,SQUARE,pautoplayer[0].score_count,pautoplayer[1].score_count,pautoplayer[2].score_count);
			free_padding(dladder);//�󂢂���Δz����l�߂�
		}else{
			sci0_receive_start();//��M���I����Ă���̂ŊJ�n
			break;
		}
	}
	send_serial(CURSOR_2LINE_ADVANCE,4);
}

/********************************************************************
/*�G�̃^�[���֐�
/*void enemy_turn(void)
/*	�����Fstruct Enemy* enemy�@�U������G�̏��
/*	�߂�l�Funsigned char
********************************************************************/
void enemy_turn(void)
{
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
	unsigned short combo_value = 0;//�R���{�J�E���g�̃��Z�b�g��Y��Ȃ��悤��
	unsigned char deleted_number;
	if(activity == ADD_ATTACK || activity == RECOVERY){
		/*�_���[�W�v�Z�ƃR���{���\��*/
		combo_value++;//
		deleted_number						= delete_jewel(param);//��΂������āA��������ΐ������炤
		damage_value						= damage_to_enemy_calculation(penemy,combo_value,attack_ally.el,deleted_number);//�_���[�W�v�Z
		if(combo_value >= 2){//�R���{�J�E���g���Q�ȏ�Ȃ�R���{���\��
			sprintf(output_string[COMBO],"%d%s",combo_value,COMBO_DISPLAY);
			send_serial(output_string[COMBO],strlen(output_string[COMBO]));
		}
	}
	if(activity == RECOVERY){
		/*�񕜕\���̎�*/
		sprintf(output_string[RECOVERY],"%s%s%d%s",pplayer->name,LIFE_JEWEL_DISPLAY,damage_value,RECOVERY_DISPLAY);
		pplayer->hp = pplayer->hp + damage_value;//������HP�Ƀ_���[�W�𑫂��ő�l�𒴂��Ȃ��悤��
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
				penemy->hp 			= penemy->hp - damage_value;//�����X�^�[��HP����_���[�W������
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
//�G�Ɩ����̃X�e�[�^�X��\������
/********************************************************************/
/*���l�𕶎���ɕϊ�����											*/
/*void i_to_a(unsigned short i)										*/
/*	�����Funsigned short i ���l										*/
/********************************************************************/
//���͂�65535�܂�
//�o�͂̓��[�N�G���A��`��output_string�ɕϊ������
//�Ō��NULL����������
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


