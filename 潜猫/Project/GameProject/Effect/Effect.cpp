#include "Effect.h"
#include "../Game/Character/Player.h"
Effect::Effect(const char* model, const CVector3D& pos, const CVector3D& vec, const CVector3D& rot, const CVector3D& accel, float scale, float change_scale, float alpha, float change_alpha, int blend, bool builbord, bool depth, int time) :Task(TaskType::eEffect)
	,m_rot_vec(rot)
	,m_vec_accel(accel)
	,m_scale(scale)
	,m_scale_speed(change_scale)
	,m_alpha(1.0f)
	,m_alpha_speed(change_alpha)
	,m_blend(blend)
	,m_builbord(builbord)
	,m_depth(depth)
	,m_time(time)
{
	mp_player = dynamic_cast<Player*>(TaskManeger::FindObject(TaskType::ePlayer));
	m_pos = pos;
	m_vec = vec;
	m_model = COPY_RESOURCE(model, CModelObj);
}

Effect::~Effect()
{
}

void Effect::Update()
{
	m_vec += m_vec_accel;
	m_pos += m_vec;
	m_rot += m_rot_vec * CFPS::GetDeltaTime();
	m_scale += m_scale_speed;
	m_alpha += m_alpha_speed;
	if (m_time > 0) m_time -= CFPS::GetDeltaTime();
	if (m_time == 0 || m_scale < 0 || m_alpha < 0) Kill();
}

void Effect::Render()
{
	CLight::SetLighting(false);
	
	if (!m_depth) {
		//深度バッファへの書き込みOFF
		glDepthMask(GL_FALSE);
	}
	switch (m_blend) {
	case eBlendAlpha:
		//通常ブレンドモードに
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	case eBlendAdd:
		//加算ブレンドモードに
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	//座標指定
	m_model.SetPos(m_pos);
	//if (mp_player == nullptr) {
	//	m_model.SetPos(m_pos);
	//}
	//else {
	//	//座標指定
	//	m_model.SetPos(mp_player->m_pos + m_pos);
	//}
	
	//マテリアルのアルファ値設定
	m_model.GetMaterial(0)->m_alpha = m_alpha;
	//スケール設定
	m_model.SetScale(m_scale, m_scale, m_scale);
	//回転値設定
	m_model.SetRot(m_rot);
	if (m_builbord) {
		//ビルボード描画
		m_model.RenderBuilbord(CCamera::GetCurrent()->GetBuilbordMatrix());
	}
	else
	{
		//通常描画

		m_model.Render();
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_TRUE);
	CLight::SetLighting(true);
}

#define WIDE 1300
#define HIGH 690

//Effect2D用
Effect2D::Effect2D(const char* img):Task(TaskType::eUI)
	,mp_player(nullptr)
{
	m_img = COPY_RESOURCE(img, CImage);
}

Effect2D::~Effect2D()
{
}

void Effect2D::Update()
{
	if (!mp_player) {
		mp_player= dynamic_cast<Player*>(TaskManeger::FindObject(TaskType::ePlayer));
	}
	if (mp_player != nullptr) {
		if (!mp_player->m_isSearch) {
			Kill();
		}
	}
}

void Effect2D::Draw()
{
	m_img.SetPos(WIDE, HIGH);
	m_img.Draw();
}
