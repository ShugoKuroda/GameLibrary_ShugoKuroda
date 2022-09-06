//-------------------------------------------
//
//エフェクト情報[effect.h]
//Author:平野舞
//
//-------------------------------------------
#ifndef _EFFECT_H_
#define _EFFECT_H_

//インクルードファイル
#include "main.h"

//エフェクトの種類
typedef enum
{
	EFFECT_EFE1 = 0,		//まる
	EFFECT_EFE2,			//輪
	EFFECT_EFE3,			//ぼわぼわ
	EFFECT_EFE4,			//まる
	EFFECT_EFE5,			//花火
	EFFECT_MAX
}EFFECT;

//エフェクトの構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXCOLOR col;			//色
	float fSize;			//半径(大きさ)
	int nLife;				//寿命(表示時間)
	int nType;				//種類
	int nSelect;			//選択
	bool bUse;				//使用しているかどうか
}Effect;

//プロトタイプ宣言
void InitEffect(void);		//初期化処理
void UninitEffect(void);	//終了処理
void UpdateEffect(void);	//更新処理
void DrawEffect(void);		//描画処理
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fSize, int nLife, EFFECT nType,int nSelect);		//設定処理
void SelectEffect(int nCount,int nSelect);		//エフェクト処理の種類
void LoadEffectFile(void);			//外部ファイル情報の読み込み

#endif
