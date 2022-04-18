#include "main.h"
#include "result.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "score.h"
#include "game.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_RESULT] = { NULL };	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult[MAX_RESULT] = {};	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureResultScore = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultScore = NULL;		//頂点バッファへのポインタ
bool g_GameResultPush;				//リザルト演出の繰り返し防止
D3DXVECTOR3 g_resultpos;			//リザルトの位置
D3DXVECTOR3 g_resultscorepos[2];		//リザルトスコアの位置
int g_nResultScore;

//======================================================
//		リザルト画面の初期化処理
//======================================================
void InitResult(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Result.jpg",
		&g_pTextureResult[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/You_Score.png",
		&g_pTextureResult[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number000.png",
		&g_pTextureResultScore);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult[0],
		NULL);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult[1],
		NULL);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultScore,
		NULL);

	g_GameResultPush = false;			//終了演出繰り返し防止の初期化
	g_resultpos = D3DXVECTOR3((SCREEN_WIDTH / 2.0f), 500.0f, 0.0f);
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		g_resultscorepos[nCnt] = D3DXVECTOR3(SCORE_SIZE_X * nCnt + (SCREEN_WIDTH / 2.0f) - (SCORE_SIZE_X / 2.0f), 500.0f, 0.0f);
	}

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

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_resultpos.x - (SCREEN_WIDTH / 2.0f), g_resultpos.y - (SCREEN_HEIGHT / 2.0f), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_resultpos.x + (SCREEN_WIDTH / 2.0f), g_resultpos.y - (SCREEN_HEIGHT / 2.0f), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_resultpos.x - (SCREEN_WIDTH / 2.0f), g_resultpos.y + (SCREEN_HEIGHT / 2.0f), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_resultpos.x + (SCREEN_WIDTH / 2.0f), g_resultpos.y + (SCREEN_HEIGHT / 2.0f), 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffResult[1]->Unlock();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	//スコア設定
	int nResultScore = GetScore();

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_resultscorepos[nCnt].x - (SCORE_SIZE_X / 2.0f), g_resultscorepos[nCnt].y - (SCORE_SIZE_Y / 2.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_resultscorepos[nCnt].x + (SCORE_SIZE_X / 2.0f), g_resultscorepos[nCnt].y - (SCORE_SIZE_Y / 2.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_resultscorepos[nCnt].x - (SCORE_SIZE_X / 2.0f), g_resultscorepos[nCnt].y + (SCORE_SIZE_Y / 2.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_resultscorepos[nCnt].x + (SCORE_SIZE_X / 2.0f), g_resultscorepos[nCnt].y + (SCORE_SIZE_Y / 2.0f), 0.0f);

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

		int aPosTexU[MAX_SCORE];		//各桁の数字を格納

		aPosTexU[0] = nResultScore % 100 / 10;
		aPosTexU[1] = nResultScore % 10 / 1;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCnt], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCnt] + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCnt], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCnt] + 1), 1.0f);

		pVtx += 4;  //頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロック
	g_pVtxBuffResultScore->Unlock();

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

	//テクスチャの破棄
	if (g_pTextureResultScore != NULL)
	{
		g_pTextureResultScore->Release();
		g_pTextureResultScore = NULL;
	}

	for (int nCnt = 0; nCnt < MAX_RESULT; nCnt++)
	{
		//頂点バッファの破棄
		if (g_pVtxBuffResult[nCnt] != NULL)
		{
			g_pVtxBuffResult[nCnt]->Release();
			g_pVtxBuffResult[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffResultScore != NULL)
	{
		g_pVtxBuffResultScore->Release();
		g_pVtxBuffResultScore = NULL;
	}
}

//======================================================
//		リザルト画面の更新処理
//======================================================
void UpdateResult(void)
{
	if (g_GameResultPush == false)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A, PLAYER_1) == true || GetJoypadTrigger(JOYKEY_START, PLAYER_1) == true)
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

	{//リザルトスコアの描画
	 //頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffResultScore, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResultScore);

		for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				4 * nCnt,		//描画する最初の頂点インデックス
				2);				//プリミティブ（ポリゴン）数
		}
	}
}