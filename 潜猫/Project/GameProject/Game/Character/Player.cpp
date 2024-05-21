#include "Player.h"
#include "PlayerCamera.h"
#include "Enemy.h"
#include "../Stage/TyukanBox.h"
#include "../Stage/Stage.h"
#include "../Gamedata/GameData.h"
#include "../Game.h"
#include "../UI/UI.h"
#include "../../Debug/DebugPrint.h"
#include "../../Effect/Effect.h"
#include "../../Navigation/NavNode.h"
#include "../../Navigation/NavManeger.h"
#include "../Stage/FellBox.h"

//歩き速度
#define WALK_SPEED 5.0f//10.0f
//忍び速度
#define SILENT_WALK_SPEED 3.0f//10.0f
//回転速度
#define ROTATE_SPEED 15.0f
//ジャンプ速度
//#define JUMP_SPEED 10.0f
#define JUMP_SPEED 0.2f
//大ジャンプ速度
//#define	HIGH_JUMP_SPEED 16.0f
#define	HIGH_JUMP_SPEED 0.4f
//待機時間
#define IDLE_TIME 2.0f

#define UP_SPEED 10.0f

#define DBUG_SPEED 0.1f
//#define DBUG_SPEED 0.7f

Player::Player(const CVector3D& pos) :CharaBase(TaskType::ePlayer)
	,mp_camera(nullptr)
	,m_moveDir(0.0f,0.0f,0.0f)
	,m_elapsedTime(0.0f)
	,m_state(State::Move)
	,m_viewAngle(35.0f)
	,m_viewLength(3.0f)
	,m_isSearch(false)
	,m_isBark(false)
	,m_enemyflg(false)
{
	m_model = COPY_RESOURCE("Ninja", CModelA3M);
	m_pos = m_tyukan = pos;
	//m_rad = 0.4f;
	//m_rad = 0.2f;
	m_rad = 0.4f;
	//m_rad = 0.4f;
	//m_height = 0.5f;
	//m_height = 0.2f;
	m_height = 1.2f;
	m_time = 0;
	m_isGround = true;
	m_isHide = false;
	//m_isFind = false;
	mp_enemy = nullptr;
	mp_tyukan = nullptr;
	mp_fellbox = nullptr;

	//プレイヤー位置に経路探索用のノードを作成
	m_navNode = new NavNode
	(
		m_pos + CVector3D(0.0f, 1.0f, 0.0f),
		NavNode::NodeType::Destination
	);
	m_navNode->SetNodeColor(CVector3D(1.0f, 0.25f, 0.25f));

}

Player::~Player() {

}

