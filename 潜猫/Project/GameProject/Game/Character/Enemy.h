#pragma once
#include "CharaBase.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class Player;
class NavNode;
class SearchNode;
class NoiseBox;

class Enemy : public CharaBase {
	//視野の角度
	float view_ang = DtoR(35);
	//視野の距離
	float view_length = 5;
public:
	/*struct CourseData {
		CVector3D * points;
		int size;
	};*/
private:
	Player* mp_player;				//プレイヤー
	NoiseBox* mp_noise;				//物音BOX
	Task* mp_field;					//ステージ
	CModelA3M m_model;				//モデル
	CModelObj m_modelobj;				//モデル
	CVector3D m_moveDir;			//移動方向
	CVector3D m_movePos;			//移動先座標
	NavNode* m_moveNode;			//移動目的地のノード
	NavNode* m_nextNode;			//次に移動するノードの格納用
	NavNode* m_lostNode;			//プレイヤーを見失った場所に配置したノード
	SearchNode* m_searchNode;		//探索中のノード
	CVector4D color;
	float m_elapsedTime;			//待機経過時間
	CVector3D m_pos_old;			//前の位置
	float m_emotion;				//感情
	
	CVector3D m_rot_target;
	//const CourseData* mp_cource;	//通る道
	//int		m_cource_idx;

	//アニメーション番号
	enum class AnimId
	{
		Idle,			//ノーマル待機
		Walk,			//ノーマル歩き
		HappyIdle,		//うれしい待機
		HappyIdle2,		//うれしい待機2
		HappyWalk,		//うれしい歩き
		BackSearch,		//後ろ警戒
		BackSearch2,	//後ろ警戒2
		WarningIdle,	//警戒待機
		WarningIdle2,	//警戒待機2
		WarningSearch,	//全方位警戒
		CrossIdle,		//イライラ待機
		CrossIdle2,		//イライラ待機2
		CrossWalk,		//イライラ歩き
		CrossWalk2,		//イライラ歩き2
		Run,			//全力走り
		Run2,			//走り
		SadIdle,		//落ち込み待機
		SadWalk,		//落ち込み歩き
		SleepIdle,		//眠り待機
		Sleeping,		//眠り
	};

	//状態
	enum class State
	{
		Normal,		//通常
		Happy,		//うれしい
		Warning,	//警戒
		Cross,		//イライラ
		Sad,		//落ち込み
		Sleep,		//眠り
	};
	State m_state;	//現在の状態

	//行動
	enum class Action
	{
		Idle,			//待機
		Move,			//移動
		Search,			//警戒
		Chase,			//追跡
		Lost,			//プレイヤーを見失った
		/*
		HappyIdle,		//うれしい待機
		HappyWalk,		//うれしい歩き
		BackSearch,		//後ろ警戒
		WarningIdle,	//警戒待機
		CrossIdle,		//イライラ待機
		CrossWalk,		//イライラ歩き
		SadIdle,		//落ち込み待機
		SadWalk,		//落ち込み歩き
		SleepIdle,		//眠り待機
		Sleeping,		//眠り
		*/
	};
	Action m_action;	//現在の行動状態

	//通常行動の処理
	void ActionIdle();
	//移動行動の処理
	void ActionMove();
	//警戒行動の処理
	void ActionSearch();
	//追跡行動の処理
	void ActionChase();
	//プレイヤーを見失った状態の処理
	void ActionLost();

public:
	bool isFoll;
	int m_cntfoll;
	bool m_isTouch;
	bool m_isWall;
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">出現位置</param>
	Enemy(const CVector3D& pos,float emotion);
	//デストラクタ
	~Enemy();
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

	//現在位置からプレイヤーが見つけたかどうか
	bool IsLookPlayer() const;

	//指定座標への移動処理
	bool MoveTo(const CVector3D& target);

	//探索ノードを設定
	void SetSearchNode(SearchNode* node);
	//探索中のノードを取得
	SearchNode* GetSearchNode() const;

};