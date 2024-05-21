#pragma once
//#include "../Base/Base.h"
#include "../TaskSystem/Task.h"

class Choose :public Task {
	//‰æ‘œ
	CImage m_img;
	//•¶Žš
	CFont m_title_text;
	int m_cnt;
	float m_time;
	bool m_isOn;
	float fuwafuwa_r = 0;

	enum class Step {
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
	Choose();
	~Choose();
	void Update();
	void Draw();
};
