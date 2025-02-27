/*
 * score.h
 *
 *  Created on: 2025/02/27
 *      Author: kameyamahiroki
 */
#include "main.h"
#ifndef SCORE_H_
#define SCORE_H_

extern const unsigned short *NOTE_POINTER_ARRAY[SONG_NUM][SPEAKER_NUM];
extern const unsigned char *SCORE_POINTER_ARRAY[SONG_NUM][SPEAKER_NUM];
extern const unsigned char USE_SPEAKER_ARRAY[SONG_NUM];
extern const unsigned short NOTE_SIZE_ARRAY[SONG_NUM][SPEAKER_NUM];
extern T_SCORE DORAGONQUEST_SCORE1[];
extern T_SCORE DORAGONQUEST_SCORE2[];
extern T_SCORE DORAGONQUEST_SCORE3[];
extern T_NOTE DORAGONQUEST_NOTE_VALUE1[];
extern T_NOTE DORAGONQUEST_NOTE_VALUE2[];
extern T_NOTE DORAGONQUEST_NOTE_VALUE3[];

#endif /* SCORE_H_ */
