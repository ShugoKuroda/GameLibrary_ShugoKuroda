//-------------------------------------------
//
//タイム情報[time.h]
//Author:平野舞
//
//-------------------------------------------
#ifndef _TIME_H_
#define _TIME_H_

//インクルードファイル
#include "main.h"

//マクロ定義
#define NUM_TIME		(3)			//タイムの最大桁数

//タイマーの構造体
typedef struct
{
	D3DXVECTOR3 pos[NUM_TIME];		//位置
	D3DXCOLOR color;				//カラー
	int nTime;						//時間指定
	int nFlame;						//フレーム数
	bool bUse;						//使用しているかどうか
}Time;

//プロトタイプ宣言
void InitTime(void);		//初期化処理
void UninitTime(void);		//終了処理
void UpdateTime(void);		//更新処理
void DrawTime(void);		//描画処理
void SetTime(D3DXVECTOR3 pos, D3DXCOLOR color, int nTime);		//タイマーの設定
bool FinishGame(void);

#endif