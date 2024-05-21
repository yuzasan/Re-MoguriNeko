/*!
*	@brief	���f���̊�{�N���X
*
*
**/
#pragma once

#include "CMatrix.h"
#include "CTexture.h"
#include "CShader.h"
#include "CResource.h"
#include "Vertex.h"
class COBB;
//�A�j���[�V�����f�[�^�\����
struct SAnimData {
	int	start;	//�J�n�t���[��
	int end;	//�I���t���[��
};

#define NAME_STR_SIZE 256
/*!
@brief	�}�e���A���N���X
**/
class CMaterial : public CFragShaderCallback {
public:
	char m_name[NAME_STR_SIZE];
	CVector4D m_ambient;	//�A���r�G���g�J���[
	CVector4D m_diffuse;	//�f�t���[�Y�J���[
	CVector3D m_specular;	//�X�y�L�����[�J���[
	CVector3D m_emissive;	//���Ȕ���
	float m_shininess;		//�X�y�L�����[�W��
	float m_alpha;
	char m_texture_name[NAME_STR_SIZE];
	CVector2D m_st;
	CTexture *mp_texture;		//�e�N�X�`���[
	CShader* mp_shader;
public:
	CMaterial();
	CMaterial(const CMaterial& mat);
	/*!
	@brief	�}�e���A���i�̃e�N�X�`���[�j���

	@retval	����
	**/

	void Release();
	/*!
	@brief	�}�e���A���𔽉f
	@param	shader		[in]	�V�F�[�_�[�I�u�W�F�N�g
	@retval	����
	**/
	void Map();
	/*!
	@brief	�}�e���A���i�̃e�N�X�`���[�j�𖳌���
	@retval	����
	**/
	void Unmap();



	/*!
		@brief	�t���O�����g�V�F�[�_�[�ւ̃f�[�^�]��
				�h���}�e���A���N���X���`���I�[�o�[���C�h����ƓƎ��V�F�[�_�[�œ���
		@retval	����
	**/
	virtual void SendFragParam();

};
struct CCollTriangle {
	CVector3D	m_normal;		//�ڐG�ʂ̖@��
	CVector3D	m_vertex[3];	//�ڐG�ʂ̂R���_
	CVector3D	m_cross;		//�ڐG�_
	float	m_dist;				//�ʂ܂ł̋���(���̐��̏ꍇ�͂߂荞�ݗ�)
	int		m_material_id;		//�}�e���A��
	CCollTriangle(const CVector3D& normal, const CVector3D& v0, const CVector3D& v1, const CVector3D& v2, float dist, int material_id) {
		m_normal = normal;
		m_vertex[0] = v0; m_vertex[1] = v1; m_vertex[2] = v2;
		m_dist = dist;
		m_material_id = material_id;
	}
	CCollTriangle(const CVector3D& normal, const CVector3D& v0, const CVector3D& v1, const CVector3D& v2, const CVector3D& cross, float dist, int material_id) {
		m_normal = normal;
		m_vertex[0] = v0; m_vertex[1] = v1; m_vertex[2] = v2;
		m_cross = cross;
		m_dist = dist;
		m_material_id = material_id;
	}
	void Transform(const CMatrix &m) {
		m_normal = (m*m_normal).GetNormalize();
		m_vertex[0] = (m*CVector4D(m_vertex[0], 1.0f));
		m_vertex[1] = (m*CVector4D(m_vertex[1], 1.0f));
		m_vertex[2] = (m*CVector4D(m_vertex[2], 1.0f));
	}
};


class CModel : public  CResource {
public:
	CVector3D m_pos;	//���W		
	CVector3D m_rot;	//��]�l
	CVector3D m_scale;	//�X�P�[���l
	
	CVector3D m_baserot;	//��]�l

