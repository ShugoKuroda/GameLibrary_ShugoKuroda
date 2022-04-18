#include "main.h"
#include "result.h"
#include "player.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_RESULT] = { NULL };//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;//頂点バッファへのポインタ
bool g_GameResultPush;				//リザルト演出の繰り返し防止

//======================================================
//		リザルト画面の初期化処理
//======================================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/result000.png",
		&g_pTextureResult[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/result001.png",
		&g_pTextureResult[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	g_GameResultPush = false;			//終了演出繰り返し防止の初期化

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffResult->Unlock();

	//サウンド再生
	PlaySound(SOUND_LABEL_BGM002);
}

//======================================================
//		リザルト画面の終了処理
//======================================================
void UninitResult(void)
{
	for (int nCnt = 0; nCnt < MAX_RESULT; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTextureResult[nCnt] != NULL)
		{
			g_pTextureResult[nCnt]->Release();
			g_pTextureResult[nCnt] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//======================================================
//		リザルト画面の更新処理
//======================================================
void UpdateResult(void)
{
	if (g_GameResultPush == false)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//エンターキーを押されたら
			//モード移行
			SetFade(MODE_RANK);

			//リザルト演出の繰り返し防止
			g_GameResultPush = true;
		}
	}
}

//======================================================
//		リザルト画面の描画処理
//======================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ
	Player *pPlayer;	//プレイヤーのポインタ

	//プレイヤー情報の取得
	pPlayer = GetPlayer();

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (pPlayer->bVictory == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult[1]);
	}
	else
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult[0]);
	}
		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,		//描画する最初の頂点インデックス
			2);		//プリミティブ（ポリゴン）数
}