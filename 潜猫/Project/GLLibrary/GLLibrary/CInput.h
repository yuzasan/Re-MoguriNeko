/*!
*	@brief	�L�[���̓N���X
*
*
**/
#pragma once

//�w�b�_�[�t�@�C���̃C���N���[�h
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
//�K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")


#include "windows.h"
#include "CVector.h"




#define PAD_MAX 5

class CInput {
public:
	enum E_STATE{
		ePush,		//�������u��
		eHold,		//�����Ă���
		ePull,		//�������u��
		eFree,		//�����Ă���
		eStateMax,
	};

	enum E_BUTTON {
		eButton1 = 0,
		eButton2,
		eButton3,
		eButton4,
		eButton5,
		eButton6,
		eButton7,
		eButton8,
		eButton9,
		eButton10,
		eButton11,
		eButton12,
		eButton13,
		eMouseL,
		eMouseR,
		eMouseC,
		eUp,
		eDown,
		eLeft,
		eRight,
		eKeyMax = 31
	};
	static unsigned char	m_key_code[PAD_MAX][eKeyMax];
	static unsigned char	m_pad_code[PAD_MAX][16];
	static unsigned long m_key_old[PAD_MAX];
	static unsigned long m_key_state[PAD_MAX][eStateMax];
	static CVector2D m_mouse_vec;
	static int		m_mouse_wheel;
	static int		m_mouse_wheelOld;
	static bool		m_mouse_inside;

	static LPDIRECTINPUT8 m_pDinput;
	static LPDIRECTINPUTDEVICE8 m_pMouse;
	struct SPadDevice{
		LPDIRECTINPUTDEVICE8 m_pPadDevice;
		DIJOYSTATE2 js;
		int no;
	};

	static SPadDevice m_device[PAD_MAX];
	static bool CALLBACK _padCallback(const LPDIDEVICEINSTANCE lpddi, LPVOID pvRef);
public:
	static bool Init();
	static void ClearInstance();

	/*!
		@brief	�L�[�̏�Ԃ�����
		@param	no		[in] �R���g���[���[�ԍ�
		@param	state	[in] ���
		@param	button	[in] �{�^��
		@retval	true:�w�肵����ԁ@false:�w�肵����Ԃł͂Ȃ�
	**/
	static bool GetState(int no,CInput::E_STATE state, CInput::E_BUTTON button){
		return (m_key_state[no][state] & (1<<button))>0;
	}
	/*!
		@brief	�L�[�̏�Ԃ������ݒ�
		@param	no		[in] �R���g���[���[�ԍ�
		@param	state	[in] ���
		@param	button	[in] �{�^��
		@retval	����
	**/
	static void SetState(int no, CInput::E_STATE state, CInput::E_BUTTON button){
		m_key_state[no][state] |= (1<<button);
	}

	/*!
		@brief	�e�{�^���ɑΉ�����L�[��ݒ�
		@param	no		[in] �R���g���[���[�ԍ�
		@param	button	[in] �{�^��
		@param	code	[in] �L�[�R�[�h
		@retval	����
	**/
	static void SetButton(int no, CInput::E_BUTTON button, unsigned char code){
		m_key_code[no][button] = code;
	}
	/*!
		@brief	�e�{�^���ɑΉ�����L�[��ݒ�
		@param	no		[in] �R���g���[���[�ԍ�
		@param	button	[in] �{�^��
		@param	num		[in] �p�b�h�{�^��
		@retval	����
	**/
	static void SetPadButton(int no, CInput::E_BUTTON button, unsigned char num){
		m_pad_code[no][button] = num;
	}

	
	/*!
		@brief	�}�E�X�J�[�\���̍��W���擾
		@retval	�}�E�X�J�[�\���̍��W
	**/
	static CVector2D GetMousePoint() {
		POINT pt;
		//�}�E�X�̌��݂̍��W���擾����
		GetCursorPos(&pt);
		ScreenToClient(GL::hWnd,&pt);

		return CVector2D((float)pt.x, (float)pt.y);
	}
	
	/*!
		@brief	�}�E�X�̈ړ��ʂ��擾
		@retval	�}�E�X�̈ړ���
	**/
	static CVector2D GetMouseVec() {
		return m_mouse_vec;
	}
	
	/*!
		@brief	�}�E�X�J�[�\�����E�C���h�E���Ɏ��߂�t���O�̐ݒ�
		@param	f		[in] true:�E�C���h�E���Ɏ��߂�
		@param	f		[in] true:�E�C���h�E���ŃJ�[�\�������[�v����
		@retval	����
	**/
	static void SetMouseInside(bool f);

	/*!
		@brief	�}�E�X�J�[�\���̕\���ݒ�
		@param	f		[in] true:�\���@false:��\��
		@retval	����
	**/
	static void ShowCursor(bool f) {
		 ::ShowCursor(f);
		 ::ShowCursor(f);
	}
	
	/*!
		@brief	�}�E�X�z�C�[���̕ω��ʎ擾
		@retval	�}�E�X�z�C�[���̕ω���
	**/
	static int GetMouseWheel() {
		return m_mouse_wheelOld;
	}
	/*!
		@brief	�}�E�X�z�C�[���̕ω��ʂ�������
		@param	a		[in] �ω���
		@retval	����
	**/
	static void AddMouseWheel(int a) {
		m_mouse_wheel+=a;
	}
	/*!
		@brief	�p�b�h�f�[�^�̎擾
		@param	no		[in] �R���g���[���ԍ�
		@retval	����
	**/
	static DIJOYSTATE2* GetPadData(int no) {
		return (m_device[no].m_pPadDevice) ? &m_device[no].js : NULL;
	}
	/*!
		@brief	�E�X�e�B�b�N���擾
		@param	no		[in] �R���g���[���ԍ�
		@retval Vec -1.0�`1.0
	**/
	static CVector2D GetRStick(int no);
	/*!
		@brief	���X�e�B�b�N���擾
		@param	no		[in] �R���g���[���ԍ�
		@retval Vec -1.0�`1.0
	**/
	static CVector2D GetLStick(int no);
	/*!
		@brief	�L�[�̏�Ԃ��X�V����
		@retval	����
	**/
	static void Update();
	
	static float toFloat(int i){
		return ((float)i - (0x7FFF)) / 0x8000;
	};
	static void UpdateClipCursor(int f);
};

#define PUSH(key) CInput::GetState(0,CInput::ePush,key)
#define HOLD(key) CInput::GetState(0,CInput::eHold,key)
#define PULL(key) CInput::GetState(0,CInput::ePull,key)
#define FREE(key) CInput::GetState(0,CInput::eFree,key)


#define PUSH_PAD(id,key) CInput::GetState(id,CInput::ePush,key)
#define HOLD_PAD(id,key) CInput::GetState(id,CInput::eHold,key)
#define PULL_PAD(id,key) CInput::GetState(id,CInput::ePull,key)
#define FREE_PAD(id,key) CInput::GetState(id,CInput::eFree,key)