//========================================================
//
//						ビルボード
//						AUTHOR:越本愛彪
//
//========================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//テクスチャタイプの構造体
typedef enum
{
	TEXTURETYPE_1 = 0,
	TEXTURETYPE_2,
	TEXTURETYPE_3,
	TEXTURETYPE_4,
	TEXTURETYPE_5,
	TEXTURETYPE_MAX
}BILLBOARDTEXTURE;

//プロトタイプ宣言
void InitBillboard(void);										//ビルボード初期化処理
void UninitBillboard(void);										//ビルボード終了処理
void UpdateBillboard(void);										//ビルボード更新処理
void DrawBillboard(void);										//ビルボード描画処理
void SetBillboard(D3DXVECTOR3 pos, float fSize,int nCntTime, char *pFilename);	//ビルボードの設定処理(位置、サイズ、画像ファイルの名前、テクスチャの種類)

#endif 
