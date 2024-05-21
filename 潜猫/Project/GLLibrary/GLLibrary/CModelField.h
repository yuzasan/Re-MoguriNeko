/*!
*	@brief	field�t�@�C���i�V�[�����X�}�b�v�j�p���f���N���X
*			ConvertField.exe��obj�t�@�C����field�t�@�C���փR���o�[�g
*
**/
#pragma once

#include "CModel.h"
#include <vector>
#if _MSC_VER < 1600
#include <process.h>
#else
#define USE_THREAD_CLASS
#include <thread>
#endif
#include <mutex>
class COBB;
//SFieldModelHeader
//meshOffset * �u���b�N��
//SMaterial * �}�e���A����
//CMesh *�@���b�V����
//

struct SFieldMeshHeader {
	unsigned long *offSet;
};
struct SFieldModelHeader {
	char fileType[2];
	unsigned short verversion;
	unsigned long cutX;
	unsigned long cutZ;
	float			cutLength;
	unsigned long	colCut;
	float			colCutLength;
	CVector3D	min;
	CVector3D	max;
	unsigned long meshHeaderOffset;
	unsigned long materialCnt;
	unsigned long materialOffset;
};

class SFieldMaterial {
public:
	char m_name[128];
	char m_texName[128];
	CVector4D m_ambient;	//�A���r�G���g�J���[
	CVector4D m_diffuse;	//�f�t���[�Y�J���[
	CVector3D m_specular;	//�X�y�L�����[�J���[
	CVector3D m_emissive;	//���Ȕ���
	float m_shininess;		//�X�y�L�����[�W��
	SFieldMaterial(){
		memset(this, 0, sizeof(SFieldMaterial));
	}
};
struct SFieldVertex {
	CVector3D		m_vertex;
	CVector3D		m_normal;
	CVector2D		m_texcode;

};
struct CFieldColIndex {
	unsigned long m_size;
	unsigned long *m_idx;
	CFieldColIndex() :m_size(0),m_idx(NULL){

	}
	~CFieldColIndex(){
		if (m_idx) delete m_idx;
	}
};
class CFieldPoly {
public:

	unsigned long	m_materialIdx;
	unsigned long	m_vertexCnt;
	SFieldVertex			*m_pVertex;
	unsigned long	m_cut;
	CFieldColIndex	*m_colIdx;
	CVector3D		*colNormal;

	unsigned long*	m_colmask;
	GLuint			m_buffer;
	GLuint			m_vao;
	CFieldPoly();
	~CFieldPoly();
	void Load(FILE *fp);
	void Render(CShader *shader);

	bool CollisionRay(CVector3D *c, CVector3D *n, const CVector3D &s, const CVector3D &e, float *pLength, int raws, int rawe, int cols, int cole);
	int CollisionRay(std::vector<CCollTriangle>& out, const CVector3D& s, const CVector3D& e, int raws, int rawe, int cols, int cole,int matNo);
	int CollisionSphere(std::vector<CCollTriangle>& out, const CVector3D &center, float radius, int raws, int rawe, int cols, int cole,int matNo);
	int CollisionCupsel(std::vector<CCollTriangle>& out, const CVector3D &top, const CVector3D &bottom, float radius, int raws, int rawe, int cols, int cole, int matNo);
	int CollisionAABB(std::vector<CCollTriangle>& out, const CVector3D& aabb_max, const CVector3D& aabb_min, const CMatrix& trans, int raws, int rawe, int cols, int cole, int matNo);

};
class CFieldMesh {
public:
	bool		m_load;
	CVector3D		m_min;
	CVector3D		m_max;
	unsigned long m_polyCnt;
	CFieldPoly*		m_poly;
	CFieldMesh() :m_load(false),m_poly(NULL){
		m_polyCnt = 0;
	}
	~CFieldMesh() {
		Release();
	}
	void Load(FILE *fp);
	void Render(CShader *shader, int matNo);
	void Release();

