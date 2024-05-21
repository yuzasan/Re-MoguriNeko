#include "CCollision.h"
#include "CCamera.h"
#include "CShader.h"
#include <float.h>

void CAABB::Draw(const CVector4D& color) const{
	SVector3D v[8] = {
		{ min.x,max.y,max.z },
		{ min.x,-max.y,max.z },
		{ max.x,-max.y,max.z },
		{ max.x,max.y,max.z },
		{ min.x,max.y,min.z },
		{ min.x,-max.y,min.z },
		{ max.x,-max.y,min.z },
		{ max.x,max.y,min.z },
	};
	int idx[6][4] = {
		{ 0,1,2,3 },
		{ 4,7,6,5 },
		{ 0,4,5,1 },
		{ 3,2,6,7 },
		{ 0,3,7,4 },
		{ 1,5,6,2 },
	};
	CMatrix pvm = CCamera::GetCurrent()->GetProjectionMatrix() * CCamera::GetCurrent()->GetViewMatrix();

	CShader* shader = CShader::GetInstance("Solid");
	shader->Enable();


	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "PVWMatrix"), 1, GL_FALSE, pvm.f);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "Color"), 1, color.v);

	glEnableVertexAttribArray(CShader::eVertexLocation);

	glVertexAttribPointer(CShader::eVertexLocation, 3, GL_FLOAT, GL_FALSE, 0, &v);

	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, idx);


	glDisableVertexAttribArray(CShader::eVertexLocation);


	shader->Disable();

}
COBB::COBB(const CVector3D& center, const CVector3D& axis_x, const CVector3D& axis_y, const CVector3D& axis_z, const CVector3D& length) {
	m_center = center;
	m_axis[0] = axis_x; m_axis[1] = axis_y; m_axis[2] = axis_z;
	m_length = length;
}
COBB::COBB(const CVector3D& center, const CVector3D& rot, const CVector3D& length) {
	m_center = center;
	CMatrix mat = CMatrix::MRotation(rot);
	m_axis[0] = mat.GetLeft();
	m_axis[1] = mat.GetUp();
	m_axis[2] = mat.GetFront();
	m_length = length;
}

void COBB::Transeform(const CMatrix &mtx) {
	CMatrix mat = GetMatrix();
	mat = mat * mtx;
	m_length.x = mat.GetLeft().Length();
	m_length.y = mat.GetUp().Length();
	m_length.z = mat.GetFront().Length();
	m_axis[0] = mat.GetLeft() / m_length.x;
	m_axis[1] = mat.GetUp() / m_length.y;
	m_axis[2] = mat.GetFront() / m_length.z;
	m_center = mtx * CVector4D(m_center,1);
	
}

CMatrix COBB::GetMatrix()
{
	return CMatrix::MTranselate(m_center)
		* CMatrix(m_axis[0].x, m_axis[1].x, m_axis[2].x, 0,
			m_axis[0].y, m_axis[1].y, m_axis[2].y, 0,
			m_axis[0].z, m_axis[1].z, m_axis[2].z, 0,
			0, 0, 0, 1)
		* CMatrix::MScale(m_length);
}

void COBB::Draw(const CVector4D& color)const
{
	/*
	float f[16] = 
	{
		m_axis[0].x*m_length[0],m_axis[0].y,m_axis[0].z,0,
		m_axis[1].x,m_axis[1].y*m_length[1],m_axis[1].z,0,
		m_axis[2].x,m_axis[2].y,m_axis[2].z*m_length[2],0,
		m_center.x,m_center.y,m_center.z,1
	};
	*/
	SVector3D v[8] = {
		{-1,1,1},
		{-1,-1,1},
		{1,-1,1},
		{1,1,1},
		{-1,1,-1},
		{-1,-1,-1},
		{1,-1,-1},
		{1,1,-1},
	};
	int idx[6][4] = {
		{0,1,2,3},
		{4,7,6,5},
		{0,4,5,1},
		{3,2,6,7},
		{0,3,7,4},
		{1,5,6,2},
	};
	CMatrix pvm = CCamera::GetCurrent()->GetProjectionMatrix() * CCamera::GetCurrent()->GetViewMatrix() * 
		CMatrix::MTranselate(m_center)*
		CMatrix(m_axis[0].x, m_axis[1].x, m_axis[2].x, 0,
		m_axis[0].y, m_axis[1].y, m_axis[2].y, 0,
		m_axis[0].z, m_axis[1].z, m_axis[2].z, 0,
		0, 0, 0, 1)*CMatrix::MScale(m_length);

	CShader* shader = CShader::GetInstance("Solid");
	shader->Enable();


	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "PVWMatrix"), 1, GL_FALSE, pvm.f);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "Color"), 1, color.v);

	glEnableVertexAttribArray(CShader::eVertexLocation);

	glVertexAttribPointer(CShader::eVertexLocation, 3, GL_FLOAT, GL_FALSE, 0, &v);

	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, idx);


	glDisableVertexAttribArray(CShader::eVertexLocation);


	shader->Disable();

}


