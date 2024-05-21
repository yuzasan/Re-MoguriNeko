#pragma once
#include <list>
#include "NavNode.h"
#include "../TaskSystem/Task.h"
#include "../TaskSystem/TaskManeger.h"

//�o�H�T�������̊Ǘ��N���X
class NavManeger : public Task {
private:
	static NavManeger* ms_instance;

	//�T���p�m�[�h�̃��X�g
	std::list<NavNode*> m_nodes;

	//�ǉ������m�[�h�̐�
	int m_addNodeCount;
	//�f�o�b�O�p�F�`�悷�邩�ǂ���
	bool m_isRender;
	//�f�o�b�O�p�F���[�g�̊J�n�m�[�h
	NavNode* m_routeStartNode;

	//�S�m�[�h�̏�Ԃ����Z�b�g
	void Reset();

public:
	static NavManeger* Instance();

	NavManeger();
	~NavManeger();

	//�o�H�T���p�̃m�[�h��ǉ�
	void AddNode(NavNode* node);
	//�o�H�T���p�̃m�[�h����菜��
	void RemoveNode(NavNode* node);

	//�w�肵���m�[�h����m�[�h�܂ł̌o�H��T��
	NavNode* Navigate(NavNode* start, NavNode* goal);

	//�w�肳�ꂽ���W�Ɉ�ԋ߂��m�[�h���擾
	NavNode* GetNearNavNode(const CVector3D& pos);
	//�w�肳�ꂽ�m�[�h�Ɉ�ԋ߂��m�[�h���擾
	NavNode* GetNearNavNode(NavNode* node);

	//�ڑ��ł���m�[�h������
	//�i�ԂɎՕ���������m�[�h�ɂ͐ڑ��ł��Ȃ��j
	int FindConnectNavNodes(NavNode* node, float distance);

	//�S�m�[�h��`��
	void Render();
};