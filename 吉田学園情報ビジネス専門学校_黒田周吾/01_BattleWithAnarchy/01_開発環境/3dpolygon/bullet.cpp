//===================================================================
//
//	弾の処理[bullet.cpp]
//	Author:黒田 周吾
//
//===================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "effect.h"
#include "particle.h"
#include "function.h"
#include "score.h"
#include "sound.h"

//======================================================
//	マクロ定義
//======================================================
#define MAX_BULLET			(128)		//弾の最大数
#define BULLET_SIZE			(8.0f)		//弾のサイズ
#define BULLET_TYPE			(1)			//弾の種類
#define BULLET_LIFE			(180)		//弾の寿命
#define BULLET_COLLISION	(5.0f)		//弾の当たり判定(球)の半径		

//================================================
//	グローバル変数
//================================================
LPDIRECT3DTEXTURE9 g_pTextureBullet[BULLET_TYPE] = { NULL };	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;				//頂点バッファへのポインタ
Bullet g_aBullet[MAX_BULLET];									//弾の情報

//================================================
//	弾の初期化処理
//================================================
void InitBullet(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet000.png", &g_pTextureBullet[0]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	//弾構造体の初期化
	ZeroMemory(&g_aBullet, sizeof(g_aBullet));

	//各項目の初期化
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (BULLET_SIZE / 2), g_aBullet[nCntBullet].pos.y + (BULLET_SIZE / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (BULLET_SIZE / 2), g_aBullet[nCntBullet].pos.y + (BULLET_SIZE / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (BULLET_SIZE / 2), g_aBullet[nCntBullet].pos.y - (BULLET_SIZE / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (BULLET_SIZE / 2), g_aBullet[nCntBullet].pos.y - (BULLET_SIZE / 2), 0.0f);

		//各頂点の法線の設定(ベクトルの大きさは１にする必要がある)
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
	g_pVtxBuffBullet->Unlock();
}

//======================================================
//	弾の終了処理
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
//	弾の更新処理
//======================================================
void UpdateBullet(void)
{
	//発射処理
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//敵情報の取得
		Enemy *pEnemy = GetEnemy();

		if (g_aBullet[nCntBullet].bUse)
		{//弾が使用されている場合

			g_aBullet[nCntBullet].nLife--;

			//弾の位置の更新
			switch (g_aBullet[nCntBullet].nType)
			{
			case 0:
				//位置を更新処理
				g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
				g_aBullet[nCntBullet].pos.z += g_aBullet[nCntBullet].move.z;
				break;

			case 1:
				g_aBullet[nCntBullet].move.x = sinf(g_aBullet[nCntBullet].pos.z) * BULLET_MOVE;
				g_aBullet[nCntBullet].move.z = cosf(g_aBullet[nCntBullet].pos.z) * BULLET_MOVE;

				//位置を更新処理
				g_aBullet[nCntBullet].pos.x -= g_aBullet[nCntBullet].move.x;
				g_aBullet[nCntBullet].pos.z -= g_aBullet[nCntBullet].move.z;
				break;

			default:
				break;
			}
			for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++)
			{
				if (pEnemy->bUse)
				{//敵が使用されている場合

					//弾と敵との当たり判定
					if (SphereCollision(pEnemy->pos, g_aBullet[nCntBullet].pos, ENEMY_COLLISION + 10.0f, BULLET_COLLISION))
					{
						pEnemy->nLife -= 5.0f;		//敵の体力減少
						g_aBullet[nCnt].bUse = false;		//弾を消す
						AddScore(2);			//スコア加算

						//サウンド再生
						PlaySound(SOUND_LABEL_SE_HIT);

						//パーティクル処理
						for (int nCnt = 0; nCnt < 20; nCnt++)
						{
							SetParticle(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y, pEnemy->pos.z),		//位置
								D3DXVECTOR3(sinf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f,		//移動量
									tanf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f,
									cosf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f),
								D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f),		//色
								D3DXVECTOR3(1.0f, 1.0f, 1.0f),			//大きさ
								0);				//大きさ
						}
					}
				}
			}

			//エフェクトの設定
			SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.5f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0);		//（座標、色、大きさ、寿命、種類）

			if (g_aBullet[nCntBullet].nLife <= 0)
			{//弾の寿命が無くなった
				g_aBullet[nCntBullet].bUse = false;
			}
		}
	}
}

//======================================================
//	弾の描画処理
//======================================================
void DrawBullet(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DXMATRIX mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxView;		//ビューマトリックス

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if (g_aBullet[nCnt].bUse)
		{//弾が使用されている場合
			 //ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBullet[nCnt].mtxWorld);

			//ビューマトリックスの設定
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//カメラの逆行列を設定
			g_aBullet[nCnt].mtxWorld._11 = mtxView._11;
			g_aBullet[nCnt].mtxWorld._12 = mtxView._21;
			g_aBullet[nCnt].mtxWorld._13 = mtxView._31;
			g_aBullet[nCnt].mtxWorld._21 = mtxView._12;
			g_aBullet[nCnt].mtxWorld._22 = mtxView._22;
			g_aBullet[nCnt].mtxWorld._23 = mtxView._32;
			g_aBullet[nCnt].mtxWorld._31 = mtxView._13;
			g_aBullet[nCnt].mtxWorld._32 = mtxView._23;
			g_aBullet[nCnt].mtxWorld._33 = mtxView._33;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCnt].pos.x, g_aBullet[nCnt].pos.y, g_aBullet[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCnt].mtxWorld, &g_aBullet[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCnt].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBullet[0]);

			//プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				nCnt * 4,	//描画する最初の頂点インデックス
				2);			//プリミティブ（ポリゴン）数
		}
	}
	//ライトを有効に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//テクスチャを元に戻す
	pDevice->SetTexture(0, NULL);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//======================================================
//	弾の設定処理
//======================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int Type)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (!g_aBullet[nCntBullet].bUse)
		{//弾が使用されていない場合
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].nType = Type;
			g_aBullet[nCntBullet].nLife = BULLET_LIFE;
			g_aBullet[nCntBullet].bUse = true;

			break;
		}
	}
}