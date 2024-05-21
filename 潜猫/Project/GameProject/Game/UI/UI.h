#pragma once
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"

class Font : public Task {
private:
	CFont m_text;
public:
	const char* m_texts = {};
	int m_d;
	CVector2D m_iti;
	CVector3D m_color;
	Font(const char* text, const int& d, const CVector2D& pos, const CVector3D& color);
	~Font();
	void Update();
	void Draw();
};

class Fade : public Task {
private:
	Fade();
	~Fade();
	static Fade* Instance();
	static Fade* ms_instance;
public:
	bool m_isFadeOut;	//フェードアウトor
	bool m_isFade;		//フェード中
	float m_alpha;			//アルファ値
	static bool IsFade();
	static void FadeIn();
	static void FadeOut();
	void LateUpdate();
	void Draw() override;//オーバーライドを付けると元の名前が変わるとエラーが出る
};

class Timer : public Task {
private:
	CImage m_img;
	CImage m_img2;
public:
	Timer();
	~Timer();
	int m_ang;
	int m_ang2;
	int m_time;
	void Update() override;
	void Draw() override;
};

class UI :public Task {
private:
	CFont m_midasi;
	CFont m_text;
	
public:
	UI();
	~UI();
	void Update();
	void Draw();
};