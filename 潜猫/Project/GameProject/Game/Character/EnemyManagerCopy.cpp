#include "EnemyManagerCopy.h"
#include "../../Debug/DebugPrint.h"
#include "../Gamedata/GameData.h"

//�G�Ǘ��N���X�̃C���X�^���X
EnemyManagerC* EnemyManagerC::ms_instance = nullptr;
//�G�̒T���ʒu�m�[�h�̃��X�g
std::list<SearchNodeC> EnemyManagerC::ms_searchNodes =
{
	//2�K
	SearchNodeC(CVector3D(-19,18,4)),
	//SearchNodeC(CVector3D(-19,18,35)),

	SearchNodeC(CVector3D(-27,18,6)),

	//SearchNodeC(CVector3D(-47,18,4)),
	//SearchNodeC(CVector3D(-47,18,35)),

	//�e�����̃m�[�h
	
	SearchNodeC(CVector3D(-25,18,-3)),
	SearchNodeC(CVector3D(-37,18,19)),
	SearchNodeC(CVector3D(-41,18,27)),
	SearchNodeC(CVector3D(-48,18,-3)),
	SearchNodeC(CVector3D(-55,18,21)),
	SearchNodeC(CVector3D(-56,18,-3)),
	

};

//�R���X�g���N�^
EnemyManagerC::EnemyManagerC() :Task(TaskType::eEnemyManager)
, m_flg(false)
{
	//TaskManeger::Instance()->Add(this);

}

//�f�X�g���N�^
EnemyManagerC::~EnemyManagerC()
{
	ms_instance = nullptr;
}

//�C���X�^���X���擾
EnemyManagerC* EnemyManagerC::Instance()
{
	//�C���X�^���X���Ȃ����
	if (ms_instance == nullptr) {
		//�C���X�^���X�𐶐�����
		ms_instance = new EnemyManagerC();
	}
	return ms_instance;
}

//�G�����X�g�ɒǉ�
void EnemyManagerC::Add(EnemyC* enemyc)
{
	m_enemies.push_back(enemyc);
}

//�G�����X�g�����菜��
void EnemyManagerC::Remove(EnemyC* enemyc)
{
	m_enemies.remove(enemyc);
}

//���ɒT������m�[�h���擾
SearchNodeC* EnemyManagerC::GetNextSearchNode() const
{
	//�T���m�[�h�̑I������
	//�@���̓G���T�����̃m�[�h�͑I�����Ȃ�
	//�A���݊m������ԍ����m�[�h��I������

	SearchNodeC* ret = nullptr;
	//���X�g�̐�[���珇�Ԃɒ��ׂāA
	//��ԏ����ɋ߂���v����m�[�h���擾����
	for (SearchNodeC& node : ms_searchNodes) {
		//���ɑ��̓G���T�����̃m�[�h�͑ΏۊO
		if (node.enemyc != nullptr) continue;
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
void EnemyManagerC::Update()
{
	for (SearchNodeC& node : ms_searchNodes) {
		node.prob = Utility::Clamp01(node.prob + 0.0005f);
		//node.prob = Utility::Clamp01(node.prob + 0.03 * CFPS::GetDeltaTime());
	}

	if (GameData::m_diecnt == 3 && !m_flg) {
		new EnemyC(CVector3D(-8.0f, 6.0f, 25.0f), 75.0f);
		new EnemyC(CVector3D(6.0f, 3.0f, 24.0f), 5.0f);
		m_flg = true;
	}

}

//�`��
void EnemyManagerC::Render()
{
	if (!g_isRenderDebug) return;

	for (SearchNodeC& node : ms_searchNodes) {
		float per = node.prob;
		CVector3D color;
		if (per > 0.5f) {
			color = CVector3D::Leap
			(
				CVector3D(1.0f, 1.0f, 0.0f),
				CVector3D(0.0f, 0.0f, 1.0f),
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
