//===================================================================
//
//	敵処理のヘッダー[enemy.h]
//	Author:黒田 周吾
//
//===================================================================
#ifndef _ENEMY_H_			//このマクロ定義がされなかったら
#define _ENEMY_H_			//2重インクルード防止のマクロ定義

//======================================================
//	マクロ定義
//======================================================
#define MAX_ENEMY			(32)		//敵の最大数
#define MAX_ENEMY_PARTS		(10)		//プレイヤーのモデルパーツの最大数
#define MAX_ENEMY_MOTION	(20)		//モーションの最大数
#define MAX_ENEMY_KEY		(20)		//モーションキーの最大数
#define MAX_ENEMY_FRAME		(120)		//モーションの最大フレーム数
#define ENEMY_COLLISION		(50.0f)		//敵の当たり判定(球)の半径

//======================================================
//	各プレイヤー構造体の定義
//======================================================

//プレイヤーパーツ構造体
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 rot;			//回転
	int nIndex;					//このパーツの番号
	int nParent;				//親パーツの番号
	bool bUse;					//使用しているかどうか
}EnemyParts;

//モーション情報の構造体
typedef struct
{
	D3DXVECTOR3 pos[MAX_ENEMY_KEY];		//モーション再生中の位置
	D3DXVECTOR3 rot[MAX_ENEMY_KEY];		//モーション再生中の向き
}EnemyKeySet;

//モーション情報の構造体
typedef struct
{
	int nLoop;
	int nNumKey;
	int nFrame[MAX_ENEMY_FRAME];
	EnemyKeySet aKey[MAX_ENEMY_KEY];
	bool bUse;
}EnemyMotionSet;

//モーション情報の構造体
typedef struct
{
	EnemyParts aParts[MAX_ENEMY_PARTS];				//パーツの情報
	EnemyMotionSet aMotion[MAX_ENEMY_MOTION];		//敵のモーション情報
	int nNumParts;			//パーツ数
	float fMove;			//移動量(不変)
	float fJump;			//ジャンプ量
	int nCntSpawn;			//敵の出現カウンター
}EnemyInfo;

//敵構造体
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 posOld;			//前回の位置
	D3DXVECTOR3 rot;			//回転
	D3DXVECTOR3	rotDest;		//目的の角度
	D3DXVECTOR3 move;			//移動量
	D3DXVECTOR3 moveKnockBack;	//被ダメ時のノックバック移動量
	int nIdxShadow;				//影の番号
	int nCntAttack;				//射撃間隔のカウンター
	int nLife;					//体力
	int nCntDamage;				//被ダメ状態の切り替えカウンター
	int nCntDie;				//敵が死ぬまでのカウンター
	int nCntPlayerAttack;		//プレイヤーの多段剣攻撃防止カウンター
	bool bIsJumping;			//ジャンプ中かどうか
	bool bIsSwordHit;			//プレイヤーの剣攻撃Hit後の多段Hit防止
	bool bIsAttacking;			//攻撃中かどうか
	bool bIsDamage;				//被ダメ中かどうか
	bool bUse;					//使用しているかどうか
}Enemy;

//======================================================
//	プロトタイプ宣言
//======================================================
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
Enemy *GetEnemy(void);
void SetEnemy(void);
void HomingEnemy(int nCnt);
EnemyInfo *GetEnemyInfo(void);
HRESULT LoadEnemyType(HWND hWnd, char *sXFilePath);
void MotionEnemy(void);

#endif