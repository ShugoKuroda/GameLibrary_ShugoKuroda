#ifndef _GAME_TUTORIAL_H_		//このマクロ定義がされてなかったら
#define _GAME_TUTORIAL_H_		//2重インクルード防止のマクロ定義

//マクロ定義
#define TUTORIAL_SIZE_X		(200.0f)		//ゲーム中のロゴのXサイズ
#define TUTORIAL_SIZE_Y		(50.0f)			//ゲーム中のロゴのYサイズ
#define MAX_GAME_TUTORIAL		(3)				//ゲーム中のロゴテクスチャ数

//ゲーム中のチュートリアル構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	int nType;				//種類
	bool bUse;				//使用しているかどうか
}GAME_TUTORIAL;

// プロトタイプ宣言
void InitGameTutorial(void);
void UninitGameTutorial(void);
void UpdateGameTutorial(void);
void DrawGameTutorial(void);
void SetGameTutorial(D3DXVECTOR3 pos, int nType);

#endif