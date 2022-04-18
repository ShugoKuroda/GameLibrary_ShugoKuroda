#include "bullet.h"
#include "main.h"
#include "input.h"
#include "enemy.h"
#include "player.h"
#include "explosion.h"
#include "score.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBullet[BULLET_TYPE] = { NULL };	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;				//頂点バッファへのポインタ
Bullet g_aBullet[MAX_BULLET];									//弾の情報
D3DXVECTOR3 g_PosPlayer;										//プレイヤーの位置情報
int g_nCntScore;												//スコア加算用

//================================================
//			弾の初期化処理
//================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	int nCntBullet;

	pDevice = GetDevice();			//デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gobo.png", &g_pTextureBullet[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet002.png", &g_pTextureBullet[1]);

	g_nCntScore = 0;

	//弾の情報の初期化
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(500.0f, 300.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].bUse = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZE, g_aBullet[nCntBullet].pos.y - BULLET_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZE, g_aBullet[nCntBullet].pos.y - BULLET_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZE, g_aBullet[nCntBullet].pos.y + BULLET_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZE, g_aBullet[nCntBullet].pos.y + BULLET_SIZE, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//======================================================
//		弾の終了処理
//======================================================
void UninitBullet(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < BULLET_TYPE; nCnt++)
	{
		if (g_pTextureBullet[nCnt] != NULL)
		{
			g_pTextureBullet[nCnt]->Release();
			g_pTextureBullet[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//======================================================
//		弾の更新処理
//======================================================
void UpdateBullet(void)
{
	int nCntBullet;		
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	//発射処理
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている
			Enemy *pEnemy;		//エネミーのポインタ
			Player *pPlayer;	//プレイヤーのポインタ
			int nCntEnemy;

			//敵情報取得
			pEnemy = GetEnemy();

			//プレイヤー情報を取得
			pPlayer = GetPlayer();

			//弾の位置の更新
			if (g_aBullet[nCntBullet].nType == 0)
			{
				//位置を更新処理
				g_aBullet[nCntBullet].pos += D3DXVECTOR3(15.0f, 0.0f, 0.0f);

			}
			else if (g_aBullet[nCntBullet].nType == 1)
			{
				g_aBullet[nCntBullet].move.x = sinf(g_aBullet[nCntBullet].pos.z)*BULLET_MOVE;
				g_aBullet[nCntBullet].move.y = cosf(g_aBullet[nCntBullet].pos.z)*BULLET_MOVE;

				//位置を更新処理
				g_aBullet[nCntBullet].pos.x -= g_aBullet[nCntBullet].move.x;
				g_aBullet[nCntBullet].pos.y -= g_aBullet[nCntBullet].move.y;
			}

			//頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZE, g_aBullet[nCntBullet].pos.y - BULLET_SIZE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZE, g_aBullet[nCntBullet].pos.y - BULLET_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZE, g_aBullet[nCntBullet].pos.y + BULLET_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZE, g_aBullet[nCntBullet].pos.y + BULLET_SIZE, 0.0f);

			g_aBullet[nCntBullet].g_nCounterAnimBullet++;		//カウンターを加算
			if (g_aBullet[nCntBullet].g_nCounterAnimBullet % 3 == 0)
			{
				g_aBullet[nCntBullet].g_nCounterAnimBullet = 0;			//カウンターリセット
				g_aBullet[nCntBullet].g_nPatternAnimBullet++;			//アニメーションを進める

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2((0.25f*g_aBullet[nCntBullet].g_nPatternAnimBullet), 0.0f);
				pVtx[1].tex = D3DXVECTOR2((0.25f*(g_aBullet[nCntBullet].g_nPatternAnimBullet + 1)), 0.0f);
				pVtx[2].tex = D3DXVECTOR2((0.25f*g_aBullet[nCntBullet].g_nPatternAnimBullet), 1.0f);
				pVtx[3].tex = D3DXVECTOR2((0.25f*(g_aBullet[nCntBullet].g_nPatternAnimBullet + 1)), 1.0f);
			}

			if (g_aBullet[nCntBullet].pos.x < 0.0f || g_aBullet[nCntBullet].pos.x > SCREEN_WIDTH || g_aBullet[nCntBullet].pos.y < 0.0f || g_aBullet[nCntBullet].pos.y > SCREEN_HEIGHT)
			{//弾が画面の外に出たら
				g_aBullet[nCntBullet].bUse = false;			//使用していない状態にする
			}

			//敵が出す弾の当たり判定
			if (g_aBullet[nCntBullet].nType == 1)
			{
				if (pPlayer->bUse == true)
				{//プレイヤーがいたら
					if (pPlayer->posPlayer.x + PLAYER_SIZE_X / 2 > pVtx[0].pos.x && pPlayer->posPlayer.x - PLAYER_SIZE_X / 2 < pVtx[1].pos.x &&pPlayer->posPlayer.y + PLAYER_SIZE_Y / 2 > pVtx[1].pos.y && pPlayer->posPlayer.y - PLAYER_SIZE_Y / 2 < pVtx[2].pos.y)
					{//プレイヤーに弾が当たったら(↑ここで当たり判定調整)
					 //プレイヤーのヒット処理
						HitPlayer();
						g_aBullet[nCntBullet].bUse = false;		//使用していない状態にする
					}
				}
			}

			//プレイヤーが出す弾の当たり判定
			if (g_aBullet[nCntBullet].nType == 0)
			{
				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//敵が使用されている
						if (pEnemy->posEnemy.x + pEnemy->nSize / 1.5 > pVtx[0].pos.x 
							&& pEnemy->posEnemy.x - pEnemy->nSize / 1.5 < pVtx[1].pos.x
							&&pEnemy->posEnemy.y + pEnemy->nSize / 1.5 > pVtx[1].pos.y
							&& pEnemy->posEnemy.y - pEnemy->nSize / 1.5 < pVtx[2].pos.y)
						{//敵に弾が当たったら(↑ここで当たり判定調整)

							//敵のヒット処理
							HitEnemy(nCntEnemy, 20);		//ここでダメージ量の調整

							g_aBullet[nCntBullet].bUse = false;		//使用していない状態にする
						}
					}
				}
			}
		}
		pVtx += 4;			//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//======================================================
//		弾の描画処理
//======================================================
void DrawBullet(void)
{
	int nCntBullet;
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている
		 //テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBullet[g_aBullet[nCntBullet].nType]);
			//プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				nCntBullet * 4,						//描画する最初の頂点インデックス
				2);		//プリミティブ（ポリゴン）数
		}
	}
}

//======================================================
//		弾の設定処理
//======================================================
void SetBullet(D3DXVECTOR3 pos, int Type)
{
	int nCntBullet;
	int nCntType;
	nCntType = Type;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//弾が使用されていない場合
			g_aBullet[nCntBullet].pos = pos;

			//頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZE, g_aBullet[nCntBullet].pos.y - BULLET_SIZE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZE, g_aBullet[nCntBullet].pos.y - BULLET_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZE, g_aBullet[nCntBullet].pos.y + BULLET_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZE, g_aBullet[nCntBullet].pos.y + BULLET_SIZE, 0.0f);

			g_aBullet[nCntBullet].bUse = true;
			g_aBullet[nCntBullet].nType = nCntType;
			g_aBullet[nCntBullet].g_nCounterAnimBullet = 0;
			g_aBullet[nCntBullet].g_nPatternAnimBullet = 0;

			break;
		}
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}