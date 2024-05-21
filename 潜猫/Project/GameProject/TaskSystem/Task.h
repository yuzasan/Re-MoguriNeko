#pragma once

//タスクの種類
//上から順番に更新、描画される、削除
enum class TaskType {
	//3D
	eCamera,		//メインカメラ
	eField,			//フィールド
	eFieldWatch,	//フィールド
	eFellBox,		//デスゾーン
	eBox,			//すり抜けボックス
	eRotateBox,		//回転ボックス
	eMoveBox,		//移動ボックス
	eHideBox,		//隠れボックス
	eTyukanBox,		//中間ボックス
	eNoiseBox,		//物音ボックス
	eBreakBox,
	eAppearPoint,	//移動点
	ePlayer,		//プレイヤー
	eEnemy,			//敵
	eNavNode,		//ノード
	eEnemyManager,	//敵管理
	ePlayer_Attack,	//プレイヤー攻撃
	eEnemy_Attack,	//敵攻撃
	eNavManeger,	//ノード管理
	eBullet,		//弾
	eEffect,		//エフェクト
	eItem,			//アイテム
	eLastItem,			//アイテム
	eEventCamera,	//イベントカメラ
	//eFieldCamera,	//ミニマップ

	//2D
	eFieldCamera,	//ミニマップ
	eUI,			//UI
	eScene,			//シーン
	eFade,			//フェード
	eDebug,			//デバッグ

	//eStart2D=eUI,	//=の部分から2D
	eStart2D = eFieldCamera,	//=の部分から2D
};

//タスククラス
class Task {
private:
	TaskType m_type;	//タスクの種類
	//元の場所
	//bool m_isKill;		//削除フラグ
public:
	//TaskType	m_type;		//オブジェクトの種類
	CVector3D	m_pos;		//座標
	CVector3D	m_rot;		//回転値
	CVector3D	m_vec;		//移動ベクトル
	CVector3D	m_dir;		//前方向

	//球、カプセル用
	float		m_rad;		//半径
	float		m_height;	//高さ
	//カプセル用
	CVector3D	m_lineS;	//線分始点
	CVector3D	m_lineE;	//線分終点
	//OBB用
	COBB m_obb;				//Box
	COBB m_obb_old;
	float m_time;			//時間経過
	bool m_isGround;		//着地フラグ
	bool m_isHide;			//HideBoxに隠れているか
	bool m_isFind;			//PlayerがEnemyに触っているか
	bool m_isFindplayer;	//Enemyの探知範囲以内にPlayerがいるか


	//攻撃の連続ヒット防止措置
	int m_hit_no;			//攻撃ヒット側番号
	int m_attack_no;		//攻撃番号
	bool m_isKill;			//削除フラグ
	bool m_dontKill;		//削除しないフラグ
	//コンストラクタ
	Task(TaskType type, bool dontKill = false);
	//デストラクタ
	virtual ~Task();

	//タスクの種類を取得
	TaskType GetType() const;

	//オブジェクトが持つモデルの取得
	virtual CModel* GetModel();

	//削除フラグを取得
	bool IsKill() const;
	//タスクを削除
	void Kill();
	
	//更新
	virtual void Update();
	//描画
	virtual void Render();
	//2D描画
	virtual void Draw();
	//enemy以外の描画処理
	virtual void NoEnemyRender();
	//描画後に描画
	virtual void LateRender();
	//描画後に2D描画
	virtual void LateDraw();
	//描画後に更新
	virtual void LateUpdate();
	//オブジェクト同士の当たり判定検証
	virtual void Collision(Task* b);
	
};