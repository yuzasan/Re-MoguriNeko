#include "CShadow.h"

CShadow *CShadow::m_shadow=nullptr;

CShadow::CShadow(float length, float height, int texWidth, int texHeight): m_state(eNone) {
	
	m_depthtexHeight = texHeight;
	m_depthtexWidth = texWidth;
	m_lightViewLength = length;
	m_lightHeight = height;

	mp_render_target = new CTextureFrame(m_depthtexHeight, m_depthtexWidth, CVector4D(1, 1, 1, 1));


}
CShadow::~CShadow() {
	delete mp_render_target;

}
void CShadow::Render(std::function<void()> render) {

	CCamera* back = CCamera::GetCurrent();
	//�V���h�E�}�b�v�`�揀��
	CVector3D cam_pos = CCamera::GetCurrent()->GetPos();
	//�V���h�E�}�b�v�p�@��������̃J�����ɐ؂�ւ�
	CCamera::SetCurrent(CCamera::GetCamera(CCamera::eShadowCamera));
	//���e�s��
	CCamera::GetCurrent()->Ortho(-m_lightViewLength, m_lightViewLength, -m_lightViewLength, m_lightViewLength, 1.0f, 100.0f);
	//0�Ԗڂ̃��C�g�̌���
	CVector3D ld = CLight::GetDirPointer()[0];
	//�����̍���
	const float dist = m_lightHeight;
	//�r���[�s��
	CCamera::GetCurrent()->LookAt(cam_pos - ld * dist, cam_pos, CVector3D(0, 1, 0));
	//�e�N�X�`���s��
	static  const CMatrix texMat(0.5f, 0.0f, 0.0f, 0.5f,
		0.0f, 0.5f, 0.0f, 0.5f,
		0.0f, 0.0f, 0.5f, 0.5f,
		0.0f, 0.0f, 0.0f, 1.0f);
	//�V���h�E�}�b�v�ϊ��s��
	CLight::m_shadowMatrix = texMat * CCamera::GetCurrent()->GetProjectionMatrix() * CCamera::GetCurrent()->GetViewMatrix();
	CLight::SetLighting(false);
	//�O�ʂ�`�悵�Ȃ�
	glCullFace(GL_FRONT);
	m_state = eShadow;
	//�V���h�E�}�b�v�p�`��
	mp_render_target->BeginDraw();
	render();
	mp_render_target->EndDraw();
	//�w�ʂ�`�悵�Ȃ�
	glCullFace(GL_BACK);
	CLight::SetLighting(true);
	//���̃J�����ɖ߂�
	CCamera::SetCurrent(back);
	//7�Ԃ̃e�N�X�`���[���V���h�E�}�b�v�Ƃ���
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, mp_render_target->GetDepthTexture()->m_bufID);
	glEnable(GL_TEXTURE_2D);
	//0�ԃe�N�X�`���[����ɖ߂�
	glActiveTexture(GL_TEXTURE0);

	m_state = eNone;
	//�ʏ�`��
	render();
}

void CShadow::DrawDepthTex(int x,int y,int w, int h)
{
	mp_render_target->Draw(x, y, w, h, mp_render_target->GetDepthTexture());
}
void CShadow::CreateInscance(float length, float height, int texWidth, int texHeight)
{
	if (m_shadow) return;
	m_shadow = new CShadow(length, height,texWidth, texHeight);
}

CShadow* CShadow::GetInstance()
{
	if (!m_shadow) CreateInscance();
	return m_shadow;
}

void CShadow::ClearInstance()
{
	if (!m_shadow) return;
	delete m_shadow;
	m_shadow = nullptr;
}