/*!
 *	@brief		�摜�Ǘ��N���X
 *	
 */

#pragma once

#include "CRect.h"
#include "CMatrix.h"
#include "CTexture.h"

struct TexAnim {
	int pat;		//�g�p�摜No�i�摜�̍���O�`�j
	int cnt;		//���̉摜�܂ł̑ҋ@�t���[��
};

#define ANIMDATA(data) { data,sizeof(data) / sizeof(data[0])}

struct TexAnimData {
	TexAnim* pData;
	int		size;
};


class CImage : public CResource {
public:
	CTexture* mp_texture;
	CVector2D	m_pos;		//�ʒu(pixel)
	CVector2D	m_center;	//���S�ʒu(pixel)
	CVector2D	m_size;	//�T�C�Y(pixel)
	CRect		m_src_rect;	//�\������`
	float		m_ang;		//��]�l
	bool		m_flipH;	//���E���]
	bool		m_flipV;	//�㉺���]
	CColorRGBA	m_color;	//�F
	int			m_width;	//�e�N�X�`���[��
	int			m_height;	//�e�N�X�`���[����	
	TexAnimData* m_animData;	//�A�j���[�V�����f�[�^
	int			m_animJam;	//�A�j���[�V�����F���
	int			m_animIndex;	//�A�j���[�V�����F�ʒu
	float		m_animCount;	//�A�j���[�V�����F����
	bool		m_animLoop;		//�A�j���[�V�����F���[�v
	float		m_animSpeed;	//�A�j���[�V�����F���x
	int			m_pat_width;	//�A�j���[�V�����F�ꖇ�̕�
	int			m_pat_height;	//�A�j���[�V�����F�ꖇ�̍���
	int			m_cut_width;	//�A�j���[�V�����F��


public:
	CImage();

	virtual ~CImage();
	/*!
	@brief	������Z�I�[�o�[���[�h
	@param	img			[in] �ǂݍ��ݍς݂̉摜�I�u�W�F�N�g
	**/
	CImage& operator = (const CImage& img);
	/*!
		@brief	�t�@�C������摜��ǂݍ��ށi24bitbmp,png�Ή��j
		@param	d			[in] TexAnimData�z��̐擪�A�h���X
		@param	width		[in] �ꖇ������̕�
		@param	height		[in] �ꖇ������̍���
		@retval	����
	**/

	bool Load(const char* filename, TexAnimData* d = nullptr, int width = 0, int height = 0);
	/*!
	@brief	�ǂݍ���
	@param	filename	[in] �摜�t�@�C��
	@param	d			[in] TexAnimData�z��̐擪�A�h���X
	@param	width		[in] �ꖇ������̕�
	@param	height		[in] �ꖇ������̍���
	**/
	static CImage* CreateImage(const char* filename, TexAnimData* d = nullptr, int width = 0, int height = 0);


	/*!
		@brief	�\���ʒu��ݒ肷��
		@param	x		[in] ���ʒu
		@param	y		[in] �c�ʒu
		@retval	����
	**/
	void SetPos(float x,float y) {
		m_pos.x = x; m_pos.y = y;
	}
	/*!
		@brief	�\���ʒu��ݒ肷��
		@param	p		[in] ���W
		@retval	����
	**/
	void SetPos(const CVector2D& p) {
		m_pos = p;
	}

	CVector2D* GetPosPointer()
	{
		return &m_pos;
	}

	/*!
		@brief	��]�̒��S�ʒu��ݒ肷��
		@param	x		[in] ���ʒu
		@param	y		[in] �c�ʒu
		@retval	����
	**/
	void SetCenter(float x,float y) {
		m_center.x = x; m_center.y = y;
	}/*!
		@brief	��]�̒��S�ʒu��ݒ肷��
		@param	p		[in] �ʒu
		@retval	����
	**/
	void SetCenter(const CVector2D& p) {
		m_center = p;
	}

	/*!
		@brief	�\���T�C�Y��ݒ肷��
		@param	w		[in] ��
		@param	h		[in] ����
		@retval	����
	**/
	void SetSize(float w,float h) {
		m_size.x = w; m_size.y = h;
	}
	/*!
	@brief	�\���T�C�Y��ݒ肷��
	@param	p		[in] ��,����
	@retval	����
**/
	void SetSize(const CVector2D& s) {
		m_size = s;
	}
	CVector2D GetSize() {
		return m_size;
	}
	/*!
		@brief	�\������`��ݒ�
		@param	left	[in] ���ʒu
		@param	top		[in] ��ʒu
		@param	right	[in] �E�ʒu
		@param	bottom	[in] ���ʒu
		@retval	����
	**/
	void SetRect(float left, float top, float right, float bottom);

