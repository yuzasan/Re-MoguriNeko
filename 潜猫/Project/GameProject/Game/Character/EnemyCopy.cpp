#include "EnemyCopy.h"
#include "Player.h"
#include "NoiseBox.h"
#include "../Stage/Item.h"
#include "../Stage/Stage.h"
#include "../GameData/GameData.h"
#include "../../Effect/Effect.h"
#include "../../Debug/DebugPrint.h"
#include "../../Navigation/NavNode.h"
#include "../../Navigation/NavManeger.h"
#include "EnemyManagerCopy.h"


//歩き速度
#define WALK_SPEED 2.5f//5.0f//4.8f//2.4f
//追跡速度
//#define CHASE_SPEED 0.05f
//回転速度
#define ROTATE_SPEED 15.0f
//待機時間
#define IDLE_TIME 2.0f
//目的地との間距離
#define SPACE 1.0f
//重力
//#define G 0.8f
//最大重力
//#define MAX_G 0.001f

#define DBUG_SPEED 5.0f

//移動先
#define MOVE_RANGE_MIN -48.0f//-6.0f//-13.0f
#define MOVE_RANGE_MAX 48.0f//6.0f//13.0f

EnemyC::EnemyC(const CVector3D& pos, float emotion) :CharaBase(TaskType::eEnemy)
, mp_player(nullptr)
, mp_noise(nullptr)
, mp_field(nullptr)
, m_moveDir(0.0f, 0.0f, 0.0f)
, m_movePos(0.0f, 0.0f, 0.0f)
, m_moveNode(nullptr)
, m_elapsedTime(0.0f)
, m_action(Action::Idle)
, isFoll(false)
, m_cntfoll(0)
, m_isTouch(false)
, m_isWall(false)
, m_nextNode(nullptr)
, m_lostNode(nullptr)
, m_emotion(emotion)
{
	//敵の管理クラスのリストに自身を追加
	EnemyManagerC::Instance()->Add(this);

	mp_player = dynamic_cast<Player*>(TaskManeger::FindObject(TaskType::ePlayer));
	//mp_hidebox = Base::FindObject(BaseType::eHideBox);
	//m_model = COPY_RESOURCE("Golem", CModelA3M);
	m_model = COPY_RESOURCE("Samurai", CModelA3M);
	m_modelobj = COPY_RESOURCE("Tenohira", CModelObj);
	m_pos = pos;
	m_dir = CVector3D(0.0f, 0.0f, 1.0f);
	m_moveDir = m_dir;

	//m_rad = 0.8f / 2.0f;
	//m_rad = 0.8f;
	m_rad = 0.3f;
	//m_rad = 0.2f;
	m_isFind = false;
	m_isFindplayer = false;

	//敵の経路探索用のノードを作成
	m_navNode = new NavNode
	(
		m_pos + CVector3D(0.0f, 1.0f, 0.0f),
		NavNode::NodeType::Destination
	);
	m_navNode->SetNodeColor(CVector3D(0.125f, 0.25f, 1.0f));
}

//デストラクタ
EnemyC::~EnemyC() {
	//敵の管理クラスのリストから自信を取り除く
	EnemyManagerC::Instance()->Remove(this);
}

