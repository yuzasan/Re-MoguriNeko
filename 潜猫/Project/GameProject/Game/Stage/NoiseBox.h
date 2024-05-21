#pragma once
//#include "../Base/Base.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class Enemy;

class NoiseBox : public Task {
private:
	Enemy* mp_enemy;
	//箱のサイズ
	CVector3D m_boxSize;

public:
	NoiseBox(const CVector3D& pos, const CVector3D& rot, const CVector3D& size);
	~NoiseBox();

	void Update();
	void Render();
	/// <summary>
	/// enemy以外の描画処理
	/// </summary>
	void NoEnemyRender();
	void Collision(Task* b);

};