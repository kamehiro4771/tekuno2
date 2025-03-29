#ifndef __AUTO_PLAY
#define __AUTO_PLAY
unsigned char automatic_playing_start(unsigned short title,unsigned char wave_type,unsigned short start1,unsigned short start2,unsigned short start3);
struct AUTOPLAYER *get_autoplayer(void);
struct AUTOPLAYER get_interrupt_data(unsigned char player_num);
#endif
