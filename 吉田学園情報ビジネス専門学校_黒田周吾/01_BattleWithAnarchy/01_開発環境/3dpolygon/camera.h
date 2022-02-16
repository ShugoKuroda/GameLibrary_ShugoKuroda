//===================================================================
//
//		カメラ処理のヘッダー[camera.h]
//		Author:黒田 周吾
//
//===================================================================
#ifndef _CAMERA_H_			//このマクロ定義がされなかったら
#define _CAMERA_H_			//2重インクルード防止のマクロ定義

//======================================================
//	マクロ定義
//======================================================
#define CAMERA_POS_MOVE		(1.7f)		//視点の移動量
#define CAMERA_ROT_MOVE		(0.03f)		//回転の移動量

//======================================================
//	カメラの構造体
//======================================================
typedef struct
{
	D3DXVECTOR3 posV;		//視点
	D3DXVECTOR3 posR;		//注視点
	D3DXVECTOR3 vecU;		//上方向ベクトル
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 posVDest;	//目的の視点
	D3DXVECTOR3 posRDest;	//目的の注視点
	float fDistance;		//視点から注視点までの距離
	D3DXMATRIX mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX mtxView;			//ビューマトリックス
}Camera;

//======================================================
//	プロトタイプ宣言
//======================================================
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
Camera *GetCamera(void);

#endif