#include "main.h"
#include "back.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBack = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBack = NULL;		//頂点バッファへのポインタ
int g_nCounterAnimBack;								//背景のアニメーションカウンター
int g_nPatternAnimBack;								//背景のアニメーションパターン

//======================================================
//		背景の初期化処理
//======================================================
void InitBack(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/back001.png", &g_pTextureBack);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBack,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBack->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffBack->Unlock();
}

//======================================================
//		背景の終了処理
//======================================================
void UninitBack(void)
{
	//テクスチャの破棄
	if (g_pTextureBack != NULL)
	{
		g_pTextureBack->Release();
		g_pTextureBack = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffBack != NULL)
	{
		g_pVtxBuffBack->Release();
		g_pVtxBuffBack = NULL;
	}
}

//======================================================
//		背景の更新処理
//======================================================
void UpdateBack(void)
{
	VERTEX_2D *pVtx;			//頂点情報へのポインタ
	g_nCounterAnimBack++;		//カウンターを加算

	if (g_nCounterAnimBack % 6 == 0)
	{
		g_nCounterAnimBack = 0;
		g_nPatternAnimBack = (g_nPatternAnimBack - 1) % 10;		//パターンNo.を更新

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffBack->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.1f * (g_nPatternAnimBack % 10), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * ((g_nPatternAnimBack % 10) - 10), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * (g_nPatternAnimBack % 10), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * ((g_nPatternAnimBack % 10) - 10), 1.0f);

		//頂点バッファをアンロック
		g_pVtxBuffBack->Unlock();
	}
}

//======================================================
//		背景の描画処理
//======================================================
void DrawBack(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBack, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBack);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,											//描画する最初の頂点インデックス
		2);											//プリミティブ（ポリゴン）数
}