//移動状態の処理
void Player::StateMove() {
	//移動速度をリセット
	m_vec.x = 0.0f;
	m_vec.z = 0.0f;

	//入力方向のベクトルを取得
	CVector3D inputDir = CVector3D::zero;
	if (HOLD(CInput::eLeft))	inputDir.x = 1.0f;
	if (HOLD(CInput::eRight))	inputDir.x = -1.0f;
	if (HOLD(CInput::eUp))		inputDir.z = 1.0f;
	if (HOLD(CInput::eDown))	inputDir.z = -1.0f;

	//キーが入力されていれば、
	if (inputDir.LengthSq() > 0.1f) {
		if (mp_camera->GetModel() == 0) {
			//カメラの向きに合わせた、移動ベクトルを求める
			CMatrix camMtx = CMatrix::MRotation(mp_camera->m_rot);
			m_moveDir = camMtx * inputDir;
			m_moveDir.y = 0.0f;
			m_moveDir.Normalize();
		}
		else {
			m_moveDir = CVector3D::Leap(m_moveDir, inputDir.GetNormalize(), 0.25f);
		}
		//プレイヤーの向きを徐々に移動方向へ向ける
		m_dir = CVector3D::Sleap(m_dir, m_moveDir, ROTATE_SPEED * CFPS::GetDeltaTime());
		//プレイヤーの向き反映
		m_rot.y = atan2f(m_dir.x, m_dir.z);

		//移動速度を取得
		if (HOLD(CInput::eButton11)) {
			float moveSpeed = SILENT_WALK_SPEED;
			//移動方向と移動速度から移動ベクトルを求める
			CVector3D moveVec = m_moveDir * moveSpeed;
			m_vec = CVector3D(moveVec.x, m_vec.y, moveVec.z);
			m_pos += m_vec;
		}
		else {
			float moveSpeed = DBUG_SPEED;
			//float moveSpeed = WALK_SPEED;
			//移動方向と移動速度から移動ベクトルを求める
			CVector3D moveVec = m_moveDir * moveSpeed;
			m_vec = CVector3D(moveVec.x, m_vec.y, moveVec.z);
			m_pos += m_vec;
		}
		//移動アニメーション再生
		m_model.ChangeAnimation((int)AnimId::Walk);
	}
	//キーが入力されていない
	else {
		//待機アニメーション再生
		m_model.ChangeAnimation((int)AnimId::Idle);
	}

	//鳴く	Cキー
	if (PUSH(CInput::eButton3)) {
		//new Effect("Nya",CVector3D(0, 0.8, 0), CVector3D(0, 0, 0), CVector3D(0, 0, 0), CVector3D(0, 0, 0), 0.5f, 0.0f, 1.0f, 0.0f, 0, true, false, 30);
		new Effect("Nya", m_pos + CVector3D(0, 0.8, 0), CVector3D(0, 0, 0), CVector3D(0, 0, 0), CVector3D(0, 0, 0), 0.5f, 0.0f, 1.0f, 0.0f, 0, true, false, 30);
		//鳴くアニメーション再生
		m_model.ChangeAnimation((int)AnimId::Naku);
		/*m_isBark = true;
		if (m_model.isAnimationEnd((int)AnimId::Naku)) {
			m_isBark = false;
		}*/
	}

	//ジャンプ
	if (m_isGround) {
		if (HOLD(CInput::eButton5)) {
			m_time += CFPS::GetDeltaTime();
			m_model.ChangeAnimation((int)AnimId::Jump, false);
		}
		else {
			//3秒以上Spaceキー長押しで大ジャンプ
			if (m_time >= 1) {//if (m_time >= 3) {
				//if (GameData::m_isdownflg) {
					//m_vec.y += HIGH_JUMP_SPEED + UP_SPEED;
				//}
				//else {
					m_vec.y += HIGH_JUMP_SPEED;
				//}
				m_isGround = false;
			}
			else if (m_time != 0) {
				//if (GameData::m_isdownflg) {
					//m_vec.y += JUMP_SPEED + UP_SPEED;
				//}
				//else {
					m_vec.y += JUMP_SPEED;
				//}
				m_isGround = false;
			}
			m_time = 0;
		}
	}
}

//透明状態
void Player::StateInvisible() {
	//移動速度をリセット
	m_vec.x = 0.0f;
	m_vec.z = 0.0f;
	//待機時間待ち
	if (m_elapsedTime < IDLE_TIME || Fade::IsFade()) {
		if (m_elapsedTime > 0.85f) {
			m_pos = CVector3D(0.0f, 10000.0f, 0.0f);
		}
		m_elapsedTime += CFPS::GetDeltaTime();
		/*m_isFindplayer = false;
		m_isHide = true;*/
	}
	else {
		
		Fade::FadeIn();
		GameData::m_diecnt++;
		m_elapsedTime = 0.0f;
		//m_isHide = false;
		m_pos = m_tyukan;
		m_enemyflg = false;
		//m_isDie = false;
		m_state = State::Move;
	}
}

void Player::Update() {
	m_model.UpdateAnimation();
	/*
	CVector3D key_dir(0, 0, 0);
	//カメラの方向ベクトル
	CVector3D cam_dir = CCamera::GetCurrent()->GetDir();
	//入力回転値
	float key_ang = 0;
	//カメラの回転値
	float cam_ang = atan2(cam_dir.x, cam_dir.z);

	if (!mp_camera) {
		mp_camera = dynamic_cast<PlayerCamera*>(TaskManeger::FindObject(TaskType::eCamera));
	}
	//キャラクターの回転値をカメラの回転値に合わせる
	m_rot.y = mp_camera->m_rot.y;

	if (HOLD(CInput::eLeft))key_dir.x = 1;
	if (HOLD(CInput::eRight))key_dir.x = -1;
	if (HOLD(CInput::eUp))key_dir.z = 1;
	if (HOLD(CInput::eDown))key_dir.z = -1;

	if (key_dir.LengthSq() > 0.1) {
		//入力方向から入力回転値を計算
		key_ang = atan2(key_dir.x, key_dir.z);
		//カメラの回転値と入力回転値からキャラクターの回転値を決定
		//即座にrot.yに設定せず一旦目標値を設定する
		//m_rot_target.y = cam_ang + key_ang;
		m_rot.y = cam_ang + key_ang;
		//方向ベクトル
		CVector3D dir(sin(m_rot.y), 0, cos(m_rot.y));
		//移動
		m_pos += dir * move_speed;
		m_model.ChangeAnimation(1);
	}
	else {
		m_model.ChangeAnimation(0);
	}
	m_model.UpdateAnimation();

	if (m_isGround) {
		if (HOLD(CInput::eButton5)) {
			m_time += CFPS::GetDeltaTime();
		}
		else {
			//3秒以上Spaceキー長押しで大ジャンプ
			if (m_time >= 1) {//if (m_time >= 3) {
				m_vec.y = 0.5f;
				m_isGround = false;
			}
			else if (m_time != 0) {
				m_vec.y = 0.3f;
				m_isGround = false;
			}
			m_time = 0;
		}
	}
	//printf("%.0f秒\n", m_time);
	if (!mp_isenemy) {
		mp_isenemy = TaskManeger::FindObject(TaskType::eEnemy);
	}
	if (PUSH(CInput::eMouseL) && !mp_isenemy->m_isFindplayer) {//if (HOLD(CInput::eMouseL)) {
		Shot();
	}
	*/
	// 視野範囲のカラー(初期色は緑)
	color = CVector4D(0.0f, 1.0f, 0.0f, 0.75f);

	/*if (!mp_enemy) {
		mp_enemy = dynamic_cast<Enemy*>(TaskManeger::FindObject(TaskType::eEnemy));
	}*/

	/*if (!mp_isenemy) {
		mp_isenemy = TaskManeger::FindObject(TaskType::eEnemy);
	}*/

	//メインカメラ取得
	if (!mp_camera) {
		mp_camera = dynamic_cast<PlayerCamera*>(TaskManeger::FindObject(TaskType::eCamera));
	}

	//現在の状態に合わせて、処理を切り替える
	switch (m_state)
	{
	//移動状態
	case State::Move:
		StateMove();
		break;
	//透明状態
	case State::Invisible:
		StateInvisible();
		break;
	}

	m_vec.y -= GRAVITY;

	////重力落下
	//if (m_vec.y < -10.0f) {//m_vec.y < -0.26f

	//}
	//else {
	//	m_vec.y -= GRAVITY * CFPS::GetDeltaTime();
	//}
	//移動
	//m_pos += m_vec * CFPS::GetDeltaTime();
	m_pos.y += m_vec.y;

	//カプセル
	m_lineS = m_pos + CVector3D(0, m_height - m_rad, 0);
	m_lineE = m_pos + CVector3D(0, m_rad, 0);

	if (PUSH(CInput::eMouseR)){
		printf("x:%.0f,y:%.0f,z:%.0f\n", m_pos.x, m_pos.y, m_pos.z);
	}

	// ベースクラスの更新
	CharaBase::Update();

	NavNode* node = NavManeger::Instance()->GetNearNavNode(m_navNode);

	//DebugPrint::Print("P:%f, %f, %f", m_pos.x, m_pos.y, m_pos.z);
	if (node != nullptr){
		CVector3D npos = node->GetPos();
		//DebugPrint::Print("N:%f, %f, %f", npos.x, npos.y, npos.z);
	}

	//デバッグ表示
	//Debug();
}

