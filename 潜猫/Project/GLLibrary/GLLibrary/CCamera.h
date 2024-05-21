/*!
 *	@brief		�J�����N���X
 *	5�̃J���������݂���B
 *�@�J�����g�J�����ɐݒ肵���r���[�s��Ń��f�����\�������B
 */
#pragma once

#include "CVector.h"
#include "CMatrix.h"
#include "CRect.h"
class CCamera {
public:

	enum {
		eMainCamera,
		eSkyBox,
		eShadowCamera,
		eOsser_1,
		eMax
	};
private:
	CVector3D m_pos;	//�J�����̈ʒu
	CVector3D m_at;		//�����_
	CVector3D m_up;		//Up�x�N�g��
	CVector3D m_dir;	//�����iFront�x�N�g���j
	CMatrix m_mView;	//�J�����s��
	CMatrix m_mProj;	//�ˉe�s��
	CMatrix m_m2D;		//2D�ˉe�s��
	CMatrix m_mViewport;	//�r���[�|�[�g�s��
	CRect m_viewportRect;	//�r���[�|�[�g��`
	float	m_whidth;	//�J�����̕�
	float	m_height;	//�@�V�@�@����
	static CCamera m_camera[eMax];
	static CCamera *m_pCurrent;
//	CRect	m_screen;
public:

	/*!
		@brief	�J�����I�u�W�F�N�g���擾
		@param	num			[in] �擾����J�����̔ԍ�
		@retval	�J�����I�u�W�F�N�g�̃A�h���X
	**/
	static CCamera* GetCamera(int num=0);
	
	/*!
		@brief	���ݎg�p���Ă���J�����I�u�W�F�N�g���擾
		@retval	�J�����g�J�����I�u�W�F�N�g�̃A�h���X
	**/
	static CCamera* GetCurrent();

	/*!
		@brief	�g�p����J�����ɐݒ�
		@param	num			[in] �g�p����J�����̔ԍ�
		@retval�@����
	**/

	static void SetCurrent(int num);
	/*!
		@brief	�g�p����J�����ɐݒ�
		@param	p			[in] �g�p����J�����̃A�h���X
		@retval�@����
	**/
	static void SetCurrent(CCamera *p);
	/*!
		@brief	�R���X�g���N�^
		@retval�@����
	**/
	CCamera();
	/*!
		@brief	�J�����̕����擾
		@retval�@�J�����̕��@
	**/
	float GetWhidth() const;
	/*!
		@brief	�J�����̍������擾
		@retval�@�J�����̍����@
	**/
	float GetHeight() const;
	/*!
		@brief	�J�����̕��ƍ����ݒ�
		@param	w			[in] ��
		@param	h			[in] ����
		@retval�@�����@
	**/
	void SetSize(float w,float h);

	/*!
		@brief	�����J�����Ńr���[�s��
		@param	eye			[in] �ʒu
		@param	at			[in] �����_
		@param	up			[in] ��x�N�g��
		@retval�@�����@
	**/
	void LookAt(const CVector3D &eye, const CVector3D &at, const CVector3D &up);

	/*!
		@brief	�ʒu�Ɖ�]�l�Ńr���[�s���ݒ�
		@param	pos			[in] �ʒu
		@param	rot			[in] ��]�l
		@retval�@�����@
	**/
	void SetTranseRot(const CVector3D &pos, const CVector3D &rot);
	/*!
		@brief	���f���s�񂩂�r���[�s���ݒ�
		@param	mtx			[in] ���f���s��
		@retval�@�����@
	**/
	void SetTranseRot(const CMatrix &mtx);

	/*!
		@brief	�r���[�s��𒼐ڐݒ�
		@param	m			[in] �r���[�s��
		@retval�@�����@
	**/
	void SetViewMatrix(const CMatrix &m);

	/*!
		@brief	�r���[�s����擾
		@retval�@�����@
	**/
	const CMatrix& GetViewMatrix() const {

		return m_mView;
	}

	
	/*!
		@brief	�r���[�s����擾
		@retval�@�����@
	**/
	CMatrix GetBuilbordMatrix() const;
	
	/*!
		@brief	�J�����̈ʒu���擾
		@retval�@�J�����̈ʒu
	**/
	CVector3D GetPos() const;

	/*!
		@brief	�J�����̌������擾
		@retval�@�J�����̌���
	**/
	CVector3D GetDir() const;
	/*!
	@brief	�J�����̌������擾
	@retval�@�J�����̌���
	**/
	CVector3D GetUp() const;
	/*!
	@brief	�������e�s��p
	@param	fovy			[in] ����
	@param	aspect			[in] �A�X�y�N�g��
	@param	zNear			[in] �O�ʋ���
	@param	zFar			[in] �w�ʋ���
	@retval�@
	**/
	void Perspective(float fovy, float aspect, float zNear, float zFar);
	void Ortho(float left, float right, float bottom, float top, float zNear, float zFar);
	/*!
		@brief	�ˉe�s��̐ݒ�
		@param	m			[in] �ˉe�s��
	**/
	void SetProjectionMatrix(const CMatrix &m);
	/*!
		@brief	2�����`��p�ˉe�s��̐ݒ�
		@param	m			[in] �ˉe�s��
	**/
	void SetProjectionMatrix2D(const CMatrix& m);
	/*!
		@brief	 �ˉe�s����擾
		@retval�@�ˉe�s��
	**/
	const CMatrix& GetProjectionMatrix() const{
		return m_mProj;
	}

	/*void SetScreenRect(const CRect& r) {
		m_screen = r;
	}
	CRect GetScreenRect() {
		return m_screen;
	}*/
	/*!
	@brief	�������e�s��p
	@param	x			[in] X�ʒu
	@param	y			[in] Y�ʒu
	@param	w			[in] ��
	@param	h			[in] ����
	*/
	void Viewport(float x, float y, float w, float h);
	const CMatrix& GetViewportMatrix() const {
		return m_mViewport;
	}
	const CRect& GetViewportRect() const {
		return m_viewportRect;
	}

	void Update2DProjectionMatrix();
	const CMatrix& Get2DProjectionMatrix() const {
		return m_m2D;
	}
	
};