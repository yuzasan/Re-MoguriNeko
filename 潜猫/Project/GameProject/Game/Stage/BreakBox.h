#pragma once
//#include "../Base/Base.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class BreakBox : public Task {
private:
	//Base* mp_player;
	Task* mp_player;
	//���̃T�C�Y
	CVector3D m_boxSize;
	int m_cnt;
	bool m_flg;

public:
	BreakBox(const CVector3D& pos, const CVector3D& rot, const CVector3D& size,const int &item);
	~BreakBox();

	void Update();
	void Render();
	/// <summary>
	/// enemy�ȊO�̕`�揈��
	/// </summary>
	void NoEnemyRender();
	//void Collision(Base* b);
	void Collision(Task* b);

};