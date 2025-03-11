/*
 * damage.h
 *
 *  Created on: 2025/01/25
 *      Author: kameyamahiroki
 */

#ifndef DAMAGE_H_
#define DAMAGE_H_

unsigned short damage_or_recovery_value_calculate(struct Enemy* enemy,unsigned short combo_count,unsigned char type,unsigned char deleted_number);
unsigned short damge_from_enemy_calculation(unsigned short player_gp,struct Enemy* enemy);

#endif /* DAMAGE_H_ */
