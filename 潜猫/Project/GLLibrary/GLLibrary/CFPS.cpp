#include "CFPS.h"

//�P�b�Ԃ̃J�E���g��
LARGE_INTEGER CFPS::freq;
//�O�t���[���̃J�E���g��
LARGE_INTEGER CFPS::time_buf;
int CFPS::fps;
int CFPS::m_Setfps = 60;
float CFPS::deltaTime;

void CFPS::Init(){
	//���Ԍv���p
	//�V�X�e���̂P�b�Ԃ̃J�E���g���i���g���j���擾
	QueryPerformanceFrequency(&freq);
	//���݂̃V�X�e���̃J�E���g�����擾
	QueryPerformanceCounter(&time_buf);
}


LONGLONG CFPS::GetTimeCnt() {
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	return time.QuadPart/(freq.QuadPart/ m_Setfps);
}
void CFPS::Wait() {
	//freq.QuadPart�@	1�b������̃J�E���g���i�Œ�j
	//time.QuadPart�@	���̃J�E���g��
	//time_buf.QuadPart�@�O��̃J�E���g��

	//��L��3�̏�񂩂�A�����őO��Ăяo�������1/60�b�o�܂őҋ@����

	LARGE_INTEGER time;
	//�O�t���[���ƍ��̃J�E���g�̍���1/60�b�����Ȃ烋�[�v
	do{
		//���݂̃V�X�e���̃J�E���g�����擾
		QueryPerformanceCounter(&time);

		//�@���̃J�E���g�@�O��̃J�E���g�@ < 1�b������̃J�E���g����60�Ŋ���(1/60�b������̃J�E���g��)
	}while(m_Setfps > 0 && time.QuadPart-time_buf.QuadPart < freq.QuadPart / m_Setfps);

	fps = (int)(freq.QuadPart/(time.QuadPart-time_buf.QuadPart));
	deltaTime = ((time.QuadPart - time_buf.QuadPart) / (float)freq.QuadPart);
	//deltaTime = 1 / 60.0f;
	time_buf = time;

}

void CFPS::SetFPS(int fps)
{
	m_Setfps = fps;
}


void Time::Start() {
	QueryPerformanceCounter(&time_buf);
	//�V�X�e���̂P�b�Ԃ̃J�E���g���i���g���j���擾
	QueryPerformanceFrequency(&freq);
}

float Time::End() {
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	return (time.QuadPart - time_buf.QuadPart) / (float)freq.QuadPart;
}

//�P�b�Ԃ̃J�E���g��
LARGE_INTEGER Time::freq;
//�O�t���[���̃J�E���g��
LARGE_INTEGER Time::time_buf;
