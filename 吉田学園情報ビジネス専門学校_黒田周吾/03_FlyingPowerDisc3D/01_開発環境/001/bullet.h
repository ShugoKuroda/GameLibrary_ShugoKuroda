//===================================================================
//
//	ディスク処理[bullet.h]
//	Author:黒田 周吾
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
#define BULLET_MOVE			(6.0f)		//弾の移動量

//======================================================
//	弾の構造体定義
//======================================================
//弾のモデル(Xファイル)情報
typedef struct
{
	LPD3DXMESH g_pMeshModel = NULL;			//メッシュ情報へのポインタ
	LPD3DXBUFFER g_pBuffMatModel = NULL;	//マテリアル情報へのポインタ
	DWORD g_nNumMatModel = 0;				//マテリアル情報の数
}BulletInfo;

//弾の情報
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 rot;				//回転
	D3DXVECTOR3 move;				//移動量
	D3DXMATRIX mtxWorld;			//ワールドマトリックス
	bool bUse;						//使用しているかどうか
	float fRotCurve;				//弾のカーブ角度
	int nType;						//種類
}Bullet;

//======================================================
//	プロトタイプ宣言
//======================================================
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int Type);
D3DXVECTOR3 ReflectionBullet(const D3DXVECTOR3 move, const D3DXVECTOR3 nor);
Bullet *GetBullet(void);

#endif