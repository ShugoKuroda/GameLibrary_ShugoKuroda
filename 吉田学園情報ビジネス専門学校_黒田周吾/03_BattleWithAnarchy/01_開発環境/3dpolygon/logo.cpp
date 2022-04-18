//===================================================================
//
//	ゲーム内の指示処理[logo.cpp]
//	Author:黒田 周吾
//
//===================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include "logo.h"
#include "fade.h"

//======================================================
//	構造体
//======================================================
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	int nCntDraw;			//出現カウンター
	int nType;				//種類
	bool bUse;				//使用されているかどうか
}Logo;

//======================================================
//	グローバル変数
//======================================================
LPDIRECT3DTEXTURE9 g_pTexturelogo[MAX_LOGO] = { NULL };	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufflogo = NULL;			//頂点バッファへのポインタ
Logo g_Logo[MAX_LOGO];			//ロゴ情報

//======================================================
//	ゲーム中のロゴ初期化処理
//======================================================
void InitLogo(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ロゴ情報の初期化
	ZeroMemory(&g_Logo, sizeof(g_Logo));

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Ready000.png",
		&g_pTexturelogo[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Start000.png",
		&g_pTexturelogo[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Victory000.png",
		&g_pTexturelogo[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Lose000.png",
		&g_pTexturelogo[3]);

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

	//頂点座標の初期化
	for (int nCnt = 0; nCnt < MAX_LOGO; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Logo[nCnt].pos.x - LOGO_SIZE_X, g_Logo[nCnt].pos.y - LOGO_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Logo[nCnt].pos.x + LOGO_SIZE_X, g_Logo[nCnt].pos.y - LOGO_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Logo[nCnt].pos.x - LOGO_SIZE_X, g_Logo[nCnt].pos.y + LOGO_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Logo[nCnt].pos.x + LOGO_SIZE_X, g_Logo[nCnt].pos.y + LOGO_SIZE_Y, 0.0f);

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
	}

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
	}
	//頂点バッファの破棄
	if (g_pVtxBufflogo != NULL)
	{
		g_pVtxBufflogo->Release();
		g_pVtxBufflogo = NULL;
	}
}

//======================================================
//		ゲーム中のロゴの更新処理
//======================================================
void UpdateLogo(void)
{
	for (int nCnt = 0; nCnt < MAX_LOGO; nCnt++)
	{
		if (g_Logo[nCnt].bUse)
		{
			g_Logo[nCnt].nCntDraw++;		//カウンターの加算

			if (g_Logo[nCnt].nType == 3 || g_Logo[nCnt].nType == 2)
			{
				if (g_Logo[nCnt].nCntDraw >= 180)
				{//カウンターが一定数
					//モード移行
					SetFade(MODE_RANK);
					g_Logo[nCnt].nCntDraw = 0;		//カウンターリセット
				}
			}
			else
			{
				if (g_Logo[nCnt].nCntDraw >= 90)
				{//カウンターが一定数
					g_Logo[nCnt].bUse = false;		//ロゴを消す
					g_Logo[nCnt].nCntDraw = 0;		//カウンターリセット
				}
			}
		}
	}
}

//======================================================
//		ゲーム中のロゴの描画処理
//======================================================
void DrawLogo(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_LOGO; nCnt++)
	{
		if (g_Logo[nCnt].bUse)
		{
			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBufflogo, 0, sizeof(VERTEX_2D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTexturelogo[g_Logo[nCnt].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				0,				//描画する最初の頂点インデックス
				2);				//プリミティブ（ポリゴン）数
		}
	}
}

//======================================================
//		ゲーム中のロゴの設定処理
//======================================================
void SetLogo(int nType)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	for (int nCnt = 0; nCnt < MAX_LOGO; nCnt++)
	{
		if (!g_Logo[nCnt].bUse)
		{
			g_Logo[nCnt].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
			g_Logo[nCnt].nType = nType;
			g_Logo[nCnt].bUse = true;

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBufflogo->Lock(0, 0, (void**)&pVtx, 0);

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_Logo[nCnt].pos.x - LOGO_SIZE_X, g_Logo[nCnt].pos.y - LOGO_SIZE_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Logo[nCnt].pos.x + LOGO_SIZE_X, g_Logo[nCnt].pos.y - LOGO_SIZE_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Logo[nCnt].pos.x - LOGO_SIZE_X, g_Logo[nCnt].pos.y + LOGO_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Logo[nCnt].pos.x + LOGO_SIZE_X, g_Logo[nCnt].pos.y + LOGO_SIZE_Y, 0.0f);

			//頂点バッファをアンロック
			g_pVtxBufflogo->Unlock();

			break;
		}
	}
}