//待機状態の処理
void EnemyC::ActionIdle()
{
	switch (m_state) {
	case State::Sleep:
		//スリープアニメーション
		m_model.ChangeAnimation((int)AnimId::Sleeping);
		break;
	case State::Sad:
		//落ち込みアニメーション
		m_model.ChangeAnimation((int)AnimId::SadIdle);
		break;
	case State::Cross:
		//イライラアニメーション
		m_model.ChangeAnimation((int)AnimId::CrossIdle2);
		break;
	case State::Warning:
		//警戒アニメーション
		m_model.ChangeAnimation((int)AnimId::Idle);
		break;
	case State::Normal:
		//待機アニメーション
		m_model.ChangeAnimation((int)AnimId::Idle);
		break;
	case State::Happy:
		//うれしいアニメーション
		m_model.ChangeAnimation((int)AnimId::HappyIdle2);
		break;
	}

	m_vec.x = 0.0f;
	m_vec.z = 0.0f;

	//待機時間待ち
	if (m_elapsedTime < IDLE_TIME) {
		m_elapsedTime += CFPS::GetDeltaTime();
	}
	//待機時間が終われば、
	else {
		m_elapsedTime = 0.0f;

		//次に探索するノードを取得
		SearchNodeC* node = EnemyManagerC::Instance()->GetNextSearchNode();
		//if (mp_noise != nullptr) {
		if (mp_noise->m_isNoise && !mp_noise->m_isNoisemove) {
			m_moveNode = NavManeger::Instance()->GetNearNavNode(mp_noise->m_pos);
			mp_noise->m_isNoisemove = true;
		}
		//}
		else if (node != nullptr) {
			//探索ノードに一番近いノードを目的地とする
			m_searchNode = node;
			m_searchNode->enemyc = this;
			m_moveNode = NavManeger::Instance()->GetNearNavNode(m_searchNode->pos);
		}
		//探索するノードが存在しなかった場合
		else {
			//移動位置をランダムで求める
			m_movePos.x = Utility::Rand(MOVE_RANGE_MIN, MOVE_RANGE_MAX);
			m_movePos.z = Utility::Rand(MOVE_RANGE_MIN, MOVE_RANGE_MAX);
			m_moveNode = NavManeger::Instance()->GetNearNavNode(m_movePos);
		}
		//移動状態へ移行
		//m_action = Action::Move;
		if (m_state != State::Warning) {
			m_action = Action::Move;
		}
		else {
			m_action = Action::Search;
		}
	}

	//プレイヤーを見つけたら、強制的に追跡状態へ
	//if (IsFoundPlayer() && m_emotion > 0.5f) {
	if (IsFoundPlayer() && m_state != State::Happy) {
		m_action = Action::Chase;
	}
}

//移動状態の処理
void EnemyC::ActionMove()
{
	switch (m_state) {
	case State::Sad:
		//待機アニメーション
		m_model.ChangeAnimation((int)AnimId::SadWalk);
		break;
	case State::Cross:
		//待機アニメーション
		m_model.ChangeAnimation((int)AnimId::CrossWalk2);
		break;
	case State::Warning:
		//警戒アニメーション
		m_model.ChangeAnimation((int)AnimId::Walk);
		break;
	case State::Normal:
		//待機アニメーション
		m_model.ChangeAnimation((int)AnimId::Walk);
		break;
	case State::Happy:
		//待機アニメーション
		m_model.ChangeAnimation((int)AnimId::HappyWalk);
		break;
	}
	if (m_isFind && !IsFoundPlayer()) {
		isFoll = true;
		m_isFind = false;
		m_isTouch = true;
	}
	else {
		FollBox();
		if (m_moveNode != nullptr) {
			//現在位置から目的地のモードまでの経路探索を行う
			NavManeger* navMgr = NavManeger::Instance();
			//if (mp_noise != nullptr) {
			if (mp_noise->m_isNoise && mp_noise->m_isNoisemove) {
				NavNode* noiseNode = mp_noise->GetNavNode();
				if (!mp_noise->m_isKill) {
					m_nextNode = navMgr->Navigate(m_navNode, noiseNode);
				}
				else {
					m_nextNode = navMgr->Navigate(m_navNode, m_moveNode);
				}
			}
			//}
			else {
				m_nextNode = navMgr->Navigate(m_navNode, m_moveNode);
			}

			//次に移動すべきノードが存在すれば、
			if (m_nextNode != nullptr) {
				//次に移動すべきノードへ向けて移動
				if (MoveTo(m_nextNode->GetPos())) {
					//移動が終われば、目的地のノードと
					//次に移動すべきノードをnullにする
					m_moveNode = nullptr;
					m_nextNode = nullptr;
					//探索ノードが存在しない場合は、そのまま待機状態へ
					if (m_searchNode == nullptr) {
						m_action = Action::Idle;
					}
				}
			}
		}
		else {
			//探索ノードまで移動
			if (MoveTo(m_searchNode->pos)) {
				//移動が終われば、
				m_searchNode->prob = 0.0f;
				m_searchNode->enemyc = nullptr;
				m_searchNode = nullptr;
				m_action = Action::Idle;
			}
		}
	}

	//プレイヤーを見つけたら、強制的に追跡状態へ
	//if (IsFoundPlayer()&&m_emotion > 0.5f) {
	if (IsFoundPlayer() && m_state != State::Happy) {
		m_action = Action::Chase;
	}
}

