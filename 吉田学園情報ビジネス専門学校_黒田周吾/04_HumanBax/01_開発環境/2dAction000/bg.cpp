#include "main.h"
#include "bg.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBg[MAX_BG] = { NULL };			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg[MAX_BG] = { NULL };	//頂点バッファへのポインタ
int g_nCounterAnimBg;										//背景のアニメーションカウンター
int g_nPatternAnimBg;										//背景のアニメーションパターン

//======================================================
//		背景の初期化処理
//======================================================
void InitBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	//5D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/bg000.png", &g_pTextureBg[0]);

	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		//頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffBg[nCnt],
			NULL);
	}

	g_nCounterAnimBg = 0;
	g_nPatternAnimBg = 0;

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffBg[nCnt]->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos.x = 0.0f;
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = SCREEN_WIDTH;
		pVtx[1].pos.y = 0.0f;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = 0.0f;
		pVtx[2].pos.y = SCREEN_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = SCREEN_WIDTH;
		pVtx[3].pos.y = SCREEN_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		//rhwの設定(1.0fで固定)
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//頂点バッファをアンロックする
		g_pVtxBuffBg[nCnt]->Unlock();
	}
}

//======================================================
//		背景の終了処理
//======================================================
void UninitBg(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		if (g_pTextureBg[nCnt] != NULL)
		{
			g_pTextureBg[nCnt]->Release();
			g_pTextureBg[nCnt] = NULL;
		}
	}
	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		//頂点バッファの破棄
		if (g_pVtxBuffBg[nCnt] != NULL)
		{
			g_pVtxBuffBg[nCnt]->Release();
			g_pVtxBuffBg[nCnt] = NULL;
		}
	}
}

//======================================================
//		背景の更新処理
//======================================================
void UpdateBg(void)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	g_nCounterAnimBg++;		//カウンターを加算

	if (g_nCounterAnimBg % 10 == 0)
	{
		g_nCounterAnimBg = 0;
		g_nPatternAnimBg = (g_nPatternAnimBg + 1) % 1000;		//パターンNo.を更新

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffBg[0]->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.002f * (g_nPatternAnimBg % 1000), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.002f * ((g_nPatternAnimBg % 1000) + 500), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.002f * (g_nPatternAnimBg % 1000), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.002f * ((g_nPatternAnimBg % 1000) + 500), 1.0f);
		
		//頂点バッファをアンロック
		g_pVtxBuffBg[0]->Unlock();
	}
}

//======================================================
//		背景の描画処理
//======================================================
void DrawBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffBg[nCnt], 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBg[nCnt]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,											//描画する最初の頂点インデックス
			2);											//プリミティブ（ポリゴン）数
	}
}