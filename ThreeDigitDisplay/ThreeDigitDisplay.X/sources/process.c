#include "common.h"
#include "device.h"
#include "process.h"
#include "timer0.h"

// １秒間当たりの割込み回数（0.8192×1221）
#define INT_PER_SEC 1221

// ボタン押下連続検知抑止カウンター（0.8192×611 = 約１秒）
#define BTN_PUSH_PREVENT_CNT 611

// カウントダウン秒数（180秒）
#define COUNT_DOWN_SEC 180

//
// 桁数表示用の変数
//
#define N_DIGIT 3
static unsigned char digit_cnt;
static unsigned char digit_ctrl_cnt;

// カウンターとして使用する変数
static unsigned long user_sec_count;
static unsigned long cnt_int_per_sec;

//
// ボタン押下検知処理
//
static unsigned long btn_push_prevent_cnt;

// ボタン押下時の処理
static int process_on_button_press()
{
    int ret = 1;

    // スイッチOnに対する処理を実行
    if (BUTTON_0 == 0) { // プルアップされているので Low 判定
        //
        // 表示用秒数のカウントダウンをスタートさせる
        //
        user_sec_count = COUNT_DOWN_SEC;
        cnt_int_per_sec = INT_PER_SEC;

    } else {
        ret = 0;
    }
    return ret;
}

// 割込みごとに処理（0.8192 ms）
static void switch_prevent()
{
    // カウンターが０の時は終了
    if (0 == btn_push_prevent_cnt) {
        return;
    }

    // ボタン押下連続検知抑止カウンターを更新
    btn_push_prevent_cnt-- ;
}

// イベントループ内の最後のステップで処理
static void switch_detection()
{
    // カウンターが０でない時は終了
    if (0 < btn_push_prevent_cnt) {
        return;
    }

    // スイッチ押下時の処理を実行
    if (process_on_button_press() != 0) {
        // ボタン押下連続検知抑止カウンターを設定
        btn_push_prevent_cnt = BTN_PUSH_PREVENT_CNT;
    } else {
        btn_push_prevent_cnt = 0;
    }
}

//
// 初期化処理
//
void process_init()
{
    // ローカル変数の初期化
    btn_push_prevent_cnt = 0;

    // 表示桁
    digit_cnt = 0;
    // 表示制御のためのカウンター
    digit_ctrl_cnt = 0;
    // 表示する秒数
    user_sec_count = 0;
}

//
// 数字点灯制御信号を送る
//   mode == 0 の時は消灯（Anode Off）
//   mode == 1 の時は点灯（Anode On）
//
static void digit_signal(unsigned char mode)
{
    unsigned char n;

    //
    // 桁の制御
    //   桁に対応する Anode を On にします
    //   ただし mode が 0 の時は、
    //   表示対象桁であっても点灯しません
    //   (ダイナミック点灯時の残像表示を回避するための措置)
    //
    switch (digit_cnt) {
    case 0:
        TLP_A_1 = 1 & mode;
        TLP_A_2 = 0;
        TLP_A_3 = 0;

        n = user_sec_count / 60;
        break;
    case 1:
        TLP_A_1 = 0;
        TLP_A_2 = 1 & mode;
        TLP_A_3 = 0;

        n = (user_sec_count % 60) / 10;
        break;
    case 2:
        TLP_A_1 = 0;
        TLP_A_2 = 0;
        TLP_A_3 = 1 & mode;

        n = user_sec_count % 10;
        break;
    }

    // 数字の制御
    // 数字に対応するピンを high にします
    PIC_A = (n & 1) >> 0;
    PIC_B = (n & 2) >> 1;
    PIC_C = (n & 4) >> 2;
    PIC_D = (n & 8) >> 3;
}

// 割込みごとに処理（0.8192 ms）
void process_on_interval()
{
    // 秒あたり割込み回数をカウントダウン
    if (0 < cnt_int_per_sec) {
        cnt_int_per_sec--;
    } else {
        // 表示用秒数のカウントダウンを続ける
        if (0 < user_sec_count) {
            user_sec_count--;
            cnt_int_per_sec = INT_PER_SEC;
        }
    }

    //
    // ダイナミック点灯時の残像表示回避対応
    //
    //   digit_ctrl_cntで消灯／点灯を制御
    //    1:   数字を消灯 0.8192ms*1
    //    2-5: 数字を点灯 0.8192ms*4
    //
    digit_ctrl_cnt++;
    if (digit_ctrl_cnt < 2) {
        digit_signal(0);

    } else if (digit_ctrl_cnt < 6) {
        digit_signal(1);

    } else {
        // 表示対象桁を変えます
        digit_cnt++;
        if (digit_cnt == N_DIGIT) {
            digit_cnt = 0;
        }

        // カウンターをリセット
        digit_ctrl_cnt = 0;
    }
}

//
// 主処理
//
void process()
{
    // 割込みごとに処理（0.8192 ms）
    if (tmr0_toggle == 1) {
        tmr0_toggle = 0;
        process_on_interval();

        // ボタン押下連続検知抑止
        switch_prevent();
    }

    // ボタン押下検知処理
    switch_detection();
}
