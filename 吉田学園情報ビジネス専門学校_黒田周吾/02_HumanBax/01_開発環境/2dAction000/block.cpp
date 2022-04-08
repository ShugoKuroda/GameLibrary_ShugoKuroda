#include "main.h"
#include "block.h"
#include "player.h"
#include "switch.h"
#include "particle.h"
#include "flag.h"
#include "fade.h"
#include "sound.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBlock[MAX_BLOCK_TEX] = { NULL };			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;		//頂点バッファへのポインタ
Block g_aBlock[MAX_BLOCK];							//ブロックの情報
int g_nFlag;		//中間地点の通過番号

//================================================
//			ブロックの初期化処理
//================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	pDevice = GetDevice();			//デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block000.png", &g_pTextureBlock[0]);	//通常ブロック
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block001.png", &g_pTextureBlock[1]);	//可動ブロック
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block002.png", &g_pTextureBlock[2]);	//可動ブロック(ゴールブロック)
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block003.png", &g_pTextureBlock[3]);	//ゲートブロック
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block004.png", &g_pTextureBlock[4]);	//可動ブロック(とげ付き)
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block005.png", &g_pTextureBlock[5]);	//とげブロック
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Exit000.png", &g_pTextureBlock[6]);	//出口

	//ブロックの情報の初期化
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].Airmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].fWidth = 0.0f;
		g_aBlock[nCntBlock].fHeight = 0.0f;
		g_aBlock[nCntBlock].bUse = false;
		g_aBlock[nCntBlock].nMoveCntMax = 0;
		g_aBlock[nCntBlock].nMoveCnt = 0;
		g_aBlock[nCntBlock].nMoveCntSwitch = 0;
		g_aBlock[nCntBlock].nType = 0;
		g_aBlock[nCntBlock].nTypeSwitch = 0;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y, 0.0f);

		//rhwの設定(1.0fで固定)
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBlock->Unlock();
}

//======================================================
//		ブロックの終了処理
//======================================================
void UninitBlock(void)
{
	for (int nCnt = 0; nCnt > MAX_BLOCK_TEX; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTextureBlock != NULL)
		{
			g_pTextureBlock[nCnt]->Release();
			g_pTextureBlock[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//======================================================
//		ブロックの更新処理
//======================================================
void UpdateBlock(void)
{
	Player *pPlayer;

	pPlayer = GetPlayer();

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (g_aBlock[nCnt].bUse == true)
		{//使用している場合

			//前回の位置を保存
			g_aBlock[nCnt].posOld = g_aBlock[nCnt].pos;

			//重力負荷をかける
			if (g_aBlock[nCnt].nType == 1 || g_aBlock[nCnt].nType == 2 || g_aBlock[nCnt].nType == 4)
			{
				g_aBlock[nCnt].move.y += 1.0f;
			}

			//動くブロックの動作処理
			g_aBlock[nCnt].nMoveCnt++;
			if (g_aBlock[nCnt].nMoveCnt == g_aBlock[nCnt].nMoveCntMax)
			{
				g_aBlock[nCnt].nMoveCnt = 0;
				g_aBlock[nCnt].Airmove *= -1.0f;
			}

			//横スクロール処理
			GameScroll(0);

			//位置更新
			g_aBlock[nCnt].pos += g_aBlock[nCnt].Airmove;
			g_aBlock[nCnt].pos += g_aBlock[nCnt].move;
			g_aBlock[nCnt].pos += g_aBlock[nCnt].BlockMove;

			//当たり判定
			CollisionPlayer(&g_aBlock[nCnt].pos, &g_aBlock[nCnt].posOld, &g_aBlock[nCnt].Airmove, g_aBlock[nCnt].fWidth, g_aBlock[nCnt].fHeight);
			CollisionMoveBlock(&g_aBlock[nCnt].pos, &g_aBlock[nCnt].posOld, &g_aBlock[nCnt].move, g_aBlock[nCnt].fWidth, g_aBlock[nCnt].fHeight);

			if (g_aBlock[nCnt].nType == 1 || g_aBlock[nCnt].nType == 2 || g_aBlock[nCnt].nType == 4)
			{
				CollisionBlock(&g_aBlock[nCnt].pos, &g_aBlock[nCnt].posOld, &g_aBlock[nCnt].move, &g_aBlock[nCnt].BlockMove, g_aBlock[nCnt].fWidth, g_aBlock[nCnt].fHeight, 0);
			}
			if (g_aBlock[nCnt].nType == 0 || g_aBlock[nCnt].nType == 5)
			{
				CollisionBlock(&g_aBlock[nCnt].pos, &g_aBlock[nCnt].posOld, &g_aBlock[nCnt].move, &g_aBlock[nCnt].BlockMove, g_aBlock[nCnt].fWidth, g_aBlock[nCnt].fHeight, 1);
			}
			if (g_aBlock[nCnt].nType == 2)
			{
				CollisionFlag(&g_aBlock[nCnt].pos, g_aBlock[nCnt].fWidth, g_aBlock[nCnt].fHeight);
			}

			//頂点座標の更新
			if (g_aBlock[nCnt].nType == 1 || g_aBlock[nCnt].nType == 2 || g_aBlock[nCnt].nType == 4)
			{
				pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y, 0.0f);
			}
			else if (g_aBlock[nCnt].nType == 0 || g_aBlock[nCnt].nType == 3 || g_aBlock[nCnt].nType == 5 || g_aBlock[nCnt].nType == 6)
			{
				pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y - (g_aBlock[nCnt].fHeight / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y - (g_aBlock[nCnt].fHeight / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y + (g_aBlock[nCnt].fHeight / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y + (g_aBlock[nCnt].fHeight / 2), 0.0f);
			}

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_aBlock[nCnt].fTexX, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, g_aBlock[nCnt].fTexY);
			pVtx[3].tex = D3DXVECTOR2(g_aBlock[nCnt].fTexX, g_aBlock[nCnt].fTexY);
		}
		pVtx += 4;		//頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBlock->Unlock();
}

//======================================================
//		ブロックの描画処理
//======================================================
void DrawBlock(void)
{
	int nCntBlock;
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{//弾が使用されている
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBlock[g_aBlock[nCntBlock].nType]);
			//プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				nCntBlock * 4,								//描画する最初の頂点インデックス
				2);		//プリミティブ（ポリゴン）数
		}
	}
}

