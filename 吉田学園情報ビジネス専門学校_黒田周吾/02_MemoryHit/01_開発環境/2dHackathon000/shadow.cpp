//=============================================================================
//
// ゲーム画面のフェード処理 [shadow.cpp]
// Author : 黒田 周吾
//
//=============================================================================

//======================================================
//	インクルード
//======================================================
#include "shadow.h"
#include "main.h"

//======================================================
//	グローバル変数
//======================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//頂点バッファへのポインタ
SHADOW g_Shadow;									//フェードの状態
D3DXCOLOR g_colorShadow;							//ポリゴン（フェード）の色
int g_nCntFinish;	

//======================================================
//	フェードの初期化処理
//======================================================
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	g_Shadow = SHADOW_OUT;			//フェードイン状態にする
	g_colorShadow = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//画面を黒くする(不透明)

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = D3DXCOLOR(g_colorShadow);
	pVtx[1].col = D3DXCOLOR(g_colorShadow);
	pVtx[2].col = D3DXCOLOR(g_colorShadow);
	pVtx[3].col = D3DXCOLOR(g_colorShadow);

	//頂点バッファをアンロック
	g_pVtxBuffShadow->Unlock();
}

//======================================================
//	フェードの終了処理
//======================================================
void UninitShadow(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//======================================================
//	フェードの更新処理
//======================================================
void UpdateShadow(void)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	if (g_Shadow != SHADOW_NONE)
	{
		if (g_Shadow == SHADOW_OUT)
		{//フェードアウト状態
			g_colorShadow.a += 0.006f;		//ポリゴンを不透明にしていく
			if (g_colorShadow.a >= 1.0f)
			{
				g_colorShadow.a = 1.0f;
				g_Shadow = SHADOW_NONE;		//フェードイン状態に
			}
		}
		else if (g_Shadow == SHADOW_IN)
		{//フェードイン状態
			g_colorShadow.a -= 0.006f;		//ポリゴンを透明にしていく
			if (g_colorShadow.a <= 0.0f)
			{
				g_colorShadow.a = 0.0f;
				if (g_nCntFinish == 1)
				{
					g_Shadow = SHADOW_NONE;		//何もしていない状態にする
				}
				else
				{
					g_Shadow = SHADOW_OUT;		//フェードアウト状態にする
				}
			}
		}

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(g_colorShadow);
		pVtx[1].col = D3DXCOLOR(g_colorShadow);
		pVtx[2].col = D3DXCOLOR(g_colorShadow);
		pVtx[3].col = D3DXCOLOR(g_colorShadow);

		//頂点バッファをアンロック
		g_pVtxBuffShadow->Unlock();
	}
}

//======================================================
//	フェードの描画処理
//======================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//プレイヤーの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,		//描画する最初の頂点インデックス
		2);		//プリミティブ（ポリゴン）数

}

//======================================================
//	フェードの設定処理
//======================================================
void SetShadow(int nFinish)
{
	g_nCntFinish = nFinish;
	g_Shadow = SHADOW_IN;		//フェードアウト状態にする
	g_colorShadow = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//画面を黒くする(不透明)
}

//======================================================
//	フェードの情報を取得
//======================================================
SHADOW *GetShadow(void)
{
	return &g_Shadow;
}