	CMatrix m_matrix;	//���[���h�i���f���j�s��
	CMatrix m_transMtx;	//���s�ړ��s��
	CMatrix m_rotMtx;	//��]�s��
	CMatrix m_scaleMtx;	//�X�P�[�����O�s��
protected:
	static const int PATH_SIZE = NAME_STR_SIZE;
	char	m_filePath[PATH_SIZE];
	void	MakePath(const char* filenPath);
	//CShader		*m_shader;	//�V�F�[�_�[

public:
	/*!
		@brief	�R���X�g���N�^

	**/
	CModel();
	/*!
		@brief �f�X�g���N�^
	*/
	virtual ~CModel() {

	}
	/*!
		@brief	���W��ݒ肷��
		@param	pos				[in] ���W
		@retval	����
	**/
	void SetPos(CVector3D pos){
		m_pos = pos;
	}
	/*!
		@brief	���W��ݒ肷��
		@param	x				[in] x���W
		@param	y				[in] y���W
		@param	z				[in] z���W
		@retval	����
	**/
	void SetPos(float x,float y,float z){
		m_pos.x = x;
		m_pos.y = y;
		m_pos.z = z;
	}	

	/*!
		@brief	���W���擾����
		@retval ���W
	**/
	CVector3D GetPos()const {
		return m_pos;
	}
	/*!
		@brief	��]�l��ݒ肷��
		@param	rot				[in] ��]�l
		@retval	����
	**/

	void SetRot(CVector3D rot){
		m_rot = rot;
	}

	/*!
		@brief	��b��]�l��ݒ肷��(���f�����O��t�ɂȂ��Ă��铙�p)
		@param	x				[in] x����]�l
		@param	y				[in] y����]�l
		@param	z				[in] z����]�l
		@retval	����
	**/
	void SetBasetRot(float x,float y,float z){
		m_baserot.x = x;
		m_baserot.y = y;
		m_baserot.z = z;
	}
	/*!
		@brief	��]�l��ݒ肷��
		@param	x				[in] x����]�l
		@param	y				[in] y����]�l
		@param	z				[in] z����]�l
		@retval	����
	**/
	void SetRot(float x,float y,float z){
		m_rot.x = x;
		m_rot.y = y;
		m_rot.z = z;
	}

	/*!
		@brief	��]�l���擾����
		@retval	��]�l
	**/
	CVector3D GetRot()const {
		return m_rot;
	}

	/*!
		@brief	�X�P�[����ݒ肷��
		@param	scale			[in] �X�P�[���l
		@retval	����
	**/

	void SetScale(CVector3D scale){
		m_scale = scale;
	}
	
	/*!
		@brief	�X�P�[���l��ݒ肷��
		@param	x				[in] x���X�P�[��
		@param	y				[in] y���X�P�[��
		@param	z				[in] z���X�P�[��
		@retval	����
	**/
	void SetScale(float x,float y,float z){
		m_scale.x = x;
		m_scale.y = y;
		m_scale.z = z;
	}

	/*!
		@brief	�X�P�[���l���擾��
		@retval	�X�P�[���l
	**/
	CVector3D GetScale()const {
		return m_scale;
	}

	/*!
		@brief	���s�ړ��s����擾����
		@retval	���s�ړ����s��
	**/
	CMatrix GetTransMatrix()const {
		return m_transMtx;
	}

	/*!
		@brief	��]�s����擾����
		@retval	��]�s��
	**/
	CMatrix GetRotMatrix()const {
		return m_rotMtx;
	}
	/*!
		@brief	�X�P�[�����O�s����擾����
		@retval	�X�P�[�����O�s��
	**/
	CMatrix GetScaleMatrix() const {
		return m_scaleMtx;
	}

	/*!
		@brief	���W�A��]�l�A�X�P�[���l���烏�[���h�s����쐬����
		@retval	����
	**/
	virtual void UpdateMatrix();


	/*!
		@brief	�w��̍s��Ŋe�{�[�����X�V����
		@retval	����
	**/
	virtual void UpdateMatrix(CMatrix& m) {}


	/*!
		@brief	���[���h�s���ݒ肷��
		@param	m				[in] �s��
		@retval	����
	**/
	void SetMatrix(CMatrix &m){
		m_matrix = m;
	}
	/*!
		@brief�@���[���h�s����擾����
		@retval	���[���h�s��
	**/
	CMatrix GetMatrix()const {
		return m_matrix;
	}

	/*!
		@brief	�t�@�C�����烂�f����ǂݍ���
		@retval	���ʁ@true���� false���s
	**/
	virtual bool Load(const char *path){ return false; }
	
