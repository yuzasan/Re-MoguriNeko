//#include "Base/Base.h"
#include "Game/Character/Player.h"
#include "Game/Character/Enemy.h"
#include "Game/Character/EnemyManager.h"
#include "Game/Character/PlayerCamera.h"
#include "Game/Camera/FieldCamera.h"
#include "Game/GameData/GameData.h"
#include "Game/Stage/MoveBox.h"
#include "Game/Stage/RotateBox.h"
#include "Game/Stage/Box.h"
#include "Game/Stage/HideBox.h"
#include "Game/Stage/TyukanBox.h"
#include "Game/Stage/FellBox.h"
#include "Game/Stage/Item.h"
#include "Game/Stage/Stage.h"
#include "Debug/DebugPrint.h"
#include "Debug/DebugProfiler.h"
#include "Title/Title.h"
#include "Effect/Effect.h"
#include "Navigation/NavManeger.h"
#include "TaskSystem/TaskManeger.h"

//--------------------------------------------
//グローバル変数領域
//-------------------------------------------

//デバッグ表示するかどうか
bool g_isRenderDebug = false;

void Load() {
	//モデルの読み込み
	//プレイヤー
	ADD_RESOURCE("Ninja", CModel::CreateModel("Character/Cat/cat4.a3m"));
	//エネミー
	ADD_RESOURCE("Samurai", CModel::CreateModel("Character/Samurai/samurai3.a3m"));
	//ステージの読み込み
	//ADD_RESOURCE("Stage", CModel::CreateModel("Field/Castle/tamesi3.obj", 20, 10, 20));
	//ADD_RESOURCE("Stage", CModel::CreateModel("Field/Castle/tamesi6.obj", 20, 10, 20));
	ADD_RESOURCE("Stage", CModel::CreateModel("Field/Stage/ColorStage.obj", 20, 10, 20));
	//ステージColの読み込み
	//ADD_RESOURCE("Stagecol", CModel::CreateModel("Field/Castle/tamesicol3.obj", 20, 10, 20));
	//ADD_RESOURCE("Stagecol", CModel::CreateModel("Field/Castle/tamesiCol6.obj", 20, 10, 20));
	ADD_RESOURCE("Stagecol", CModel::CreateModel("Field/Stage/ColorStage.obj", 20, 10, 20));
	//吹き出し「なんだ猫か」
	ADD_RESOURCE("Fukidasi", CModel::CreateModel("Effect/Fukidasi/Fukidasi.obj"));
	//吹き出し「にゃ～」
	ADD_RESOURCE("Nya", CModel::CreateModel("Effect/Fukidasi/nya.obj"));
	//吹き出し「気のせいか」
	ADD_RESOURCE("Kinosei", CModel::CreateModel("Effect/Fukidasi/kinoseika.obj"));
	//猫の手画像
	ADD_RESOURCE("Nekonote", CImage::CreateImage("Effect/Cathand/blackcatbackhand45.png"));
	//猫の手の平画像
	ADD_RESOURCE("Tenohira", CModel::CreateModel("Effect/Cathand/Tenohira.obj"));

	//時計針画像
	ADD_RESOURCE("Needlethin", CImage::CreateImage("UI/Needlethin.png"));
	ADD_RESOURCE("Needle", CImage::CreateImage("UI/Needle.png"));

	//タイトル読み込み
	ADD_RESOURCE("Title", CImage::CreateImage("Title/Title.png"));
	ADD_RESOURCE("FusumaL", CModel::CreateModel("Effect/Load/FusumaL.obj"));
	ADD_RESOURCE("FusumaR", CModel::CreateModel("Effect/Load/FusumaR.obj"));

	//スカイボックス
	ADD_RESOURCE("Sky", CModel::CreateModel("Field/Sky/nightsky.obj"));

	new Title();
}

