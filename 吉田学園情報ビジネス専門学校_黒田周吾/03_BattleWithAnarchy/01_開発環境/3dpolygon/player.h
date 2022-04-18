//===================================================================
//
//		プレイヤー処理のヘッダー[player.h]
//		Author:黒田 周吾
//
//===================================================================
#ifndef _PLAYER_H_			//このマクロ定義がされなかったら
#define _PLAYER_H_			//2重インクルード防止のマクロ定義

//======================================================
//	マクロ定義
//======================================================
#define MAX_PARTS			(20)		//プレイヤーのモデルパーツの最大数
#define MAX_MOTION			(20)		//モーションの最大数
#define MAX_KEY				(20)		//モーションキーの最大数
#define MAX_FRAME			(120)		//モーションの最大フレーム数
#define PLAYER_COLLISION	(40.0f)		//プレイヤーの当たり判定
#define PLAYER_LIFE			(100)		//プレイヤーの体力

//======================================================
//	各プレイヤー構造体の定義
//======================================================

//プレイヤーパーツ構造体
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 rot;			//回転
	D3DXVECTOR3 baseRot;		//基本の角度
	int nIndex;					//このパーツの番号
	int nParent;				//親パーツの番号
	bool bUse;					//使用しているかどうか
}PlayerParts;

//モーション情報の構造体
typedef struct
{
	D3DXVECTOR3 pos[MAX_KEY];		//モーション再生中の位置
	D3DXVECTOR3 rot[MAX_KEY];		//モーション再生中の向き
}KeySet;

//モーション情報の構造体
typedef struct
{
	int nLoop;
	int nNumKey;
	int nFrame[MAX_FRAME];
	KeySet aKey[MAX_KEY];
	bool bUse;
}MotionSet;

//プレイヤーの武器
typedef enum
{
	PLAYERWEAPON_GUN = 0,		//銃
	PLAYERWEAPON_SWORD,			//剣
	PLAYERWEAPON_MAX
}PLAYERWEAPON;

//プレイヤーの剣攻撃時のコンボ状態
typedef enum
{
	PLAYERATTACK_1COMBO = 0,	//一撃目
	PLAYERATTACK_2COMBO,		//二撃目
	PLAYERATTACK_3COMBO,		//三撃目
	PLAYERATTACK_LOOP,			//繰り返し
	PLAYERATTACK_MAX
}PLAYERATTACK;

//プレイヤー構造体
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 posOld;			//前回の位置
	D3DXVECTOR3 rot;			//回転
	D3DXVECTOR3	rotDest;		//目的の角度
	D3DXVECTOR3 move;			//移動量
	D3DXVECTOR3 moveKnockBack;	//被ダメ時のノックバック移動量
	PLAYERWEAPON Weapon;		//武器
	PLAYERATTACK AttackCombo;	//剣攻撃時のコンボ状態
	PlayerParts aParts[MAX_PARTS];		//パーツの情報
	MotionSet aMotion[MAX_MOTION];		//各モーションの情報
	int nIdxShadow;				//影の番号
	int nNumParts;				//パーツ数
	int nCntShot;				//射撃間隔のカウンター
	int nCntAttack;				//武器切り替えカウンター
	int nCntDamage;				//被ダメ状態の切り替えカウンター
	int nCntDie;				//死亡後の挙動用カウンター
	float fMove;				//移動量(不変)
	float fMoveDash;			//ダッシュ時の追加移動量
	float fJump;				//ジャンプ力
	bool bIsJumping;			//ジャンプ中かどうか
	bool bIsAttacking;			//プレイヤーが攻撃中のフラグ(他のモーション誤再生防止用)
	bool bIsAttackingSword;		//プレイヤーが近接攻撃中のフラグ(攻撃中のキー入力無効用)
	bool bIsDashing;			//ダッシュ中かどうか(移動量変化用)
	bool bIsDamage;				//ダメージを受けているかどうか
	bool bIsDie;				//死亡したかどうか
	bool bIsVictory;			//ゲームに勝利したかどうか
	bool bUse;					//使用しているかどうか
}Player;

//======================================================
//	プロトタイプ宣言
//======================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
//bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float Width, float Height);
HRESULT LoadPlayerType(HWND hWnd, char *sXFilePath);
void SetPlayer(Player Player);
void MotionPlayer(void);

#endif