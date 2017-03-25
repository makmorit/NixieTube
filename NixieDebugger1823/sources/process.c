#include "main.h"
#include "device.h"
#include "process.h"

// ----------
//
// �����\���p�̕ϐ�
//
#define N_DIGIT 2
static unsigned char digit_cnt;

// �J�E���^�[�Ƃ��Ďg�p����ϐ�
static unsigned long user_count;

//
// �����_������M���𑗂�
//
static void digit_signal()
{
	unsigned char n;

	// ���̐���
	// ���ɑΉ����� Anode �� On �ɂ��܂�
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

	// �����̐���
	// �����ɑΉ�����s���� high �ɂ��܂�
	PIC_A = (n & 1) >> 0;
	PIC_B = (n & 2) >> 1;
	PIC_C = (n & 4) >> 2;
	PIC_D = (n & 8) >> 3;
}
// ----------


// ----------
//
// �{�^���������m����
//
static unsigned long btn_push_prevent_cnt;

static int process_on_button_press()
{
	int ret = 1;

	// �X�C�b�`On�ɑ΂��鏈�������s
	if (BUTTON_0 == 0) { // �v���A�b�v����Ă���̂� Low ����
		// �J�E���g�_�E�����X�^�[�g������
		user_count = 100;

	} else {
		ret = 0;
	}
	return ret;
}

// �����݂��Ƃɏ����i3.2768 ms�j
void switch_prevent()
{
	// �J�E���^�[���O�̎��͏I��
	if (0 == btn_push_prevent_cnt) {
		return;
	}

	// �{�^���A�������}�~�J�E���^�[���X�V
	btn_push_prevent_cnt-- ;
}

// �C�x���g���[�v���̍Ō�̃X�e�b�v�ŏ���
void switch_detection()
{
	// �J�E���^�[���O�łȂ����͏I��
	if (0 < btn_push_prevent_cnt) {
		return;
	}

	// �X�C�b�`�������̏��������s
	if (process_on_button_press() != 0) {
		// �����}�~�J�E���^�[��ݒ�(��P�b�ɐݒ�)
		btn_push_prevent_cnt = 300;
	} else {
		btn_push_prevent_cnt = 0;
	}
}

// �� 10 �~���b���Ƃɏ����i3.2768ms �~ 3��j
void process_on_10m_second()
{
	// ������_��
	digit_signal();

	// ����ς��܂�
	digit_cnt++;
	if (digit_cnt == N_DIGIT) {
		digit_cnt = 0;
	}
}

// �� 1.0 �b���Ƃɏ����i3.2768ms �~ 305��j
void process_on_one_second()
{
	// �J�E���^�[���A�b�v
	if (0 < user_count) {
		user_count--;
	}
}

void process_init()
{
	// ���[�J���ϐ��̏�����
	btn_push_prevent_cnt = 0;

	// �\����
	digit_cnt = 0;

	// �J�E���^�[�Ƃ��ĕ\�����������l
	user_count = 0;
}