bool CCollision::CollisionRect(const CRect &rect1, const CRect &rect2) {
	if (rect1.m_left <= rect2.m_right && rect1.m_right >= rect2.m_left &&
		rect1.m_top <= rect2.m_bottom && rect1.m_bottom >= rect2.m_top)
		return true;
	return false;
}

bool CCollision::CollisionRectPoint(const CRect &rect, const CVector2D point) {
	if (rect.m_left <= point.x && rect.m_right >= point.x &&
		rect.m_top <= point.y && rect.m_bottom >= point.y)
		return true;
	return false;

}
bool CCollision::CollisionShpere(const CVector3D &centerA, float radiusA, const CVector3D &centerB, float radiusB, float* dist, CVector3D* dir) {
	CVector3D v = centerB-centerA;
	float l = v.LengthSq();
	if (l < (radiusA + radiusB) * (radiusA + radiusB)) {
		if (dist || dir) {
			float ll = sqrtf(l);
			if (dist) *dist = ll;
			if (dir) *dir = v / ll;
		}
		return true;
	}
	return false;
}
bool CCollision::CollisionShpere(const CVector2D& centerA, float radiusA, const CVector2D& centerB, float radiusB, float* dist, CVector2D* dir)
{
	CVector2D v = centerB - centerA;
	float l = v.LengthSq();
	if (l < (radiusA + radiusB) * (radiusA + radiusB)) {
		if (dist || dir) {
			float ll = sqrtf(l);
			if (dist) *dist = ll;
			if (dir) *dir = v / ll;
		}
		return true;
	}
	return false;
}
bool CCollision::TriangleIntersect(const CVector3D &c,const CVector3D &v0, const CVector3D &v1,const  CVector3D &v2,const CVector3D &n){

	if(CVector3D::Dot(CVector3D::Cross(v1-v0, c-v0),n)<0) return false;
	if(CVector3D::Dot(CVector3D::Cross(v2-v1, c-v1),n)<0) return false; 
	if(CVector3D::Dot(CVector3D::Cross(v0-v2, c-v2),n)<0) return false;
	return true;
}
//�����ƎO�p�`�|���S���Ƃ̏Փ˔���
bool CCollision::IntersectTriangleRay(CVector3D *corss, const CVector3D &p1, const CVector3D &p2, const CVector3D &v0, const CVector3D &v1, const CVector3D &v2, float *pt) {
	CVector3D e1, e2, normal;

	e1 = v1 - v0;
	e2 = v2 - v0;

	//�ʂ̖@�������߂�
	normal = CVector3D::Cross(e1, e2);
	if (normal.LengthSq() <= 0) return false;
	normal.Normalize();
	return IntersectTriangleRay(corss, p1, p2, v0, v1, v2, normal,pt);
}
//�����ƎO�p�`�|���S���Ƃ̏Փ˔���
bool CCollision::IntersectTriangleRay(CVector3D *corss, const CVector3D &p1, const CVector3D &p2, const CVector3D &v0, const CVector3D &v1, const CVector3D &v2, const CVector3D &normal,float *pt) {
	CVector3D pv1,pv2;


	//�n�_����|���S����̂���n�_�i�ǂ��ł������j�ւ̃x�N�g��
	pv1 = p1-v0;
	//�I�_����|���S����̂���n�_�i�ǂ��ł������j�ւ̃x�N�g��
	pv2 = p2-v0;

	//�|���S���̖@���Ƃ̓��ς����߂�
	float d1 = CVector3D::Dot(pv1,normal);
	float d2 = CVector3D::Dot(pv2,normal);

	//�|���S�����ђʂ��Ă��Ȃ�
	if(d1*d2>=0) return false;

	//�n�_����|���S���܂ł̋����Ɛ����̒����̔䗦�����߂�
	//�ڒn�n�_���o���̂Ɏg�p����
	float t = d1/(d1-d2);
	if( *pt < t ) return false;

	//������ђʂ��Ă���ꍇ�͏Փ˂��Ă��Ȃ����Ƃɂ���
	if(t<0) return false;

	//�����ƕ��ʂ̐ڒn�n�_�����߂�
	CVector3D c = p1+(p2-p1)*t;

	//�ڒn�n�_���O�p�`�|���S���ォ���ׂ�
	if(!TriangleIntersect(c,v0,v1,v2,normal)) return false;
	
	if(pt) *pt = t;
	if(corss) *corss = c;

	return true;
}

