//===================================================================
//
//	�p�[�e�B�N���̃w�b�_�[�t�@�C��[particle.h]
//	Author:���c ����
//
//===================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//======================================================
//	�C���N���[�h
//======================================================
#include "main.h"

//======================================================
//	�v���g�^�C�v�錾
//======================================================
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR3 scale, int nType);

#endif