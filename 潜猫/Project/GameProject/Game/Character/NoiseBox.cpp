#include "NoiseBox.h"
#include "Player.h"
#include "Enemy.h"
#include "Game/Gamedata/GameData.h"
#include "../../Navigation/NavNode.h"
#include "../../Navigation/NavManeger.h"

NoiseBox::NoiseBox(const CVector3D& pos, const CVector3D& rot, const CVector3D& size)
	: CharaBase(TaskType::eNoiseBox)
	, mp_player(nullptr)
	, mp_enemy(nullptr)
	, m_isNoise(false)
	, m_isNoisemove(false)
{
	m_pos = pos;
	m_rot = rot;
	m_boxSize = size;
	m_obb = COBB(
		m_pos,
		m_rot,
		m_boxSize
	);

	//ボックス位置に経路探索用のノードを作成
	m_navNode = new NavNode
	(
		m_pos + CVector3D(0.0f, 1.0f, 0.0f),
		NavNode::NodeType::Destination
	);
	m_navNode->SetNodeColor(CVector3D(0.25f, 1.0f, 0.25f));
}

NoiseBox::~NoiseBox()
{
	//経路探索管理クラスのノードリストから取り除く
	NavManeger::Instance()->RemoveNode(m_navNode);
	NavNode* playerNode = mp_player->GetNavNode();
	//自身とプレイヤーのノードの接続を切る
	m_navNode->RemoveConnect(playerNode);
}

void NoiseBox::Update()
{
	//プレイヤー取得
	if (!mp_player)
	{
		mp_player = dynamic_cast<Player*>(TaskManeger::FindObject(TaskType::ePlayer));
	}
	//敵取得
	//if (!mp_enemy)
	//{
	//	//複数いる場合最初の一体だけポインターを持つ
	//	mp_enemy = dynamic_cast<Enemy*>(TaskManeger::FindObject(TaskType::eEnemy));
	//}
}

void NoiseBox::Render()
{
	if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	if (m_isNoise) {
		Utility::DrawOBB(m_obb, CVector4D(1.0f, 0.25f, 0.25f, 1));
	}
	else {
		Utility::DrawOBB(m_obb, CVector4D(0.25f, 1.0f, 0.25f, 1));
	}
}

void NoiseBox::NoEnemyRender()
{
	if (m_isNoise) {
		Utility::DrawOBB(m_obb, CVector4D(1.0f, 0.25f, 0.25f, 1));
	}
	else {
		Utility::DrawOBB(m_obb, CVector4D(0.25f, 1.0f, 0.25f, 1));
	}
}

void NoiseBox::Collision(Task* b) {
	switch (b->GetType()) {
	case TaskType::ePlayer:
	{
		//■OBBとカプセル
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(m_obb, mp_player->m_lineS, mp_player->m_lineE, mp_player->m_rad, &axis, &dist)) {
			GameData::m_isnoiseflg = true;
			m_isNoise = true;
		}
	}
	break;
	case TaskType::eEnemy:
	{
		//■OBBとカプセル
		float dist;
		CVector3D axis;
		Enemy* enemy = dynamic_cast<Enemy*>(b);
		if (enemy != nullptr) {
			if (CCollision::CollisionOBBCapsule(m_obb, enemy->m_lineS, enemy->m_lineE, enemy->m_rad, &axis, &dist)) {
				if (m_isNoise) {
					m_isNoise = false;
					m_isNoisemove = false;
					Kill();
				}
			}
		}
	}
	break;
	}
}
