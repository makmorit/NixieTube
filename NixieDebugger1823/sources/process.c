#include "main.h"
#include "device.h"
#include "process.h"

// ----------
//
// �����\���p�̕ϐ�
//
#define N_DIGIT 2
static unsigned char digit_cnt;
static unsigned char digit_ctrl_cnt;

// �J�E���^�[�Ƃ��Ďg�p����ϐ�
static unsigned long user_count;

//
// �����_������M���𑗂�
//   mode == 0 �̎��͏����iAnode Off�j
//   mode == 1 �̎��͓_���iAnode On�j
//
static void digit_signal(unsigned char mode)
{
	unsigned char n;

	// ���̐���
	// ���ɑΉ����� Anode �� On �ɂ��܂�
	switch (digit_cnt) {
	case 0:
		TLP_A_1 = 1 & mode;
		TLP_A_2 = 0;

		n = (user_count % 100) / 10;
		break;
	case 1:
		TLP_A_1 = 0;
		TLP_A_2 = 1 & mode;

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

// �����݂��Ƃɏ����i3.2768 ms�j
void process_on_3m_second()
{
	digit_ctrl_cnt++;
	if (digit_ctrl_cnt < 2) {
		// ����������
		digit_signal(0);

	} else if (digit_ctrl_cnt < 4) {
		// ������_��
		digit_signal(1);

	} else {
		// ����ς��܂�
		digit_cnt++;
		if (digit_cnt == N_DIGIT) {
			digit_cnt = 0;
		}

		// �J�E���^�[�����Z�b�g
		digit_ctrl_cnt = 0;
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

	// �\������̂��߂̃J�E���^�[
	digit_ctrl_cnt = 0;

	// �J�E���^�[�Ƃ��ĕ\�����������l
	user_count = 0;
}
