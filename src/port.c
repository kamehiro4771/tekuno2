

/********************************************************************/
/*�w�b�_�[�t�@�C��													*/
/********************************************************************/
#include "main.h"

/********************************************************************/
/*i/o�|�[�g�̏�����													*/
/*void io_port_initialize(void)										*/
/********************************************************************/
void io_port_initialize(void)
{
	//LED1~8�̃|�[�g�̐ݒ�
	PORTD.DDR.BYTE				= 0xff;
	PORTE.DDR.BYTE				= 0xff;
	PORTB.DDR.BYTE				= 0xff;
	//7SEGLEDa~DP�̃|�[�g�̐ݒ�
	PORT4.DDR.BYTE				= 0xff;
	//7SEGLED�̃R�����[�q�̐ݒ�
	PORTA.DDR.BIT.B1			= 1;
	PORTA.DDR.BIT.B2			= 1;
	PORTA.DDR.BIT.B3			= 1;
	//�X�C�b�`�̐ݒ�
	PORTC.DDR.BIT.B0			= 0;
	PORTC.DDR.BIT.B1			= 0;
	PORTC.DDR.BIT.B2			= 0;
	PORTC.DDR.BIT.B3			= 0;
	PORTC.DDR.BIT.B4			= 0;
	PORTC.DDR.BIT.B5			= 0;
	PORTC.DDR.BIT.B6			= 0;
	PORTC.DDR.BIT.B7			= 0;
	PORT5.DDR.BIT.B0			= 0;
	PORT5.DDR.BIT.B1			= 0;
	PORT5.DDR.BIT.B2			= 0;
	PORT5.DDR.BIT.B3			= 0;
	PORT5.DDR.BIT.B4			= 0;
}