//======================================================
//		ブロックの設定処理
//======================================================
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nMoveCntMax, int nType,int nTypeSwitch, float width, float height, float TexSizeX, float TexSizeY)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (g_aBlock[nCnt].bUse == false)
		{//ブロックが使用されていない場合

			g_aBlock[nCnt].pos = pos;
			g_aBlock[nCnt].Airmove = move;
			g_aBlock[nCnt].nMoveCntMax = nMoveCntMax;
			g_aBlock[nCnt].nMoveCnt = 0;
			g_aBlock[nCnt].nType = nType;
			g_aBlock[nCnt].nTypeSwitch = nTypeSwitch;
			g_aBlock[nCnt].fWidth = width;
			g_aBlock[nCnt].fHeight = height;
			g_aBlock[nCnt].fTexX = TexSizeX;
			g_aBlock[nCnt].fTexY = TexSizeY;

			//頂点座標の更新
			if (g_aBlock[nCnt].nType == 1 || g_aBlock[nCnt].nType == 2 || g_aBlock[nCnt].nType == 4)
			{
				pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y, 0.0f);
			}
			else if (g_aBlock[nCnt].nType == 0 || g_aBlock[nCnt].nType == 3 || g_aBlock[nCnt].nType == 5 || g_aBlock[nCnt].nType == 6)
			{
				pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y - (g_aBlock[nCnt].fHeight / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y - (g_aBlock[nCnt].fHeight / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y + (g_aBlock[nCnt].fHeight / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y + (g_aBlock[nCnt].fHeight / 2), 0.0f);
			}

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_aBlock[nCnt].fTexX, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, g_aBlock[nCnt].fTexY);
			pVtx[3].tex = D3DXVECTOR2(g_aBlock[nCnt].fTexX, g_aBlock[nCnt].fTexY);

			g_aBlock[nCnt].bUse = true;

			break;
		}
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBlock->Unlock();
}

