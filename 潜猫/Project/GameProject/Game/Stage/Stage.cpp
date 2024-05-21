#include "Stage.h"
#include "StageWatch.h"
#include "../../Navigation/NavNode.h"
#include "../../Navigation/NavManeger.h"

Stage* Stage::ms_instance = nullptr;

//経路探索用ノードのテーブル
std::list<CVector3D> Stage::ms_nodes =
{	
	CVector3D(-8,6,18),
	CVector3D(-8,6,25),
	CVector3D(-8,6,50),

	CVector3D(-13,6,-2),
	CVector3D(-14,6,5),
	CVector3D(-14,6,23),
	CVector3D(-14,6,43),
	
	CVector3D(-20,6,11),
	CVector3D(-20,6,23),
	CVector3D(-20,6,36),

	CVector3D(-34,6,-2),
	CVector3D(-34,6,5),
	CVector3D(-34,6,11),
	CVector3D(-34,6,36),
	CVector3D(-34,6,43),
	CVector3D(-34,6,50),
	
	CVector3D(-45,6,11),
	CVector3D(-45,6,23),
	CVector3D(-45,6,36),

	CVector3D(-52,6,5),
	CVector3D(-52,6,23),
	CVector3D(-52,6,36),
	
	CVector3D(-58,6,-2),
	CVector3D(-58,6,24),
	CVector3D(-58,6,48),

	//2階
	//CVector3D(-8, 18, -3),
	//CVector3D(-8, 13, 8),
	//CVector3D(-8,14,0),
	//CVector3D(-8,18,-5),
	//CVector3D(-16,17,-4),

	CVector3D(-19,18,4),
	//CVector3D(-19,18,-4),
	CVector3D(-19,18,35),

	CVector3D(-27,18,6),

	CVector3D(-47,18,4),
	CVector3D(-47,18,35),

	//各部屋につながるノード
	CVector3D(-29,18,26),
	CVector3D(-29,18,37),

	CVector3D(-36,18,-3),
	CVector3D(-36,18,5),
	CVector3D(-36,18,12),

	CVector3D(-47,18,12),
	CVector3D(-47,18,30),

	CVector3D(-56,18,12),
	CVector3D(-56,18,31),

	//各部屋のノード
	CVector3D(-25,18,-3),
	CVector3D(-37,18,19),
	CVector3D(-41,18,27),
	CVector3D(-48,18,-3),
	CVector3D(-55,18,21),
	CVector3D(-56,18,-3),

	//外
	//CVector3D(-2, 6, 24),
	//CVector3D(13, 1, 24),
	//CVector3D(13, 1, 32),

};

//コンストラクタ
Stage::Stage()
	:Task(TaskType::eField)
{
	ms_instance = this;

	//m_model = COPY_RESOURCE("Stage", CModelObj);
	m_model = GET_RESOURCE("Stagecol", CModel);
	m_Navmodel = GET_RESOURCE("Stagecol", CModel);
	
	skybox = COPY_RESOURCE("Sky", CModelObj);
	for (int i = 0; i < skybox.GetMaterialSize(); i++) {
		if (CTexture* t = skybox.GetMaterial(i)->mp_texture) {
			//テクスチャ―の貼り方変更
			t->SetWrapST(GL_CLAMP_TO_EDGE);
			t->SetFilter(GL_LINEAR);
		}
	}

	m_Watchmodel = new StageWatch();

	//経路探索用のノードを作成
	CreateNavNodes();
}

//デストラクタ
Stage::~Stage()
{
	if (ms_instance == this) {
		ms_instance = nullptr;
	}
}

