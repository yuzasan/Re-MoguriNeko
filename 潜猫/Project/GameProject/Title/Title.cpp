#include "Title.h"
#include "../Game/Game.h"
#include "../Effect/Effect.h"
//#include "../Base/Base.h"

void Title::StepFirst()
{
	/*new Effect("FusumaR", CVector3D(-0.4f, -2, 0), CVector3D(0.4f, -0.3f, 0), CVector3D(0, 0, 0), 1.0f, 0.1f, 1.0f, 0.0f, 0, true, 120);
	new Effect("FusumaL", CVector3D(0.4f, -2, 0), CVector3D(-0.4f, -0.3f, 0), CVector3D(0, 0, 0), 1.0f, 0.1f, 1.0f, 0.0f, 0, true, 120);
	new Effect("FusumaR", CVector3D(-0.1f, -2, 0), CVector3D(0.1f, -0.3f, 0), CVector3D(0, 0, 0), 1.0f, 0.1f, 1.0f, 0.0f, 0, true, 120);
	new Effect("FusumaL", CVector3D(0.1f, -2, 0), CVector3D(-0.1f, -0.3f, 0), CVector3D(0, 0, 0), 1.0f, 0.1f, 1.0f, 0.0f, 0, true, 120);
	*/

	new Effect("FusumaR", CVector3D(-0.2f, -2, 0), CVector3D(0.2f, -0.2f, 0), CVector3D(0, 0, 0), CVector3D(0, 0, 0), 1.0f, 0.1f, 1.0f, 0.0f, 0, true, true, 120);
	new Effect("FusumaL", CVector3D(0.2f, -2, 0), CVector3D(-0.2f, -0.2f, 0), CVector3D(0, 0, 0), CVector3D(0, 0, 0), 1.0f, 0.1f, 1.0f, 0.0f, 0, true, true, 120);
	m_step = Step::Mid;
}

void Title::StepSecond()
{
	if (m_cnt < 9) {
		new Effect("FusumaR", CVector3D(0, -2, -1), CVector3D(0, 0, 0), CVector3D(0, 0, 0), CVector3D(0, 0, 0), 1.0f, 0.0f, 1.0f, 0.0f, 0, true, true, 30);
		new Effect("FusumaL", CVector3D(0, -2, -1), CVector3D(0, 0, 0), CVector3D(0, 0, 0), CVector3D(0, 0, 0), 1.0f, 0.0f, 1.0f, 0.0f, 0, true, true, 30);
	}
	m_time += CFPS::GetDeltaTime();
	if (m_time >= 0.5f) {
		m_step = Step::End;
		m_time = 0;
	}
}

void Title::StepLast()
{
	m_cnt++;
	if (m_cnt <= 10) {
		m_step = Step::Start;
	}
	else {
		m_isOn = true;
	}
}

void Title::FuwaFuwa() {
	//�������Ă���
	fuwafuwa_r += CFPS::GetDeltaTime();
}

Title::Title():Task(TaskType::eScene), m_title_text("C:\\Windows\\Fonts\\msgothic.ttc", 64)
	,m_step(Step::End)
	,m_cnt(0)
	,m_time(0)
	,m_isOn(false)
{
	//m_img = COPY_RESOURCE("Title", CImage);
	m_img = COPY_RESOURCE("Start", CImage);
	//new Effect("Fukidasi", m_pos + CVector3D(0, 2.5, -1), CVector3D(0, 0, 0), CVector3D(0, 0, 0), 1.0f, 1.0f, 1.0f, 0.0f, 0, true, 60);
	//new Effect("FusumaR", CVector3D(-0.3f, -2, 0), CVector3D( 0.3f, -0.3f, 0), CVector3D(0, 0, 0), 1.0f, 0.1f, 1.0f, 0.0f, 0, true, 120);
	//new Effect("FusumaL", CVector3D( 0.3f, -2, 0), CVector3D(-0.3f, -0.3f, 0), CVector3D(0, 0, 0), 1.0f, 0.1f, 1.0f, 0.0f, 0, true, 120);
}

Title::~Title(){
	//�S�ẴI�u�W�F�N�g��j��
	//Base::KillALL();
	//�Q�[���V�[����
	//Base::Add(new Game());
	
	//�S�ẴI�u�W�F�N�g��j��
	//Task::Kill();
	TaskManeger::KillALL();
	//�Q�[���V�[����
	new Game();
}

void Title::Update() 
{	
	switch (m_step) {
	case Step::Start:
		StepFirst();
		break;
	case Step::Mid:
		StepSecond();
		break;
	case Step::End:
		StepLast();
		break;
	}

	//���[�h�I���`�F�b�N
	if (CLoadThread::GetInstance()->CheckEnd()) {
		//�������Ă���
		//���N���b�N
		if (PUSH(CInput::eMouseL)) {
			Kill();
		}
	}
}

void Title::Draw()
{
	if (m_isOn)m_img.Draw();
	
	if (CLoadThread::GetInstance()->CheckEnd()) {
		FuwaFuwa();
		//�����\��
		//m_title_text.Draw(800, 800 - abs(sin(fuwafuwa_r)) * 64, 0, 0, 0, "���N���b�N");
		m_title_text.Draw(800, 900 - abs(sin(fuwafuwa_r)) * 64, 1, 1, 1, "���N���b�N");
	}
}
