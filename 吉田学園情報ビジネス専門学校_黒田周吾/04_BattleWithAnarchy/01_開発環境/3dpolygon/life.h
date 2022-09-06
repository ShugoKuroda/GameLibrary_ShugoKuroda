//====================================================================================
//
//	プレイヤーの体力処理ヘッダー[life.h]
//	Author:SHUGO KURODA
//
//====================================================================================
#ifndef _LIFE_H_		//このマクロ定義がされなかったら
#define _LIFE_H_		//2重インクルード防止のマクロ定義

//----------------------------------------
//	プロトタイプ宣言
//----------------------------------------
void InitLife(void);
void UninitLife(void);
void UpdateLife(void);
void DrawLife(void);
void SetLife(int nLife);
void AddLife(int nValue);

#endif 