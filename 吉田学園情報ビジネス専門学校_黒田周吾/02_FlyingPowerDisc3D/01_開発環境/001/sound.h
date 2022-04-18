//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0(�^�C�g����)
	SOUND_LABEL_BGM001,			// BGM1(�Q�[����)
	SOUND_LABEL_SE_SHOT,		// �f�B�X�N�𓊂��鉹
	SOUND_LABEL_SE_REVERSE,		// �f�B�X�N�̔��ˉ�
	SOUND_LABEL_SE_KANKYAKU,	// ������
	SOUND_LABEL_SE_VICTORY,		// ����SE
	SOUND_LABEL_SE_SELECT,		// �I����(���艹)
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif