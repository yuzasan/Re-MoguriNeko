#include "HideBox.h"
#include "../Gamedata/GameData.h"

HideBox::HideBox(const CVector3D& pos, const CVector3D& rot, const CVector3D& size)
	: Task(TaskType::eHideBox)
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

HideBox::~HideBox() {

}

//�X�V����
void HideBox::Update()
{
}

//�`�揈��
void HideBox::Render()
{
	if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	Utility::DrawOBB(m_obb, CVector4D(1, 1, 1, 0.5));
}

void HideBox::NoEnemyRender()
{
	Utility::DrawOBB(m_obb, CVector4D(1, 1, 1, 0.5));
}

void HideBox::Collision(Task* b) {
	switch (b->GetType()) {
	case TaskType::ePlayer:
	{
		//��OBB�ƃJ�v�Z��
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(m_obb, mp_player->m_lineS, mp_player->m_lineE, mp_player->m_rad, &axis, &dist)) {
			//if (axis.y > 0.5f) {
			//	//�ʂ���������n�ʂ���������
			//	//�d�͗������x��0�ɖ߂�
			//	if (mp_player->m_vec.y < 0) {
			//		mp_player->m_vec.y = 0;
			//		
			//	}
			//}
			mp_player->m_isHide = true;
		}
		else {
			mp_player->m_isHide = false;
		}
	}
	break;
	}
}