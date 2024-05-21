#pragma once
#include "CTexture.h"
#include "Vertex.h"
class CTextureFrame {
	CVector4D m_clear_color;
	CTexture *mp_texture;	//!�`���e�N�X�`���[
	CTexture* mp_depth;		//!�[�x�e�N�X�`���[
	GLuint m_frame;			//!�t���[���o�b�t�@

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
	CTextureFrame(int width,int height,const CVector4D& clear_color);
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
	int GetWidth();
	/*!
		@brief	�e�N�X�`���[�������擾
		@retval	�e�N�X�`���[����
	**/
	int GetHeight();
	/*!
		@brief	�J���[�p�e�N�X�`���[���擾
		@retval	�e�N�X�`���[�̃|�C���^
	**/
	CTexture* GetTexture() const{
		return mp_texture;
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
	static void Draw(int x, int y,int w,int h, CTexture*target);
};
