#ifndef __AUTO_PLAY
#define __AUTO_PLAY
//�X�s�[�J�Ǝ������t�Ŏg���ϐ��𕪂���
typedef struct AUTOPLAYER{
	unsigned char wave_type;			//�o�͔g�`�i�X�s�[�J�[�P�́j
	T_SCORE *pscore;		//�y���z��ւ̃|�C���^
	T_NOTE *pnote_value;	//���̒����z��ւ̃|�C���^
	unsigned short score_count;			//�X�R�A�J�E���g
	unsigned long elapsed_time;			//�o�ߎ��ԊĎ��G���A
	unsigned short note_size;			//�����̐�
	unsigned char set_flg;				//�X�R�A�J�E���g���i�񂾂̂ŃZ�b�g���K�v�ł��邱�Ƃ�m�点��
	unsigned char end_flg;
}AUTOPLAYER;

void autoplay_start(unsigned short title,unsigned char wave_type);
AUTOPLAYER *get_autoplayer(unsigned char player_num);
AUTOPLAYER get_interrupt_data(unsigned char player_num);
#endif