void MainLoop(void) {
	//--------------------------------------------------------------
	//ゲーム中の動きはここに書く
	//ゲーム中はこの関数_を1秒間に60回呼び出している
	//--------------------------------------------------------------
	/*
	Base::CheckKillALL();
	Base::UpdateALL();
	Base::CollisionALL();
	Base::RenderALL();
	Base::LateRenderALL();
	Base::LateUpdateALL();
	*/

	// Zキーでデバッグ表示をオンオフ切り替え
	if (PUSH(CInput::eButton1))
	{
		//g_isRenderDebug = !g_isRenderDebug;
	}
	
	//全Taskの更新
	DebugProfiler::StartTimer("Update");
	TaskManeger::Instance()->Update();
	DebugProfiler::EndTimer("Update");
	//全Taskの当たり判定
	DebugProfiler::StartTimer("Collision");
	TaskManeger::Instance()->Collision();
	DebugProfiler::EndTimer("Collision");
	//影の描画
	CShadow::GetInstance()->Render([]() {
		//全Taskの描画
		DebugProfiler::StartTimer("Render");
		TaskManeger::Instance()->Render();
		DebugProfiler::EndTimer("Render");
		//全Taskの更新後描画
		DebugProfiler::StartTimer("LateRender");
		TaskManeger::Instance()->LateRender();
		DebugProfiler::EndTimer("LateRender");
	});


	//全Taskの更新後更新
	DebugProfiler::StartTimer("LateUpdate");
	TaskManeger::Instance()->LateUpdate();
	DebugProfiler::EndTimer("LateUpdate");
	//全Taskの2Dを描画
	DebugProfiler::StartTimer("Draw");
	TaskManeger::Instance()->Draw();
	DebugProfiler::EndTimer("Draw");
	//描画後の2Dを描画
	DebugProfiler::StartTimer("LateDraw");
	TaskManeger::Instance()->LateDraw();
	DebugProfiler::EndTimer("LateDraw");


	//// デバッグ表示がオンであれば、
	//if (g_isRenderDebug)
	//{
	//	// 経路探索関連の描画
	//	DebugProfiler::StartTimer("NavMgrRender");
	//	NavManeger::Instance()->Render();
	//	DebugProfiler::EndTimer("NavMgrRender");
	//}

	/*
	//世界の軸を表示
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(100, 0, 0), CVector4D(1, 0, 0, 1));
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 100, 0), CVector4D(0, 1, 0, 1));
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 0, 100), CVector4D(0, 0, 1, 1));

	for (int i = -5; i <= 5; i++) {
		Utility::DrawLine(CVector3D(-5, 0, i * 1.0f), CVector3D(5, 0, i * 1.0f), CVector4D(0.2, 0.2, 0.2, 1));
		Utility::DrawLine(CVector3D(i * 1.0f, 0, -5), CVector3D(i * 1.0f, 0, 5), CVector4D(0.2, 0.2, 0.2, 1));
	}
	*/

	//fopen_s

	//ロード終了チェック
	if (CLoadThread::GetInstance()->CheckEnd()) {
		//完了している

	}
	else {
		CImage m_img = COPY_RESOURCE("Start", CImage);
		m_img.Draw();
		//ロード中
		//NowLoadingの文字をアニメーション
		static int t = 0;
		t++;
		char buf[128] = "Now Loading";
		int tt = (t / 30) % 4;
		for (int i = 0; i < tt; i++)
			strcat_s(buf, 128, ".");

		FONT_T()->Draw(1500, 1000, 1, 0, 0, buf);
	}
	// 計測結果の描画
	//DebugProfiler::Print();
}
void Init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);//ブレンドの有効化
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//固定シェーダー用
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_ALPHA_TEST);

	CFPS::SetFPS(60);
	//フレーム制御初期化
	CFPS::Init();
	//ボタンの設定
	CInput::Init();
	CInput::SetButton(0, CInput::eButton1, 'Z');
	CInput::SetButton(0, CInput::eButton2, 'X');
	CInput::SetButton(0, CInput::eButton3, 'C');
	CInput::SetButton(0, CInput::eButton4, 'V');
	CInput::SetButton(0, CInput::eButton5, VK_SPACE);
	CInput::SetButton(0, CInput::eButton10, VK_RETURN);
	CInput::SetButton(0, CInput::eButton11, VK_SHIFT);
	CInput::SetButton(0, CInput::eButton12, 'K');
	CInput::SetButton(0, CInput::eUp, 'W');
	CInput::SetButton(0, CInput::eDown, 'S');
	CInput::SetButton(0, CInput::eLeft, 'A');
	CInput::SetButton(0, CInput::eRight, 'D');
	CInput::SetButton(0, CInput::eMouseL, VK_LBUTTON);
	CInput::SetButton(0, CInput::eMouseR, VK_RBUTTON);
	CInput::SetButton(0, CInput::eMouseC, VK_MBUTTON);

	//マウス非表示
		CInput::ShowCursor(false);
		CInput::SetMouseInside(true);
	CInput::Update();
	CInput::Update();

	//ライト設定
	CLight::SetType(0, CLight::eLight_Direction);
	CLight::SetPos(0, CVector3D(0, 200, 200));
	CLight::SetDir(0, CVector3D(-1, -2, 1).GetNormalize());
	//CLight::SetColor(0, CVector3D(0.1f, 0.1f, 0.1f), CVector3D(0.6f, 0.6f, 0.6f));
	//CLight::SetColor(0, CVector3D(0.2f, 0.2f, 0.2f), CVector3D(0.8f, 0.8f, 0.8f));
	CLight::SetColor(0, CVector3D(0.9f, 0.9f, 0.9f), CVector3D(0.1f, 0.1f, 0.1f));

	//霧のようになる(フォグ)				,手前5,奥20
	CLight::SetFogParam(CVector4D(1, 1, 1, 1), 700, 800);

	//カメラ初期化
	/*CCamera::GetCamera()->LookAt(CVector3D(5, 5, 5),
		CVector3D(0, 0, 0),
		CVector3D(0.0, 1.0, 0.0));*/
	CCamera::GetCamera()->LookAt(CVector3D(0, 0, 10),
		CVector3D(0, 0, 0),
		CVector3D(0.0, 1.0, 0.0));

	SetCurrentDirectory("data");


	CShader::GetInstance("StaticMesh");
	CShader::GetInstance("SkinMesh");
	CSound::GetInstance();


	//-----------------------------------------------------
	//初期化の命令を書く
	//ゲーム起動時に一度だけ呼ばれる
	//-----------------------------------------------------

	/*
	//モデルの読み込み
	//プレイヤー
	ADD_RESOURCE("Ninja", CModel::CreateModel("Character/Ninja/Ninja.a3m"));
	//エネミー
	ADD_RESOURCE("Golem", CModel::CreateModel("Character/golem/golem.a3m"));

	//ステージの読み込み
	ADD_RESOURCE("Stage", CModel::CreateModel("Field/Test/testField6.obj", 1, 1, 1));
	*/

	////タイトル読み込み
	ADD_RESOURCE("Start", CImage::CreateImage("Title/Start.png"));
	//ADD_RESOURCE("FusumaL", CModel::CreateModel("Effect/Load/FusumaL.obj"));
	//ADD_RESOURCE("FusumaR", CModel::CreateModel("Effect/Load/FusumaR.obj"));
	//ADD_RESOURCE("fusumaL", CImage::CreateImage("Effect/Load/fusumaL.png"));
	//ADD_RESOURCE("fusumaR", CImage::CreateImage("Effect/Load/fusumaR.png"));
	//
	//new Title();

	CLoadThread::GetInstance()->LoadStart(Load);
	
	//影描画機能を生成
	CShadow::CreateInscance(20.0f, 20.0f, 2048, 2048);

	// 計測結果の描画
	DebugProfiler::Print();
}


