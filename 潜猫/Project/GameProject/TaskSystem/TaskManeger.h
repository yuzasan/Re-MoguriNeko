#pragma once
#include "Task.h"
#include <list>

class TaskManeger {
private:
	//TaskManegerのインスタンス
	static TaskManeger* ms_instance;

	//Taskのリスト
	using TaskList = std::list<Task*>;
	//生成されているTaskのリスト
	static TaskList m_taskList;

public:
	//デストラクタ
	~TaskManeger();

	//インスタンス取得
	static TaskManeger* Instance();
	//インスタンス破棄
	static void ClearInstance();

	
	//Taskをリストに追加
	void Add(Task* task);
	//Taskをリストから取り除く
	void Remove(Task* task);
	
	//全TaskをKill
	static void KillALL();
	//全Taskを更新
	void Update();
	//全Taskを描画
	void Render();
	//全Taskの2Dを描画
	void Draw();
	//全当たり判定
	void Collision();
	//全Enemy以外描画
	void NoEnemyRender();
	//更新後全描画
	void LateRender();
	//描画後2Dを描画
	void LateDraw();
	//更新後全更新
	void LateUpdate();

	//オブジェクトの検索
	static Task* FindObject(TaskType type);
	//オブジェクトの検索(配列で取得)
	static std::vector<Task*> FindObjects(TaskType type);
};