	/*!
		@brief	�s����w�肵�ĕ`�悷��
		@param	m				[in] ���[���h�s��
		@retval	����
	**/
	virtual void Render(CMatrix &m)=0;

	/*!
		@brief	�`�悷��
		@retval	����
	**/
	virtual void Render() = 0;

	/*!
		@brief	�`�悷��
		@retval	����
	**/
	virtual void RenderBuilbord(CMatrix& m);

	virtual void RenderBuilbord();


	/*!
		@brief	���f����j������
		@retval	����
	**/
	virtual void Release()=0;


	/*
		@brief	�V�F�[�_�[�֊�{���i�ϊ��s��A���C�g�j��]������
		@retval	����
	*/
	static  void SendShaderParam(CShader * s,const CMatrix &m, const CMatrix &mv, const CMatrix &p);

	/*!
	@brief	�A�j���[�V�����X�V
	@retval	����
	**/
	virtual void UpdateAnimation(){ printf("�񓋍�"); }
	
	/*!
	@brief�@�A�j���[�V�����I������
	@retval	true:�I��
	**/
	virtual bool isAnimationEnd()const { printf("�񓋍�"); return false; };
	/*!
	@brief	�A�j���[�V�����̔ԍ����擾
	@retval	�Đ����ԍ��i0�`�j
	**/
	virtual int GetAnimationJam()const { printf("�񓋍�"); return 0; }
	/*!
	@brief	�A�j���[�V�����̑��x�Đ����x�ύX
	@retval	����
	**/
	virtual void SetAnimationSpeed(float s){ printf("�񓋍�"); }
	/*!
	@brief	�A�j���[�V�����̌��݃t���[�����擾
	@retval	�t���[��
	**/
	virtual float GetAnimationFrame()const { printf("�񓋍�"); return 0; }
	/*
	@brief	�A�j���[�V�����؂�ւ�
	@param	jam				[in] �A�j���[�V�����ԍ�
	@param	loop			[in] �A�j���[�V�������[�v�t���O
	@param	check			[in] ���ɍĐ������`�F�b�N
	@param	frame			[in] �؂�ւ���Ԏ���
	@retval	����
	**/
	virtual void ChangeAnimation(int jam, bool loop = true, bool check = true, float time = 0, float blendtime = 0.1){ printf("�񓋍�"); }
	/*
	@brief	�A�j���[�V�������擾
	@retval	�A�j���[�V������
	**/
	virtual int GetAnimationSize() const { printf("�񓋍�"); return 0; }
	virtual char* GetAnimationName(int jam) const { return nullptr; }


	/*!
	@brief	�{�[���A�m�[�h�̐����擾

	@retval	�{�[����
	**/
	virtual int GetBoneSize() const { printf("�񓋍�"); return 0; }

	/*!
	@brief	�{�[���̍s����擾
	@param	name			[in] �{�[����
	@param	local			[in] ���[�J���s��Ŏ擾�t���O
	@retval	����
	**/
	virtual CMatrix GetFrameMatrix(const char *name, bool local = false) const { printf("�񓋍�"); return CMatrix(); }
	/*!
	@brief	�{�[���̍s����擾
	@param	name			[in] �t���[���ԍ�
	@param	local			[in] ���[�J���s��Ŏ擾�t���O
	@retval	����
	**/
	virtual CMatrix GetFrameMatrix(int no, bool local = false) const { printf("�񓋍�"); return CMatrix(); }


	/*
	�@�{�[���̕␳�s���ݒ�
	 */
	virtual void SetFrameRevMatrix(const char *name, CMatrix &m) { printf("�񓋍�"); }
	/*
	�@�{�[���̕␳�s���ݒ�
	 */
	virtual void SetFrameRevMatrix(int no, CMatrix &m) { printf("�񓋍�"); }


	/*!
	@brief	�{�[�����w��̕����֌�����
	@param	no			[in] �{�[���ԍ�
	@param	mat			[in] ��]�s��
	**/
	virtual void BindFrameMatrix(int no, const CMatrix& mat) { printf("�񓋍�"); }
	/*!
	@brief	�{�[�����w��̕����֌�����
	@param	no			[in] �{�[�����O
	@param	mat			[in] ��]�s��
	**/
	virtual void BindFrameMatrix(const char* name, const CMatrix& mat) { printf("�񓋍�"); }


