//===================================================================
//
//		モデル処理のヘッダー[model.h]
//		Author:黒田 周吾
//
//===================================================================
#ifndef _MODEL_H_			//このマクロ定義がされなかったら
#define _MODEL_H_			//2重インクルード防止のマクロ定義

//======================================================
//	マクロ定義
//======================================================
#define MAX_MODEL		(40)		//モデルの最大数

//======================================================
//	モデルの構造体定義
//======================================================
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 posOld;				//前回の位置
	D3DXVECTOR3 rot;				//回転
	D3DXVECTOR3 move;				//移動量
	D3DXVECTOR3 vtxMax,vtxMin;		//モデルの最大値,最小値
	D3DXMATRIX mtxWorld;			//ワールドマトリックス
	int nIdxShadow;					//影の番号
	int nType;						//種類
	bool bUse;						//使用しているかどうか
}Model;

//======================================================
//	プロトタイプ宣言
//======================================================
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
HRESULT SetModel(HWND hWnd, Model Model);
HRESULT LoadModelType(HWND hWnd, char *sXFilePath);
void LoadModelTexture(int nCnt);
bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float Width, float Height, int nType);
Model *GetModel(void);

#endif