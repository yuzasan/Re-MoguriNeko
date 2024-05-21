#pragma once
#define GRAVITY (0.01f)
//��ނ̗�
enum class BaseType {
	eCamera,
	eField,
	eFellBox,
	eBox,
	eRotateBox,
	eMoveBox,
	eHideBox,
	eTyukanBox,
	eEnemyManager,
	eAppearPoint,
	ePlayer,
	eEnemy,
	ePlayer_Attack,
	eEnemy_Attack,
	eBullet,
	eEffect,
	eItem,
	eFieldCamera,
	eUI,
	eScene,
};
class Base {
public:
	BaseType	m_type;		//�I�u�W�F�N�g�̎��
	CVector3D	m_pos;		//���W
	CVector3D	m_rot;		//��]�l
	CVector3D	m_vec;		//�ړ��x�N�g��
	CVector3D	m_dir;		//�O����
	int		m_kill;		//�폜�t���O
	
	//���A�J�v�Z���p
	float		m_rad;		//���a
	//�J�v�Z���p
	CVector3D	m_lineS;	//�����n�_
	CVector3D	m_lineE;	//�����I�_
	//OBB�p
	COBB m_obb;
	float m_time;
	bool m_isGround;
	//HideBox�ɉB��Ă��邩
	bool m_isHide;
	//Player��Enemy�ɐG���Ă��邩
	bool m_isFind;
	//Enemy�̒T�m�͈͈ȓ���Player�����邩
	bool m_isFindplayer;


	//�U���̘A���q�b�g�h�~�[�u
	int m_hit_no;			//�U���q�b�g���ԍ�
	int m_attack_no;		//�U�����ԍ�
private:
	//�I�u�W�F�N�g���X�g
	static std::list<Base*> m_list;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="type">�I�u�W�F�N�g�̎��</param>
	Base(BaseType type);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Base();
	/// <summary>
	/// �폜�t���OON
	/// </summary>
	void SetKill();
	/// <summary>
	/// �I�u�W�F�N�g�̎�ނ��擾
	/// </summary>
	/// <returns></returns>
	BaseType GetType() {
		return m_type;
	}
	/// <summary>
	/// �I�u�W�F�N�g�������f���̎擾
	/// </summary>
	/// <returns></returns>
	virtual CModel* GetModel() {
		return nullptr;
	}
	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update();
	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Render();
	/// <summary>
	/// enemy�ȊO�̕`�揈��
	/// </summary>
	virtual void NoEnemyRender();
	//�`���ɕ`��
	virtual void LateRender();
	//�`���ɍX�V
	virtual void LateUpdate();
	/// <summary>
	/// �I�u�W�F�N�g���m�̓����蔻�茟��
	/// </summary>
	/// <param name="b">�ՓˑΏۃI�u�W�F�N�g</param>
	virtual void Collision(Base* b);

	/// <summary>
	/// �S�ẴI�u�W�F�N�g�̍폜
	/// </summary>
	static void KillALL();


	/// <summary>
	/// �S�ẴI�u�W�F�N�g�̍폜�i���j���j
	/// </summary>
	static void ClearInstance();


	/// <summary>
	/// �S�ẴI�u�W�F�N�g�̍폜�`�F�b�N
	/// </summary>
	static void CheckKillALL();
	/// <summary>
	/// �S�ẴI�u�W�F�N�g�̍X�V
	/// </summary>
	static void UpdateALL();
	/// <summary>
	/// �S�ẴI�u�W�F�N�g�̕`��
	/// </summary>
	static void RenderALL();

	/// <summary>
	/// enemy�ȊO�̑S�ẴI�u�W�F�N�g�̕`��
	/// </summary>
	static void NoEnemyRenderALL();
	static void LateRenderALL();
	static void LateUpdateALL();
	/// <summary>
	/// �S�ẴI�u�W�F�N�g�̓����蔻��
	/// </summary>
	static void CollisionALL();
	/// <summary>
	/// �I�u�W�F�N�g��ǉ�
	/// </summary>
	/// <param name="b">�ǉ��I�u�W�F�N�g</param>
	static void Add(Base* b);
	/// <summary>
	/// �I�u�W�F�N�g�̌���
	/// </summary>
	/// <param name="type">�����Ώۂ̎��</param>
	/// <returns></returns>
	static Base* FindObject(BaseType type);
	/// <summary>
	/// �I�u�W�F�N�g�̌���(�z��Ŏ擾)
	/// </summary>
	/// <param name="type">�����Ώۂ̎��</param>
	/// <returns></returns>
	static std::vector<Base*> FindObjects(BaseType type);
	//�t�����h�N���X
	friend class Base;
};
