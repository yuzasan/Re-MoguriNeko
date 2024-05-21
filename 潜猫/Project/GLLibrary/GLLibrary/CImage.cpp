#include "CImage.h"
#include "CCollision.h"
#include "CShader.h"
#include "CCamera.h"



CImage::CImage() : mp_texture(nullptr),m_pos(0,0),m_center(0,0),m_size(0,0),m_src_rect(0,0,0,0),m_ang(0),m_flipH(false),m_color(1.0f,1.0f,1.0f,1.0f), m_animData(nullptr) {
}
CImage::~CImage(){
}
bool CImage::Load(const char* filename, TexAnimData* d, int width, int height){
	if(mp_texture = CTexture::LoadTexture(filename)){
		m_width = mp_texture->m_width;
		m_height = mp_texture->m_height;
		SetPos(0, 0);
		SetRect(0, 0, (float)m_width, (float)m_height);
		AttachAnimationData(d, width, height);
		if(d) SetSize((float)width, (float)height);
		else SetSize((float)m_width, (float)m_height);
		return true;
	} else {
		char str[256]="";
		sprintf_s(str,256, "%s�̓ǂݍ��݂Ɏ��s���܂���\n", filename);
		MessageBox(GL::hWnd, str, "", MB_OK);
	}
	return false;
	

}

CImage* CImage::CreateImage(const char* filename, TexAnimData* d, int width, int height) {
	CImage* img = new CImage();
	if (img->Load(filename,  d, width, height)) {
		return img;
	}
	delete img;
	return nullptr;
}

CImage& CImage::operator = (const CImage& img) {
	memcpy(this, &img, sizeof(CImage));
	return *this;
}
void CImage::SetRect(float left, float top, float right, float bottom)
{
	m_src_rect.m_left = left; m_src_rect.m_right = right;
	m_src_rect.m_top = top; m_src_rect.m_bottom = bottom;
}
void CImage::SetColor(float r, float g, float b, float a)
{
	m_color.r = r;
	m_color.g = g;
	m_color.b = b;
	m_color.a = a;
}
void CImage::UpdateAnimation() {
	if (!m_animData) return;
	m_animCount += m_animSpeed;
	if (m_animCount >= m_animData[m_animJam].pData[m_animIndex].cnt) {
		m_animIndex++;
		m_animCount = 0;
		if (m_animIndex >= m_animData[m_animJam].size) {
			if (m_animLoop) {
				m_animIndex = 0;
			}
			else {
				m_animIndex = m_animData[m_animJam].size - 1;
				m_animCount = (float)m_animData[m_animJam].pData[m_animIndex].cnt;
			}
		}
	}
}
void CImage::AttachAnimationData(TexAnimData* d, int width, int height) {

	m_animData = d;
	m_animJam = -1;
	m_animSpeed = 1;
	m_pat_width = width;
	m_pat_height = height;
	if(m_pat_width>0)m_cut_width = m_width / abs((int)m_pat_width);
	else m_cut_width=0;
}
void CImage::ChangeAnimation(int num, bool loop, int index, bool check) {
	if (!m_animData) return;
	if (check) {
		if (m_animJam == num) return;
	}
	m_animJam = num;
	m_animIndex = index;
	m_animCount = 0;
	m_animLoop = loop;

}
void CImage::SetAnimationSpeed(float speed) {
	m_animSpeed = speed;
}
bool CImage::CheckAnimationEnd() {
	if (!m_animData) return true;
	return (m_animCount == m_animData[m_animJam].pData[m_animIndex].cnt && m_animIndex >= m_animData[m_animJam].size - 1);

}


