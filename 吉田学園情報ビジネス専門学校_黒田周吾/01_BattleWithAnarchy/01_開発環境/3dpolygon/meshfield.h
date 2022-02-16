//===================================================================
//
//		メッシュフィールド処理のヘッダー[meshfield.h]
//		Author:黒田 周吾
//
//===================================================================
#ifndef _MESHFIELD_H_			//このマクロ定義がされなかったら
#define _MESHFIELD_H_			//2重インクルード防止のマクロ定義

//======================================================
//	マクロ定義
//======================================================
#define MESHFIELD_SIZE		(6400.0f)	//メッシュフィールドのサイズ

//======================================================
//	プロトタイプ宣言
//======================================================
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);

#endif