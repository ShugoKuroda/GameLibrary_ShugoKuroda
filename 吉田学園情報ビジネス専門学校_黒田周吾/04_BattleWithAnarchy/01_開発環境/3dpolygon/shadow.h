//===================================================================
//
//		影の設定処理のヘッダー[shadow.h]
//		Author:黒田 周吾
//
//===================================================================
#ifndef _SHADOW_H_			//このマクロ定義がされなかったら
#define _SHADOW_H_			//2重インクルード防止のマクロ定義

//======================================================
//	影の構造体定義
//======================================================
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	float fRadiusX;			//X幅
	float fRadiusZ;			//Z幅
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	bool bUse;				//使用しているかどうか
}Shadow;

//======================================================
//	プロトタイプ宣言
//======================================================
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadiusX, float fRadiusZ);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif