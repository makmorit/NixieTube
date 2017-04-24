#include "common.h"
#include "device.h"

//
// ピンなどの設定を行う
//
void setup_port()
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

//
// TIMER0の設定
//
void setup_timer0()
{
    // TMR0ON: Enables Timer0
    // T08BIT: Timer0 is configured as an 8-bit timer/counter
    // プリスケーラー:16  － １カウント 3.2μ秒(=1/20MHz*4*16)
    // OPTION_REG の先頭ビット:!WPUEN (1:内部プルアップ無)
    OPTION_REG = 0b10000011;

    // 256カウント（0.8192 ms）で割込み発生させる
    TMR0 = 0;

    // TMR0割り込み許可
    TMR0IE = 1;
}