	/*!
	@brief	�{�[���̎w������ւ̌Œ������
	@param	no			[in] �{�[���ԍ�
	**/
	virtual void UnbindFrameMatrix(int no) { printf("�񓋍�"); }
	/*!
	@brief	�{�[���̎w������ւ̌Œ������
	@param	no			[in] �{�[�����O
	**/
	virtual void UnbindFrameMatrix(const char* name) { printf("�񓋍�"); }

	/*!
	@brief	�}�e���A�����擾
	@param	no				[in]�}�e���A���ԍ�
	@param	mesh			[in] ���b�V���ԍ�
	@retval	����
	**/
	virtual CMaterial *GetMaterial(int no)const { printf("�񓋍�"); return NULL; }

	/*!
	@brief	�}�e���A�����擾
	@param	no				[in]�}�e���A���ԍ�
	@param	mesh			[in] ���b�V���ԍ�
	@retval	����
	**/
	virtual CMaterial* GetMaterial(const char* name)const { printf("�񓋍�"); return NULL; }

	/*!
	@brief	�}�e���A����ݒ�
	@param	no				[n]�}�e���A���ԍ�
	@param	mesh			[in] ���b�V���ԍ�
	@retval	����
	**/
	virtual void SetMaterial(int no,CMaterial *m) { printf("�񓋍�"); }
	/*!
	@brief	�}�e���A�����̎擾
	@param	no				[in] �}�e���A���ԍ�
	@retval	�}�e���A���̃|�C���^
	**/
	virtual int GetMaterialSize() const { printf("�񓋍�"); return NULL; }
	/*!
		@brief	�����ƃ��f���̔���
		@param	c				[out] �ڐG�n�_
		@param	n				[out] �ڐG�ʂ̖@��
		@param	s				[in] �����̊J�n�n�_
		@param	e				[in] �����̏I���n�_
		@retval	true:�ڐG�@false:��ڐG
		**/
	virtual bool CollisionRay(CVector3D* c, CVector3D* n, const CVector3D& s, const CVector3D& e) const { return false; }

	/*!
	@brief	�����ƃ��f���̔���
	@param	s				[in] �����̊J�n�n�_
	@param	e				[in] �����̏I���n�_
	@retval	CCollTriangle	�Փ˂����ʃf�[�^
	**/
	virtual std::vector<CCollTriangle> CollisionRay(const CVector3D& s, const CVector3D& e)const {
		return std::vector<CCollTriangle>();
	}


	/*!
	@brief	���ƃ��f���̔���
	@param	center			[in] ���̒��S
	@param	radius			[in] ���̔��a
	@retval	CCollTriangle	�Փ˂����ʃf�[�^
	**/
	virtual std::vector<CCollTriangle> CollisionSphere(const CVector3D& center, float radius)const {
		return std::vector<CCollTriangle>();
	}



	/*!
	@brief	�J�v�Z���ƃ��f���̔���
	@param	top				[in] �J�v�Z���̓V��
	@param	bottom			[in] �J�v�Z���̒�
	@param	radius			[in] ���̔��a
	@retval	CCollTriangle	�Փ˂����ʃf�[�^
	**/
	virtual std::vector<CCollTriangle> CollisionCupsel(const CVector3D& top, const CVector3D& bottom, float radius)const{
		return std::vector<CCollTriangle>();
	}
	/*!
	@brief	OBB�ƃ��f���̔���
	@param	obb				[in] OBB
	@retval	CCollTriangle	�Փ˂����ʃf�[�^
	**/
	virtual std::vector<CCollTriangle> CollisionOBB(const COBB& obb)const {
		return std::vector<CCollTriangle>();
	}


	/*!
	@brief	���f���I�u�W�F�N�g�̐���[x,obj,fbx]
	@param	path			[in] ���f���t�@�C����
	@param	cutX			[in] �����蔻��p������
	@param	cutZ			[in] �����蔻��p������
	@retval ����:true ���s:false
	**/
	static CModel* CreateModel(const char *path, int cut_x = 0, int cut_y = 0, int cut_z = 0);



};
