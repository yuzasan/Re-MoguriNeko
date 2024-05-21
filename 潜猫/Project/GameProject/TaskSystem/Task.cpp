#include "Task.h"
#include "TaskManeger.h"

//コンストラクタ
Task::Task(TaskType type, bool dontKill)
	: m_type(type)
	, m_isKill(false)
	, m_dontKill(dontKill)
	, m_attack_no(0)
	, m_hit_no(0)
	, m_isFind(false)
	, m_isFindplayer(false)
	, m_isGround(true)
	, m_isHide(false)
	, m_pos(0.0f, 0.0f, 0.0f)
	, m_rot(0.0f, 0.0f, 0.0f)
	, m_dir(0.0f, 0.0f, 1.0f)
	, m_vec(0.0f, 0.0f, 0.0f)
	, m_rad(0.0f)
	, m_time(0.0f)
{
	TaskManeger::Instance()->Add(this);
}

//デストラクタ
Task::~Task(){
	TaskManeger::Instance()->Remove(this);
}

//タスクの種類を取得
TaskType Task::GetType() const{
	return m_type;
}

//オブジェクトが持つモデルの取得
CModel* Task::GetModel()
{
	return nullptr;
}

//削除フラグ取得
bool Task::IsKill() const{
	return m_isKill;
}

//タスクを削除
void Task::Kill(){
	m_isKill = true;
}

//更新
void Task::Update() {

}

//描画
void Task::Render() {

}

//2D描画
void Task::Draw() {

}

//当たり判定
void Task::Collision(Task* b) {

}

//Enemy以外描画
void Task::NoEnemyRender(){

}

//描画後に描画
void Task::LateRender(){

}

//描画後に2D描画
void Task::LateDraw() {

}

//描画後に更新
void Task::LateUpdate(){

}
