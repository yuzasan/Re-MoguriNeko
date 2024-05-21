#pragma once
#include "GL.h"
class CVector3D;
class CVector4D;
class CMatrix;
//�x�N�g���̒�`�Ɗe��v�Z
struct SVector2D{
	union {
		float v[2];
		struct {
			float x;
			float y;
		};
		struct {
			int ix;
			int iy;
		};
	};
};
class CVector2D : public SVector2D{
public:
	CVector2D(){}
	CVector2D(float x, float y);
	CVector2D(const CVector2D& v);
	
	//�P�����Z�q-
	CVector2D operator - () const;

	//�x�N�g�������Z( this - v )
	CVector2D operator - (const CVector2D& v) const;
	//�x�N�g�������Z( this - v )
	void operator -= (const CVector2D& v);
	
	//�x�N�g�������Z( this + v )
	CVector2D operator + (const CVector2D& v) const;
	
	//�x�N�g�������Z( this + v )
	void operator += (const CVector2D& v);

	//�x�N�g���|���Z( this * f )
	CVector2D operator * (const float& f) const;
	//�x�N�g���|���Z( this * f )
	void operator *= (const float& f);
	//�x�N�g������Z( this * f )
	CVector2D operator / (const float& f) const;
	
	bool operator == (const CVector2D& v) const;
	bool operator != (const CVector2D& v) const;

	//�x�N�g���̓���
	float Dot(const CVector2D& v)const;
	static float Dot(const CVector2D& v1, const CVector2D& v2);
	//�x�N�g���̊O��
	float Cross(const CVector2D& v)const;
	static float Cross(const CVector2D& v1, const CVector2D& v2);


	//�x�N�g���̑傫��
	float Length() const;
	//�x�N�g���̑傫���̂Q��
	float LengthSq() const;
	//���g��P�ʃx�N�g���ɂ���
	void Normalize();
	//�P�ʃx�N�g����ԋp
	CVector2D GetNormalize() const;

	const static CVector2D up;
	const static CVector2D down;
	const static CVector2D left;
	const static CVector2D right;
	const static CVector2D zero;
	const static CVector2D one;
};

typedef struct SVector3D{
	union {
		float v[3];
		struct {
			float x;
			float y;
			float z;
		};
		struct {
			float r;
			float g;
			float b;
		};
		struct {
			long ix;
			long iy;
			long iz;
		};
	};
}SColorRGB;
class CVector3D : public SVector3D{
public:
	
	CVector3D(){}
	CVector3D(float x, float y, float z);
	CVector3D(const CVector3D& v);
	CVector3D(const SVector3D& v);
	CVector3D(const SVector2D& v);
	CVector3D(const CVector4D&);
	CVector3D operator = (const CVector4D&);

	//�x�N�g�������Z( this - v )
	CVector3D operator - () const;


	//�x�N�g�������Z( this - v )
	CVector3D operator - (const CVector3D& v) const;
	//�x�N�g�������Z( this - v )
	void operator -= (const CVector3D& v);
	
	//�x�N�g�������Z( this + v )
	CVector3D operator + (const CVector3D& v) const;
	
	//�x�N�g�������Z( this + v )
	void operator += (const CVector3D& v);

	//�x�N�g���|���Z( this * f )
	CVector3D operator * (const float& f) const;
	//�x�N�g���|���Z( this * f )
	void operator *= (const float& f);
	//�x�N�g������Z( this * f )
	CVector3D operator / (const float& f) const;	
	//�x�N�g���O��( this �~ vr )
	CVector3D operator * (const CVector3D& vr) const;
	
	bool operator == (const CVector3D& v) const;
	bool operator != (const CVector3D& v) const;

	//�x�N�g���̓���
	float Dot(const CVector3D& v)const;
	static float Dot(const CVector3D& v1, const CVector3D& v2);
	//�x�N�g���̊O��
	CVector3D Cross(CVector3D& v)const;
	static CVector3D Cross(const CVector3D& v1, const CVector3D& v2);
	//�x�N�g���̑傫��
	float Length() const;
	//�x�N�g���̑傫���̂Q��
	float LengthSq() const;
	static CVector3D Leap(const CVector3D& s, const CVector3D& e, float t);
	static CVector3D Sleap(const CVector3D& s, const CVector3D& e, float t);
	/*
	CVector3D GetEuler() {
		CVector3D ret(0, 0, 0);
		ret.x = -asin(y);
		ret.y = atan2(x,z);
		return ret;
	}
	void SetDir(CVector3D v) {
		x = sin(v.y) * cos(v.x);
		y = -sin(v.x);
		z = cos(v.y) * cos(v.x);

	}
	*/
	//���g��P�ʃx�N�g���ɂ���B���K��
	void Normalize();
	//�P�ʃx�N�g����ԋp
	CVector3D GetNormalize() const;

	const static CVector3D up;
	const static CVector3D down;
	const static CVector3D left;
	const static CVector3D right;
	const static CVector3D front;
	const static CVector3D back;
	const static CVector3D zero;
	const static CVector3D one;

};
typedef struct SVector4D{
	union {
		float v[4];
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		struct {
			float r;
			float g;
			float b;
			float a;
		};
	};
}SColorRGBA;

class CVector4D : public SVector4D{
public:
	CVector4D(){}
	CVector4D(float x, float y, float z, float w);
	CVector4D(const CVector3D& v, float w);
	//�x�N�g�������Z( this - v )
	CVector4D operator - (const CVector4D& v) const;
	//�x�N�g�������Z( this - v )
	void operator -= (const CVector4D& v);

	//�x�N�g�������Z( this + v )
	CVector4D operator + (const CVector4D& v) const;

	//�x�N�g�������Z( this + v )
	void operator += (const CVector4D& v);

};
typedef CVector4D CColorRGBA; 


class CQuaternion : public SVector4D {
public:
	//�R���X�g���N�^
	CQuaternion() {
		x = 0; y = 0; z = 0; w = 1;
	}
	//�x�N�g���Ƃ̐�
	CVector3D operator* (const CVector3D &v) const;
	//�N�H�[�^�j�I�����m�̐�
	CQuaternion operator *(const CQuaternion &a) const;
	//X����]
	void RotationX(float a);
	//Y����]
	void RotationY(float a);
	//Z����]
	void RotationZ(float a);
	//�C�ӎ���]
	void RotationAxis(float ax, float ay, float az, float angle);
	//��]�s�񂩂�N�H�[�^�j�I����ݒ�
	void SetRotateMatrix(const CMatrix &m);
	//�����x�N�g������N�H�[�^�j�I����ݒ�
	void SetVecter(CVector3D &v);
	//�w��̏ꏊ�ւ̃N�H�[�^�j�I�����쐬
	void LookAt(CVector3D &p, CVector3D &at);
	//�傫�����擾
	float Length();
	//���K��
	void Normalize();
	//�I�C���[�p����N�H�[�^�j�I���ݒ�
	void SetEuler(float ax, float ay, float az);
	void SetEuler(const CVector3D& e);
	//�I�C���[�p���擾
	CVector3D GetEuler(int type) const;
	//����
	static float Dot(const CQuaternion &q0, const CQuaternion &q1);
	//���`���
	static CQuaternion Slerp(const CQuaternion &q0, const CQuaternion &q1, float t);
};