void EnemyC::ActionSearch() {
	//Vector3D dir;
	m_model.ChangeAnimation((int)AnimId::BackSearch, false);
	// プレイヤーの向きを徐々に移動方向へ向ける
	//m_dir = CVector3D::Sleap(m_dir, -m_dir, (ROTATE_SPEED-14.9999f) * CFPS::GetDeltaTime());
	//回転値から方向ベクトルを計算
	//dir = CVector3D(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
	if (m_model.isAnimationEnd()) {
		m_action = Action::Move;
	}
}

void EnemyC::ActionChase()
{
	//GameData::m_islostflg = false;
	/*if (m_emotion <= 0.5f) {
		m_state = State::Move;
		return;
	}*/
	color = CVector4D(1.0f, 0.0f, 0.0f, 1.0f);

	switch (m_state)
	{
	case State::Cross:
		//イライラ全力疾走アニメーション
		m_model.ChangeAnimation((int)AnimId::Run);
		break;
	default:
		//前進アニメーション
		m_model.ChangeAnimation((int)AnimId::Walk);
		break;
	}

	//各ノードの座標を取得
	NavNode* playerNode = mp_player->GetNavNode();
	NavNode* enemyNode = m_navNode;
	CVector3D playerNodePos = playerNode->GetPos();
	CVector3D enemyNodePos = enemyNode->GetPos();
	//現在位置からプレイヤーの位置までの距離を求める
	CVector3D vec = playerNodePos - enemyNodePos;
	float dist = vec.Length();

	NavManeger* navMgr = NavManeger::Instance();

	//プレイヤーの位置まで視線が通っているか判定
	CVector3D hitPos, hitNormal;
	/*enemyNodePos.y = 1.0f;
	playerNodePos.y = 1.0f;*/
	bool isHit = Stage::CollisionRay(enemyNodePos, playerNodePos, &hitPos, &hitNormal);
	//プレイヤーの位置までレイを飛ばして、何かに衝突したら、
	if (isHit) {
		//プレイヤーまでの距離 > 衝突地点までの距離ならば、
		//自身とプレイヤーまで間に遮蔽物が存在する
		float hitDist = (hitPos - enemyNodePos).Length();
		isHit = dist > hitDist;
	}

	//プレイヤーの位置まで遮蔽物が存在しなければ、
	if (!isHit) {
		//現在位置からプレイヤーの位置までの経路探索を行う
		m_nextNode = navMgr->Navigate(enemyNode, playerNode);

		//見失った時の最終位置に配置するノードを準備
		if (m_lostNode == nullptr) {
			m_lostNode = new NavNode
			(
				playerNodePos,
				NavNode::NodeType::Destination
			);
			m_lostNode->SetNodeColor(CVector3D(1.0f, 0.5f, 0.25f));
		}
		//プレイヤーの最終位置用のノードが既に存在するならば、
		//プレイヤーの最終位置を更新
		else {
			CVector3D range = (playerNodePos - enemyNodePos).GetNormalize();
			//m_lostNode->SetPos(playerNodePos - range * 0.5f);
			m_lostNode->SetPos(playerNodePos);
		}
		m_isFind = false;
	}
	//プレイヤーとの間に遮蔽物が存在する
	else {
		//自身とプレイヤーのノードの接続を切る
		enemyNode->RemoveConnect(playerNode);

		//現在位置からプレイヤーの最終位置までの経路探索を行う
		m_nextNode = navMgr->Navigate(enemyNode, m_lostNode);

		//見失い状態へ移行
		m_action = Action::Lost;
		return;
	}

	//次に移動すべきノードが存在する場合は、
	//そのノードの位置まで移動
	if (m_nextNode != nullptr) {
		MoveTo(m_nextNode->GetPos());
	}
	//次に移動すべきノードが存在しない場合は、
	//待機状態へ移行
	else {
		m_action = Action::Idle;
	}
}

