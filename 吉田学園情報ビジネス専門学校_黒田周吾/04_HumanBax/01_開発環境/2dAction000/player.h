#ifndef _PLAYER_H_					//このマクロ定義がされてなかったら
#define _PLAYER_H_					//2重インクルード防止のマクロ定義

//マクロ定義
#define PLAYER_SIZE_X		(60.0f)		//プレイヤーのサイズ(X)
#define PLAYER_SIZE_LOOK_X	(15.0f)		//プレイヤーの見た目Xサイズ(余白分を無視した横幅の大きさ)
#define PLAYER_SIZE_Y		(80.0f)		//プレイヤーのサイズ(Y)
#define PLAYER_MOVE			(0.7f)		//プレイヤーの移動量
#define PLAYER_MOVEDOWN		(1.0f)		//プレイヤーへの重力
#define PLAYER_MOVEUP		(-23.0f)	//プレイヤーのジャンプ力

//プレイヤーの構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 posOld;				//前回の位置
	D3DXVECTOR3 move;				//移動量
	D3DXVECTOR3 BlockMove;			//乗っているブロックの移動量情報
	bool bUse;						//使用しているかどうか
	int nDirectionMove;				//プレイヤーの向き(0:右向き,1:左向き)
	bool bIsJumping;				//ジャンプ中かどうか
	int nCounterAnim;				//アニメーションカウンター
	int nPatternAnim;				//アニメー ションのパターンNo.
}Player;

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float Width, float Height);
void GameScroll(int nType);
bool DiePlayer();

#endif