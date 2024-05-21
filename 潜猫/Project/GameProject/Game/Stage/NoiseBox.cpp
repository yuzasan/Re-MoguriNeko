#include "NoiseBox.h"
#include "../Character/Enemy.h"

NoiseBox::NoiseBox(const CVector3D& pos, const CVector3D& rot, const CVector3D& size)
	: Task(TaskType::eNoiseBox)
	,mp_enemy(nullptr)
{
	m_pos = pos;
	m_rot = rot;
	m_boxSize = size;
	m_obb = COBB(
		m_pos,
		m_rot,
		m_boxSize
	);
}

NoiseBox::~NoiseBox()
{
}

void NoiseBox::Update()
{
	//メインカメラ取得
	if (!mp_enemy)
	{
		mp_enemy = dynamic_cast<Enemy*>(TaskManeger::FindObject(TaskType::eEnemy));
	}
}

void NoiseBox::Render()
{
	if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	Utility::DrawOBB(m_obb, CVector4D(1, 1, 1, 1));
}

void NoiseBox::NoEnemyRender()
{
	Utility::DrawOBB(m_obb, CVector4D(1, 1, 1, 1));
}

void NoiseBox::Collision(Task* b) {
	switch (b->GetType()) {
	case TaskType::ePlayer:
	{
		//■OBBとカプセル
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(m_obb, mp_enemy->m_lineS, mp_enemy->m_lineE, mp_enemy->m_rad, &axis, &dist)) {
			
			mp_enemy->m_isHide = true;
		}
		else {
			mp_enemy->m_isHide = false;
		}
	}
	break;
	}
}
