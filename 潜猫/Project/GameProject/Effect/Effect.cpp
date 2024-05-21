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
		//�[�x�o�b�t�@�ւ̏�������OFF
		glDepthMask(GL_FALSE);
	}
	switch (m_blend) {
	case eBlendAlpha:
		//�ʏ�u�����h���[�h��
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	case eBlendAdd:
		//���Z�u�����h���[�h��
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	//���W�w��
	m_model.SetPos(m_pos);
	//if (mp_player == nullptr) {
	//	m_model.SetPos(m_pos);
	//}
	//else {
	//	//���W�w��
	//	m_model.SetPos(mp_player->m_pos + m_pos);
	//}
	
	//�}�e���A���̃A���t�@�l�ݒ�
	m_model.GetMaterial(0)->m_alpha = m_alpha;
	//�X�P�[���ݒ�
	m_model.SetScale(m_scale, m_scale, m_scale);
	//��]�l�ݒ�
	m_model.SetRot(m_rot);
	if (m_builbord) {
		//�r���{�[�h�`��
		m_model.RenderBuilbord(CCamera::GetCurrent()->GetBuilbordMatrix());
	}
	else
	{
		//�ʏ�`��

		m_model.Render();
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_TRUE);
	CLight::SetLighting(true);
}

#define WIDE 1300
#define HIGH 690

//Effect2D�p
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
