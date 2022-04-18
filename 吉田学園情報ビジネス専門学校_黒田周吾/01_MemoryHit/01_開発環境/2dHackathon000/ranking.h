//-------------------------------------------
//
//ランキング情報[ranking.h]
//Author:平野舞
//
//-------------------------------------------

#ifndef _RANKING_H_
#define _RANKING_H_

//インクルードファイル
#include "main.h"

//マクロ定義
#define MAX_RANKX				(2)			//Xの数

//ランキング構造体
typedef struct
{
	D3DXVECTOR3 pos[MAX_RANKX];	//位置
	D3DXCOLOR color;			//カラー
	float fWidth;				//幅間隔
	float fHeigth;				//縦間隔
	int nScore;					//スコア
}Rank;

//プロトタイプ宣言
void InitRanking(void);		//初期化処理
void UninitRanking(void);	//終了処理
void UpdateRanking(void);	//更新処理
void DrawRanking(void);		//描画処理
void ResetRanking(void);	//リセット処理
void SetRanking(D3DXVECTOR3 pos, D3DXCOLOR color,float width, float heigth);		//設定処理

#endif