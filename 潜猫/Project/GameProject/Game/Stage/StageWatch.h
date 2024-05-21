#pragma once
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"

class StageWatch : public Task
{
private:
	CModel* m_model;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	StageWatch();
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈��
	/// </summary>
	void Render();

	void NoEnemyRender();
	/// <summary>
	/// ���f���̎擾
	/// </summary>
	/// <returns></returns>
	CModel* GetModel() {
		return m_model;
	}
};