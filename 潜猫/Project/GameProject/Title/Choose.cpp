#include "Choose.h"
#include "../Title/Title.h"
#include "../Game/Game.h"
#include "../Effect/Effect.h"
//#include "../Base/Base.h"

void Choose::StepFirst()
{
	new Effect("FusumaR", CVector3D(-0.2f, -2, 0), CVector3D(0.2f, -0.2f, 0), CVector3D(0, 0, 0), CVector3D(0, 0, 0), 1.0f, 0.1f, 1.0f, 0.0f, 0, true, true, 120);
	new Effect("FusumaL", CVector3D(0.2f, -2, 0), CVector3D(-0.2f, -0.2f, 0), CVector3D(0, 0, 0), CVector3D(0, 0, 0), 1.0f, 0.1f, 1.0f, 0.0f, 0, true, true, 120);
	m_step = Step::Mid;
}

void Choose::StepSecond()
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

void Choose::StepLast()
{
	m_cnt++;
	if (m_cnt <= 10) {
		m_step = Step::Start;
	}
	else {
		m_isOn = true;
	}
}

void Choose::FuwaFuwa() {
	//完了している
	fuwafuwa_r += CFPS::GetDeltaTime();
}

Choose::Choose() :Task(TaskType::eScene), m_title_text("C:\\Windows\\Fonts\\msgothic.ttc", 64)
, m_step(Step::End)
, m_cnt(0)
, m_time(0)
, m_isOn(false)
{
	m_img = COPY_RESOURCE("Title", CImage);
}

Choose::~Choose() {
	TaskManeger::KillALL();
	//ゲームシーンへ
	new Game();
}

void Choose::Update()
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

	//左クリック
	if (PUSH(CInput::eMouseL)) {
		Kill();
	}
}

void Choose::Draw()
{
	if (m_isOn)m_img.Draw();

	//if (CLoadThread::GetInstance()->CheckEnd()) {
		FuwaFuwa();
		//文字表示
		//m_title_text.Draw(800, 800 - abs(sin(fuwafuwa_r)) * 64, 0, 0, 0, "左クリック");
		m_title_text.Draw(800, 900 - abs(sin(fuwafuwa_r)) * 64, 0, 0, 0, "左クリック");
	//}
}
