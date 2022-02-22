#include "main.h"
#include "game_logo.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturelogo[2] = { NULL };	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufflogo = NULL;		//頂点バッファへのポインタ
D3DXVECTOR3 g_poslogo;		//ゲーム中のロゴ位置
int g_Typelogo;				//ゲーム中のロゴ種類
bool g_blogo;				//ゲーム中にロゴがあるかどうか

//======================================================
//		ゲーム中のロゴ初期化処理
//======================================================
void InitLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//ロゴのいろいろ初期化
	g_poslogo = D3DXVECTOR3(SCREEN_WIDTH + 400.0f, SCREEN_HEIGHT / 2, 0.0f);
	g_Typelogo = 0;
	g_blogo = false;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/start000.png",
		&g_pTexturelogo[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/warning000.png",
		&g_pTexturelogo[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBufflogo,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBufflogo->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_poslogo.x - LOGO_SIZE_X, g_poslogo.y - LOGO_SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_poslogo.x + LOGO_SIZE_X, g_poslogo.y - LOGO_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_poslogo.x - LOGO_SIZE_X, g_poslogo.y + LOGO_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_poslogo.x + LOGO_SIZE_X, g_poslogo.y + LOGO_SIZE_Y, 0.0f);

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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBufflogo->Unlock();
}

//======================================================
//		ゲーム中のロゴの終了処理
//======================================================
void UninitLogo(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < MAX_LOGO; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTexturelogo[nCnt] != NULL)
		{
			g_pTexturelogo[nCnt]->Release();
			g_pTexturelogo[nCnt] = NULL;
		}

		//頂点バッファの破棄
		if (g_pVtxBufflogo != NULL)
		{
			g_pVtxBufflogo->Release();
			g_pVtxBufflogo = NULL;
		}
	}
}

//======================================================
//		ゲーム中のロゴの更新処理
//======================================================
void UpdateLogo(void)
{
	if (g_blogo == true)
	{
		VERTEX_2D *pVtx;		//頂点情報へのポインタ

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBufflogo->Lock(0, 0, (void**)&pVtx, 0);

		g_poslogo += D3DXVECTOR3(-10.0f, 0.0f, 0.0f);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_poslogo.x - LOGO_SIZE_X, g_poslogo.y - LOGO_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_poslogo.x + LOGO_SIZE_X, g_poslogo.y - LOGO_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_poslogo.x - LOGO_SIZE_X, g_poslogo.y + LOGO_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_poslogo.x + LOGO_SIZE_X, g_poslogo.y + LOGO_SIZE_Y, 0.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//頂点バッファをアンロック
		g_pVtxBufflogo->Unlock();

		if (g_poslogo.x < -500.0f || g_poslogo.x > SCREEN_WIDTH + 500.0f)
		{
			g_blogo = false;
		}
	}
}

//======================================================
//		ゲーム中のロゴの描画処理
//======================================================
void DrawLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBufflogo, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturelogo[g_Typelogo]);

	if (g_blogo == true)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,				//描画する最初の頂点インデックス
			2);				//プリミティブ（ポリゴン）数
	}
}

//======================================================
//		ゲーム中のロゴの設定処理
//======================================================
void Setlogo(int nType)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	g_poslogo = D3DXVECTOR3(SCREEN_WIDTH + 400.0f, SCREEN_HEIGHT / 2, 0.0f);
	g_Typelogo = nType;
	g_blogo = true;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBufflogo->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_poslogo.x - LOGO_SIZE_X, g_poslogo.y - LOGO_SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_poslogo.x + LOGO_SIZE_X, g_poslogo.y - LOGO_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_poslogo.x - LOGO_SIZE_X, g_poslogo.y + LOGO_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_poslogo.x + LOGO_SIZE_X, g_poslogo.y + LOGO_SIZE_Y, 0.0f);

	//頂点バッファをアンロック
	g_pVtxBufflogo->Unlock();
}