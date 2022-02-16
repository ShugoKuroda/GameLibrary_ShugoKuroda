#include "main.h"
#include "time.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;		//頂点バッファへのポインタ
Time g_Time;

//===============================================
//		スコアの初期化処理
//===============================================
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	pDevice = GetDevice();			//デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureTime);

	g_Time.pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f - 40.0f, 0.0f, 0.0f);		//位置の初期化
	g_Time.nData = 0;			//値の初期化

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_TIME; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Time.pos.x + TIME_SIZE_X * nCnt, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Time.pos.x + TIME_SIZE_X * (nCnt + 1), 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Time.pos.x + TIME_SIZE_X * nCnt, TIME_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Time.pos.x + TIME_SIZE_X * (nCnt + 1), TIME_SIZE_Y, 0.0f);

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
	g_pVtxBuffTime->Unlock();
}

//======================================================
//		スコアの終了処理
//======================================================
void UninitTime(void)
{
	//テクスチャの破棄
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

//======================================================
//		スコアの更新処理
//======================================================
void UpdateTime(void)
{
	/*ここに更新処理を入れる*/
}

//======================================================
//		スコアの描画処理
//======================================================
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTime);

	for (int nCnt = 0; nCnt < MAX_TIME; nCnt++)
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
void SetTime(int nScore)
{
	int aPosTexU[MAX_TIME];		//各桁の数字を格納

	g_Time.nData = nScore;

	aPosTexU[0] = g_Time.nData % 100000000 / 10000000;
	aPosTexU[1] = g_Time.nData % 10000000 / 1000000;
	aPosTexU[2] = g_Time.nData % 1000000 / 100000;
	aPosTexU[3] = g_Time.nData % 100000 / 10000;
	aPosTexU[4] = g_Time.nData % 10000 / 1000;
	aPosTexU[5] = g_Time.nData % 1000 / 100;
	aPosTexU[6] = g_Time.nData % 100 / 10;
	aPosTexU[7] = g_Time.nData % 10 / 1;

	VERTEX_2D *pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_TIME; nCnt++)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCnt], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCnt] + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCnt], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCnt] + 1), 1.0f);

		pVtx += 4;  //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffTime->Unlock();
}

//======================================================
//		スコアの加算処理
//======================================================
void AddTime(int nValue)
{
	/*ここに加算処理を入れる*/
}

//======================================================
//		スコア情報を取得
//======================================================
int GetTime(void)
{
	return g_Time.nData;
}