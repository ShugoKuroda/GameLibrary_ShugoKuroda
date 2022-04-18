//===================================================================
//
//		�G�t�F�N�g�̏���[effect.h]
//		Author:���c ����
//
//===================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//======================================================
//	�C���N���[�h
//======================================================
#include "main.h"

//======================================================
//	�v���g�^�C�v�錾
//======================================================
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 scale, int nType);

#endif