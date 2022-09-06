#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//プロトタイプ宣言
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, int Type);

//マクロ定義
#define MAX_BULLET			(128)		//弾の最大数
#define BULLET_SIZE			(15.0f)		//弾のサイズ
#define BULLET_TYPE			(2)			//弾の種類
#define BULLET_MOVE			(6.0f)		//弾の移動量

//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 move;				//移動量
	bool bUse;						//使用しているかどうか
	int nType;						//種類
	int g_nCounterAnimBullet;		//アニメーションカウンター
	int g_nPatternAnimBullet;		//アニメーションのパターンNo.
}Bullet;

#endif