void Player::Render() {
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	//m_model.SetScale(1.0f, 1.0f, 1.0f);
	m_model.SetScale(0.003f, 0.003f, 0.003f);
	m_model.Render();
	//プレイヤーカプセルの表示
	Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(1, 0, 0, 1));
	//回転値から方向ベクトルを計算
	CVector3D dir(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
	//扇の見た目
	CMatrix m;
	m.LookAt(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + dir * m_viewLength, CVector3D(0, 1, 0));
	//Utility::DrawSector(m, -DtoR(m_viewAngle), DtoR(m_viewAngle), m_viewLength, color);

	/*CLight::SetType(1, CLight::eLight_Spot);
	CLight::SetPos(1, m_pos + CVector3D(0, 2, 0));
	CLight::SetDir(1, CVector3D(0, -1, 0));
	CLight::SetColor(1, CVector3D(0.1f, 0.1f, 0.1f), CVector3D(0.6f, 0.6f, 0.6f));
	CLight::SetType(0, CLight::eLight_Spot);
	CLight::SetPos(0, m_pos + CVector3D(0, 0.5, 0));
	CLight::SetDir(0, m_moveDir);
	CLight::SetColor(0, CVector3D(0.1f, 0.1f, 0.1f), CVector3D(0.6f, 0.6f, 0.6f));
	CLight::SetType(2, CLight::eLight_Spot);
	CLight::SetPos(2, m_pos + CVector3D(0, 0, -2));
	CLight::SetDir(2, -m_moveDir);
	CLight::SetColor(2, CVector3D(0.1f, 0.1f, 0.1f), CVector3D(0.6f, 0.6f, 0.6f));*/
	
}