void Release()
{
	TaskManeger::ClearInstance();
	CShader::ClearInstance();
	CInput::ClearInstance();
	CLoadThread::ClearInstance();
	CSound::ClearInstance();
	CResourceManager::ClearInstance();
}

static void ResizeCallback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);

	//画面解像度変動
	CCamera::GetCamera()->SetSize((float)w, (float)h);
	//画面解像度固定
	//CCamera::GetCamera()->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	CCamera::GetCamera()->Viewport(0, 0, w, h);
	CCamera::GetCurrent()->Perspective(DtoR(45.0), (float)w / (float)h, 1.0, 10000.0);
	
	glfwGetWindowPos(window, &GL::window_x, &GL::window_y);
	GL::UpdateWindowRect(GL::window_x, GL::window_y, w, h);
	CInput::UpdateClipCursor(true);

}
static void WheelCallback(GLFWwindow* _window, double _offsetx, double _offsety) {
	CInput::AddMouseWheel((int)_offsety);

}
static void PosCallback(GLFWwindow* _window, int x, int y) {
	GL::window_x = x;
	GL::window_y = y;
	GL::UpdateWindosRect(x, y, GL::window_width, GL::window_height);
	CInput::UpdateClipCursor(true);

}
static void FocusCallback(GLFWwindow* _window, int f) {
	CInput::UpdateClipCursor(f);
	GL::focus = f;
	CFPS::Wait();
}

