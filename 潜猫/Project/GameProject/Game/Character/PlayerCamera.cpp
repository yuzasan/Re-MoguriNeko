#include "PlayerCamera.h"
#include "Player.h"
#include "../Camera/FieldCamera.h"
#include "../Camera/EventCamera.h"
#include "../Stage/Stage.h"

PlayerCamera::PlayerCamera() :Task(TaskType::eCamera) {
	cam_at = CVector3D(0, 0, 0);
	cam_pos = CVector3D(0, 0, 0);
	mp_player = TaskManeger::FindObject(TaskType::ePlayer);
}

PlayerCamera::~PlayerCamera()
{
	CCamera::GetCamera()->LookAt(CVector3D(0, 0, 10),
		CVector3D(0, 0, 0),
		CVector3D(0.0, 1.0, 0.0));
}

void PlayerCamera::Update() {

}

void PlayerCamera::Render() {
	if (EventCamera::eventrendring)return;
	if (FieldCamera::rendring)return;
	if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	//’Ž‹“_‚Ü‚Å‚Ì‹——£
	//static float cam_dist = 3.0f;
	static float cam_dist = 1.5f;
	//‰ñ“]‘¬“xŒW”
	float cam_speed = 0.002f;
	//ƒ}ƒEƒX‚ÌˆÚ“®—Ê
	CVector2D mouse_vec = CInput::GetMouseVec();
	//ƒ}ƒEƒXˆÚ“®Y¨XŽ²‰ñ“](ã‰º)
	//ƒ}ƒEƒXˆÚ“®X¨YŽ²‰ñ“](¶‰E)
	m_rot += CVector3D(mouse_vec.y, -mouse_vec.x, 0) * cam_speed;
	//ã‰ºŠp§ŒÀ
	m_rot.x = min(DtoR(45), max(DtoR(-45), m_rot.x));
	//-180`180‚É³‹K‰»
	m_rot.y = Utility::NormalizeAngle(m_rot.y);

	//‹——£§ŒÀ
	//cam_dist = min(10.0f, max(2.0f, cam_dist - CInput::GetMouseWheel()));
	//ˆÊ’u‚Æ’Ž‹“_‚ðÝ’è
	//cam_at = mp_player->m_pos + CVector3D(0, 1.5, 0);
	//cam_at = mp_player->m_pos + CVector3D(0, 0.75f, 0);
	cam_at = mp_player->m_pos + CVector3D(0, 0.65f, 0);
	//cam_pos = cam_at + CMatrix::MRotation(m_rot).GetFront() * -cam_dist;
	cam_pos = cam_at + CMatrix::MRotation(m_rot).GetFront() * -cam_dist;

	CCamera::GetCurrent()->LookAt(cam_pos, cam_at, CVector3D(0, 1, 0));

}

void PlayerCamera::Collision(Task* b) {
	switch (b->GetType()) {
	case TaskType::eField:
	{
		//ƒJƒƒ‰–„‚ßž‚Ý‰ñ”ð
		CVector3D cross, normal;
		if (b->GetModel()->CollisionRay(&cross, &normal, cam_at, cam_pos)) {
			cam_pos = cross;
		}
		
	}
	break;
	}
}