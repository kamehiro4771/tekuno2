

/********************************************************************/
/*�w�b�_�[�t�@�C��													*/
/********************************************************************/
#include "main.h"
#if 1
/*********************************************************************/
/*�萔��`															 */
/*********************************************************************/
const unsigned char ATTACK[]			= {"�U���I\n"};
const unsigned char COMBO[]				= {"�R���{�I\n"};
const unsigned char DAMAGE[]			= {"�̃_���[�W�I\n"};
const unsigned char RECOVERY[]			= {"�񕜁I\n"};
const unsigned char APPEAR[]			= {"�����ꂽ�I\n"};
const unsigned char OPERATION_METHOD[]	= {"�ꕶ���ړ�������΂̌��ݒn�A2�����ړ�������"};
const unsigned char INPUT_ERROR[]		= {"���͂�����������܂���"};

/*********************************************************************/
/*���[�N�G���A��`													 */
/*********************************************************************/
struct playing_resume_point resume_point[3] = {0};//�퓬�̃e�[�}���ĊJ����ʒu
struct SPEAKER *battle_speaker;
unsigned char output_string[128];
unsigned char first_turn_flg;
/********************************************************************/
/*�v���g�^�C�v�錾													*/
/********************************************************************/
void player_turn(struct Enemy* enemy);
void enemy_turn(struct Enemy* enemy);
void i_to_a(unsigned short i);
void learn_resume_point(void);
/********************************************************************/
/*�o�g�����C���֐�													*/
/*unsigned char battle_main(struct Enemy* enemy)					*/
/*  �����Fstruct Enemy* enemy �ΐ킵�Ă���G���					*/
/*	�߂�l�Funsigned char ret 1����									*/
/*							  0�s�k									*/
/********************************************************************/
unsigned char battle_main(Player *player, Enemy *enemy)
{
	battle_speaker = get_speaker();
	first_turn_flg = ON;
	send_serial(enemy->name,sizeof(enemy->name));//�G�̖��O�\��
	send_serial(APPEAR,sizeof(APPEAR));//�u���ꂽ�I�v�\��
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
/*�v���[���[�̃^�[���֐�											*/
/*void player_turn(struct Enemy* enemy)								*/
/*	�����Fstruct Enemy* enemy �퓬���̓G�̃f�[�^					*/
/********************************************************************/
void player_turn(struct Enemy* enemy)
{
	unsigned char input[2] = {0};
	unsigned char *dladder;
	unsigned char ret,deleted_type,deleted_number,i = 0;
	unsigned short combo_count = 0;
	signed short damage;
	send_serial(enemy->name,strlen((const char*)enemy->name));//�G�Ǝ����̃p�����[�^�[�\��
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
		if(ret == ON){//�G���^�[�������ꂽ�Ƃ�
			resume_point[0].start			= battle_speaker[0].score_count;
			resume_point[1].start			= battle_speaker[1].score_count;
			resume_point[2].start			= battle_speaker[2].score_count;
			battle_speaker[0].elapsed_time	= get_interrupt_data(0).elapsed_time;//�o�ߎ��Ԃ��o����
			battle_speaker[1].elapsed_time	= get_interrupt_data(1).elapsed_time;
			battle_speaker[2].elapsed_time	= get_interrupt_data(2).elapsed_time;
			ret								= sci0_get_receive_count();
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
			}
		}else//�������͂��ꂸ�ɋȏI��
			resume_point[0].start	= resume_point[1].start = resume_point[2].start = 0;//�Ȃ̉��t�J�n�ʒu���ŏ��ɐݒ�
	}
	automatic_playing(BATTLE1,SQUARE,resume_point[0].start,resume_point[1].start,resume_point[2].start);
	move_jewel(input[0],input[1]);//��΂𓮂���
	while(1){
		dladder							= count_jewel();//3�ȏ��΂���v���Ă�����z��̃A�h���X��Ԃ��B��v���ĂȂ�������NULL��Ԃ�
		deleted_type					= *dladder;
		if(dladder != NULL){
			play_up_to_last				= ON;//��Γ��������́A�������t�G���^�[��X�C�b�`�ŏI�����Ȃ��ݒ�
			combo_count++;//
			deleted_number				= delete_jewel(dladder);//��΂�������������ΐ������炤
			send_serial(CURSOR_2LINE_ADVANCE,4);
			i_to_a(combo_count);//
			send_serial(output_string,strlen((const char*)output_string));//�u�Z�R���{�I�v�\��
			send_serial(COMBO,sizeof(COMBO));
			damage						= damage_calculation(enemy,combo_count,deleted_type,deleted_number);//�_���[�W�v�Z
			if(deleted_type != LIFE){//damage���}�C�i�X�Ȃ�G�ւ̃_���[�W�A�v���X�Ȃ�񕜕\��
				auto_play_end_processing();
				resume_point[0].start		= battle_speaker[0].score_count;
				resume_point[1].start		= battle_speaker[1].score_count;
				resume_point[2].start		= battle_speaker[2].score_count;
				resume_point[0].elapsed		= get_interrupt_data(0).elapsed_time;//�o�ߎ��Ԃ��o����
				resume_point[1].elapsed		= get_interrupt_data(1).elapsed_time;
				resume_point[2].elapsed		= get_interrupt_data(2).elapsed_time;
				automatic_playing(ALLY_ATACK,SQUARE,0,0,0);//�U�������t
				play_up_to_last				= OFF;
				send_serial(enemy->name,strlen((const char*)enemy->name));//�����X�^�[���\��
				i_to_a(damage);
				send_serial(output_string,sizeof(output_string));//�_���[�W�l�\��
				send_serial(DAMAGE,sizeof(DAMAGE));//�̃_���[�W
				if(enemy->hp >= damage)
					enemy->hp 				= enemy->hp - damage;//�����X�^�[��HP����_���[�W������
				else
					enemy->hp 				= 0;
			}else{
				auto_play_end_processing();
				resume_point[0].start		= battle_speaker[0].score_count;
				resume_point[1].start		= battle_speaker[1].score_count;
				resume_point[2].start		= battle_speaker[2].score_count;
				resume_point[0].elapsed		= get_interrupt_data(0).elapsed_time;//�o�ߎ��Ԃ��o����
				resume_point[1].elapsed		= get_interrupt_data(1).elapsed_time;
				resume_point[2].elapsed		= get_interrupt_data(2).elapsed_time;
				automatic_playing(ALLY_ATACK,SQUARE,0,0,0);
				i_to_a(damage);
				send_serial(output_string,strlen((const char*)output_string));
				send_serial(RECOVERY,sizeof(RECOVERY));//�u�񕜁I�v
				//������HP�Ƀ_���[�W�𑫂�
			}
			free_padding(dladder);//�󂢂���Δz����l�߂�
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
/*�G�̃^�[���֐�													*/
/*unsigned char player_turn(struct Enemy* enemy)					*/
/*	�����Fstruct Enemy* enemy�@�U������G�̏��
/*	�߂�l�Funsigned char
/********************************************************************/
void enemy_turn(struct Enemy* enemy)
{
	
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

/*
 * ���t���f�����ʒu���o����
 * void learn_resume_point(RESUME_POINT resume_point)
 */
/*
void learn_resume_point(RESUME_POINT resume_point)
{
	resume_point.start[0]		= speaker[0].score_count;
	resume_point.start[1]		= speaker[1].score_count;
	resume_point.start[2]		= speaker[2].score_count;
	resume_point.elapsed[0]		= get_interrupt_data(0).elapsed_time;//�o�ߎ��Ԃ��o����
	resume_point.elapsed[1]		= get_interrupt_data(1).elapsed_time;
	resume_point.elapsed[2]		= get_interrupt_data(2).elapsed_time;
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

#endif
