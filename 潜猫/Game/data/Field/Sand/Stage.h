#pragma once
#include "../Base/Base.h"

class Stage : public Base{
private:
	//�X�e�[�W���f��
	CModelObj m_model;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Stage();
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈��
	/// </summary>
	void Render();
	/// <summary>
	/// �I�u�W�F�N�g�������f���̎擾
	/// </summary>
	/// <returns></returns>
	CModel* GetModel() {
		return &m_model;
	}
};