#include "main.h"
#include "device.h"

//
// ピンなどの設定を行う
//
void port_init()
{
	// Port A
	//   アナログは使用しない（すべてデジタルI/Oに割当てる）
	//   RA3=入力（10k でプルアップ）
	//          76543210
	ANSELA  = 0b00000000;
	TRISA   = 0b00001000;
	PORTA   = 0b00000000;

	// Port C
	//   アナログは使用しない（すべてデジタルI/Oに割当てる）
	//          76543210
	ANSELC  = 0b00000000;
	TRISC   = 0b00000000;
	PORTC   = 0b00000000;
}
