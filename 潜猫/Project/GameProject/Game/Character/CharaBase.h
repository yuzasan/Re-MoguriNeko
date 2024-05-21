#pragma once
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class NavNode;

//キャラクターのベース
class CharaBase :public Task {
protected:
	NavNode* m_navNode;
public:
	//コンストラクタ
	CharaBase(TaskType type);
	//デストラクタ
	virtual ~CharaBase();

	NavNode* GetNavNode() const;

	//更新
	void Update();

	// コリジョン描画
	void RenderCol();
};