static void error_callback(int error, const char* description)
{
	printf("Error: %s\n", description);
}

//フルスクリーン?ウインドウモードの切り替え
//Alt+Enterで切り替える
void CheckFullScreen() {
	static int key_enter = 0;
	int key_enter_buf = key_enter;
	if (key_enter_buf ^ (key_enter = glfwGetKey(GL::window, GLFW_KEY_ENTER)) && key_enter && (glfwGetKey(GL::window, GLFW_KEY_LEFT_ALT) || glfwGetKey(GL::window, GLFW_KEY_RIGHT_ALT))) {
		GL::ChangeFullScreen(!GL::full_screen);
	}
}

int __main(int* argcp, char** argv) {
	// メモリリーク検出
	//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	//OpenGL4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) return -1;
	//	glutInit(argcp, argv);

	GL::window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple", nullptr, nullptr);
	glfwGetWindowSize(GL::window, &GL::window_width, &GL::window_height);
	glfwGetWindowPos(GL::window, &GL::window_x, &GL::window_y);

	glfwSetFramebufferSizeCallback(GL::window, ResizeCallback);
	glfwSetScrollCallback(GL::window, WheelCallback);
	glfwSetWindowFocusCallback(GL::window, FocusCallback);
	glfwSetWindowPosCallback(GL::window, PosCallback);
	if (!GL::window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(GL::window);
	glfwSwapInterval(1);
	ResizeCallback(GL::window, SCREEN_WIDTH, SCREEN_HEIGHT);

	GLenum err = glewInit();
	if (err == GLEW_OK) {
		printf("%s\n", glewGetString(GLEW_VERSION));
	}
	else {
		printf("%s\n", glewGetErrorString(err));
		getchar();
		return -1;
	}
	HDC glDc = wglGetCurrentDC();
	GL::hWnd = WindowFromDC(glDc);

	Init();
	while (!glfwWindowShouldClose(GL::window)) {
		CheckFullScreen();


		CInput::Update();
		//各バッファーをクリア
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		MainLoop();


		glfwSwapBuffers(GL::window);


		CFPS::Wait();

		char title[32];
		sprintf_s(title, "math fps:%d dt:%.3f", CFPS::GetFPS(), CFPS::GetDeltaTime());
		glfwSetWindowTitle(GL::window, title);

		glfwPollEvents();
		if (glfwGetKey(GL::window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(GL::window, GL_TRUE);
		}

	}
	Release();
	glfwTerminate();//本当はこの後にgl形を持ってくるとエラーになる




	//Release();
	return 0;
}

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR argv, INT argc)
{
	return __main(&__argc, __argv);
}

int main(int argc, char** argv)
{
	return __main(&argc, argv);
}
