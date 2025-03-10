#include "main.h"

//定数定義
const float ATTRIBUTE_CORRECTION_VALUE[ATTRIBUTE_NUM][ATTRIBUTE_NUM] = {//[攻撃する側の属性][攻撃される側の属性]
											{1.0,0.5,2.0,1.0,},{2.0,1.0,1.0,0.5,},{0.5,1.0,1.0,2.0,},{1.0,2.0,0.5,1.0,},
																};
/************************************************************************************************************************************/
/*ダメージ及び回復量の計算																											*/
/*signed short damage_calculation(struct Enemy* enemy,unsigned short combo_count,unsigned char type,unsigned char deleted_number)	*/
/*	引数：struct Enemy* enemy　敵モンスターの情報																					*/
/*		　unsigned short combo_count　コンボ数																						*/
/*		　unsigned char type　宝石の種類																							*/
/*		　unsigned char deleted_number　消滅宝石数																					*/
/*	戻り値：signed short　計算結果																									*/
/************************************************************************************************************************************/
unsigned short damage_to_enemy_calculation(struct Enemy* enemy,unsigned short combo_count,unsigned char type,unsigned char deleted_number)
{
	unsigned char i,exponent;
	float result = 1.0, base = 1.5;
	unsigned short damage,ally_ap;
	unsigned char random_num	= random_number_acquisition(21);
	exponent	= deleted_number - 3 + combo_count;//何乗するか求める
	for(i = 0;i < exponent;i++)//累乗する
		result *= base;
	if(type == LIFE)//消した宝石が命属性の時
		damage	= ((20 * result) * (90 + random_num)) / 100;
	else{
		ally_ap = get_ally_data(type).ap;
		damage	= ((ally_ap - enemy->gp) * ATTRIBUTE_CORRECTION_VALUE[type][enemy->el] * result * (90 + random_num)) / 100;
		if(damage < 1)
			damage = 1;//１未満の場合１とする
	}
	return damage;
}

/*
 * 敵からのダメージ計算
 * unsigned short damge_from_enemy_calculation(struct Enemy* enemy)
 */
unsigned short damge_from_enemy_calculation(unsigned short player_gp,struct Enemy* enemy)
{
	unsigned short damage;
	unsigned char random_num	= random_number_acquisition(21);
	damage = enemy->ap - player_gp * (90 + random_num) / 100;
	return damage;
}
