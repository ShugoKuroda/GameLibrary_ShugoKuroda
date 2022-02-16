#ifndef _TIME_H_		//このマクロ定義がされなかったら
#define _TIME_H_		//2重インクルード防止のマクロ定義

//マクロ定義
#define MAX_TIME		(8)			//最大スコア数
#define TIME_SIZE_X	(20.0f)		//スコア表記のサイズ(x)
#define TIME_SIZE_Y	(40.0f)		//スコア表記のサイズ(y)

//タイマー構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	int nData;				//値
}Time;

//プロトタイプ宣言
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
void SetTime(int nScore);
void AddTime(int nValue);
int GetTime(void);

#endif