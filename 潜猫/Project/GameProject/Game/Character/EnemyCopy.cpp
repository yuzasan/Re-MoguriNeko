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


//�������x
#define WALK_SPEED 2.5f//5.0f//4.8f//2.4f
//�ǐՑ��x
//#define CHASE_SPEED 0.05f
//��]���x
#define ROTATE_SPEED 15.0f
//�ҋ@����
#define IDLE_TIME 2.0f
//�ړI�n�Ƃ̊ԋ���
#define SPACE 1.0f
//�d��
//#define G 0.8f
//�ő�d��
//#define MAX_G 0.001f

#define DBUG_SPEED 5.0f

//�ړ���
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
	//�G�̊Ǘ��N���X�̃��X�g�Ɏ��g��ǉ�
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

	//�G�̌o�H�T���p�̃m�[�h���쐬
	m_navNode = new NavNode
	(
		m_pos + CVector3D(0.0f, 1.0f, 0.0f),
		NavNode::NodeType::Destination
	);
	m_navNode->SetNodeColor(CVector3D(0.125f, 0.25f, 1.0f));
}

//�f�X�g���N�^
EnemyC::~EnemyC() {
	//�G�̊Ǘ��N���X�̃��X�g���玩�M����菜��
	EnemyManagerC::Instance()->Remove(this);
}

//�ҋ@��Ԃ̏���
void EnemyC::ActionIdle()
{
	switch (m_state) {
	case State::Sleep:
		//�X���[�v�A�j���[�V����
		m_model.ChangeAnimation((int)AnimId::Sleeping);
		break;
	case State::Sad:
		//�������݃A�j���[�V����
		m_model.ChangeAnimation((int)AnimId::SadIdle);
		break;
	case State::Cross:
		//�C���C���A�j���[�V����
		m_model.ChangeAnimation((int)AnimId::CrossIdle2);
		break;
	case State::Warning:
		//�x���A�j���[�V����
		m_model.ChangeAnimation((int)AnimId::Idle);
		break;
	case State::Normal:
		//�ҋ@�A�j���[�V����
		m_model.ChangeAnimation((int)AnimId::Idle);
		break;
	case State::Happy:
		//���ꂵ���A�j���[�V����
		m_model.ChangeAnimation((int)AnimId::HappyIdle2);
		break;
	}

	m_vec.x = 0.0f;
	m_vec.z = 0.0f;

	//�ҋ@���ԑ҂�
	if (m_elapsedTime < IDLE_TIME) {
		m_elapsedTime += CFPS::GetDeltaTime();
	}
	//�ҋ@���Ԃ��I���΁A
	else {
		m_elapsedTime = 0.0f;

		//���ɒT������m�[�h���擾
		SearchNodeC* node = EnemyManagerC::Instance()->GetNextSearchNode();
		//if (mp_noise != nullptr) {
		if (mp_noise->m_isNoise && !mp_noise->m_isNoisemove) {
			m_moveNode = NavManeger::Instance()->GetNearNavNode(mp_noise->m_pos);
			mp_noise->m_isNoisemove = true;
		}
		//}
		else if (node != nullptr) {
			//�T���m�[�h�Ɉ�ԋ߂��m�[�h��ړI�n�Ƃ���
			m_searchNode = node;
			m_searchNode->enemyc = this;
			m_moveNode = NavManeger::Instance()->GetNearNavNode(m_searchNode->pos);
		}
		//�T������m�[�h�����݂��Ȃ������ꍇ
		else {
			//�ړ��ʒu�������_���ŋ��߂�
			m_movePos.x = Utility::Rand(MOVE_RANGE_MIN, MOVE_RANGE_MAX);
			m_movePos.z = Utility::Rand(MOVE_RANGE_MIN, MOVE_RANGE_MAX);
			m_moveNode = NavManeger::Instance()->GetNearNavNode(m_movePos);
		}
		//�ړ���Ԃֈڍs
		//m_action = Action::Move;
		if (m_state != State::Warning) {
			m_action = Action::Move;
		}
		else {
			m_action = Action::Search;
		}
	}

	//�v���C���[����������A�����I�ɒǐՏ�Ԃ�
	//if (IsFoundPlayer() && m_emotion > 0.5f) {
	if (IsFoundPlayer() && m_state != State::Happy) {
		m_action = Action::Chase;
	}
}

