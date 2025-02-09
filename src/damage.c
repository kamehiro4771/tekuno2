#include "main.h"

/************************************************************************************************************************************/
/*ダメージ及び回復量の計算																											*/
/*signed short damage_calculation(struct Enemy* enemy,unsigned short combo_count,unsigned char type,unsigned char deleted_number)	*/
/*	引数：struct Enemy* enemy　敵モンスターの情報																					*/
/*		　unsigned short combo_count　コンボ数																						*/
/*		　unsigned char type　宝石の種類																							*/
/*		　unsigned char deleted_number　消滅宝石数																					*/
/*	戻り値：signed short　計算結果																									*/
/************************************************************************************************************************************/
signed short damage_calculation(struct Enemy* enemy,unsigned short combo_count,unsigned char type,unsigned char deleted_number)
{
	unsigned char i,exponent;
	float result = 1.0, base = 1.5;
	float correction;
	signed short damage;
	unsigned char random_num	= random_number_acquisition(21);
	Ally *ally = get_ally_data(type);
	exponent	= deleted_number - 3 + combo_count;//何乗するか求める
	for(i = 0;i < exponent;i++)//累乗する
		result *= base;
	if(type == LIFE)//消した宝石が命属性の時
		damage	= ((20 * result) * (90 + random_num)) / 100;
	else{
		if(enemy->el == WATER && type == FIRE)//敵水・味方火
			correction	= 0.5;
		else if(enemy->el == FIRE && type == WIND)//敵火・味方風
			correction	= 0.5;
		else if(enemy->el == WIND && type == SOIL)//敵風・味方土
			correction	= 0.5;
		else if(enemy->el == SOIL && type == WATER)//敵土・味方水
			correction	= 0.5;
		else if(enemy->el == FIRE && type == WATER)//敵火・味方水
			correction	= 2.0;
		else if(enemy->el == WIND && type == FIRE)//敵風・味方火
			correction	= 2.0;
		else if(enemy->el == SOIL && type == WIND)//敵土・味方風
			correction	= 2.0;
		else if(enemy->el == WATER && type == SOIL)//敵水・味方土
			correction	= 2.0;
		else
			correction	= 1.0;
		damage	= ((ally->ap - enemy->gp) * correction * result * (90 + random_num)) / 100;
		if(damage <= 1)
			damage = 1;//１以下の場合１とする
	}
	return damage;
}
