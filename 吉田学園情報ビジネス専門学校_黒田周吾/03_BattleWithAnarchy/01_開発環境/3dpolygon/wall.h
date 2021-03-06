//===================================================================
//
//		壁の処理のヘッダー[wall.h]
//		Author:黒田 周吾
//
//===================================================================
#ifndef _WALL_H_		//このマクロ定義がされなかったら
#define _WALL_H_		//2重インクルード防止のマクロ定義

//======================================================
//	マクロ定義
//======================================================
#define WALL_WIDTH			(400.0f)	//影の幅
#define WALL_HEIGHT			(100.0f)	//影の高さ

//======================================================
//	壁の構造体
//======================================================
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	bool bUse;		//使用しているかどうか
}Wall;

//======================================================
//	プロトタイプ宣言
//======================================================
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif