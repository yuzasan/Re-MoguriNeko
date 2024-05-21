#include "RotateBox.h"

RotateBox::RotateBox(const CVector3D& pos, const CVector3D& rot, const CVector3D& size,
	const float rotate, const int xyz, const float intervalTime, const int number, bool isStart)
	//	const float rotate, const int xyz, const float moveTime, const float intervalTime, const int number, bool isStart)
	: Task(TaskType::eRotateBox)
	, m_rotate(rotate)
	, m_xyz(xyz)
	, now_rot(0.0f)
	, m_intervalTime(intervalTime)
	, m_number(number)
	, m_state(State::eMove)
	, m_isEnable(isStart)
	, m_elapsedTime(0.0f)
{
	m_pos = pos;
	m_rot = rot;
	m_boxSize = size;
	m_obb = COBB(
		m_pos,
		m_rot,
		m_boxSize
	);
	m_moveTime = 240.0f;
	mp_player = TaskManeger::FindObject(TaskType::ePlayer);
}

RotateBox::~RotateBox() {

}

void RotateBox::Start()
{
	m_isEnable = true;
}

void RotateBox::Stop()
{
	m_isEnable = false;
}

//ˆÚ“®ó‘Ô‚ÌXVˆ—
void RotateBox::UpdateMove()
{
	m_moveTime--;
	switch (m_xyz)
	{
	case 0://XŽ²‰ñ“]
		now_rot += m_rotate * CFPS::GetDeltaTime();
		if (now_rot >= M_PI * 2.0f) now_rot -= M_PI * 2.0f;
		m_obb = COBB(
			m_pos,
			CVector3D(m_rot.x + now_rot, 0.0f, 0.0f),
			m_boxSize
		);
		break;
	case 1://YŽ²‰ñ“]
		now_rot += m_rotate * CFPS::GetDeltaTime();
		if (now_rot >= M_PI * 2.0f) now_rot -= M_PI * 2.0f;
		m_obb = COBB(
			m_pos,
			CVector3D(0.0f, m_rot.y + now_rot, 0.0f),
			m_boxSize
		);
		break;
	case 2://ZŽ²‰ñ“]
		now_rot += m_rotate * CFPS::GetDeltaTime();
		if (now_rot >= M_PI * 2.0f) now_rot -= M_PI * 2.0f;
		m_obb = COBB(
			m_pos,
			CVector3D(0.0f, 0.0f, m_rot.z + now_rot),
			m_boxSize
		);
		break;
	}
	if (m_moveTime <= 0.0f) {
		//‘Ò‹@ó‘Ô‚ÖˆÚs
		m_state = State::eInterval;
	}
	
}

//‘Ò‹@ó‘Ô‚ÌXVˆ—
void RotateBox::UpdateInterval()
{
	//‘Ò‹@ŽžŠÔŒo‰ß‘Ò‚¿
	if (m_elapsedTime < m_intervalTime)
	{
		m_elapsedTime += CFPS::GetDeltaTime();
	}
	//‘Ò‹@I—¹
	else
	{
		m_moveTime = 240.0;
		m_elapsedTime -= m_intervalTime;
		//ˆÚ“®ó‘Ô‚ÖˆÚs
		m_state = State::eMove;
	}
}

//XVˆ—
void RotateBox::Update()
{
	switch (m_number)
	{
	case 0:
		switch (m_xyz)
		{
		case 0:
			//XŽ²‰ñ“]
			now_rot += m_rotate * CFPS::GetDeltaTime();
			if (now_rot >= M_PI * 2.0f) now_rot -= M_PI * 2.0f;
			m_obb = COBB(
				m_pos,
				CVector3D(m_rot.x + now_rot, 0.0f, 0.0f),
				m_boxSize
			);
			break;
		case 1:
			//YŽ²‰ñ“]
			now_rot += m_rotate * CFPS::GetDeltaTime();
			if (now_rot >= M_PI * 2.0f) now_rot -= M_PI * 2.0f;
			m_obb = COBB(
				m_pos,
				CVector3D(0.0f, m_rot.y + now_rot, 0.0f),
				m_boxSize
			);
			break;
		case 2:
			//ZŽ²‰ñ“]
			now_rot += m_rotate * CFPS::GetDeltaTime();
			if (now_rot >= M_PI * 2.0f) now_rot -= M_PI * 2.0f;
			m_obb = COBB(
				m_pos,
				CVector3D(0.0f, 0.0f, m_rot.z + now_rot),
				m_boxSize
			);
			break;
		}
		break;
	case 1:
		//ˆÚ“®‚ª—LŒø‚Å‚È‚¯‚ê‚Îˆ—‚µ‚È‚¢
		if (!m_isEnable) return;

		//ó‘Ô‚É‡‚í‚¹‚ÄXVˆ—‚ðØ‚è‘Ö‚¦
		switch (m_state)
		{
			//ˆÚ“®ó‘Ô
		case State::eMove:
			UpdateMove();
			break;
			//‘Ò‹@ó‘Ô
		case State::eInterval:
			UpdateInterval();
			break;
		}
		break;
	}
}

//•`‰æˆ—
void RotateBox::Render()
{
	if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	switch (m_xyz)
	{
	case 0://XŽ²‰ñ“]
		Utility::DrawOBB(m_obb, CVector4D(1, 0, 0, 1));
		break;
	case 1://YŽ²‰ñ“]
		Utility::DrawOBB(m_obb, CVector4D(0, 1, 0, 1));
		break;
	case 2://ZŽ²‰ñ“]
		Utility::DrawOBB(m_obb, CVector4D(0, 0, 1, 1));
		break;
	}
}

void RotateBox::NoEnemyRender()
{
	switch (m_xyz)
	{
	case 0://XŽ²‰ñ“]
		Utility::DrawOBB(m_obb, CVector4D(1, 0, 0, 1));
		break;
	case 1://YŽ²‰ñ“]
		Utility::DrawOBB(m_obb, CVector4D(0, 1, 0, 1));
		break;
	case 2://ZŽ²‰ñ“]
		Utility::DrawOBB(m_obb, CVector4D(0, 0, 1, 1));
		break;
	}
}

void RotateBox::Collision(Task* b) {
	switch (b->GetType()) {
	case TaskType::ePlayer:
	{
		//¡OBB‚ÆƒJƒvƒZƒ‹
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(m_obb, mp_player->m_lineS, mp_player->m_lineE, mp_player->m_rad, &axis, &dist)) {
			if (axis.y > 0.5f) {
				//–Ê‚ªãŒü‚«¨’n–Ê‚ª“–‚½‚Á‚½
				//d—Í—Ž‰º‘¬“x‚ð0‚É–ß‚·
				if (mp_player->m_vec.y < 0) {
					mp_player->m_vec.y = 0;
					mp_player->m_isGround = true;
				}
			}
			//‰Ÿ‚µ–ß‚µ
			float s = mp_player->m_rad - dist;
			mp_player->m_pos += axis * s;
		}
	}
	break;
	}
}