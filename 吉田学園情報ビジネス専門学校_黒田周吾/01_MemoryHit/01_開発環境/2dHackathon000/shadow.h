//=============================================================================
// フェード処理のヘッダーファイル
// Author : 黒田 周吾
//=============================================================================
#ifndef _SHADOW_H_			//このマクロ定義がされなかったら
#define _SHADOW_H_			//2重インクルード防止のマクロ定義

#include "main.h"

//フェードの状態
typedef enum
{
	SHADOW_NONE = 0,		//何もしていない状態
	SHADOW_IN,			//フェードイン状態
	SHADOW_OUT,			//フェードアウト
	SHADOW_MAX
}SHADOW;

//======================================================
//	プロトタイプ宣言
//======================================================
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
void SetShadow(int nFinish);
SHADOW *GetShadow(void);

#endif