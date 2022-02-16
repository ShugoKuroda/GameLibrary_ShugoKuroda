//===================================================================
//
//	エフェクトの処理[effect.h]
//	Author:黒田 周吾
//
//===================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//======================================================
//	インクルード
//======================================================
#include "main.h"

//======================================================
//	プロトタイプ宣言
//======================================================
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 scale, int nType);

#endif