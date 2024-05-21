#pragma once
//#include "../Base/Base.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class Player;

class Enemy : public Task {
	//����̊p�x
	float view_ang = DtoR(35);
	//����̋���
	float view_length = 5;
public:
	struct CourseData {
		CVector3D* points;
		int size;
	};

private:
	Player* mp_player;
	Task* mp_field;
	//���f���I�u�W�F�N�g
	CModelA3M m_model;
	CVector3D m_rot_target;
	const CourseData* mp_cource;	//�ʂ铹
	int		m_cource_idx;
	CVector4D color;
	float m_elapsedTime;	//�ҋ@�o�ߎ���
	CVector3D m_pos_old;

	//�A�j���[�V�����ԍ�
	enum class AnimId
	{
		Idle,	//�ҋ@
		Walk,	//����
	};

	//���
	enum class State
	{
		Idle,	//�ҋ@
		Move,	//�ړ�
		Chase,	//�ǐ�
	};
	State m_state;	//���݂̏��

	//�ҋ@��Ԃ̏���
	void StateIdle();
	//�ړ���Ԃ̏���
	void StateMove();
	//�ǐՏ�Ԃ̏���
	void StateChase();

public:
	bool isFoll;
	int m_cntfoll;
	bool m_isTouch;
	bool m_isWall;
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�o���ʒu</param>
	Enemy(const CourseData* data);
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
	void Collision(Task* b);
	/// <summary>
	/// ���ʂ̃J�v�Z���Ɛ����Ƃ̔���
	/// </summary>
	/// <param name="lineS">�����n�_</param>
	/// <param name="lineE">�����I�_</param>
	/// <param name="dir">��������</param>
	/// <param name="cross">�ڐG�_</param>
	/// <returns>1�Ȃ�ڐG��������:(0)��ڐG</returns>
	int CollisionLine(const CVector3D& lineS, const CVector3D& lineE, const CVector3D& dir, CVector3D* cross);
	//�v���C���[�����������ǂ���
	bool IsFoundPlayer();
	//�ǂ��Ȃ���
	void Shot();
	//�f�o�b�O����
	void Debug();
	//�A�C�e��
	void FollBox();
	void LateRender();
};