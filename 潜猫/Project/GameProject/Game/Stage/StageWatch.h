#pragma once
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"

class StageWatch : public Task
{
private:
	CModel* m_model;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	StageWatch();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Render();

	void NoEnemyRender();
	/// <summary>
	/// モデルの取得
	/// </summary>
	/// <returns></returns>
	CModel* GetModel() {
		return m_model;
	}
};