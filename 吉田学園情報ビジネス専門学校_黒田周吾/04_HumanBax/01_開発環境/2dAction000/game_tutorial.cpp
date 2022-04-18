#include "main.h"
#include "game_tutorial.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureGameTutorial[MAX_GAME_TUTORIAL] = { NULL };	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameTutorial = NULL;				//頂点バッファへのポインタ
GAME_TUTORIAL g_aGameTutorial[MAX_GAME_TUTORIAL];						//ゲーム中のチュートリアル画像の情報

//======================================================
//		ゲーム中のチュートリアル画像初期化処理
//======================================================
void InitGameTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/tutorial000.png",
		&g_pTextureGameTutorial[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/tutorial001.png",
		&g_pTextureGameTutorial[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/tutorial002.png",
		&g_pTextureGameTutorial[2]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GAME_TUTORIAL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameTutorial,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameTutorial->Lock(0, 0, (void**)&pVtx, 0);

	//各項目の初期化
	for (int nCnt = 0; nCnt < MAX_GAME_TUTORIAL; nCnt++)
	{
		g_aGameTutorial[nCnt].pos = D3DXVECTOR3(SCREEN_WIDTH + 400.0f, SCREEN_HEIGHT / 2, 0.0f);
		g_aGameTutorial[nCnt].nType = 0;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aGameTutorial[nCnt].pos.x - TUTORIAL_SIZE_X, g_aGameTutorial[nCnt].pos.y - TUTORIAL_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aGameTutorial[nCnt].pos.x + TUTORIAL_SIZE_X, g_aGameTutorial[nCnt].pos.y - TUTORIAL_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aGameTutorial[nCnt].pos.x - TUTORIAL_SIZE_X, g_aGameTutorial[nCnt].pos.y + TUTORIAL_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aGameTutorial[nCnt].pos.x + TUTORIAL_SIZE_X, g_aGameTutorial[nCnt].pos.y + TUTORIAL_SIZE_Y, 0.0f);

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

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロック
	g_pVtxBuffGameTutorial->Unlock();
}

//======================================================
//		ゲーム中のチュートリアル画像の終了処理
//======================================================
void UninitGameTutorial(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < MAX_GAME_TUTORIAL; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTextureGameTutorial[nCnt] != NULL)
		{
			g_pTextureGameTutorial[nCnt]->Release();
			g_pTextureGameTutorial[nCnt] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffGameTutorial != NULL)
	{
		g_pVtxBuffGameTutorial->Release();
		g_pVtxBuffGameTutorial = NULL;
	}
}

//======================================================
//		ゲーム中のチュートリアル画像の更新処理
//======================================================
void UpdateGameTutorial(void)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_GAME_TUTORIAL; nCnt++)
	{
		if (g_aGameTutorial[nCnt].bUse == true)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aGameTutorial[nCnt].pos.x - TUTORIAL_SIZE_X, g_aGameTutorial[nCnt].pos.y - TUTORIAL_SIZE_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aGameTutorial[nCnt].pos.x + TUTORIAL_SIZE_X, g_aGameTutorial[nCnt].pos.y - TUTORIAL_SIZE_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aGameTutorial[nCnt].pos.x - TUTORIAL_SIZE_X, g_aGameTutorial[nCnt].pos.y + TUTORIAL_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aGameTutorial[nCnt].pos.x + TUTORIAL_SIZE_X, g_aGameTutorial[nCnt].pos.y + TUTORIAL_SIZE_Y, 0.0f);
		}
		pVtx += 4;		//頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffGameTutorial->Unlock();
}

//======================================================
//		ゲーム中のチュートリアル画像の描画処理
//======================================================
void DrawGameTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGameTutorial, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < MAX_GAME_TUTORIAL; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureGameTutorial[g_aGameTutorial[nCnt].nType]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			nCnt * 4,		//描画する最初の頂点インデックス
			2);								//プリミティブ（ポリゴン）数
	}
}

//======================================================
//		ゲーム中のチュートリアル画像の設定処理
//======================================================
void SetGameTutorial(D3DXVECTOR3 pos, int nType)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_GAME_TUTORIAL; nCnt++)
	{
		if (g_aGameTutorial[nCnt].bUse == false)
		{
			g_aGameTutorial[nCnt].pos = pos;
			g_aGameTutorial[nCnt].nType = nType;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aGameTutorial[nCnt].pos.x - TUTORIAL_SIZE_X, g_aGameTutorial[nCnt].pos.y - TUTORIAL_SIZE_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aGameTutorial[nCnt].pos.x + TUTORIAL_SIZE_X, g_aGameTutorial[nCnt].pos.y - TUTORIAL_SIZE_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aGameTutorial[nCnt].pos.x - TUTORIAL_SIZE_X, g_aGameTutorial[nCnt].pos.y + TUTORIAL_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aGameTutorial[nCnt].pos.x + TUTORIAL_SIZE_X, g_aGameTutorial[nCnt].pos.y + TUTORIAL_SIZE_Y, 0.0f);

			g_aGameTutorial[nCnt].bUse = true;

			break;
		}
	}
	//頂点バッファをアンロック
	g_pVtxBuffGameTutorial->Unlock();
}