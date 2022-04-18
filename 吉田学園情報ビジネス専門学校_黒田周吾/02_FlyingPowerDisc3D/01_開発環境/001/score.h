//========================================================
//
//			　　　	得点の処理
//					Author:越本愛彪
//
//========================================================
#ifndef _SCORE_H_
#define _SCORE_H

#include "main.h"

//得点時の得点の構造体
typedef struct
{
	D3DXVECTOR3 pos;
	int nScore;
} Score;

//プロトタイプ宣言
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetPoint(int nIdxPlayer, D3DXVECTOR3 Bulletpos);
void ResetGame(void);

#endif
