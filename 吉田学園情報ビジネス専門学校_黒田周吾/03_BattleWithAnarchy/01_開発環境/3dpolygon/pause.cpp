//=============================================================================
//
//	ポーズ画面処理 [pause.cpp]
//	Author : 黒田 周吾
//
//=============================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include <stdio.h>
#include "load.h"
#include "game.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "input.h"
#include "shadow.h"
#include "wall.h"
#include "billboard.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "bullet.h"
#include "effect.h"
#include "player.h"
#include "line.h"
#include "sound.h"
#include "fade.h"

//======================================================
//	マクロ定義
//======================================================
#define	PAUSE_IMAGE_MAX			(5)			//ポーズ画面に必要な画像数
#define	PAUSE_SELECT_MAX		(4)			//ポーズ画面に必要な選択数
#define PAUSE_UI_SIZE_X			(1000.0f)	//ポーズ画面UIのXサイズ
#define PAUSE_UI_SIZE_Y			(400.0f)	//ポーズ画面UIのYサイズ
#define PAUSE_SELECT_SIZE_X		(160.0f)	//ポーズ画面の選択項目のXサイズ
#define PAUSE_SELECT_SIZE_Y		(200.0f)	//ポーズ画面の選択項目のYサイズ

//======================================================
//	グローバル変数
//======================================================
LPDIRECT3DTEXTURE9 g_pTexturePause[PAUSE_IMAGE_MAX] = { NULL };	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause[3] = { NULL };			//頂点バッファへのポインタ
D3DXVECTOR3 g_PausePos[2];		//ポーズ画面の位置
bool g_bPause;					//ゲームのポーズ(一時停止)判定
bool g_bDiePlayer;				//プレイヤーの死亡判定
float g_fColPauseA[PAUSE_IMAGE_MAX + 1];	//ポーズアルファ値
int g_nPauseSelecting;						//ポーズ画面中に選択した項目の変数
int g_nCntCol;			//ポーズ画面のフェードの出す順用
int g_nColPoseCnt;		//ポーズ画面中に選択している項目をチカチカさせる用変数
int g_nFade;			//フェードまでの余韻
int g_nCntSet;			//ブロックの配置タイミングずらし用変数

