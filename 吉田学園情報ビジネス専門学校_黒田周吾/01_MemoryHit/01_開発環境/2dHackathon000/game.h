//=============================================================================
// ゲーム画面処理のヘッダーファイル
// Author : 黒田 周吾
//=============================================================================
#ifndef _GAME_H_			//このマクロ定義がされなかったら
#define _GAME_H_			//2重インクルード防止のマクロ定義

//======================================================
//	列挙型
//======================================================
//プレイヤー番号
typedef enum
{
	PLAYER_1 = 0,		//1P
	PLAYER_MAX
} PLAYER;

//======================================================
//	プロトタイプ宣言
//======================================================
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

#endif