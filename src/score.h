/*
 * score.h
 *
 *  Created on: 2025/02/27
 *      Author: kameyamahiroki
 */
#include "auto_play.h"
#ifndef SCORE_H_
#define SCORE_H_

extern T_SCORE ave_maria_score[];
extern T_SCORE saint_march_score[];
extern T_SCORE menuett_score[];
extern T_SCORE jesu_joy_of_man_s_score[];
extern T_SCORE aura_lee_score[];
extern T_SCORE sakura_score[];
extern T_SCORE zyounetutairiku_score[];
extern T_SCORE let_it_be_score[];
extern T_SCORE nhk_nodozimann_score[];
extern T_SCORE DORAGONQUEST_SCORE1[];
extern T_SCORE DORAGONQUEST_SCORE2[];
extern T_SCORE DORAGONQUEST_SCORE3[];
extern T_SCORE boukennnosyo_score1[];
extern T_SCORE boukennnosyo_score2[];
extern T_SCORE boukennnosyo_score3[];
extern T_SCORE battle_score1[];
extern T_SCORE battle_score2[];
extern T_SCORE battle_score3[];
extern T_SCORE ally_attack_score[];
extern T_SCORE level_up_score1[];
extern T_SCORE level_up_score2[];
extern T_SCORE win_score[];
extern T_SCORE yadoya_score1[];
extern T_SCORE yadoya_score2[];
extern T_SCORE CANON_SCORE1[];
extern T_SCORE CANON_SCORE2[];
extern T_SCORE CANON_SCORE3[];
extern T_SCORE zennmetu_score1[];
extern T_SCORE zennmetu_score2[];
extern T_SCORE zennmetu_score3[];
extern T_SCORE initial_check_score[];
extern T_NOTE DORAGONQUEST_NOTE_VALUE1[];
extern T_NOTE DORAGONQUEST_NOTE_VALUE2[];
extern T_NOTE DORAGONQUEST_NOTE_VALUE3[];

extern T_NOTE *const NOTE_POINTER_ARRAY[SONG_NUM][SPEAKER_NUM];
extern T_SCORE *const SCORE_POINTER_ARRAY[SONG_NUM][SPEAKER_NUM];
extern const unsigned char USE_SPEAKER_ARRAY[SONG_NUM];
extern const unsigned short NOTE_SIZE_ARRAY[SONG_NUM][SPEAKER_NUM];
#endif /* SCORE_H_ */
