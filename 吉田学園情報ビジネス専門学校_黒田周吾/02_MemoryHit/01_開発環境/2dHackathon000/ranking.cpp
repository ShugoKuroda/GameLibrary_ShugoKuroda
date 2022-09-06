//-------------------------------------------
//
//ランキング処理[ranking.cpp]
//Author:平野舞
//
//-------------------------------------------

//インクルードファイル
#include "ranking.h"
#include "input.h"
#include <stdio.h>
#include "fade.h"
#include "score.h"
#include "game.h"
#include "sound.h"

//マクロ定義
#define MAX_RANKY				(5)			//Yの数
#define RANKING_WIDTH			(50)		//順位の幅
#define RANKING_HEIGHT			(100)		//順位の高さ
#define MAX_TEX					(2)			//画像の枚数

//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTextureRank[MAX_TEX] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank[MAX_TEX] = {};		//頂点バッファへのポインタ
Rank g_RankScore[MAX_RANKY];								//ランキングスコア情報
bool g_bRankingPush;		//ランキング演出の繰り返し防止

//-------------------------------------------
//初期化処理
//-------------------------------------------
void InitRanking(void)
{
	//スコア構造体の初期化
	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++)
		{
			g_RankScore[nCntY].pos[nCntX] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		}
		g_RankScore[nCntY].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//カラー
		g_RankScore[nCntY].fWidth = 0.0f;								//幅間隔
		g_RankScore[nCntY].fHeigth = 0.0f;								//縦間隔
	}

	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	g_bRankingPush = false;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ranking000.jpg",
		&g_pTextureRank[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number000.png",
		&g_pTextureRank[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 ,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRank[0],
		NULL);

		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANKX * 5,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffRank[1],
			NULL);

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//----------------------------背景------------------------------------

	//頂点バッファをロックし、頂点情報へのポインタを取得
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
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffRank[0]->Unlock();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank[1]->Lock(0, 0, (void**)&pVtx, 0);

	//----------------------------数値------------------------------------
	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_RankScore[nCntY].pos[nCntX].x - (RANKING_WIDTH / 2.0f) + (nCntX * g_RankScore[nCntY].fWidth), g_RankScore[nCntY].pos[nCntX].y - (RANKING_HEIGHT / 2.0f) + (nCntY * g_RankScore[nCntY].fHeigth), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_RankScore[nCntY].pos[nCntX].x + (RANKING_WIDTH / 2.0f) + (nCntX * g_RankScore[nCntY].fWidth), g_RankScore[nCntY].pos[nCntX].y - (RANKING_HEIGHT / 2.0f) + (nCntY * g_RankScore[nCntY].fHeigth), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_RankScore[nCntY].pos[nCntX].x - (RANKING_WIDTH / 2.0f) + (nCntX * g_RankScore[nCntY].fWidth), g_RankScore[nCntY].pos[nCntX].y + (RANKING_HEIGHT / 2.0f) + (nCntY * g_RankScore[nCntY].fHeigth), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_RankScore[nCntY].pos[nCntX].x + (RANKING_WIDTH / 2.0f) + (nCntX * g_RankScore[nCntY].fWidth), g_RankScore[nCntY].pos[nCntX].y + (RANKING_HEIGHT / 2.0f) + (nCntY * g_RankScore[nCntY].fHeigth), 0.0f);

			//rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(g_RankScore[nCntY].color.r, g_RankScore[nCntY].color.g, g_RankScore[nCntY].color.b, g_RankScore[nCntY].color.a);
			pVtx[1].col = D3DXCOLOR(g_RankScore[nCntY].color.r, g_RankScore[nCntY].color.g, g_RankScore[nCntY].color.b, g_RankScore[nCntY].color.a);
			pVtx[2].col = D3DXCOLOR(g_RankScore[nCntY].color.r, g_RankScore[nCntY].color.g, g_RankScore[nCntY].color.b, g_RankScore[nCntY].color.a);
			pVtx[3].col = D3DXCOLOR(g_RankScore[nCntY].color.r, g_RankScore[nCntY].color.g, g_RankScore[nCntY].color.b, g_RankScore[nCntY].color.a);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;		//頂点データを４つ分進める
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRank[1]->Unlock();

	//ランキング設定
	SetRanking(D3DXVECTOR3(400.0f,100.0f,0.0f), D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),70.0f,140.0f);
}

//-------------------------------------------
//終了処理
//-------------------------------------------
void UninitRanking(void)
{
	//サウンド停止
	StopSound();

	for (int nCount = 0; nCount < MAX_TEX; nCount++)
	{
		//テクスチャの破棄
		if (g_pTextureRank[nCount] != NULL)
		{				 
			g_pTextureRank[nCount]->Release();
			g_pTextureRank[nCount] = NULL;
		}
	}

	for (int nCount = 0; nCount < MAX_TEX; nCount++)
	{
		//頂点バッファの破棄
		if (g_pVtxBuffRank[nCount] != NULL)
		{				 
			g_pVtxBuffRank[nCount]->Release();
			g_pVtxBuffRank[nCount] = NULL;
		}
	}
}

//-------------------------------------------
//更新処理
//-------------------------------------------
void UpdateRanking(void)
{
	if (g_bRankingPush == false)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A, PLAYER_1) == true || GetJoypadTrigger(JOYKEY_START, PLAYER_1) == true)
		{//ENTERキーを押したら

			//モード設定
			SetFade(MODE_TITLE);

			g_bRankingPush = true;
		}
	}
}