	/*!
		@brief	��]�l��ݒ�
		@param	ang		[in] ��]�l
		@retval	����
	**/
	void SetAng(float ang){
		m_ang = ang;
	}

	/*!
		@brief	���E���]�t���O��ݒ�
		@param	f		[in] true:�������] false:�ʏ�
		@retval	����
	**/
	void SetFlipH(bool f) {
		m_flipH = f;
	}
	/*!
		@brief	�㉺���]�t���O��ݒ�
		@param	f		[in] true:�㉺���]�@false:�ʏ� 
		@retval	����
	**/
	void SetFlipV(bool f) {
		m_flipV = f;
	}

	/*!
		@brief	�F��ݒ�
		@param	r		[in] �Ԑ���
		@param	g		[in] �ΐ���
		@param	b		[in] ����
		@param	a		[in] �s�����x
		@retval	����
	**/
	void SetColor(float r, float g, float b, float a);

	/*!
	@brief	�A�j���[�V�����̍X�V
	**/
	void UpdateAnimation();
	/*!
	@brief	�A�j���[�V�����f�[�^�̃Z�b�g
	@param	d			[in] TexAnimData�z��̐擪�A�h���X
	@param	width		[in] �ꖇ������̕�
	@param	height		[in] �ꖇ������̍���
	**/
	void AttachAnimationData(TexAnimData* d, int width, int height);
	/*!
	@brief	�A�j���[�V�����؂�ւ�
	@param	num			[in] �A�j���[�V�����ԍ�
	@param	loop		[in] �A�j���[�V�������[�v�t���O
	@param	index		[in] �J�n�ʒu
	@param	check		[in] ���ɍĐ����Ȃ�؂�ւ��Ȃ��i�ăX�^�[�g���Ȃ��j
	**/
	void ChangeAnimation(int num, bool loop = true, int index = 0, bool check = true);
	/*!
	@brief	�A�j���[�V�������x�ύX
	@param	speed		[in] �Đ����x�i�f�t�H���g1�j
	**/
	void SetAnimationSpeed(float speed);
	/*!
	@brief	�A�j���[�V�����I���`�F�b�N
	@retval	�I���Ȃ�true
	**/
	bool CheckAnimationEnd();
	/*!
	@brief	�Đ����̃A�j���[�V�����f�[�^�̎擾�v�t���O
	@retval	���݂̃A�j���[�V�����f�[�^
	**/
	const TexAnim* GetAnimationParam() const {
		return &m_animData[m_animJam].pData[m_animIndex];
	}
	/*!
	@brief	���݂̍Đ��ʒu���擾
	@retval	�Đ��ʒu
	**/
	int GetIndex() {
		return m_animIndex;
	}
	/*!
	@brief	���݂̃J�E���g���擾
	@retval	�J�E���g
	**/
	int GetCount() {
		return (int)m_animCount;
	}
	/*!
	@brief	�`��
	@retval	����
	**/
	void Draw();



	/*!
		@brief	�����摜�̘A���`��J�n�i�X�R�A��}�b�v�`�b�v�Ŏg�p�j
		@retval	����
	**/
	void DrawBegin();

	/*!
		@brief	�����摜�̘A���`��
		@retval	����
	**/
	void DrawS();
	/*!
		@brief	�����摜�̘A���`��I��
		@retval	����
	**/
	void DrawEnd();

	/*!
	@brief	�摜���m�̏Փ˔���
	@param	img		[in] �ՓˑΏە�
	@retval	�ڐG�Ftrue�@,��ڐG:false
	**/
	bool CollisionImage(const CImage &img);


	/*!
	@brief	�摜�Ƌ�`�̏Փ˔���
	@param	rect		[in] �Փˋ�`
	@retval	�ڐG�Ftrue�@,��ڐG:false
	**/
	bool CollisionRect(const CRect &rect);


	/*!
	@brief	�摜�Ɠ_�̏Փ˔���
	@param	img		[in] �ՓˑΏە�
	@retval	�ڐG�Ftrue�@,��ڐG:false
	**/
	bool CollisionPoint(const CVector2D &pos);

	void Release();

	virtual unsigned int GetDataSize() {
		return m_width * m_height * 4;
	};

};