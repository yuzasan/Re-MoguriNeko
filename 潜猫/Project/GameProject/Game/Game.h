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
	//‰æ‘œ
	CImage m_img;
	//•¶š
	CFont m_title_text;
public:
	Clear();
	~Clear();
	void Update();
	void Draw();
};

/*
//ˆÃ“]
class BlackOut : public Task {
public:
	int m_gauge;	//c•
	int m_num;		//”Ô†
	float m_alpha;
	BlackOut();
	~BlackOut();
	void LateUpdate();
};
*/
