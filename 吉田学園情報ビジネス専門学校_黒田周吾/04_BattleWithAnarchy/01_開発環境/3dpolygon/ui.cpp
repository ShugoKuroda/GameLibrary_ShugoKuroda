//=============================================================================
//
//	ゲーム画面のUI処理 [ui.cpp]
//	Author : 黒田 周吾
//
//=============================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include "ui.h"

//======================================================
//	マクロ定義
//======================================================
#define MAX_UI				(2)			//UI画面で必要な画像数

//======================================================
//	グローバル変数
//======================================================
LPDIRECT3DTEXTURE9 g_pTextureUI[MAX_UI] = { NULL };				//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUI[MAX_UI] = { NULL };		//頂点バッファへのポインタ
UI g_aUI[MAX_UI];		//UI情報

//======================================================
//	UI画面の初期化処理
//======================================================
void InitUI(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ScoreUI000.png",
		&g_pTextureUI[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/HPUI000.png",
		&g_pTextureUI[1]);

	//頂点バッファの生成
	for (int nCnt = 0; nCnt < MAX_UI; nCnt++)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffUI[nCnt],
			NULL);
	}

	//UI情報の初期化
	ZeroMemory(&g_aUI, sizeof(g_aUI));

	g_aUI[0].pos = D3DXVECTOR3((SCREEN_WIDTH / 2.0f), 25.0f, 0.0f);
	g_aUI[0].size = D3DXVECTOR2(400.0f, 50.0f);

	g_aUI[1].pos = D3DXVECTOR3(120.0f, SCREEN_HEIGHT - 50.0f, 0.0f);
	g_aUI[1].size = D3DXVECTOR2(200.0f, 50.0f);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	for (int nCnt = 0; nCnt < MAX_UI; nCnt++)
	{
		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffUI[nCnt]->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aUI[nCnt].pos.x - (g_aUI[nCnt].size.x / 2), g_aUI[nCnt].pos.y - (g_aUI[nCnt].size.y / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aUI[nCnt].pos.x + (g_aUI[nCnt].size.x / 2), g_aUI[nCnt].pos.y - (g_aUI[nCnt].size.y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aUI[nCnt].pos.x - (g_aUI[nCnt].size.x / 2), g_aUI[nCnt].pos.y + (g_aUI[nCnt].size.y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aUI[nCnt].pos.x + (g_aUI[nCnt].size.x / 2), g_aUI[nCnt].pos.y + (g_aUI[nCnt].size.y / 2), 0.0f);

		//rhwの設定
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

		//頂点バッファをアンロック
		g_pVtxBuffUI[nCnt]->Unlock();
	}
}

//======================================================
//	UI画面の終了処理
//======================================================
void UninitUI(void)
{
	for (int nCnt = 0; nCnt < MAX_UI; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTextureUI[nCnt] != NULL)
		{
			g_pTextureUI[nCnt]->Release();
			g_pTextureUI[nCnt] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < MAX_UI; nCnt++)
	{
		//頂点バッファの破棄
		if (g_pVtxBuffUI[nCnt] != NULL)
		{
			g_pVtxBuffUI[nCnt]->Release();
			g_pVtxBuffUI[nCnt] = NULL;
		}
	}
}

//======================================================
//	UI画面の更新処理
//======================================================
void UpdateUI(void)
{

}

//======================================================
//	UI画面の描画処理
//======================================================
void DrawUI(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_UI; nCnt++)
	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffUI[nCnt], 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureUI[nCnt]);

		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,		//描画する最初の頂点インデックス
			2);		//プリミティブ（ポリゴン）数
	}
}