#include "EnemyManagerCopy.h"
#include "../../Debug/DebugPrint.h"
#include "../Gamedata/GameData.h"

//敵管理クラスのインスタンス
EnemyManagerC* EnemyManagerC::ms_instance = nullptr;
//敵の探索位置ノードのリスト
std::list<SearchNodeC> EnemyManagerC::ms_searchNodes =
{
	//2階
	SearchNodeC(CVector3D(-19,18,4)),
	//SearchNodeC(CVector3D(-19,18,35)),

	SearchNodeC(CVector3D(-27,18,6)),

	//SearchNodeC(CVector3D(-47,18,4)),
	//SearchNodeC(CVector3D(-47,18,35)),

	//各部屋のノード
	
	SearchNodeC(CVector3D(-25,18,-3)),
	SearchNodeC(CVector3D(-37,18,19)),
	SearchNodeC(CVector3D(-41,18,27)),
	SearchNodeC(CVector3D(-48,18,-3)),
	SearchNodeC(CVector3D(-55,18,21)),
	SearchNodeC(CVector3D(-56,18,-3)),
	

};

//コンストラクタ
EnemyManagerC::EnemyManagerC() :Task(TaskType::eEnemyManager)
, m_flg(false)
{
	//TaskManeger::Instance()->Add(this);

}

//デストラクタ
EnemyManagerC::~EnemyManagerC()
{
	ms_instance = nullptr;
}

//インスタンスを取得
EnemyManagerC* EnemyManagerC::Instance()
{
	//インスタンスがなければ
	if (ms_instance == nullptr) {
		//インスタンスを生成する
		ms_instance = new EnemyManagerC();
	}
	return ms_instance;
}

//敵をリストに追加
void EnemyManagerC::Add(EnemyC* enemyc)
{
	m_enemies.push_back(enemyc);
}

//敵をリストから取り除く
void EnemyManagerC::Remove(EnemyC* enemyc)
{
	m_enemies.remove(enemyc);
}

//次に探索するノードを取得
SearchNodeC* EnemyManagerC::GetNextSearchNode() const
{
	//探索ノードの選択条件
	//①他の敵が探索中のノードは選択しない
	//②存在確率が一番高いノードを選択する

	SearchNodeC* ret = nullptr;
	//リストの先端から順番に調べて、
	//一番条件に近い一致するノードを取得する
	for (SearchNodeC& node : ms_searchNodes) {
		//既に他の敵が探索中のノードは対象外
		if (node.enemyc != nullptr) continue;
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
void EnemyManagerC::Update()
{
	for (SearchNodeC& node : ms_searchNodes) {
		node.prob = Utility::Clamp01(node.prob + 0.0005f);
		//node.prob = Utility::Clamp01(node.prob + 0.03 * CFPS::GetDeltaTime());
	}

	if (GameData::m_diecnt == 3 && !m_flg) {
		new EnemyC(CVector3D(-8.0f, 6.0f, 25.0f), 75.0f);
		new EnemyC(CVector3D(6.0f, 3.0f, 24.0f), 5.0f);
		m_flg = true;
	}

}

//描画
void EnemyManagerC::Render()
{
	if (!g_isRenderDebug) return;

	for (SearchNodeC& node : ms_searchNodes) {
		float per = node.prob;
		CVector3D color;
		if (per > 0.5f) {
			color = CVector3D::Leap
			(
				CVector3D(1.0f, 1.0f, 0.0f),
				CVector3D(0.0f, 0.0f, 1.0f),
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
