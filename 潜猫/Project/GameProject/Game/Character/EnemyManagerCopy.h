#pragma once
//#include "../Base/Base.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
#include "EnemyCopy.h"
class SearchNodeC;

class EnemyManagerC : public Task {
private:
	//�G�̊Ǘ��N���X�̃C���X�^���X
	static EnemyManagerC* ms_instance;
	//�G�̒T���ʒu�m�[�h�̃��X�g
	static std::list<SearchNodeC> ms_searchNodes;

	//�G�̃��X�g
	std::list<EnemyC*> m_enemies;

	bool m_flg;

public:
	//�R���X�g���N�^
	EnemyManagerC();
	//�f�X�g���N�^
	~EnemyManagerC();

	//�C���X�^���X���擾
	static EnemyManagerC* Instance();

	//�G�����X�g�ɒǉ�
	void Add(EnemyC* enemyc);
	//�G�����X�g�����菜��
	void Remove(EnemyC* enemyc);

	//���ɒT������m�[�h���擾
	SearchNodeC* GetNextSearchNode() const;

	//�X�V
	void Update();

	//�`��
	void Render();
};

//�T������m�[�h���
class SearchNodeC {
public:
	CVector3D pos;	//�m�[�h�ʒu
	float prob;		//���݊m��
	EnemyC* enemyc;	//���ݒT���Ɍ������Ă���G
	SearchNodeC(const CVector3D& p)
		:pos(p)
		, prob(1.0f)
		, enemyc(nullptr) {}
};