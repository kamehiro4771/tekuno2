/*
 * sci.h
 *
 *  Created on: 2024/02/20
 *      Author: kameyamahiroki
 */

#ifndef SCI_H_
#define SCI_H_

void sci0_init(int baudrate);
void sci0_received(void);
void sci0_receive_start(void);
void send_serial(const unsigned char *send_data,unsigned short length);
int sci0_return_keyboard_value(void);
unsigned char *sci0_get_receive_data(void);
unsigned char sci0_data_cpy(unsigned char *data);
unsigned char sci0_str_cpy(unsigned char *data);
void sci0_receive_error(void);
unsigned char sci0_enter_check(void);
unsigned char sci0_find_received_data(unsigned char find_char);
signed long a_to_i(void);
#endif /* SCI_H_ */