CVector3D CCollision::PointOnLineSegmentNearestPoint(const CVector3D &v0,const CVector3D &v1,const CVector3D &point){
	CVector3D V = v1-v0;
	float a = V.x*V.x+V.y*V.y+V.z*V.z;
	//�����̎n�_�ƏI�_�������ꏊ
	if(a==0) return v0;
	CVector3D VP = point-v0;
	float b = CVector3D::Dot(V,VP);
	float t = b/a;
	//v0���ɉ����ꍇ�͋߂��_��V0��
	if(t<0) return v0;
	//V1���ɉ����ꍇ�͋߂��_��V1��
	if(t>1) return v1;
	//t=<1 t>=0 �̏ꍇ�͐�����ɋ߂��_������ 
	return v0 + V*t;


}

CVector2D CCollision::PointOnLineSegmentNearestPoint(const CVector2D &v0,const CVector2D &v1,const CVector2D &point){
	CVector2D V = v1-v0;
	float a = V.x*V.x+V.y*V.y;
	//�����̎n�_�ƏI�_�������ꏊ
	if(a==0) return v0;
	CVector2D VP = v0-point;
	float b = CVector2D::Dot(V,VP);
	float t = -(b/a);
	//v0���ɉ����ꍇ�͋߂��_��V0��
	if(t<0) return v0;
	//V1���ɉ����ꍇ�͋߂��_��V1��
	if(t>1) return v1;
	//t=<1 t>=0 �̏ꍇ�͐�����ɋ߂��_������ 
	return v0 + V*t;


}
//-----------------------------------------------------------------------------
bool CCollision::CollisionTriangleSphere(const CVector3D &v0, const CVector3D &v1, const CVector3D &v2, const CVector3D &center, float radius, CVector3D *cross, float *length)
{

	CVector3D V1(v1 - v0);
	CVector3D V2(v2 - v1);
	CVector3D N;


	N = CVector3D::Cross(V1, V2);
	if (N.LengthSq() <= 0) return false;
	N.Normalize();
	return CollisionTriangleSphere(v0, v1, v2,  N, center, radius, cross,length);
}
bool CCollision::CollisionTriangleSphere(const CVector3D &v0, const CVector3D &v1, const CVector3D &v2, const CVector3D &N,const CVector3D &center, float radius, CVector3D *cross, float *length)
{
   
	 
	 CVector3D V = center - v0;
	 //���ʂƓ_�̋��������߂�
	 float Dist = CVector3D::Dot(V,N);
  
	//���̔��a��藣��Ă���ꍇ�͐ڐG����
	 if(fabsf(Dist) > radius) return false;

	 //�_���畽�ʏ�ɐ����ɉ��낵���n�_�����߂�
	 CVector3D Point =  center - ( N * Dist );
	 
	 
	 //��L�̓_���O�p�`�|���S�����Ȃ�ڐG���Ă���
	 if(TriangleIntersect( Point, v0, v1, v2 , N)) {
		if(cross) *cross = Point;
		if(length) *length = fabsf(Dist);
		return true;
	}



	 //�e�ӂɋ����������Ă���\��������
	 //�P�ӂ��Ƃɋ��ƕӂ̍ŒZ���������߂�

	 //�ŒZ����
	 float l;
	 //�ŒZ�ڐG�n�_
	 CVector3D c;
	 
	 //������r�p
	 float LengthSq;



	 //�ӂP(v0��v1)
	 Point = PointOnLineSegmentNearestPoint( v0, v1, center );
	 LengthSq = (center - Point).LengthSq();
	 l  = LengthSq;
	 c = Point;

	 //�ӂQ(v1��v2)
	 Point = PointOnLineSegmentNearestPoint( v1, v2, center );
	 LengthSq = (center - Point).LengthSq();
	 if(l>LengthSq) {
		 l = LengthSq;
		 c = Point;
	 }
	 
	 //�ӂR(v2��v0)
	 Point = PointOnLineSegmentNearestPoint( v2, v0, center );
	 LengthSq = (center - Point).LengthSq();
	 if(l>LengthSq) {
		 l = LengthSq;
		 c = Point;
	 }


	 l = sqrtf(l);
	 //�ŒZ�������m��
	 if(length) *length = l;
	 //�ŒZ�n�_���m��
	 if(cross) *cross = c;
	
    return (l<=radius);
}

