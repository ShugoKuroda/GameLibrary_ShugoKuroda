#ifndef _LIFE_H_
#define _LIFE_H_

//マクロ定義
#define LIFE_SIZE_X		(50.0f)			//プレイヤー残機表示のXサイズ
#define LIFE_SIZE_Y		(30.0f)			//プレイヤー残機表示のYサイズ
#define MAX_LIFE		(2)				//プレイヤー残機表示数(実際は3機)

// プロトタイプ宣言
void InitLife(void);
void UninitLife(void);
void UpdateLife(void);
void DrawLife(void);
void DownLifePlayer(void);

#endif