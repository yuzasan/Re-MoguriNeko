
/*!
*		@brief	�V�F�[�_�[�p�����N���X
*
**/
#pragma once
#include "GL.h"
#include "CVector.h"
#include "CMatrix.h"


class CLight {
public:
	enum {
		eLight_None=0,		//��������
		eLight_Direction,	//���s��
		eLight_Point,		//�S�����|�C���g��
		eLight_Spot,		//�X�|�b�g���C�g
		eLight_Hemi,		//����
	};
	static const int LIGHT_MAX = 5;
	static CVector3D	m_lightPos[LIGHT_MAX];
	static CVector3D	m_lightDir[LIGHT_MAX];
	static CVector3D	m_AmbientColor[LIGHT_MAX];
	static CVector3D	m_DiffuseColor[LIGHT_MAX];
	static int			m_Type[LIGHT_MAX];
	static float		m_Attenuation[LIGHT_MAX];
	static float		m_RadiationAngle[LIGHT_MAX];
	static int			m_Lighting;
	struct SFog {
		CVector4D m_Color;
		float	m_Near;
		float	m_Far;
	};
	static SFog m_fog;
	
private:
public:
	static CMatrix m_shadowMatrix;
	
	/*!
		@brief	�����̗L����ݒ肷��
		@retval	����
	**/
	static void SetLighting(bool f){
		if(f) m_Lighting = 1;
		else m_Lighting=0;
	}
	/*!
		@brief	�����̗L�����擾����
		@retval	
	**/
	static int GetLighting(){
		return m_Lighting;
	}
	/*!
		@brief	�����̈ʒu��ݒ肷��
		@param	no		[in] �����ԍ�
		@param	v		[in] �ʒu
		@retval	����
	**/
	static void SetPos(int no,CVector3D v){
		m_lightPos[no] = v;
	}

	/*!
		@brief	�����̌�����ݒ肷��
		@param	no		[in] �����ԍ�
		@param	v		[in] �����x�N�g��
		@retval	����
	**/
	static void SetDir(int no,CVector3D v){
		m_lightDir[no] = v;
	}

	
	/*!
		@brief	�����̎�ނ�ݒ肷��
		@param	no		[in] �����ԍ�
		@param	type	[in] �����̎��
		@retval	����
	**/
	static void SetType(int no,int type){
		m_Type[no] = type;
	}

	
	/*!
		@brief	�����̌�������ݒ肷��
		@param	no				[in] �����ԍ�
		@param	Attenuation		[in] ������
		@retval	����
	**/
	static void SetAttenuation(int no,float Attenuation){
		m_Attenuation[no] = Attenuation;
	}

	/*!
		@brief	�����̍L�p��ݒ肷��
		@param	no				[in] �����ԍ�
		@param	RadiationAngle	[in] �p�x
		@retval	����
	**/
	static void SetRadiationAngle(int no,float RadiationAngle){
		m_RadiationAngle[no] = RadiationAngle;
	}

	/*!
		@brief	�����̐F��ݒ肷��
		@param	no				[in] �����ԍ�
		@param	a				[in] �A���r�G���g�i�����j
		@param	d				[in] �f�t���[�Y�i�g�U���ˌ��j
		@retval	����
	**/
	static void SetColor(int no,CVector3D a,CVector3D d){
		m_AmbientColor[no]  = a;
		m_DiffuseColor[no]  = d;
	}


	/*!
		@brief	�����̈ʒu���擾����
		@param	no				[in] �����ԍ�
		@retval	�ʒu
	**/
	static CVector3D GetPos(int no){
		return m_lightPos[no];
	}
	
	/*!
		@brief	�����̃A���r�G���g�J���[���擾����
		@param	no				[in] �����ԍ�
		@retval	�A���r�G���g�i�����j
	**/
	static CVector3D GetAmbientColor(int no){
		return m_AmbientColor[no];
	}
	
	/*!
		@brief	�����̃f�t���[�Y�J���[���擾����
		@param	no				[in] �����ԍ�
		@retval	�f�t���[�Y�i�g�U���ˌ��j
	**/
	static CVector3D GetDiffuseColor(int no){
		return m_DiffuseColor[no];
	}


	
	/*!
		@brief	�����̈ʒu�z��̃A�h���X���擾����i�V�F�[�_�[�p�j
		@retval	�ʒu�z��̃A�h���X
	**/
	static CVector3D *GetPosPointer(){
		return m_lightPos;
	}

	/*!
		@brief	�����̌����z��̃A�h���X���擾����i�V�F�[�_�[�p�j
		@retval	�����z��̃A�h���X
	**/
	static CVector3D *GetDirPointer(){
		return m_lightDir;
	}

	
	/*!
		@brief	�����̃A���r�G���g�J���[�z��̃A�h���X���擾����i�V�F�[�_�[�p�j
		@retval	�A���r�G���g�J���[�z��̃A�h���X
	**/
	static CVector3D *GetAmbientColorPointer(){
		return m_AmbientColor;
	}
	
	/*!
		@brief	�����̃f�t���[�Y�J���[�z��̃A�h���X���擾����i�V�F�[�_�[�p�j
		@retval	�f�t���[�Y�J���[�z��̃A�h���X
	**/
	static CVector3D *GetDiffuseColorPointer(){
		return m_DiffuseColor;
	}
	
	/*!
		@brief	�����̎�ޔz��̃A�h���X���擾����i�V�F�[�_�[�p�j
		@retval	��ޔz��̃A�h���X
	**/
	static int *GetTypeColorPointer(){
		return m_Type;
	}
	
	/*!
		@brief	�����̌������z��̃A�h���X���擾����i�V�F�[�_�[�p�j
		@retval	�������z��̃A�h���X
	**/
	static float *GetAttenuationPointer(){
		return m_Attenuation;
	}

	
	/*!
		@brief	�����̍L�p�z��̃A�h���X���擾����i�V�F�[�_�[�p�j
		@retval	�L�p�z��̃A�h���X
	**/
	static float *GetRadiationAnglePointer(){
		return m_RadiationAngle;
	}

	/*!
	@brief	�t�H�O��ݒ肷��
	@param	color			[in] �F
	@param	n				[in] �J�n����
	@param	f				[in] �Œ�����
	@retval	����
	**/
	static void SetFogParam(CVector4D& color, float n,float f){
		m_fog.m_Color = color;
		m_fog.m_Near = n;
		m_fog.m_Far = f;
	}
	/*!
	@brief	�t�H�O���擾
	@retval	�t�H�O�p�����[�^
	**/
	static SFog& GetFogParam(){
		return m_fog;
	}

};