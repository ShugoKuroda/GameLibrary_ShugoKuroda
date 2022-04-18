//===================================================================
//
//	パーティクルのヘッダーファイル[particle.h]
//	Author:黒田 周吾
//
//===================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//======================================================
//	インクルード
//======================================================
#include "main.h"

//======================================================
//	プロトタイプ宣言
//======================================================
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR3 scale, int nType);

#endif