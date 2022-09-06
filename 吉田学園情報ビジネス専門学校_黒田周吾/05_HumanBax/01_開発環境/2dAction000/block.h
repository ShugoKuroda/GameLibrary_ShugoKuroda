#ifndef _BLOCK_H_
#define _BLOCK_H_

//マクロ定義
#define MAX_BLOCK			(128)		//ブロックの最大数
#define MAX_BLOCK_TEX		(7)			//ブロックの最大テクスチャ数

//ブロック構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 posOld;				//前回の位置
	D3DXVECTOR3	move;				//移動量
	D3DXVECTOR3 Airmove;			//動く足場の移動量
	D3DXVECTOR3 BlockMove;			//横スクロールの移動量
	int nMoveCnt;					//移動カウンター
	int nMoveCntMax;				//移動カウンター(上限)
	int nTypeSwitch;				//スイッチと連動するギミック番号
	int nMoveCntSwitch;				//ゲートが開ききるまでのカウンター
	int nType;						//種類
	float fWidth;					//幅
	float fHeight;					//高さ
	bool bUse;						//使用しているかどうか
	float fTexX;					//テクスチャ座標(X)
	float fTexY;					//テクスチャ座標(Y)
}Block;

//プロトタイプ宣言
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nMoveCntMax, int nType, int nTypeSwitch, float width, float height, float TexSizeX, float TexSizeY);
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pMovePlayer, float Width, float Height, int nType);
void CollisionMoveBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float Width, float Height);
int OpenGate(int nOpenGate);
Block*GetBlock(void);

#endif