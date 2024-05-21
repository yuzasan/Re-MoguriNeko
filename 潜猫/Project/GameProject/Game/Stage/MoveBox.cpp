#include "MoveBox.h"
#include "../Gamedata/GameData.h"

MoveBox::MoveBox(const CVector3D& pos, const CVector3D& rot, const CVector3D& size,
	const CVector3D& startPos, const CVector3D& endPos,
	const float moveTime, const float intervalTime,const int yxz, const int cnt, bool isStart)//const float moveTime, const float intervalTime,const int number, bool isStart)
	: Task(TaskType::eMoveBox)
	, m_startPos(startPos)
	, m_endPos(endPos)
	, m_moveTime(moveTime)
	, m_intervalTime(intervalTime)
	, m_yxz(yxz)//, m_number(number)
	, m_cnt(cnt)
	, m_state(State::eMove)
	, m_isEnable(isStart)
	, m_isReverse(false)
	, m_elapsedTime(0.0f)
	, m_isMoveflg(false)
{
	m_pos = pos;
	m_rot = rot;
	m_boxSize = size;
	m_obb = COBB(
		m_pos,
		m_rot,
		m_boxSize
	);
	m_obb_old = COBB(
		m_pos,
		m_rot,
		m_boxSize
	);
	mp_player = TaskManeger::FindObject(TaskType::ePlayer);
}

MoveBox::~MoveBox() {

}

void MoveBox::Start()
{
	m_isEnable = true;
}

void MoveBox::Stop()
{
	m_isEnable = false;
}

//移動状態の更新処理
void MoveBox::UpdateMove()
{
	////開始位置と終了位置を取得
	//CVector3D start = m_isReverse ? m_endPos : m_startPos;
	//CVector3D end = m_isReverse ? m_startPos : m_endPos;
	//
	////経過時間が移動時間を超えていない場合
	//if (m_elapsedTime < m_moveTime)
	//{
	//	if (m_pos.y <= m_endPos.y) {
	//		m_pos.y += 0.1f;
	//		m_obb = COBB(
	//			m_pos,
	//			m_rot,
	//			m_boxSize
	//		);
	//	}
	//	//経過時間を加算
	//	m_elapsedTime += CFPS::GetDeltaTime();
	//}
	////移動終了
	//else
	//{
	//	m_obb = COBB(
	//		m_pos,
	//		m_rot,
	//		m_boxSize
	//	);
	//	//移動方向を反転
	//	m_isReverse = !m_isReverse;
	//	m_elapsedTime -= m_moveTime;
	//	//待機状態へ移行
	//	m_state = State::eInterval;
	//}
	switch (m_yxz)//switch (m_number)
	{
	case 0://y軸（上下）
		if (m_cnt % 2 == 0) {
			if (m_pos.y <= m_endPos.y) {
				//m_pos.y += MOVE_SPEED;
				m_pos.y += MOVE_SPEED * CFPS::GetDeltaTime();
				m_obb = COBB(
					m_pos,
					m_rot,
					m_boxSize
				);
			}
			else {
				m_obb = COBB(
					m_pos,
					m_rot,
					m_boxSize
				);
				//待機状態へ移行
				m_state = State::eInterval;
			}
		}
		else {
			if (m_pos.y >= m_startPos.y) {
				GameData::m_isdownflg = true;
				//m_pos.y -= MOVE_SPEED;
				m_pos.y -= MOVE_SPEED * CFPS::GetDeltaTime();
				m_obb = COBB(
					m_pos,
					m_rot,
					m_boxSize
				);
				if (GameData::m_isdownflg && m_isMoveflg) {
					mp_player->m_pos.y -= MOVE_SPEED * CFPS::GetDeltaTime();
				}
			}
			else {
				GameData::m_isdownflg = false;
				m_obb = COBB(
					m_pos,
					m_rot,
					m_boxSize
				);
				//待機状態へ移行
				m_state = State::eInterval;
			}
		}
		break;
	case 1://x軸（左右）
		if (m_cnt % 2 == 0) {
			if (m_pos.x <= m_endPos.x) {
				GameData::m_isleftflg = true;
				//m_pos.x += MOVE_SPEED;
				m_pos.x += MOVE_SPEED * CFPS::GetDeltaTime();
				m_obb = COBB(
					m_pos,
					m_rot,
					m_boxSize
				);
				if (GameData::m_isleftflg && m_isMoveflg) {
					mp_player->m_pos.x += MOVE_SPEED * CFPS::GetDeltaTime();
				}
			}
			else {
				GameData::m_isleftflg = false;
				m_obb = COBB(
					m_pos,
					m_rot,
					m_boxSize
				);
				//待機状態へ移行
				m_state = State::eInterval;
			}
		}
		else {
			if (m_pos.x >= m_startPos.x) {
				GameData::m_isrightflg = true;
				//m_pos.x -= MOVE_SPEED;
				m_pos.x -= MOVE_SPEED * CFPS::GetDeltaTime();
				m_obb = COBB(
					m_pos,
					m_rot,
					m_boxSize
				);
				if (GameData::m_isrightflg && m_isMoveflg) {
					mp_player->m_pos.x -= MOVE_SPEED * CFPS::GetDeltaTime();
				}
			}
			else {
				GameData::m_isrightflg = false;
				m_obb = COBB(
					m_pos,
					m_rot,
					m_boxSize
				);
				//待機状態へ移行
				m_state = State::eInterval;
			}
		}
		break;
	case 2://z軸（前後）
		if (m_cnt % 2 == 0) {
			if (m_pos.z <= m_endPos.z) {
				GameData::m_isforwardflg = true;
				//m_pos.z += MOVE_SPEED;
				m_pos.z += MOVE_SPEED * CFPS::GetDeltaTime();
				m_obb = COBB(
					m_pos,
					m_rot,
					m_boxSize
				);
				if (GameData::m_isforwardflg && m_isMoveflg) {
					mp_player->m_pos.z += MOVE_SPEED * CFPS::GetDeltaTime();
				}
			}
			else {
				GameData::m_isforwardflg = false;
				m_obb = COBB(
					m_pos,
					m_rot,
					m_boxSize
				);
				//待機状態へ移行
				m_state = State::eInterval;
			}
		}
		else {
			if (m_pos.z >= m_startPos.z) {
				GameData::m_isbackflg = true;
				//m_pos.z -= MOVE_SPEED;
				m_pos.z -= MOVE_SPEED * CFPS::GetDeltaTime();
				m_obb = COBB(
					m_pos,
					m_rot,
					m_boxSize
				);
				if (GameData::m_isbackflg && m_isMoveflg) {
					mp_player->m_pos.z -= MOVE_SPEED * CFPS::GetDeltaTime();
				}
			}
			else {
				GameData::m_isbackflg = false;
				m_obb = COBB(
					m_pos,
					m_rot,
					m_boxSize
				);
				//待機状態へ移行
				m_state = State::eInterval;
			}
		}
		break;
	}
}

