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
#define MAX_UI		(10)			//UI画面で必要な画像数

//======================================================
//	グローバル変数
//======================================================
LPDIRECT3DTEXTURE9 g_pTextureUI[MAX_UI] = { NULL };				//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUI[MAX_UI] = { NULL };		//頂点バッファへのポインタ
UI g_aUI[MAX_UI];		//UI情報
int g_nCntUI;			//UIを出すまでのカウンター

//======================================================
//	UI画面の初期化処理
//======================================================
void InitUI(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,		//1P
		"data/TEXTURE/1P000.png",
		&g_pTextureUI[0]);
	D3DXCreateTextureFromFile(pDevice,		//2P
		"data/TEXTURE/2P000.png",
		&g_pTextureUI[1]);
	D3DXCreateTextureFromFile(pDevice,		//Set1
		"data/TEXTURE/set001.png",
		&g_pTextureUI[2]);
	D3DXCreateTextureFromFile(pDevice,		//Set2
		"data/TEXTURE/set002.png",
		&g_pTextureUI[3]);
	D3DXCreateTextureFromFile(pDevice,		//SetFinal
		"data/TEXTURE/setFinal.png",
		&g_pTextureUI[4]);
	D3DXCreateTextureFromFile(pDevice,		//win
		"data/TEXTURE/win000.png",
		&g_pTextureUI[5]);
	D3DXCreateTextureFromFile(pDevice,		//lose
		"data/TEXTURE/lose000.png",
		&g_pTextureUI[6]);
	D3DXCreateTextureFromFile(pDevice,		//SetCount表記
		"data/TEXTURE/setcount000.png",
		&g_pTextureUI[7]);
	D3DXCreateTextureFromFile(pDevice,		//1Set15point表記
		"data/TEXTURE/1set15points000.png",
		&g_pTextureUI[8]);

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
	g_nCntUI = 0;

	SetUI(D3DXVECTOR3(100.0f, 25.0f, 0.0f), D3DXVECTOR2(200.0f, 50.0f),-1,0);		//1P
	SetUI(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, 25.0f, 0.0f), D3DXVECTOR2(200.0f, 50.0f), -1,1);		//2P
	SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, 25.0f, 0.0f), D3DXVECTOR2(200.0f, 50.0f), -1, 8);			//スコア表記

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
	for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
	{
		if (g_aUI[nCntUI].nCntUse != -1)
		{//表示カウンターが-1の場合、常に表示する

			if (g_aUI[nCntUI].nCntUse <= 0)
			{//カウンター0で非表示	
				g_aUI[nCntUI].bUse = false;
			}
			else
			{
				g_aUI[nCntUI].nCntUse--;		//表示カウンター減算
			}
		}
	}
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
		if (g_aUI[nCnt].bUse)
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
}

//======================================================
//	UIの設定処理
//======================================================
void SetUI(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nCntUse, int nCntUI)
{
	g_aUI[nCntUI].pos = pos;			//位置
	g_aUI[nCntUI].size = size;			//サイズ
	g_aUI[nCntUI].nCntUse = nCntUse;	//表示時間
	g_aUI[nCntUI].bUse = true;			//表示状態にする

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUI[nCntUI]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].size.x / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].size.x / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].size.x / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].size.x / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].size.y / 2), 0.0f);

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
	g_pVtxBuffUI[nCntUI]->Unlock();
}