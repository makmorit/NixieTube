#include "main.h"
#include "device.h"

//
// �s���Ȃǂ̐ݒ���s��
//
void port_init()
{
	// Port A
	//   �A�i���O�͎g�p���Ȃ��i���ׂăf�W�^��I/O�Ɋ����Ă�j
	//   RA3=���́i10k �Ńv���A�b�v�j
	//          76543210
	ANSELA  = 0b00000000;
	TRISA   = 0b00001000;
	PORTA   = 0b00000000;

	// Port C
	//   �A�i���O�͎g�p���Ȃ��i���ׂăf�W�^��I/O�Ɋ����Ă�j
	//          76543210
	ANSELC  = 0b00000000;
	TRISC   = 0b00000000;
	PORTC   = 0b00000000;
}