bool CCollision::CollisionTriangleCapsule(const CVector3D &v0, const CVector3D &v1, const CVector3D &v2, const CVector3D &top, const CVector3D &bottom, float radius, CVector3D *cross, float *length){
	//�|���S���̖@�������߂�
	CVector3D N;

	N = CVector3D::Cross(v1 - v0, v2 - v0);
	if (N.LengthSq() <= 0) return false;
	N.Normalize();
	return CollisionTriangleCapsule(v0, v1, v2, N,top, bottom, radius, cross, length);

}
bool CCollision::CollisionTriangleCapsule(const CVector3D &v0, const CVector3D &v1, const CVector3D &v2, const CVector3D &N,const CVector3D &top, const CVector3D &bottom, float radius, CVector3D *cross, float *length){
	
	CVector3D V(top-bottom);

	CVector3D  VP;
	float Dist = 1e10;


	//�n�_����|���S����̂���n�_�i�ǂ��ł������j�ւ̃x�N�g��
	CVector3D PV1 = top-v0;
	//�I�_����|���S����̂���n�_�i�ǂ��ł������j�ւ̃x�N�g��
	CVector3D PV2 = bottom-v0;

	//�|���S���̖@���Ƃ̓��ς����߂�
	float d1 = CVector3D::Dot(PV1,N);
	float d2 = CVector3D::Dot(PV2,N);

	CVector3D c;
	if(d1*d2<0) {
		//�ђʂ��Ă���ꍇ�͐��ƃ|���S���̔�����s��
		if(IntersectTriangleRay(&c,top+CVector3D(0,radius,0),bottom+CVector3D(0,-radius,0),v0,v1,v2,&Dist)) {
			if(length) {
				//�ђʓ_�܂ł̋��������߂�
				float lt = (c - top).LengthSq();
				float lb = (c - bottom).LengthSq();
				if(lt<lb) *length = sqrtf(lt);
				else *length = sqrtf(lb);
				if (cross) *cross = c;
			}
			return true;
		}
	}

	d1=fabsf(d1);
	d2=fabsf(d2);
	//���ʏ�̓_�Ƃ̍ŒZ�n�_�����߂�
	CVector3D C1(top-N*d1);
	CVector3D C2(bottom-N*d2);
	//�_�����ʏ�ɂȂ��ꍇ�͖����A��̕ӂƂ̐ڐG�Œ��ׂ�
	if(!TriangleIntersect(C1,v0,v1,v2,N)) d1=1e10;
	if(!TriangleIntersect(C2,v0,v1,v2,N)) d2=1e10;


	//�ʂƂ̋������߂��_�̋�����I��
	Dist = (d1<d2) ? d1:d2; 
	if(Dist<=radius) {
		//�ǉ�
		if(length) *length = Dist;
		return true;
	}
	

	//�e�ӂƂ̋��������߂�
	Dist = min(min(DistanceLine(v0,v1,top,bottom),DistanceLine(v1,v2,top,bottom)),DistanceLine(v2,v0,top,bottom));

	if(length) *length = Dist;

    return (Dist<=radius);
	




}
bool CCollision::CompareLength(const COBB &A,const COBB &B,const CVector3D &distVec,const CVector3D &separate, CVector3D* axis ,float *length){
	float dist = fabsf(CVector3D::Dot(distVec,separate));
	float distA = 0;
	for(int i=0;i<3;i++) distA+=fabsf( CVector3D::Dot(A.m_axis[i],separate)) * A.m_length.v[i];
	float distB = 0;
	for(int i=0;i<3;i++) distB+=fabsf( CVector3D::Dot(B.m_axis[i],separate)) * B.m_length.v[i];
	if (length) {
		float l = dist - (distA + distB);
		if (l < 0 && l > *length) {
			*length = l;
			*axis = CVector3D::Dot(distVec, separate) > 0 ? separate:-separate;
		}
	}
	if(dist > distA + distB) return false;
	return true;
}
bool CCollision::CollisionOBB(const COBB &A,const COBB &B, CVector3D* axis, float* length){
	CVector3D distVec = B.m_center - A.m_center;
	if (length) *length = -FLT_MAX;
	for(int i=0;i<3;i++)
		if(!CompareLength(A,B,distVec,A.m_axis[i], axis,length)) return false;
	
	for(int i=0;i<3;i++)
		if(!CompareLength(A,B,distVec,B.m_axis[i], axis,length)) return false;
	
	CVector3D separate;
	for(int i=0;i<3;i++) {
		for(int j=0;j<3;j++) {
			separate = CVector3D::Cross(A.m_axis[i],B.m_axis[j]);
			if(!CompareLength(A,B,distVec,separate)) return false;
		}
	}

	return true;
}

