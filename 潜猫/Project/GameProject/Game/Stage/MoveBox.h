#pragma once
//#include "../Base/Base.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"

class MoveBox : public Task{
private:
	Task* mp_player;
	//箱のサイズ
	CVector3D m_boxSize;
	CVector3D m_startPos;
	CVector3D m_endPos;
	float m_moveTime;
	float m_intervalTime;
	

	enum class State
	{
		eMove,
		eInterval,
	};
	State m_state;
	bool m_isEnable;
	bool m_isReverse;
	float m_elapsedTime;
	int m_yxz;//m_number
	int m_cnt;
	bool m_isMoveflg;
public:
	MoveBox(const CVector3D& pos, const CVector3D& rot, const CVector3D& size,
		const CVector3D& startPos, const CVector3D& endPos,
		const float moveTime, const float intervalTime, const int yxz, const int cnt, bool isStart = false);
	~MoveBox();

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