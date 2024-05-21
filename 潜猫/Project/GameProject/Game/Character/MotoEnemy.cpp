#include "Enemy.h"
#include "Player.h"
#include "../Stage/Item.h"
#include "../Stage/Stage.h"
#include "../GameData/GameData.h"
#include "Debug/DebugPrint.h"
#include "Effect/Effect.h"

//�������x
#define WALK_SPEED 2.4f
//�ǐՑ��x
#define CHASE_SPEED 0.05f
//�ҋ@����
#define IDLE_TIME 2.0f
//�ړI�n�Ƃ̊ԋ���
#define SPACE 1.0f
//�d��
#define G 0.8f
//�ő�d��
#define MAX_G 0.001f


Enemy::Enemy(const CourseData* data) :Task(TaskType::eEnemy)
, mp_player(nullptr)
, mp_field(nullptr)
, m_elapsedTime(0.0f)
, m_state(State::Move)
, isFoll(false)
, m_cntfoll(0)
, m_isTouch(false)
, m_isWall(false)
{
	mp_player = dynamic_cast<Player*>(TaskManeger::FindObject(TaskType::ePlayer));
	//mp_hidebox = Base::FindObject(BaseType::eHideBox);
	m_model = COPY_RESOURCE("Golem", CModelA3M);
	//�i�s���[�g�ݒ�
	mp_cource = data;
	//�o���ʒu�͐i�s���[�g�̊J�n�n�_
	m_pos = mp_cource->points[0];
	//�ڕW�n�_�̓Y����(0�͏o���ʒu)
	m_cource_idx = 1;
	//m_rad = 0.8f / 2.0f;
	m_rad = 0.8f;
	m_isFind = false;
	m_isFindplayer = false;
}

//�ҋ@��Ԃ̏���
void Enemy::StateIdle()
{
	//�ҋ@�A�j���[�V����
	m_model.ChangeAnimation((int)AnimId::Idle);

	// �G�̎���̐F�ϊ��p(�����F�͐F)
	color = CVector4D(0.0f, 1.0f, 0.0f, 0.75f);

	m_vec.x = 0.0f;
	m_vec.z = 0.0f;

	//�ҋ@���ԑ҂�
	if (m_elapsedTime < IDLE_TIME) {
		m_elapsedTime += CFPS::GetDeltaTime();
	}
	//�ҋ@���Ԃ��I���΁A
	else {
		m_elapsedTime = 0.0f;
		//�ړ���Ԃֈڍs
		m_state = State::Move;
	}

	//�v���C���[����������A�����I�ɒǐՏ�Ԃ�
	if (IsFoundPlayer()) {
		m_state = State::Chase;

	}
}

