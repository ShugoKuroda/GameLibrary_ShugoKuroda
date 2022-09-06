//=============================================================================
//
//	チュートリアル処理 [tutorial.cpp]
//	Author : 黒田 周吾
//
//=============================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include "tutorial.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "game.h"

//======================================================
//	グローバル変数
//======================================================
LPDIRECT3DTEXTURE9 g_pTextureTutorial[MAX_TUTORIAL] = { NULL };			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;	//頂点バッファへのポインタ
bool g_GameStartPushTutorial;		//スタート演出の繰り返し防止
int g_nCnt;							//チュートリアル画面切り替えカウンター
//Title g_aTitle[2];				//タイトル情報

//======================================================
//	タイトル画面の初期化処理
//======================================================
void InitTutorial(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Tutorial000.jpg",
		&g_pTextureTutorial[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Tutorial001.jpg",
		&g_pTextureTutorial[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL);

	//タイトル情報の初期化
	//ZeroMemory(&g_aTitle, sizeof(g_aTitle));

	g_GameStartPushTutorial = false;		//スタート演出繰り返し防止の初期化
	g_nCnt = 0;

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffTutorial->Unlock();

	//サウンド再生
	PlaySound(SOUND_LABEL_BGM000);
}

//======================================================
//	タイトル画面の終了処理
//======================================================
void UninitTutorial(void)
{
	//サウンド停止
	StopSound();

	for (int nCnt = 0; nCnt < MAX_TUTORIAL; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTextureTutorial[nCnt] != NULL)
		{
			g_pTextureTutorial[nCnt]->Release();
			g_pTextureTutorial[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}
}

//======================================================
//	タイトル画面の更新処理
//======================================================
void UpdateTutorial(void)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	if (!g_GameStartPushTutorial)
	{//スタート演出の繰り返し防止
		if (g_nCnt == 0)
		{
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A, PLAYER_1) == true || GetJoypadTrigger(JOYKEY_START, PLAYER_1) == true)
			{//エンターキーを押されたら

				//テクスチャを変える
				g_nCnt++;

				//サウンド再生
				PlaySound(SOUND_LABEL_SE_SELECT1);
			}
		}
		else if (g_nCnt == 1)
		{
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A, PLAYER_1) == true || GetJoypadTrigger(JOYKEY_START, PLAYER_1) == true)
			{//エンターキーを押されたら

				//スタート演出の繰り返し防止
				g_GameStartPushTutorial = true;

				//モード移行
				SetFade(MODE_TITLE);

				//サウンド再生
				PlaySound(SOUND_LABEL_SE_SELECT2);
			}
		}
	}
}

//======================================================
//	タイトル画面の描画処理
//======================================================
void DrawTutorial(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_TUTORIAL; nCnt++)
	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTutorial[g_nCnt]);

		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,		//描画する最初の頂点インデックス
			2);		//プリミティブ（ポリゴン）数
	}
}