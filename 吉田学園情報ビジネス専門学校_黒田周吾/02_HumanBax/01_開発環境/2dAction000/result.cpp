#include "main.h"
#include "result.h"
#include "player.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_RESULT] = { NULL };			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult[MAX_RESULT] = { NULL };	//頂点バッファへのポインタ
bool g_GameResultPush;		//リザルト演出の繰り返し防止

//リザルト演出用オブジェクト情報
D3DXVECTOR3 g_PlayerPos2;		//位置を初期化する
D3DXVECTOR3 g_BlockPos2;			//位置を初期化する
D3DXVECTOR3 g_PlayerMove2;		//移動量を初期化する
int g_PlayerDirectionMove2;		//プレイヤーの向き初期化する
int g_PlayerCounterAnim2;		//カウンターを初期化する
int g_PlayerPatternAnim2;		//パターンNo.を初期化する

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
		"data/TEXTURE/player000.png",
		&g_pTextureResult[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block002.png",
		&g_pTextureResult[2]);

	for (int nCnt = 0; nCnt < MAX_RESULT; nCnt++)
	{
		//頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffResult[nCnt],
			NULL);
	}

	g_PlayerPos2 = D3DXVECTOR3(-200.0f, 470.0f, 0.0f);	//位置を初期化する
	g_BlockPos2 = D3DXVECTOR3(-170.0f, 435.0f, 0.0f);	//位置を初期化する
	g_PlayerMove2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量を初期化する
	g_PlayerDirectionMove2 = 0;		//プレイヤーの向き初期化する
	g_PlayerCounterAnim2 = 0;		//カウンターを初期化する
	g_PlayerPatternAnim2 = 0;		//パターンNo.を初期化する

	g_GameResultPush = false;			//終了演出繰り返し防止の初期化

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult[0]->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffResult[0]->Unlock();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult[1]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定(中心座標は下辺の中心)
	pVtx[0].pos.x = g_PlayerPos2.x - PLAYER_SIZE_X / 2;
	pVtx[0].pos.y = g_PlayerPos2.y - PLAYER_SIZE_Y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_PlayerPos2.x + PLAYER_SIZE_X / 2;
	pVtx[1].pos.y = g_PlayerPos2.y - PLAYER_SIZE_Y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_PlayerPos2.x - PLAYER_SIZE_X / 2;
	pVtx[2].pos.y = g_PlayerPos2.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_PlayerPos2.x + PLAYER_SIZE_X / 2;
	pVtx[3].pos.y = g_PlayerPos2.y;
	pVtx[3].pos.z = 0.0f;

	//rhwの設定(1.0fで固定)
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
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffResult[1]->Unlock();


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult[2]->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(g_BlockPos2.x - (70.0f / 2), g_BlockPos2.y - (70.0f / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_BlockPos2.x + (70.0f / 2), g_BlockPos2.y - (70.0f / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_BlockPos2.x - (70.0f / 2), g_BlockPos2.y + (70.0f / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_BlockPos2.x + (70.0f / 2), g_BlockPos2.y + (70.0f / 2), 0.0f);

	//rhwの設定(1.0fで固定)
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
	g_pVtxBuffResult[2]->Unlock();

	//サウンド再生
	PlaySound(SOUND_LABEL_BGM002);
}

//======================================================
//		リザルト画面の終了処理
//======================================================
void UninitResult(void)
{
	//サウンド停止
	StopSound();

	for (int nCnt = 0; nCnt < MAX_RESULT; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTextureResult[nCnt] != NULL)
		{
			g_pTextureResult[nCnt]->Release();
			g_pTextureResult[nCnt] = NULL;
		}
		//頂点バッファの破棄
		if (g_pVtxBuffResult != NULL)
		{
			g_pVtxBuffResult[nCnt]->Release();
			g_pVtxBuffResult[nCnt] = NULL;
		}
	}
}

//======================================================
//		リザルト画面の更新処理
//======================================================
void UpdateResult(void)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	if (g_GameResultPush == false)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//エンターキーを押されたら
			//モード移行
			SetFade(MODE_TITLE);

			//リザルト演出の繰り返し防止
			g_GameResultPush = true;
		}
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult[1]->Lock(0, 0, (void**)&pVtx, 0);

	g_PlayerMove2.x -= sinf(-D3DX_PI * 0.5f)*(PLAYER_MOVE - 0.3f);
	g_PlayerMove2.y -= cosf(-D3DX_PI * 0.5f)*(PLAYER_MOVE - 0.3f);

	g_PlayerCounterAnim2++;		//カウンターを加算
	if (g_PlayerCounterAnim2 > 6)
	{
		g_PlayerCounterAnim2 = 0;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.25f * g_PlayerPatternAnim2, 0.5f * g_PlayerDirectionMove2);
		pVtx[1].tex = D3DXVECTOR2(0.25f * (g_PlayerPatternAnim2 + 1), 0.5f * g_PlayerDirectionMove2);
		pVtx[2].tex = D3DXVECTOR2(0.25f * g_PlayerPatternAnim2, 0.5f * (g_PlayerDirectionMove2 + 1));
		pVtx[3].tex = D3DXVECTOR2(0.25f * (g_PlayerPatternAnim2 + 1), 0.5f * (g_PlayerDirectionMove2 + 1));

		g_PlayerPatternAnim2++;

		if (g_PlayerPatternAnim2 >= 4)
		{//パターンNo.を初期値に戻す
			g_PlayerPatternAnim2 = 0;
		}
	}

	//位置更新
	g_PlayerPos2 += g_PlayerMove2;
	g_BlockPos2.x += 4.0f;

	//移動量更新(減衰)
	g_PlayerMove2.x += -g_PlayerMove2.x * 0.1f;			//(目的の値 - 現在の値) * 減衰係数

	//頂点座標の設定(中心座標は下辺の中心)
	pVtx[0].pos.x = g_PlayerPos2.x - PLAYER_SIZE_X / 2;
	pVtx[0].pos.y = g_PlayerPos2.y - PLAYER_SIZE_Y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_PlayerPos2.x + PLAYER_SIZE_X / 2;
	pVtx[1].pos.y = g_PlayerPos2.y - PLAYER_SIZE_Y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_PlayerPos2.x - PLAYER_SIZE_X / 2;
	pVtx[2].pos.y = g_PlayerPos2.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_PlayerPos2.x + PLAYER_SIZE_X / 2;
	pVtx[3].pos.y = g_PlayerPos2.y;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロック
	g_pVtxBuffResult[1]->Unlock();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult[2]->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(g_BlockPos2.x - (70.0f / 2), g_BlockPos2.y - (70.0f / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_BlockPos2.x + (70.0f / 2), g_BlockPos2.y - (70.0f / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_BlockPos2.x - (70.0f / 2), g_BlockPos2.y + (70.0f / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_BlockPos2.x + (70.0f / 2), g_BlockPos2.y + (70.0f / 2), 0.0f);

	//頂点バッファをアンロック
	g_pVtxBuffResult[2]->Unlock();
}

//======================================================
//		リザルト画面の描画処理
//======================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_RESULT; nCnt++)
	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffResult[nCnt], 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult[nCnt]);

		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,		//描画する最初の頂点インデックス
			2);		//プリミティブ（ポリゴン）数
	}
}