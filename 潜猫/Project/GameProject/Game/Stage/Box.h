#pragma once
//#include "../Base/Base.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class Box : public Task {
private:
	//Base* mp_player;
	Task* mp_player;
	//箱のサイズ
	CVector3D m_boxSize;

public:
	Box(const CVector3D& pos, const CVector3D& rot, const CVector3D& size);
	~Box();

	void Update();
	void Render();
	/// <summary>
	/// enemy以外の描画処理
	/// </summary>
	void NoEnemyRender();
	//void Collision(Base* b);
	void Collision(Task* b);

};