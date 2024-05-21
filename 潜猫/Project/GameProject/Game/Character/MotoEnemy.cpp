#include "Enemy.h"
#include "Player.h"
#include "../Stage/Item.h"
#include "../Stage/Stage.h"
#include "../GameData/GameData.h"
#include "Debug/DebugPrint.h"
#include "Effect/Effect.h"

//歩き速度
#define WALK_SPEED 2.4f
//追跡速度
#define CHASE_SPEED 0.05f
//待機時間
#define IDLE_TIME 2.0f
//目的地との間距離
#define SPACE 1.0f
//重力
#define G 0.8f
//最大重力
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
	//進行ルート設定
	mp_cource = data;
	//出現位置は進行ルートの開始地点
	m_pos = mp_cource->points[0];
	//目標地点の添え字(0は出現位置)
	m_cource_idx = 1;
	//m_rad = 0.8f / 2.0f;
	m_rad = 0.8f;
	m_isFind = false;
	m_isFindplayer = false;
}

//待機状態の処理
void Enemy::StateIdle()
{
	//待機アニメーション
	m_model.ChangeAnimation((int)AnimId::Idle);

	// 敵の視野の色変換用(初期色は色)
	color = CVector4D(0.0f, 1.0f, 0.0f, 0.75f);

	m_vec.x = 0.0f;
	m_vec.z = 0.0f;

	//待機時間待ち
	if (m_elapsedTime < IDLE_TIME) {
		m_elapsedTime += CFPS::GetDeltaTime();
	}
	//待機時間が終われば、
	else {
		m_elapsedTime = 0.0f;
		//移動状態へ移行
		m_state = State::Move;
	}

	//プレイヤーを見つけたら、強制的に追跡状態へ
	if (IsFoundPlayer()) {
		m_state = State::Chase;

	}
}

