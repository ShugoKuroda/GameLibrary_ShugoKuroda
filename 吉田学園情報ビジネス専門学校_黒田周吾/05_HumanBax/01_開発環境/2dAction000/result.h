#ifndef _RESULT_H_			//このマクロ定義がされなかったら
#define _RESULT_H_			//2重インクルード防止のマクロ定義

//マクロ定義
#define MAX_RESULT	(3)		//リザルト画面で必要な画像数

// プロトタイプ宣言
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

#endif