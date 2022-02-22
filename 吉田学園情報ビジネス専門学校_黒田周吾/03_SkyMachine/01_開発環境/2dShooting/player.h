#ifndef _PLAYER_H_					//このマクロ定義がされてなかったら
#define _PLAYER_H_					//2重インクルード防止のマクロ定義

//マクロ定義
#define PLAYER_SIZE_X	(40.0f)		//プレイヤーのサイズ(X)
#define PLAYER_SIZE_Y	(20.0f)		//プレイヤーのサイズ(Y)
#define PLAYER_MOVE		(10.0f)		//プレイヤーの移動量

//プレイヤーの状態
typedef enum
{
	PLAYERSTATE_NORMAL = 0,		//通常
	PLAYERSTATE_RESPAWN,		//無敵(リスポーン)状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

//プレイヤーの構造体の定義
typedef struct
{
	D3DXVECTOR3 posPlayer;			//位置
	D3DXVECTOR3 movePlayer;			//移動量
	PLAYERSTATE State;				//状態
	int nCounterState;				//状態カウンター
	int nCounterAnimPlayer;			//アニメーションカウンター
	int nPatternAnimPlayer;			//アニメーションのパターンNo.
	int nCntAttack = 0;				//攻撃間隔のカウンター
	int nHitCnt;					//ヒットカウント
	bool bUse;						//プレイヤーがいるかどうかの判定
	bool bcontrol;					//プレイヤーを操作可能かどうかの判定
	bool bcontrol2;					//プレイヤーを操作可能かどうかの判定2
	int nCntRespawn;				//プレイヤーの復活カウンター
	int nCntRespawn2;				//プレイヤーの復活カウンター2
	int nCntEndGame;				//ゲーム終了遷移までのカウンター
	bool bEndGame;					//ゲーム終了判定
	bool bInvincible;				//プレイヤーの点滅状態切り替え判定
	bool bVictory;					//プレイヤーの勝利判定
	bool bDie;						//死亡判定
}Player;

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void HitPlayer(void);
void DiePlayer(void);

#endif