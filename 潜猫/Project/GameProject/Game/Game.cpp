#include "Game.h"
#include "../Base/Base.h"
#include "Character/Player.h"
#include "Character/Enemy.h"
#include "Character/EnemyManager.h"
#include "Character/EnemyCopy.h"
#include "Character/EnemyManagerCopy.h"
#include "Character/NoiseBox.h"
#include "Character/PlayerCamera.h"
#include "Camera/FieldCamera.h"
#include "Camera/EventCamera.h"
#include "GameData/GameData.h"
#include "Stage/MoveBox.h"
#include "Stage/RotateBox.h"
#include "Stage/Box.h"
#include "Stage/HideBox.h"
#include "Stage/TyukanBox.h"
#include "Stage/FellBox.h"
#include "Stage/Item.h"
#include "Stage/Stage.h"
#include "Stage/BreakBox.h"
#include "../Debug/DebugPrint.h"
#include "../Title/Title.h"
#include "../TaskSystem/TaskManeger.h"
#include "../Effect/Effect.h"
#include "UI/UI.h"

Game::Game():Task(TaskType::eScene){
	//new Player(CVector3D(11, 5.0f, 25));
	new Player(CVector3D(0, 1.5f, 0));

	new EnemyC(CVector3D(-19.0f, 17.0f, 17.0f), 75.0f);
	new EnemyC(CVector3D(-19.0f, 17.0f, 17.0f), 110.0f);

	new Enemy(CVector3D(-8.0f, 6.0f, 25.0f), 75.0f);
	new Enemy(CVector3D(-8.0f, 6.0f, 25.0f), 110.0f);
	new Enemy(CVector3D(-8.0f, 6.0f, 25.0f), 100.0f);
	new Enemy(CVector3D(-8.0f, 6.0f, 25.0f), 25.0f);

	new Stage();
	new PlayerCamera();
	new FieldCamera();
	//new Timer();
	new UI();
	//new EventCamera();
	//�t�B�[���h�ɔz�u���鏰�𐶐�
	float ratio = 1.0f;
	float moveTime = 5.0f * ratio;//float moveTime = 5.0f * ratio;
	float intervalTime = 1.0f * ratio;//float intervalTime = 1.0f * ratio;

	/*new MoveBox
	(
		CVector3D(-4.0f, 1.0f, 19.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(5.0f, 1.0f, 4.0f),
		CVector3D(0.0f, 1.0f, 0.0f),
		CVector3D(0.0f, 10.0f, 0.0f),
		moveTime, intervalTime, 0, 0, true
	);

	new MoveBox
	(
		CVector3D(-30.0f, 1.0f, 20.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(5.0f, 1.0f, 4.0f),
		CVector3D(0.0f, 1.0f, 0.0f),
		CVector3D(0.0f, 10.0f, 0.0f),
		moveTime, intervalTime, 0, 0, true
	);

	new MoveBox
	(
		CVector3D(-11.0f, 13.0f, 10.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(2.0f, 2.0f, 1.0f),
		CVector3D(-11.0f, 0.0f, 0.0f),
		CVector3D(-9.0f, 0.0f, 0.0f),
		moveTime, intervalTime, 1, 1, true
	);

	new MoveBox
	(
		CVector3D(-11.0f, 13.0f, 6.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(2.0f, 2.0f, 1.0f),
		CVector3D(-11.0f, 0.0f, 0.0f),
		CVector3D(-9.0f, 0.0f, 0.0f),
		moveTime, intervalTime, 1, 0, true
	);

	new MoveBox
	(
		CVector3D(-11.0f, 13.0f, 2.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(2.0f, 2.0f, 1.0f),
		CVector3D(-11.0f, 0.0f, 0.0f),
		CVector3D(-9.0f, 0.0f, 0.0f),
		moveTime, intervalTime, 1, 1, true

	);

	new MoveBox
	(
		CVector3D(-11.0f, 13.0f, -2.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(2.0f, 2.0f, 1.0f),
		CVector3D(-11.0f, 0.0f, 0.0f),
		CVector3D(-9.0f, 0.0f, 0.0f),
		moveTime, intervalTime, 1, 0, true
	);

	new MoveBox
	(
		CVector3D(-11.0f, 13.0f, -6.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(2.0f, 2.0f, 1.0f),
		CVector3D(-11.0f, 0.0f, 0.0f),
		CVector3D(-9.0f, 0.0f, 0.0f),
		moveTime, intervalTime, 1, 1, true
	);*/

	/*new RotateBox
	(
		CVector3D(5.0f, 10.0f, -10.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(3.0f, 1.0f, 3.0f),
		DtoR(-45.0f), 2, intervalTime, 0, true
	);

	new RotateBox
	(
		CVector3D(15.0f, 15.0f, -10.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(3.0f, 1.0f, 3.0f),
		DtoR(60.0f), 2, intervalTime, 0, true
	);

	new Box
	(
		CVector3D(26.0f, 20.0f, -10.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(3.0f, 1.0f, 3.0f)
	);

	new MoveBox
	(
		CVector3D(26.0f, 1.0f, -2.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(3.0f, 1.0f, 3.0f),
		CVector3D(26.0f, 1.0f, 0.0f),
		CVector3D(33.0f, 1.0f, 0.0f),
		moveTime, intervalTime, 1, 0, true
	);

	new TyukanBox
	(
		CVector3D(5.0f, 1.0f, 0.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(1.0f, 2.0f, 1.0f)
	);
	*/
	/*new HideBox
	(
		CVector3D(-8.0f, 5.0f, 23.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(1.0f, 2.0f, 1.0f)
	);*/

	new HideBox
	(
		CVector3D(-11.0f, 17.0f, 0.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(1.0f, 2.0f, 1.0f)
	);

	new TyukanBox
	(
		CVector3D(-2.0f, 5.0f, 24.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(1.0f, 0.15f, 3.4f)
	);

	new TyukanBox
	(
		CVector3D(-13.5f, 16.8f, -3.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(1.0f, 0.15f, 3.4f)
	);

	//new FellBox
	//(
	//	CVector3D(0.0f, 1.0f, 5.0f),	//pos
	//	CVector3D(0.0f, 0.0f, 0.0f),	//rot
	//	CVector3D(1.0f, 2.0f, 1.0f)		//size
	//);

	new FellBox
	(
		CVector3D(0.0f, -30.0f, 0.0f),	//pos
		CVector3D(0.0f, 0.0f, 0.0f),	//rot
		CVector3D(130.0f, 2.0f, 130.0f)		//size
	);

	new FellBox
	(
		CVector3D(17, 0.0f, 25),	//pos
		CVector3D(0.0f, 0.0f, 0.0f),	//rot
		CVector3D(1.0f, 2.0f, 1.0f)		//size
	);

	//new BreakBox
	//(
	//	CVector3D(-43.0f, 0.0f, 0.0f),	//pos
	//	CVector3D(0.0f, 0.0f, 0.0f),	//rot
	//	CVector3D(3.0f, 3.0f, 3.0f),	//size
	//	4
	//);
	//new BreakBox
	//(
	//	CVector3D(-43.0f, 0.0f, 43.0f),	//pos
	//	CVector3D(0.0f, 0.0f, 0.0f),	//rot
	//	CVector3D(3.0f, 3.0f, 3.0f),	//size
	//	4
	//);
	//new BreakBox
	//(
	//	CVector3D(0.0f, 0.0f, 43.0f),	//pos
	//	CVector3D(0.0f, 0.0f, 0.0f),	//rot
	//	CVector3D(3.0f, 3.0f, 3.0f),	//size
	//	1
	//);
	//new BreakBox
	//(
	//	CVector3D(43.0f, 0.0f, 43.0f),	//pos
	//	CVector3D(0.0f, 0.0f, 0.0f),	//rot
	//	CVector3D(3.0f, 3.0f, 3.0f),	//size
	//	4
	//);
	//new BreakBox
	//(
	//	CVector3D(43.0f, 0.0f, 0.0f),	//pos
	//	CVector3D(0.0f, 0.0f, 0.0f),	//rot
	//	CVector3D(3.0f, 3.0f, 3.0f),	//size
	//	2
	//);
	//new BreakBox
	//(
	//	CVector3D(43.0f, 0.0f, -43.0f),	//pos
	//	CVector3D(0.0f, 0.0f, 0.0f),	//rot
	//	CVector3D(3.0f, 3.0f, 3.0f),	//size
	//	4
	//);
	//new BreakBox
	//(
	//	CVector3D(0.0f, 0.0f, -43.0f),	//pos
	//	CVector3D(0.0f, 0.0f, 0.0f),	//rot
	//	CVector3D(3.0f, 3.0f, 3.0f),	//size
	//	3
	//);
	//new BreakBox
	//(
	//	CVector3D(-43.0f, 0.0f, -43.0f),	//pos
	//	CVector3D(0.0f, 0.0f, 0.0f),	//rot
	//	CVector3D(3.0f, 3.0f, 3.0f),	//size
	//	4
	//);

	/*
	new BreakBox
	(
		CVector3D(6, 1, 30),	//pos
		CVector3D(0.0f, 0.0f, 0.0f),	//rot
		CVector3D(5.0f, 3.0f, 5.0f),	//size
		1
	);

	new BreakBox
	(
		CVector3D(6, 1, 40),	//pos
		CVector3D(0.0f, 0.0f, 0.0f),	//rot
		CVector3D(5.0f, 3.0f, 5.0f),	//size
		2
	);
	*/

	/*new Item
	(
		CVector3D(6, 1, 20),
		CVector3D(0, 0, 0),
		CVector3D(0.1f, 0.1f, 0.1f)
	);*/

	new Item
	(
		CVector3D(13, 1, -23),
		CVector3D(0, 0, 0),
		CVector3D(0.1f, 0.1f, 0.1f)
	);

	new Item
	(
		CVector3D(-69, 1, 23),
		CVector3D(0, 0, 0),
		CVector3D(0.1f, 0.1f, 0.1f)
	);

	new Item
	(
		CVector3D(-33, 5, 58),
		CVector3D(0, 0, 0),
		CVector3D(0.1f, 0.1f, 0.1f)
	);

	new Item
	(
		CVector3D(-14, 5, -2),
		CVector3D(0, 0, 0),
		CVector3D(0.1f, 0.1f, 0.1f)
	);

	new Item
	(
		CVector3D(3, 1, 30),
		CVector3D(0, 0, 0),
		CVector3D(0.1f, 0.1f, 0.1f)
	);

	/*new Item2
	(
		CVector3D(6, 1.0f, 25),
		CVector3D(0, 0, 0),
		CVector3D(0.1f, 0.1f, 0.1f)
	);
	*/
	new Item2
	(
		CVector3D(-8, 17, 50),
		CVector3D(0, 0, 0),
		CVector3D(0.1f, 0.1f, 0.1f)
	);

	new Item2
	(
		CVector3D(-40, 18, 42),
		CVector3D(0, 0, 0),
		CVector3D(0.1f, 0.1f, 0.1f)
	);

	new Item2
	(
		CVector3D(-9, 17, 4),
		CVector3D(0, 0, 0),
		CVector3D(0.1f, 0.1f, 0.1f)
	);

	new Item2
	(
		CVector3D(-13, 18, 4),
		CVector3D(0, 0, 0),
		CVector3D(0.1f, 0.1f, 0.1f)
	);

	new Item2
	(
		CVector3D(-27, 1, 64),
		CVector3D(0, 0, 0),
		CVector3D(0.1f, 0.1f, 0.1f)
	);

	new Item2
	(
		CVector3D(-19, 17, -4),
		CVector3D(0, 0, 0),
		CVector3D(0.1f, 0.1f, 0.1f)
	);

	new Item2
	(
		CVector3D(-42, 17, 29),
		CVector3D(0, 0, 0),
		CVector3D(0.1f, 0.1f, 0.1f)
	);

	new Item2
	(
		CVector3D(-59, 18, 29),
		CVector3D(0, 0, 0),
		CVector3D(0.1f, 0.1f, 0.1f)
	);

	new Item2
	(
		CVector3D(-28, 17, 8),
		CVector3D(0, 0, 0),
		CVector3D(0.1f, 0.1f, 0.1f)
	);

	new BreakBox
	(
		CVector3D(-8.0f, 6, 15.0f),	//pos
		CVector3D(0.0f, 0.0f, 0.0f),	//rot
		CVector3D(2.0f, 4.0f, 2.0f),	//size
		2
	);

	new BreakBox
	(
		CVector3D(-27.0f, 18.9f, 15.0f),	//pos
		CVector3D(0.0f, 0.0f, 0.0f),	//rot
		CVector3D(5.0f, 3.0f, 5.0f),	//size
		5
	);

	new LastItem
	(
		CVector3D(-27, 7, 15),
		CVector3D(0, 0, 0),
		CVector3D(1.0f, 1.0f, 1.0f)
	);

	//��Ȃ���v
	 new NoiseBox
	(
		CVector3D(-2.0f, 1000.0f, 22.0f),	//pos
		CVector3D(0.0f, 0.0f, 0.0f),	//rot
		CVector3D(1.0f, 2.0f, 1.0f)		//size
	);
	//new NoiseBox
	//(
	//	CVector3D(-2.0f, 5.0f, 22.0f),	//pos
	//	CVector3D(0.0f, 0.0f, 0.0f),	//rot
	//	CVector3D(1.0f, 2.0f, 1.0f)		//size
	//);

	//new Effect2D("Nekonote");
	CFPS::Wait();
}

Game::~Game()
{
	//�S�ẴI�u�W�F�N�g��j��
	//Base::KillALL();
	TaskManeger::KillALL();
	//�^�C�g���V�[����
	//Base::Add(new Title());
	GameData::m_item = 0;
	GameData::m_item2 = 0;
	GameData::m_lastitem = 0;
	GameData::m_diecnt = 0;
	GameData::m_step = 0;
	GameData::m_breakflg = 0;
	GameData::m_isdownflg = false;
	//�^�C�g���V�[����
	//new Title();
	new Clear();
}

void Game::Update()
{
	//���E�̎���\��
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(100, 0, 0), CVector4D(1, 0, 0, 1));
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 100, 0), CVector4D(0, 1, 0, 1));
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 0, 100), CVector4D(0, 0, 1, 1));

	for (int i = -5; i <= 5; i++) {
		Utility::DrawLine(CVector3D(-5, 0, i * 1.0f), CVector3D(5, 0, i * 1.0f), CVector4D(0.2, 0.2, 0.2, 1));
		Utility::DrawLine(CVector3D(i * 1.0f, 0, -5), CVector3D(i * 1.0f, 0, 5), CVector4D(0.2, 0.2, 0.2, 1));
	}
	if (GameData::m_lastitem >= 1) {
		//SetKill();
		Kill();
	}
}

