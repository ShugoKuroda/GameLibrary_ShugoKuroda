//=============================================================================
//
//	�}�E�X���͏��� [inputMouse.cpp]
//	Author : ���c ����
//
//=============================================================================

//======================================================
//	�C���N���[�h
//======================================================
#include "main.h"
#include "inputMouse.h"

//======================================================
//	�}�N����`
//======================================================
#define NUM_MOUSE_MAX		(3)		//�}�E�X�L�[�̍ő吔

//======================================================
//	�O���[�o���ϐ�
//======================================================
LPDIRECTINPUT8 g_pDInput = NULL;				//DirectInput�I�u�W�F�N�g
LPDIRECTINPUTDEVICE8 g_pDIMouse = NULL;			//�}�E�X�f�o�C�X
DIMOUSESTATE g_MouseStatePress;					//�}�E�X�̃v���X���
DIMOUSESTATE g_MouseStateTrigger;				//�}�E�X�̃g���K�[���

//============================================
//	�}�E�X�̏���������
//============================================
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pDInput, NULL)))
	{
		return E_FAIL;
	}

	//���̓f�o�C�X(�}�E�X)�̐���
	if (FAILED(g_pDInput->CreateDevice(GUID_SysMouse, &g_pDIMouse, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDIMouse->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(g_pDIMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return E_FAIL;
	}

	//�}�E�X�ւ̃A�N�Z�X�����l��
	g_pDIMouse->Acquire();

	return S_OK;
}

//============================================
//	�}�E�X�̏I������
//============================================
void UninitMouse(void)
{
	//���̓f�o�C�X(�}�E�X)�̔j��
	if (g_pDIMouse != NULL)
	{
		g_pDIMouse->Unacquire();
		g_pDIMouse->Release();
		g_pDIMouse = NULL;
	}

	//DirectInput�I�u�W�F�N�g�̔j��
	if (g_pDInput != NULL)
	{
		g_pDInput->Release();
		g_pDInput = NULL;
	}
}

//============================================
//	�L�[�{�[�h�̍X�V����
//============================================
void UpdateMouse(void)
{
	DIMOUSESTATE aMouseState;
	int nCntMouse;

	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDIMouse->GetDeviceState(sizeof(aMouseState), &aMouseState)))
	{
		for (nCntMouse = 0; nCntMouse < NUM_MOUSE_MAX; nCntMouse++)
		{
			g_MouseStateTrigger.rgbButtons[nCntMouse] = (g_MouseStatePress.rgbButtons[nCntMouse] ^ aMouseState.rgbButtons[nCntMouse]) & aMouseState.rgbButtons[nCntMouse];		//�g���K�[���̕ۑ�
		}
		g_MouseStatePress = aMouseState;			//�v���X���̕ۑ�														//�L�[�{�[�h�̃v���X����ۑ�
	}
	else
	{
		g_pDIMouse->Acquire();		//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}

//============================================
//	�}�E�X�̃v���X�����擾
//============================================
bool GetMousePress(int nMouse)
{
	return (g_MouseStatePress.rgbButtons[nMouse] & 0x80) ? true : false;		//�O�����Z�q(������)?A:B;(���������^�Ȃ�A��Ԃ��A�U�Ȃ�B��Ԃ�)
}

//============================================
//	�}�E�X�̃g���K�[���̎擾
//============================================
bool GetMouseTrigger(int nMouse)
{
	return(g_MouseStateTrigger.rgbButtons[nMouse] & 0x80) ? true : false;
}

//============================================
//	�}�E�X�̈ړ��ʏ��̎擾
//============================================
DIMOUSESTATE *GetMouseMove(void)
{
	return &g_MouseStatePress;
}