//�ړ���Ԃ̏���
void EnemyC::ActionMove()
{
	switch (m_state) {
	case State::Sad:
		//�ҋ@�A�j���[�V����
		m_model.ChangeAnimation((int)AnimId::SadWalk);
		break;
	case State::Cross:
		//�ҋ@�A�j���[�V����
		m_model.ChangeAnimation((int)AnimId::CrossWalk2);
		break;
	case State::Warning:
		//�x���A�j���[�V����
		m_model.ChangeAnimation((int)AnimId::Walk);
		break;
	case State::Normal:
		//�ҋ@�A�j���[�V����
		m_model.ChangeAnimation((int)AnimId::Walk);
		break;
	case State::Happy:
		//�ҋ@�A�j���[�V����
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
			//���݈ʒu����ړI�n�̃��[�h�܂ł̌o�H�T�����s��
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

			//���Ɉړ����ׂ��m�[�h�����݂���΁A
			if (m_nextNode != nullptr) {
				//���Ɉړ����ׂ��m�[�h�֌����Ĉړ�
				if (MoveTo(m_nextNode->GetPos())) {
					//�ړ����I���΁A�ړI�n�̃m�[�h��
					//���Ɉړ����ׂ��m�[�h��null�ɂ���
					m_moveNode = nullptr;
					m_nextNode = nullptr;
					//�T���m�[�h�����݂��Ȃ��ꍇ�́A���̂܂ܑҋ@��Ԃ�
					if (m_searchNode == nullptr) {
						m_action = Action::Idle;
					}
				}
			}
		}
		else {
			//�T���m�[�h�܂ňړ�
			if (MoveTo(m_searchNode->pos)) {
				//�ړ����I���΁A
				m_searchNode->prob = 0.0f;
				m_searchNode->enemyc = nullptr;
				m_searchNode = nullptr;
				m_action = Action::Idle;
			}
		}
	}

	//�v���C���[����������A�����I�ɒǐՏ�Ԃ�
	//if (IsFoundPlayer()&&m_emotion > 0.5f) {
	if (IsFoundPlayer() && m_state != State::Happy) {
		m_action = Action::Chase;
	}
}

