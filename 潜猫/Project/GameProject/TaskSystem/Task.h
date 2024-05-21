#pragma once

//�^�X�N�̎��
//�ォ�珇�ԂɍX�V�A�`�悳���A�폜
enum class TaskType {
	//3D
	eCamera,		//���C���J����
	eField,			//�t�B�[���h
	eFieldWatch,	//�t�B�[���h
	eFellBox,		//�f�X�]�[��
	eBox,			//���蔲���{�b�N�X
	eRotateBox,		//��]�{�b�N�X
	eMoveBox,		//�ړ��{�b�N�X
	eHideBox,		//�B��{�b�N�X
	eTyukanBox,		//���ԃ{�b�N�X
	eNoiseBox,		//�����{�b�N�X
	eBreakBox,
	eAppearPoint,	//�ړ��_
	ePlayer,		//�v���C���[
	eEnemy,			//�G
	eNavNode,		//�m�[�h
	eEnemyManager,	//�G�Ǘ�
	ePlayer_Attack,	//�v���C���[�U��
	eEnemy_Attack,	//�G�U��
	eNavManeger,	//�m�[�h�Ǘ�
	eBullet,		//�e
	eEffect,		//�G�t�F�N�g
	eItem,			//�A�C�e��
	eLastItem,			//�A�C�e��
	eEventCamera,	//�C�x���g�J����
	//eFieldCamera,	//�~�j�}�b�v

	//2D
	eFieldCamera,	//�~�j�}�b�v
	eUI,			//UI
	eScene,			//�V�[��
	eFade,			//�t�F�[�h
	eDebug,			//�f�o�b�O

	//eStart2D=eUI,	//=�̕�������2D
	eStart2D = eFieldCamera,	//=�̕�������2D
};

//�^�X�N�N���X
class Task {
private:
	TaskType m_type;	//�^�X�N�̎��
	//���̏ꏊ
	//bool m_isKill;		//�폜�t���O
public:
	//TaskType	m_type;		//�I�u�W�F�N�g�̎��
	CVector3D	m_pos;		//���W
	CVector3D	m_rot;		//��]�l
	CVector3D	m_vec;		//�ړ��x�N�g��
	CVector3D	m_dir;		//�O����

	//���A�J�v�Z���p
	float		m_rad;		//���a
	float		m_height;	//����
	//�J�v�Z���p
	CVector3D	m_lineS;	//�����n�_
	CVector3D	m_lineE;	//�����I�_
	//OBB�p
	COBB m_obb;				//Box
	COBB m_obb_old;
	float m_time;			//���Ԍo��
	bool m_isGround;		//���n�t���O
	bool m_isHide;			//HideBox�ɉB��Ă��邩
	bool m_isFind;			//Player��Enemy�ɐG���Ă��邩
	bool m_isFindplayer;	//Enemy�̒T�m�͈͈ȓ���Player�����邩


	//�U���̘A���q�b�g�h�~�[�u
	int m_hit_no;			//�U���q�b�g���ԍ�
	int m_attack_no;		//�U���ԍ�
	bool m_isKill;			//�폜�t���O
	bool m_dontKill;		//�폜���Ȃ��t���O
	//�R���X�g���N�^
	Task(TaskType type, bool dontKill = false);
	//�f�X�g���N�^
	virtual ~Task();

	//�^�X�N�̎�ނ��擾
	TaskType GetType() const;

	//�I�u�W�F�N�g�������f���̎擾
	virtual CModel* GetModel();

	//�폜�t���O���擾
	bool IsKill() const;
	//�^�X�N���폜
	void Kill();
	
	//�X�V
	virtual void Update();
	//�`��
	virtual void Render();
	//2D�`��
	virtual void Draw();
	//enemy�ȊO�̕`�揈��
	virtual void NoEnemyRender();
	//�`���ɕ`��
	virtual void LateRender();
	//�`����2D�`��
	virtual void LateDraw();
	//�`���ɍX�V
	virtual void LateUpdate();
	//�I�u�W�F�N�g���m�̓����蔻�茟��
	virtual void Collision(Task* b);
	
};