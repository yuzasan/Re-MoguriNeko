#include "EnemyManager.h"
#include "../../Debug/DebugPrint.h"
#include "../Gamedata/GameData.h"

//�G�Ǘ��N���X�̃C���X�^���X
EnemyManager* EnemyManager::ms_instance = nullptr;
//�G�̒T���ʒu�m�[�h�̃��X�g
std::list<SearchNode> EnemyManager::ms_searchNodes =
{
	SearchNode(CVector3D(-8,6,51)),
	SearchNode(CVector3D(-57,6,51)),
	SearchNode(CVector3D(-57,6,-2)),
	SearchNode(CVector3D(-13,6,-2)),

	//SearchNode(CVector3D(-17,17,-4)),

	//--------------------------------
	
	//SearchNode(CVector3D(-8,6,12)),
	//SearchNode(CVector3D(-8,6,24)),
	
	SearchNode(CVector3D(-14,6,5)),
	//SearchNode(CVector3D(-14,6,23)),
	SearchNode(CVector3D(-14,6,43)),

	//SearchNode(CVector3D(-20,6,11)),
	//SearchNode(CVector3D(-20,6,23)),
	//SearchNode(CVector3D(-20,6,36)),

	//SearchNode(CVector3D(-34,6,-2)),
	//SearchNode(CVector3D(-34,6,5)),
	//SearchNode(CVector3D(-34,6,11)),
	//SearchNode(CVector3D(-34,6,36)),
	//SearchNode(CVector3D(-34,6,43)),
	//SearchNode(CVector3D(-34,6,50)),

	//SearchNode(CVector3D(-45,6,36)),
	//SearchNode(CVector3D(-45,6,23)),
	//SearchNode(CVector3D(-45,6,11)),

	SearchNode(CVector3D(-52,6,5)),
	//SearchNode(CVector3D(-52,6,23)),
	SearchNode(CVector3D(-52,6,36)),

	//SearchNode(CVector3D(-58,6,23)),


	//�O
	//SearchNode(CVector3D(-8,18,-5)),


	//2�K
	//SearchNode(CVector3D(-8, 18, -3)),
	//SearchNode(CVector3D(13, 1, 32)),
	//SearchNode(CVector3D(-19,18,4)),
	//SearchNode(CVector3D(-19,18,-4)),
	//SearchNode(CVector3D(-19,18,35)),

	//SearchNode(CVector3D(-27,18,6)),

	//SearchNode(CVector3D(-47,18,4)),
	//SearchNode(CVector3D(-47,18,35)),

	
	
	
	

	
};

//�R���X�g���N�^
EnemyManager::EnemyManager():Task(TaskType::eEnemyManager)
	,m_flg(false)
{
	//TaskManeger::Instance()->Add(this);
	
}

//�f�X�g���N�^
EnemyManager::~EnemyManager()
{
	ms_instance = nullptr;
}

//�C���X�^���X���擾
EnemyManager* EnemyManager::Instance()
{
	//�C���X�^���X���Ȃ����
	if (ms_instance == nullptr) {
		//�C���X�^���X�𐶐�����
		ms_instance = new EnemyManager();
	}
	return ms_instance;
}

//�G�����X�g�ɒǉ�
void EnemyManager::Add(Enemy* enemy)
{
	m_enemies.push_back(enemy);
}

//�G�����X�g�����菜��
void EnemyManager::Remove(Enemy* enemy)
{
	m_enemies.remove(enemy);
}

//���ɒT������m�[�h���擾
SearchNode* EnemyManager::GetNextSearchNode() const
{
	//�T���m�[�h�̑I������
	//�@���̓G���T�����̃m�[�h�͑I�����Ȃ�
	//�A���݊m������ԍ����m�[�h��I������

	SearchNode* ret = nullptr;
	//���X�g�̐�[���珇�Ԃɒ��ׂāA
	//��ԏ����ɋ߂���v����m�[�h���擾����
	for (SearchNode& node : ms_searchNodes) {
		//���ɑ��̓G���T�����̃m�[�h�͑ΏۊO
		if (node.enemy != nullptr) continue;
		//�߂�l�̃m�[�h���܂��ݒ肳��Ă��Ȃ��������́A
		//���݊m�����������̃m�[�h�̕��������̂ł���΁A
		//�������̃m�[�h��D�悷��
		if (ret == nullptr || node.prob > ret->prob) {
			ret = &node;
		}
	}
	return ret;
}

//�X�V
void EnemyManager::Update()
{
	for (SearchNode& node : ms_searchNodes) {
		node.prob = Utility::Clamp01(node.prob + 0.0005f);
		//node.prob = Utility::Clamp01(node.prob + 0.03 * CFPS::GetDeltaTime());
	}

	if (GameData::m_diecnt == 3 && !m_flg) {
		new Enemy(CVector3D(-8.0f, 6.0f, 25.0f), 75.0f);
		new Enemy(CVector3D(6.0f, 3.0f, 24.0f), 5.0f);
		m_flg = true;
	}
	
}

//�`��
void EnemyManager::Render()
{
	if (!g_isRenderDebug) return;

	for (SearchNode& node : ms_searchNodes) {
		float per = node.prob;
		CVector3D color;
		if (per > 0.5f) {
			color = CVector3D::Leap
			(
				CVector3D(1.0f, 1.0f, 0.0f),
				CVector3D(1.0f, 0.0f, 0.0f),
				(per - 0.5f) / 0.5f
			);
		}
		else {
			color = CVector3D::Leap
			(
				CVector3D(0.0f, 1.0f, 0.0f),
				CVector3D(1.0f, 1.0f, 0.0f),
				per / 0.5f
			);
		}
		Utility::DrawCube(node.pos, 2.0f, CVector4D(color, 1.0f));
	}
}