//======================================================
//		ブロックの当たり判定
//======================================================
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pMoveBlock, float Width, float Height, int nType)
{
	bool bIsLanding = false;		//着地しているかどうか

	//プレイヤー情報を取得
	Player *pPlayer = GetPlayer();

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (g_aBlock[nCnt].bUse == true)
		{
			if (g_aBlock[nCnt].nType == 0 || g_aBlock[nCnt].nType == 5)
			{//通常のブロックの場合
				if (pPos->x + (Width / 2) > g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
					&& pPos->x - (Width / 2) < g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
					&& pPosOld->y <= g_aBlock[nCnt].pos.y - (g_aBlock[nCnt].fHeight / 2)
					&& pPos->y > g_aBlock[nCnt].pos.y - (g_aBlock[nCnt].fHeight / 2))
				{//上からブロックに着地した場合
					if (g_aBlock[nCnt].nType == 5)
					{//とげブロックに乗ったら
						if (nType == 2)
						{
							//サウンド再生
							PlaySound(SOUND_LABEL_SE_EXPLOSION);

							//プレイヤーを消去する
							pPlayer->bUse = false;

							//パーティクル処理
							for (int nCnt = 0; nCnt < 20; nCnt++)
							{
								SetParticle(D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y - (PLAYER_SIZE_Y / 2), pPlayer->pos.z));
							}
							break;
						}
					}
					pPos->y = g_aBlock[nCnt].pos.y - (g_aBlock[nCnt].fHeight / 2);
					bIsLanding = true;
					pMove->y = 0.0f;
					*pMoveBlock = g_aBlock[nCnt].Airmove;

					//リフトブロックに乗った場合
					if (g_aBlock[nCnt].Airmove.x < 0.0f || g_aBlock[nCnt].Airmove.x > 0.0f)
					{
						//横スクロール処理
						GameScroll(1);
					}
				}
			}
			if (g_aBlock[nCnt].nType == 1 || g_aBlock[nCnt].nType == 2 || g_aBlock[nCnt].nType == 4)
			{//可動ブロックの場合
				if (pPos->x + (Width / 2) > g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
					&& pPos->x - (Width / 2) < g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
					&& pPosOld->y <= g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight
					&& pPos->y > g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight)
				{//上からブロックに着地した場合
					if (g_aBlock[nCnt].nType == 4)
					{//とげ付きブロックに乗ったら
						if (nType == 2)
						{
							//プレイヤーを消去する
							pPlayer->bUse = false;

							//パーティクル処理
							for (int nCnt = 0; nCnt < 20; nCnt++)
							{
								SetParticle(D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y - (PLAYER_SIZE_Y / 2), pPlayer->pos.z));
							}

							//サウンド再生
							PlaySound(SOUND_LABEL_SE_EXPLOSION);
							break;
						}
					}
					pPos->y = g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight;
					bIsLanding = true;
					pMove->y = 0.0f;
					*pMoveBlock = g_aBlock[nCnt].Airmove;
				}
			}
			if (nType == 0 || nType == 2)
			{
				if (g_aBlock[nCnt].nType == 1 || g_aBlock[nCnt].nType == 2 || g_aBlock[nCnt].nType == 4)
				{//可動ブロックの場合
					//下からブロックにぶつかった場合
					if (pPos->x + (Width / 2) > g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
						&& pPos->x - (Width / 2) < g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
						&& pPos->y - Height < g_aBlock[nCnt].pos.y
						&& pPosOld->y - Height >= g_aBlock[nCnt].pos.y)
					{
						pPos->y = g_aBlock[nCnt].pos.y + Height;
						pMove->y = 0.0f;
					}
				}
				else
				{
					//下からブロックにぶつかった場合
					if (pPos->x + (Width / 2) > g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
						&& pPos->x - (Width / 2) < g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
						&& pPos->y - Height < g_aBlock[nCnt].pos.y + (g_aBlock[nCnt].fHeight / 2)
						&& pPosOld->y - Height >= g_aBlock[nCnt].pos.y + (g_aBlock[nCnt].fHeight / 2))
					{
						pPos->y = g_aBlock[nCnt].pos.y + (g_aBlock[nCnt].fHeight / 2) + Height;
						pMove->y = 0.0f;
					}
				}
			}

			//if (nType == 1)
			//{
			//	//下からブロックにぶつかった場合
			//	if (pPos->x + (Width / 2) > g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
			//		&& pPos->x - (Width / 2) < g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
			//		&& pPos->y - (Height / 2) < g_aBlock[nCnt].pos.y
			//		&& pPosOld->y - (Height / 2) >= g_aBlock[nCnt].pos.y)
			//	{
			//		CollisionMoveBlock(&g_aBlock[nCnt].pos, &g_aBlock[nCnt].posOld, &g_aBlock[nCnt].Airmove, g_aBlock[nCnt].fWidth, g_aBlock[nCnt].fHeight);
			//	}
			//}

			if (nType == 0 || nType == 2)
			{//可動ブロック、プレイヤーの場合
				if (g_aBlock[nCnt].nType == 1 || g_aBlock[nCnt].nType == 2 || g_aBlock[nCnt].nType == 4)
				{
					//右からブロックにぶつかった時場合
					if (pPosOld->x - (Width / 2) >= g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
						&& pPos->x - (Width / 2) < g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
						&& pPos->y > g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight
						&& pPos->y - Height < g_aBlock[nCnt].pos.y)
					{
						g_aBlock[nCnt].posOld.x = g_aBlock[nCnt].pos.x;
						g_aBlock[nCnt].pos.x = pPos->x - (Width / 2) - (g_aBlock[nCnt].fWidth / 2) - 3;
						CollisionBlock(&g_aBlock[nCnt].pos, &g_aBlock[nCnt].posOld, &g_aBlock[nCnt].move, &g_aBlock[nCnt].Airmove, g_aBlock[nCnt].fWidth, g_aBlock[nCnt].fHeight, 0);
						GameScroll(2);
						pPos->x = g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2) + (Width / 2);
						pMove->x = 0.0f;
					}
				}
				else
				{
					//右からブロックにぶつかった時場合
					if (pPosOld->x - (Width / 2) >= g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
						&& pPos->x - (Width / 2) < g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
						&& pPos->y > g_aBlock[nCnt].pos.y - (g_aBlock[nCnt].fHeight / 2)
						&& pPos->y - Height < g_aBlock[nCnt].pos.y + (g_aBlock[nCnt].fHeight / 2))
					{
						pPos->x = g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2) + (Width / 2);
						pMove->x = 0.0f;
					}
				}
			}

			if (nType == 1)
			{//通常ブロック(リフト)の場合
			 //右からブロックにぶつかった時場合
				if (pPosOld->x - (Width / 2) >= g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
					&& pPos->x - (Width / 2) < g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
					&& pPos->y - (Height / 2) > g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight
					&& pPos->y + (Height / 2) < g_aBlock[nCnt].pos.y)
				{
					pPos->x = g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2) + (Width / 2);
				}
			}

			if (nType == 0 || nType == 2)
			{
				if (g_aBlock[nCnt].nType == 1 || g_aBlock[nCnt].nType == 2 || g_aBlock[nCnt].nType == 4)
				{
					//左からブロックにぶつかった時場合
					if (pPosOld->x + (Width / 2) <= g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
						&& pPos->x + (Width / 2) > g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
						&& pPos->y > g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight
						&& pPos->y - Height < g_aBlock[nCnt].pos.y)
					{
						g_aBlock[nCnt].posOld.x = g_aBlock[nCnt].pos.x;
						g_aBlock[nCnt].pos.x = pPos->x + (Width / 2) + (g_aBlock[nCnt].fWidth / 2) + 3;
						CollisionBlock(&g_aBlock[nCnt].pos, &g_aBlock[nCnt].posOld, &g_aBlock[nCnt].move, &g_aBlock[nCnt].Airmove, g_aBlock[nCnt].fWidth, g_aBlock[nCnt].fHeight, 0);
						GameScroll(2);
						pPos->x = g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2) - (Width / 2);
						pMove->x = 0.0f;
					}
				}
				else
				{
					//左からブロックにぶつかった時場合
					if (pPosOld->x + (Width / 2) <= g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
						&& pPos->x + (Width / 2) > g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
						&& pPos->y > g_aBlock[nCnt].pos.y - (g_aBlock[nCnt].fHeight / 2)
						&& pPos->y - Height < g_aBlock[nCnt].pos.y + (g_aBlock[nCnt].fHeight / 2))
					{
						if (nType == 0)
						{
							if (g_aBlock[nCnt].nType == 6)
							{//出口に触れたら
								//モード移行
								SetFade(MODE_RESULT);

								g_aBlock[nCnt].bUse = false;
							}
						}
						pPos->x = g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2) - (Width / 2);
						pMove->x = 0.0f;
					}
				}
			}

			if (nType == 1)
			{
				//左からブロックにぶつかった時場合
				if (pPosOld->x + (Width / 2) <= g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
					&& pPos->x + (Width / 2) > g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
					&& pPos->y - (Height / 2) > g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight
					&& pPos->y + (Height / 2) < g_aBlock[nCnt].pos.y)
				{
					pPos->x = g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2) - (Width / 2);
				}
			}

		}
	}
	return bIsLanding;
}

