#pragma once
#include "CharaBase.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class PlayerCamera;
class Enemy;
class FellBox;

class Player : public CharaBase {
private:
	CModelA3M m_model;			//モデルデータ
	CVector3D m_moveDir;		//移動方向
	CVector3D m_rot_target;
	//Task* mp_camera;
	PlayerCamera* mp_camera;
	Enemy* mp_enemy;
	Task* mp_isenemy;
	Task* mp_tyukan;
	Task* mp_fellbox;
	float m_viewAngle;			//視野角度
	float m_viewLength;			//視野距離
	int m_gauge;				//色
	CVector3D m_tyukan;			//中間座標
	float m_elapsedTime;		//待機経過時間
	CVector4D color;
	bool m_enemyflg;

	//アニメーション番号
	enum class AnimId 
	{
		Idle,	//待機
		Walk,	//歩き
		Jump,	//ジャンプ
		Naku,	//鳴く
		Eat,	//食べる
	};

	//状態
	enum class State
	{
		Move,		//移動状態
		Invisible,	//透明状態	
	};
	State m_state;	//現在の状態

	//移動状態の処理
	void StateMove();
	//透明状態の処理
	void StateInvisible();

public:
	bool m_isSearch;	//探索範囲にいるかどうか
	bool m_isBark;			//鳴いているかどうか
	//bool m_isFind;
	Player(const CVector3D& pos);
	~Player();
	void Update();
	void Render();
	/// <summary>
	/// enemy以外の描画処理
	/// </summary>
	void NoEnemyRender();
	// 敵を見つけたかどうか
	bool IsFoundEnemy(Enemy* e) const;
	//現在地から敵がいるか
	bool IsSearchEnemy(Enemy* e) const;
	void Shot(Enemy* e);
	//デバッグ処理
	void Debug();
	void Collision(Task* b);
	void LateUpdate();
};