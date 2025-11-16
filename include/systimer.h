/*
 * systimer.h
 *
 *  Created on: 2025/11/16
 *      Author: kameyamahiroki
 */

#ifndef SYSTIMER_H_
#define SYSTIMER_H_

void system_timer_open(void);
void count_time(void);
unsigned char interval_function_set(unsigned long interval,void func(void));
unsigned char timer_area_registration(unsigned long* area);
typedef void (*T_FUNC_PTR)(void);							//引数なし、戻り値なしの関数へのポインタ
#endif /* SYSTIMER_H_ */
