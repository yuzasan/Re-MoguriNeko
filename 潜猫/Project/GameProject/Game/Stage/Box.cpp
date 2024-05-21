#include "Box.h"

Box::Box(const CVector3D& pos, const CVector3D& rot, const CVector3D& size)
	: Task(TaskType::eBox)
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
	mp_player = TaskManeger::FindObject(TaskType::ePlayer);
}

Box::~Box() {

}

//更新処理
void Box::Update()
{
}

//描画処理
void Box::Render()
{
	if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	Utility::DrawOBB(m_obb, CVector4D(0, 1, 1, 1));
}

void Box::NoEnemyRender()
{
	Utility::DrawOBB(m_obb, CVector4D(0, 1, 1, 1));
}

void Box::Collision(Task* b) {
	switch (b->GetType()) {
	case TaskType::ePlayer:
	{
		//■OBBとカプセル
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(m_obb, mp_player->m_lineS, mp_player->m_lineE, mp_player->m_rad, &axis, &dist)) {
			if (axis.y > 0.5f) {
				//面が上向き→地面が当たった
				//重力落下速度を0に戻す
				if (mp_player->m_vec.y < 0) {
					mp_player->m_vec.y = 0;
					mp_player->m_isGround = true;
				}
			}
			//押し戻し
			float s = mp_player->m_rad - dist;
			mp_player->m_pos += axis * s;
		}
	}
	break;
	}
}