//待機状態の更新処理
void MoveBox::UpdateInterval()
{
	//待機時間経過待ち
	if (m_elapsedTime < m_intervalTime)
	{
		m_elapsedTime += CFPS::GetDeltaTime();
	}
	//待機終了
	else
	{
		m_elapsedTime -= m_intervalTime;
		m_cnt++;
		//移動状態へ移行
		m_state = State::eMove;
	}
}

//更新処理
void MoveBox::Update()
{
	//m_obb_old = m_obb;
	
	/*
	m_pos.y += 0.1f;
	m_obb = COBB(
		m_pos,
		m_rot,
		m_boxSize
	);
	*/
	//移動が有効でなければ処理しない
	if (!m_isEnable) return;

	//状態に合わせて更新処理を切り替え
	switch (m_state)
	{
		//移動状態
	case State::eMove:
		UpdateMove();
		break;
		//待機状態
	case State::eInterval:
		UpdateInterval();
		break;
	}
}

//描画処理
void MoveBox::Render()
{
	if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	switch (m_yxz)//switch (m_number)
	{
	case 0://y軸（上下）
		Utility::DrawOBB(m_obb, CVector4D(0, 1, 0, 1));
		break;
	case 1://x軸（左右）
		Utility::DrawOBB(m_obb, CVector4D(1, 0, 0, 1));
		break;
	case 2://z軸（前後）
		Utility::DrawOBB(m_obb, CVector4D(0, 0, 1, 1));
		break;
	}
}

void MoveBox::NoEnemyRender()
{
	switch (m_yxz)//switch (m_number)
	{
	case 0://y軸（上下）
		Utility::DrawOBB(m_obb, CVector4D(0, 1, 0, 1));
		break;
	case 1://x軸（左右）
		Utility::DrawOBB(m_obb, CVector4D(1, 0, 0, 1));
		break;
	case 2://z軸（前後）
		Utility::DrawOBB(m_obb, CVector4D(0, 0, 1, 1));
		break;
	}
}

void MoveBox::Collision(Task* b) {
	switch (b->GetType()) {
	case TaskType::ePlayer:
	{
		//■OBBとカプセル
		float dist;
		CVector3D axis;
		//if (CCollision::CollisionOBBCapsule(m_obb_old, mp_player->m_lineS, mp_player->m_lineE, mp_player->m_rad, &axis, &dist)) {
		//	if (axis.y > 0.5f) {
		//		//面が上向き→地面が当たった
		//		//重力落下速度を0に戻す
		//		if (mp_player->m_vec.y < 0) {
		//			mp_player->m_vec.y = 0;
		//			mp_player->m_isGround = true;
		//		}
		//	}
		//	//押し戻し
		//	float s = mp_player->m_rad - dist;
		//	mp_player->m_pos += axis * s;
		//}

		if (CCollision::CollisionOBBCapsule(m_obb, mp_player->m_lineS, mp_player->m_lineE, mp_player->m_rad, &axis, &dist)) {
			m_isMoveflg = true;
			if (axis.y > 0.5f) {
				//面が上向き→地面が当たった
				//重力落下速度を0に戻す
				if (mp_player->m_vec.y < 0) {
					mp_player->m_vec.y = 0;
					mp_player->m_isGround = true;
				}
			}
			//押し戻し
			float s = mp_player->m_rad - dist;
			mp_player->m_pos += axis * s;
		}
		else {
			m_isMoveflg = false;
		}
	}
	break;
	}
}