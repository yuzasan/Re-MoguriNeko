#pragma once
#include "CharaBase.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class Player;
class NavNode;
class SearchNode;
class NoiseBox;

class Enemy : public CharaBase {
	//����̊p�x
	float view_ang = DtoR(35);
	//����̋���
	float view_length = 5;
public:
	/*struct CourseData {
		CVector3D * points;
		int size;
	};*/
private:
	Player* mp_player;				//�v���C���[
	NoiseBox* mp_noise;				//����BOX
	Task* mp_field;					//�X�e�[�W
	CModelA3M m_model;				//���f��
	CModelObj m_modelobj;				//���f��
	CVector3D m_moveDir;			//�ړ�����
	CVector3D m_movePos;			//�ړ�����W
	NavNode* m_moveNode;			//�ړ��ړI�n�̃m�[�h
	NavNode* m_nextNode;			//���Ɉړ�����m�[�h�̊i�[�p
	NavNode* m_lostNode;			//�v���C���[�����������ꏊ�ɔz�u�����m�[�h
	SearchNode* m_searchNode;		//�T�����̃m�[�h
	CVector4D color;
	float m_elapsedTime;			//�ҋ@�o�ߎ���
	CVector3D m_pos_old;			//�O�̈ʒu
	float m_emotion;				//����
	
	CVector3D m_rot_target;
	//const CourseData* mp_cource;	//�ʂ铹
	//int		m_cource_idx;

	//�A�j���[�V�����ԍ�
	enum class AnimId
	{
		Idle,			//�m�[�}���ҋ@
		Walk,			//�m�[�}������
		HappyIdle,		//���ꂵ���ҋ@
		HappyIdle2,		//���ꂵ���ҋ@2
		HappyWalk,		//���ꂵ������
		BackSearch,		//���x��
		BackSearch2,	//���x��2
		WarningIdle,	//�x���ҋ@
		WarningIdle2,	//�x���ҋ@2
		WarningSearch,	//�S���ʌx��
		CrossIdle,		//�C���C���ҋ@
		CrossIdle2,		//�C���C���ҋ@2
		CrossWalk,		//�C���C������
		CrossWalk2,		//�C���C������2
		Run,			//�S�͑���
		Run2,			//����
		SadIdle,		//�������ݑҋ@
		SadWalk,		//�������ݕ���
		SleepIdle,		//����ҋ@
		Sleeping,		//����
	};

	//���
	enum class State
	{
		Normal,		//�ʏ�
		Happy,		//���ꂵ��
		Warning,	//�x��
		Cross,		//�C���C��
		Sad,		//��������
		Sleep,		//����
	};
	State m_state;	//���݂̏��

	//�s��
	enum class Action
	{
		Idle,			//�ҋ@
		Move,			//�ړ�
		Search,			//�x��
		Chase,			//�ǐ�
		Lost,			//�v���C���[����������
		/*
		HappyIdle,		//���ꂵ���ҋ@
		HappyWalk,		//���ꂵ������
		BackSearch,		//���x��
		WarningIdle,	//�x���ҋ@
		CrossIdle,		//�C���C���ҋ@
		CrossWalk,		//�C���C������
		SadIdle,		//�������ݑҋ@
		SadWalk,		//�������ݕ���
		SleepIdle,		//����ҋ@
		Sleeping,		//����
		*/
	};
	Action m_action;	//���݂̍s�����

	//�ʏ�s���̏���
	void ActionIdle();
	//�ړ��s���̏���
	void ActionMove();
	//�x���s���̏���
	void ActionSearch();
	//�ǐՍs���̏���
	void ActionChase();
	//�v���C���[������������Ԃ̏���
	void ActionLost();

public:
	bool isFoll;
	int m_cntfoll;
	bool m_isTouch;
	bool m_isWall;
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�o���ʒu</param>
	Enemy(const CVector3D& pos,float emotion);
	//�f�X�g���N�^
	~Enemy();
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

	//���݈ʒu����v���C���[�����������ǂ���
	bool IsLookPlayer() const;

	//�w����W�ւ̈ړ�����
	bool MoveTo(const CVector3D& target);

	//�T���m�[�h��ݒ�
	void SetSearchNode(SearchNode* node);
	//�T�����̃m�[�h���擾
	SearchNode* GetSearchNode() const;

};