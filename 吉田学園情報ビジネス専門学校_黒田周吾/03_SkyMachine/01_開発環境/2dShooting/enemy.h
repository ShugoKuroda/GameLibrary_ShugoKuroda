#ifndef _ENEMY_H_			//このマクロ定義がされなかったら
#define _ENEMY_H_			//2重インクルード防止のマクロ定義

//マクロ定義
#define MAX_ENEMY			(128)		//敵の最大数
#define MAX_ENEMY_TYPE		(5)			//敵の種類数
#define ENEMY_SIZE			(50.0f)		//敵のサイズ

//敵の状態
typedef enum
{
	ENEMYSTATE_NORMAL = 0,		//通常
	ENEMYSTATE_DAMAGE,			//ダメージ状態
	ENEMYSTATE_MAX
}ENEMYSTATE;

//敵構造体の定義
typedef struct
{
	D3DXVECTOR3 posEnemy;		//位置
	D3DXVECTOR3 moveEnemy;		//移動量
	ENEMYSTATE state;			//状態
	float nSize;				//サイズ
	int nType;					//種類
	int nMoveType;				//動き方の種類
	bool bMoveCnt;				//動き方切り替えカウンター
	int nCntAttack;				//攻撃間隔カウンター
	int nCounterState;			//状態カウンター
	int nLife;					//体力
	bool bUse;					//使用してるかどうか
}Enemy;

//プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nType,int nMoveType,int nLife,float fSize);		//SetEnemy(位置,種類,動き方のパターン,ライフ,サイズ)
void HitEnemy(int nCntEnemy, int nDamage);
Enemy*GetEnemy(void);

#endif
