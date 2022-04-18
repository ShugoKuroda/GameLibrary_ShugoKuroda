#include "main.h"
#include "explosion.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;		//頂点バッファへのポインタ
Explosion g_aExplosion[MAX_EXPLOSION];					//構造体の宣言

//===============================================
//		爆発の初期化処理
//===============================================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/explosion000.png",
		&g_pTextureExplosion);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		g_aExplosion[nCnt].nSizeExplosion = 100;
		g_aExplosion[nCnt].bUse = false;
		//アニメーション初期化
		g_aExplosion[nCnt].nCounterAnimExplosion = 0;
		g_aExplosion[nCnt].nPatternAnimExplosion = 0;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCnt].posExplosion.x - g_aExplosion[nCnt].nSizeExplosion, g_aExplosion[nCnt].posExplosion.y - g_aExplosion[nCnt].nSizeExplosion, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCnt].posExplosion.x + g_aExplosion[nCnt].nSizeExplosion, g_aExplosion[nCnt].posExplosion.y - g_aExplosion[nCnt].nSizeExplosion, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCnt].posExplosion.x - g_aExplosion[nCnt].nSizeExplosion, g_aExplosion[nCnt].posExplosion.y + g_aExplosion[nCnt].nSizeExplosion, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCnt].posExplosion.x + g_aExplosion[nCnt].nSizeExplosion, g_aExplosion[nCnt].posExplosion.y + g_aExplosion[nCnt].nSizeExplosion, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.25f);
		pVtx[3].tex = D3DXVECTOR2(0.25f, 0.25f);

		pVtx += 4; //データを４つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
}

//===============================================
//		爆発の終了処理
//===============================================
void UninitExplosion(void)
{
	//テクスチャの破棄
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//===============================================
//		爆発の更新処理
//===============================================
void UpdateExplosion(void)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		if (g_aExplosion[nCnt].bUse == true)
		{
			//頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCnt].posExplosion.x - g_aExplosion[nCnt].nSizeExplosion, g_aExplosion[nCnt].posExplosion.y - g_aExplosion[nCnt].nSizeExplosion, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCnt].posExplosion.x + g_aExplosion[nCnt].nSizeExplosion, g_aExplosion[nCnt].posExplosion.y - g_aExplosion[nCnt].nSizeExplosion, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCnt].posExplosion.x - g_aExplosion[nCnt].nSizeExplosion, g_aExplosion[nCnt].posExplosion.y + g_aExplosion[nCnt].nSizeExplosion, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCnt].posExplosion.x + g_aExplosion[nCnt].nSizeExplosion, g_aExplosion[nCnt].posExplosion.y + g_aExplosion[nCnt].nSizeExplosion, 0.0f);

			g_aExplosion[nCnt].nCounterAnimExplosion++;//カウンターを加算
			if (g_aExplosion[nCnt].nCounterAnimExplosion % 2 == 0)
			{
				g_aExplosion[nCnt].nPatternAnimExplosion = (g_aExplosion[nCnt].nPatternAnimExplosion + 1) % 16;

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.25f * ((g_aExplosion[nCnt].nPatternAnimExplosion % 4) + 1), 0.25f * (g_aExplosion[nCnt].nPatternAnimExplosion / 4));
				pVtx[1].tex = D3DXVECTOR2(0.25f * (g_aExplosion[nCnt].nPatternAnimExplosion % 4), 0.25f * (g_aExplosion[nCnt].nPatternAnimExplosion / 4));
				pVtx[2].tex = D3DXVECTOR2(0.25f * ((g_aExplosion[nCnt].nPatternAnimExplosion % 4) + 1), 0.25f * ((g_aExplosion[nCnt].nPatternAnimExplosion / 4) + 1));
				pVtx[3].tex = D3DXVECTOR2(0.25f * (g_aExplosion[nCnt].nPatternAnimExplosion % 4), 0.25f * ((g_aExplosion[nCnt].nPatternAnimExplosion / 4) + 1));

				if (g_aExplosion[nCnt].nPatternAnimExplosion >= 15)
				{
					g_aExplosion[nCnt].bUse = false;
				}
			}
		}
		pVtx += 4; //データを４つ分進める
	}

	//頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
}

//===============================================
//		爆発の描画処理
//===============================================
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureExplosion);

	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		if (g_aExplosion[nCnt].bUse == true)
		{
			//爆発の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				nCnt * 4,	//描画する最初の頂点インデックス
				2);			//プリミティブ（ポリゴン）数
		}
	}
}

//===============================================
//		爆発の設定処理
//===============================================
void SetExplosion(D3DXVECTOR3 pos,float Size)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		if (g_aExplosion[nCnt].bUse == false)
		{
			//位置の取得
			g_aExplosion[nCnt].posExplosion = pos;
			g_aExplosion[nCnt].nSizeExplosion = Size;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCnt].posExplosion.x - g_aExplosion[nCnt].nSizeExplosion, g_aExplosion[nCnt].posExplosion.y - g_aExplosion[nCnt].nSizeExplosion, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCnt].posExplosion.x + g_aExplosion[nCnt].nSizeExplosion, g_aExplosion[nCnt].posExplosion.y - g_aExplosion[nCnt].nSizeExplosion, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCnt].posExplosion.x - g_aExplosion[nCnt].nSizeExplosion, g_aExplosion[nCnt].posExplosion.y + g_aExplosion[nCnt].nSizeExplosion, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCnt].posExplosion.x + g_aExplosion[nCnt].nSizeExplosion, g_aExplosion[nCnt].posExplosion.y + g_aExplosion[nCnt].nSizeExplosion, 0.0f);

			g_aExplosion[nCnt].bUse = true;	//使用している状態にする
			g_aExplosion[nCnt].nCounterAnimExplosion = 0;
			g_aExplosion[nCnt].nPatternAnimExplosion = 0;

			break;
		}
		pVtx += 4; //データを４つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
}