//========================================================================================================
//
//	タイマー処理[time.cpp]
//	Author:SHUGO KURODA
//
//========================================================================================================

//----------------------------------------
//	インクルード
//----------------------------------------
#include "main.h"
#include "player.h"
#include "input.h"
#include "enemy.h"
#include "bullet.h"
#include "title.h"
#include "game.h"
#include "score.h"
#include "time.h"
#include "logo.h"

//----------------------------------------
//	マクロ定義
//----------------------------------------
#define MAX_TIME		(3)			//最大タイム数
#define TIME_SIZE_X		(20.0f)		//タイマー表記のサイズ(x)
#define TIME_SIZE_Y		(40.0f)		//タイマー表記のサイズ(y)

//----------------------------------------
//	グローバル変数
//----------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;		//頂点バッファへのポインタ	
D3DXVECTOR3 g_posTime;		//タイムの位置
int g_Time;					//タイムの値
int g_nCntTime;
bool bTimeUp;		//時間切れかどうか

//----------------------------------------
//	タイマーの初期化処理
//----------------------------------------
void InitTime(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,"data/TEXTURE/number000.png",&g_pTextureTime);

	//各タイマー情報の初期化
	g_posTime = D3DXVECTOR3(490.0f, 40.0f, 0.0f);
	g_Time = 0;
	g_nCntTime = 0;
	bool bTimeUp = false;

	//頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へポインタ

	//頂点バッファをロックし、頂点情報へポインタ取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posTime.x + TIME_SIZE_X * nCntTime, 10.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posTime.x + TIME_SIZE_X * (nCntTime + 1), 10.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posTime.x + TIME_SIZE_X * nCntTime, TIME_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posTime.x + TIME_SIZE_X * (nCntTime + 1), TIME_SIZE_Y, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの描画
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;//ポインタを４つ分進める
	}
	//頂点バッファアンロック
	g_pVtxBuffTime->Unlock();

	//タイマーの初期設定
	AddTime(120);
}

//----------------------------------------
//	タイマーの終了処理
//----------------------------------------
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

//----------------------------------------
//	タイマーの更新処理
//----------------------------------------
void UpdateTime(void)
{
	//ロゴ情報の取得
	LogoCount *pLogoCount = GetLogoCount();

	if (pLogoCount->bNot)
	{
		g_nCntTime++;
		if (g_nCntTime % 60 == 0)
		{
			AddTime(-1);
		}

		if (g_Time <= 0)
		{
			//勝利ロゴの表示
			SetLogo(2);
		}
	}
}

//----------------------------------------
//	タイマーの描画処理
//----------------------------------------
void DrawTime(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTime);

	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			nCntTime * 4,
			2);											//プリミティブ（ポリゴン）数
	}
}

//----------------------------------------
//	タイマーの設定処理
//----------------------------------------
void SetTime(int nTime)
{
	int aPosTexU[MAX_TIME];	//各桁の数字を格納

	g_Time = nTime;

	aPosTexU[0] = g_Time % 1000 / 100;
	aPosTexU[1] = g_Time % 100 / 10;
	aPosTexU[2] = g_Time % 10 / 1;

	VERTEX_2D*pVtx;		//頂点情報へポインタ

	//頂点バッファをロックし、頂点情報へポインタ取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntTime], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCntTime] + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntTime], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCntTime] + 1), 1.0f);

		pVtx += 4;//ポインタを４つ分進める
	}

	//頂点バッファアンロック
	g_pVtxBuffTime->Unlock();
}

//----------------------------------------
//	タイマーの加算処理
//----------------------------------------
void AddTime(int nValue)
{
	int aPosTexU[MAX_TIME];	//各桁の数字を格納

	g_Time += nValue;

	aPosTexU[0] = g_Time % 1000 / 100;
	aPosTexU[1] = g_Time % 100 / 10;
	aPosTexU[2] = g_Time % 10 / 1;

	VERTEX_2D *pVtx;		//頂点情報へポインタ

	//頂点バッファをロックし、頂点情報へポインタ取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntTime], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCntTime] + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntTime], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCntTime] + 1), 1.0f);

		pVtx += 4;//ポインタを４つ分進める
	}

	//頂点バッファアンロック
	g_pVtxBuffTime->Unlock();
}