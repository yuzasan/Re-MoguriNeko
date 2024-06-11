#include "CTextureFrame.h"
#include "CCamera.h"
#include "CShader.h"


CTextureFrame::CTextureFrame(int width, int height, const CVector4D& clear_color, int color_rayer, GLenum* internal_formats, GLenum* formats, GLenum* types)
{
	m_clear_color = clear_color;
	m_width = width;
	m_height = height;
	for (int i = 0; i < color_rayer; i++) {
		//�J���[�p�e�N�X�`������
		if (types)
			m_texture_list.push_back(new CTexture(width, height, internal_formats[i], formats[i], types[i]));
		else
			m_texture_list.push_back(new CTexture(width, height, GL_RGBA, GL_RGBA));
	}
	//�[�x�o�b�t�@�p�e�N�X�`������
	mp_depth = new CTexture(width, height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT);
	//�t���[���o�b�t�@�쐬
	glGenFramebuffersEXT(1, &m_frame);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_frame);

	for (int i = 0; i < color_rayer; i++) {
		//�e�N�X�`���[�ڑ��i�J���[�j
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_texture_list[i]->m_bufID, 0);
	}
	//�e�N�X�`���[�ڑ��i�[�x�j
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, mp_depth->m_bufID, 0);


	//�ʏ�`��֖߂�
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

CTextureFrame::~CTextureFrame()
{

	glDeleteFramebuffers(1, &m_frame);
	for (auto& t : m_texture_list) {
		t->Release();
		delete t;
	}
	if(mp_depth) {
		mp_depth->Release();
		delete mp_depth;
	}

}

void CTextureFrame::BeginDraw()
{
	//�t���[���o�b�t�@�L�����B�ȍ~�̕`�揈���� m_texture_list,mp_depth�֏������܂��B
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_frame);
	const GLenum bufferList[] = {
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3,
		GL_COLOR_ATTACHMENT4,
		GL_COLOR_ATTACHMENT5,
		GL_COLOR_ATTACHMENT6,
		GL_COLOR_ATTACHMENT7,
	};
	glDrawBuffers((GLsizei)m_texture_list.size(), bufferList);
	//���̃r���[�|�[�g��ۑ�
	glGetIntegerv(GL_VIEWPORT, viewport_old);
	//�r���[�|�[�g���e�N�X�`���[�̃T�C�Y�ɐݒ�
	glViewport(0, 0, m_width, m_height);

	float back_color[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE,back_color);

	//����
	glClearColor(m_clear_color.r, m_clear_color.g, m_clear_color.b, m_clear_color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//�N���A�J���[�����ɖ߂�
	glClearColor(back_color[0], back_color[1], back_color[2], back_color[3]);
}

void CTextureFrame::EndDraw()
{
	
	//m_black�ւ̏������ݏI��
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	//�����܂ł̕`�施�߂���U����
	glFlush();
	glViewport(viewport_old[0], viewport_old[1], viewport_old[2], viewport_old[3]);

}

void CTextureFrame::ClearBuffer()
{
	//�t���[���o�b�t�@�L�����B�ȍ~�̕`�揈���� mp_texture,mp_depth�֏������܂��B
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_frame);
	float back_color[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, back_color);
	//����
	glClearColor(m_clear_color.r, m_clear_color.g, m_clear_color.b, m_clear_color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//�N���A�J���[�����ɖ߂�
	glClearColor(back_color[0], back_color[1], back_color[2], back_color[3]);
	//m_black�ւ̏������ݏI��
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

int CTextureFrame::GetWidth()const
{
	return m_width;
}

int CTextureFrame::GetHeight()const
{
	return m_height;
}

void CTextureFrame::Draw(float x, float y,float w,float h, CTexture* target)
{

	glDisable(GL_CULL_FACE);
	//���C�e�B���O����
	glDisable(GL_LIGHTING);
	//�f�v�X�e�X�g����
	glDisable(GL_DEPTH_TEST);

	//
	//���˓��e�̍s����쐬
	//
	const CMatrix& mProj = CCamera::GetCurrent()->Get2DProjectionMatrix();



	CMatrix pvm = mProj * CMatrix::MTranselate(x, y, 0) * CMatrix::MScale(w, h, 1);

	CShader* s = CShader::GetInstance("Image");



	CVector3D vertices[] = {
		CVector3D(0,0,0),
		CVector3D(1,0,0),
		CVector3D(1,1,0),
		CVector3D(0,1,0)
	};
	CVector2D texcoord[] = {
		CVector2D(0,1),
		CVector2D(1,1),
		CVector2D(1,0),
		CVector2D(0,0)
	};
	CTexture* t = target;


	s->Enable();
	t->MapTexture();

	GLint samplerId = glGetUniformLocation(s->GetProgram(), "sampler");
	glUniform1i(samplerId, 0);//GL_TEXTURE0��K�p

	glUniformMatrix4fv(glGetUniformLocation(s->GetProgram(), "PVWMatrix"), 1, GL_FALSE, pvm.f);
	glUniform4fv(glGetUniformLocation(s->GetProgram(), "Color"), 1, CVector4D(1,1,1,1).v);

	glEnableVertexAttribArray(CShader::eVertexLocation);
	glEnableVertexAttribArray(CShader::eTexcoordlLocation);
	glVertexAttribPointer(CShader::eVertexLocation, 3, GL_FLOAT, GL_FALSE, 0, vertices->v);
	glVertexAttribPointer(CShader::eTexcoordlLocation, 2, GL_FLOAT, GL_FALSE, 0, texcoord->v);


	glDrawArrays(GL_QUADS, 0, 4);

	glDisableVertexAttribArray(CShader::eVertexLocation);
	glDisableVertexAttribArray(CShader::eTexcoordlLocation);

	s->Disable();
	t->UnmapTexture();

	//���̐ݒ�ɖ߂�

	glEnable(GL_CULL_FACE);
	//���C�e�B���O����
	glEnable(GL_LIGHTING);
	//�f�v�X�e�X�g����
	glEnable(GL_DEPTH_TEST);



}
