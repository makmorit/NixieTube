#include "main.h"
#include "device.h"

__CONFIG(CLKOUTEN_OFF & FOSC_HS & FCMEN_OFF & IESO_OFF & BOREN_ON & PWRTE_ON & WDTE_OFF & MCLRE_OFF & CP_OFF & CPD_OFF);
__CONFIG(PLLEN_OFF & STVREN_ON & WRT_OFF & LVP_OFF);


//
// �^�C�}�[�Ŏg�p����ϐ�
//
static unsigned long total_tmr0_cnt_100m;
static unsigned long total_tmr0_cnt_1s;
static unsigned char tmr0_toggle;

//
// ����������
//
static void initialize()
{
	// �s���Ȃǂ̏����ݒ���s��
	port_init();

	//
	// �^�C�}�[�O�̐ݒ���s��
	//
	// TMR0ON: Enables Timer0
	// T08BIT: Timer0 is configured as an 8-bit timer/counter
	// �v���X�P�[���[:64  �| �P�J�E���g12.8�ʕb(=1/20MHz*4*64)
	// OPTION_REG �̐擪�r�b�g:!WPUEN (1:�����v���A�b�v��)
	OPTION_REG = 0b10000101;
	// 256�J�E���g�i3.2768 ms�j�Ŋ����ݔ���������
	TMR0 = 0;
	// TMR0���荞�݋���
	TMR0IE = 1;

	// �S�����ݏ�����������
	PEIE = 1;
	GIE  = 1;

	// TIMER2 on prescale=1
	// TIMER2�X�^�[�g�ion�r�b�g���P�j
	T2CON = 0b100;
}

//
// �����ݏ���
//
static void interrupt intr(void)
{
	// �^�C�}�[�O�����݁i1�~���b���Ɓj�̏ꍇ
	if (TMR0IF == 1) {
		// �����݃J�E���^�[
		total_tmr0_cnt_100m++;
		total_tmr0_cnt_1s++;
		tmr0_toggle = 1;
		// 256�J�E���g�i3.2768 ms�j�Ŋ����ݔ���������
		TMR0 = 0;
		// TMR0���荞�݃N���A
		TMR0IF = 0;
	}
}

//
// �C�x���g����
//
static void do_events()
{
	static unsigned char n;

	//
	// �����݂��Ƃɏ����i3.2768 ms�j
	//
	if (tmr0_toggle == 1) {
		tmr0_toggle = 0;

		// TODO
	}

	//
	// �� 0.1 �b���Ƃɏ����i3.2768ms �~ 30��j
	//
	if (total_tmr0_cnt_100m > 30) {
		// �J�E���^�[��������
		total_tmr0_cnt_100m = 0;

		if (n == 0) {
			n = 10;
		} else {
			n--;
		}

		// TODO
		PIC_A = (n & 1) >> 0;
		PIC_B = (n & 2) >> 1;
		PIC_C = (n & 4) >> 2;
		PIC_D = (n & 8) >> 3;
	}

	//
	// �� 1.0 �b���Ƃɏ����i3.2768ms �~ 305��j
	//
	if (total_tmr0_cnt_1s > 305) {
		// �J�E���^�[��������
		total_tmr0_cnt_1s = 0;

		// TODO
		TLP_A_2 = ~TLP_A_2;
	}
}

//
// ���C�����[�`��
//
void main() 
{
	// �s����@�\���̏���������
	initialize();

	// do_events �����񐔃J�E���^�[
	//   �������_�ł̊����݃J�E���^�[
	total_tmr0_cnt_100m = 0;
	total_tmr0_cnt_1s = 0;

	// Anode1��On�ɂ��܂�
	TLP_A_1 = 1;

	while (1) {
		// �C�x���g����
		do_events();
	}
}
