#ifndef __DEVICE_H
#define __DEVICE_H

//
// タクトスイッチ
//
#define BUTTON_0 RA3

//
// 点灯制御用
//   SN74141制御用
//
#define PIC_A RC2
#define PIC_B RC3
#define PIC_C RC4
#define PIC_D RC5
//
//   TLP627制御用
//
#define TLP_A_1 RA0
#define TLP_A_2 RA1
#define TLP_A_3 RA2

// 関数
void port_init();

#endif // __DEVICE_H
