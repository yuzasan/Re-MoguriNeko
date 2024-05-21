#pragma once
//#include "../Base/Base.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class Item : public Task {
private:
	Task* mp_player;
	//箱のサイズ
	CVector3D m_itemSize;

public:
	Item(const CVector3D& pos, const CVector3D& rot, const CVector3D& size);
	~Item();

	void Update();
	void Render();
	/// <summary>
	/// enemy以外の描画処理
	/// </summary>
	void NoEnemyRender();
	void Collision(Task* b);

};

class Item2 : public Task {
private:
	Task* mp_player;
	//箱のサイズ
	CVector3D m_itemSize;

public:
	Item2(const CVector3D& pos, const CVector3D& rot, const CVector3D& size);
	~Item2();

	void Update();
	void Render();
	/// <summary>
	/// enemy以外の描画処理
	/// </summary>
	void NoEnemyRender();
	void Collision(Task* b);

};

class LastItem : public Task {
private:
	//箱のサイズ
	CVector3D m_itemSize;

public:
	LastItem(const CVector3D& pos, const CVector3D& rot, const CVector3D& size);
	~LastItem();

	void Update();
	void Render();
	/// <summary>
	/// enemy以外の描画処理
	/// </summary>
	void NoEnemyRender();
	void Collision(Task* b);

};