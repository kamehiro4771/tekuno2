#include "main.h"

/************************************************************************************************************************************/
/*ƒ_ƒ[ƒW‹y‚Ñ‰ñ•œ—Ê‚ÌŒvZ																											*/
/*signed short damage_calculation(struct Enemy* enemy,unsigned short combo_count,unsigned char type,unsigned char deleted_number)	*/
/*	ˆø”Fstruct Enemy* enemy@“Gƒ‚ƒ“ƒXƒ^[‚Ìî•ñ																					*/
/*		@unsigned short combo_count@ƒRƒ“ƒ{”																						*/
/*		@unsigned char type@•óÎ‚Ìí—Ş																							*/
/*		@unsigned char deleted_number@Á–Å•óÎ”																					*/
/*	–ß‚è’lFsigned short@ŒvZŒ‹‰Ê																									*/
/************************************************************************************************************************************/
signed short damage_calculation(struct Enemy* enemy,unsigned short combo_count,unsigned char type,unsigned char deleted_number)
{
	unsigned char i,exponent;
	float result = 1.0, base = 1.5;
	float correction;
	signed short damage;
	unsigned char random_num	= random_number_acquisition(21);
	Ally *ally = get_ally_data(type);
	exponent	= deleted_number - 3 + combo_count;//‰½æ‚·‚é‚©‹‚ß‚é
	for(i = 0;i < exponent;i++)//—İæ‚·‚é
		result *= base;
	if(type == LIFE)//Á‚µ‚½•óÎ‚ª–½‘®«‚Ì
		damage	= ((20 * result) * (90 + random_num)) / 100;
	else{
		if(enemy->el == WATER && type == FIRE)//“G…E–¡•û‰Î
			correction	= 0.5;
		else if(enemy->el == FIRE && type == WIND)//“G‰ÎE–¡•û•—
			correction	= 0.5;
		else if(enemy->el == WIND && type == SOIL)//“G•—E–¡•û“y
			correction	= 0.5;
		else if(enemy->el == SOIL && type == WATER)//“G“yE–¡•û…
			correction	= 0.5;
		else if(enemy->el == FIRE && type == WATER)//“G‰ÎE–¡•û…
			correction	= 2.0;
		else if(enemy->el == WIND && type == FIRE)//“G•—E–¡•û‰Î
			correction	= 2.0;
		else if(enemy->el == SOIL && type == WIND)//“G“yE–¡•û•—
			correction	= 2.0;
		else if(enemy->el == WATER && type == SOIL)//“G…E–¡•û“y
			correction	= 2.0;
		else
			correction	= 1.0;
		damage	= ((ally->ap - enemy->gp) * correction * result * (90 + random_num)) / 100;
		if(damage <= 1)
			damage = 1;//‚PˆÈ‰º‚Ìê‡‚P‚Æ‚·‚é
	}
	return damage;
}
