//========================================================
//
//			�@�@�@	���_�̏���
//					Author:�z�{���V
//
//========================================================
#ifndef _SCORE_H_
#define _SCORE_H

#include "main.h"

//���_���̓��_�̍\����
typedef struct
{
	D3DXVECTOR3 pos;
	int nScore;
} Score;

//�v���g�^�C�v�錾
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetPoint(int nIdxPlayer, D3DXVECTOR3 Bulletpos);
void ResetGame(void);

#endif
