#ifndef __AUTO_PLAY
#define __AUTO_PLAY
void automatic_playing(unsigned short title,unsigned char wave_type,unsigned short start1,unsigned short start2,unsigned short start3);
struct SPEAKER get_interrupt_data(speaker_num);
unsigned char input_check(void);
#endif