	bool CollisionRay(CVector3D *c, CVector3D *n, const CVector3D &s, const CVector3D &e, const CVector3D &dir,const CVector3D &box_min, const CVector3D &box_max, float *pLength, const SFieldModelHeader &header);
	int CollisionRay(std::vector<CCollTriangle>& out, const CVector3D& s, const CVector3D& e, const CVector3D& dir, const CVector3D& box_min, const CVector3D& box_max,  const SFieldModelHeader& header);
	int CollisionSphere(std::vector<CCollTriangle>& out, const CVector3D &center, float radius, const SFieldModelHeader &header);
	int CollisionCupsel(std::vector<CCollTriangle>& out, const CVector3D &top, const CVector3D &bottom, float radius, const SFieldModelHeader &header);
	int CollisionAABB(std::vector<CCollTriangle>& out, const CVector3D& aabb_max, const CVector3D& aabb_min, const CVector3D& check_max, const CVector3D& check_min, const CMatrix& trans, const SFieldModelHeader& header);

};


class CModelField : public CModel {
	SFieldModelHeader m_header;
	SFieldMeshHeader m_offSet;
	char			m_fileName[256];
	CVector3D		m_center;
	int				m_posX;
	int				m_posZ;

	int				m_minX;
	int				m_maxX;
	int				m_minZ;
	int				m_maxZ;

	int				m_OldminX;
	int				m_OldmaxX;
	int				m_OldminZ;
	int				m_OldmaxZ;
	float			m_length;
	CMaterial		*m_pMaterial;
	int				m_materialCnt;
	CFieldMesh		*m_pMesh;
	FILE *m_fp;
	int				m_raw;
	int				m_col;

	bool			m_isLoading;
	bool			m_isDestroy;
	std::mutex		m_mutex;			//!�}���`�X���b�h�R���W�������
#ifdef USE_THREAD_CLASS
	std::thread		*m_pLoadThread;
#else
	HANDLE			m_threadHandle;
	static void		_loadThread(void *arg) {
		((CModelField*)arg)->LoadMeshProc();
	}

#endif


	/*
	@brief	����̒��_�f�[�^��ǂݍ���
	@param	x					[in] ���ʒu��
	@param	z					[in] ���ʒu��
	@retval	����true ���sfalse
	*/
	bool	LoadMesh(int x, int z);


	int GetIdx(int x, int z)const {
		return x + z*m_header.cutX;
	}
	void CalcRenderRange(CVector3D center);
public:
	/*!
	@brief	�R���X�g���N�^
	**/
	CModelField();

	/*!
	@brief	�f�X�g���N�^
	**/
	~CModelField();

	/*!
	@brief	�R���X�g���N�^
	**/
	CModelField(const CModelField &f);

	/*!
	@brief	field�t�@�C�����烂�f���f�[�^��ǂݍ���
	@param	path				[in] �t�@�C����
	@retval	����true ���sfalse
	**/
	bool	Load(const char *path);
	/*!
	@brief	����̏ꏊ�̃��b�V����ǂ݂���
	@param	pos				[in]�ǂݍ��ݏꏊ
	@retval	����
	**/
	void LoadMesh(CVector3D pos);
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
	void	Render();
	/*!
	@brief	���f���f�[�^��j������
	@retval	����
	**/
	void	Release();
	void SetDrawLength(float l) {
		m_length = l;
	}
	/*!
	@brief	�}�e���A�����擾
	@param	no				[in]�}�e���A���ԍ�
	@retval	����
	**/
	CMaterial* GetMaterial(int no)const;

	/*!
	@brief	�}�e���A�����擾
	@param	name				[in]�}�e���A����
	@retval	����
	**/
	CMaterial* GetMaterial(const char* name)const;
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
	bool CollisionRay(CVector3D *c, CVector3D *n, const CVector3D &s, const CVector3D &e)const;

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
	std::vector<CCollTriangle> CollisionSphere(const CVector3D &center, float radius)const;

	/*!
	@brief	OBB�ƃ��f���̔���
	@param	obb				[in] OBB
	@retval	CCollTriangle	�Փ˂����ʃf�[�^
	**/
	std::vector<CCollTriangle> CollisionOBB(const COBB& obb)const;

	/*!
	@brief	�J�v�Z���ƃ��f���̔���
	@param	top				[in] �J�v�Z���̓V��
	@param	bottom			[in] �J�v�Z���̒�
	@param	radius			[in] ���̔��a
	@retval	CCollTriangle	�Փ˂����ʃf�[�^
	**/
	std::vector<CCollTriangle> CollisionCupsel(const CVector3D &top, const CVector3D &bottom, float radius)const;


	void		LoadMeshProc();
};