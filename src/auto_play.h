#ifndef __AUTO_PLAY
#define __AUTO_PLAY
void autoplay_start(unsigned short title,unsigned char wave_type);
struct AUTOPLAYER *get_autoplayer(void);
struct AUTOPLAYER get_interrupt_data(unsigned char player_num);
#endif
