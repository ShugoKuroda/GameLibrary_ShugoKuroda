//========================================================
//
//					得点時のSetカウント処理
//					AUTHOR:黒田周吾
//
//========================================================
#include "player.h"
#include "set.h"
//#include "game.h"
//#include "result.h"
//#include "fade.h"

//マクロ定義
#define NUM_SET		(2)			//得点の桁

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureSet = NULL;			//得点のテクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSet = NULL;		//得点の頂点情報へのポインタ
Set g_aSet[MAX_PLAYER];							//得点の情報を格納
bool g_bUseSet;				//得点の表示・非表示を管理
int g_nCntSet;				//セットカウンターの表示時間
	
//========================================================
//					得点の初期化処理
//========================================================
void InitSet(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureSet);

	//グローバル変数の初期化
	g_bUseSet = false;						//表示していない状態にする
	g_nCntSet = 0;
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_aSet[nCntPlayer].nScore = 0;							//スコアの初期化
		g_aSet[nCntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置の初期化
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (NUM_SET * MAX_PLAYER),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSet,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSet->Lock(0, 0, (void**)&pVtx, 0);

	//プレイヤー[1]の得点
	for (int nCntScore = 0; nCntScore < NUM_SET; nCntScore++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(320.0f + (80.0f * nCntScore), 450.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(400.0f + (80.0f * nCntScore), 450.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(320.0f + (80.0f * nCntScore), 630.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(400.0f + (80.0f * nCntScore), 630.0f, 0.0f);

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

		//テクスチャの座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	//プレイヤー[2]の得点
	for (int nCntScore = 0; nCntScore < NUM_SET; nCntScore++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(800.0f + (80.0f * nCntScore), 450.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(880.0f + (80.0f * nCntScore), 450.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(800.0f + (80.0f * nCntScore), 630.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(880.0f + (80.0f * nCntScore), 630.0f, 0.0f);

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

		//テクスチャの座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	//頂点座標をアンロック
	g_pVtxBuffSet->Unlock();
}

//========================================================
//					得点の終了処理
//========================================================
void UninitSet(void)
{
	//テクスチャの破棄
	if (g_pTextureSet != NULL)
	{
		g_pTextureSet->Release();
		g_pTextureSet = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffSet != NULL)
	{
		g_pVtxBuffSet->Release();
		g_pVtxBuffSet = NULL;
	}
}

//========================================================
//					得点の更新処理
//========================================================
void UpdateSet(void)
{
	if (g_bUseSet)
	{
		g_nCntSet++;
		if (g_nCntSet >= 150)
		{
			g_nCntSet = 0;
			g_bUseSet = false;
		}
	}
}

//========================================================
//					得点の描画処理
//========================================================
void DrawSet(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSet, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntScore = 0; nCntScore < NUM_SET * MAX_PLAYER; nCntScore++)
	{
		if (g_bUseSet == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureSet);

			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,								//プリミティブの種類
				4 * nCntScore,
				2);													//プリミティブの数	
		}
	}
}

//========================================================
//				Set得点の設定処理
//========================================================
void SetCounter(int nIdxPlayer)
{
	VERTEX_2D *pVtx;
	float aPosTexU[MAX_PLAYER][NUM_SET];							//各桁の数値を格納

	g_bUseSet = true;		//スコアを使用状態にする

	if (nIdxPlayer == 0)
	{
		g_aSet[PLAYER_1].nScore++;
	}
	else if (nIdxPlayer == 1)
	{
		g_aSet[PLAYER_2].nScore++;
	}

	//プレイヤー[1]
	aPosTexU[0][0] = ((g_aSet[0].nScore % 100) / 10) * 0.1f;
	aPosTexU[0][1] = (g_aSet[0].nScore % 10) * 0.1f;

	//プレイヤー[2]
	aPosTexU[1][0] = ((g_aSet[1].nScore % 100) / 10) * 0.1f;
	aPosTexU[1][1] = (g_aSet[1].nScore % 10) * 0.1f;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSet->Lock(0, 0, (void**)&pVtx, 0);

	//プレイヤー[1]の得点
	for (int nCntScore = 0; nCntScore < NUM_SET; nCntScore++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + aPosTexU[0][nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aPosTexU[0][nCntScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aPosTexU[0][nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aPosTexU[0][nCntScore], 1.0f);

		pVtx += 4;
	}

	//プレイヤー[2]の得点
	for (int nCntScore = 0; nCntScore < NUM_SET; nCntScore++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + aPosTexU[1][nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aPosTexU[1][nCntScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aPosTexU[1][nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aPosTexU[1][nCntScore], 1.0f);

		pVtx += 4;
	}

	//頂点座標をアンロック
	g_pVtxBuffSet->Unlock();
}

//========================================================
//					リザルトタイムの表示処理
//========================================================
//void DrawResultTime(int nResultTime)
//{
//	VERTEX_2D *pVtx;
//	float aPosTexU[NUM_SCORE];									//各桁の数値を収納
//
//	//各桁の数値を計算
//	aPosTexU[0] = ((nResultTime % 1000) / 100) * 0.1f;
//	aPosTexU[1] = ((nResultTime % 100) / 10) * 0.1f;
//	aPosTexU[2] = (nResultTime % 10) * 0.1f;
//
//	//頂点バッファをロックし、頂点情報へのポインタを取得
//	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (int nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
//	{
//		//頂点座標の設定
//		pVtx[0].pos = D3DXVECTOR3(570.0f + (60.0f * nCntScore), 220.0f, 0.0f);
//		pVtx[1].pos = D3DXVECTOR3(630.0f + (60.0f * nCntScore), 220.0f, 0.0f);
//		pVtx[2].pos = D3DXVECTOR3(570.0f + (60.0f * nCntScore), 300.0f, 0.0f);
//		pVtx[3].pos = D3DXVECTOR3(630.0f + (60.0f * nCntScore), 300.0f, 0.0f);
//
//		//頂点カラーの設定
//		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//		//テクスチャの設定
//		pVtx[0].tex = D3DXVECTOR2(0.0f + aPosTexU[nCntScore], 0.0f);
//		pVtx[1].tex = D3DXVECTOR2(0.1f + aPosTexU[nCntScore], 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(0.0f + aPosTexU[nCntScore], 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(0.1f + aPosTexU[nCntScore], 1.0f);
//
//		pVtx += 4;
//	}
//
//	//頂点座標をアンロック
//	g_pVtxBuffScore->Unlock();
//
//	DrawScore();
//}