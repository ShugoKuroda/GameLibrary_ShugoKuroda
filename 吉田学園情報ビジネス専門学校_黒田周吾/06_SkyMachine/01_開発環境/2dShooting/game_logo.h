#ifndef _LOGO_H_
#define _LOGO_H_

//マクロ定義
#define LOGO_SIZE_X		(200.0f)		//ゲーム中のロゴのXサイズ
#define LOGO_SIZE_Y		(50.0f)			//ゲーム中のロゴのYサイズ
#define MAX_LOGO		(2)				//ゲーム中のロゴテクスチャ数

// プロトタイプ宣言
void InitLogo(void);
void UninitLogo(void);
void UpdateLogo(void);
void DrawLogo(void);
void Setlogo(int nType);

#endif