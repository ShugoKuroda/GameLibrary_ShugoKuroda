#include "main.h"
#include "tutorial.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureTutorial[MAX_TUTORIAL] = { NULL };			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial[MAX_TUTORIAL] = { NULL };	//頂点バッファへのポインタ
bool g_GameStartPush2;				//スタート演出の繰り返し防止

//======================================================
//		チュートリアル画面の初期化処理
//======================================================
void InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Tutorial000.png",
		&g_pTextureTutorial[0]);

	//頂点バッファの生成
	for (int nCnt = 0; nCnt < MAX_TUTORIAL; nCnt++)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffTutorial[nCnt],
			NULL);
	}

	g_GameStartPush2 = false;		//スタート演出繰り返し防止の初期化

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorial[0]->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffTutorial[0]->Unlock();
}

//======================================================
//		チュートリアル画面の終了処理
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

	for (int nCnt = 0; nCnt < MAX_TUTORIAL; nCnt++)
	{
		//頂点バッファの破棄
		if (g_pVtxBuffTutorial[nCnt] != NULL)
		{
			g_pVtxBuffTutorial[nCnt]->Release();
			g_pVtxBuffTutorial[nCnt] = NULL;
		}
	}
}

//======================================================
//		チュートリアル画面の更新処理
//======================================================
void UpdateTutorial(void)
{
	if (g_GameStartPush2 == false)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//エンターキーを押されたら
			//モード移行
			SetFade(MODE_GAME);

			//スタート演出の繰り返し防止
			g_GameStartPush2 = true;
		}
	}
}

//======================================================
//		チュートリアル画面の描画処理
//======================================================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_TUTORIAL; nCnt++)
	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffTutorial[nCnt], 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTutorial[nCnt]);

		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,		//描画する最初の頂点インデックス
			2);		//プリミティブ（ポリゴン）数
	}
}