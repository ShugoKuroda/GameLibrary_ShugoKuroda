//===================================================================
//
//		ビルボード処理のヘッダー[billboard.h]
//		Author:黒田 周吾
//
//===================================================================
#ifndef _BILLBOARD_H_		//このマクロ定義がされなかったら
#define _BILLBOARD_H_		//2重インクルード防止のマクロ定義

//======================================================
//	プロトタイプ宣言
//======================================================
void InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(void);
void SetBillboard(D3DXVECTOR3 pos);

#endif