//プレイヤーを見失った状態の処理
void EnemyC::ActionLost() {
	GameData::m_islostflg = false;
	if (!mp_player->m_isHide) {
		//目的地が存在する
		if (m_nextNode != nullptr) {
			//見失った場合は、視野範囲を無視して、
			//プレイヤーまでの視線が通るかどうかで判定する
			if (IsLookPlayer()) {
				if (!mp_player->m_isHide && !GameData::m_islostflg) {
					//CVector3D pos = mp_player->m_pos - m_pos;
					//if (IsLookPlayer() && pos.Length() < 30.0f || !mp_player->m_isHide && !GameData::m_islostflg) {
						//追跡状態へ移行
					m_action = Action::Chase;
					m_isFind = false;
				}
			}
			//プレイヤーが視線の通らないところにいる
			else {
				GameData::m_islostflg = true;
				//目的地まで移動
				if (MoveTo(m_nextNode->GetPos())) {
					if (m_nextNode != m_lostNode) {
						NavManeger* navMgr = NavManeger::Instance();
						m_nextNode = navMgr->Navigate(m_nextNode, m_lostNode);
					}
					else {
						delete m_lostNode;
						m_lostNode = nullptr;
						m_nextNode = nullptr;
					}
				}
			}
		}
		//目的地まで移動が終われば、
		else {
			//new Effect("Fukidasi",CVector3D(0, 2.5, 0), CVector3D(0, 0, 0), CVector3D(0, 0, 0), CVector3D(0, 0, 0), 1.0f, 0.0f, 1.0f, 0.0f, 0, true, false, 60);
			//new Effect("Fukidasi", m_pos + CVector3D(0, 2.5, -1), CVector3D(0, 0, 0), CVector3D(0, 0, 0), CVector3D(0, 0, 0), 1.0f, 0.0f, 1.0f, 0.0f, 0, true, false, 60);
			new Effect("Kinosei", m_pos + CVector3D(0, 2.5, -1), CVector3D(0, 0, 0), CVector3D(0, 0, 0), CVector3D(0, 0, 0), 1.0f, 0.0f, 1.0f, 0.0f, 0, true, false, 60);
			//待機状態へ移行
			m_action = Action::Idle;
		}
	}
	else {
		//待機状態へ移行
		m_action = Action::Idle;
	}
}