bool CCollision::CollisionOBBShpere(const COBB &A, const CVector3D &center, float radius, CVector3D *axis,float *length) {
	CVector3D V = center - A.m_center;
	float d = 0;
	float l = 0;
	CVector3D VL;
	for (int i = 0; i<3; i++) {
		float t = CVector3D::Dot(A.m_axis[i], V);
		VL.v[i] = fabsf(t) - A.m_length.v[i];
		if (VL.v[i] < 0) {
			VL.v[i] = 0;
		}
		if (VL.v[i] > d) {
			if (axis) *axis = (t > 0) ? A.m_axis[i] : -A.m_axis[i];
			d = VL.v[i];
		}
	}
	l = VL.Length();
	if (length) *length = l;
	return (l <= radius);




}
bool CCollision::CollisionOBBCapsule(const COBB& obb, const CVector3D& lineS, const CVector3D& lineE, float radius, CVector3D* axis, float* length)
{
	CVector3D VS = lineS - obb.m_center;
	CVector3D VE = lineE - obb.m_center;
	CVector3D LD = lineE - lineS;
	CVector3D LC = (lineE + lineS) * 0.5f;
	CVector3D OCV = LC - obb.m_center;
	CVector3D u = LD.GetNormalize();
	float d0 = abs(CVector3D::Dot(u, obb.m_axis[0]));
	float d1 = abs(CVector3D::Dot(u, obb.m_axis[1]));
	float d2 = abs(CVector3D::Dot(u, obb.m_axis[2]));
	CVector3D f = d0 < d1 ? obb.m_axis[0]: obb.m_axis[1];
	CVector3D r = CVector3D::Cross(f, u);
	f = CVector3D::Cross(r,u);
	CMatrix mat(r.x,u.x,f.x,0,
				r.y, u.y, f.y, 0,
				r.z, u.z, f.z, 0,
				0,0,0,1);
	//�J�v�Z���̉���O���œ��e
	for (int k = 0; k < 3; k++) {
		float distA = 0, distB,l;
		CVector3D separate = CVector3D(mat.m[k][0], mat.m[k][1], mat.m[k][2]);
		for (int i = 0; i < 3; i++) distA += fabsf(CVector3D::Dot(obb.m_axis[i], separate)) * obb.m_length.v[i];
		distB = fabsf(CVector3D::Dot(LD, separate));
		l = fabsf(CVector3D::Dot(OCV, separate)) - (distA + distB);
		if (l > radius) return false;
	}


	//OBB�̉���O���œ��e
	float d = -FLT_MAX;
	float l = 0;
	CVector3D VL;
	for (int i = 0; i < 3; i++) {
		float ts = CVector3D::Dot(obb.m_axis[i], VS);
		float te = CVector3D::Dot(obb.m_axis[i], VE);
		float t = abs(ts) > abs(te) ? ts : te;
		float lt = CVector3D::Dot(obb.m_axis[i], LD);
		VL.v[i] = fabsf(t) - (obb.m_length.v[i] + abs(lt));
		if (VL.v[i] >= d) {
			if (axis) *axis = (t > 0) ? obb.m_axis[i] : -obb.m_axis[i];
			d = VL.v[i];
		}
		if (VL.v[i] < 0) {
			VL.v[i] = 0;
		}
	}
	l = VL.Length();
	if (length) *length = l;
	return (l <= radius);
}
bool CCollision::CollisionAABBTriangle(const CVector3D& aabb_max, const CVector3D& aabb_min, const CVector3D* v, CVector3D* normal, float* dist, float* push)
{
	float p[3], r;


	CVector3D center = (aabb_max + aabb_min) * 0.5f;
	CVector3D extents = aabb_max - center;

	CVector3D vv[3] = {
		v[0] - center,
		v[1] - center,
		v[2] - center
	};
	CVector3D fv[3] = {
		vv[1] - vv[0],
		vv[2] - vv[1],
		vv[0] - vv[2]
	};
	CVector3D av[3][3] = {
		CVector3D(0, -fv[0].z, fv[0].y),
		CVector3D(0, -fv[1].z, fv[1].y),
		CVector3D(0, -fv[2].z, fv[2].y),

		CVector3D(fv[0].z, 0, -fv[0].x),
		CVector3D(fv[1].z, 0, -fv[1].x),
		CVector3D(fv[2].z, 0, -fv[2].x),

		CVector3D(-fv[0].y, fv[0].x, 0),
		CVector3D(-fv[1].y, fv[1].x, 0),
		CVector3D(-fv[2].y, fv[2].x, 0),
	};
	float vr[3][3] = {
		extents.y * abs(fv[0].z) + extents.z * abs(fv[0].y),
		extents.y * abs(fv[1].z) + extents.z * abs(fv[1].y),
		extents.y * abs(fv[2].z) + extents.z * abs(fv[2].y),
		extents.x * abs(fv[0].z) + extents.z * abs(fv[0].x),
		extents.x * abs(fv[1].z) + extents.z * abs(fv[1].x),
		extents.x * abs(fv[2].z) + extents.z * abs(fv[2].x),
		extents.x * abs(fv[0].y) + extents.y * abs(fv[0].x),
		extents.x * abs(fv[1].y) + extents.y * abs(fv[1].x),
		extents.x * abs(fv[2].y) + extents.y * abs(fv[2].x)
	};
	// Test axis a00
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			p[0] = CVector3D::Dot(vv[0], av[i][j]);
			p[1] = CVector3D::Dot(vv[1], av[i][j]);
			p[2] = CVector3D::Dot(vv[2], av[i][j]);
			r = vr[i][j];

			if (max(-max(max(p[0], p[1]), p[2]), min(min(p[0], p[1]), p[2])) > r) {

				return false; // Axis is a separating axis

			}
		}
	}
	for (int i = 0; i < 3; i++)
		if (max(max(vv[0].v[i], vv[1].v[i]), vv[2].v[i]) < -extents.v[i] || min(min(vv[0].v[i], vv[1].v[i]), vv[2].v[i]) > extents.v[i])
			return false;
	CVector3D N = CVector3D::Cross(fv[0], fv[1]).GetNormalize();
	if (normal) *normal = N;
	return CollisionAABBTriangle(aabb_max, aabb_min, v[0], N, dist, push);
}
bool CCollision::CollisionAABBTriangle(const CVector3D& aabb_max, const CVector3D& aabb_min, const CVector3D& v, const CVector3D& N, float* dist, float* push)
{

	CVector3D center = (aabb_max + aabb_min) * 0.5f;
	CVector3D extents = aabb_max - center;

	float r = extents.x * abs(N.x) + extents.y * abs(N.y) + extents.z * abs(N.z);
	float s = CVector3D::Dot(N, center) - CVector3D::Dot(N, v);
	if (dist) *dist = abs(s) - r;
	if (push) *push = r - abs(s);

	return abs(s) <= r;
}
//�J�v�Z���Ƌ��̏Փ˔���
bool CCollision::CollisionCapsuleShpere(const CVector3D &c_top,const CVector3D &c_bottom,float c_radius,const CVector3D &s_center , float s_radius, float* dist,CVector3D* cross, CVector3D *dir){
	float l = DistancePointToLine(c_top,c_bottom,s_center,cross,dir);
	if (dist) *dist = l;
	if(l<=c_radius+s_radius) return true;
	return false;
}
bool CCollision::CollisionCapsuleShpere(const CVector2D& c_top, const CVector2D& c_bottom, float c_radius, const CVector2D& s_center, float s_radius, float* dist, CVector2D* cross, CVector2D* dir)
{
	float l = DistancePointToLine(c_top, c_bottom, s_center,cross,dir);
	if (dist) *dist = l;
	if (l <= c_radius + s_radius) return true;
	return false;
}
//�J�v�Z�����m�̏Փ˔���
bool CCollision::CollisionCapsule(const CVector3D &top1,const CVector3D &bottom1,float radius1,const CVector3D &top2,const CVector3D &bottom2,float radius2, float* dist, CVector3D* c1, CVector3D* dir1, CVector3D* c2, CVector3D* dir2){
	float l  = DistanceLine(top1,bottom1,top2,bottom2,c1,dir1,c2,dir2);
	if (dist) *dist = l;
	if(l<=radius1+radius2) return true;
	return false;
}
bool CCollision::CollisionCapsule(const CVector2D& top1, const CVector2D& bottom1, float radius1, const CVector2D& top2, const CVector2D& bottom2, float radius2, float* dist ,CVector2D* c1, CVector2D* dir1, CVector2D* c2, CVector2D* dir2)
{
	float l = DistanceLine(top1, bottom1, top2, bottom2, c1, dir1, c2, dir2);
	if (dist) *dist = l;
	if (l <= radius1 + radius2) return true;
	return false;
}
//AABB���m�̏Փ˔���
bool CCollision::CollisionAABB(const CVector3D &minA,const CVector3D &maxA,const CVector3D &minB,const CVector3D &maxB){
	if( minA.x  <= maxB.x && maxA.x >= minB.x &&
		minA.y  <= maxB.y && maxA.y >= minB.y &&
		minA.z  <= maxB.z && maxA.z >= minB.z) return true;
	return false;
}

