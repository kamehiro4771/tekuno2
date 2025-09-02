/*
 * damage.h
 *
 *  Created on: 2025/01/25
 *      Author: kameyamahiroki
 */

#include "game.h"
#ifndef DAMAGE_H_
#define DAMAGE_H_

unsigned short damage_or_recovery_value_calculate(struct Enemy* enemy,unsigned short combo_count,unsigned char type,unsigned char deleted_number);
unsigned short damge_from_enemy_calculation(unsigned short player_gp, unsigned short enemy_ap);
unsigned short recovery_value_calculate(unsigned short combo_count, unsigned char deleted_number);
unsigned short damage_from_ally_calculation(T_MONSTER* enemy, unsigned short combo_count, unsigned char type, unsigned char deleted_number);

#endif /* DAMAGE_H_ */
