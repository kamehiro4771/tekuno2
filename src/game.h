/*
 * game.h
 *
 *  Created on: 2024/05/27
 *      Author: kameyamahiroki
 */

#ifndef GAME_H_
#define GAME_H_

#define FIELD_NUM (13)//�o�g���t�B�[���h�̐�
#define ONE_JEWEL (6)//��̕�΂̃o�C�g��
#define ENEMY_NUM (5)//�G�����X�^�[�̐�
#define ALLY_NUM (4)//�����̐�
#define FIRE (0)
#define WATER (1)
#define WIND (2)
#define SOIL (3)
#define LIFE (4)
#define COLOR_NUM (5)
#define WIN (1)
#define LOSE (0)
#define NEW_FIELD (0)
#define CURRENT_FIELD (1)
#define UPDATE_FIELD (2)
#define ATTRIBUTE_NUM (4)
/********************************************************************************************/
/*�v���g�^�C�v�錾																		*/
/********************************************************************************************/
void game_main(void);
/********************************************************************************************/
/*�\���̐錾																				*/
/********************************************************************************************/
typedef struct Enemy{
	unsigned char name[32];
	signed short hp;
	unsigned short mhp;
	unsigned short el;
	unsigned short ap;
	unsigned short gp;
}T_ENEMY;

typedef struct Ally{
	unsigned char name[64];
	unsigned short hp;
	unsigned short el;
	unsigned short ap;
	unsigned short gp;
}T_ALLY;

typedef struct Player{
	unsigned char name[64];
	signed short hp;
	unsigned short mhp;
	unsigned short gp;
}T_PLAYER;

T_ALLY get_ally_data(unsigned char type);


#endif /* GAME_H_ */
