///========================================================
//
//					プレイヤー
//					AUTHOR:越本愛彪,黒田周吾
//
//========================================================
#ifndef _PLAYER_H_			//このマクロ定義がされなかったら
#define _PLAYER_H_			//2重インクルード防止のマクロ定義

//======================================================
//	インクルード
//======================================================
#include "main.h"

//======================================================
//	マクロ定義
//======================================================
#define MAX_PARTS			(20)		//プレイヤーのモデルパーツの最大数
#define MAX_MOTION			(20)		//モーションの最大数aa
#define MAX_KEY				(20)		//モーションキーの最大数
#define MAX_FRAME			(120)		//モーションの最大フレーム数
#define PLAYER_WIDTH		(50.0f)		//プレイヤーの幅
#define PLAYER_COLLISION	(10.0f)		//プレイヤーの当たり判定範囲
#define MAX_PLAYER			(2)			//プレイヤーの最大数
#define FIELD_SIZE_X		(240.0f)	//ステージのサイズ(X)
#define FIELD_SIZE_Z		(480.0f)	//ステージのサイズ(Z)

//======================================================
//	列挙型定義
//======================================================
//プレイヤーの状態
typedef enum
{
	PLAYERSTATE_NORMAL = 0,		//通常の状態
	PLAYERSTATE_HAVEDISC,		//ディスクを持っている状態
	PLAYERSTATE_THROW,			//ディスクを投げる状態
	PLAYERSTATE_CATCH,			//ディスクをキャッチした状態
	PLAYERSTATE_WIN,			//ゲームに勝利した状態
	PLAYERSTATE_LOSE,			//ゲームに敗北した状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

//プレイヤーがディスクを投げる方向
typedef enum
{
	PLAYERTHROW_STRAIGHT = 0,		//正面
	PLAYERTHROW_STRAIGHT_LEFT,		//左前
	PLAYERTHROW_LEFT,				//左
	PLAYERTHROW_STRAIGHT_RIGHT,		//右前
	PLAYERTHROW_RIGHT,				//右
	PLAYERTHROW_MAX
}PLAYERTHROW;

//プレイヤー
typedef enum
{
	PLAYER_1 = 0,		//1P
	PLAYER_2,			//2P
	PLAYER_MAX
} PLAYER;

//======================================================
//	構造体定義
//======================================================
//プレイヤーパーツ構造体
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 rot;			//回転
	D3DXVECTOR3 baseRot;		//基本の角度
	D3DXMATRIX mtxWorld;			//ワールドマトリックス
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

//モーション番号情報の構造体
typedef struct
{
	int nFrame;					//現在のフレーム数
	int nKeySetIdx;				//再生中のキーセット番号
	int nMotionIdx;				//再生中のモーション番号
}MotionIdx;

//プレイヤー構造体
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 posOld;			//前回の位置
	D3DXVECTOR3 rot;			//回転
	D3DXVECTOR3	rotDest;		//目的の角度
	D3DXVECTOR3 move;			//移動量
	PLAYERSTATE State;			//状態
	PLAYERTHROW Throw;			//投げる方向
	PlayerParts aParts[MAX_PARTS];			//パーツの情報
	MotionIdx MotionIdx;					//モーション番号情報
	MotionSet aMotion[MAX_MOTION];			//各モーションの情報
	int nIdxShadow;				//影の番号
	int nNumParts;				//パーツ数
	int nSlidingTime;			//スライディングの持続時間
	int nSlidingCoolTime;		//スライディングのクールタイム
	float fMove;				//移動量(不変)
	float fMoveDash;			//ダッシュ時の追加移動量
	float fJump;				//ジャンプ力
	bool bUse;					//使用しているかどうか
	int nCntThrow;				//投げる動作をやめるまでのカウンター
}Player;

//======================================================
//	プロトタイプ宣言
//======================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
HRESULT LoadPlayerType(HWND hWnd, char *sXFilePath);
void SetPlayer(Player player, D3DXVECTOR3 InitPos, int nCnt);
void MotionPlayer(int nCntPlayer);
void CollisionDisc(int nIdxPlayer);
void DiscChange(int nIdxPlayer);
void GameSet(int nWinPlayer, int nLosePlayer);
Player *GetPlayer(void);

#endif