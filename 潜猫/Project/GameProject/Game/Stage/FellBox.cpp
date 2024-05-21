#include "FellBox.h"

FellBox::FellBox(const CVector3D& pos, const CVector3D& rot, const CVector3D& size)
	: Task(TaskType::eFellBox)
{
	m_pos = pos;
	m_rot = rot;
	m_boxSize = size;
	m_obb = COBB(
		m_pos,
		m_rot,
		m_boxSize
	);
	//mp_player = Base::FindObject(BaseType::ePlayer);
}

FellBox::~FellBox() {

}

//更新処理
void FellBox::Update()
{
}

//描画処理
void FellBox::Render()
{
	if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	Utility::DrawOBB(m_obb, CVector4D(0, 0, 0, 1));
}

void FellBox::NoEnemyRender()
{
	Utility::DrawOBB(m_obb, CVector4D(0, 0, 0, 1));
}

void FellBox::Collision(Task* b) {
	//switch (b->GetType()) {
	//case BaseType::ePlayer:
	//{
	//	//■OBBとカプセル
	//	float dist;
	//	CVector3D axis;
	//	if (CCollision::CollisionOBBCapsule(m_obb, mp_player->m_lineS, mp_player->m_lineE, mp_player->m_rad, &axis, &dist)) {
	//		//mp_player->m_isFellBox = true;
	//	}
	//	else {
	//		//mp_player->m_isFellBox = false;
	//	}
	//}
	//break;
	//}
}