#include "main.h"
#include "device.h"
#include "process.h"

__CONFIG(CLKOUTEN_OFF & FOSC_HS & FCMEN_OFF & IESO_OFF & BOREN_ON & PWRTE_ON & WDTE_OFF & MCLRE_OFF & CP_OFF & CPD_OFF);
__CONFIG(PLLEN_OFF & STVREN_ON & WRT_OFF & LVP_OFF);


//
// タイマーで使用する変数
//
static unsigned long total_tmr0_cnt_10m;
static unsigned long total_tmr0_cnt_100m;
static unsigned long total_tmr0_cnt_1s;
static unsigned char tmr0_toggle;

//
// 初期化処理
//
static void initialize()
{
	// ピンなどの初期設定を行う
	port_init();

	//
	// タイマー０の設定を行う
	//
	// TMR0ON: Enables Timer0
	// T08BIT: Timer0 is configured as an 8-bit timer/counter
	// プリスケーラー:64  － １カウント12.8μ秒(=1/20MHz*4*64)
	// OPTION_REG の先頭ビット:!WPUEN (1:内部プルアップ無)
	OPTION_REG = 0b10000101;
	// 256カウント（3.2768 ms）で割込み発生させる
	TMR0 = 0;
	// TMR0割り込み許可
	TMR0IE = 1;

	// 全割込み処理を許可する
	PEIE = 1;
	GIE  = 1;

	// TIMER2 on prescale=1
	// TIMER2スタート（onビットを１）
	T2CON = 0b100;
}

//
// 割込み処理
//
static void interrupt intr(void)
{
	// タイマー０割込み（1ミリ秒ごと）の場合
	if (TMR0IF == 1) {
		// 割込みカウンター
		total_tmr0_cnt_10m++;
		total_tmr0_cnt_100m++;
		total_tmr0_cnt_1s++;
		tmr0_toggle = 1;
		// 256カウント（3.2768 ms）で割込み発生させる
		TMR0 = 0;
		// TMR0割り込みクリア
		TMR0IF = 0;
	}
}

//
// イベント処理
//
static void do_events()
{
	//
	// 割込みごとに処理（3.2768 ms）
	//
	if (tmr0_toggle == 1) {
		tmr0_toggle = 0;

		// ボタン連続押下抑止処理
		switch_prevent();
	}

	//
	// 約 10 ミリ秒ごとに処理（3.2768ms × 3回）
	//
	if (total_tmr0_cnt_10m > 3) {
		// カウンターを初期化
		total_tmr0_cnt_10m = 0;
		process_on_10m_second();
	}

	//
	// 約 1.0 秒ごとに処理（3.2768ms × 305回）
	//
	if (total_tmr0_cnt_1s > 305) {
		// カウンターを初期化
		total_tmr0_cnt_1s = 0;
		process_on_one_second();
	}

	// ボタン押下検知処理
	switch_detection();
}

//
// メインルーチン
//
void main() 
{
	// ピンや機能等の初期化処理
	initialize();

	// do_events 処理回数カウンター
	//   処理時点での割込みカウンター
	total_tmr0_cnt_10m = 0;
	total_tmr0_cnt_100m = 0;
	total_tmr0_cnt_1s = 0;

	process_init();

	while (1) {
		// イベント処理
		do_events();
	}
}
