/*
 * game.h
 *
 *  Created on: 2024/05/27
 *      Author: kameyamahiroki
 */

#ifndef GAME_H_
#define GAME_H_
/********************************************************************************************/
/*プロトタイプ宣言																		*/
/********************************************************************************************/
void game_main(void);
/********************************************************************************************/
/*構造体宣言																				*/
/********************************************************************************************/
typedef struct Enemy{
	unsigned char name[32];
	signed short hp;
	unsigned short mhp;
	unsigned short el;
	unsigned short ap;
	unsigned short gp;
}Enemy;

typedef struct Ally{
	unsigned char name[64];
	unsigned short hp;
	unsigned short el;
	unsigned short ap;
	unsigned short gp;
}Ally;

typedef struct Player{
	unsigned char name[64];
	signed short hp;
	unsigned short mhp;
	unsigned short gp;
}Player;

Ally* get_ally_data(unsigned char type);
Enemy* get_enemy_data(unsigned char num);


#endif /* GAME_H_ */
