//=============================================================================
//
//	メッシュ(球)処理 [cylinder.h]
//	Author : SHUGO KURODA
//
//=============================================================================
#ifndef _CYLINDER_H_			//このマクロ定義がされなかったら
#define _CYLINDER_H_			//2重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
//	インクルードファイル
//-----------------------------------------------------------------------------
#include "main.h"

//-----------------------------------------------------------------------------
//	列挙型の定義
//-----------------------------------------------------------------------------
//テクスチャ列挙型
typedef enum
{
	CYLINDER_TEXTURE_0 = 0,		//テクスチャがない状態
	CYLINDER_TEXTURE_1,			//床テクスチャ
	CYLINDER_TEXTURE_MAX		//テクスチャの最大数
}TexturetCylinder;

//-----------------------------------------------------------------------------
//	プロトタイプ宣言
//-----------------------------------------------------------------------------
void InitMeshCylinder(void);		//初期化
void UninitMeshCylinder(void);		//終了処理
void UpdateMeshCylinder(void);		//更新処理
void DrawMeshCylinder(void);		//描画処理
void SetCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadiusX, float fRadiusZ, int nMeshX, int nMeshZ, TexturetCylinder Texturetstate);

#endif
