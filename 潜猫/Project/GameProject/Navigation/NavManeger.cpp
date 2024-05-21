#include "NavManeger.h"
#include <GLLibrary/Utility.h>
#include "../Game/Stage/Stage.h"
#include "../Debug/DebugPrint.h"

//�o�H�T���̊Ǘ��N���X�̃C���X�^���X
NavManeger* NavManeger::ms_instance = nullptr;

//�C���X�^���X�̎擾
NavManeger* NavManeger::Instance()
{
	//�C���X�^���X���Ȃ����
	if (ms_instance == nullptr) {
		//�C���X�^���X�𐶐�����
		ms_instance = new NavManeger();
	}
	return ms_instance;
}

//�R���X�g���N�^
NavManeger::NavManeger() :Task(TaskType::eNavManeger)
	,m_addNodeCount(0)
	,m_isRender(false)
	,m_routeStartNode(nullptr)
{

}

//�f�X�g���N�^
NavManeger::~NavManeger()
{
	std::list<NavNode*>::iterator it = m_nodes.begin();
	while (it != m_nodes.end()) {
		NavNode* del = *it;
		it++;
		delete del;
	}
	m_nodes.clear();
	ms_instance = nullptr;
}

//�S�m�[�h�̏�Ԃ����Z�b�g
void NavManeger::Reset()
{
	for (NavNode* node : m_nodes) {
		node->Reset();
	}
}

//�o�H�T���p�̃m�[�h��ǉ�
void NavManeger::AddNode(NavNode* node)
{
	node->m_no = m_addNodeCount++;
	m_nodes.push_back(node);
}

//�o�H�T���p�̃m�[�h����菜��
void NavManeger::RemoveNode(NavNode* node)
{
	m_nodes.remove(node);
	for (NavNode* n : m_nodes) {
		n->RemoveConnect(node);
	}
	Reset();
}

//�w�肵���m�[�h����m�[�h�܂ł̌o�H��T��
NavNode* NavManeger::Navigate(NavNode* start, NavNode* goal)
{
	if (start == nullptr || goal == nullptr)return nullptr;

	//�m�[�h�̏��i�ړ��R�X�g��o�H��̎��̃m�[�h�̃|�C���^�j�����Z�b�g
	Reset();

	std::list<NavNode*> work1;
	std::list<NavNode*> work2;

	std::list<NavNode*>* currLevel = &work1;
	std::list<NavNode*>* nextLevel = &work2;
	std::list<NavNode*>* for_swap;

	//�ړI�n���珇�ԂɃR�X�g���������邽�߁A
	//�ŏ��̌������X�g�ɂ͖ړI�n�̃m�[�h��ǉ�
	goal->m_cost = 0.0f;
	currLevel->push_back(goal);

	std::list<NavNode*>::iterator itr;
	float nodeCost;
	//�������X�g�̍Ō�܂Ń��[�v����
	while (currLevel->size()) {
		for (itr = currLevel->begin(); itr != currLevel->end(); itr++) {
			//�������̃m�[�h�̐ڑ����X�g��S�Ē��ׂ�
			for (NavNode::ConnectData& connect : (*itr)->m_connects) {
				//��������m�[�h�̎�ނ�
				//�ړI�n�Ƃ��Ă̂ݎg�p�ł���m�[�h�������ꍇ�́A
				//�ړI�n�܂ł̒ʉߒn�_�Ƃ��Ĉ���Ȃ����߁A
				//�ړ��R�X�g�̌v�Z�͂��Ȃ�
				NavNode::NodeType type = connect.node->m_type;
				if (connect.node != start && connect.node != goal
					&& type == NavNode::NodeType::Destination)
				{
					continue;
				}

				//�ړI�n���玩�g�̃m�[�h�܂ł̈ړ��R�X�g��
				//���g�̃m�[�h����ڑ���̃m�[�h�܂ł̈ړ��R�X�g�����Z���āA
				//�ړI�n����ڑ���̃m�[�h�܂łɊ|����R�X�g�����߂�
				nodeCost = (*itr)->m_cost + connect.cost;

				//����̒T���������́A���ɓo�^����Ă���ړ��R�X�g��菭�Ȃ��Ȃ�΁A
				if (connect.node->m_cost < 0.0f ||
					nodeCost < connect.node->m_cost)
				{
					//�ړ��R�X�g�ƈړ����̃m�[�h��ݒ�
					connect.node->m_cost = nodeCost;
					connect.node->m_nextNodeToGoal = (*itr);
				}
				else {
					continue;
				}

				//���̌������X�g�ɐڑ���̃m�[�h��ǉ�
				nextLevel->push_back(connect.node);
			}
		}

		//�������X�g�����ւ���
		for_swap = currLevel;
		currLevel = nextLevel;
		nextLevel = for_swap;
		nextLevel->clear();
	}

	//���[�g�̊J���m�[�h���L�����Ă���
	m_routeStartNode = start;

	//�ړI�n�̃m�[�h�܂ňړ����邽�߂̌o�H��
	//���Ɉړ�����m�[�h��Ԃ�
	return start->m_nextNodeToGoal;
}

