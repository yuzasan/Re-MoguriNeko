#include "TyukanBox.h"
TyukanBox::TyukanBox(const CVector3D& pos, const CVector3D& rot, const CVector3D& size)
	: Task(TaskType::eTyukanBox)
{
	m_pos = pos;
	m_rot = rot;
	m_boxSize = size;
	m_obb = COBB(
		m_pos,
		m_rot,
		m_boxSize
	);
	mp_player = TaskManeger::FindObject(TaskType::ePlayer);
}

TyukanBox::~TyukanBox() {

}

//更新処理
void TyukanBox::Update()
{
}

//描画処理
void TyukanBox::Render()
{
	if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	Utility::DrawOBB(m_obb, CVector4D(1, 0, 0.5, 0.5));
}

void TyukanBox::NoEnemyRender()
{
	Utility::DrawOBB(m_obb, CVector4D(1, 0, 0.5, 0.5));
}

void TyukanBox::Collision(Task* b) {
	//switch (b->GetType()) {
	//case BaseType::ePlayer:
	//{
	//	//■OBBとカプセル
	//	float dist;
	//	CVector3D axis;
	//	if (CCollision::CollisionOBBCapsule(m_obb, mp_player->m_lineS, mp_player->m_lineE, mp_player->m_rad, &axis, &dist)) {
	//		//SetKill();
	//	}
	//}
	//break;
	//}
}