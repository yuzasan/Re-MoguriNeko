#pragma once
//#include "../Base/Base.h"
#include "../TaskSystem/Task.h"

class Game : public Task {
public:
	Game();
	~Game();
	void Update();
};

class Clear :public Task {
	//�摜
	CImage m_img;
	//����
	CFont m_title_text;
public:
	Clear();
	~Clear();
	void Update();
	void Draw();
};

/*
//�Ó]
class BlackOut : public Task {
public:
	int m_gauge;	//�c��
	int m_num;		//�ԍ�
	float m_alpha;
	BlackOut();
	~BlackOut();
	void LateUpdate();
};
*/
