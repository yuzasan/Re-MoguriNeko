#include "CharaBase.h"
#include "../Stage/Stage.h"
#include "../../Navigation/NavNode.h"

CharaBase::CharaBase(TaskType type):Task(type){
	m_rad = 0.5f;
}

CharaBase::~CharaBase(){

}

NavNode* CharaBase::GetNavNode() const
{
	return m_navNode;
}

void CharaBase::Update()
{
	// 当たり判定カプセルの設定
	m_lineS = m_pos + CVector3D(0, 1.0f - m_rad, 0);
	m_lineE = m_pos + CVector3D(0, m_rad, 0);
	if (m_navNode != nullptr) {
		m_navNode->SetPos(m_pos + CVector3D(0.0f, 1.0f, 0.0f));
	}
}

// コリジョン描画
void CharaBase::RenderCol()
{
	Utility::DrawCapsule
	(
		m_lineS, m_lineE, m_rad,
		CVector4D(1, 0, 0, 0.1)
	);
}
