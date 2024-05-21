#include "FieldCamera.h"
#include "../Character/Player.h"
#include "../Character/Enemy.h"
#include "../Stage/Stage.h"
//static は最初の#includeのところに書く
bool FieldCamera::rendring = false;

FieldCamera::FieldCamera() :Task(TaskType::eFieldCamera) {
	//■レンダーターゲット生成
	//texture_frame_mini_map = new CTextureFrame(512, 512, CVector4D(1, 1, 1, 1));
	texture_frame_mini_map = new CTextureFrame(1028, 1028, CVector4D(1, 1, 1, 1));
	mp_player = TaskManeger::FindObject(TaskType::ePlayer);
}

FieldCamera::~FieldCamera()
{
}

void FieldCamera::Update(){
	/*
	{
		rendring = true;
		//--■別始点からのゲーム画面描画---------
		//現在のカメラをコピー
		CCamera back = *CCamera::GetCurrent();

		//キャラクター上面からのカメラ	下に向いている
		//CCamera::GetCurrent()->SetTranseRot(mp_player->m_pos + CVector3D(0, 10, 0), CVector3D(DtoR(90), 0, 0));
		CCamera::GetCurrent()->SetTranseRot(mp_player->m_pos + CVector3D(0, 15, 0), CVector3D(DtoR(90), 0, 0));

		//描画先の画面サイズが横512:縦512のためアスペクト比は1.0f
		CCamera::GetCurrent()->Perspective(DtoR(60), 1.0f, 1.0f, 1000.0f);

		//影を消去
		CShadow::GetInstance()->GetRenderTarget()->ClearBuffer();

		texture_frame_mini_map->BeginDraw();

		//Base::RenderALL();
		//Base::NoEnemyRenderALL();
		//Base::LateRenderALL();

		TaskManeger::Instance()->NoEnemyRender();
		TaskManeger::Instance()->LateRender();
		texture_frame_mini_map->EndDraw();
		//カメラを元の状態に戻す
		*CCamera::GetCurrent() = back;
		rendring = false;
	}
	*/
}

void FieldCamera::Draw(){
	//if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	{
		rendring = true;
		//--■別始点からのゲーム画面描画---------
		//現在のカメラをコピー
		CCamera back = *CCamera::GetCurrent();

		//キャラクター上面からのカメラ	下に向いている
		CCamera::GetCurrent()->SetTranseRot(mp_player->m_pos + CVector3D(-2.5, 15, 0), CVector3D(DtoR(90), 0, 0));

		//描画先の画面サイズが横512:縦512のためアスペクト比は1.0f
		//CCamera::GetCurrent()->Perspective(DtoR(60), 1.0f, 1.0f, 1000.0f);
		CRect rect = { -5,-5,5,5 };
		//正射投影行列で設定
		CCamera::GetCurrent()->Ortho(rect.m_left, rect.m_right, rect.m_top, rect.m_bottom, 1.0f, 1000.0f);

		//影を消去
		CShadow::GetInstance()->GetRenderTarget()->ClearBuffer();

		texture_frame_mini_map->BeginDraw();

		TaskManeger::Instance()->NoEnemyRender();
		TaskManeger::Instance()->LateRender();
		texture_frame_mini_map->EndDraw();
		//カメラを元の状態に戻す
		*CCamera::GetCurrent() = back;
		rendring = false;

		Utility::DrawQuad(CVector2D(0, 0), CVector2D(258, 258), CVector4D(0.0f, 0.0f, 0.0f, 1.0f));
		//レンダーターゲットのテクスチャ―を簡易表示
		CTextureFrame::Draw(0, 0, 256, 256, texture_frame_mini_map->GetTexture());
	}
}

void FieldCamera::LateUpdate(){
	//Utility::DrawQuad(CVector2D(0,0), CVector2D(258, 258), CVector4D(0.0f, 0.0f, 0.0f, 1.0f));
	////レンダーターゲットのテクスチャ―を簡易表示
	//CTextureFrame::Draw(0, 0, 256, 256, texture_frame_mini_map->GetTexture());
}