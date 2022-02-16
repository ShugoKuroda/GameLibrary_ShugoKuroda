#ifndef _EXPLOSION_H_		 //このマクロ定義がされなかったら
#define _EXPLOSION_H_		 //2重インクルード防止のマクロ定義

//マクロ定義
#define MAX_EXPLOSION		(128)		//爆発の最大数
#define EXPLOSION_SIZE		(100.0f)	//爆発のサイズ

//エネミーの構造体の定義
typedef struct
{
	D3DXVECTOR3 posExplosion;				//位置
	float nSizeExplosion;					//サイズ
	bool bUse;								//使用してるかどうか
	int nCounterAnimExplosion;				//アニメーションカウンター
	int nPatternAnimExplosion;				//アニメーションパターンNo.
}Explosion;

//プロトタイプ宣言
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos, float Size);

#endif

