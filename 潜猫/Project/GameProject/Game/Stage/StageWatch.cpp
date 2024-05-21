#include "StageWatch.h"

StageWatch::StageWatch(): Task(TaskType::eFieldWatch)
{
	m_model = GET_RESOURCE("Stage", CModel);
}

void StageWatch::Update()
{
}

void StageWatch::Render()
{
	m_model->Render();
}

void StageWatch::NoEnemyRender()
{
	//m_model->Render();
}