#include "common.h"
#include "device.h"
#include "process.h"
#include "timer0.h"

// CONFIG1
#pragma config FOSC = HS // 20MHz
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config MCLRE = OFF
#pragma config CP = OFF
#pragma config CPD = OFF
#pragma config BOREN = ON
#pragma config CLKOUTEN = OFF
#pragma config IESO = OFF
#pragma config FCMEN = OFF

// CONFIG2
#pragma config WRT = OFF
#pragma config PLLEN = OFF
#pragma config STVREN = ON
#pragma config BORV = HI
#pragma config LVP = OFF

//
// 各種設定
//   ピン設定, TIME 0, UART, CCP
//
static void setup()
{
    setup_port();
    setup_timer0();

    // 全割込み処理を許可
    PEIE = 1;
    GIE  = 1;
}

//
// 割込み処理
//   UART, TIMER 0
//
static void interrupt intr(void)
{
    timer0_intr();
}

//
// メインルーチン
//
void main() 
{
    // ピンや機能等の設定処理
    setup();

    // 初期化処理
    //   TIMER 0
    timer0_init();
    
    // 主処理ループで使用される変数の初期化
    process_init();

    // 主処理ループ
    while (1) {
        process();
    }
}
