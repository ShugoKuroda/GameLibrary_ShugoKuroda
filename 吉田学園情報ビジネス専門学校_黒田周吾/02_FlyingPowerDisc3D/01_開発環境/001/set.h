//========================================================
//
//					得点時のSetカウント処理
//					AUTHOR:黒田周吾
//
//========================================================
#ifndef _SET_H_
#define _SET_H_

#include "main.h"

//得点時の得点の構造体
typedef struct
{
	D3DXVECTOR3 pos;
	int nScore;
} Set;

//プロトタイプ宣言
void InitSet(void);
void UninitSet(void);
void UpdateSet(void);
void DrawSet(void);
void SetCounter(int nIdxPlayer);

#endif
