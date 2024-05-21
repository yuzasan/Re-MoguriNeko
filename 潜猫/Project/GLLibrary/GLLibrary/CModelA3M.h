#pragma once
#include "../GLLibrary.h"

#ifdef USE_ATHOR_MODEL
#include "pugixml.hpp"
#endif
#include <list>
#define A3MVERSION	0x101
namespace A3M {
	/*
		�m�[�h�̎��
	*/
	enum E_NODE_TYPE {
		eNode,	//�ʏ�
		eMesh,	//���b�V��
	};
	enum E_VERTEX_TYPE {
		eStaticMesh,
		eSkinMesh,
	};
};

class CA3MMesh;
/*
	�m�[�h�N���X
*/
class CA3MNode {
#ifdef _GLLIBRARY_TOOL
public:
#else
protected:
#endif
	int	 m_no;			//�m�[�h�ԍ�
	int m_bone_no;
	char m_name[NAME_STR_SIZE];
	A3M::E_NODE_TYPE m_node_type;	//�m�[�h�̎��
	CMatrix m_matrix;
	CMatrix m_offset;
	//CMatrix m_bind_pose_inv;
	//CMatrix *mp_bone;
	CMatrix m_local_matrix;
	CVector3D m_pos;
	CQuaternion m_rot;
	CVector3D m_scale;
	bool m_bind;
	CMatrix m_bind_matrix;
	bool m_visibility;
	int m_animation_layer;
	char m_euler_order[3];		//��]�s�񏇔�
	CA3MMesh* mp_mesh;
	CA3MNode* mp_child;	//�q�t���[��
	CA3MNode* mp_parent;	//�e�t���[��
	CA3MNode* mp_next;		//�Z��t���[��
	CA3MNode* mp_prev;
public:
	CA3MNode();
	CA3MNode(int no, const char* name);

	CA3MNode(const CA3MNode& n);
	virtual ~CA3MNode();
	char* GetName() {
		return m_name;
	}
	int GetNo() { return m_no;}
	A3M::E_NODE_TYPE GetNodeType() { return m_node_type;}

	void SetMatrix(const CMatrix &m) {		
		m_matrix = m;
	}
	CMatrix GetMatrix() const{		
		return m_matrix;
	}
	void SetAnimationLayer(int layer) {
		m_animation_layer = layer;
	}
	/*
	  ���O����m�[�h���擾
	*/
	static CA3MNode* FindNode(const char* name, CA3MNode* n);
	/*
	  �ԍ�����m�[�h���擾
	*/
	static CA3MNode* FindNode(const int no, CA3MNode*n);

	virtual void Release();
	/*
	  �f�o�b�O�p
	*/
	static void PintMatrix(CA3MNode* n);
	static void PrintNode(CA3MNode* n,int level);
	void SetVisibility(bool f) { m_visibility = f; }

	virtual void Draw(){};

	friend class CModelA3M;
	friend class CA3MMesh;

};

class CA3MColl {
public:
	CVector3D m_max;
	CVector3D m_min;
	CVector3D m_cut;
	CVector3D m_length;
	CVector3D* m_colmin;
	CVector3D* m_colmax;
public:
	CA3MColl();
	~CA3MColl();
	int GetIndex(int x, int y, int z) const{
		return x + (z * m_cut.ix) + (y * m_cut.iz * m_cut.ix);
	}
	void CalcRange(CVector3D* min, CVector3D* max,const CAABB& aabb)const;
};
/*
�|���S���N���X
�}�e���A�����ɂ܂Ƃ߂�
*/
class CA3MPoly {
public:
	unsigned int*	mp_use_skin;
	MY_VERTEX*		mp_vertex;
	unsigned int	m_vertex_cnt;
	unsigned int	m_material;
	GLuint			m_buffer;
//	GLuint			m_vao;
	const CA3MColl*	m_col_cut;
	unsigned long* m_colmask;
	std::vector<int>* m_colIdex;
public:
	CA3MPoly(unsigned int* use_skin);
	~CA3MPoly();
	int Load(const char* buf);
	/*
	�`��
	*/
	void Draw(const CShader* shader);

