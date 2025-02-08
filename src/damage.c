#include "main.h"

/************************************************************************************************************************************/
/*�_���[�W�y�щ񕜗ʂ̌v�Z																											*/
/*signed short damage_calculation(struct Enemy* enemy,unsigned short combo_count,unsigned char type,unsigned char deleted_number)	*/
/*	�����Fstruct Enemy* enemy�@�G�����X�^�[�̏��																					*/
/*		�@unsigned short combo_count�@�R���{��																						*/
/*		�@unsigned char type�@��΂̎��																							*/
/*		�@unsigned char deleted_number�@���ŕ�ΐ�																					*/
/*	�߂�l�Fsigned short�@�v�Z����																									*/
/************************************************************************************************************************************/
signed short damage_calculation(struct Enemy* enemy,unsigned short combo_count,unsigned char type,unsigned char deleted_number)
{
	unsigned char i,exponent;
	float result = 1.0, base = 1.5;
	float correction;
	signed short damage;
	unsigned char random_num	= random_number_acquisition(21);
	Ally *ally = get_ally_data(type);
	exponent	= deleted_number - 3 + combo_count;//���悷�邩���߂�
	for(i = 0;i < exponent;i++)//�ݏ悷��
		result *= base;
	if(type == LIFE)//��������΂��������̎�
		damage	= ((20 * result) * (90 + random_num)) / 100;
	else{
		if(enemy->el == WATER && type == FIRE)//�G���E������
			correction	= 0.5;
		else if(enemy->el == FIRE && type == WIND)//�G�΁E������
			correction	= 0.5;
		else if(enemy->el == WIND && type == SOIL)//�G���E�����y
			correction	= 0.5;
		else if(enemy->el == SOIL && type == WATER)//�G�y�E������
			correction	= 0.5;
		else if(enemy->el == FIRE && type == WATER)//�G�΁E������
			correction	= 2.0;
		else if(enemy->el == WIND && type == FIRE)//�G���E������
			correction	= 2.0;
		else if(enemy->el == SOIL && type == WIND)//�G�y�E������
			correction	= 2.0;
		else if(enemy->el == WATER && type == SOIL)//�G���E�����y
			correction	= 2.0;
		else
			correction	= 1.0;
		damage	= ((ally->ap - enemy->gp) * correction * result * (90 + random_num)) / 100;
		if(damage <= 1)
			damage = 1;//�P�ȉ��̏ꍇ�P�Ƃ���
	}
	return damage;
}
