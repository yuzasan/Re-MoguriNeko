#pragma once
//#include "../Base/Base.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"

class Player;
class Stage;
class PlayerCamera : public Task {
private:
	Task* mp_player;
	//モデルオブジェクト
	CModelA3M m_player;
	CMatrix cam_matrix;
	CVector3D cam_at;
	CVector3D cam_pos;
	//回転速度係数
	const float cam_speed = 0.002f;
public:
	PlayerCamera();
	~PlayerCamera();
	void Update();
	void Render();
	void Collision(Task* b);
};
