//=============================================================================
//
//	�}�E�X���͏����̃w�b�_�[�t�@�C��[inputMouse.h]
//	Author : ���c ����
//
//=============================================================================
#ifndef _INPUT_MOUSE_H_		//���̃}�N����`������Ȃ�������
#define _INPUT_MOUSE_H_		//��d�C���N���[�h�h�~�̃}�N����`

//�}�E�X�{�^���̔ԍ�
typedef enum
{
	CLICK_LEFT = 0,			//���N���b�N
	CLICK_RIGHT,			//�E�N���b�N
	CLICK_WHEEL,			//�z�C�[���N���b�N(�����N���b�N)
	CLICK_MAX
}CLICKMOUSE;

//======================================================
//	�v���g�^�C�v�錾
//======================================================
HRESULT InitMouse(HINSTANCE hinstance, HWND hWnd);
void UninitMouse(void);
void UpdateMouse(void);
bool GetMousePress(int nMouse);
bool GetMouseTrigger(int nMouse);
DIMOUSESTATE *GetMouseMove(void);

#endif