//-------------------------------------------
//描画処理
//-------------------------------------------
void DrawRanking(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRank[0], 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャ設定
	pDevice->SetTexture(0, g_pTextureRank[0]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRank[1], 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	int nCount = 0;

	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++, nCount++)
		{
			//テクスチャ設定
			pDevice->SetTexture(0, g_pTextureRank[1]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
}

//-------------------------------------------
//ランキングのリセット
//-------------------------------------------
void ResetRanking(void)
{
	FILE * pFile;		//ファイルポインタ

	//ファイルを開く
	pFile = fopen("data/Ranking.txt", "r");
	if(pFile != NULL)
	{//ファイルが開けた場合
		//ファイルからランキングを読み込む
		for (int nCount = 0; nCount < MAX_RANKY; nCount++)
		{
			fscanf(pFile, "%d\n", &g_RankScore[nCount].nScore);
		}
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("ファイルが開けませんでした。");
	}
}

//-------------------------------------------
//ランキングの設定
//
//D3DXVECTOR3 pos → 位置を指定
//D3DXCOLOR color → 色を指定
//float width	  → 横幅の数字間隔を指定
//float heigth	  → 縦の数字間隔を指定
//
//-------------------------------------------
void SetRanking(D3DXVECTOR3 pos, D3DXCOLOR color, float width, float heigth)
{
	int aPosTexU[MAX_RANKY][MAX_RANKX];			//各桁の数字を格納

	//###############################################################
	//↓ここに獲得したスコア等を入れる
	//################################################################

	g_RankScore[5].nScore = GetScore();

	//バブルソート
	for (int nCnt = 0; nCnt < MAX_RANKY; nCnt++)
	{
		for (int nCnt1 = nCnt + 1; nCnt1 < MAX_RANKY + 1; nCnt1++)
		{
			if (g_RankScore[nCnt].nScore < g_RankScore[nCnt1].nScore)
			{
				int nData = g_RankScore[nCnt].nScore;
				g_RankScore[nCnt].nScore = g_RankScore[nCnt1].nScore;
				g_RankScore[nCnt1].nScore = nData;
			}
		}
	}

	FILE * pFile;			//ファイルポインタを宣言

	//ランキング結果をファイルに書き出す
	pFile = fopen("data/Ranking.txt", "w");
	if (pFile != NULL)
	{
		for (int nCount = 0; nCount < MAX_RANKY; nCount++)
		{
			//ファイルにランキングを書き出す
			fprintf(pFile, "%d\n", g_RankScore[nCount].nScore);
		}
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("ファイルが開けませんでした。");
	}

	//テクスチャ設定
	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++)
		{
			aPosTexU[nCntY][0] = g_RankScore[nCntY].nScore % 100 / 10;
			aPosTexU[nCntY][1] = g_RankScore[nCntY].nScore % 10 / 1;
		}
	}

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank[1]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++)
		{
			g_RankScore[nCntY].pos[nCntX] = pos;	//位置
			g_RankScore[nCntY].color = color;		//カラー
			g_RankScore[nCntY].fWidth = width;		//幅間隔
			g_RankScore[nCntY].fHeigth = heigth;	//縦間隔

			//頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(g_RankScore[nCntY].pos[nCntX].x - (RANKING_WIDTH / 2.0f) + (nCntX * g_RankScore[nCntY].fWidth), g_RankScore[nCntY].pos[nCntX].y - (RANKING_HEIGHT / 2.0f) + (nCntY * g_RankScore[nCntY].fHeigth), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_RankScore[nCntY].pos[nCntX].x + (RANKING_WIDTH / 2.0f) + (nCntX * g_RankScore[nCntY].fWidth), g_RankScore[nCntY].pos[nCntX].y - (RANKING_HEIGHT / 2.0f) + (nCntY * g_RankScore[nCntY].fHeigth), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_RankScore[nCntY].pos[nCntX].x - (RANKING_WIDTH / 2.0f) + (nCntX * g_RankScore[nCntY].fWidth), g_RankScore[nCntY].pos[nCntX].y + (RANKING_HEIGHT / 2.0f) + (nCntY * g_RankScore[nCntY].fHeigth), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_RankScore[nCntY].pos[nCntX].x + (RANKING_WIDTH / 2.0f) + (nCntX * g_RankScore[nCntY].fWidth), g_RankScore[nCntY].pos[nCntX].y + (RANKING_HEIGHT / 2.0f) + (nCntY * g_RankScore[nCntY].fHeigth), 0.0f);

			//頂点カラーの更新
			pVtx[0].col = D3DXCOLOR(g_RankScore[nCntY].color.r, g_RankScore[nCntY].color.g, g_RankScore[nCntY].color.b, g_RankScore[nCntY].color.a);
			pVtx[1].col = D3DXCOLOR(g_RankScore[nCntY].color.r, g_RankScore[nCntY].color.g, g_RankScore[nCntY].color.b, g_RankScore[nCntY].color.a);
			pVtx[2].col = D3DXCOLOR(g_RankScore[nCntY].color.r, g_RankScore[nCntY].color.g, g_RankScore[nCntY].color.b, g_RankScore[nCntY].color.a);
			pVtx[3].col = D3DXCOLOR(g_RankScore[nCntY].color.r, g_RankScore[nCntY].color.g, g_RankScore[nCntY].color.b, g_RankScore[nCntY].color.a);

			//テクスチャ座標の更新
			pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntY][nCntX], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntY][nCntX] + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntY][nCntX], 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntY][nCntX] + 0.1f, 1.0f);

			pVtx += 4;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRank[1]->Unlock();
}
