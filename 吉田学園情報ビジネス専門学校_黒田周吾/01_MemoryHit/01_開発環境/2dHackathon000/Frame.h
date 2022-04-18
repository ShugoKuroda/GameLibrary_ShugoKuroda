#ifndef _FRAME_H_
#define _FRAME_H_

#include "main.h"

//プロトタイプ宣言
void InitFrame(void);
void UninitFrame(void);
void UpdateFrame(void);
void DrawFrame(void);
void SetFrame(D3DXVECTOR3 pos, D3DXVECTOR3 size);
void CollisionFrame(D3DXVECTOR3 pos, float size);
void SelectFrame(void);

#endif