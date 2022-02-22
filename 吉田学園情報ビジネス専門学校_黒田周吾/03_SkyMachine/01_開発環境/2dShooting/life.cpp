#include "main.h"
#include "life.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureLife = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;		//頂点バッファへのポインタ
D3DXVECTOR3 g_posLife;		//ライフの位置
int g_nLife;				//現在のライフ

//======================================================
//		プレイヤー残機の初期化処理
//======================================================
void InitLife(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//ライフの位置の初期化
	g_posLife = D3DXVECTOR3(10.0f, 10.0f, 0.0f);

	//現在のライフの初期化
	g_nLife = MAX_LIFE;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Player001.png",
		&g_pTextureLife);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LIFE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_LIFE; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posLife.x + LIFE_SIZE_X * nCnt, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posLife.x + LIFE_SIZE_X * (nCnt + 1), 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posLife.x + LIFE_SIZE_X * nCnt, LIFE_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posLife.x + LIFE_SIZE_X * (nCnt + 1), LIFE_SIZE_Y, 0.0f);

		//rhwの設定
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
		pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

		pVtx += 4;  //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffLife->Unlock();
}

//======================================================
//		プレイヤー残機の終了処理
//======================================================
void UninitLife(void)
{
		//テクスチャの破棄
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}

		//頂点バッファの破棄
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}
}

//======================================================
//		プレイヤー残機の更新処理
//======================================================
void UpdateLife(void)
{

}

//======================================================
//		プレイヤー残機の描画処理
//======================================================
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffLife, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureLife);

	for (int nCnt = 0; nCnt < g_nLife; nCnt++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			4 * nCnt,		//描画する最初の頂点インデックス
			2);				//プリミティブ（ポリゴン）数
	}
}

//======================================================
//		プレイヤー残機の減少処理
//======================================================
void DownLifePlayer(void)
{
	g_nLife--;
}