void Player::NoEnemyRender()
{
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	//m_model.SetScale(1.0f, 1.0f, 1.0f);
	m_model.SetScale(0.003f, 0.003f, 0.003f);
	m_model.Render();
	//プレイヤーカプセルの表示
	//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(1, 0, 0, 1));
	
	//回転値から方向ベクトルを計算
	CVector3D dir(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
	//扇の見た目
	CMatrix m;
	m.LookAt(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + dir * m_viewLength, CVector3D(0, 1, 0));
	//Utility::DrawSector(m, -DtoR(m_viewAngle), DtoR(m_viewAngle), m_viewLength, color);
}

bool Player::IsFoundEnemy(Enemy* e) const
{
	CVector3D enemyPos = e->m_pos;

	// 敵からプレイヤーまでのベクトルを求める
	CVector3D vec = enemyPos - m_pos;

	// 敵の正面ベクトル と 敵からプレイヤーまでのベクトル で
	// 内積を取って角度を求める
	float dot = CVector3D::Dot(m_dir, vec.GetNormalize());
	// 求めた角度(内積)が視野角度内か判断する
	if (dot < cos(DtoR(m_viewAngle))) return false;

	// 敵からプレイヤーまでの距離を求める
	float length = vec.Length();
	// 求めた距離が視野距離内か判断する
	if (length > m_viewLength) return false;

	// 最後に自身からプレイヤーまでの間に
	// 遮蔽物がないか判定する
	if (!IsSearchEnemy(e)) return false;
	
	return true;
}

//現在地から敵がいるか
bool Player::IsSearchEnemy(Enemy* e) const
{
	NavNode* playerNode = m_navNode;
	NavNode* enemyNode = e->GetNavNode();
	CVector3D playerNodePos = playerNode->GetPos();
	CVector3D enemyNodePos = enemyNode->GetPos();
	CVector3D vec = playerNodePos - enemyNodePos;

	/*CVector3D enemyPos = e->m_pos;
	CVector3D vec = enemyPos - m_pos;*/
	// 現在位置からプレイヤーまでの距離を求める
	float dist = vec.Length();



	// プレイヤーの位置までのレイと壁との衝突判定を行う
	/*CVector3D start = m_pos;
	CVector3D end = enemyPos;*/
	CVector3D start = enemyNodePos;
	CVector3D end = playerNodePos;
	/*start.y = 1.0f;
	end.y = 1.0f;*/
	CVector3D hitPos, hitNormal;
	if (Stage::CollisionRay(start, end, &hitPos, &hitNormal))
	{
		float hitDist = (hitPos - start).Length();
		if (dist > hitDist) return false;
	}

	// 壁と衝突していないもしくは、
	// 衝突位置がプレイヤーより奥の位置であるならば、
	// 視線が通っているので、プレイヤーが見える状態
	return true;
}

void Player::Shot(Enemy* e){
	if (IsFoundEnemy(e)) {
		e->m_isFind = true;
	}
	else {
		if (e != nullptr) {
			e->m_isFind = false;
		}
	}
}

void Player::Collision(Task* b) {
	switch (b->GetType()) {
	case TaskType::eField:
	{
		CVector3D v(0, 0, 0);
		//カプセルとモデルの衝突
		auto tri = b->GetModel()->CollisionCupsel(m_lineS, m_lineE, m_rad);
		//接触した面の数繰り返す
		for (auto& t : tri) {
			if (t.m_normal.y < -0.5f) {//if (t.m_normal.y < 0.5f) {
				//面が下向き→天井に当たった	
				//上昇速度を0に戻す
				if (m_vec.y > 0)
					m_vec.y = 0;
			}
			else if (t.m_normal.y > 0.5f) {//else if (t.m_normal.y > -0.5f) {
				//面が上向き→地面に当たった
				//重力落下速度を0に戻す
				if (m_vec.y < 0)
					m_vec.y = 0;
				m_isGround = true;
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
	}
	break;
	case TaskType::eEnemy:
	{
		Enemy* enemy = dynamic_cast<Enemy*>(b);
		if (enemy != nullptr) {
			if (IsFoundEnemy(enemy)) {
				m_isSearch = true;
				if (m_isSearch && !enemy->m_isTouch && !enemy->m_isFindplayer) {
					new Effect2D("Nekonote");
				}
				if (PUSH(CInput::eMouseL) && !enemy->m_isFindplayer) {//if (HOLD(CInput::eMouseL)) {
					Shot(enemy);

					m_isSearch = false;
				}
			}
			else {
				m_isSearch = false;
			}
		}

		//敵の判定
		CVector3D c1, dir1, c2, dir2;
		float dist;
		if (!m_isHide) {
			if (!m_enemyflg) {
				if (CCollision::CollisionCapsule(m_lineS, m_lineE, m_rad,
					b->m_lineS, b->m_lineE, b->m_rad,
					&dist, &c1, &dir1, &c2, &dir2)) {
					m_enemyflg = true;
				}
				if (m_enemyflg) {
					//m_pos = m_pos - CVector3D(0.1f, 0, 0.1f);
					Fade::FadeOut();
					m_state = State::Invisible;
				}
			}
		}
	}
	break;
	case TaskType::eTyukanBox:
	{
		//dynamic_castは触れている対象を取得※Baseにあるpublicの関数・変数が使える
		if (TyukanBox* t = dynamic_cast<TyukanBox*>(b)) {
			//■OBBとカプセル
			float dist;
			CVector3D axis;
			if (CCollision::CollisionOBBCapsule(t->m_obb, m_lineS, m_lineE, m_rad, &axis, &dist)) {
				m_tyukan = t->m_pos;
			}
		}
	}
	break;
	case TaskType::eFellBox:
	{
		if (m_state != State::Invisible) {
			//mp_fellbox = TaskManeger::FindObject(TaskType::eFellBox);
			FellBox* fellbox = dynamic_cast<FellBox*>(b);
			//■OBBとカプセル
			float dist;
			CVector3D axis;
			if (CCollision::CollisionOBBCapsule(fellbox->m_obb, m_lineS, m_lineE, m_rad, &axis, &dist)) {
				Fade::FadeOut();
				//m_isDie = true;
				m_state = State::Invisible;
				//m_pos = m_tyukan;
			}
		}
	}
	break;
	}
}

void Player::LateUpdate(){
	CVector4D color = CVector4D(0.0f, 0.0f, 1.0f, 1.0f);
	m_gauge = (257.0f / 1.0f * (float)m_time);//m_gauge = (257.0f / 3.0f * (float)m_time);
	if (m_gauge >= 257) {
		m_gauge = 257;
		color = CVector4D(cosf(m_time * 2.0f), sinf(m_time * 7.0f), tanf(m_time * 11.0f), 1);
		//color = CVector4D(cosf(m_time * 5), sinf(m_time * 5), tanf(m_time * 5), 1);
	}
	Utility::DrawQuad(CVector2D(0, 1080), CVector2D(259, -259), CVector4D(0.0f, 0.0f, 0.0f, 1.0f));
	Utility::DrawQuad(CVector2D(1, 1079), CVector2D(257, -m_gauge), color);
	
}

void Player::Debug(){
	DebugPrint::Print("Player座標 : X:%f , Y:%f , Z:%f", m_pos.x, m_pos.y, m_pos.z);
}

