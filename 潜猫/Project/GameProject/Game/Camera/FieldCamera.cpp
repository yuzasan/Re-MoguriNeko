#include "FieldCamera.h"
#include "../Character/Player.h"
#include "../Character/Enemy.h"
#include "../Stage/Stage.h"
//static �͍ŏ���#include�̂Ƃ���ɏ���
bool FieldCamera::rendring = false;

FieldCamera::FieldCamera() :Task(TaskType::eFieldCamera) {
	//�������_�[�^�[�Q�b�g����
	texture_frame_mini_map = new CTextureFrame(512, 512, CVector4D(1, 1, 1, 1));
	mp_player = TaskManeger::FindObject(TaskType::ePlayer);
}

FieldCamera::~FieldCamera()
{
}

void FieldCamera::Update() {

}

void FieldCamera::Draw(){
	{
		rendring = true;
		//--���ʎn�_����̃Q�[����ʕ`��---------
		//���݂̃J�������R�s�[
		CCamera back = *CCamera::GetCurrent();

		//�L�����N�^�[��ʂ���̃J����	���Ɍ����Ă���
		CCamera::GetCurrent()->SetTranseRot(mp_player->m_pos + CVector3D(0.0, 15, 0), CVector3D(DtoR(90), 0, 0));

		//�`���̉�ʃT�C�Y����512:�c512�̂��߃A�X�y�N�g���1.0f
		CRect rect = { -5,-5,5,5 };
		//���˓��e�s��Őݒ�
		CCamera::GetCurrent()->Ortho(rect.m_left, rect.m_right, rect.m_top, rect.m_bottom, 5.0f, 45.0f);

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
	
}