//�ړ���Ԃ̏���
void Enemy::StateMove()
{
	//�O�i�A�j���[�V����
	m_model.ChangeAnimation((int)AnimId::Walk);
	if (m_isFind && !IsFoundPlayer()) {
		isFoll = true;
		m_isFind = false;
		m_isTouch = true;
	}
	else {
		FollBox();
		//���߂�ꂽ�i�s���[�g���ړ�����
		const float move_speed = WALK_SPEED;	//const float move_speed = 0.03f;
		if (m_cource_idx < mp_cource->size) {
			//�ڕW�n�_�ւ̃x�N�g��
			CVector3D vec = mp_cource->points[m_cource_idx] - m_pos;
			//�x�N�g�������]�l���t�Z
			m_rot_target.y = atan2(vec.x, vec.z);
			//��]�l��������x�N�g�����v�Z
			CVector3D dir(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
			// �G�̎���̐F�ϊ��p(�����F�͐F)
			color = CVector4D(0.0f, 1.0f, 0.0f, 0.75f);
			//�ڕW�n�_�ւ̋�����1.0f��艓�����
			if (vec.Length() > SPACE) {
				//�O�i
				m_vec = dir * move_speed;
			}
			else {
				//���̒n�_��
				m_cource_idx++;
			}
		}
		else {
			m_cource_idx %= m_cource_idx;
		}
	}
	//�v���C���[����������A�����I�ɒǐՏ�Ԃ�
	if (IsFoundPlayer()) {
		m_state = State::Chase;
	}
}

void Enemy::StateChase()
{
	//�O�i�A�j���[�V����
	m_model.ChangeAnimation((int)AnimId::Walk);
	//���߂�ꂽ�i�s���[�g���ړ�����
	const float move_speed = WALK_SPEED;
	// �G���v���C���[�̕����ֈړ�
	const float speedB = CHASE_SPEED;

	//��]�l��������x�N�g�����v�Z
	CVector3D dir(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
	CVector3D vec = mp_player->m_pos - m_pos;
	m_rot.y = atan2(vec.x, vec.z);

	//�G���v���C���[�����������ǂ�������
	if (IsFoundPlayer())
	{
		color = CVector4D(1.0f, 0.0f, 0.0f, color.a);
		if (vec.Length() < SPACE) {
			m_vec.x = 0.0f;
			m_vec.z = 0.0f;
			//�ҋ@�A�j���[�V����
			m_model.ChangeAnimation((int)AnimId::Idle);
		}
		else {
			m_pos += dir * speedB;
			m_model.ChangeAnimation((int)AnimId::Walk);
		}
		m_isFind = false;
	}
	else {
		new Effect("Fukidasi", m_pos + CVector3D(0, 2.5, -1), 1.0f, true, 120);
		//�ҋ@��Ԃֈڍs
		m_state = State::Idle;
	}
}

int Enemy::CollisionLine(const CVector3D& lineS, const CVector3D& lineE, const CVector3D& dir, CVector3D* cross) {
	CVector3D c1, c2, d1, d2;
	float dist;
	if (CCollision::CollisionCapsule(m_lineS, m_lineE, m_rad,
		lineS, lineE, 0.01f,
		&dist, &c1, &d1, &c2, &c2)) {
		//�J�v�Z���ƒe�̐����Ƃ̐ڐG�_
		if (cross) {
			*cross = c2 + -dir * (m_rad - dist);
		}
		return 1;	//�������Ă�����1(1)
	}
	return 0;		//�������Ă��Ȃ��i0�j
}

bool Enemy::IsFoundPlayer() {
	//��]�l��������x�N�g�����v�Z
	CVector3D dir(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
	// �p�x(����)�Ƌ����Ŏ���͈͓�������
	CVector3D v1 = (mp_player->m_pos - m_pos);
	float dot = CVector3D::Dot(dir, v1.GetNormalize());//v�𐳋K�����ē���

	if (v1.Length() <= view_length) {
		if (dot > cos(view_ang)) {
			Shot();
			if (!mp_player->m_isHide && !m_isWall || mp_player->m_isHide && m_isFindplayer == true && !m_isWall) {
				m_isFindplayer = true;
				return true;
			}
		}
	}
	m_isFindplayer = false;
	return false;
}

void Enemy::Shot() {
	CVector3D lineS = m_pos + CVector3D(0, 0.75f, 0);
	CVector3D lineE = mp_player->m_pos + CVector3D(0, 0.75f, 0);
	//�ł��߂��I�u�W�F�N�g�ւ̋���
	float dist = FLT_MAX;
	//�ː��Ƃ̏Փ˓_
	CVector3D hit_field_point;
	//�Փ˂����X�e�[�W�I�u�W�F�N�g
	Stage* hit_field = nullptr;
	if (Stage* s = dynamic_cast<Stage*>(TaskManeger::FindObject(TaskType::eField))) {
		//�ڐG�ʂ̖@����(�g��Ȃ�)
		CVector3D n;
		if (s->GetModel()->CollisionRay(&hit_field_point, &n, lineS, lineE)) {
			//���ˈʒu����ڐG�ʂւ̋���
			dist = (hit_field_point - lineS).LengthSq();
			//�ڐG�����X�e�[�W���X�V
			hit_field = s;
		}
	}
	//�ǂɓ�����
	if (hit_field) {
		m_isWall = true;
	}
	else {
		m_isWall = false;
	}
}

void Enemy::Update()
{
	// ���݂̏�Ԃɍ��킹�āA������؂�ւ���
	switch (m_state)
	{
		// �ҋ@���
	case State::Idle:
		StateIdle();
		break;
		// �ړ����
	case State::Move:
		StateMove();
		break;
		// �ǐՏ��
	case State::Chase:
		StateChase();
		break;
	}

	//�A�j���[�V�����X�V
	m_model.UpdateAnimation();

	float a = Utility::NormalizeAngle(m_rot_target.y - m_rot.y);
	//�����ɂ����
	m_rot.y += a * 0.1f;
	//m_rot.y = Utility::NormalizeAngle(m_rot.y);

	m_vec.x *= G;
	if (abs(m_vec.x) < MAX_G)m_vec.x = 0;
	m_vec.z *= G;
	if (abs(m_vec.z) < MAX_G)m_vec.z = 0;

	m_vec.y -= GRAVITY;
	m_pos.y += m_vec.y;
	m_pos += m_vec * CFPS::GetDeltaTime();

	//�J�v�Z���X�V
	float height = 2.0f;
	m_lineS = m_pos + CVector3D(0, height - m_rad, 0);
	m_lineE = m_pos + CVector3D(0, m_rad, 0);

	//�f�o�b�O�\��
	//Debug();
}

void Enemy::Render()
{
	//if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	//m_model.SetScale(0.005f, 0.005f, 0.005f);
	m_model.SetScale(0.01f, 0.01f, 0.01f);
	m_model.Render();
	//��]�l��������x�N�g�����v�Z
	CVector3D dir(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
	// ����͈̔͂�\��
	auto lineWidth = 5.0f;
	Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + dir * view_length, CVector4D(1, 0, 1, 1), lineWidth);
	Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + CMatrix::MRotationY(view_ang) * dir * view_length, CVector4D(0, 1, 1, 1), lineWidth);
	Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + CMatrix::MRotationY(-view_ang) * dir * view_length, CVector4D(0, 1, 1, 1), lineWidth);
	//��̌�����
	CMatrix m;
	m.LookAt(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + dir * view_length, CVector3D(0, 1, 0));
	Utility::DrawSector(m, -view_ang, view_ang, view_length, color);

	//if (mp_player->m_isFind && !IsFoundPlayer()) {
	if (m_isTouch) {//m_isFind && !IsFoundPlayer()
		//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(1, 0, 1, 1));
		Utility::DrawCube(m_pos + CVector3D(0, 2, 0), CVector3D(1, 0, 1), CVector4D(1, 0, 1, 1));
	}
	else {
		//�G�J�v�Z���̕\��
		//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(0, 0, 1, 1));
		Utility::DrawCube(m_pos + CVector3D(0, 2, 0), CVector3D(1, 0, 1), CVector4D(0, 0, 1, 1));
	}

}

void Enemy::Collision(Task* b)
{
	switch (b->GetType()) {
	case TaskType::eField:
	{
		CVector3D v(0, 0, 0);
		//�J�v�Z���ƃ��f���̏Փ�
		auto tri = b->GetModel()->CollisionCupsel(m_lineS, m_lineE, m_rad);
		//�ڐG�����ʂ̐��J��Ԃ�
		for (auto& t : tri) {
			if (t.m_normal.y < -0.5f) {
				//�ʂ����������V��ɓ�������	
				//�㏸���x��0�ɖ߂�
				if (m_vec.y > 0)
					m_vec.y = 0;
			}
			else if (t.m_normal.y > 0.5f) {
				//�ʂ���������n�ʂɓ�������
				//�d�͗������x��0�ɖ߂�
				if (m_vec.y < 0)
					m_vec.y = 0;
			}
			float max_y = max(t.m_vertex[0].y, max(t.m_vertex[1].y, t.m_vertex[2].y));
			//�ڐG�����ʂ̕����ցA�߂荞�񂾕������߂�
			CVector3D nv = t.m_normal * (m_rad - t.m_dist);
			//�ł��傫�Ȉړ��ʂ����߂�
			v.y = fabs(v.y) > fabs(nv.y) ? v.y : nv.y;
			v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
			v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;
		}
		m_pos += v;
		m_lineS += v;
		m_lineE += v;
	}
	break;
	case TaskType::eEnemy:
	{
		//�G���m�̔���
		CVector3D c1, dir1, c2, dir2;
		float dist;
		if (CCollision::CollisionCapsule(m_lineS, m_lineE, m_rad,
			b->m_lineS, b->m_lineE, b->m_rad,
			&dist, &c1, &dir1, &c2, &dir2)) {
			float s = (m_rad + b->m_rad) - dist;
			b->m_pos += dir1 * s * 0.5f;
			b->m_lineS += dir1 * s * 0.5f;
			b->m_lineE += dir1 * s * 0.5f;
			m_pos += dir2 * s * 0.5f;
			m_lineS += dir2 * s * 0.5f;
			m_lineE += dir2 * s * 0.5f;

		}
	}
	break;
	}
}

void Enemy::FollBox() {
	if (isFoll && m_cntfoll < 1) {
		/*
		Base::Add(new Item(
			m_pos + CVector3D(0, 0.5f, 0),
			CVector3D(0.0f, 0.0f, 0.0f),
			CVector3D(0.1f, 0.1f, 0.1f)
		));
		*/
		new Item(
			m_pos + CVector3D(0, 0.5f, 0),
			CVector3D(0.0f, 0.0f, 0.0f),
			CVector3D(0.1f, 0.1f, 0.1f)
		);
		m_cntfoll++;
	}
	isFoll = false;
}

void Enemy::LateRender() {
	if (m_isTouch) {
		m_model.SetPos(m_pos);
		m_model.SetRot(m_rot);
		//m_model.SetScale(0.005f, 0.005f, 0.005f);
		m_model.SetScale(0.01f, 0.01f, 0.01f);
		m_model.Render();
		//��]�l��������x�N�g�����v�Z
		CVector3D dir(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
		// ����͈̔͂�\��
		auto lineWidth = 5.0f;
		Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + dir * view_length, CVector4D(1, 0, 1, 1), lineWidth);
		Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + CMatrix::MRotationY(view_ang) * dir * view_length, CVector4D(0, 1, 1, 1), lineWidth);
		Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + CMatrix::MRotationY(-view_ang) * dir * view_length, CVector4D(0, 1, 1, 1), lineWidth);
		//��̌�����
		CMatrix m;
		m.LookAt(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + dir * view_length, CVector3D(0, 1, 0));
		Utility::DrawSector(m, -view_ang, view_ang, view_length, color);

		//if (mp_player->m_isFind && !IsFoundPlayer()) {
		if (m_isTouch) {//m_isFind && !IsFoundPlayer()
			//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(1, 0, 1, 1));
			Utility::DrawCube(m_pos + CVector3D(0, 2, 0), CVector3D(1, 0, 1), CVector4D(1, 0, 1, 1));
		}
		else {
			//�G�J�v�Z���̕\��
			//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(0, 0, 1, 1));
			Utility::DrawCube(m_pos + CVector3D(0, 2, 0), CVector3D(1, 0, 1), CVector4D(0, 0, 1, 1));
		}
	}
}

void Enemy::Debug() {
	DebugPrint::Print("Enemy���W : X:%f , Y:%f , Z:%f", m_pos.x, m_pos.y, m_pos.z);
}
