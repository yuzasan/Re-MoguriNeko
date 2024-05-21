#pragma once
//#include "../Base/Base.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class TyukanBox : public Task {
private:
	Task* mp_player;
	//���̃T�C�Y
	CVector3D m_boxSize;

public:
	TyukanBox(const CVector3D& pos, const CVector3D& rot, const CVector3D& size);
	~TyukanBox();

	void Update();
	void Render();
	/// <summary>
	/// enemy�ȊO�̕`�揈��
	/// </summary>
	void NoEnemyRender();
	void Collision(Task* b);

};