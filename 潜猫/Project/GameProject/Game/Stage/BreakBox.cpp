#include "BreakBox.h"
#include "../Gamedata/GameData.h"
#include "../UI/UI.h"

BreakBox::BreakBox(const CVector3D& pos, const CVector3D& rot, const CVector3D& size, const int& item)
	: Task(TaskType::eBreakBox)
	,m_cnt(item)
	,m_flg(false)
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

BreakBox::~BreakBox() {
	
}

//更新処理
void BreakBox::Update()
{

}

//描画処理
void BreakBox::Render()
{
	if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	//Utility::DrawOBB(m_obb, CVector4D(1, 1, 1, 1));
	switch (m_cnt)
	{
	case 1:
		Utility::DrawOBB(m_obb, CVector4D(1, 1, 1, 1));
		break;
	case 2:
		Utility::DrawOBB(m_obb, CVector4D(1, 0, 0, 1));
		break;
	case 3:
		Utility::DrawOBB(m_obb, CVector4D(0, 1, 0, 1));
		break;
	case 4:
		Utility::DrawOBB(m_obb, CVector4D(0, 0, 1, 1));
		break;
	case 5:
		Utility::DrawOBB(m_obb, CVector4D(0, 0, 0, 1));
		break;
	}
}

void BreakBox::NoEnemyRender()
{
	if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	switch (m_cnt)
	{
	case 1:
		Utility::DrawOBB(m_obb, CVector4D(1, 1, 1, 1));
		break;
	case 2:
		Utility::DrawOBB(m_obb, CVector4D(1, 0, 0, 1));
		break;
	case 3:
		Utility::DrawOBB(m_obb, CVector4D(0, 1, 0, 1));
		break;
	case 4:
		Utility::DrawOBB(m_obb, CVector4D(0, 0, 1, 1));
		break;
	case 5:
		Utility::DrawOBB(m_obb, CVector4D(0, 0, 0, 1));
		break;
	}
}

void BreakBox::Collision(Task* b) {
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
			if (!m_flg) {
				//new Font("残り%d足りません", m_cnt - GameData::m_item, CVector2D(480, 540), CVector3D(0, 0, 0));
			}
			m_flg = true;
			if (GameData::m_item >= m_cnt && GameData::m_step == 1) {
				GameData::m_breakflg = 1;
				Kill();
			}
			if (GameData::m_item2 >= m_cnt && GameData::m_step == 3) {
				Kill();
			}
		}
	}
	break;
	}
}