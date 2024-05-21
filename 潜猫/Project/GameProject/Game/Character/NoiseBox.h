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
	//���̃T�C�Y
	CVector3D m_boxSize;
	
public:
	NoiseBox(const CVector3D& pos, const CVector3D& rot, const CVector3D& size);
	~NoiseBox();
	void Update();
	void Render();
	/// <summary>
	/// enemy�ȊO�̕`�揈��
	/// </summary>
	void NoEnemyRender();
	void Collision(Task* b);
	bool m_isNoise;			//�������Ȃ������ǂ���
	bool m_isNoisemove;		//�����̕����Ɉړ��J�n


};