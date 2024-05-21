#pragma once
#include "../TaskSystem/Task.h"
#include "../TaskSystem/TaskManeger.h"
class Player;
class Effect :public Task {
private:
	Player* mp_player;
	CModelObj	m_model;		//モデル
	CVector3D	m_vec_accel;	//移動量増加
	CVector3D	m_rot_vec;		//回転量
	int			m_blend;		//ブレンド種類
	bool		m_builbord;		//ビルボード描画設定
	float		m_scale;		//スケール
	float		m_scale_speed;	//スケール変化量
	float		m_alpha;		//アルファ値
	float		m_alpha_speed;	//アルファ値変化量
	bool		m_depth;		//深度バッファ設定
	int m_time;
public:
	enum {
		eBlendAlpha,	//通常ブレンド
		eBlendAdd,		//加算ブレンド
	};
	/*!
		@brief	3Dエフェクト
		@param	model			[in] モデル
		@param	pos				[in] 座標
		@param	vec				[in] 移動量
		@param	rot				[in] 回転量
		@param	accel			[in] 加速度
		@param	scale			[in] スケール
		@param	change_scale	[in] スケール変化量
		@param	alpha			[in] アルファ値
		@param	change_alpha	[in] アルファ値変化量
		@param	blend			[in] ブレンド種類
		@param	builbord		[in] ビルボード描画設定
		@param	depth			[in] 深度バッファ設定
		@param	time			[in] 時間
		
	**/
	Effect(const char* model, const CVector3D& pos, const CVector3D& vec, const CVector3D& rot, const CVector3D& accel, float scale, float change_scale, float alpha, float change_alpha, int blend, bool builbord, bool depth, int time = -1);
	~Effect();
	void Update();
	void Render();
};

class Effect2D :public Task {
private:
	CImage m_img;
	Player* mp_player;
public:
	Effect2D(const char* img);
	~Effect2D();
	void Update();
	void Draw();
};