//�w�肳�ꂽ���W�Ɉ�ԋ߂��m�[�h���擾
NavNode* NavManeger::GetNearNavNode(const CVector3D& pos)
{
	//��ԋ߂��m�[�h���Q�Ƃ���|�C���^�[
	NavNode* nearNode = nullptr;
	//��ԋ߂��m�[�h�܂ł̋������i�[����ϐ�
	float nearDist = 0.0f;
	//�S�Ẵm�[�h�̋������v��
	for (NavNode* node : m_nodes) {
		//�ړI�n�p�̃m�[�h�͌����ΏۂɂȂ�Ȃ�
		if (node->m_type == NavNode::NodeType::Destination) {
			continue;
		}

		//�m�[�h���m�̋������擾
		float dist = (pos - node->m_pos).Length();
		if (dist < nearDist || nearNode == nullptr) {
			nearNode = node;
			nearDist = dist;
		}
	}
	return nearNode;
}

//�w�肳�ꂽ���W�Ɉ�ԋ߂��m�[�h���擾
NavNode* NavManeger::GetNearNavNode(NavNode* node)
{
	CVector3D pos = node->GetPos();

	//��ԋ߂��m�[�h���Q�Ƃ���|�C���^�[
	NavNode* nearNode = nullptr;
	//��ԋ߂��m�[�h�܂ł̋������i�[����ϐ�
	float nearDist = 0.0f;
	//�S�Ẵm�[�h�̋������v��
	for (NavNode* findNode : m_nodes) {
		//���ׂ�m�[�h�Ɠ����ꍇ�́A�X���[
		if (node == findNode)continue;
		//�ړI�n�p�̃m�[�h�͌����Ώۂɂ͂Ȃ�Ȃ�
		if (node->m_type == NavNode::NodeType::Destination) {
			continue;
		}

		//�m�[�h���m�̋������擾
		float dist = (pos - findNode->m_pos).Length();
		//���ݒ肳��Ă���m�[�h���߂��܂��́A��ԍŏ��̃m�[�h�ł���΁A
		//��ԋ߂��m�[�h�Ƃ��ēo�^
		if (dist < nearDist || nearNode == nullptr) {
			nearNode = findNode;
			nearDist = dist;
		}
	}

	//�擾������ԋ߂��m�[�h��Ԃ�
	return nearNode;
}

//�ڑ��ł���m�[�h������
int NavManeger::FindConnectNavNodes(NavNode* node, float distance)
{
	//���݂̐ڑ���̏����N���A�ɂ��Ă���
	node->ClearConnects();

	for (NavNode* n : m_nodes) {
		//�������g�̓X���[
		if (n == node) continue;
		
		//�w�肳�ꂽ�����͈͓̔�������
		float dist = (n->m_pos - node->m_pos).Length();
		if (dist > distance) continue;

		//�ԂɎՕ��������݂��Ȃ�������
		CVector3D hitPos, hitNormal;
		CVector3D start = node->m_pos;
		CVector3D end = n->m_pos;
		/*start.y = 1.0f;
		end.y = 1.0f;*/
		//�܂����C���΂��āA�����Ƀq�b�g���邩�ǂ���
		if (Stage::CollisionRay(start, end, &hitPos, &hitNormal)) {
			//�����Ƀq�b�g�����ꍇ�́A�q�b�g�ʒu�܂ł̋����ƁA
			//�J�n�n�_����I���n�_�܂ł̋����𑪂�A
			//�q�b�g�ʒu�܂ł̋����̕����������ꍇ�́A
			//����̃m�[�h���Օ����ɉB��Ă��邽�߁A�X���[����
			float hitDist = (hitPos - start).Length();
			if (dist > hitDist)continue;
		}
		//�����̏����𖞂������m�[�h��ڑ����X�g�ɒǉ�
		node->AddConnect(n);
	}

	return node->m_connects.size();
}

//�S�m�[�h��`��
void NavManeger::Render()
{
	if (!g_isRenderDebug)return;
	//���X�g���m�[�h��S�ĕ`��
	for (NavNode* node : m_nodes) {
		node->Render();
	}

	glDisable(GL_DEPTH_TEST);

	//�n�ʂƏd�Ȃ�Ȃ��悤�ɏ�����ɏグ��l
	float offsetY = 0.01f;
	NavNode* current = m_routeStartNode;
	while (current != nullptr) {
		NavNode* next = current->m_nextNodeToGoal;
		if (next == nullptr) break;

		//���[�g��̃m�[�h���ǂ����ŐF�𕪂���
		CVector4D lineColor = CVector4D(0.25f, 1.0f, 1.0f, 1.0f);
		float lineWidth = 4.0f;
		//���C����`��
		Utility::DrawLine
		(
			current->m_pos + CVector3D(0.0f, offsetY, 0.0f),
			next->m_pos + CVector3D(0.0f, offsetY, 0.0f),
			lineColor,
			lineWidth
		);

		//���̃m�[�h��
		current = next;
	}

	glEnable(GL_DEPTH_TEST);
}
