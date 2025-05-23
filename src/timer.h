/*
 * timer.h
 *
 *  Created on: 2024/01/08
 *      Author: kameyamahiroki
 */

#ifndef TIMER_H_
#define TIMER_H_

void mtu0_initialize(void);
void mtu1_initialize(void);
void cmt1_initiralize(void);
void count_timer_start(void);
unsigned char count_timer_set(unsigned long *timer,void func(void));
void count_timer_dell(void func(void));
void cmt2_wait(unsigned long cnt,unsigned char cks);
unsigned short random_number_acquisition(void);
typedef void (*T_FUNC_PTR)(void);							//引数なし、戻り値なしの関数へのポインタ

#endif /* TIMER_H_ */