	void CreateCollisionIndex(const CA3MColl* c);
	bool CollisionRay(CVector3D* c, CVector3D* n, const CVector3D& s, const CVector3D& e, const CVector3D& dir, float* pLength, const CVector3D& start, const CVector3D& end);
	void CollisionRay(std::vector<CCollTriangle>& out, const CVector3D& s, const CVector3D& e, const CVector3D& dir, const CVector3D& start, const CVector3D& end);
	void CollisionSphere(std::vector<CCollTriangle>& out, const CVector3D& center, float radius, const CVector3D& start, const CVector3D& end);
	void CollisionCupsel(std::vector<CCollTriangle>& out, const CVector3D& top, const CVector3D& bottom, float radius, const CVector3D& start, const CVector3D& end);
	
};
/*
/*
  [Mesh]
*/
class CA3MMesh {
public:
	std::list<CA3MPoly*> m_poly_list;
	unsigned long m_poly_cnt;
	const CA3MColl* m_col_cut;
	bool *mp_enable_animation;
	unsigned int m_use_skin;
	struct SBoneOffset {
		//CA3MNode* m_node;
		unsigned int m_bone_no;
		CMatrix m_offset;
		//SBoneOffset(CA3MNode* node, const CMatrix& offset) {
		//	m_node = node;
		//	m_offset = offset;
		//}
		SBoneOffset(int bone_no, const CMatrix& offset) {
			m_bone_no = bone_no;
			m_offset = offset;
		}
	};
	std::list<SBoneOffset> m_offset;
public:
	CA3MMesh(bool* p_enable_animation);
//	CA3MMesh(const CA3MMesh& mesh);
	~CA3MMesh();
	CA3MMesh& operator = (const CA3MMesh& mesh);
	int Load(const char* buf);
	virtual void Release();
	/*
	�`��
	*/
	void Draw(std::vector<CMaterial*>& materialList,CMatrix* send_matrix, CMatrix* bone_matrix_base,int bone_size, const CMatrix& mv, const CMatrix& m, const CMatrix& lm);
	void CreateCollisionIndex(const CA3MColl* c);

	bool CollisionRay(CVector3D* c, CVector3D* n, const CVector3D& s, const CVector3D& e, float* pLength, CMatrix& matrix);
	void CollisionRay(std::vector<CCollTriangle>& out, const CVector3D& s, const CVector3D& e, CMatrix& matrix);
	void CollisionSphere(std::vector<CCollTriangle>& out,const CVector3D& center, float radius, CMatrix& matrix);
	void CollisionCupsel(std::vector<CCollTriangle>& out,const CVector3D& top, const CVector3D& bottom, float radius, CMatrix& matrix);
	friend class CA3MMesh;

};

struct CA3MAnimationKey {
	int m_frame;						//0
	union {
		CMatrix m_matrix;
		struct {
			CVector3D m_position;		//1,2.3
			CQuaternion m_rotation;		//4,5,6,7
			CVector3D m_scale;			//8,9,10
			float pat[6];				//11,12,13,14,15,16
		};
	};
	CA3MAnimationKey():m_frame(0){}
	CA3MAnimationKey(const CVector3D& pos,const CQuaternion& rotation, const CVector3D& scale):
		m_frame(0),m_position(pos),m_rotation(rotation),m_scale(scale) {}
};
struct CA3MAnimationAction {
	int m_size;
	CA3MAnimationKey* m_keys;
	~CA3MAnimationAction() {
		delete[] m_keys;
	}
};

struct CA3MAnimationSet {
	char m_name[NAME_STR_SIZE];
	float m_fps;
	union {
		int m_time[2];
		struct {
			int m_start;
			int m_end;
		};
	};
	int m_end_base;
	std::map<int, CA3MAnimationAction*> m_bones;
	~CA3MAnimationSet() {
		for (auto& a : m_bones) {
			delete a.second;
		}
		m_bones.clear();
	}
};
/*
	�A�j���[�V����
*/
class CA3MAnimation {

private:


	int m_jam;
	float m_speed;
	bool m_loop;
	float m_frame;
	float m_blend;					//��ԌW��
	float m_blend_rate;
	std::vector<CA3MAnimationSet*> *mp_animation_list;
public:
	CA3MAnimation(std::vector<CA3MAnimationSet*>* animation_list);
	~CA3MAnimation();
	bool ChangeAnimation(int jam, bool loop, bool check, float time, float blend_time);
	void UpdateAnimation();
	bool isAnimationEnd();
	int GetAnimationJam();
	void SetAnimationSpeed(float s);
	int GetAnimationFrame();
	friend class CModelA3M;
};
/*
	A3M�p���f���t�@�C��
*/
class CModelA3M : public CModel {
private:
	std::vector<CMaterial*> m_material_list;
	CA3MNode* mp_root;
	unsigned int			m_bone_num;
	CMatrix* m_send_matrix;
	CMatrix* m_bone_matrix;
	CMatrix* m_old_matrix;
	std::vector<CA3MAnimation> m_animation;
	int		m_animation_size;
	std::vector<CA3MAnimationSet*> m_animation_list;
	std::vector<CA3MNode*> m_mesh_list;
	std::vector<CA3MNode*> m_node_list;
	//A3M::E_VERTEX_TYPE	m_vertex_type;
	CA3MColl	*m_col_cut;
	CVector3D	m_min;
	CVector3D	m_max;

