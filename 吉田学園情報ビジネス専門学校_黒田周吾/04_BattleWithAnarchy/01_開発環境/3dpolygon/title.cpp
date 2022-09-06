//=============================================================================
//
//	タイトル処理 [title.cpp]
//	Author : 黒田 周吾
//
//=============================================================================

//======================================================
//======================================================
#include "main.h"
#include "title.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "player.h"

//======================================================
//	グローバル変数
//======================================================
LPDIRECT3DTEXTURE9 g_pTextureTitle[MAX_TITLE] = { NULL };			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle[MAX_TITLE] = { NULL };		//頂点バッファへのポインタ
bool g_GameStartPush;			//スタート演出の繰り返し防止
Title g_aTitle[2];				//タイトル情報
int g_nCounterAnimBg;
int g_nPatternAnimBg;

//======================================================
//	タイトル画面の初期化処理
//======================================================
void InitTitle(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/titlebg000.jpg",
		&g_pTextureTitle[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/titlename000.png",
		&g_pTextureTitle[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/PressEnter000.png",
		&g_pTextureTitle[2]);

	//頂点バッファの生成
	for (int nCnt = 0; nCnt < MAX_TITLE; nCnt++)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffTitle[nCnt],
			NULL);
	}

	//タイトル情報の初期化
	ZeroMemory(&g_aTitle, sizeof(g_aTitle));
	g_aTitle[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3.0f, 0.0f);
	g_aTitle[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.2f, 0.0f);
	g_nCounterAnimBg = 0;
	g_nPatternAnimBg = 0;

	g_GameStartPush = false;		//スタート演出繰り返し防止の初期化
	
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle[0]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
	g_pVtxBuffTitle[0]->Unlock();

	for (int nCnt = 1; nCnt < MAX_TITLE; nCnt++)
	{
		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffTitle[nCnt]->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aTitle[nCnt].pos.x - (TITLE_SIZE_X / 2), g_aTitle[nCnt].pos.y - (TITLE_SIZE_Y / nCnt), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTitle[nCnt].pos.x + (TITLE_SIZE_X / 2), g_aTitle[nCnt].pos.y - (TITLE_SIZE_Y / nCnt), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTitle[nCnt].pos.x - (TITLE_SIZE_X / 2), g_aTitle[nCnt].pos.y + (TITLE_SIZE_Y / nCnt), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTitle[nCnt].pos.x + (TITLE_SIZE_X / 2), g_aTitle[nCnt].pos.y + (TITLE_SIZE_Y / nCnt), 0.0f);

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
		g_pVtxBuffTitle[nCnt]->Unlock();
	}

	//サウンド再生
	PlaySound(SOUND_LABEL_BGM000);
}

//======================================================
//	タイトル画面の終了処理
//======================================================
void UninitTitle(void)
{
	//サウンド停止
	StopSound();

	for (int nCnt = 0; nCnt < MAX_TITLE; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTextureTitle[nCnt] != NULL)
		{
			g_pTextureTitle[nCnt]->Release();
			g_pTextureTitle[nCnt] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < MAX_TITLE; nCnt++)
	{
		//頂点バッファの破棄
		if (g_pVtxBuffTitle[nCnt] != NULL)
		{
			g_pVtxBuffTitle[nCnt]->Release();
			g_pVtxBuffTitle[nCnt] = NULL;
		}
	}
}

//======================================================
//	タイトル画面の更新処理
//======================================================
void UpdateTitle(void)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	if (g_GameStartPush == false)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//エンターキーを押されたら
			//モード移行
			SetFade(MODE_GAME);

			//スタート演出の繰り返し防止
			g_GameStartPush = true;

			//サウンド再生
			PlaySound(SOUND_LABEL_SE_SELECT2);
		}
	}

	g_nCounterAnimBg++;		//カウンターを加算

	if (g_nCounterAnimBg % 2 == 0)
	{
		g_nCounterAnimBg = 0;
		g_nPatternAnimBg = (g_nPatternAnimBg + 1) % 1000;		//パターンNo.を更新

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffTitle[0]->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.002f * (g_nPatternAnimBg % 1000), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.002f * ((g_nPatternAnimBg % 1000) + 500), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.002f * (g_nPatternAnimBg % 1000), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.002f * ((g_nPatternAnimBg % 1000) + 500), 1.0f);

		//頂点バッファをアンロック
		g_pVtxBuffTitle[0]->Unlock();
	}
}

//======================================================
//	タイトル画面の描画処理
//======================================================
void DrawTitle(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_TITLE; nCnt++)
	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffTitle[nCnt], 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[nCnt]);

		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,		//描画する最初の頂点インデックス
			2);		//プリミティブ（ポリゴン）数
	}
}