bool CCollision::CollisionAABBPoint(const CVector3D &min, const CVector3D &max, const CVector3D &point) {
	if (min.x  <= point.x && max.x >= point.x &&
		min.y  <= point.y && max.y >= point.y &&
		min.z  <= point.z && max.z >= point.z) return true;
	return false;

}
bool CCollision::CollisionAABBRay(CVector3D *cross, float* length,const CVector3D& min, const CVector3D& max, const CVector3D& point, const CVector3D& dir)
{
	float t = -FLT_MAX;
	float t_max = FLT_MAX;

	for (int i = 0; i < 3; ++i) {
		if (fabsf(dir.v[i]) < FLT_EPSILON) {
			if (point.v[i] < min.v[i] || point.v[i] > max.v[i])
				return false; // �������Ă��Ȃ�
		}
		else {
			// �X���u�Ƃ̋������Z�o
			// t1���߃X���u�At2�����X���u�Ƃ̋���
			float odd = 1.0f / dir.v[i];
			float t1 = (min.v[i] - point.v[i]) * odd;
			float t2 = (max.v[i] - point.v[i]) * odd;
			if (t1 > t2) {
				float tmp = t1; t1 = t2; t2 = tmp;
			}

			if (t1 > t) t = t1;
			if (t2 < t_max) t_max = t2;

			// �X���u�����`�F�b�N
			if (t >= t_max)
				return false;
		}
	}

	// �������Ă���
	if (cross) {
		*cross = point + dir *t;
	}
	if (length) {
		*length =  t;
	}

	return true;
}
//2�����Ԃ̋���
float CCollision::DistanceLine(const CVector3D &s1,const CVector3D &e1,const CVector3D &s2,const CVector3D &e2,CVector3D *c1, CVector3D* dir1,CVector3D *c2, CVector3D* dir2){

	//���̃x�N�g�������߂�
	CVector3D V1(e1-s1);
	CVector3D V2(e2-s2);
	//2�̐����̊O�ς����߂�
	CVector3D N(CVector3D::Cross(V1,V2).GetNormalize());
	//2�̐��������s�łȂ��ꍇ
	if (N.LengthSq()) {
		float l = CVector3D::Dot(N, s1-s2);
		CVector3D m = N*l;
		//�����Q������P�Ɠ������ꕽ�ʏ�ɍ��킹��
		CVector3D S2 = s2+m;
		CVector3D E2 = e2+m;
		//�������Ă��邩���ׂ�
		float d1=CVector3D::Dot(CVector3D::Cross(V1,s1-S2),CVector3D::Cross(V1,s1-E2));
		float d2=CVector3D::Dot(CVector3D::Cross(V2,S2-s1),CVector3D::Cross(V2,S2-e1));
		if( d1 < 0 &&
			 d2 < 0 ) {

			if (dir1 || dir2) {
				CVector3D d = m.GetNormalize();
				if (dir1) *dir1 = -d;
				if (dir2) *dir2 = d;
			}
			if (c1 || c2) {
				CVector3D VD1 = V1.GetNormalize();
				CVector3D VD2 = V2.GetNormalize();
				float Dv = CVector3D::Dot(VD1, VD2);
				float D1 = CVector3D::Dot(s2 - s1, VD1);
				float D2 = CVector3D::Dot(s2 - s1, VD2);

				float t1 = (D1 - D2 * Dv) / (1.0f - Dv * Dv);
				float t2 = (D2 - D1 * Dv) / (Dv * Dv - 1.0f);

				CVector3D
					Q1 = s1 + VD1 * t1,
					Q2 = s2 + VD2 * t2;

				if (c1)
					*c1 = Q1;

				if (c2)
					*c2 = Q2;
			}


				return fabsf(l);
		}
	} else {
		//��̐��������s�ȏꍇ
	//	CVector3D l_dir = (e1 - s1).GetNormalize();
	//	CVector3D v = CVector3D::Cross(s2 - s1, l_dir);
	//	CVector3D d = CVector3D::Cross(l_dir,v.GetNormalize());
	//	if (dir1) *dir1 = d;
	//	if (dir2) *dir2 = -d;
	//	if (c1) *c1 = s1;
	//	if (c2) *c2 = s2;
	//	return v.Length();
	}
	//�n�_�ƏI�_��������Ƃ̋��������߁A�����Ƃ��Z��������Ԃ�
	CVector3D cross[4];
	CVector3D dir[4];
	float length[4];
	length[0] = DistancePointToLineSq(s2, e2, s1,&cross[0],&dir[0]);
	length[1] = DistancePointToLineSq(s2, e2, e1, &cross[1], &dir[1]);
	length[2] = DistancePointToLineSq(s1, e1, s2, &cross[2], &dir[2]);
	length[3] = DistancePointToLineSq(s1, e1, e2, &cross[3], &dir[3]);
	
	int idx = 0;
	for(int i=1;i<4;i++){
		if (length[idx] > length[i]) {
			idx = i;
		}
	}
	float* pl = &length[idx];
	CVector3D* pc = &cross[idx];
	if (c1 || c2 || dir1 || dir2){
		if (c1) {
			if (idx == 0)
				*c1 = s1;
			else if (idx == 1)
				*c1 = e1;
			else
				*c1 = cross[idx];
		}
		if (dir1) {
			if (idx <= 1)
				*dir1 = -dir[idx];
			else
				*dir1 = dir[idx];
		}
		if (c2) {
			if (idx == 2)
				*c2 = s2;
			else if (idx == 3)
				*c2 = e2;
			else
				*c2 = cross[idx];
		}
		if (dir2) {
			if (idx <= 1)
				*dir2 = dir[idx];
			else
				*dir2 = -dir[idx];
		}
	}
	return sqrtf(*pl);





}
float CCollision::DistanceLine(const CVector2D& s1, const CVector2D& e1, const CVector2D& s2, const CVector2D& e2, CVector2D* c1, CVector2D* dir1, CVector2D* c2, CVector2D* dir2)
{
	//���̃x�N�g�������߂�
	CVector2D V1(e1 - s1);
	CVector2D V2(e2 - s2);
	//2�̐����̊O�ς����߂�
	float f = CVector2D::Cross(V1, V2);
	//2�̐��������s�łȂ��ꍇ
	if (f!=0) {
		//�������Ă��邩���ׂ�
		float d1 = CVector2D::Cross(V1, s1 - s2)*CVector2D::Cross(V1, s1 - e2);
		float d2 = CVector2D::Cross(V2, s2 - s1)*CVector2D::Cross(V2, e2 - e1);
		if (d1 < 0 &&
			d2 < 0) {
			return 0;
		}
	}
	//�n�_�ƏI�_��������Ƃ̋��������߁A�����Ƃ��Z��������Ԃ�
	return sqrtf(min(
		min(DistancePointToLineSq(s2, e2, s1),
			DistancePointToLineSq(s2, e2, e1)),
		min(DistancePointToLineSq(s1, e1, s2),
			DistancePointToLineSq(s1, e1, e2))
	));
}
//2�����Ԃ̌���
bool CCollision::CollitionLine(const CVector2D &s1, const CVector2D &e1, const CVector2D &s2, const CVector2D &e2) {
	float ta = (s2.x - e2.x) * (s1.y - s2.y) + (s2.y - e2.y) * (s2.x - s1.x);
	float tb = (s2.x - e2.x) * (e1.y - s2.y) + (s2.y - e2.y) * (s2.x - e1.x);
	float tc = (s1.x - e1.x) * (s2.y - s1.y) + (s1.y - e1.y) * (s1.x - s2.x);
	float td = (s1.x - e1.x) * (e2.y - s1.y) + (s1.y - e1.y) * (s1.x - e2.x);

	return tc * td < 0 && ta * tb < 0;
}
	//�_�Ɛ��̋���
