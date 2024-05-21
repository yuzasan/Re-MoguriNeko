#pragma once
#include "Task.h"
#include <list>

class TaskManeger {
private:
	//TaskManeger�̃C���X�^���X
	static TaskManeger* ms_instance;

	//Task�̃��X�g
	using TaskList = std::list<Task*>;
	//��������Ă���Task�̃��X�g
	static TaskList m_taskList;

public:
	//�f�X�g���N�^
	~TaskManeger();

	//�C���X�^���X�擾
	static TaskManeger* Instance();
	//�C���X�^���X�j��
	static void ClearInstance();

	
	//Task�����X�g�ɒǉ�
	void Add(Task* task);
	//Task�����X�g�����菜��
	void Remove(Task* task);
	
	//�STask��Kill
	static void KillALL();
	//�STask���X�V
	void Update();
	//�STask��`��
	void Render();
	//�STask��2D��`��
	void Draw();
	//�S�����蔻��
	void Collision();
	//�SEnemy�ȊO�`��
	void NoEnemyRender();
	//�X�V��S�`��
	void LateRender();
	//�`���2D��`��
	void LateDraw();
	//�X�V��S�X�V
	void LateUpdate();

	//�I�u�W�F�N�g�̌���
	static Task* FindObject(TaskType type);
	//�I�u�W�F�N�g�̌���(�z��Ŏ擾)
	static std::vector<Task*> FindObjects(TaskType type);
};