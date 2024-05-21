/*!
*	@brief	OBJ�p���f���N���X
*
*
**/
#pragma once
#include "GL.h"
#include "CTexture.h"
#include "CModel.h"
#include "vector"

class COBB;
struct SObjModelHeader {
	char fileType[2];
	unsigned short verversion;
	int		m_materialCnt;
	SVector3D m_cut;
};


class CMeshObj {
public:
	struct SVertex {
		CVector3D v;
		CVector3D n;
		CVector2D t;
	};
	SVertex* mp_vertex_array;
	std::vector<int>* m_colIdex;
	CVector3D* m_colmin;
	CVector3D* m_colmax;
	int			m_vertexNum;		//���_��
	CVector3D* m_normal;
	CVector3D	m_max;
	CVector3D	m_min;
	CVector3D	m_cut;
	unsigned long *m_colmask;
	CVector3D	m_length;
	GLuint		m_buffer;
	GLuint		m_vao;
	
	CMeshObj();
	void Render(CMaterial* m);
	void Release();
	void CreatCollsionIndex(CVector3D cut);
	bool CollisionRay(CVector3D *c, CVector3D *n, const CVector3D &s, const CVector3D &e,const CVector3D& dir,float *pLength);
	int CollisionRay(std::vector<CCollTriangle>& out, const CVector3D& s, const CVector3D& e, const CVector3D& dir, int matNo);
	int CollisionSphere(std::vector<CCollTriangle>& out, const CVector3D &center, float radius,int matNo);
	int CollisionCupsel(std::vector<CCollTriangle>&out, const CVector3D &top, const CVector3D &bottom, float radius,int matNo);
	int CollisionAABB(std::vector<CCollTriangle>& out, const CVector3D& aabb_max, const CVector3D& aabb_min, const CVector3D& check_max, const CVector3D& check_min,const CMatrix &trans,int matNo);
	int GetIndex(int x, int y, int z) {
		return x + (z*m_cut.ix) + (y*m_cut.iz*m_cut.ix);
	}
};
/*!
		@brief	OBJ�t�@�C���p���f���N���X
**/
class CModelObj : public CModel {
private:
	
	CMeshObj *m_mesh;
	int		m_materialCnt;
	CVector3D	m_cut;
	CMaterial**	m_material;		//�}�e���A���f�[�^
	/*!
		@brief	�}�e���A���f�[�^�̓ǂݍ���
		@param	path		[in]	�t�@�C����
		@retval	����
	**/
	bool	LoadMaterial(char *path);
public:

	/*!
		@brief	�R���X�g���N�^
	**/
	CModelObj();

	CModelObj(const CModelObj& m);
	CModelObj& operator=(const CModelObj& m);
	/*!
		@brief	�f�X�g���N�^
	**/
	~CModelObj();

	
	/*!
	@brief	OBJ�t�@�C�����烂�f���f�[�^��ǂݍ���
	@param	path				[in] �t�@�C����
	@param	cut_x				[in] X������
	@param	cut_y				[in] Y������
	@param	cut_z				[in] Z������
	@retval	����
	**/
	bool	Load(const char *path,int cut_x=1,int cut_y=1, int cut_z=1);

	/*!
		@brief	�`����s��
		@param	m				[in] ���[���h�s��
		@retval	����
	**/
	void	Render(CMatrix &m);

	/*!
		@brief	�`�悷��
		@retval	����
	**/
	void Render();

	/*!
		@brief	���f���f�[�^��j������
		@retval	����
	**/
	void	Release();


	/*!
	@brief	�}�e���A���̎擾
	@param	no				[in] �}�e���A���ԍ�
	@retval	�}�e���A���̃|�C���^
	**/
	CMaterial* GetMaterial(int no) const;
	/*!
	@brief	�}�e���A���̎擾
	@param	name				[in] �}�e���A�����O
	@retval	�}�e���A���̃|�C���^
	**/
	CMaterial* GetMaterial(const char* name) const;

	/*!
	@brief	�}�e���A����ݒ�
	@param	no				[n]�}�e���A���ԍ�
	@param	m				[in] �}�e���A��
	@retval	����
	**/
	virtual void SetMaterial(int no, CMaterial* m)const {
		m_material[no] = m;
	}
	/*!
	@brief	�}�e���A�����̎擾
	@param	no				[in] �}�e���A���ԍ�
	@retval	�}�e���A���̃|�C���^
	**/
	int GetMaterialSize() const {
		return m_materialCnt;
	}

	/*!
	@brief	�����ƃ��f���̔���
	@param	c				[out] �ڐG�n�_
	@param	n				[out] �ڐG�ʂ̖@��
	@param	s				[in] �����̊J�n�n�_
	@param	e				[in] �����̏I���n�_
	@retval	true:�ڐG�@false:��ڐG
	**/
	bool CollisionRay(CVector3D* c, CVector3D* n, const CVector3D& s, const CVector3D& e)const;

	/*!
	@brief	�����ƃ��f���̔���
	@param	s				[in] �����̊J�n�n�_
	@param	e				[in] �����̏I���n�_
	@retval	CCollTriangle	�Փ˂����ʃf�[�^
	**/
	std::vector<CCollTriangle> CollisionRay(const CVector3D& s, const CVector3D& e)const;


	/*!
	@brief	���ƃ��f���̔���
	@param	center			[in] ���̒��S
	@param	radius			[in] ���̔��a
	@retval	CCollTriangle	�Փ˂����ʃf�[�^
	**/
	std::vector<CCollTriangle> CollisionSphere(const CVector3D& center, float radius)const;



	/*!
	@brief	�J�v�Z���ƃ��f���̔���
	@param	top				[in] �J�v�Z���̓V��
	@param	bottom			[in] �J�v�Z���̒�
	@param	radius			[in] ���̔��a
	@retval	CCollTriangle	�Փ˂����ʃf�[�^
	**/
	std::vector<CCollTriangle> CollisionCupsel(const CVector3D& top, const CVector3D& bottom, float radius)const;

	/*!
	@brief	OBB�ƃ��f���̔���
	@param	obb				[in] OBB
	@retval	CCollTriangle	�Փ˂����ʃf�[�^
	**/
	std::vector<CCollTriangle> CollisionOBB(const COBB& obb)const;

	/*!
	@brief	���f���I�u�W�F�N�g�̐���[obj]
	@param	path			[in] ���f���t�@�C����
	@param	raw					[in] X������
	@param	col					[in] Z������
	@retval ����:���f���I�u�W�F�N�g ���s:null
	**/
	static CModelObj* CreateModel(char *path, int cut_x=1, int cut_y=1, int cut_z=1);


	friend class CModelA3M;
};