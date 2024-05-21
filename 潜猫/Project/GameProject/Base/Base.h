#pragma once
#define GRAVITY (0.01f)
//種類の列挙
enum class BaseType {
	eCamera,
	eField,
	eFellBox,
	eBox,
	eRotateBox,
	eMoveBox,
	eHideBox,
	eTyukanBox,
	eEnemyManager,
	eAppearPoint,
	ePlayer,
	eEnemy,
	ePlayer_Attack,
	eEnemy_Attack,
	eBullet,
	eEffect,
	eItem,
	eFieldCamera,
	eUI,
	eScene,
};
class Base {
public:
	BaseType	m_type;		//オブジェクトの種類
	CVector3D	m_pos;		//座標
	CVector3D	m_rot;		//回転値
	CVector3D	m_vec;		//移動ベクトル
	CVector3D	m_dir;		//前方向
	int		m_kill;		//削除フラグ
	
	//球、カプセル用
	float		m_rad;		//半径
	//カプセル用
	CVector3D	m_lineS;	//線分始点
	CVector3D	m_lineE;	//線分終点
	//OBB用
	COBB m_obb;
	float m_time;
	bool m_isGround;
	//HideBoxに隠れているか
	bool m_isHide;
	//PlayerがEnemyに触っているか
	bool m_isFind;
	//Enemyの探知範囲以内にPlayerがいるか
	bool m_isFindplayer;


	//攻撃の連続ヒット防止措置
	int m_hit_no;			//攻撃ヒット側番号
	int m_attack_no;		//攻撃側番号
private:
	//オブジェクトリスト
	static std::list<Base*> m_list;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">オブジェクトの種類</param>
	Base(BaseType type);
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Base();
	/// <summary>
	/// 削除フラグON
	/// </summary>
	void SetKill();
	/// <summary>
	/// オブジェクトの種類を取得
	/// </summary>
	/// <returns></returns>
	BaseType GetType() {
		return m_type;
	}
	/// <summary>
	/// オブジェクトが持つモデルの取得
	/// </summary>
	/// <returns></returns>
	virtual CModel* GetModel() {
		return nullptr;
	}
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Render();
	/// <summary>
	/// enemy以外の描画処理
	/// </summary>
	virtual void NoEnemyRender();
	//描画後に描画
	virtual void LateRender();
	//描画後に更新
	virtual void LateUpdate();
	/// <summary>
	/// オブジェクト同士の当たり判定検証
	/// </summary>
	/// <param name="b">衝突対象オブジェクト</param>
	virtual void Collision(Base* b);

	/// <summary>
	/// 全てのオブジェクトの削除
	/// </summary>
	static void KillALL();


	/// <summary>
	/// 全てのオブジェクトの削除（即破棄）
	/// </summary>
	static void ClearInstance();


	/// <summary>
	/// 全てのオブジェクトの削除チェック
	/// </summary>
	static void CheckKillALL();
	/// <summary>
	/// 全てのオブジェクトの更新
	/// </summary>
	static void UpdateALL();
	/// <summary>
	/// 全てのオブジェクトの描画
	/// </summary>
	static void RenderALL();

	/// <summary>
	/// enemy以外の全てのオブジェクトの描画
	/// </summary>
	static void NoEnemyRenderALL();
	static void LateRenderALL();
	static void LateUpdateALL();
	/// <summary>
	/// 全てのオブジェクトの当たり判定
	/// </summary>
	static void CollisionALL();
	/// <summary>
	/// オブジェクトを追加
	/// </summary>
	/// <param name="b">追加オブジェクト</param>
	static void Add(Base* b);
	/// <summary>
	/// オブジェクトの検索
	/// </summary>
	/// <param name="type">検索対象の種類</param>
	/// <returns></returns>
	static Base* FindObject(BaseType type);
	/// <summary>
	/// オブジェクトの検索(配列で取得)
	/// </summary>
	/// <param name="type">検索対象の種類</param>
	/// <returns></returns>
	static std::vector<Base*> FindObjects(BaseType type);
	//フレンドクラス
	friend class Base;
};