Clear::Clear() :Task(TaskType::eScene), m_title_text("C:\\Windows\\Fonts\\msgothic.ttc", 64)
{
	m_img = COPY_RESOURCE("Title", CImage);
}

Clear::~Clear()
{
	//�^�C�g���V�[����
	new Title();
}

void Clear::Update()
{
	if (PUSH(CInput::eButton5)) {
		Kill();
	}
}

void Clear::Draw()
{
	m_title_text.Draw(700, 700, 0, 0, 0, "�Q�[���N���A");
	m_title_text.Draw(700, 900, 0, 0, 0, "Space�L�[�Ń^�C�g���ɖ߂�");
}


//BlackOut::BlackOut() :Task(TaskType::eUI)
//	, m_gauge(0)
//	,m_num(0)
//	,m_alpha(0.0f)
//{
//	m_time = 0;
//}
//
//BlackOut::~BlackOut(){
//
//}
//
//void BlackOut::LateUpdate() {
//	m_time += 1.5f;
//	if (m_time <= 90) {
//		m_alpha = sinf(DtoR(m_time));
//	}
//	else {
//		Kill();
//	}
//	CVector4D color = CVector4D(0.0f, 0.0f, 0.0f, m_alpha);
//	//CVector4D color = CVector4D(0.0f, 0.0f, 0.0f, 1.0f);
//	//switch (m_num) {
//	//case 0:
//	//	if (m_gauge <= 1080) {
//	//		m_time++;
//	//		m_gauge = 20.0f * (float)m_time;//m_gauge = (257.0f / 3.0f * (float)m_time);
//	//	}
//	//	else {
//	//		m_num = 1;
//	//	}
//	//	break;
//	//case 1:
//	//	if (m_gauge >= 0) {
//	//		m_time--;
//	//		m_gauge = 20.0f * (float)m_time;//m_gauge = (257.0f / 3.0f * (float)m_time);
//	//	}
//	//	else {
//	//		m_num = 2;
//	//	}
//	//	break;
//	//case 2:
//	//	Kill();
//	//}
//	//Utility::DrawQuad(CVector2D(0, 0), CVector2D(1920, m_gauge), color);
//
//	Utility::DrawQuad(CVector2D(0, 0), CVector2D(1920, 1080), color);
//}
