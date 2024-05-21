#pragma once
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"

class EventCamera : public Task {
private:
	//���f���I�u�W�F�N�g
	CVector3D eventcam_at;
	CVector3D eventcam_pos;
	//��]���x�W��
	const float cam_speed = 0.002f;
public:
	//static�͕ʂ̂Ƃ��납��Q�Ƃł���
	static bool eventrendring;
	EventCamera();
	~EventCamera();
	void Update();
	void Render();
};
