//-------------------------------------------
//
//	スコア処理
//	Author:橋野幹生
//
//-------------------------------------------
#include "main.h"
#include "score.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureScoreLogo = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScoreLogo = NULL;	//頂点バッファへのポインタ
D3DXVECTOR3 g_posScore[2];								//スコアの位置
D3DXVECTOR3 g_posScoreLogo;							//スコアロゴの位置
int g_nScore;										//スコアの値

//----------------------------------------
//  スコアの初期化設定処理
//----------------------------------------
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	pDevice = GetDevice();			//デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureScore);

	//ロゴテクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Score_rogo.png", &g_pTextureScoreLogo);


	for (int nCntScore = 0; nCntScore < 2; nCntScore++)
	{
		g_posScore[nCntScore] = D3DXVECTOR3(SCORE_SIZE_X * nCntScore + 1050.0f, 500.0f, 0.0f);
	}
	g_posScoreLogo = D3DXVECTOR3(1100.0f, 500.0f, 0.0f);	//スコアロゴの位置初期化
	g_nScore = 0;			//値の初期化

	//スコアの頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	//スコアロゴの頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScoreLogo,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	{//スコア
		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_posScore[nCnt].x - (SCORE_SIZE_X / 2.0f), g_posScore[nCnt].y - (SCORE_SIZE_Y / 2.0f), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posScore[nCnt].x + (SCORE_SIZE_X / 2.0f), g_posScore[nCnt].y - (SCORE_SIZE_Y / 2.0f), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posScore[nCnt].x - (SCORE_SIZE_X / 2.0f), g_posScore[nCnt].y + (SCORE_SIZE_Y / 2.0f), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posScore[nCnt].x + (SCORE_SIZE_X / 2.0f), g_posScore[nCnt].y + (SCORE_SIZE_Y / 2.0f), 0.0f);

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
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;  //頂点データのポインタを４つ分進める
		}
		//頂点バッファをアンロック
		g_pVtxBuffScore->Unlock();
	}

	{//スコアロゴ
		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffScoreLogo->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posScoreLogo.x - 250.0f, g_posScoreLogo.y - 120.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posScoreLogo.x + 250.0f, g_posScoreLogo.y - 120.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posScoreLogo.x - 250.0f, g_posScoreLogo.y + 120.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posScoreLogo.x + 250.0f, g_posScoreLogo.y + 120.0f, 0.0f);

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
		g_pVtxBuffScoreLogo->Unlock();
	}
}

//----------------------------------------
//　スコアの終了処理
//----------------------------------------
void UninitScore(void)
{
	{//スコア
		//テクスチャの破棄
		if (g_pTextureScore != NULL)
		{
			g_pTextureScore->Release();
			g_pTextureScore = NULL;
		}
		//頂点バッファの破棄
		if (g_pVtxBuffScore != NULL)
		{
			g_pVtxBuffScore->Release();
			g_pVtxBuffScore = NULL;
		}
	}

	{//スコアロゴ
		//テクスチャの破棄
		if (g_pTextureScoreLogo != NULL)
		{
			g_pTextureScoreLogo->Release();
			g_pTextureScoreLogo = NULL;
		}
		//頂点バッファの破棄
		if (g_pVtxBuffScoreLogo != NULL)
		{
			g_pVtxBuffScoreLogo->Release();
			g_pVtxBuffScoreLogo = NULL;
		}
	}
}

//----------------------------------------
//  プレイヤーの更新処理
//----------------------------------------
void UpdateScore(void)
{
	SetScore(g_nScore);
}

//----------------------------------------
//  スコアの描画処理
//----------------------------------------
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	{//スコアロゴの描画
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffScoreLogo, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureScoreLogo);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,		//描画する最初の頂点インデックス
			2);				//プリミティブ（ポリゴン）数
	}

	{//スコアの描画
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureScore);

		for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				4 * nCnt,		//描画する最初の頂点インデックス
				2);				//プリミティブ（ポリゴン）数
		}
	}
}

//----------------------------------------
//  スコアの設定処理
//----------------------------------------
void SetScore(int nScore)
{
	int aPosTexU[MAX_SCORE];		//各桁の数字を格納

	//スコアを代入
	g_nScore = nScore;

	if (g_nScore <= 0)
	{//スコアが0以下
		g_nScore = 0;		//0に戻す
	}

	aPosTexU[0] = g_nScore % 100 / 10;
	aPosTexU[1] = g_nScore % 10 / 1;

	VERTEX_2D *pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCnt], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCnt] + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCnt], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCnt] + 1), 1.0f);

		pVtx += 4;  //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffScore->Unlock();
}

//----------------------------------------
//  スコアの加算処理
//----------------------------------------
void AddScore(int nValue)
{
	//スコアに値追加
	g_nScore += nValue;
}

//----------------------------------------
//  スコア情報取得
//----------------------------------------
int GetScore(void)
{
	return g_nScore;
}