int EnemyC::CollisionLine(const CVector3D& lineS, const CVector3D& lineE, const CVector3D& dir, CVector3D* cross) {
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

bool EnemyC::IsFoundPlayer() {
	CVector3D dir;
	if (m_state != State::Warning) {
		//回転値から方向ベクトルを計算
		//CVector3D dir(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
		dir = CVector3D(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
	}
	else {
		dir = CVector3D(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
		m_dir = dir;
	}
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

//現在位置からプレイヤーが見えるかどうか
bool EnemyC::IsLookPlayer() const {

	NavNode* playerNode = mp_player->GetNavNode();
	NavNode* enemyNode = m_navNode;
	CVector3D playerNodePos = playerNode->GetPos();
	CVector3D enemyNodePos = enemyNode->GetPos();
	CVector3D vec = playerNodePos - enemyNodePos;
	//現在位置からプレイヤーまでの距離を求める
	float dist = vec.Length();

	//プレイヤーの位置までのレイと壁との衝突判定を行う
	CVector3D start = enemyNodePos;
	CVector3D end = playerNodePos;
	/*start.y = 1.0f;
	end.y = 1.0f;*/
	CVector3D hitPos, hitNormal;
	if (Stage::CollisionRay(start, end, &hitPos, &hitNormal)) {
		float hitDist = (hitPos - start).Length();
		if (dist > hitDist)return false;
	}

	//壁と衝突していないもしくは、
	//衝突位置がプレイヤーより奥の位置であるならば、
	//視野が通っているので、プレイヤーが見える状態
	return true;

	/*
	CVector3D playerPos = mp_player->m_pos;
	CVector3D vec = playerPos - m_pos;
	//現在位置からプレイヤーまでの距離を求める
	float dist = vec.Length();

	//プレイヤーの位置までのレイと壁との衝突判定を行う
	CVector3D start = m_pos;
	CVector3D end = playerPos;
	//start.y = 1.0f;
	//end.y = 1.0f;
	CVector3D hitPos, hitNormal;
	if (Stage::CollisionRay(start, end, &hitPos, &hitNormal)) {
		float hitDist = (hitPos - start).Length();
		if (dist > hitDist)return false;
	}

	//壁と衝突していないもしくは、
	//衝突位置がプレイヤーより奥の位置であるならば、
	//視野が通っているので、プレイヤーが見える状態
	return true;
	*/
}

//指定座標への移動処理
bool EnemyC::MoveTo(const CVector3D& target) {

	float moveSpeed = DBUG_SPEED * (m_emotion / 100);
	//float moveSpeed = WALK_SPEED * (m_emotion/100);

	CVector3D vec = target - m_pos;
	vec.y = 0.0f;
	m_moveDir = vec.GetNormalize();

	//目的地までの距離が移動距離より大きいならば
	float dist = vec.Length();
	float speed = moveSpeed * CFPS::GetDeltaTime();
	if (dist > moveSpeed * CFPS::GetDeltaTime()) {
		CVector3D moveVec = m_moveDir * moveSpeed;
		m_vec = CVector3D(moveVec.x, m_vec.y, moveVec.z);
	}
	//距離が移動速度より短いのであれば、
	else {
		CVector3D moveVec = m_moveDir * dist;
		m_vec = CVector3D(moveVec.x, m_vec.y, moveVec.z);
		//移動終了
		return true;
	}

	//移動継続
	return false;
}

void EnemyC::Shot() {
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
		//if (s->GetModel()->CollisionRay(&hit_field_point, &n, lineS, lineE)) {
		if (s->GetColNavModel()->CollisionRay(&hit_field_point, &n, lineS, lineE)) {
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

//探索ノードを設定
void EnemyC::SetSearchNode(SearchNodeC* node)
{
	m_searchNode = node;
}

//探索中のノードを取得
SearchNodeC* EnemyC::GetSearchNode() const
{
	return m_searchNode;
}


void EnemyC::Update()
{
	//物音BOX取得
	if (!mp_noise)
	{
		mp_noise = dynamic_cast<NoiseBox*>(TaskManeger::FindObject(TaskType::eNoiseBox));
	}

	// 視野範囲のカラー(初期色は緑)
	color = CVector4D(0.0f, 1.0f, 0.0f, 0.75f);

	if (m_emotion <= 5) {
		m_state = State::Sleep;
	}
	else if (m_emotion <= 25) {
		m_state = State::Sad;
	}
	else if (m_emotion <= 50) {
		m_state = State::Warning;
	}
	else if (m_emotion <= 75) {
		m_state = State::Normal;
	}
	else if (m_emotion <= 100) {
		m_state = State::Happy;
	}
	else {
		m_state = State::Cross;
	}

	/*
	if (m_emotion <= 0) {
		m_state = State::Sleep;
	}
	else if (m_emotion <= 25) {
		m_state = State::Sad;
	}
	else if (m_emotion <= 50) {
		m_state = State::Cross;
	}
	else if (m_emotion <= 75) {
		m_state = State::Warning;
	}
	else if (m_emotion <= 100) {
		m_state = State::Normal;
	}
	else{
		m_state = State::Happy;
	}
	*/

	// 現在の状態に合わせて、処理を切り替える
	switch (m_action)
	{
		// 待機行動
	case Action::Idle:
		ActionIdle();
		break;
		// 移動行動
	case Action::Move:
		ActionMove();
		break;
		// 追跡行動
	case Action::Chase:
		ActionChase();
		//color = CVector4D(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case Action::Search:
		ActionSearch();
		break;
		//プレイヤーを見失った
	case Action::Lost:
		ActionLost();
		color = CVector4D(1.0f, 1.0f, 0.0f, 0.75f);
		break;
	}

	//アニメーション更新
	m_model.UpdateAnimation();

	if (m_action != Action::Search) {
		// プレイヤーの向きを徐々に移動方向へ向ける
		m_dir = CVector3D::Sleap(m_dir, m_moveDir, ROTATE_SPEED * CFPS::GetDeltaTime());
	}
	// プレイヤーの向き反映
	m_rot.y = atan2f(m_dir.x, m_dir.z);

	/*m_vec.x *= G;
	if (abs(m_vec.x) < MAX_G)m_vec.x = 0;
	m_vec.z *= G;
	if (abs(m_vec.z) < MAX_G)m_vec.z = 0;*/

	if (m_vec.y < -10.0f) {//m_vec.y < -0.26f

	}
	else {
		m_vec.y -= GRAVITY * CFPS::GetDeltaTime();
	}

	//m_pos.y += m_vec.y;
	m_pos += m_vec * CFPS::GetDeltaTime();

	//カプセル更新
	float height = 2.0f;
	m_lineS = m_pos + CVector3D(0, height - m_rad, 0);
	m_lineE = m_pos + CVector3D(0, m_rad, 0);

	// ベースクラスの更新
	CharaBase::Update();

	//デバッグ表示
	//sDebug();
}

void EnemyC::Render()
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
	/*Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + dir * view_length, CVector4D(1, 0, 1, 1), lineWidth);
	Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + CMatrix::MRotationY(view_ang) * dir * view_length, CVector4D(0, 1, 1, 1), lineWidth);
	Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + CMatrix::MRotationY(-view_ang) * dir * view_length, CVector4D(0, 1, 1, 1), lineWidth);*/
	//扇の見た目
	CMatrix m;
	m.LookAt(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + dir * view_length, CVector3D(0, 1, 0));
	Utility::DrawSector(m, -view_ang, view_ang, view_length, color);

	//if (mp_player->m_isFind && !IsFoundPlayer()) {
	if (m_isTouch) {//m_isFind && !IsFoundPlayer()
		//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(1, 0, 1, 1));
		//Utility::DrawCube(m_pos + CVector3D(0, 2, 0), CVector3D(1, 0, 1), CVector4D(1, 0, 1, 1));

		switch (m_state)
		{
		case EnemyC::State::Normal:
			Utility::DrawSphere(m_pos + CVector3D(0, 2.0f, 0), 0.1f, CVector4D(0, 1, 0, 1));
			break;
		case EnemyC::State::Happy:
			Utility::DrawSphere(m_pos + CVector3D(0, 2.0f, 0), 0.1f, CVector4D(1, 0, 0.5, 1));
			break;
		case EnemyC::State::Warning:
			Utility::DrawSphere(m_pos + CVector3D(0, 2.0f, 0), 0.1f, CVector4D(1, 1, 0, 1));
			break;
		case EnemyC::State::Cross:
			Utility::DrawSphere(m_pos + CVector3D(0, 2.0f, 0), 0.1f, CVector4D(1, 0, 0, 1));
			break;
		case EnemyC::State::Sad:
			Utility::DrawSphere(m_pos + CVector3D(0, 2.0f, 0), 0.1f, CVector4D(0, 0, 1, 1));
			break;
		case EnemyC::State::Sleep:
			Utility::DrawSphere(m_pos + CVector3D(0, 2.0f, 0), 0.1f, CVector4D(1, 1, 1, 1));
			break;
		default:
			break;
		}
		m_modelobj.SetPos(m_pos + CVector3D(0, 2, 0));
		m_modelobj.SetRot(CVector3D(0, 1, 0));
		m_modelobj.Render();
	}
	else {
		//敵カプセルの表示
		//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(0, 0, 1, 1));
		//Utility::DrawCube(m_pos + CVector3D(0, 2, 0), CVector3D(1, 0, 1), CVector4D(0, 0, 1, 1));
	}

}

void EnemyC::Collision(Task* b)
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
			//if (max_y > m_pos.y + 1) {
			v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
			v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;
			//}
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
			/*float s = (m_rad + b->m_rad) - dist;
			b->m_pos += dir1 * s * 0.5f;
			b->m_lineS += dir1 * s * 0.5f;
			b->m_lineE += dir1 * s * 0.5f;
			m_pos += dir2 * s * 0.5f;
			m_lineS += dir2 * s * 0.5f;
			m_lineE += dir2 * s * 0.5f;*/

		}
	}
	break;
	}
}

void EnemyC::FollBox() {
	if (isFoll && m_cntfoll < 1) {
		/*
		Base::Add(new Item(
			m_pos + CVector3D(0, 0.5f, 0),
			CVector3D(0.0f, 0.0f, 0.0f),
			CVector3D(0.1f, 0.1f, 0.1f)
		));
		*/
		new Item2(
			m_pos + CVector3D(0, 0.5f, 0),
			CVector3D(0.0f, 0.0f, 0.0f),
			CVector3D(0.1f, 0.1f, 0.1f)
		);
		m_cntfoll++;
	}
	isFoll = false;
}

void EnemyC::LateRender() {
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
		/*Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + dir * view_length, CVector4D(1, 0, 1, 1), lineWidth);
		Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + CMatrix::MRotationY(view_ang) * dir * view_length, CVector4D(0, 1, 1, 1), lineWidth);
		Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + CMatrix::MRotationY(-view_ang) * dir * view_length, CVector4D(0, 1, 1, 1), lineWidth);*/
		//扇の見た目
		CMatrix m;
		m.LookAt(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + dir * view_length, CVector3D(0, 1, 0));
		Utility::DrawSector(m, -view_ang, view_ang, view_length, color);

		//if (mp_player->m_isFind && !IsFoundPlayer()) {
		if (m_isTouch) {//m_isFind && !IsFoundPlayer()
			//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(1, 0, 1, 1));
			//Utility::DrawCube(m_pos + CVector3D(0, 2, 0), CVector3D(1, 0, 1), CVector4D(1, 0, 1, 1));
			m_modelobj.SetPos(m_pos + CVector3D(0, 2, 0));
			m_modelobj.SetRot(CVector3D(0, 1, 0));
			m_modelobj.Render();
		}
		else {
			//敵カプセルの表示
			//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(0, 0, 1, 1));
			Utility::DrawCube(m_pos + CVector3D(0, 2, 0), CVector3D(1, 0, 1), CVector4D(0, 0, 1, 1));
		}
	}
}

void EnemyC::Debug() {
	DebugPrint::Print("Enemy座標 : X:%f , Y:%f , Z:%f", m_pos.x, m_pos.y, m_pos.z);
	std::string action = "";
	switch (m_action)
	{
	case EnemyC::Action::Idle: action = "待機"; break;
	case EnemyC::Action::Move: action = "移動"; break;
	case EnemyC::Action::Chase:action = "追跡"; break;
	case EnemyC::Action::Lost: action = "見失った"; break;
	}
	DebugPrint::Print("State:%s", action.c_str());
}
