/*
 * speaker.h
 *
 *  Created on: 2023/10/21
 *      Author: kameyamahiroki
 */

#ifndef __SPEAKER_H__
#define __SPEAKER_H__
void speaker_initialize(void);
void set_output_value(unsigned char output_num,unsigned char speaker_num);
void set_output_speaker_length(unsigned char set_pattern);
void output_speaker_start(unsigned char pattern);
void mute(unsigned char speaker_num);
typedef struct SPEAKER{
	float duty_value;					//�f���[�e�B��
	unsigned char wave_type;			//�o�͔g�`�i�X�s�[�J�[�P�́j
	unsigned char speaker_num;			//�X�s�[�J�[�ԍ�
}SPEAKER;
#endif /*__SPEAKER_H__ */
