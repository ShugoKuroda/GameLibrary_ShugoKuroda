//====================================================================================
//
//	タイマーのヘッダー[time.h]
//	Author:SHUGO KURODA
//
//====================================================================================
#ifndef _TIME_H_		//このマクロ定義がされなかったら
#define _TIME_H_		//2重インクルード防止のマクロ定義

//----------------------------------------
//プロトタイプ宣言
//----------------------------------------
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
void SetTime(int nTime);
void AddTime(int nValue);

#endif 