void EnemyC::ActionSearch() {
	//Vector3D dir;
	m_model.ChangeAnimation((int)AnimId::BackSearch, false);
	// �v���C���[�̌��������X�Ɉړ������֌�����
	//m_dir = CVector3D::Sleap(m_dir, -m_dir, (ROTATE_SPEED-14.9999f) * CFPS::GetDeltaTime());
	//��]�l��������x�N�g�����v�Z
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
		//�C���C���S�͎����A�j���[�V����
		m_model.ChangeAnimation((int)AnimId::Run);
		break;
	default:
		//�O�i�A�j���[�V����
		m_model.ChangeAnimation((int)AnimId::Walk);
		break;
	}

	//�e�m�[�h�̍��W���擾
	NavNode* playerNode = mp_player->GetNavNode();
	NavNode* enemyNode = m_navNode;
	CVector3D playerNodePos = playerNode->GetPos();
	CVector3D enemyNodePos = enemyNode->GetPos();
	//���݈ʒu����v���C���[�̈ʒu�܂ł̋��������߂�
	CVector3D vec = playerNodePos - enemyNodePos;
	float dist = vec.Length();

	NavManeger* navMgr = NavManeger::Instance();

	//�v���C���[�̈ʒu�܂Ŏ������ʂ��Ă��邩����
	CVector3D hitPos, hitNormal;
	/*enemyNodePos.y = 1.0f;
	playerNodePos.y = 1.0f;*/
	bool isHit = Stage::CollisionRay(enemyNodePos, playerNodePos, &hitPos, &hitNormal);
	//�v���C���[�̈ʒu�܂Ń��C���΂��āA�����ɏՓ˂�����A
	if (isHit) {
		//�v���C���[�܂ł̋��� > �Փ˒n�_�܂ł̋����Ȃ�΁A
		//���g�ƃv���C���[�܂ŊԂɎՕ��������݂���
		float hitDist = (hitPos - enemyNodePos).Length();
		isHit = dist > hitDist;
	}

	//�v���C���[�̈ʒu�܂ŎՕ��������݂��Ȃ���΁A
	if (!isHit) {
		//���݈ʒu����v���C���[�̈ʒu�܂ł̌o�H�T�����s��
		m_nextNode = navMgr->Navigate(enemyNode, playerNode);

		//�����������̍ŏI�ʒu�ɔz�u����m�[�h������
		if (m_lostNode == nullptr) {
			m_lostNode = new NavNode
			(
				playerNodePos,
				NavNode::NodeType::Destination
			);
			m_lostNode->SetNodeColor(CVector3D(1.0f, 0.5f, 0.25f));
		}
		//�v���C���[�̍ŏI�ʒu�p�̃m�[�h�����ɑ��݂���Ȃ�΁A
		//�v���C���[�̍ŏI�ʒu���X�V
		else {
			CVector3D range = (playerNodePos - enemyNodePos).GetNormalize();
			//m_lostNode->SetPos(playerNodePos - range * 0.5f);
			m_lostNode->SetPos(playerNodePos);
		}
		m_isFind = false;
	}
	//�v���C���[�Ƃ̊ԂɎՕ��������݂���
	else {
		//���g�ƃv���C���[�̃m�[�h�̐ڑ���؂�
		enemyNode->RemoveConnect(playerNode);

		//���݈ʒu����v���C���[�̍ŏI�ʒu�܂ł̌o�H�T�����s��
		m_nextNode = navMgr->Navigate(enemyNode, m_lostNode);

		//��������Ԃֈڍs
		m_action = Action::Lost;
		return;
	}

	//���Ɉړ����ׂ��m�[�h�����݂���ꍇ�́A
	//���̃m�[�h�̈ʒu�܂ňړ�
	if (m_nextNode != nullptr) {
		MoveTo(m_nextNode->GetPos());
	}
	//���Ɉړ����ׂ��m�[�h�����݂��Ȃ��ꍇ�́A
	//�ҋ@��Ԃֈڍs
	else {
		m_action = Action::Idle;
	}
}

