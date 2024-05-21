#pragma once
//#include "../Base/Base.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class FellBox : public Task {
private:
	Task* mp_player;
	//箱のサイズ
	CVector3D m_boxSize;

public:
	FellBox(const CVector3D& pos, const CVector3D& rot, const CVector3D& size);
	~FellBox();

	void Update();
	void Render();
	/// <summary>
	/// enemy以外の描画処理
	/// </summary>
	void NoEnemyRender();
	void Collision(Task* b);

};