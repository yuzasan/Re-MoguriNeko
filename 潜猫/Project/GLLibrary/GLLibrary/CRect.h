#pragma once
#include "CVector.h"
class CRect {
public:
	float m_left;
	float m_top;
	float m_right;
	float m_bottom;
	union {
		struct {
			float m_x;
			float m_y;
			float m_width;
			float m_height;
		};
		struct {
			CVector2D m_pos;
			CVector2D m_size;
		};
	};
	CRect();

	/**
	* @fn
	* �R���X�g���N�^�i��`�ݒ�j
	* @brief ��`��ݒ肷��
	* @param l	[in]�@��
	* @param t	[in]�@��
	* @param r	[in]�@�E
	* @param b	[in]�@��
	*/
	CRect(float l, float t, float r, float b);
	/**
	* @fn
	* �R���X�g���N�^�i��`�ݒ�j
	* @brief �ʒu�Ƒ傫���ŋ�`��ݒ肷��
	* @param pos	[in]�@�ʒu
	* @param size	[in]�@�傫��
	*/
	CRect(const CVector2D& pos, const CVector2D& size);
	CRect(const CRect& rect);

	CRect operator=(const CRect& rect);
	/**
	* @fn
	* ��`��ݒ肷��
	* @brief ��`��ݒ肷��
	* @param l	[in]�@��
	* @param t	[in]�@��
	* @param r	[in]�@�E
	* @param b	[in]�@��
	*/
	void Set(float l, float t, float r, float b);
	/**
	* @fn
	* ��`��ݒ肷��
	* @brief �ʒu�Ƒ傫���ŋ�`��ݒ肷��
	* @param pos	[in]�@�ʒu
	* @param size	[in]�@�傫��
	**/
	void Set(const CVector2D& pos, const CVector2D& size);

	/**
	* @fn
	* ��`�̑傫�����擾����
	* @brief ��`�̑傫�����擾����
	* @return ��`�̃T�C�Y
	**/
	CVector2D GetSize() { return m_size; }

	/**
	* @fn
	* ��`�̍ŏ��ʒu���擾����
	* @brief ��`�̍ŏ��ʒu���擾����
	* @return ����̍��W
	**/	
	CVector2D GetMin() { return CVector2D(m_left, m_top); }

	/**
	* @fn
	* ��`�̍ő�ʒu���擾����
	* @brief ��`�̍ő�ʒu���擾����	* @return �E���̍��W
	**/
	CVector2D GetMax() { return CVector2D(m_top, m_bottom); }

};