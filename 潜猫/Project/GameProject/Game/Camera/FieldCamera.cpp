#include "FieldCamera.h"
#include "../Character/Player.h"
#include "../Character/Enemy.h"
#include "../Stage/Stage.h"
//static �͍ŏ���#include�̂Ƃ���ɏ���
bool FieldCamera::rendring = false;

FieldCamera::FieldCamera() :Task(TaskType::eFieldCamera) {
	//�������_�[�^�[�Q�b�g����
	//texture_frame_mini_map = new CTextureFrame(512, 512, CVector4D(1, 1, 1, 1));
	texture_frame_mini_map = new CTextureFrame(1028, 1028, CVector4D(1, 1, 1, 1));
	mp_player = TaskManeger::FindObject(TaskType::ePlayer);
}

FieldCamera::~FieldCamera()
{
}

void FieldCamera::Update(){
	/*
	{
		rendring = true;
		//--���ʎn�_����̃Q�[����ʕ`��---------
		//���݂̃J�������R�s�[
		CCamera back = *CCamera::GetCurrent();

		//�L�����N�^�[��ʂ���̃J����	���Ɍ����Ă���
		//CCamera::GetCurrent()->SetTranseRot(mp_player->m_pos + CVector3D(0, 10, 0), CVector3D(DtoR(90), 0, 0));
		CCamera::GetCurrent()->SetTranseRot(mp_player->m_pos + CVector3D(0, 15, 0), CVector3D(DtoR(90), 0, 0));

		//�`���̉�ʃT�C�Y����512:�c512�̂��߃A�X�y�N�g���1.0f
		CCamera::GetCurrent()->Perspective(DtoR(60), 1.0f, 1.0f, 1000.0f);

		//�e������
		CShadow::GetInstance()->GetRenderTarget()->ClearBuffer();

		texture_frame_mini_map->BeginDraw();

		//Base::RenderALL();
		//Base::NoEnemyRenderALL();
		//Base::LateRenderALL();

		TaskManeger::Instance()->NoEnemyRender();
		TaskManeger::Instance()->LateRender();
		texture_frame_mini_map->EndDraw();
		//�J���������̏�Ԃɖ߂�
		*CCamera::GetCurrent() = back;
		rendring = false;
	}
	*/
}

void FieldCamera::Draw(){
	//if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	{
		rendring = true;
		//--���ʎn�_����̃Q�[����ʕ`��---------
		//���݂̃J�������R�s�[
		CCamera back = *CCamera::GetCurrent();

		//�L�����N�^�[��ʂ���̃J����	���Ɍ����Ă���
		CCamera::GetCurrent()->SetTranseRot(mp_player->m_pos + CVector3D(-2.5, 15, 0), CVector3D(DtoR(90), 0, 0));

		//�`���̉�ʃT�C�Y����512:�c512�̂��߃A�X�y�N�g���1.0f
		//CCamera::GetCurrent()->Perspective(DtoR(60), 1.0f, 1.0f, 1000.0f);
		CRect rect = { -5,-5,5,5 };
		//���˓��e�s��Őݒ�
		CCamera::GetCurrent()->Ortho(rect.m_left, rect.m_right, rect.m_top, rect.m_bottom, 1.0f, 1000.0f);

		//�e������
		CShadow::GetInstance()->GetRenderTarget()->ClearBuffer();

		texture_frame_mini_map->BeginDraw();

		TaskManeger::Instance()->NoEnemyRender();
		TaskManeger::Instance()->LateRender();
		texture_frame_mini_map->EndDraw();
		//�J���������̏�Ԃɖ߂�
		*CCamera::GetCurrent() = back;
		rendring = false;

		Utility::DrawQuad(CVector2D(0, 0), CVector2D(258, 258), CVector4D(0.0f, 0.0f, 0.0f, 1.0f));
		//�����_�[�^�[�Q�b�g�̃e�N�X�`���\���ȈՕ\��
		CTextureFrame::Draw(0, 0, 256, 256, texture_frame_mini_map->GetTexture());
	}
}

void FieldCamera::LateUpdate(){
	//Utility::DrawQuad(CVector2D(0,0), CVector2D(258, 258), CVector4D(0.0f, 0.0f, 0.0f, 1.0f));
	////�����_�[�^�[�Q�b�g�̃e�N�X�`���\���ȈՕ\��
	//CTextureFrame::Draw(0, 0, 256, 256, texture_frame_mini_map->GetTexture());
}