//======================================================
//		可動ブロックの当たり判定
//======================================================
void CollisionMoveBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float Width, float Height)
{
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (g_aBlock[nCnt].bUse == true)
		{
			//可動ブロックの場合
			if (g_aBlock[nCnt].nType == 1 || g_aBlock[nCnt].nType == 2 || g_aBlock[nCnt].nType == 4)
			{
				//上からブロックに着地した場合
				if (pPos->x + (Width / 2) > g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
					&& pPos->x - (Width / 2) < g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
					&& pPosOld->y - (Height / 2) <= g_aBlock[nCnt].pos.y
					&& pPos->y - (Height / 2) > g_aBlock[nCnt].pos.y)
				{//可動ブロックを下降させる
					g_aBlock[nCnt].posOld = g_aBlock[nCnt].pos;
					g_aBlock[nCnt].pos.y = pPos->y - (Height / 2);
					CollisionBlock(&g_aBlock[nCnt].pos, &g_aBlock[nCnt].posOld, &g_aBlock[nCnt].move, &g_aBlock[nCnt].BlockMove, g_aBlock[nCnt].fWidth, g_aBlock[nCnt].fHeight, 0);
				}

				//上からブロックに着地した場合
				if (pPos->x + (Width / 2) > g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
					&& pPos->x - (Width / 2) < g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
					&& pPos->y - (Height / 2) < g_aBlock[nCnt].pos.y
					&& pPosOld->y - (Height / 2) >= g_aBlock[nCnt].pos.y)
				{//可動ブロック上昇させる
					g_aBlock[nCnt].pos.y = pPos->y - (Height / 2);
				}

				//右からブロックにぶつかった時場合
				if (pPosOld->x + (Width / 2) <= g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
					&& pPos->x + (Width / 2) > g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
					&& pPos->y - (Height / 2) < g_aBlock[nCnt].pos.y
					&& pPos->y + (Height / 2) > g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight)
				{//ブロックの右に押し出す
					g_aBlock[nCnt].pos.x = pPos->x + (Width / 2) + (g_aBlock[nCnt].fWidth / 2);
				}

				//左からブロックにぶつかった時場合
				if (pPosOld->x - (Width / 2) >= g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
					&& pPos->x - (Width / 2) < g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
					&& pPos->y - (Height / 2) < g_aBlock[nCnt].pos.y
					&& pPos->y + (Height / 2) > g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight)
				{//ブロックの左に押し出す
					g_aBlock[nCnt].pos.x = pPos->x - (Width / 2) - (g_aBlock[nCnt].fWidth / 2);
				}
			}
		}
	}
}

