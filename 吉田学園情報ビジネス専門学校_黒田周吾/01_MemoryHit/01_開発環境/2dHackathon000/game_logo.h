//=============================================================================
// ロゴ処理
// Author : 黒田 周吾
//=============================================================================
#ifndef _LOGO_H_
#define _LOGO_H_

//======================================================
//	マクロ定義
//======================================================
#define LOGO_SIZE_X		(200.0f)		//ゲーム中のロゴのXサイズ
#define LOGO_SIZE_Y		(50.0f)			//ゲーム中のロゴのYサイズ
#define MAX_LOGO		(5)				//ゲーム中のロゴテクスチャ数

//======================================================
//	列挙型の定義
//======================================================
//ロゴのテクスチャ番号
typedef enum
{
	LOGONUM_READY = 0,		//Ready
	LOGONUM_START,			//Start
	LOGONUM_MEMORIZE,		//覚えろ
	LOGONUM_SEARCH,			//探せ
	LOGONUM_FINISH,			//しゅうりょーう
	LOGONUM_MAX
}TITLENUM;

//======================================================
//	構造体定義
//======================================================
//プレイヤー構造体
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	int nLife;			//寿命
	int nType;			//種類
	bool bUse;			//使用しているかどうか
}Logo;

//プレイヤー構造体
typedef struct
{
	int nCnt;			//表示するまでのカウンター
	bool bUse;			//使用しているかどうか
}LogoInfo;

// プロトタイプ宣言
void InitLogo(void);
void UninitLogo(void);
void UpdateLogo(void);
void DrawLogo(void);
void SetLogo(int nType, int nLife);

#endif