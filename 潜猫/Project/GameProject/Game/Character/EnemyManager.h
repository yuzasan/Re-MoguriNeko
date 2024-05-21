#pragma once
//#include "../Base/Base.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
#include "Enemy.h"
class SearchNode;

class EnemyManager: public Task {
private:
	//敵の管理クラスのインスタンス
	static EnemyManager* ms_instance;
	//敵の探索位置ノードのリスト
	static std::list<SearchNode> ms_searchNodes;

	//敵のリスト
	std::list<Enemy*> m_enemies;

	bool m_flg;

public:	
	//コンストラクタ
	EnemyManager();
	//デストラクタ
	~EnemyManager();

	//インスタンスを取得
	static EnemyManager* Instance();

	//敵をリストに追加
	void Add(Enemy* enemy);
	//敵をリストから取り除く
	void Remove(Enemy* enemy);

	//次に探索するノードを取得
	SearchNode* GetNextSearchNode() const;

	//更新
	void Update();

	//描画
	void Render();
};

//探索するノード情報
class SearchNode {
public:
	CVector3D pos;	//ノード位置
	float prob;		//存在確率
	Enemy* enemy;	//現在探索に向かっている敵
	SearchNode(const CVector3D& p)
		:pos(p)
		, prob(1.0f)
		, enemy(nullptr) {}
};