//======================================================
//		スイッチのゲートギミック処理
//======================================================
int OpenGate(int nTypeSwitch)
{
	int IsnPushing = 1;

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (g_aBlock[nCnt].bUse == true)
		{
			//可動ブロックの場合
			if (g_aBlock[nCnt].nType == 3)
			{
				if (nTypeSwitch == 0)
				{
					if (g_aBlock[nCnt].nTypeSwitch == 1)
					{
						g_aBlock[nCnt].pos.y -= 4.0f;
						g_aBlock[nCnt].nMoveCntSwitch++;
						if (g_aBlock[nCnt].nMoveCntSwitch == 30.0f)
						{
							IsnPushing = 2;
						}
					}
				}
				if (nTypeSwitch == 1)
				{
					if (g_aBlock[nCnt].nTypeSwitch == 2)
					{
						g_aBlock[nCnt].pos.y -= 4.0f;
						g_aBlock[nCnt].nMoveCntSwitch++;
						if (g_aBlock[nCnt].nMoveCntSwitch == 30.0f)
						{
							IsnPushing = 2;
						}
					}
				}
				if (nTypeSwitch == 2)
				{
					if (g_aBlock[nCnt].nTypeSwitch == 3)
					{
						g_aBlock[nCnt].pos.y -= 4.0f;
						g_aBlock[nCnt].nMoveCntSwitch++;
						if (g_aBlock[nCnt].nMoveCntSwitch == 30.0f)
						{
							IsnPushing = 2;
						}
					}
				}
			}
		}
	}
	return IsnPushing;
}

//======================================================
//		ブロックの情報を取得
//======================================================
Block *GetBlock(void)
{
	return &g_aBlock[0];
}