#pragma once
//#include "../Base/Base.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
#include "Enemy.h"
class ApperPoint : public Task {
	int m_cnt;
	const Enemy::CourseData* mp_course;
public:
	ApperPoint(const Enemy::CourseData* course);
	void Update();
	void Render();
};
class EnemyManager : public Task {
private:
	int m_cnt;
public:
	EnemyManager();
	void Update();
};