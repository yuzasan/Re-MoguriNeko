#pragma once
//#include "../Base/Base.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
#include "EnemyCopy.h"
class SearchNodeC;

class EnemyManagerC : public Task {
private:
	//敵の管理クラスのインスタンス
	static EnemyManagerC* ms_instance;
	//敵の探索位置ノードのリスト
	static std::list<SearchNodeC> ms_searchNodes;

	//敵のリスト
	std::list<EnemyC*> m_enemies;

	bool m_flg;

public:
	//コンストラクタ
	EnemyManagerC();
	//デストラクタ
	~EnemyManagerC();

	//インスタンスを取得
	static EnemyManagerC* Instance();

	//敵をリストに追加
	void Add(EnemyC* enemyc);
	//敵をリストから取り除く
	void Remove(EnemyC* enemyc);

	//次に探索するノードを取得
	SearchNodeC* GetNextSearchNode() const;

	//更新
	void Update();

	//描画
	void Render();
};

//探索するノード情報
class SearchNodeC {
public:
	CVector3D pos;	//ノード位置
	float prob;		//存在確率
	EnemyC* enemyc;	//現在探索に向かっている敵
	SearchNodeC(const CVector3D& p)
		:pos(p)
		, prob(1.0f)
		, enemyc(nullptr) {}
};