#include "NavNode.h"
#include "NavManeger.h"
#include <GLLibrary/Utility.h>

//�T���m�[�h�̋���
#define FIND_NODE_DISTANCE 50.0f

//�R���X�g���N�^
NavNode::NavNode(const CVector3D& pos, NodeType type) :Task(TaskType::eNavNode)
	,m_no(0)
	,m_type(type)
	,m_cost(-1.0f)
	,m_nextNodeToGoal(nullptr)
	,m_nodeColor(0.25f,1.0f,0.25f,1.0f)
{
	//���W��ݒ�
	SetPos(pos);
	//�o�H�T���Ǘ��N���X�̃m�[�h���X�g�ɒǉ�
	NavManeger::Instance()->AddNode(this);
}

//�f�X�g���N�^
NavNode::~NavNode()
{
	//�o�H�T���Ǘ��N���X�̃m�[�h���X�g�����菜��
	NavManeger::Instance()->RemoveNode(this);
}

//�m�[�h�̏�Ԃ����Z�b�g
void NavNode::Reset()
{
	m_cost = -1.0f;
	m_nextNodeToGoal = nullptr;
}

//�m�[�h�ԍ����擾
int NavNode::GetNodeNo() const
{
	return m_no;
}

//�m�[�h�̍��W���擾
const CVector3D& NavNode::GetPos() const
{
	return m_pos;
}

//�m�[�h�̍��W��ݒ�
void NavNode::SetPos(const CVector3D& pos)
{
	this->m_pos = pos;
	//���W�ݒ��ɐڑ����Ă���m�[�h�������Ń��X�g�A�b�v����
	NavManeger::Instance()->FindConnectNavNodes(this, FIND_NODE_DISTANCE);
}

//�ڑ����Ă���m�[�h��ǉ�
void NavNode::AddConnect(NavNode* node)
{
	//���ɐڑ����Ă���m�[�h�ł���΁A�X���[
	for (NavNode::ConnectData connect : m_connects) {
		if (connect.node == node)return;
	}

	//�ڑ�����m�[�h�܂ł̋������R�X�g�Ƃ���
	float cost = (node->m_pos - m_pos).Length();

	//���g�Ƒ���̐ڑ��m�[�h���X�g�ɒǉ�
	m_connects.push_back(ConnectData(node, cost));
	node->m_connects.push_back(ConnectData(this, cost));
}

//�ڑ����Ă���m�[�h�����X�g�Œǉ�
void NavNode::AddConnects(std::list<NavNode*> nodes)
{
	for (NavNode* node : nodes) {
		AddConnect(node);
	}
}

//�ڑ����Ă���m�[�h����菜��
void NavNode::RemoveConnect(NavNode* node)
{
	std::list<ConnectData>::iterator it = m_connects.begin();
	while (it != m_connects.end()) {
		if ((*it).node == node) {
			it = m_connects.erase(it);
		}
		else {
			it++;
		}
	}
}

//�ڑ��m�[�h���X�g���N���A
void NavNode::ClearConnects()
{
	for (ConnectData connect : m_connects) {
		connect.node->RemoveConnect(this);
	}
	m_connects.clear();
}

//�f�o�b�O�p�F�`�悷��m�[�h�̐F��ݒ�
void NavNode::SetNodeColor(const CVector3D& color)
{
	m_nodeColor.r = color.r;
	m_nodeColor.g = color.g;
	m_nodeColor.b = color.b;
}

//�f�o�b�O�p�F�m�[�h�̕`�揈��
void NavNode::Render()
{
	if (!g_isRenderDebug)return;
	//�n�ʂƏd�Ȃ�Ȃ��悤�ɏ�����ɏグ��l
	float offsetY = 0.01;
	int nodeNo = m_no;
	//�ڑ���̃m�[�h�܂ł̃��C����`��
	for (NavNode::ConnectData& connect : m_connects) {
		//�m�[�h�ԍ������g�̕����傫���ꍇ�́A
		//����̃m�[�h�̕`�揈���Ń��C����`�悷��
		if (nodeNo >= connect.node->GetNodeNo())continue;

		//���[�g��̃m�[�h���ǂ����ŐF�𕪂���
		CVector4D lineColor = CVector4D(0.125f, 0.25f, 1.0f, 1.0f);
		float lineWidth = 2.0f;
		//���C����`��
		Utility::DrawLine
		(
			m_pos + CVector3D(0.0f, offsetY, 0.0f),
			connect.node->m_pos + CVector3D(0.0f, offsetY, 0.0f),
			lineColor,
			lineWidth
		);
	}

	//�m�[�h�̈ʒu�ɋ���`��
	Utility::DrawSphere
	(
		m_pos + CVector3D(0.0f, offsetY, 0.0f),
		0.5f,
		m_nodeColor
	);
}
