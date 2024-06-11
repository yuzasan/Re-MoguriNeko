#pragma once
#include "CTexture.h"
#include "Vertex.h"
class CTextureFrame {
	CVector4D m_clear_color;
	std::vector<CTexture*> m_texture_list;	//!�`���e�N�X�`���[
	CTexture* mp_depth;		//!�[�x�e�N�X�`���[
	GLuint m_frame;			//!�t���[���o�b�t�@
	int m_width;
	int m_height;
	GLint viewport_old[4];

public:
	/**
	* @fn �R���X�g���N�^
	* �R���X�g���N�^
	* @brief �t���[���o�b�t�@�ƕ`���e�N�X�`���[�𐶐��B
	* @param width	[in]�@��
	* @param height	[in]�@����
	* @param clear_color	[in]�@��ʏ����F

	*/
	CTextureFrame(int width, int height, const CVector4D& clear_color, int color_rayer = 1, GLenum* internal_formats = nullptr, GLenum* formats = nullptr, GLenum* types = nullptr);
	/**
	* @fn �f�X�g���N�^
	* �f�X�g���N�^
	* @brief �o�b�t�@��j�����܂��B
	*/
	~CTextureFrame();
	/**
	* @fn �`��J�n
	* 
	* @brief BeginDraw �`�@EndDraw�̊Ԃ́A�`��悪�e�N�X�`���[�ɂȂ�܂��B
	*/
	void BeginDraw();
	/**
	* @fn �`��I��
	*
	* @brief BeginDraw �`�@EndDraw�̊Ԃ́A�`��悪�e�N�X�`���[�ɂȂ�܂��B
	*/
	void EndDraw();

	/**
	* @fn �e�N�X�`��������
	*
	* @brief �`���̃J���[�o�b�t�@�A�f�v�X�o�b�t�@���������܂��B
	*/
	void ClearBuffer();

	/*!
		@brief	�e�N�X�`���[�̕����擾
		@retval	�e�N�X�`���[�̕�
	**/
	int GetWidth()const;
	/*!
		@brief	�e�N�X�`���[�������擾
		@retval	�e�N�X�`���[����
	**/
	int GetHeight()const;
	/*!
		@brief	�J���[�p�e�N�X�`���[���擾
		@retval	�e�N�X�`���[�̃|�C���^
	**/
	CTexture* GetTexture(int idx=0) const{
		return m_texture_list[idx];
	}

	/*!
		@brief	�[�x�p�e�N�X�`���[���擾
		@retval	�e�N�X�`���[�̃|�C���^
	**/
	CTexture* GetDepthTexture() const{
		return mp_depth;
	}

	/*!
		@brief	�f�o�b�O�p�\��
		@param  x		[in]	�ʒuX
		@param  y		[in]	�ʒuY
		@param  w		[in]	��
		@param  h		[in]	����
		@param  target	[in]	�`��e�N�X�`���[
		@retval	�e�N�X�`���[����
	**/
	static void Draw(float x, float y,float w,float h, CTexture*target);
};