//経路探索用のノードを作成
void Stage::CreateNavNodes()
{
	//テーブルの座標に経路探索用のノードを作成
	for (CVector3D nodePos : ms_nodes) {
		new NavNode(nodePos);
	}

	/*
	float xMin = -14.0f;
	float xMax = 14.0f;
	float zMin = -14.0f;
	float zMax = 14.0f;

	int w = 10;
	int h = 10;
	for (int i = 0; i < w; i++) {
		float wper = (float)i / (w - 1);
		for (int j = 0; j < h; j++) {
			float hper = (float)j / (h - 1);

			CVector3D pos = CVector3D::zero;
			pos.x = Utility::Lerp(xMin, xMax, wper);
			pos.z = Utility::Lerp(zMin, zMax, hper);
			new NavNode(pos);
		}
	}
	*/

}

CModel* Stage::GetColNavModel() const
{
	return m_Navmodel;
}

CModel* Stage::GetModel()
{
	return m_model;
}


//指定したレイとStageのコリジョンとのヒット判定
bool Stage::CollisionRay(const CVector3D& start, const CVector3D& end, CVector3D* outHitPos, CVector3D* outHitNormal)
{
	//フィールドが作成されていなければ、スルー
	Stage* s = ms_instance;
	if (s == nullptr) return false;

	//コリジョンモデルが作成されていなければ、スルー
	//CModel* col = s->GetModel();
	CModel* col = s->GetColNavModel();
	if (col == nullptr) return false;

	//コリジョンが存在すれば、レイとのヒット判定の結果を返す
	return col->CollisionRay(outHitPos, outHitNormal, start, end);
}

void Stage::Update()
{

}

void Stage::Render()
{
	//■スカイボックス
	//背景として描画するので、一番最初に描画する

	//深度テストOFF
	//ここでの描画は深度バッファへ影響しない
	glDisable(GL_DEPTH_TEST);
	//ライティング無効
	CLight::SetLighting(false);
	//現在使用中のカメラを取得
	CCamera* back = CCamera::GetCurrent();
	//スカイボックス用カメラを取得
	CCamera c = *back;
	//ビュー行列を取得
	CMatrix matrix = c.GetViewMatrix();
	//ビュー行列の座標を無効
	matrix.m03 = 0; matrix.m13 = 0; matrix.m23 = 0;
	//スカイボックス用のビュー行列に設定
	c.SetViewMatrix(matrix);
	//使用するカメラをスカイボックス用に設定
	CCamera::SetCurrent(&c);

	//スカイボックス描画
	skybox.Render();
	//設定を元の設定に戻す
	//カメラを元のカメラに戻す
	CCamera::SetCurrent(back);
	//ライティング有効
	CLight::SetLighting(true);
	//深度テストON
	glEnable(GL_DEPTH_TEST);
	if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	/*m_model.SetScale(1, 1, 1);
	m_model.Render();*/
	//m_model->SetScale(1, 1, 1);

	//m_model->Render();
}

void Stage::NoEnemyRender()
{
	//■スカイボックス
	//背景として描画するので、一番最初に描画する

	//深度テストOFF
	//ここでの描画は深度バッファへ影響しない
	glDisable(GL_DEPTH_TEST);
	//ライティング無効
	CLight::SetLighting(false);
	//現在使用中のカメラを取得
	CCamera* back = CCamera::GetCurrent();
	//スカイボックス用カメラを取得
	CCamera c = *back;
	//ビュー行列を取得
	CMatrix matrix = c.GetViewMatrix();
	//ビュー行列の座標を無効
	matrix.m03 = 0; matrix.m13 = 0; matrix.m23 = 0;
	//スカイボックス用のビュー行列に設定
	c.SetViewMatrix(matrix);
	//使用するカメラをスカイボックス用に設定
	CCamera::SetCurrent(&c);

	//スカイボックス描画
	skybox.Render();
	//設定を元の設定に戻す
	//カメラを元のカメラに戻す
	CCamera::SetCurrent(back);
	//ライティング有効
	CLight::SetLighting(true);
	//深度テストON
	glEnable(GL_DEPTH_TEST);
	if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	/*m_model.SetScale(1, 1, 1);
	m_model.Render();*/
	//m_model->SetScale(1, 1, 1);

	m_model->Render();
}