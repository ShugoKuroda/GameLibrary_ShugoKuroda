#ifndef _TUTORIAL_H_			//このマクロ定義がされなかったら
#define _TUTORIAL_H_			//2重インクルード防止のマクロ定義

//マクロ定義
#define MAX_TUTORIAL	(1)//タイトル画面で必要な画像数

// プロトタイプ宣言
void InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);

#endif