//移動状態の処理
void Enemy::StateMove()
{
	//前進アニメーション
	m_model.ChangeAnimation((int)AnimId::Walk);
	if (m_isFind && !IsFoundPlayer()) {
		isFoll = true;
		m_isFind = false;
		m_isTouch = true;
	}
	else {
		FollBox();
		//決められた進行ルートを移動する
		const float move_speed = WALK_SPEED;	//const float move_speed = 0.03f;
		if (m_cource_idx < mp_cource->size) {
			//目標地点へのベクトル
			CVector3D vec = mp_cource->points[m_cource_idx] - m_pos;
			//ベクトルから回転値を逆算
			m_rot_target.y = atan2(vec.x, vec.z);
			//回転値から方向ベクトルを計算
			CVector3D dir(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
			// 敵の視野の色変換用(初期色は色)
			color = CVector4D(0.0f, 1.0f, 0.0f, 0.75f);
			//目標地点への距離が1.0fより遠ければ
			if (vec.Length() > SPACE) {
				//前進
				m_vec = dir * move_speed;
			}
			else {
				//次の地点へ
				m_cource_idx++;
			}
		}
		else {
			m_cource_idx %= m_cource_idx;
		}
	}
	//プレイヤーを見つけたら、強制的に追跡状態へ
	if (IsFoundPlayer()) {
		m_state = State::Chase;
	}
}

void Enemy::StateChase()
{
	//前進アニメーション
	m_model.ChangeAnimation((int)AnimId::Walk);
	//決められた進行ルートを移動する
	const float move_speed = WALK_SPEED;
	// 敵をプレイヤーの方向へ移動
	const float speedB = CHASE_SPEED;

	//回転値から方向ベクトルを計算
	CVector3D dir(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
	CVector3D vec = mp_player->m_pos - m_pos;
	m_rot.y = atan2(vec.x, vec.z);

	//敵がプレイヤーを見つけたかどうか判定
	if (IsFoundPlayer())
	{
		color = CVector4D(1.0f, 0.0f, 0.0f, color.a);
		if (vec.Length() < SPACE) {
			m_vec.x = 0.0f;
			m_vec.z = 0.0f;
			//待機アニメーション
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
		//待機状態へ移行
		m_state = State::Idle;
	}
}

int Enemy::CollisionLine(const CVector3D& lineS, const CVector3D& lineE, const CVector3D& dir, CVector3D* cross) {
	CVector3D c1, c2, d1, d2;
	float dist;
	if (CCollision::CollisionCapsule(m_lineS, m_lineE, m_rad,
		lineS, lineE, 0.01f,
		&dist, &c1, &d1, &c2, &c2)) {
		//カプセルと弾の線分との接触点
		if (cross) {
			*cross = c2 + -dir * (m_rad - dist);
		}
		return 1;	//当たっていたら1(1)
	}
	return 0;		//当たっていない（0）
}

bool Enemy::IsFoundPlayer() {
	//回転値から方向ベクトルを計算
	CVector3D dir(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
	// 角度(内積)と距離で視野範囲内か判定
	CVector3D v1 = (mp_player->m_pos - m_pos);
	float dot = CVector3D::Dot(dir, v1.GetNormalize());//vを正規化して内積

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
	//最も近いオブジェクトへの距離
	float dist = FLT_MAX;
	//射線との衝突点
	CVector3D hit_field_point;
	//衝突したステージオブジェクト
	Stage* hit_field = nullptr;
	if (Stage* s = dynamic_cast<Stage*>(TaskManeger::FindObject(TaskType::eField))) {
		//接触面の法線は(使わない)
		CVector3D n;
		if (s->GetModel()->CollisionRay(&hit_field_point, &n, lineS, lineE)) {
			//発射位置から接触面への距離
			dist = (hit_field_point - lineS).LengthSq();
			//接触したステージを更新
			hit_field = s;
		}
	}
	//壁に当たる
	if (hit_field) {
		m_isWall = true;
	}
	else {
		m_isWall = false;
	}
}

void Enemy::Update()
{
	// 現在の状態に合わせて、処理を切り替える
	switch (m_state)
	{
		// 待機状態
	case State::Idle:
		StateIdle();
		break;
		// 移動状態
	case State::Move:
		StateMove();
		break;
		// 追跡状態
	case State::Chase:
		StateChase();
		break;
	}

	//アニメーション更新
	m_model.UpdateAnimation();

	float a = Utility::NormalizeAngle(m_rot_target.y - m_rot.y);
	//割合による補間
	m_rot.y += a * 0.1f;
	//m_rot.y = Utility::NormalizeAngle(m_rot.y);

	m_vec.x *= G;
	if (abs(m_vec.x) < MAX_G)m_vec.x = 0;
	m_vec.z *= G;
	if (abs(m_vec.z) < MAX_G)m_vec.z = 0;

	m_vec.y -= GRAVITY;
	m_pos.y += m_vec.y;
	m_pos += m_vec * CFPS::GetDeltaTime();

	//カプセル更新
	float height = 2.0f;
	m_lineS = m_pos + CVector3D(0, height - m_rad, 0);
	m_lineE = m_pos + CVector3D(0, m_rad, 0);

	//デバッグ表示
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
	//回転値から方向ベクトルを計算
	CVector3D dir(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
	// 視野の範囲を表示
	auto lineWidth = 5.0f;
	Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + dir * view_length, CVector4D(1, 0, 1, 1), lineWidth);
	Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + CMatrix::MRotationY(view_ang) * dir * view_length, CVector4D(0, 1, 1, 1), lineWidth);
	Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + CMatrix::MRotationY(-view_ang) * dir * view_length, CVector4D(0, 1, 1, 1), lineWidth);
	//扇の見た目
	CMatrix m;
	m.LookAt(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + dir * view_length, CVector3D(0, 1, 0));
	Utility::DrawSector(m, -view_ang, view_ang, view_length, color);

	//if (mp_player->m_isFind && !IsFoundPlayer()) {
	if (m_isTouch) {//m_isFind && !IsFoundPlayer()
		//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(1, 0, 1, 1));
		Utility::DrawCube(m_pos + CVector3D(0, 2, 0), CVector3D(1, 0, 1), CVector4D(1, 0, 1, 1));
	}
	else {
		//敵カプセルの表示
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
		//カプセルとモデルの衝突
		auto tri = b->GetModel()->CollisionCupsel(m_lineS, m_lineE, m_rad);
		//接触した面の数繰り返す
		for (auto& t : tri) {
			if (t.m_normal.y < -0.5f) {
				//面が下向き→天井に当たった	
				//上昇速度を0に戻す
				if (m_vec.y > 0)
					m_vec.y = 0;
			}
			else if (t.m_normal.y > 0.5f) {
				//面が上向き→地面に当たった
				//重力落下速度を0に戻す
				if (m_vec.y < 0)
					m_vec.y = 0;
			}
			float max_y = max(t.m_vertex[0].y, max(t.m_vertex[1].y, t.m_vertex[2].y));
			//接触した面の方向へ、めり込んだ分押し戻す
			CVector3D nv = t.m_normal * (m_rad - t.m_dist);
			//最も大きな移動量を求める
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
		//敵同士の判定
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
		//回転値から方向ベクトルを計算
		CVector3D dir(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
		// 視野の範囲を表示
		auto lineWidth = 5.0f;
		Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + dir * view_length, CVector4D(1, 0, 1, 1), lineWidth);
		Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + CMatrix::MRotationY(view_ang) * dir * view_length, CVector4D(0, 1, 1, 1), lineWidth);
		Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + CMatrix::MRotationY(-view_ang) * dir * view_length, CVector4D(0, 1, 1, 1), lineWidth);
		//扇の見た目
		CMatrix m;
		m.LookAt(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + dir * view_length, CVector3D(0, 1, 0));
		Utility::DrawSector(m, -view_ang, view_ang, view_length, color);

		//if (mp_player->m_isFind && !IsFoundPlayer()) {
		if (m_isTouch) {//m_isFind && !IsFoundPlayer()
			//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(1, 0, 1, 1));
			Utility::DrawCube(m_pos + CVector3D(0, 2, 0), CVector3D(1, 0, 1), CVector4D(1, 0, 1, 1));
		}
		else {
			//敵カプセルの表示
			//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(0, 0, 1, 1));
			Utility::DrawCube(m_pos + CVector3D(0, 2, 0), CVector3D(1, 0, 1), CVector4D(0, 0, 1, 1));
		}
	}
}

void Enemy::Debug() {
	DebugPrint::Print("Enemy座標 : X:%f , Y:%f , Z:%f", m_pos.x, m_pos.y, m_pos.z);
}
