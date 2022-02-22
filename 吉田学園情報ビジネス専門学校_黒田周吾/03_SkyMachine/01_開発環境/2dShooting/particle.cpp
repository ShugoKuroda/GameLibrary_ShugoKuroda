#include "main.h"
#include "particle.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureParticle = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;		//頂点バッファへのポインタ
Particle g_aParticle[MAX_PARTICLE];						//パーティクルの情報

//======================================================
//		パーティクルの初期化処理
//======================================================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"date/TEXTURE/particle000.png",
		&g_pTextureParticle);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PARTICLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].bUse = false;
		g_aParticle[nCntParticle].fRadius = 0.0f;
		g_aParticle[nCntParticle].nLife = 0;

		//頂点座標の設定
		pVtx[0].pos = g_aParticle[nCntParticle].pos;
		pVtx[1].pos = g_aParticle[nCntParticle].pos;
		pVtx[2].pos = g_aParticle[nCntParticle].pos;
		pVtx[3].pos = g_aParticle[nCntParticle].pos;

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = g_aParticle[nCntParticle].col;
		pVtx[1].col = g_aParticle[nCntParticle].col;
		pVtx[2].col = g_aParticle[nCntParticle].col;
		pVtx[3].col = g_aParticle[nCntParticle].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4; //データを４つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffParticle->Unlock();
}

//======================================================
//		パーティクルの終了処理
//======================================================
void UninitParticle(void)
{
	//テクスチャの破棄
	if (g_pTextureParticle != NULL)
	{
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}

//======================================================
//		パーティクルの更新処理
//======================================================
void UpdateParticle(void)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
	{
		if (g_aParticle[nCnt].bUse == true)
		{
			g_aParticle[nCnt].pos += g_aParticle[nCnt].move;		//エフェクトの位置更新
			g_aParticle[nCnt].fRadius -= 1.0f;		//エフェクトの半径を減少させる

			//減衰
			g_aParticle[nCnt].move.x -= g_aParticle[nCnt].move.x * 0.1f;

			if (g_aParticle[nCnt].fRadius <= 0.0f)
			{//半径が０以下になった時
				g_aParticle[nCnt].fRadius = 0.0f;
			}
			g_aParticle[nCnt].col.a -= 0.02f;		//エフェクトの透明度の更新

			if (g_aParticle[nCnt].col.a <= 0.0f)
			{//透明度が０以下になった時
				g_aParticle[nCnt].col.a = 0.0f;
			}
		}

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aParticle[nCnt].pos.x - g_aParticle[nCnt].fRadius, g_aParticle[nCnt].pos.y - g_aParticle[nCnt].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCnt].pos.x + g_aParticle[nCnt].fRadius, g_aParticle[nCnt].pos.y - g_aParticle[nCnt].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aParticle[nCnt].pos.x - g_aParticle[nCnt].fRadius, g_aParticle[nCnt].pos.y + g_aParticle[nCnt].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCnt].pos.x + g_aParticle[nCnt].fRadius, g_aParticle[nCnt].pos.y + g_aParticle[nCnt].fRadius, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = g_aParticle[nCnt].col;
		pVtx[1].col = g_aParticle[nCnt].col;
		pVtx[2].col = g_aParticle[nCnt].col;
		pVtx[3].col = g_aParticle[nCnt].col;

		//自然消滅
		g_aParticle[nCnt].nLife--;			//エフェクトのライフを減少
		if (g_aParticle[nCnt].nLife <= 0)
		{
			g_aParticle[nCnt].bUse = false;
		}

		pVtx += 4; //データを４つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffParticle->Unlock();
}

//======================================================
//		パーティクルの描画処理
//======================================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureParticle);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
	{
		if (g_aParticle[nCnt].bUse == true)
		{
			//プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				nCnt * 4,		//描画する最初の頂点インデックス
				2);				//プリミティブ（ポリゴン）数
		}
	}

	//αブレンディングを元の戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//======================================================
//		パーティクルの設定処理
//======================================================
void SetParticle(D3DXVECTOR3 pos)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].move = D3DXVECTOR3(sinf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 30.0f,
														 cosf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 30.0f,
														 0.0f);
			g_aParticle[nCntParticle].col = D3DXCOLOR(3.0f, 0.0f, 0.0f, 0.5f);
			g_aParticle[nCntParticle].fRadius = 30.0f;
			g_aParticle[nCntParticle].nLife = 50;

			g_aParticle[nCntParticle].bUse = true;
			break;
		}
	}
}