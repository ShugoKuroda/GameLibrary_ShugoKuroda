//-------------------------------------------
//
//タイマー処理[time.cpp]
//Author:平野舞
//
//-------------------------------------------

//インクルードファイル
#include "time.h"
#include "game_logo.h"
#include "fade.h"
#include "shadow.h"
#include "sound.h"

//マクロ定義
#define TIME_WIDTH		(20.0f)		//タイムの幅
#define TIME_HEIGHT		(45.0f)		//タイムの高さ

//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTextureTime[2] = {};			//テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime[2] = {};		//頂点バッファへのポインタ
Time g_aTime;										//タイマーの情報
int g_nCntLogoSet;			//ロゴを表示するまでのカウンター
bool g_bFinish;				//ゲームが終わったかどうか

//-------------------------------------------
//タイマーの初期化処理
//-------------------------------------------
void InitTime(void)
{
	//構造体の初期化
	for (int nCount = 0; nCount < NUM_TIME; nCount++)
	{
		g_aTime.pos[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
	}
	g_aTime.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			//カラー
	g_aTime.nTime = 0;			//何秒間か
	g_aTime.nFlame = 60;		//60フレーム
	g_aTime.bUse = false;		//使用しない

	//カウンター用変数をリセット
	g_bFinish = false;
	g_nCntLogoSet = 0;

	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//--------------------------背景-----------------------------------
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Time_rogo.png",
		&g_pTextureTime[0]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime[0],
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime[0]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(1100 - (200.0f / 2.0f), 90.0f - (70.0f / 2.0f), 0);
	pVtx[1].pos = D3DXVECTOR3(1100 + (200.0f / 2.0f), 90.0f - (70.0f / 2.0f), 0);
	pVtx[2].pos = D3DXVECTOR3(1100 - (200.0f / 2.0f), 90.0f + (70.0f / 2.0f), 0);
	pVtx[3].pos = D3DXVECTOR3(1100 + (200.0f / 2.0f), 90.0f + (70.0f / 2.0f), 0);

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
	g_pVtxBuffTime[0]->Unlock();

	//--------------------------数値-----------------------------------
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number000.png",
		&g_pTextureTime[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime[1],
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime[1]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < NUM_TIME; nCount++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aTime.pos[nCount].x - (TIME_WIDTH / 2.0f) + (nCount * 25.0f), g_aTime.pos[nCount].y - (TIME_HEIGHT / 2.0f), 0);
		pVtx[1].pos = D3DXVECTOR3(g_aTime.pos[nCount].x + (TIME_WIDTH / 2.0f) + (nCount * 25.0f), g_aTime.pos[nCount].y - (TIME_HEIGHT / 2.0f), 0);
		pVtx[2].pos = D3DXVECTOR3(g_aTime.pos[nCount].x - (TIME_WIDTH / 2.0f) + (nCount * 25.0f), g_aTime.pos[nCount].y + (TIME_HEIGHT / 2.0f), 0);
		pVtx[3].pos = D3DXVECTOR3(g_aTime.pos[nCount].x + (TIME_WIDTH / 2.0f) + (nCount * 25.0f), g_aTime.pos[nCount].y + (TIME_HEIGHT / 2.0f), 0);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(g_aTime.color.r, g_aTime.color.g, g_aTime.color.b, g_aTime.color.a);
		pVtx[1].col = D3DXCOLOR(g_aTime.color.r, g_aTime.color.g, g_aTime.color.b, g_aTime.color.a);
		pVtx[2].col = D3DXCOLOR(g_aTime.color.r, g_aTime.color.g, g_aTime.color.b, g_aTime.color.a);
		pVtx[3].col = D3DXCOLOR(g_aTime.color.r, g_aTime.color.g, g_aTime.color.b, g_aTime.color.a);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;		//頂点情報を4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffTime[1]->Unlock();
}

//-------------------------------------------
//タイマーの終了処理
//-------------------------------------------
void UninitTime(void)
{
	for (int nCount = 0; nCount < 2; nCount++)
	{
		//テクスチャの破棄
		if (g_pTextureTime[nCount] != NULL)
		{
			g_pTextureTime[nCount]->Release();
			g_pTextureTime[nCount] = NULL;
		}
	}

	for (int nCount = 0; nCount < 2; nCount++)
	{
		//頂点バッファの破棄
		if (g_pVtxBuffTime[nCount] != NULL)
		{
			g_pVtxBuffTime[nCount]->Release();
			g_pVtxBuffTime[nCount] = NULL;
		}
	}

}

//-------------------------------------------
//タイマーの更新処理
//-------------------------------------------
void UpdateTime(void)
{
	int nTimePos[NUM_TIME];		//タイムのテクスチャ位置

	if (g_aTime.nTime != 0)
	{//タイマーが0ではなかったら
		g_aTime.nFlame--;		//フレーム数を減らす
	}

	if (g_aTime.nFlame <= 0)
	{//フレーム数が0より小さくなったら
		g_aTime.nTime--;				//タイマーを1減らす				
		g_aTime.nFlame = 60;			//フレーム数を60に戻す
	}

	if (g_aTime.nTime == 0 && !g_bFinish)
	{//タイマーが0になったら
		SetLogo(LOGONUM_FINISH, 90);
		SetShadow(1);
		g_bFinish = true;

		//サウンド再生
		PlaySound(SOUND_LABEL_SE_FINISH);
	}
	else if (g_bFinish)
	{
		g_nCntLogoSet++;

		if (g_nCntLogoSet == 300)
		{
			SetFade(MODE_RESULT);
		}
	}

	//数字のテクスチャを格納
	nTimePos[0] = g_aTime.nTime % 1000 / 100;
	nTimePos[1] = g_aTime.nTime % 100 / 10;
	nTimePos[2] = g_aTime.nTime % 10;

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime[1]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < NUM_TIME; nCntTime++)
	{//テクスチャ座標の更新
		pVtx[0].tex = D3DXVECTOR2(0.1f * nTimePos[nCntTime], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * nTimePos[nCntTime] + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * nTimePos[nCntTime], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * nTimePos[nCntTime] + 0.1f, 1.0f);

		//頂点カラーの更新
		pVtx[0].col = D3DXCOLOR(g_aTime.color.r, g_aTime.color.g, g_aTime.color.b, g_aTime.color.a);
		pVtx[1].col = D3DXCOLOR(g_aTime.color.r, g_aTime.color.g, g_aTime.color.b, g_aTime.color.a);
		pVtx[2].col = D3DXCOLOR(g_aTime.color.r, g_aTime.color.g, g_aTime.color.b, g_aTime.color.a);
		pVtx[3].col = D3DXCOLOR(g_aTime.color.r, g_aTime.color.g, g_aTime.color.b, g_aTime.color.a);

		//頂点座標の更新
		pVtx[0].pos = D3DXVECTOR3(g_aTime.pos[nCntTime].x - (TIME_WIDTH / 2.0f) + (nCntTime * 25.0f), g_aTime.pos[nCntTime].y - (TIME_HEIGHT / 2.0f), 0);
		pVtx[1].pos = D3DXVECTOR3(g_aTime.pos[nCntTime].x + (TIME_WIDTH / 2.0f) + (nCntTime * 25.0f), g_aTime.pos[nCntTime].y - (TIME_HEIGHT / 2.0f), 0);
		pVtx[2].pos = D3DXVECTOR3(g_aTime.pos[nCntTime].x - (TIME_WIDTH / 2.0f) + (nCntTime * 25.0f), g_aTime.pos[nCntTime].y + (TIME_HEIGHT / 2.0f), 0);
		pVtx[3].pos = D3DXVECTOR3(g_aTime.pos[nCntTime].x + (TIME_WIDTH / 2.0f) + (nCntTime * 25.0f), g_aTime.pos[nCntTime].y + (TIME_HEIGHT / 2.0f), 0);

		pVtx += 4;		//データを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTime[1]->Unlock();
}

//-------------------------------------------
//タイマーの描画処理
//-------------------------------------------
void DrawTime(void)
{
	for (int nCount = 0; nCount < 2; nCount++)
	{
		if (g_aTime.bUse == true)
		{//使用していたら
			//デバイスへのポインタ
			LPDIRECT3DDEVICE9 pDevice;

			//デバイスの取得
			pDevice = GetDevice();

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffTime[nCount], 0, sizeof(VERTEX_2D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			for (int nCntTime = 0; nCntTime < NUM_TIME; nCntTime++)
			{
				//テクスチャ設定
				pDevice->SetTexture(0, g_pTextureTime[nCount]);

				//ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);
			}
		}
	}
}

//-------------------------------------------
//タイマーの設定処理
//
//D3DXVECTOR3 pos → タイマーの位置を指定
//D3DXCOLOR color → タイマーの数値の色を指定
//int nTime　→ タイマーの時間を指定
//
//-------------------------------------------
void SetTime(D3DXVECTOR3 pos, D3DXCOLOR color,int nTime)
{
	if (g_aTime.bUse == false)
	{//使用していなかったら
		for (int nCount = 0; nCount < NUM_TIME; nCount++)
		{
			g_aTime.pos[nCount] = pos;		//位置
		}
		g_aTime.color = color;			//カラー
		g_aTime.nTime = nTime;			//時間
		g_aTime.bUse = true;			//使用する
	}
}

//-------------------------------------------
//ゲームの終了判定
//-------------------------------------------
bool FinishGame(void)
{
	return g_bFinish;
}