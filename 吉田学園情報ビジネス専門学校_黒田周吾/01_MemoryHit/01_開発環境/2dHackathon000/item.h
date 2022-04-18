//=============================================================================
//
//	ブロック処理のヘッダーファイル
//	Author: SHUGO KURODA
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

//======================================================
//	マクロ定義
//======================================================
#define MAX_ITEM			(128)		//ブロックの最大数
#define MAX_ITEM_TEX		(7)			//ブロックの最大テクスチャ数
#define ITEM_SIZE			(60.0f)		//アイテムのサイズ

//======================================================
//	列挙型
//======================================================
//プレイヤー番号
typedef enum
{
	ITEMSTATE_ON = 0,
	ITEMSTATE_OFF,
	ITEMSTATE_MAX
}ITEMSTATE;

//======================================================
//	構造体
//======================================================
//アイテム構造体
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	int nType;						//種類
	ITEMSTATE state;				//状態
	D3DXVECTOR2 BoxNum;				//入っている枠の番号
	bool bUse;						//使用しているかどうか
	bool bSelect;					//選択したかどうか
}Item;

//======================================================
//	プロトタイプ宣言
//======================================================
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, int nType, float ItemPosX, float ItemPosY);
void CollisionItem(D3DXVECTOR3 pos, float size);
void SelectItem(void);
Item *GetItem(void);

#endif