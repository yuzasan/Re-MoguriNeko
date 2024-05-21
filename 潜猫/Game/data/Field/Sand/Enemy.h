#pragma once
#include "../Base/Base.h"

class Enemy : public Base{
private:
	//���f���I�u�W�F�N�g
	CModelA3M m_model;
	CVector3D m_rot_target;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�o���ʒu</param>
	Enemy(const CVector3D& pos);
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈��
	/// </summary>
	void Render();
	/// <summary>
	/// �����蔻�茟��
	/// </summary>
	/// <param name="b">�Ώە�</param>
	void Collision(Base* b);
};