//======================================================
//	ポーズ画面の初期化処理
//======================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスのポインタ

	//各項目の初期化
	g_PausePos[0] = D3DXVECTOR3(SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 2.0f, 0.0f);				//ポーズ画面の選択項目位置の初期化
	g_PausePos[1] = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, (SCREEN_HEIGHT / 2.0f) - 20.0f, 0.0f);		//ポーズ画面UIの位置の初期化
	g_nCntCol = 0;			//ポーズ画面のフェードの出す順用
	g_nFade = 0;			//フェードまでの余韻
	g_nPauseSelecting = 2;	//ゲームを続けるを選択に初期化
	g_nColPoseCnt = 0;		//ポーズ画面中に選択している項目をチカチカさせる用の初期化
	g_bPause = false;		//ポーズ判定
	g_bDiePlayer = false;	//プレイヤーの死亡判定

	//ポーズ画面を初期化する
	for (int nCnt = 0; nCnt < PAUSE_IMAGE_MAX + 1; nCnt++)
	{
		g_fColPauseA[nCnt] = 0.0f;	//ポーズアルファ値初期化
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/title000.png",
		&g_pTexturePause[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Pause002.png",
		&g_pTexturePause[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Pause001.png",
		&g_pTexturePause[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Pause003.png",
		&g_pTexturePause[3]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/title000.png",
		&g_pTexturePause[4]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,		//画面全体のバッファ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause[0],
		NULL);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,		//ポーズUIのバッファ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause[1],
		NULL);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSE_SELECT_MAX,		//ポーズ選択項目のバッファ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause[2],
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause[0]->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fColPauseA[0]);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fColPauseA[0]);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fColPauseA[0]);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fColPauseA[0]);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffPause[0]->Unlock();


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause[1]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_PausePos[1].x - (PAUSE_UI_SIZE_X / 2), g_PausePos[1].y - (PAUSE_UI_SIZE_Y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_PausePos[1].x + (PAUSE_UI_SIZE_X / 2), g_PausePos[1].y - (PAUSE_UI_SIZE_Y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_PausePos[1].x - (PAUSE_UI_SIZE_X / 2), g_PausePos[1].y + (PAUSE_UI_SIZE_Y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_PausePos[1].x + (PAUSE_UI_SIZE_X / 2), g_PausePos[1].y + (PAUSE_UI_SIZE_Y / 2), 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[1]);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[1]);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[1]);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[1]);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffPause[1]->Unlock();


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause[2]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < PAUSE_SELECT_MAX; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3((g_PausePos[0].x + 200.0f * nCnt) - (PAUSE_SELECT_SIZE_X / 2), g_PausePos[0].y - (PAUSE_SELECT_SIZE_Y / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3((g_PausePos[0].x + 200.0f * nCnt) + (PAUSE_SELECT_SIZE_X / 2), g_PausePos[0].y - (PAUSE_SELECT_SIZE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3((g_PausePos[0].x + 200.0f * nCnt) - (PAUSE_SELECT_SIZE_X / 2), g_PausePos[0].y + (PAUSE_SELECT_SIZE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3((g_PausePos[0].x + 200.0f * nCnt) + (PAUSE_SELECT_SIZE_X / 2), g_PausePos[0].y + (PAUSE_SELECT_SIZE_Y / 2), 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[2 + nCnt]);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[2 + nCnt]);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[2 + nCnt]);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[2 + nCnt]);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;  //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffPause[2]->Unlock();
}

//======================================================
//	ポーズ画面の終了処理
//======================================================
void UninitPause(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < PAUSE_IMAGE_MAX; nCnt++)
	{
		if (g_pTexturePause[nCnt] != NULL)
		{
			g_pTexturePause[nCnt]->Release();
			g_pTexturePause[nCnt] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		//頂点バッファの破棄
		if (g_pVtxBuffPause[nCnt] != NULL)
		{
			g_pVtxBuffPause[nCnt]->Release();
			g_pVtxBuffPause[nCnt] = NULL;
		}
	}
}

//======================================================
//	ポーズ画面の更新処理
//======================================================
bool UpdatePause(void)
{
	//プレイヤー情報の取得
	Player *pPlayer = GetPlayer();

	//Pキーが押された場合
	//if (GetKeyboardTrigger(DIK_P) == true && g_bPause == false)
	//{	
	//	//ゲームを止める
	//	g_nCntCol = 0;
	//	g_bPause = true;
	//}

	//ポーズ中の場合
	if (g_bPause == true)
	{
		if (g_nCntCol == 0)
		{//バックのゲーム画面を暗くする
			g_fColPauseA[0] += 0.1f;
			if (g_fColPauseA[0] > 0.5f)
			{
				g_fColPauseA[0] = 0.5f;
				g_nCntCol = 1;
			}
		}
		else if (g_nCntCol == 1)
		{//ポーズ画面を出す
			for (int nCnt = 1; nCnt < PAUSE_IMAGE_MAX + 1; nCnt++)
			{
				g_fColPauseA[nCnt] += 0.1f;
				if (g_fColPauseA[nCnt] > 1.0f)
				{
					g_fColPauseA[nCnt] = 1.0f;
					g_nCntCol = 2;
				}
			}
		}
		else if (g_nCntCol == 2)
		{//ポーズ選択画面
			if (GetKeyboardTrigger(DIK_A)== true)
			{
				g_nPauseSelecting--;
				//選択していない項目のリセット
				g_fColPauseA[2] = 1.0f;
				g_fColPauseA[3] = 1.0f;
				g_fColPauseA[4] = 1.0f;
				g_fColPauseA[5] = 1.0f;
				if (g_nPauseSelecting < 2)
				{
					g_nPauseSelecting = 5;
				}
			}
			else if (GetKeyboardTrigger(DIK_D) == true)
			{
				g_nPauseSelecting++;
				//選択していない項目のリセット
				g_fColPauseA[2] = 1.0f;
				g_fColPauseA[3] = 1.0f;
				g_fColPauseA[4] = 1.0f;
				g_fColPauseA[5] = 1.0f;
				if (g_nPauseSelecting > 5)
				{
					g_nPauseSelecting = 2;
				}
			}
			else if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) == true)
			{
				if (g_nPauseSelecting == 2)
				{//ゲームを続ける
					g_bPause = false;
				}
				else if (g_nPauseSelecting == 3)
				{//リトライ
					//DiedPlayer();
					SetMode(MODE_GAME);
				}
				else if (g_nPauseSelecting == 4)
				{//リタイア
					g_bPause = false;
				}
				else if (g_nPauseSelecting == 5)
				{//タイトルに戻る
					//DiedPlayer();
					SetFade(MODE_TITLE);
				}
			}

			if (g_nColPoseCnt == 0)
			{//選択中の項目をチカチカさせる
				g_fColPauseA[g_nPauseSelecting] -= 0.05f;
				if (g_fColPauseA[g_nPauseSelecting] < 0.0f)
				{
					g_fColPauseA[g_nPauseSelecting] = 0.0f;
					g_nColPoseCnt = 1;
				}
			}
			else if (g_nColPoseCnt == 1)
			{
				g_fColPauseA[g_nPauseSelecting] += 0.05f;
				if (g_fColPauseA[g_nPauseSelecting] > 1.0f)
				{
					g_fColPauseA[g_nPauseSelecting] = 1.0f;
					g_nColPoseCnt = 0;
				}
			}
		}
	}
	else
	{//ポーズ画面を出していないとき
		for (int nCnt = 0; nCnt < PAUSE_IMAGE_MAX + 1; nCnt++)
		{
			g_fColPauseA[nCnt] -= 0.1f;
			if (g_fColPauseA[nCnt] < 0.0f)
			{
				g_fColPauseA[nCnt] = 0.0f;
			}
		}
	}

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause[0]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fColPauseA[0]);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fColPauseA[0]);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fColPauseA[0]);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fColPauseA[0]);

	//頂点バッファをアンロック
	g_pVtxBuffPause[0]->Unlock();


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause[1]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[1]);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[1]);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[1]);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[1]);

	//頂点バッファをアンロック
	g_pVtxBuffPause[1]->Unlock();


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause[2]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < PAUSE_SELECT_MAX; nCnt++)
	{
		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[nCnt + 2]);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[nCnt + 2]);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[nCnt + 2]);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[nCnt + 2]);

		pVtx += 4;  //頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロック
	g_pVtxBuffPause[2]->Unlock();

	return g_bPause;
}

//======================================================
//	ポーズ画面の描画処理
//======================================================
void DrawPause(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause[0], 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//プレイヤーの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,		//描画する最初の頂点インデックス
		2);		//プリミティブ（ポリゴン）数


	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause[1], 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePause[4]);
	//プレイヤーの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,		//描画する最初の頂点インデックス
		2);		//プリミティブ（ポリゴン）数


	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause[2], 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < PAUSE_SELECT_MAX; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePause[nCnt]);
		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			4 * nCnt,	//描画する最初の頂点インデックス
			2);			//プリミティブ（ポリゴン）数
	}
}