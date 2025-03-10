#include "main.h"

//�萔��`
const float ATTRIBUTE_CORRECTION_VALUE[ATTRIBUTE_NUM][ATTRIBUTE_NUM] = {//[�U�����鑤�̑���][�U������鑤�̑���]
											{1.0,0.5,2.0,1.0,},{2.0,1.0,1.0,0.5,},{0.5,1.0,1.0,2.0,},{1.0,2.0,0.5,1.0,},
																};
/************************************************************************************************************************************/
/*�_���[�W�y�щ񕜗ʂ̌v�Z																											*/
/*signed short damage_calculation(struct Enemy* enemy,unsigned short combo_count,unsigned char type,unsigned char deleted_number)	*/
/*	�����Fstruct Enemy* enemy�@�G�����X�^�[�̏��																					*/
/*		�@unsigned short combo_count�@�R���{��																						*/
/*		�@unsigned char type�@��΂̎��																							*/
/*		�@unsigned char deleted_number�@���ŕ�ΐ�																					*/
/*	�߂�l�Fsigned short�@�v�Z����																									*/
/************************************************************************************************************************************/
unsigned short damage_to_enemy_calculation(struct Enemy* enemy,unsigned short combo_count,unsigned char type,unsigned char deleted_number)
{
	unsigned char i,exponent;
	float result = 1.0, base = 1.5;
	unsigned short damage,ally_ap;
	unsigned char random_num	= random_number_acquisition(21);
	exponent	= deleted_number - 3 + combo_count;//���悷�邩���߂�
	for(i = 0;i < exponent;i++)//�ݏ悷��
		result *= base;
	if(type == LIFE)//��������΂��������̎�
		damage	= ((20 * result) * (90 + random_num)) / 100;
	else{
		ally_ap = get_ally_data(type).ap;
		damage	= ((ally_ap - enemy->gp) * ATTRIBUTE_CORRECTION_VALUE[type][enemy->el] * result * (90 + random_num)) / 100;
		if(damage < 1)
			damage = 1;//�P�����̏ꍇ�P�Ƃ���
	}
	return damage;
}

/*
 * �G����̃_���[�W�v�Z
 * unsigned short damge_from_enemy_calculation(struct Enemy* enemy)
 */
unsigned short damge_from_enemy_calculation(unsigned short player_gp,struct Enemy* enemy)
{
	unsigned short damage;
	unsigned char random_num	= random_number_acquisition(21);
	damage = enemy->ap - player_gp * (90 + random_num) / 100;
	return damage;
}
