#pragma once
//#include "../Base/Base.h"
#include "../TaskSystem/Task.h"

class Title :public Task {
	//�摜
	CImage m_img;
	//����
	CFont m_title_text;
	int m_cnt;
	float m_time;
	bool m_isOn;
	float fuwafuwa_r = 0;

	enum class Step{
		Start,
		Mid,
		End,
	};
	Step m_step;

	void StepFirst();
	void StepSecond();
	void StepLast();
	void FuwaFuwa();
public:
	Title();
	~Title();
	void Update();
	void Draw();
};