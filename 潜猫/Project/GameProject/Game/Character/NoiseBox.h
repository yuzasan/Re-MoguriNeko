#pragma once
#include "CharaBase.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class Player;
class Enemy;

class NoiseBox : public CharaBase {
private:
	Player* mp_player;
	Enemy* mp_enemy;
	//箱のサイズ
	CVector3D m_boxSize;
	
public:
	NoiseBox(const CVector3D& pos, const CVector3D& rot, const CVector3D& size);
	~NoiseBox();
	void Update();
	void Render();
	/// <summary>
	/// enemy以外の描画処理
	/// </summary>
	void NoEnemyRender();
	void Collision(Task* b);
	bool m_isNoise;			//物音がなったかどうか
	bool m_isNoisemove;		//物音の方向に移動開始


};