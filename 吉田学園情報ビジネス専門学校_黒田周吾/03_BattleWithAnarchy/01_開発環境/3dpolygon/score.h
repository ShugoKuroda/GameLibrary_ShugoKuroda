//=============================================================================
//
//	スコア処理のヘッダー[score.h]
//	Author : 黒田 周吾
//
//=============================================================================
#ifndef _SCORE_H_		//このマクロ定義がされなかったら
#define _SCORE_H_		//2重インクルード防止のマクロ定義

//======================================================
//	プロトタイプ宣言
//======================================================
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);
void AddScore(int nType);
int GetScore(void);

#endif