void CImage::Draw() {
	if (m_animData) {
#ifdef _DEBUG
		if (m_animJam < 0) {
			MessageBox(GL::hWnd, "�Đ��A�j���[�V�������ݒ肳��Ă��܂���BChangeAnimation���Ă�ł��������B", "", MB_OK);
		}
		if (m_cut_width <= 0) {
			MessageBox(GL::hWnd, "�P�J�b�g�̃T�C�Y���Ⴂ�܂��BCreateImage��width,height�𐳂����ݒ肵�Ă������B", "", MB_OK);
		}
#endif // _DEBUG

		float x = (float)(m_pat_width * (m_animData[m_animJam].pData[m_animIndex].pat % m_cut_width));
		float y = (float)(m_pat_height * (m_animData[m_animJam].pData[m_animIndex].pat / m_cut_width));
		SetRect(x, y, x + m_pat_width, y + m_pat_height);
	}
//	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//�e��@�\�𖳌���
	//�J�����O����
	glDisable(GL_CULL_FACE);
	//���C�e�B���O����
	glDisable(GL_LIGHTING);
	//�f�v�X�e�X�g����
	glDisable(GL_DEPTH_TEST);

	CMatrix pvm;
	//
	//���˓��e�̍s����쐬
	//
	const CMatrix& mProj = CCamera::GetCurrent()->Get2DProjectionMatrix();



	//���f�����r���[�s����Đݒ�
	CMatrix mT, mR;

	mT.Transelate(m_pos.x, m_pos.y, 0.0f);

	mR.RotationZ(-m_ang);

	pvm = mProj * mT * mR;


	CShader* s = CShader::GetInstance("Image");


	//�摜�𒣂�t���邽�߂̃|���S���̍쐬
	struct SVertex {
		float tu;
		float tv;
		float vx;
		float vy;
		float vz;
	};
	SVertex vertices[] = {
		//�e�N�X�`���[���W uv														���_���Wxyz
		{ m_src_rect.m_left/m_width,m_src_rect.m_top/m_height,0-m_center.x,			0-m_center.y,			0.0f },		//����
		{ m_src_rect.m_right/m_width,m_src_rect.m_top/m_height,m_size.x-m_center.x,	0-m_center.y,			0.0f },		//�E��
		{ m_src_rect.m_left/m_width,m_src_rect.m_bottom/m_height,0-m_center.x,		m_size.y-m_center.y,	0.0f },		//����
		{ m_src_rect.m_right/m_width,m_src_rect.m_bottom/m_height,m_size.x-m_center.x,m_size.y-m_center.y,	0.0f }		//�E��
	};
	//���]����
	if(m_flipH) {
		float t = vertices[0].tu;
		vertices[0].tu = vertices[2].tu = vertices[1].tu;
		vertices[1].tu = vertices[3].tu = t;
		
	}
	if (m_flipV) {
		float t = vertices[0].tv;
		vertices[0].tv = vertices[1].tv = vertices[2].tv;
		vertices[2].tv = vertices[3].tv = t;

	}
	
	s->Enable();
	mp_texture->MapTexture();
	
	GLint samplerId = glGetUniformLocation(s->GetProgram(), "sampler");
	glUniform1i(samplerId, 0);//GL_TEXTURE0��K�p

	glUniformMatrix4fv(glGetUniformLocation(s->GetProgram(), "PVWMatrix"), 1, GL_FALSE, pvm.f);
	glUniform4fv(glGetUniformLocation(s->GetProgram(), "Color"), 1, m_color.v);

	glEnableVertexAttribArray(CShader::eVertexLocation);
	glEnableVertexAttribArray(CShader::eTexcoordlLocation);
	glVertexAttribPointer(CShader::eVertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), &vertices->vx);
	glVertexAttribPointer(CShader::eTexcoordlLocation, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex), vertices);


	glDrawArrays(GL_TRIANGLE_STRIP,0,4);

	glDisableVertexAttribArray(CShader::eVertexLocation);
	glDisableVertexAttribArray(CShader::eTexcoordlLocation);

	s->Disable();
	mp_texture->UnmapTexture();

	//���̐ݒ�ɖ߂�
	
	glEnable(GL_CULL_FACE);
	//���C�e�B���O����
	glEnable(GL_LIGHTING);
	//�f�v�X�e�X�g����
	glEnable(GL_DEPTH_TEST);

//	glPopAttrib();
	
}


