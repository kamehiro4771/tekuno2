/*
 * sci.h
 *
 *  Created on: 2024/02/20
 *      Author: kameyamahiroki
 */

#ifndef SCI_H_
#define SCI_H_

typedef struct SCI{
	unsigned char receive_data[128];
	unsigned char receive_count;
	const unsigned char *send_data[64]; //���M����f�[�^�ւ̃|�C���^
	unsigned short send_length[64];		//���M����f�[�^�̒���
	unsigned char send_counter;			//
	unsigned char reg_cnt;				//���M����f�[�^�̓o�^��
	unsigned char reg_0_flg;			//�o�^�f�[�^0�t���O
	unsigned char send_compflg;			//���M���I���������m�F����t���O
	unsigned char enter_flg;			//�G���^�[�������ꂽ���m�F���邽�߂̃t���O
	unsigned long elapsed_time;			//�^�C�}�J�E���g�G���A
}SCI;
void sci0_init(int baudrate);
void sci0_received(void);
void sci0_receive_start(void);
void send_serial(const unsigned char *send_data,unsigned short length);
unsigned char *sci0_get_receive_data(void);
unsigned char sci0_data_cpy(unsigned char *data);
unsigned char sci0_str_cpy(unsigned char *data);
void sci0_receive_error(void);
unsigned char sci0_enter_check(void);
unsigned char sci0_find_received_data(unsigned char find_char);
signed long a_to_i(void);
unsigned char sci0_get_reg_0_flg(void);
void sci0_scanf(void);
int sci0_comp(const char* str);
#endif /* SCI_H_ */