	bool	m_enable_animation;
	//CAnimation* animation;
	CA3MNode* LoadNode(FILE* fp, CA3MNode* parent, CA3MNode* prev);
	int LoadAnimation(FILE* fp);
	void DrawMesh(CA3MNode* node, const CMatrix& view_matrix);
	void ReleaseNode(CA3MNode* node);
	int WriteNode(FILE* fp, CA3MNode* node);
	int WriteAnimation(FILE* fp);
	void LeapAnimationKey(float* key, int start, int end, float vs, float ve, int max, int step);
	void LeapAnimationKey(CA3MAnimationKey* key, int start, int end);

#ifdef USE_ATHOR_MODEL
	CA3MNode* DuplicateFBXMesh(FbxNode* pNode,int& no,CA3MNode* parent, CA3MNode* prev);
	bool DuplicateFBXAnimation(FbxNode* pNode, const CModelFBX::AnimationInfo* info,unsigned int no);
	CA3MNode* DuplicateXMesh(const CModelX* model, CFrameBone* pNode, int& no, CA3MNode* parent, CA3MNode* prev);
	bool DuplicateXAnimation(CXAnimationSet* set, float fps);
	struct SMeshNodeSet {
		pugi::xml_node mp_toy_node;
		CA3MNode* mp_node;
		SMeshNodeSet(pugi::xml_node& t, CA3MNode* n) :mp_toy_node(t){
			mp_node = n;
		}
	};

	static void ReadMatrix(CMatrix* mat, pugi::xml_node& pNode);
	static void ReadMatrix(CA3MNode* node, pugi::xml_node pNode);
	std::vector<SMeshNodeSet> m_toy_mesh_list;
	void LoadToyAnimationSet(pugi::xml_node& pNode,CA3MAnimationSet *set);
	void LoadToyAnimation(pugi::xml_node& pNode);
	void LoadToyMesh(pugi::xml_node& pNode, CA3MNode* node, CA3MColl* col_cut);
	void LoadToyMaterial(pugi::xml_node& pNode);
public:
	CA3MNode* LoadToyNode(pugi::xml_node& pNode, int& no,int &bone_no, CA3MNode* parent, CA3MNode* prev);
	bool LoadToyModel(const char* file);


private:
#endif
	void CalcBoneMatrix(CA3MNode *node);
	void CreateCollisionIndex(int cntx,int cuty,int cutz);

	CA3MNode* DuplicateNode(CA3MNode* pNode, CA3MNode* parent, CA3MNode* prev);
	void OutputFrameInfo(FILE* fp, CA3MNode* node, int depth);
public:
	CModelA3M();
	~CModelA3M();
	CModelA3M(const CModelA3M& m);
	void operator = (const CModelA3M& m);
	CModelA3M(const CModelObj& m);
#ifdef USE_ATHOR_MODEL
	CModelA3M(const CModelX& m);
	CModelA3M(const CModelFBX& m);
	bool AddAnimationFBX(const char* filepath);
	bool AddAnimationX(const char* filepath);
#endif
	void RemoveAnimation(int jam);

	void Release();
	/*!
		@brief	���f���̓ǂݍ���
		@param	filePath			[in] ���f���t�@�C��
		@param	cut_x				[in] �Փ˔���p������X
		@param	cut_y				[in] �Փ˔���p������Y
		@param	cut_z				[in] �Փ˔���p������Z
		@retval�@����
	**/
	bool Load(const char* filePath,int cut_x,int cut_y,int cut_z);
	/*!
		@brief	���f���̏�������
		@param	filePath			[in] ���f���t�@�C��
		@retval�@����
	**/
	void Write(const char* filePath);

	/*!
		@brief	���f���̕`��i�s��w��j
		@param	m			[in] ���f���s��
		@retval�@����
	**/
	void Render(CMatrix& m);

	/*!
		@brief	���f���̕`��
		@retval�@����
	**/
	void Render();

	/*!
		@brief	�s��̍X�V
		@retval�@����
	**/

	void UpdateMatrix();

	/*!
		@brief	�w��̃��f���s��ōX�V����
		@retval	����
	**/
	void UpdateMatrix(CMatrix& m);


	/*!
		@brief	�A�j���[�V�����X�V
		@retval	����
	**/
	void UpdateAnimation();
	/*!
		@brief�@�A�j���[�V�����I������
		@retval	true:�I��
	**/
	bool isAnimationEnd();
	bool isAnimationEnd(int layer);


	/*!
		@brief	�A�j���[�V�����̔ԍ����擾
		@retval	�Đ����ԍ��i0�`�j
	**/
	int GetAnimationJam();
	int GetAnimationJam(int layer);