//�v���C���[������������Ԃ̏���
void EnemyC::ActionLost() {
	GameData::m_islostflg = false;
	if (!mp_player->m_isHide) {
		//�ړI�n�����݂���
		if (m_nextNode != nullptr) {
			//���������ꍇ�́A����͈͂𖳎����āA
			//�v���C���[�܂ł̎������ʂ邩�ǂ����Ŕ��肷��
			if (IsLookPlayer()) {
				if (!mp_player->m_isHide && !GameData::m_islostflg) {
					//CVector3D pos = mp_player->m_pos - m_pos;
					//if (IsLookPlayer() && pos.Length() < 30.0f || !mp_player->m_isHide && !GameData::m_islostflg) {
						//�ǐՏ�Ԃֈڍs
					m_action = Action::Chase;
					m_isFind = false;
				}
			}
			//�v���C���[�������̒ʂ�Ȃ��Ƃ���ɂ���
			else {
				GameData::m_islostflg = true;
				//�ړI�n�܂ňړ�
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
		//�ړI�n�܂ňړ����I���΁A
		else {
			//new Effect("Fukidasi",CVector3D(0, 2.5, 0), CVector3D(0, 0, 0), CVector3D(0, 0, 0), CVector3D(0, 0, 0), 1.0f, 0.0f, 1.0f, 0.0f, 0, true, false, 60);
			//new Effect("Fukidasi", m_pos + CVector3D(0, 2.5, -1), CVector3D(0, 0, 0), CVector3D(0, 0, 0), CVector3D(0, 0, 0), 1.0f, 0.0f, 1.0f, 0.0f, 0, true, false, 60);
			new Effect("Kinosei", m_pos + CVector3D(0, 2.5, -1), CVector3D(0, 0, 0), CVector3D(0, 0, 0), CVector3D(0, 0, 0), 1.0f, 0.0f, 1.0f, 0.0f, 0, true, false, 60);
			//�ҋ@��Ԃֈڍs
			m_action = Action::Idle;
		}
	}
	else {
		//�ҋ@��Ԃֈڍs
		m_action = Action::Idle;
	}
}

int EnemyC::CollisionLine(const CVector3D& lineS, const CVector3D& lineE, const CVector3D& dir, CVector3D* cross) {
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

bool EnemyC::IsFoundPlayer() {
	CVector3D dir;
	if (m_state != State::Warning) {
		//��]�l��������x�N�g�����v�Z
		//CVector3D dir(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
		dir = CVector3D(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
	}
	else {
		dir = CVector3D(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
		m_dir = dir;
	}
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

//���݈ʒu����v���C���[�������邩�ǂ���
bool EnemyC::IsLookPlayer() const {

	NavNode* playerNode = mp_player->GetNavNode();
	NavNode* enemyNode = m_navNode;
	CVector3D playerNodePos = playerNode->GetPos();
	CVector3D enemyNodePos = enemyNode->GetPos();
	CVector3D vec = playerNodePos - enemyNodePos;
	//���݈ʒu����v���C���[�܂ł̋��������߂�
	float dist = vec.Length();

	//�v���C���[�̈ʒu�܂ł̃��C�ƕǂƂ̏Փ˔�����s��
	CVector3D start = enemyNodePos;
	CVector3D end = playerNodePos;
	/*start.y = 1.0f;
	end.y = 1.0f;*/
	CVector3D hitPos, hitNormal;
	if (Stage::CollisionRay(start, end, &hitPos, &hitNormal)) {
		float hitDist = (hitPos - start).Length();
		if (dist > hitDist)return false;
	}

	//�ǂƏՓ˂��Ă��Ȃ��������́A
	//�Փˈʒu���v���C���[��艜�̈ʒu�ł���Ȃ�΁A
	//���삪�ʂ��Ă���̂ŁA�v���C���[����������
	return true;

	/*
	CVector3D playerPos = mp_player->m_pos;
	CVector3D vec = playerPos - m_pos;
	//���݈ʒu����v���C���[�܂ł̋��������߂�
	float dist = vec.Length();

	//�v���C���[�̈ʒu�܂ł̃��C�ƕǂƂ̏Փ˔�����s��
	CVector3D start = m_pos;
	CVector3D end = playerPos;
	//start.y = 1.0f;
	//end.y = 1.0f;
	CVector3D hitPos, hitNormal;
	if (Stage::CollisionRay(start, end, &hitPos, &hitNormal)) {
		float hitDist = (hitPos - start).Length();
		if (dist > hitDist)return false;
	}

	//�ǂƏՓ˂��Ă��Ȃ��������́A
	//�Փˈʒu���v���C���[��艜�̈ʒu�ł���Ȃ�΁A
	//���삪�ʂ��Ă���̂ŁA�v���C���[����������
	return true;
	*/
}

//�w����W�ւ̈ړ�����
bool EnemyC::MoveTo(const CVector3D& target) {

	float moveSpeed = DBUG_SPEED * (m_emotion / 100);
	//float moveSpeed = WALK_SPEED * (m_emotion/100);

	CVector3D vec = target - m_pos;
	vec.y = 0.0f;
	m_moveDir = vec.GetNormalize();

	//�ړI�n�܂ł̋������ړ��������傫���Ȃ��
	float dist = vec.Length();
	float speed = moveSpeed * CFPS::GetDeltaTime();
	if (dist > moveSpeed * CFPS::GetDeltaTime()) {
		CVector3D moveVec = m_moveDir * moveSpeed;
		m_vec = CVector3D(moveVec.x, m_vec.y, moveVec.z);
	}
	//�������ړ����x���Z���̂ł���΁A
	else {
		CVector3D moveVec = m_moveDir * dist;
		m_vec = CVector3D(moveVec.x, m_vec.y, moveVec.z);
		//�ړ��I��
		return true;
	}

	//�ړ��p��
	return false;
}

void EnemyC::Shot() {
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
		//if (s->GetModel()->CollisionRay(&hit_field_point, &n, lineS, lineE)) {
		if (s->GetColNavModel()->CollisionRay(&hit_field_point, &n, lineS, lineE)) {
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

//�T���m�[�h��ݒ�
void EnemyC::SetSearchNode(SearchNodeC* node)
{
	m_searchNode = node;
}

//�T�����̃m�[�h���擾
SearchNodeC* EnemyC::GetSearchNode() const
{
	return m_searchNode;
}


void EnemyC::Update()
{
	//����BOX�擾
	if (!mp_noise)
	{
		mp_noise = dynamic_cast<NoiseBox*>(TaskManeger::FindObject(TaskType::eNoiseBox));
	}

	// ����͈͂̃J���[(�����F�͗�)
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

	// ���݂̏�Ԃɍ��킹�āA������؂�ւ���
	switch (m_action)
	{
		// �ҋ@�s��
	case Action::Idle:
		ActionIdle();
		break;
		// �ړ��s��
	case Action::Move:
		ActionMove();
		break;
		// �ǐՍs��
	case Action::Chase:
		ActionChase();
		//color = CVector4D(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case Action::Search:
		ActionSearch();
		break;
		//�v���C���[����������
	case Action::Lost:
		ActionLost();
		color = CVector4D(1.0f, 1.0f, 0.0f, 0.75f);
		break;
	}

	//�A�j���[�V�����X�V
	m_model.UpdateAnimation();

	if (m_action != Action::Search) {
		// �v���C���[�̌��������X�Ɉړ������֌�����
		m_dir = CVector3D::Sleap(m_dir, m_moveDir, ROTATE_SPEED * CFPS::GetDeltaTime());
	}
	// �v���C���[�̌������f
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

	//�J�v�Z���X�V
	float height = 2.0f;
	m_lineS = m_pos + CVector3D(0, height - m_rad, 0);
	m_lineE = m_pos + CVector3D(0, m_rad, 0);

	// �x�[�X�N���X�̍X�V
	CharaBase::Update();

	//�f�o�b�O�\��
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
	//��]�l��������x�N�g�����v�Z
	CVector3D dir(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
	// ����͈̔͂�\��
	auto lineWidth = 5.0f;
	/*Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + dir * view_length, CVector4D(1, 0, 1, 1), lineWidth);
	Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + CMatrix::MRotationY(view_ang) * dir * view_length, CVector4D(0, 1, 1, 1), lineWidth);
	Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + CMatrix::MRotationY(-view_ang) * dir * view_length, CVector4D(0, 1, 1, 1), lineWidth);*/
	//��̌�����
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
		//�G�J�v�Z���̕\��
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
		//�G���m�̔���
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
		//��]�l��������x�N�g�����v�Z
		CVector3D dir(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
		// ����͈̔͂�\��
		auto lineWidth = 5.0f;
		/*Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + dir * view_length, CVector4D(1, 0, 1, 1), lineWidth);
		Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + CMatrix::MRotationY(view_ang) * dir * view_length, CVector4D(0, 1, 1, 1), lineWidth);
		Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + CMatrix::MRotationY(-view_ang) * dir * view_length, CVector4D(0, 1, 1, 1), lineWidth);*/
		//��̌�����
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
			//�G�J�v�Z���̕\��
			//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(0, 0, 1, 1));
			Utility::DrawCube(m_pos + CVector3D(0, 2, 0), CVector3D(1, 0, 1), CVector4D(0, 0, 1, 1));
		}
	}
}

void EnemyC::Debug() {
	DebugPrint::Print("Enemy���W : X:%f , Y:%f , Z:%f", m_pos.x, m_pos.y, m_pos.z);
	std::string action = "";
	switch (m_action)
	{
	case EnemyC::Action::Idle: action = "�ҋ@"; break;
	case EnemyC::Action::Move: action = "�ړ�"; break;
	case EnemyC::Action::Chase:action = "�ǐ�"; break;
	case EnemyC::Action::Lost: action = "��������"; break;
	}
	DebugPrint::Print("State:%s", action.c_str());
}
