//===================================================================
//
//	ランキング処理[rank.cpp]
//	Author:黒田 周吾
//
//===================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include "rank.h"
#include "score.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include <stdio.h>

//======================================================
//	グローバル変数
//======================================================
LPDIRECT3DTEXTURE9 g_pTextureRank[MAX_RANK_IMAGE] = { NULL };			//テクスチャへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureRank2 = NULL;								//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank[MAX_RANK_IMAGE] = { NULL };		//頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank2 = NULL;							//頂点バッファへのポインタ
D3DXVECTOR3	g_posRankNum;			//ランキング順位の表示位置
D3DXVECTOR3	g_posRankRogo;			//ランキングロゴの表示位置
RankScore g_aRankScore[MAX_RANK];	//ランキングスコアの情報
int g_nPlayerRank;					//プレイヤーの順位
bool g_bGameEndPush;				//ゲーム終了演出の繰り返し防止

//======================================================
//	ランキングの初期化処理
//======================================================
void InitRank(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bg001.jpg",		//背景のテクスチャ
		&g_pTextureRank[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/rank000.png",		//ランキング順位表示のテクスチャ
		&g_pTextureRank[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/rank001.png",		//ランキングロゴのテクスチャ
		&g_pTextureRank[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/You000.png",		//Youのテクスチャ
		&g_pTextureRank[3]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number000.png",	//スコア表示のテクスチャ
		&g_pTextureRank2);

	//頂点バッファの生成
	for (int nCnt = 0; nCnt < MAX_RANK_IMAGE; nCnt++)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffRank[nCnt],
			NULL);
	}
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUM * MAX_RANK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRank2,
		NULL);

	//テクスチャ表示位置の初期化
	g_posRankNum = D3DXVECTOR3(SCREEN_WIDTH / 3.2f, SCREEN_HEIGHT / 2.05f, 0.0f);
	g_posRankRogo = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 50.0f, 0.0f);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank[0]->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffRank[0]->Unlock();


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank[1]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_posRankNum.x - 70.0f, g_posRankNum.y - 250.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posRankNum.x + 70.0f, g_posRankNum.y - 250.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posRankNum.x - 70.0f, g_posRankNum.y + 250.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posRankNum.x + 70.0f, g_posRankNum.y + 250.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffRank[1]->Unlock();


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank[2]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_posRankRogo.x - 300.0f, g_posRankRogo.y - 40.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posRankRogo.x + 300.0f, g_posRankRogo.y - 40.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posRankRogo.x - 300.0f, g_posRankRogo.y + 40.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posRankRogo.x + 300.0f, g_posRankRogo.y + 40.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffRank[2]->Unlock();


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank2->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt1 = 0; nCnt1 < MAX_RANK; nCnt1++)
	{
		for (int nCnt2 = 0; nCnt2 < MAX_NUM; nCnt2++)
		{
			g_aRankScore[nCnt1].pos = D3DXVECTOR3(SCREEN_WIDTH + 150.0f, SCREEN_HEIGHT / 4.5f, 0.0f); //スコアの場所を初期化

			 //頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aRankScore[nCnt1].pos.x + (35.0f * nCnt2) - 15.0f, g_aRankScore[nCnt1].pos.y + (90.0f * nCnt1) - 45.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aRankScore[nCnt1].pos.x + (35.0f * nCnt2) + 15.0f, g_aRankScore[nCnt1].pos.y + (90.0f * nCnt1) - 45.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aRankScore[nCnt1].pos.x + (35.0f * nCnt2) - 15.0f, g_aRankScore[nCnt1].pos.y + (90.0f * nCnt1) + 45.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aRankScore[nCnt1].pos.x + (35.0f * nCnt2) + 15.0f, g_aRankScore[nCnt1].pos.y + (90.0f * nCnt1) + 45.0f, 0.0f);

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
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;  //頂点データのポインタを４つ分進める
		}
	}

	//頂点バッファをアンロック
	g_pVtxBuffRank2->Unlock();

	//ゲーム終了演出防止の初期化
	g_bGameEndPush = false;

	//ランキングの数値をロード
	LoadScore();

	//プレイヤーのスコア参照
	g_aRankScore[5].nScore = GetScore();

	//ランキングの更新
	int nData = 0;
	for (int nCnt1 = 0; nCnt1 < MAX_RANK - 1; nCnt1++)
	{
		for (int nCnt2 = (nCnt1 + 1); nCnt2 < MAX_RANK; nCnt2++)
		{
			if (g_aRankScore[nCnt1].nScore <= g_aRankScore[nCnt2].nScore)
			{
				nData = g_aRankScore[nCnt2].nScore;
				g_aRankScore[nCnt2].nScore = g_aRankScore[nCnt1].nScore;
				g_aRankScore[nCnt1].nScore = nData;
			}
		}
	}

	//過去ランキングのスコア参照
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		SetRank(g_aRankScore[nCnt].nScore, nCnt);
	}

	//ランキングのセーブ
	SaveScore();

	//プレイヤーのスコア順位のチェック
	g_nPlayerRank = 0;

	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		if (g_aRankScore[nCnt].nScore == GetScore())
		{
			g_nPlayerRank = nCnt;
		}
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank[3]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aRankScore[g_nPlayerRank].pos.x - 80.0f - 260.0f, g_aRankScore[g_nPlayerRank].pos.y + (91.0f * g_nPlayerRank) - 40.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aRankScore[g_nPlayerRank].pos.x + 80.0f - 260.0f, g_aRankScore[g_nPlayerRank].pos.y + (91.0f * g_nPlayerRank) - 40.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aRankScore[g_nPlayerRank].pos.x - 80.0f - 260.0f, g_aRankScore[g_nPlayerRank].pos.y + (91.0f * g_nPlayerRank) + 40.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aRankScore[g_nPlayerRank].pos.x + 80.0f - 260.0f, g_aRankScore[g_nPlayerRank].pos.y + (91.0f * g_nPlayerRank) + 40.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffRank[3]->Unlock();

	//サウンド再生
	PlaySound(SOUND_LABEL_BGM002);
}