void CImage::DrawBegin() {

	glDisable(GL_CULL_FACE);
	//���C�e�B���O����
	glDisable(GL_LIGHTING);
	//�f�v�X�e�X�g����
	glDisable(GL_DEPTH_TEST);

	CShader * s = CShader::GetInstance("Image");

	s->Enable();
	mp_texture->MapTexture();

	GLint samplerId = glGetUniformLocation(s->GetProgram(), "sampler");
	glUniform1i(samplerId, 0);//GL_TEXTURE0��K�p

	glUniform4fv(glGetUniformLocation(s->GetProgram(), "Color"), 1, m_color.v);

	glEnableVertexAttribArray(CShader::eVertexLocation);
	glEnableVertexAttribArray(CShader::eTexcoordlLocation);


}
void CImage::DrawS() {

	CShader* s = CShader::GetInstance("Image");
	CMatrix pvm;
	//
	//���˓��e�̍s����쐬
	//
	const CMatrix& mProj = CCamera::GetCurrent()->Get2DProjectionMatrix();

	//���f�����r���[�s����Đݒ�
	CMatrix mT, mR;

	mT.Transelate(m_pos.x, m_pos.y, 0.0f);

	mR.RotationZ(-m_ang);

	pvm = mProj * mT;

	glUniformMatrix4fv(glGetUniformLocation(s->GetProgram(), "PVWMatrix"), 1, GL_FALSE, pvm.f);

	//�摜�𒣂�t���邽�߂̃|���S���̍쐬
	struct SVertex {
		float tu;
		float tv;
		float vx;
		float vy;
		float vz;
	};
	SVertex vertices[] = {
		//�e�N�X�`���[���W uv														���_���Wxyz
		{ m_src_rect.m_left / m_width,m_src_rect.m_top / m_height,0 - m_center.x,			0 - m_center.y,			0.0f },		//����
		{ m_src_rect.m_right / m_width,m_src_rect.m_top / m_height,m_size.x - m_center.x,	0 - m_center.y,			0.0f },		//�E��
		{ m_src_rect.m_left / m_width,m_src_rect.m_bottom / m_height,0 - m_center.x,		m_size.y - m_center.y,	0.0f },		//����
		{ m_src_rect.m_right / m_width,m_src_rect.m_bottom / m_height,m_size.x - m_center.x,m_size.y - m_center.y,	0.0f }		//�E��
	};
	//���E���]����
	if (m_flipH) {
		float t = vertices[0].tu;
		vertices[0].tu = vertices[2].tu = vertices[1].tu;
		vertices[1].tu = vertices[3].tu = t;

	}
	if (m_flipV) {
		float t = vertices[0].tv;
		vertices[0].tv = vertices[1].tv = vertices[2].tv;
		vertices[2].tv = vertices[3].tv = t;

	}


	glVertexAttribPointer(CShader::eVertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), &vertices->vx);
	glVertexAttribPointer(CShader::eTexcoordlLocation, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex), vertices);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}
void CImage::DrawEnd() {

	CShader* s = CShader::GetInstance("Image");
	//	glPushAttrib(GL_ALL_ATTRIB_BITS);
		//�e��@�\�𖳌���
		//�J�����O����


	glDisableVertexAttribArray(CShader::eVertexLocation);
	glDisableVertexAttribArray(CShader::eTexcoordlLocation);

	s->Disable();
	mp_texture->UnmapTexture();

	//���̐ݒ�ɖ߂�

	glEnable(GL_CULL_FACE);
	//���C�e�B���O����
	glEnable(GL_LIGHTING);
	//�f�v�X�e�X�g����
	glEnable(GL_DEPTH_TEST);

	//	glPopAttrib();


//	glPopAttrib();

}
bool CImage::CollisionImage(const CImage &img) {
	CRect rect(img.m_pos.x - img.m_center.x, img.m_pos.y - img.m_center.y,
		img.m_pos.x - img.m_center.x + img.m_size.x, img.m_pos.y - img.m_center.y + img.m_size.y);

	return CollisionRect(rect);
}
bool CImage::CollisionRect(const CRect &rect) {
	CRect rect2(this->m_pos.x - this->m_center.x, this->m_pos.y - this->m_center.y,
		this->m_pos.x - this->m_center.x + this->m_size.x, this->m_pos.y - this->m_center.y + this->m_size.y);
	return CCollision::CollisionRect(rect2, rect);
}
bool CImage::CollisionPoint(const CVector2D &pos) {
	CRect rect(this->m_pos.x - this->m_center.x, this->m_pos.y - this->m_center.y,
		this->m_pos.x - this->m_center.x + this->m_size.x, this->m_pos.y - this->m_center.y + this->m_size.y);
	return CCollision::CollisionRectPoint(rect, pos);

}

void CImage::Release()
{
	if (mp_texture) {
		mp_texture->Release();
		delete mp_texture;
		mp_texture = nullptr;
	}
}
