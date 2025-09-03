#include "main.h"

//定数定義
const float ATTRIBUTE_CORRECTION_VALUE[ATTRIBUTE_NUM][ATTRIBUTE_NUM] = {//[攻撃する側の属性][攻撃される側の属性]
											//火属性				水属性				風属性				土属性
											{1.0,0.5,2.0,1.0,},{2.0,1.0,1.0,0.5,},{0.5,1.0,1.0,2.0,},{1.0,2.0,0.5,1.0,},
																};

/**********************************************************************************/
/*1.5のべき乗を求める															　*/
/*float power_calculate(unsigned short combo_count, unsigned char deleted_number) */
/*引数：unsigned short combo_count コンボした数									  */
/*		unsigned char deleted_number 消滅した宝石の数							  */
/*戻り値：float result 計算結果													  */
/**********************************************************************************/
float power_calculate(unsigned short combo_count, unsigned char deleted_number)
{
	unsigned char i, exponent;
	float result = 1.0, base = 1.5;
	exponent = deleted_number - 3 + combo_count;//1.5を何乗するか（消滅宝石数　ー　３　＋　コンボ数）
	for (i = 0; i < exponent; i++)//1.5を累乗する
		result *= base;
	return result;
}

/**********************************************************************************/
/*回復値の計算*/
/*unsigned short recovery_value_calculate(unsigned short combo_count, unsigned char deleted_number)*/
/*引数：*/
/*戻り値：*/
/**********************************************************************************/
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

/**********************************************************************************/
/*味方から敵へのダメージ計算*/
/*unsigned short damage_from_ally_calculation(T_MONSTER enemy, T_MONSTER ally, unsigned short combo_count, unsigned char deleted_number)*/
/*引数：*/
/*戻り値：*/
/**********************************************************************************/
unsigned short damage_from_ally_calculation(T_MONSTER enemy, T_MONSTER ally, unsigned short combo_count, unsigned char deleted_number)
{
	float result;
	unsigned short value;
	unsigned char random_num;
	srand(random_number_acquisition());
	random_num = rand() % 21;
	result = power_calculate(combo_count, deleted_number);
	value = ((((ally.ap - enemy.gp) > 0) ? (ally.ap - enemy.gp) : 1) * ATTRIBUTE_CORRECTION_VALUE[ally.el][enemy.el] * result * (90 + random_num)) / 100;
	return value;
}
/*
 * 敵からのダメージ計算
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
