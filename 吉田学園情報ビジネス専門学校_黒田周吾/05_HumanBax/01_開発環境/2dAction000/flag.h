#ifndef _FLAG_H_		//このマクロ定義がされてなかったら
#define _FLAG_H_		//2重インクルード防止のマクロ定義

//マクロ定義
#define FLAG_SIZE_X		(50.0f)		//ゲーム中のロゴのXサイズ
#define FLAG_SIZE_Y		(50.0f)		//ゲーム中のロゴのYサイズ
#define MAX_FLAG		(2)			//ゲーム中のロゴテクスチャ数

//中間地点構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	int nType;				//種類
	bool bUse;				//使用しているかどうか
}FLAG;

// プロトタイプ宣言
void InitGameFlag(void);
void UninitGameFlag(void);
void UpdateGameFlag(void);
void DrawGameFlag(void);
void SetGameFlag(D3DXVECTOR3 pos, int nType);
void CollisionFlag(D3DXVECTOR3 *pPos, float Width, float Height);
int FlagStage(void);
FLAG *GetFlag(void);

#endif