//======================================================
//	ランキングの終了処理
//======================================================
void UninitRank(void)
{
	//サウンド停止
	StopSound();

	for (int nCnt = 0; nCnt < MAX_RANK_IMAGE; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTextureRank[nCnt] != NULL)
		{
			g_pTextureRank[nCnt]->Release();
			g_pTextureRank[nCnt] = NULL;
		}

		//頂点バッファの破棄
		if (g_pVtxBuffRank[nCnt] != NULL)
		{
			g_pVtxBuffRank[nCnt]->Release();
			g_pVtxBuffRank[nCnt] = NULL;
		}
	}
	//テクスチャの破棄
	if (g_pTextureRank2 != NULL)
	{
		g_pTextureRank2->Release();
		g_pTextureRank2 = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffRank2 != NULL)
	{
		g_pVtxBuffRank2->Release();
		g_pVtxBuffRank2 = NULL;
	}
}

//======================================================
//	ランキングの更新処理
//======================================================
void UpdateRank(void)
{
	if (g_bGameEndPush == false)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//エンターキーを押した場合
			if (g_aRankScore[0].pos.x <= SCREEN_WIDTH / 2.5f)
			{
				//モード移行
				SetFade(MODE_TITLE);

				//リザルト演出の繰り返し防止
				g_bGameEndPush = true;
			}
			if (g_aRankScore[0].pos.x > SCREEN_WIDTH / 2.5f)
			{
				for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
				{
					g_aRankScore[nCnt].pos.x = SCREEN_WIDTH / 2.5f;
				}
			}
		}
	}

	//ランキングスコアの移動処理
	if (g_aRankScore[0].pos.x > SCREEN_WIDTH / 2.5f)
	{
		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			g_aRankScore[nCnt].pos.x -= 10.0f;
		}
	}

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank[3]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aRankScore[g_nPlayerRank].pos.x - 80.0f - 260.0f, g_aRankScore[g_nPlayerRank].pos.y + (100.0f * g_nPlayerRank) - 40.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aRankScore[g_nPlayerRank].pos.x + 80.0f - 260.0f, g_aRankScore[g_nPlayerRank].pos.y + (100.0f * g_nPlayerRank) - 40.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aRankScore[g_nPlayerRank].pos.x - 80.0f - 260.0f, g_aRankScore[g_nPlayerRank].pos.y + (100.0f * g_nPlayerRank) + 40.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aRankScore[g_nPlayerRank].pos.x + 80.0f - 260.0f, g_aRankScore[g_nPlayerRank].pos.y + (100.0f * g_nPlayerRank) + 40.0f, 0.0f);

	//頂点バッファをアンロック
	g_pVtxBuffRank[3]->Unlock();


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank2->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt1 = 0; nCnt1 < MAX_RANK; nCnt1++)
	{
		for (int nCnt2 = 0; nCnt2 < MAX_NUM; nCnt2++)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aRankScore[nCnt1].pos.x + (40.0f * nCnt2) - 20.0f, g_aRankScore[nCnt1].pos.y + (100.0f * nCnt1) - 50.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aRankScore[nCnt1].pos.x + (40.0f * nCnt2) + 20.0f, g_aRankScore[nCnt1].pos.y + (100.0f * nCnt1) - 50.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aRankScore[nCnt1].pos.x + (40.0f * nCnt2) - 20.0f, g_aRankScore[nCnt1].pos.y + (100.0f * nCnt1) + 50.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aRankScore[nCnt1].pos.x + (40.0f * nCnt2) + 20.0f, g_aRankScore[nCnt1].pos.y + (100.0f * nCnt1) + 50.0f, 0.0f);

			pVtx += 4;  //頂点データのポインタを４つ分進める
		}
	}
	//頂点バッファをアンロック
	g_pVtxBuffRank2->Unlock();
}

