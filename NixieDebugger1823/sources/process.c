#include "main.h"
#include "device.h"
#include "process.h"

// ----------
//
// 桁数表示用の変数
//
#define N_DIGIT 2
static unsigned char digit_cnt;

// カウンターとして使用する変数
static unsigned long user_count;

//
// 数字点灯制御信号を送る
//
static void digit_signal()
{
	unsigned char n;

	// 桁の制御
	// 桁に対応する Anode を On にします
	switch (digit_cnt) {
	case 0:
		TLP_A_1 = 1;
		TLP_A_2 = 0;

		n = (user_count % 100) / 10;
		//n = 5;
		break;
	case 1:
		TLP_A_1 = 0;
		TLP_A_2 = 1;

		n = user_count % 10;
		break;
	}

	// 数字の制御
	// 数字に対応するピンを high にします
	PIC_A = (n & 1) >> 0;
	PIC_B = (n & 2) >> 1;
	PIC_C = (n & 4) >> 2;
	PIC_D = (n & 8) >> 3;
}
// ----------


// ----------
//
// ボタン押下検知処理
//
static unsigned long btn_push_prevent_cnt;

static int process_on_button_press()
{
	int ret = 1;

	// スイッチOnに対する処理を実行
	if (BUTTON_0 == 0) { // プルアップされているので Low 判定
		// カウントダウンをスタートさせる
		user_count = 100;

	} else {
		ret = 0;
	}
	return ret;
}

// 割込みごとに処理（3.2768 ms）
void switch_prevent()
{
	// カウンターが０の時は終了
	if (0 == btn_push_prevent_cnt) {
		return;
	}

	// ボタン連続押下抑止カウンターを更新
	btn_push_prevent_cnt-- ;
}

// イベントループ内の最後のステップで処理
void switch_detection()
{
	// カウンターが０でない時は終了
	if (0 < btn_push_prevent_cnt) {
		return;
	}

	// スイッチ押下時の処理を実行
	if (process_on_button_press() != 0) {
		// 押下抑止カウンターを設定(約１秒に設定)
		btn_push_prevent_cnt = 300;
	} else {
		btn_push_prevent_cnt = 0;
	}
}

// 約 10 ミリ秒ごとに処理（3.2768ms × 3回）
void process_on_10m_second()
{
	// 数字を点灯
	digit_signal();

	// 桁を変えます
	digit_cnt++;
	if (digit_cnt == N_DIGIT) {
		digit_cnt = 0;
	}
}

// 約 1.0 秒ごとに処理（3.2768ms × 305回）
void process_on_one_second()
{
	// カウンターをアップ
	if (0 < user_count) {
		user_count--;
	}
}

void process_init()
{
	// ローカル変数の初期化
	btn_push_prevent_cnt = 0;

	// 表示桁
	digit_cnt = 0;

	// カウンターとして表示させたい値
	user_count = 0;
}
