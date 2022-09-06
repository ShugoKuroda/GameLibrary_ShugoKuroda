//=============================================================================
// パーティクル処理のヘッダーファイル
// Author : 黒田 周吾
//=============================================================================
#ifndef _PARTICLE_H_	//このマクロ定義がされなかったら
#define _PARTICLE_H_	//2重インクルード防止のマクロ定義

//======================================================
//	マクロ定義
//======================================================
#define MAX_PARTICLE	(1024)		//パーティクルの最大数

//パーティクルの構造体
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXCOLOR col;		//色
	float fRadius;		//半径
	int nLife;			//寿命
	bool bUse;			//使用しているかどうか
}Particle;

//======================================================
//	プロトタイプ宣言
//======================================================
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos);

#endif