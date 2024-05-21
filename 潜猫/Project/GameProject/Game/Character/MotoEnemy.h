#pragma once
//#include "../Base/Base.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class Player;

class Enemy : public Task {
	//視野の角度
	float view_ang = DtoR(35);
	//視野の距離
	float view_length = 5;
public:
	struct CourseData {
		CVector3D* points;
		int size;
	};

private:
	Player* mp_player;
	Task* mp_field;
	//モデルオブジェクト
	CModelA3M m_model;
	CVector3D m_rot_target;
	const CourseData* mp_cource;	//通る道
	int		m_cource_idx;
	CVector4D color;
	float m_elapsedTime;	//待機経過時間
	CVector3D m_pos_old;

	//アニメーション番号
	enum class AnimId
	{
		Idle,	//待機
		Walk,	//歩き
	};

	//状態
	enum class State
	{
		Idle,	//待機
		Move,	//移動
		Chase,	//追跡
	};
	State m_state;	//現在の状態

	//待機状態の処理
	void StateIdle();
	//移動状態の処理
	void StateMove();
	//追跡状態の処理
	void StateChase();

public:
	bool isFoll;
	int m_cntfoll;
	bool m_isTouch;
	bool m_isWall;
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">出現位置</param>
	Enemy(const CourseData* data);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Render();
	/// <summary>
	/// 当たり判定検証
	/// </summary>
	/// <param name="b">対象物</param>
	void Collision(Task* b);
	/// <summary>
	/// 部位のカプセルと線分との判定
	/// </summary>
	/// <param name="lineS">線分始点</param>
	/// <param name="lineE">線分終点</param>
	/// <param name="dir">線分方向</param>
	/// <param name="cross">接触点</param>
	/// <returns>1なら接触した部位:(0)非接触</returns>
	int CollisionLine(const CVector3D& lineS, const CVector3D& lineE, const CVector3D& dir, CVector3D* cross);
	//プレイヤーを見つけたかどうか
	bool IsFoundPlayer();
	//壁がないか
	void Shot();
	//デバッグ処理
	void Debug();
	//アイテム
	void FollBox();
	void LateRender();
};