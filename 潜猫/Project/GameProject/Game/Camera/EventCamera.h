#pragma once
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"

class EventCamera : public Task {
private:
	//モデルオブジェクト
	CVector3D eventcam_at;
	CVector3D eventcam_pos;
	//回転速度係数
	const float cam_speed = 0.002f;
public:
	//staticは別のところから参照できる
	static bool eventrendring;
	EventCamera();
	~EventCamera();
	void Update();
	void Render();
};
