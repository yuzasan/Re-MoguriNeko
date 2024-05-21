#pragma once
//#include "../Base/Base.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
#include "Enemy.h"
class SearchNode;

class EnemyManager: public Task {
private:
	//�G�̊Ǘ��N���X�̃C���X�^���X
	static EnemyManager* ms_instance;
	//�G�̒T���ʒu�m�[�h�̃��X�g
	static std::list<SearchNode> ms_searchNodes;

	//�G�̃��X�g
	std::list<Enemy*> m_enemies;

	bool m_flg;

public:	
	//�R���X�g���N�^
	EnemyManager();
	//�f�X�g���N�^
	~EnemyManager();

	//�C���X�^���X���擾
	static EnemyManager* Instance();

	//�G�����X�g�ɒǉ�
	void Add(Enemy* enemy);
	//�G�����X�g�����菜��
	void Remove(Enemy* enemy);

	//���ɒT������m�[�h���擾
	SearchNode* GetNextSearchNode() const;

	//�X�V
	void Update();

	//�`��
	void Render();
};

//�T������m�[�h���
class SearchNode {
public:
	CVector3D pos;	//�m�[�h�ʒu
	float prob;		//���݊m��
	Enemy* enemy;	//���ݒT���Ɍ������Ă���G
	SearchNode(const CVector3D& p)
		:pos(p)
		, prob(1.0f)
		, enemy(nullptr) {}
};