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
#include "life.h"
#include "player.h"

//----------------------------------------
//	マクロ定義
//----------------------------------------
#define MAX_LIFE		(3)			//最大タイム数
#define LIFE_SIZE_X		(20.0f)		//ライフ表記のサイズ(x)
#define LIFE_SIZE_Y		(690.0f)	//ライフ表記のサイズ(y)
#define PLAYER_LIFE		(100)		//プレイヤーの初期ライフ

//----------------------------------------
//	グローバル変数
//----------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureLife = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;		//頂点バッファへのポインタ	
D3DXVECTOR3 g_posLife;		//ライフの位置
int g_Life;					//ライフの値
int g_PlayerLife;			//プレイヤーのライフ値

//----------------------------------------
//	ライフの初期化処理
//----------------------------------------
void InitLife(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureLife);

	g_posLife = D3DXVECTOR3(120.0f, 0.0f, 0.0f);	//位置を初期化する
	g_Life = 0;		//値を初期化する
	g_PlayerLife = PLAYER_LIFE;			//プレイヤーの体力値の初期化

	//頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LIFE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へポインタ

	//頂点バッファをロックし、頂点情報へポインタ取得
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < MAX_LIFE; nCntTime++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posLife.x + LIFE_SIZE_X * nCntTime, 650.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posLife.x + LIFE_SIZE_X * (nCntTime + 1), 650.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posLife.x + LIFE_SIZE_X * nCntTime, LIFE_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posLife.x + LIFE_SIZE_X * (nCntTime + 1), LIFE_SIZE_Y, 0.0f);

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
	g_pVtxBuffLife->Unlock();

	//ライフの初期設定
	SetLife(PLAYER_LIFE);
}

//----------------------------------------
//	ライフの終了処理
//----------------------------------------
void UninitLife(void)
{
	//テクスチャの破棄
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}
}

//----------------------------------------
//	ライフの更新処理
//----------------------------------------
void UpdateLife(void)
{
	//プレイヤー情報の取得
	Player *pPlayer = GetPlayer();

	if (g_PlayerLife <= 0)
	{//プレイヤーの体力が0
		pPlayer->bIsDie = true;		//プレイヤーを死亡状態にする
	}
}

//----------------------------------------
//	ライフの描画処理
//----------------------------------------
void DrawLife(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffLife, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureLife);

	for (int nCntTime = 0; nCntTime < MAX_LIFE; nCntTime++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			nCntTime * 4,
			2);											//プリミティブ（ポリゴン）数
	}
}

//----------------------------------------
//	ライフの設定処理
//----------------------------------------
void SetLife(int nLife)
{
	int aPosTexU[MAX_LIFE];	//各桁の数字を格納

	g_Life = nLife;

	aPosTexU[0] = g_Life % 1000 / 100;
	aPosTexU[1] = g_Life % 100 / 10;
	aPosTexU[2] = g_Life % 10 / 1;

	VERTEX_2D*pVtx;		//頂点情報へポインタ

	//頂点バッファをロックし、頂点情報へポインタ取得
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < MAX_LIFE; nCntTime++)
	{
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntTime], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCntTime] + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntTime], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCntTime] + 1), 1.0f);

		pVtx += 4;//ポインタを４つ分進める
	}

	//頂点バッファアンロック
	g_pVtxBuffLife->Unlock();
}

//----------------------------------------
//	ライフの加算処理
//----------------------------------------
void AddLife(int nValue)
{
	int aPosTexU[MAX_LIFE];	//各桁の数字を格納

	g_PlayerLife += nValue;
	g_Life += nValue;

	aPosTexU[0] = g_Life % 1000 / 100;
	aPosTexU[1] = g_Life % 100 / 10;
	aPosTexU[2] = g_Life % 10 / 1;

	VERTEX_2D *pVtx;		//頂点情報へポインタ

	//頂点バッファをロックし、頂点情報へポインタ取得
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < MAX_LIFE; nCntTime++)
	{
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntTime], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCntTime] + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntTime], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCntTime] + 1), 1.0f);

		pVtx += 4;//ポインタを４つ分進める
	}

	//頂点バッファアンロック
	g_pVtxBuffLife->Unlock();
}