//======================================================
//	ランキングの描画処理
//======================================================
void DrawRank(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_RANK_IMAGE; nCnt++)
	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffRank[nCnt], 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureRank[nCnt]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,		//描画する最初の頂点インデックス
			2);		//プリミティブ（ポリゴン）数
	}


	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRank2, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRank2);

	for (int nCnt = 0; nCnt < MAX_RANK * MAX_NUM; nCnt++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			4 * nCnt,		//描画する最初の頂点インデックス
			2);				//プリミティブ（ポリゴン）数
	}
}

//======================================================
//	ランキングの設定処理
//======================================================
void SetRank(int nScore, int nCntScore)
{
	int aPosTexU[MAX_NUM];	//各位の数字を格納

	aPosTexU[0] = nScore % 100000000 / 10000000;
	aPosTexU[1] = nScore % 10000000 / 1000000;
	aPosTexU[2] = nScore % 1000000 / 100000;
	aPosTexU[3] = nScore % 100000 / 10000;
	aPosTexU[4] = nScore % 10000 / 1000;
	aPosTexU[5] = nScore % 1000 / 100;
	aPosTexU[6] = nScore % 100 / 10;
	aPosTexU[7] = nScore % 10 / 1;

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank2->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * MAX_NUM * nCntScore;  //頂点データのポインタを４つ分進める

	for (int nCnt = 0; nCnt < MAX_NUM; nCnt++)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCnt], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCnt] + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCnt], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCnt] + 1), 1.0f);

		pVtx += 4;  //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffRank2->Unlock();
}

//======================================================
//	ランキングのロード処理
//======================================================
void LoadScore(void)
{
	FILE *pFile;			//ファイルポインター宣言

	//ファイルを開く
	pFile = fopen("Rank.txt", "r");

	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < 5; nCnt++)
		{
			fscanf(pFile, "%d", &g_aRankScore[nCnt].nScore);
		}
	}
	else
	{
		printf("ファイルが開けませんでした\n");
	}

	//ファイルを閉じる
	fclose(pFile);
}

//======================================================
//	ランキングのセーブ処理
//======================================================
void SaveScore(void)
{
	FILE *pFile;			//ファイルポインター宣言

	//ファイルを開く
	pFile = fopen("Rank.txt", "w");

	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < 5; nCnt++)
		{
			fprintf(pFile, "%d\n", g_aRankScore[nCnt].nScore);
		}
	}
	else
	{
		printf("ファイルが開けませんでした\n");
	}

	//ファイルを閉じる
	fclose(pFile);
}