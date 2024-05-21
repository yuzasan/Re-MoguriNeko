#include "EnemyManager.h"
#include "../../Debug/DebugPrint.h"
#include "../Gamedata/GameData.h"

//敵管理クラスのインスタンス
EnemyManager* EnemyManager::ms_instance = nullptr;
//敵の探索位置ノードのリスト
std::list<SearchNode> EnemyManager::ms_searchNodes =
{
	SearchNode(CVector3D(-8,6,51)),
	SearchNode(CVector3D(-57,6,51)),
	SearchNode(CVector3D(-57,6,-2)),
	SearchNode(CVector3D(-13,6,-2)),

	//SearchNode(CVector3D(-17,17,-4)),

	//--------------------------------
	
	//SearchNode(CVector3D(-8,6,12)),
	//SearchNode(CVector3D(-8,6,24)),
	
	SearchNode(CVector3D(-14,6,5)),
	//SearchNode(CVector3D(-14,6,23)),
	SearchNode(CVector3D(-14,6,43)),

	//SearchNode(CVector3D(-20,6,11)),
	//SearchNode(CVector3D(-20,6,23)),
	//SearchNode(CVector3D(-20,6,36)),

	//SearchNode(CVector3D(-34,6,-2)),
	//SearchNode(CVector3D(-34,6,5)),
	//SearchNode(CVector3D(-34,6,11)),
	//SearchNode(CVector3D(-34,6,36)),
	//SearchNode(CVector3D(-34,6,43)),
	//SearchNode(CVector3D(-34,6,50)),

	//SearchNode(CVector3D(-45,6,36)),
	//SearchNode(CVector3D(-45,6,23)),
	//SearchNode(CVector3D(-45,6,11)),

	SearchNode(CVector3D(-52,6,5)),
	//SearchNode(CVector3D(-52,6,23)),
	SearchNode(CVector3D(-52,6,36)),

	//SearchNode(CVector3D(-58,6,23)),


	//外
	//SearchNode(CVector3D(-8,18,-5)),


	//2階
	//SearchNode(CVector3D(-8, 18, -3)),
	//SearchNode(CVector3D(13, 1, 32)),
	//SearchNode(CVector3D(-19,18,4)),
	//SearchNode(CVector3D(-19,18,-4)),
	//SearchNode(CVector3D(-19,18,35)),

	//SearchNode(CVector3D(-27,18,6)),

	//SearchNode(CVector3D(-47,18,4)),
	//SearchNode(CVector3D(-47,18,35)),

	
	
	
	

	
};

//コンストラクタ
EnemyManager::EnemyManager():Task(TaskType::eEnemyManager)
	,m_flg(false)
{
	//TaskManeger::Instance()->Add(this);
	
}

//デストラクタ
EnemyManager::~EnemyManager()
{
	ms_instance = nullptr;
}

//インスタンスを取得
EnemyManager* EnemyManager::Instance()
{
	//インスタンスがなければ
	if (ms_instance == nullptr) {
		//インスタンスを生成する
		ms_instance = new EnemyManager();
	}
	return ms_instance;
}

//敵をリストに追加
void EnemyManager::Add(Enemy* enemy)
{
	m_enemies.push_back(enemy);
}

//敵をリストから取り除く
void EnemyManager::Remove(Enemy* enemy)
{
	m_enemies.remove(enemy);
}

//次に探索するノードを取得
SearchNode* EnemyManager::GetNextSearchNode() const
{
	//探索ノードの選択条件
	//①他の敵が探索中のノードは選択しない
	//②存在確率が一番高いノードを選択する

	SearchNode* ret = nullptr;
	//リストの先端から順番に調べて、
	//一番条件に近い一致するノードを取得する
	for (SearchNode& node : ms_searchNodes) {
		//既に他の敵が探索中のノードは対象外
		if (node.enemy != nullptr) continue;
		//戻り値のノードがまだ設定されていないもしくは、
		//存在確率が検索中のノードの方が高いのであれば、
		//検索中のノードを優先する
		if (ret == nullptr || node.prob > ret->prob) {
			ret = &node;
		}
	}
	return ret;
}

//更新
void EnemyManager::Update()
{
	for (SearchNode& node : ms_searchNodes) {
		node.prob = Utility::Clamp01(node.prob + 0.0005f);
		//node.prob = Utility::Clamp01(node.prob + 0.03 * CFPS::GetDeltaTime());
	}

	if (GameData::m_diecnt == 3 && !m_flg) {
		new Enemy(CVector3D(-8.0f, 6.0f, 25.0f), 75.0f);
		new Enemy(CVector3D(6.0f, 3.0f, 24.0f), 5.0f);
		m_flg = true;
	}
	
}

//描画
void EnemyManager::Render()
{
	if (!g_isRenderDebug) return;

	for (SearchNode& node : ms_searchNodes) {
		float per = node.prob;
		CVector3D color;
		if (per > 0.5f) {
			color = CVector3D::Leap
			(
				CVector3D(1.0f, 1.0f, 0.0f),
				CVector3D(1.0f, 0.0f, 0.0f),
				(per - 0.5f) / 0.5f
			);
		}
		else {
			color = CVector3D::Leap
			(
				CVector3D(0.0f, 1.0f, 0.0f),
				CVector3D(1.0f, 1.0f, 0.0f),
				per / 0.5f
			);
		}
		Utility::DrawCube(node.pos, 2.0f, CVector4D(color, 1.0f));
	}
}
