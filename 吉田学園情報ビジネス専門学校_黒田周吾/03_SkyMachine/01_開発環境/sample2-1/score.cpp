#include "main.h"
#include "score.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		//頂点バッファへのポインタ
D3DXVECTOR3 g_posScore;								//スコアの位置
int g_nScore;										//スコアの値

//===============================================
//		スコアの初期化処理
//===============================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	pDevice = GetDevice();			//デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureScore);

	g_posScore = D3DXVECTOR3(SCREEN_WIDTH / 2.0f - 40.0f, 0.0f, 0.0f);		//位置の初期化
	g_nScore = 0;			//値の初期化

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posScore.x + SCORE_SIZE_X * nCnt, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posScore.x + SCORE_SIZE_X * (nCnt + 1), 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posScore.x + SCORE_SIZE_X * nCnt, SCORE_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posScore.x + SCORE_SIZE_X * (nCnt + 1), SCORE_SIZE_Y, 0.0f);

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

//======================================================
//		スコアの終了処理
//======================================================
void UninitScore(void)
{
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

//======================================================
//		スコアの更新処理
//======================================================
void UpdateScore(void)
{
	/*ここに更新処理を入れる*/
}

//======================================================
//		スコアの描画処理
//======================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

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

//======================================================
//		スコアの設定処理
//======================================================
void SetScore(int nScore)
{
	int aPosTexU[MAX_SCORE];		//各桁の数字を格納

	g_nScore = nScore;

	aPosTexU[0] = g_nScore % 100000000 / 10000000;
	aPosTexU[1] = g_nScore % 10000000 / 1000000;
	aPosTexU[2] = g_nScore % 1000000 / 100000;
	aPosTexU[3] = g_nScore % 100000 / 10000;
	aPosTexU[4] = g_nScore % 10000 / 1000;
	aPosTexU[5] = g_nScore % 1000 / 100;
	aPosTexU[6] = g_nScore % 100 / 10;
	aPosTexU[7] = g_nScore % 10 / 1;

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

//======================================================
//		スコアの加算処理
//======================================================
void AddScore(int nValue)
{
	/*ここに加算処理を入れる*/
}

//======================================================
//		スコア情報を取得
//======================================================
int GetScore(void)
{
	return g_nScore;
}