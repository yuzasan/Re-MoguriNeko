#pragma once
//#include "../Base/Base.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class StageWatch;

class Stage : public Task {
private:
	//フィールドクラスのインスタンス
	static Stage* ms_instance;
	//経路探索用ノードのテーブル
	static std::list<CVector3D> ms_nodes;

	//ステージモデル
	//CModelObj m_model;
	CModel* m_model;
	CModel* m_Navmodel;
	StageWatch* m_Watchmodel;
	//スカイボックスモデル
	CModelObj skybox;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Stage();

	//デストラクタ
	~Stage();

	//経路探索用のノードを作成
	void CreateNavNodes();

	//指定したレイとStageのコリジョンとのヒット判定
	static bool CollisionRay(const CVector3D& start, const CVector3D& end, CVector3D* outHitPos, CVector3D* outHitNormal);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Render();
	/// <summary>
	/// enemy以外の描画処理
	/// </summary>
	void NoEnemyRender();
	/// <summary>
	/// オブジェクトが持つモデルの取得
	/// </summary>
	/// <returns></returns>
	/*CModel* GetModel() {
		return &m_model;
	}*/
	CModel* GetModel();
	CModel* GetColNavModel() const;
};