float CCollision::DistancePointToLine(const CVector3D& v0, const CVector3D& v1, const CVector3D& point, CVector3D* cross, CVector3D* dir)
{
	CVector3D p = PointOnLineSegmentNearestPoint(v0,v1,point);
	CVector3D v = point - p;
	float l = v.Length();
	if (cross)*cross = p;
	if (dir)
		*dir = v / l;
	return l;
}
float CCollision::DistancePointToLine(const CVector2D &v0, const CVector2D &v1, const CVector2D &point, CVector2D* cross, CVector2D* dir)
{
	CVector2D p = PointOnLineSegmentNearestPoint(v0, v1, point);
	CVector2D v = point - p;
	float l = v.Length();
	if (cross)*cross = p;
	if (dir)
		*dir = v / l;
	return l;
}
//�_�Ɛ��̋���
float CCollision::DistancePointToLineSq(const CVector3D &v0,const CVector3D &v1,const CVector3D &point, CVector3D* cross, CVector3D* dir)
{
	CVector3D p = PointOnLineSegmentNearestPoint(v0,v1,point);
	CVector3D v = point - p;
	float l = v.LengthSq();
	if (cross)*cross = p;
	if (dir)
		*dir = v;
	return l;
}

float CCollision::DistancePointToLineSq(const CVector2D& v0, const CVector2D& v1, const CVector2D& point, CVector2D* cross, CVector2D* dir)
{
	CVector2D p = PointOnLineSegmentNearestPoint(v0, v1, point);
	CVector2D v = point - p;
	float l = v.LengthSq();
	if (cross)*cross = p;
	if (dir)
		*dir = v;
	return l;
}
