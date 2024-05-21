#include "TaskManeger.h"
//Taskのリスト
std::list<Task*> TaskManeger::m_taskList;
//TaskManegerのインスタンス
TaskManeger* TaskManeger::ms_instance = nullptr;

TaskManeger::~TaskManeger()
{
	//Taskの削除処理
	TaskList::iterator it = m_taskList.begin();
	while (it != m_taskList.end()) {
		//Taskの状態を取得
		Task* deleteTask = *it;

		//次のタスクへ進行しておく
		it = m_taskList.erase(it);

		//削除フラグが立っていたら、タスクを削除
		//（リストからのRemove処理はTaskのデストラクタで呼ばれる）
		delete deleteTask;
	}
	m_taskList.clear();
}

TaskManeger* TaskManeger::Instance()
{
	//インスタンスが存在しなければ、新しく生成する
	if (ms_instance == nullptr) {
		ms_instance = new TaskManeger();
	}
	return ms_instance;
}

//インスタンス破棄
void TaskManeger::ClearInstance(){
	//インスタンスが生成されていなければ、破棄する
	if (ms_instance == nullptr)return;
	delete ms_instance;
	ms_instance = nullptr;
}

void TaskManeger::Add(Task* task){
	TaskList::iterator it = m_taskList.begin();
	while (it != m_taskList.end()) {
		//追加するタスクが検索中のタスクより
		//処理順序が先ならば、その位置に追加
		if (task->GetType() < (*it)->GetType()) {
			m_taskList.insert(it, task);
			return;
		}
		//処理順序が後ならば、その位置に追加
		it++;
	}
	//追加するタスクより
	//処理順序が後のタスクが見つからない場合は
	//リストの最後尾に追加
	m_taskList.push_back(task);
}

//Taskをリストから取り除く
void TaskManeger::Remove(Task* task){
	m_taskList.remove(task);
}

void TaskManeger::KillALL(){
	//全ての削除フラグをONにする
	for (auto& b : m_taskList) {
		if (b->m_dontKill) continue;
		b->Kill();
	}
}

//全Taskを更新
void TaskManeger::Update(){
	//Taskの削除処理
	TaskList::iterator it = m_taskList.begin();
	while (it != m_taskList.end()) {
		//Taskの状態を取得
		Task* deleteTask = *it;
		bool isDelete = deleteTask->IsKill();
		
		//次のタスクへ進行しておく
		it++;

		//削除フラグが立っていたら、タスクを削除
		//（リストからのRemove処理はTaskのデストラクタで呼ばれる）
		if (isDelete) {
			delete deleteTask;
		}
	}

	//全TaskのUpdate関数を呼び出す
	for (Task* task : m_taskList) {
		task->Update();
	}
}

//全Taskを描画
void TaskManeger::Render(){
	//全TaskのRender関数を呼び出す
	for (Task* task : m_taskList) {
		if (task->GetType() >= TaskType::eStart2D)break;
		task->Render();
	}
}

//全Taskの2Dを描画
void TaskManeger::Draw()
{
	//全TaskのRender関数を呼び出す
	for (Task* task : m_taskList) {
		if (task->GetType() < TaskType::eStart2D)continue;
		task->Draw();
	}
}

void TaskManeger::Collision(){
	TaskList::iterator it = m_taskList.begin();
	//末尾まで繰り返す
	while (it != m_taskList.end()) {
		if (!(*it)->m_isKill) {
			auto ct = it;
			ct++;
			while (ct != m_taskList.end()) {
				if (!(*ct)->m_isKill) {
					(*it)->Collision(*ct);
					(*ct)->Collision(*it);
				}
				//次のオブジェクト
				ct++;
			}
		}
		//次のオブジェクト
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

//描画後の2Dを描画
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
