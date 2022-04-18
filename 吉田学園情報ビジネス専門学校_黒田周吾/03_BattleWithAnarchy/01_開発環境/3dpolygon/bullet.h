//===================================================================
//
//		弾の処理[bullet.h]
//		Author:黒田 周吾
//
//===================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

//======================================================
//	インクルード
//======================================================
#include "main.h"

//======================================================
//	マクロ定義
//======================================================
#define BULLET_MOVE			(10.0f)		//弾の移動量

//======================================================
//	弾の構造体定義
//======================================================
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 rot;				//回転
	D3DXVECTOR3 move;				//移動量
	D3DXMATRIX mtxWorld;			//ワールドマトリックス
	bool bUse;						//使用しているかどうか
	int nType;						//種類
	int nLife;						//寿命
	int g_nCounterAnimBullet;		//アニメーションカウンター
	int g_nPatternAnimBullet;		//アニメーションのパターンNo.
}Bullet;

//======================================================
//	プロトタイプ宣言
//======================================================
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int Type);

#endif