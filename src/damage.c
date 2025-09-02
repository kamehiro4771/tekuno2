#include "main.h"

//�萔��`
const float ATTRIBUTE_CORRECTION_VALUE[ATTRIBUTE_NUM][ATTRIBUTE_NUM] = {//[�U�����鑤�̑���][�U������鑤�̑���]
											//�Α���				������				������				�y����
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
unsigned short damage_or_recovery_value_calculate(T_MONSTER* enemy,unsigned short combo_count,unsigned char type,unsigned char deleted_number)
{
	unsigned char i,exponent;
	float result = 1.0, base = 1.5;
	unsigned short ally_ap;
	signed short value;
	unsigned char random_num;
	srand(random_number_acquisition());
	random_num	= rand() % 21;
	exponent	= deleted_number - 3 + combo_count;//���悷�邩���߂�
	for(i = 0;i < exponent;i++)//�ݏ悷��
		result *= base;
	if(type == LIFE)//��������΂��������̎�
		value	= ((20 * result) * (90 + random_num)) / 100;
	else{
		ally_ap = get_ally_data(type).ap;
		value	= ((((ally_ap - enemy->gp) > 0) ? (ally_ap - enemy->gp) : 1) * ATTRIBUTE_CORRECTION_VALUE[type][enemy->el] * result * (90 + random_num)) / 100;
	}
	return value;
}

/**********************************************************************************/
/*1.5�ׂ̂�������߂�															�@*/
/*float power_calculate(unsigned short combo_count, unsigned char deleted_number) */
/**/
/**/
/**/
/**********************************************************************************/
float power_calculate(unsigned short combo_count, unsigned char deleted_number)
{
	unsigned char i, exponent;
	float result = 1.0, base = 1.5;
	exponent = deleted_number - 3 + combo_count;//1.5�����悷�邩�i���ŕ�ΐ��@�[�@�R�@�{�@�R���{���j
	for (i = 0; i < exponent; i++)//1.5��ݏ悷��
		result *= base;
	return result;
}

unsigned short recovery_value_calculate(unsigned short combo_count, unsigned char deleted_number)
{
	float result;
	unsigned char random_num;
	unsigned short value;
	srand(random_number_acquisition());
	random_num = rand() % 21;
	result = power_calculate(combo_count, deleted_number);
	value = ((20 * result) * (90 + random_num)) / 100;
	return value;
}

unsigned short damage_from_ally_calculation(T_MONSTER* enemy, unsigned short combo_count, unsigned char type, unsigned char deleted_number)
{
	float result;
	unsigned short ally_ap, value;
	unsigned char random_num;
	srand(random_number_acquisition());
	random_num = rand() % 21;
	result = power_calculate(combo_count, deleted_number);
	ally_ap = get_ally_data(type).ap;
	value = ((((ally_ap - enemy->gp) > 0) ? (ally_ap - enemy->gp) : 1) * ATTRIBUTE_CORRECTION_VALUE[type][enemy->el] * result * (90 + random_num)) / 100;
	return value;
}
/*
 * �G����̃_���[�W�v�Z
 * unsigned short damge_from_enemy_calculation(struct Enemy* enemy)
 */
unsigned short damge_from_enemy_calculation(unsigned short player_gp,unsigned short enemy_ap)
{
	unsigned short damage;
	unsigned char random_num;
	srand(random_number_acquisition());
	random_num = rand() % 21;
	damage = enemy_ap - player_gp * (90 + random_num) / 100;
	return damage;
}