	/*!
		@brief	�A�j���[�V�����̑��x�Đ����x�ύX
		@retval	����
	**/
	void SetAnimationSpeed(float s);
	void SetAnimationSpeed(int layer,float s);

	/*!
		@brief	�A�j���[�V�����؂�ւ�
		@param	jam				[in] �A�j���[�V�����ԍ�
		@param	loop			[in] �A�j���[�V�������[�v�t���O
		@param	check			[in] ���ɍĐ������`�F�b�N
		@param	frame			[in] �؂�ւ���Ԏ���
		@retval	����
	**/

	void ChangeAnimation(int jam, bool loop = true, bool check = true, float time = 0, float blendtime = 0.1f);
	void ChangeAnimation(int layer, int jam, bool loop = true, bool check = true, float time = 0, float blendtime = 0.1f);

	/*!
		@brief	�A�j���[�V�����̌��݃t���[�����擾
		@retval	�t���[��
	**/
	float GetAnimationFrame();
	float GetAnimationFrame(int layer);

	/*!
		@brief	�A�j���[�V�����̐����擾
		@retval	�A�j���[�V�����̐�
	**/
	int GetAnimationSize() const;


	/*!
	@brief	�{�[�����w��̕����֌�����
	@param	no			[in] �{�[���ԍ�
	@param	mat			[in] ��]�s��
	**/
	void BindFrameMatrix(int no, const CMatrix& mat);

	/*!
	@brief	�{�[�����w��̕����֌�����
	@param	no			[in] �{�[�����O
	@param	mat			[in] ��]�s��
	**/
	void BindFrameMatrix(const char* name, const CMatrix& mat);


	/*!
	@brief	�{�[���̎w������ւ̌Œ������
	@param	no			[in] �{�[���ԍ�
	**/
	void UnbindFrameMatrix(int no);

	/*!
	@brief	�{�[���̎w������ւ̌Œ������
	@param	no			[in] �{�[�����O
	**/
	void UnbindFrameMatrix(const char* name);

	/*
		�m�[�h�̍s����擾
	 */
	CMatrix GetFrameMatrix(const char* name, bool local = false) const;

	/*
	�@�{�[���̍s����擾
	 */
	CMatrix GetFrameMatrix(int no, bool local = false) const;


	/*!
	@brief	�w��̃m�[�h���擾
	@param	no				[in] �m�[�h�ԍ�
	@retval	�m�[�h�̃|�C���^
	**/
	CA3MNode* GetNode(int no) const;


	/*!
	@brief	�w��̃m�[�h���擾
	@param	name			[in] �m�[�h�̖��O
	@retval	�m�[�h�̃|�C���^
	**/
	CA3MNode* GetNode(const char* name) const;


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
	CMaterial* CModelA3M::GetMaterial(const char* name) const;

	/*!
	@brief	�}�e���A����ݒ�
	@param	no				[n]�}�e���A���ԍ�
	@param	mesh			[in] ���b�V���ԍ�
	@retval	����
	**/
	virtual void SetMaterial(int no, CMaterial* m);
	/*!
	@brief	�}�e���A�����̎擾
	@param	no				[in] �}�e���A���ԍ�
	@retval	�}�e���A���̃|�C���^
	**/
	int GetMaterialSize() const {
		return (int)m_material_list.size();
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
	@brief	�{�[���̊K�w�\�����t�@�C���֏o�͂���
	@param	file_name		[in] �������ݐ�
	@retval	�ڐG�|���S����
	**/
	void OutputFrameInfo(const char* file_name);


	/*!
		@brief	�{�[�����̎擾
		@retval�@����
	**/

	int GetBoneSize() const { return m_bone_num; }

	/*!
		@brief	�e�N�X�`���[�̍����ւ�
		@param	filepath			[in] �摜�t�@�C��
		@param	material_num		[in] �Ώۃ}�e���A���ԍ�
		@retval�@����
	**/
	bool LoadTexture(const char* filepath, int material_num);

	/*!
		@brief	�A�j���[�V�����f�[�^�̎擾
		@param	jam					[in] �A�j���[�V�����̎��
		@retval�@�A�j���[�V�����f�[�^
	**/
	CA3MAnimationSet* GetAnimationSet(int jam) {
		return m_animation_list[jam];
	}

	/*!
		@brief	�A�j���[�V�����f�[�^�̕��т�����
		@param	a					[in] �A�j���[�V�����̎��
		@param	b					[in] �A�j���[�V�����̎��
		@retval�@����
	**/
	void SwapAnimationSet(int a, int b);

	/*!
		@brief	�A�j���[�V�������̎擾
		@retval�@�A�j���[�V�������
	**/
	CA3MAnimation* GetAnimation() {
		return &m_animation[0];
	}

	friend class CModelA3M;

};