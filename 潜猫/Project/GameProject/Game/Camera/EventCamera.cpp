#include "EventCamera.h"
//static ‚ÍÅ‰‚Ì#include‚Ì‚Æ‚±‚ë‚É‘‚­
bool EventCamera::eventrendring = false;

EventCamera::EventCamera() :Task(TaskType::eEventCamera)
	,eventcam_pos(CVector3D::zero)
	,eventcam_at(CVector3D::zero)
{
	
}

EventCamera::~EventCamera(){
	eventrendring = false;
}

void EventCamera::Update(){
	eventcam_at.y += 0.03f;
	eventcam_at.x += 0.02f;
	//KƒL[
	if (PUSH(CInput::eButton12)) {
		Kill();
	}
}

void EventCamera::Render(){
	{
		eventrendring = true;
		
		eventcam_pos = eventcam_at + CMatrix::MRotation(m_rot).GetFront();
		//“§Ž‹“Š‰es—ñ‚ÅÝ’è
		CCamera::GetCurrent()->LookAt(eventcam_pos, eventcam_at, CVector3D(0, 1, 0));
	}
}
