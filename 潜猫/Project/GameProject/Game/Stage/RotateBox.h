#pragma once
//#include "../Base/Base.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"

class RotateBox : public Task {
private:
	Task* mp_player;
	//箱のサイズ
	CVector3D m_boxSize;
	float m_rotate;
	float now_rot;
	float m_moveTime;
	float m_intervalTime;
	int cnt = 0;

	enum class State
	{
		eMove,
		eInterval,
	};
	State m_state;
	bool m_isEnable;
	float m_elapsedTime;
	int m_xyz;
	int m_number;
public:
	RotateBox(const CVector3D& pos, const CVector3D& rot,
		const CVector3D& size, const float rotate, const int xyz,
		//const CVector3D& size, const float rotate, const int xyz, const float moveTime,
		const float intervalTime, const int m_number, bool isStart = false);
	~RotateBox();

	void Start();
	void Stop();

	void UpdateMove();
	void UpdateInterval();

	void Update();
	void Render();
	/// <summary>
	/// enemy以外の描画処理
	/// </summary>
	void NoEnemyRender();
	void Collision(Task* b);

};