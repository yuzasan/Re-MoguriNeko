#pragma once
//#include "../Base/Base.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"

class FieldCamera : public Task {
private:
	//�������_�[�^�[�Q�b�g
//���Վ��_�ł̃Q�[����ʕ`��p
	CTextureFrame* texture_frame_mini_map = nullptr;
	Task* mp_player;
	//Task* mp_enemy;
public:
	//static�͕ʂ̂Ƃ��납��Q�Ƃł���
	static bool rendring;
	FieldCamera();
	~FieldCamera();
	void Update();
	void Draw();
	void LateUpdate();
};