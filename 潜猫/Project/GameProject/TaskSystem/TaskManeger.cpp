#include "TaskManeger.h"
//Task�̃��X�g
std::list<Task*> TaskManeger::m_taskList;
//TaskManeger�̃C���X�^���X
TaskManeger* TaskManeger::ms_instance = nullptr;

TaskManeger::~TaskManeger()
{
	//Task�̍폜����
	TaskList::iterator it = m_taskList.begin();
	while (it != m_taskList.end()) {
		//Task�̏�Ԃ��擾
		Task* deleteTask = *it;

		//���̃^�X�N�֐i�s���Ă���
		it = m_taskList.erase(it);

		//�폜�t���O�������Ă�����A�^�X�N���폜
		//�i���X�g�����Remove������Task�̃f�X�g���N�^�ŌĂ΂��j
		delete deleteTask;
	}
	m_taskList.clear();
}

TaskManeger* TaskManeger::Instance()
{
	//�C���X�^���X�����݂��Ȃ���΁A�V������������
	if (ms_instance == nullptr) {
		ms_instance = new TaskManeger();
	}
	return ms_instance;
}

//�C���X�^���X�j��
void TaskManeger::ClearInstance(){
	//�C���X�^���X����������Ă��Ȃ���΁A�j������
	if (ms_instance == nullptr)return;
	delete ms_instance;
	ms_instance = nullptr;
}

void TaskManeger::Add(Task* task){
	TaskList::iterator it = m_taskList.begin();
	while (it != m_taskList.end()) {
		//�ǉ�����^�X�N���������̃^�X�N���
		//������������Ȃ�΁A���̈ʒu�ɒǉ�
		if (task->GetType() < (*it)->GetType()) {
			m_taskList.insert(it, task);
			return;
		}
		//������������Ȃ�΁A���̈ʒu�ɒǉ�
		it++;
	}
	//�ǉ�����^�X�N���
	//������������̃^�X�N��������Ȃ��ꍇ��
	//���X�g�̍Ō���ɒǉ�
	m_taskList.push_back(task);
}

//Task�����X�g�����菜��
void TaskManeger::Remove(Task* task){
	m_taskList.remove(task);
}

void TaskManeger::KillALL(){
	//�S�Ă̍폜�t���O��ON�ɂ���
	for (auto& b : m_taskList) {
		if (b->m_dontKill) continue;
		b->Kill();
	}
}

//�STask���X�V
void TaskManeger::Update(){
	//Task�̍폜����
	TaskList::iterator it = m_taskList.begin();
	while (it != m_taskList.end()) {
		//Task�̏�Ԃ��擾
		Task* deleteTask = *it;
		bool isDelete = deleteTask->IsKill();
		
		//���̃^�X�N�֐i�s���Ă���
		it++;

		//�폜�t���O�������Ă�����A�^�X�N���폜
		//�i���X�g�����Remove������Task�̃f�X�g���N�^�ŌĂ΂��j
		if (isDelete) {
			delete deleteTask;
		}
	}

	//�STask��Update�֐����Ăяo��
	for (Task* task : m_taskList) {
		task->Update();
	}
}

//�STask��`��
void TaskManeger::Render(){
	//�STask��Render�֐����Ăяo��
	for (Task* task : m_taskList) {
		if (task->GetType() >= TaskType::eStart2D)break;
		task->Render();
	}
}

//�STask��2D��`��
void TaskManeger::Draw()
{
	//�STask��Render�֐����Ăяo��
	for (Task* task : m_taskList) {
		if (task->GetType() < TaskType::eStart2D)continue;
		task->Draw();
	}
}

void TaskManeger::Collision(){
	TaskList::iterator it = m_taskList.begin();
	//�����܂ŌJ��Ԃ�
	while (it != m_taskList.end()) {
		if (!(*it)->m_isKill) {
			auto ct = it;
			ct++;
			while (ct != m_taskList.end()) {
				if (!(*ct)->m_isKill) {
					(*it)->Collision(*ct);
					(*ct)->Collision(*it);
				}
				//���̃I�u�W�F�N�g
				ct++;
			}
		}
		//���̃I�u�W�F�N�g
		it++;
	}
}

void TaskManeger::NoEnemyRender(){
	for (Task* task : m_taskList) {
		task->NoEnemyRender();
	}
}

void TaskManeger::LateRender(){
	for (Task* task : m_taskList) {
		if (task->GetType() >= TaskType::eStart2D)break;
		task->LateRender();
	}
}

//�`����2D��`��
void TaskManeger::LateDraw()
{
	for (Task* task : m_taskList) {
		if (task->GetType() < TaskType::eStart2D)continue;
		task->LateDraw();
	}
}

void TaskManeger::LateUpdate(){
	for (Task* task : m_taskList) {
		task->LateUpdate();
	}
}

Task* TaskManeger::FindObject(TaskType type){
	for (auto& b : m_taskList) {
		if (b->GetType() == type && !b->m_isKill) return b;
	}
	return nullptr;
}

std::vector<Task*> TaskManeger::FindObjects(TaskType type){
	std::vector<Task*> list;
	for (auto& b : m_taskList) {
		if (b->GetType() == type && !b->m_isKill) list.push_back(b);
	}
	return list;
}
