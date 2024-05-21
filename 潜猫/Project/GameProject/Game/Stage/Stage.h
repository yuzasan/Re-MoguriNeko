#pragma once
//#include "../Base/Base.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class StageWatch;

class Stage : public Task {
private:
	//�t�B�[���h�N���X�̃C���X�^���X
	static Stage* ms_instance;
	//�o�H�T���p�m�[�h�̃e�[�u��
	static std::list<CVector3D> ms_nodes;

	//�X�e�[�W���f��
	//CModelObj m_model;
	CModel* m_model;
	CModel* m_Navmodel;
	StageWatch* m_Watchmodel;
	//�X�J�C�{�b�N�X���f��
	CModelObj skybox;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Stage();

	//�f�X�g���N�^
	~Stage();

	//�o�H�T���p�̃m�[�h���쐬
	void CreateNavNodes();

	//�w�肵�����C��Stage�̃R���W�����Ƃ̃q�b�g����
	static bool CollisionRay(const CVector3D& start, const CVector3D& end, CVector3D* outHitPos, CVector3D* outHitNormal);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈��
	/// </summary>
	void Render();
	/// <summary>
	/// enemy�ȊO�̕`�揈��
	/// </summary>
	void NoEnemyRender();
	/// <summary>
	/// �I�u�W�F�N�g�������f���̎擾
	/// </summary>
	/// <returns></returns>
	/*CModel* GetModel() {
		return &m_model;
	}*/
	CModel* GetModel();
	CModel* GetColNavModel() const;
};