#ifndef _SCORE_H_		//このマクロ定義がされなかったら
#define _SCORE_H_		//2重インクルード防止のマクロ定義

//マクロ定義
#define MAX_SCORE		(8)			//最大スコア数
#define SCORE_SIZE_X	(20.0f)		//スコア表記のサイズ(x)
#define SCORE_SIZE_Y	(40.0f)		//スコア表記のサイズ(y)

//プロトタイプ宣言
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);
void AddScore(int nValue);
int GetScore(void);

#endif