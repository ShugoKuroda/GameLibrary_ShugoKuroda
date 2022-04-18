//=============================================================================
//
// タイトル処理のヘッダーファイル
// Author : 黒田 周吾
//
//=============================================================================
#ifndef _TITLE_H_			//このマクロ定義がされなかったら
#define _TITLE_H_			//2重インクルード防止のマクロ定義

//======================================================
//	列挙型の定義
//======================================================
//タイトル内のテクスチャ番号
typedef enum
{
	TITLENUM_BG = 0,			//背景
	TITLENUM_TITLE,				//タイトル
	TITLENUM_PRESSENTER,		//PressEnter
	TITLENUM_SELECTGAME,		//選択肢(ゲーム)
	TITLENUM_SELECTTUTORIAL,	//選択肢(チュートリアル)
	TITLENUM_MAX
}TITLENUM;

//======================================================
//	構造体の定義
//======================================================
//各タイトル情報
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR2 size;		//大きさ
	D3DXCOLOR color;		//色
	bool bUse;				//使用しているかどうか
}Title;

//メニュー画面情報
typedef struct
{
	bool bGameStartPush;	//スタート演出の繰り返し防止
	int nCntColorA;			//α値の設定カウンター
	int nSelect;			//ゲームの選択項目カウンター
	int nCntEnter;			//メニュー選択肢用
}MenuInfo;
//======================================================
//	プロトタイプ宣言
//======================================================
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
void SetTitle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 size, D3DXCOLOR color);

#endif