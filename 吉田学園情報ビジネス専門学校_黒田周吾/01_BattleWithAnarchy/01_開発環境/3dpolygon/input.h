//=============================================================================
//
//	���͏����̃w�b�_�[�t�@�C��[input.h]
//	Author : ���c ����
//
//=============================================================================
#ifndef _INPUT_H_		//���̃}�N����`������Ȃ�������
#define _INPUT_H_		//��d�C���N���[�h�h�~�̃}�N����`

//======================================================
//	�v���g�^�C�v�錾
//======================================================
HRESULT InitKeyboard(HINSTANCE hinstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);

#endif