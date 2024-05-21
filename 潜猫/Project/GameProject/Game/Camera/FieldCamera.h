#pragma once
//#include "../Base/Base.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"

class FieldCamera : public Task {
private:
	//■レンダーターゲット
//俯瞰視点でのゲーム画面描画用
	CTextureFrame* texture_frame_mini_map = nullptr;
	Task* mp_player;
	//Task* mp_enemy;
public:
	//staticは別のところから参照できる
	static bool rendring;
	FieldCamera();
	~FieldCamera();
	void Update();
	void Draw();
	void LateUpdate();
};