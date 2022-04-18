//=============================================================================
//
// ロゴ処理 [game_logo.cpp]
// Author : 黒田 周吾
//
//=============================================================================
#include "main.h"
#include "game_logo.h"
#include "shadow.h"
#include "sound.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturelogo[MAX_LOGO] = { NULL };	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufflogo = NULL;		//頂点バッファへのポインタ
Logo g_aLogo[MAX_LOGO];		//ロゴ表示
LogoInfo g_LogoInfo;		//ロゴ情報

//======================================================
//		ゲーム中のロゴ初期化処理
//======================================================
void InitLogo(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ロゴ情報の初期化
	ZeroMemory(&g_aLogo, sizeof(g_aLogo));
	ZeroMemory(&g_LogoInfo, sizeof(LogoInfo));

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Ready.png",
		&g_pTexturelogo[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Start000.png",
		&g_pTexturelogo[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/memorize000.png",
		&g_pTexturelogo[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/search000.png",
		&g_pTexturelogo[3]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/finish000.png",
		&g_pTexturelogo[4]);

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

	for (int nCntLogo = 0; nCntLogo < MAX_LOGO; nCntLogo++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aLogo[nCntLogo].pos.x - LOGO_SIZE_X, g_aLogo[nCntLogo].pos.y - LOGO_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aLogo[nCntLogo].pos.x + LOGO_SIZE_X, g_aLogo[nCntLogo].pos.y - LOGO_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aLogo[nCntLogo].pos.x - LOGO_SIZE_X, g_aLogo[nCntLogo].pos.y + LOGO_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aLogo[nCntLogo].pos.x + LOGO_SIZE_X, g_aLogo[nCntLogo].pos.y + LOGO_SIZE_Y, 0.0f);

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
	g_LogoInfo.nCnt++;

	if (g_LogoInfo.nCnt == 20)
	{
		SetLogo(LOGONUM_READY, 70);

		//サウンド再生
		PlaySound(SOUND_LABEL_SE_READY);
	}
	if (g_LogoInfo.nCnt == 100)
	{
		SetLogo(LOGONUM_START, 70);
		SetShadow(0);		//影の設定

		//サウンド再生
		PlaySound(SOUND_LABEL_SE_GO);
	}

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBufflogo->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLogo = 0; nCntLogo < MAX_LOGO; nCntLogo++)
	{
		if (g_aLogo[nCntLogo].bUse == true)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aLogo[nCntLogo].pos.x - LOGO_SIZE_X, g_aLogo[nCntLogo].pos.y - LOGO_SIZE_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aLogo[nCntLogo].pos.x + LOGO_SIZE_X, g_aLogo[nCntLogo].pos.y - LOGO_SIZE_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aLogo[nCntLogo].pos.x - LOGO_SIZE_X, g_aLogo[nCntLogo].pos.y + LOGO_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aLogo[nCntLogo].pos.x + LOGO_SIZE_X, g_aLogo[nCntLogo].pos.y + LOGO_SIZE_Y, 0.0f);
		}

		if (g_aLogo[nCntLogo].nLife != -1)
		{
			g_aLogo[nCntLogo].nLife--;
		}
		if (g_aLogo[nCntLogo].nLife == 0)
		{//寿命が0
			g_aLogo[nCntLogo].bUse = false;
		}
	}

	//頂点バッファをアンロック
	g_pVtxBufflogo->Unlock();
}

//======================================================
//		ゲーム中のロゴの描画処理
//======================================================
void DrawLogo(void)
{
	for (int nCntLogo = 0; nCntLogo < MAX_LOGO; nCntLogo++)
	{
		if (g_aLogo[nCntLogo].bUse == true)
		{
			//デバイスの取得
			LPDIRECT3DDEVICE9 pDevice = GetDevice();

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBufflogo, 0, sizeof(VERTEX_2D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTexturelogo[g_aLogo[nCntLogo].nType]);

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
void SetLogo(int nType,int nLife)
{
	for (int nCntLogo = 0; nCntLogo < MAX_LOGO; nCntLogo++)
	{
		if (!g_aLogo[nCntLogo].bUse)
		{
			g_aLogo[nCntLogo].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
			g_aLogo[nCntLogo].nType = nType;
			g_aLogo[nCntLogo].nLife = nLife;
			g_aLogo[nCntLogo].bUse = true;

			VERTEX_2D *pVtx;		//頂点情報へのポインタ

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBufflogo->Lock(0, 0, (void**)&pVtx, 0);

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aLogo[nCntLogo].pos.x - LOGO_SIZE_X, g_aLogo[nCntLogo].pos.y - LOGO_SIZE_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aLogo[nCntLogo].pos.x + LOGO_SIZE_X, g_aLogo[nCntLogo].pos.y - LOGO_SIZE_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aLogo[nCntLogo].pos.x - LOGO_SIZE_X, g_aLogo[nCntLogo].pos.y + LOGO_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aLogo[nCntLogo].pos.x + LOGO_SIZE_X, g_aLogo[nCntLogo].pos.y + LOGO_SIZE_Y, 0.0f);

			//頂点バッファをアンロック
			g_pVtxBufflogo->Unlock();

			break;
		}
	}
}