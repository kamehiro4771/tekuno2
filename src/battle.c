

/********************************************************************/
/*�w�b�_�[�t�@�C��													*/
/********************************************************************/
#include "main.h"
#if 1
/*********************************************************************/
/*�萔��`															 */
/*********************************************************************/
T_DISPLAY ATTACK[]				= {"�̍U���I\n"};
T_DISPLAY COMBO[]				= {"�R���{�I\n"};
T_DISPLAY ADD_DAMAGE[]			= {"�̃_���[�W�I\n"};
T_DISPLAY TAKE_DAMAGE[]			= {"���������I\n"};
T_DISPLAY RECOVERY[]			= {"�񕜁I\n"};
T_DISPLAY APPEAR[]				= {"�����ꂽ�I\n"};
T_DISPLAY KILL[]				= ("��|�����I\n");
T_DISPLAY REQUEST_COMMAND[]		= {"�R�}���h?>>"};
T_DISPLAY OPERATION_METHOD[]	= {"�ꕶ���ړ�������΂̌��ݒn�A2�����ړ�������"};
T_DISPLAY INPUT_ERROR[]			= {"���͂�����������܂���"};
T_DISPLAY HP[]					= {"HP="};
T_DISPLAY COLOR_CHAR_ARRAY[COLOR_NUM][6] = {RED_CHAR,BLUE_CHAR,GREEN_CHAR,YELLOW_CHAR,PURPLE_CHAR};
/*********************************************************************/
/*���[�N�G���A��`													 */
/*********************************************************************/
AUTOPLAYER *resume_data;
unsigned char output_string[128];
unsigned char first_turn_flg;
/********************************************************************/
/*�v���g�^�C�v�錾													*/
/********************************************************************/
void player_turn(Player *player,Enemy* enemy);
void enemy_turn(Player *player,struct Enemy* enemy);
void i_to_a(unsigned short i);
void display_about_monster(Enemy *enemy,unsigned char activity,unsigned short damage);
/********************************************************************/
/*�o�g�����C���֐�													*/
/*unsigned char battle_main(struct Enemy* enemy)					*/
/*  �����Fstruct Enemy* enemy �ΐ킵�Ă���G���					*/
/*	�߂�l�Funsigned char ret 1����									*/
/*							  0�s�k									*/
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
/*�v���[���[�̃^�[���֐�											*/
/*void player_turn(Player *player,Enemy* enemy)							*/
/*	�����Fstruct Enemy* enemy �퓬���̓G�̃f�[�^					*/
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
	�U��������Ȃ����������Ȃ�
//	send_serial(CURSOR_2LINE_BUCK,5);//�G���^�[��������ăJ�[�\����������̂�1�s�߂�
	move_jewel(input[0],input[1]);//��΂𓮂���
	while(1){
		dladder							= count_jewel();//3�ȏ��΂���v���Ă�����z��̃A�h���X��Ԃ��B��v���ĂȂ�������NULL��Ԃ�
		deleted_type					= *dladder;
		if(dladder != NULL){
			combo_count++;//
			deleted_number				= delete_jewel(dladder);//��΂�������������ΐ������炤
			send_serial(CURSOR_2LINE_ADVANCE,4);
			i_to_a(combo_count);//
			send_serial(output_string,strlen((const char*)output_string));//�u�Z�R���{�I�v�\��
			send_serial(COMBO,sizeof(COMBO));
			damage						= damage_calculation(enemy,combo_count,deleted_type,deleted_number);//�_���[�W�v�Z
			if(deleted_type != LIFE){
				auto_play_end_processing();
				resume_data			= get_interrupt_data;
				automatic_playing(ALLY_ATACK,SQUARE,0,0,0);//�U�������t
				while(playing_flg == ON){
					//nop
				}
				i_to_a(damage);


				if(enemy->hp >= damage)
					enemy->hp 			= enemy->hp - damage;//�����X�^�[��HP����_���[�W������
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
				send_serial(RECOVERY,sizeof(RECOVERY));//�u�񕜁I�v
				player->hp = player->hp + damage;//������HP�Ƀ_���[�W�𑫂��ő�l�𒴂��Ȃ��悤��
				if(player->hp > player->mhp)
					player->hp = player->mhp;
			}
			pautoplayer[0].elapsed_time = resume_data[0].elapsed_time;
			pautoplayer[1].elapsed_time = resume_data[1].elapsed_time;
			pautoplayer[2].elapsed_time = resume_data[2].elapsed_time;
			automatic_playing(BATTLE1,SQUARE,resume_data[0].score_count,resume_data[1].score_count,resume_data[2].score_count);
			send_serial(CURSOR_5LINE_BUCK,sizeof(CURSOR_5LINE_BUCK));
			free_padding(dladder);//�󂢂���Δz����l�߂�
		}else{
			auto_play_end_processing();
			resume_data			= get_interrupt_data();
			break;
		}
	}
	send_serial(CURSOR_2LINE_ADVANCE,4);
}

/********************************************************************/
/*�G�̃^�[���֐�													*/
/*void enemy_turn(Player *player,struct Enemy* enemy)					*/
/*	�����Fstruct Enemy* enemy�@�U������G�̏��
/*	�߂�l�Funsigned char
/********************************************************************/
void enemy_turn(Player *player,Enemy* enemy)
{
	unsigned short ret;
	ret	= damge_from_enemy_calculation(player->gp,enemy);
	display_about_monster(enemy,TAKE_ATTACK,ret);
	player->hp = player->hp - ret;
}

/********************************************************************/
/*���l�𕶎���ɕϊ�����											*/
/*void i_to_a(unsigned short i)										*/
/*	�����Funsigned short i ���l										*/
/********************************************************************/
//���͂�65535�܂�
//�o�͂̓��[�N�G���A��`��output_string�ɕϊ������
//�Ō��NULL����������
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

//�����X�^�[�����ꂽ���A�����X�^�[���U�������Ƃ��A�����X�^�[���U�������Ƃ��A�����X�^�[��|�������A�p�����[�^�̕\��
void display_about_monster(Enemy *enemy,unsigned char activity,unsigned short param)
{
	send_serial(COLOR_CHAR_ARRAY[enemy->el],sizeof(COLOR_CHAR_ARRAY[enemy->el]));
	send_serial(enemy->name,strlen((const char*)enemy->name));//�����X�^�[���\��
	send_serial(DEFAULT_CHAR,sizeof(DEFAULT_CHAR));
	switch(activity){
	case APPEARANCE:
		send_serial(APPEAR,sizeof(APPEAR));
		break;
	case ADD_ATTACK:
		i_to_a(param);
		send_serial(output_string,strlen((const char*)output_string));//�_���[�W�l�\��
		send_serial(ADD_DAMAGE,sizeof(ADD_DAMAGE));//�̃_���[�W
		break;
	case TAKE_ATTACK:
		send_serial(ATTACK,sizeof(ATTACK));
		i_to_a(param);
		send_serial(output_string,strlen((const char*)output_string));//�_���[�W�l�\��
		break;
	case KILLED_ENEMY:

		break;
	case STATUS:
		send_serial("\n",1);
		i_to_a(param);
		send_serial(HP,sizeof(HP));
		send_serial(output_string,strlen((const char*)output_string));//HP�\��
		send_serial("\n",1);
		break;
	}
}
//�G�Ɩ����̃X�e�[�^�X��\������
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

#endif
