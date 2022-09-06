//===================================================================
//
//		影の設定処理[Shadow.cpp]
//		Author:黒田 周吾
//
//===================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include "shadow.h"

//======================================================
//	マクロ定義
//======================================================
#define MAX_SHADOW			(16)		//影の最大数

//======================================================
//	グローバル宣言
//======================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;			//テクスチャへのポインタ
Shadow g_aShadow[MAX_SHADOW];	//影の情報

//======================================================
//	影の初期化処理
//======================================================
void InitShadow(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/shadow000.jpg",
		&g_pTextureShadow);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	//影の情報の初期化
	ZeroMemory(&g_aShadow, sizeof(g_aShadow));

	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	//各情報の初期化
	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//各頂点の法線の設定(ベクトルの大きさは１にする必要がある)
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuffShadow->Unlock();
}

//======================================================
//	影の終了処理
//======================================================
void UninitShadow(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}

	//テクスチャの破棄
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}
}

//======================================================
//	影の更新処理
//======================================================
void UpdateShadow(void)
{
	//VERTEX_3D *pVtx;	//頂点情報へのポインタ

	////頂点バッファをロックし、頂点情報へのポインタを取得
	//g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	//for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	//{
	//	if (g_aShadow[nCnt].bUse == true)
	//	{//影を使用している場合
	//	 //頂点座標の更新
	//		pVtx[0].pos = D3DXVECTOR3(-(SHADOW_SIZE / 2), 0.0f,(SHADOW_SIZE / 2));
	//		pVtx[1].pos = D3DXVECTOR3((SHADOW_SIZE / 2), 0.0f,(SHADOW_SIZE / 2));
	//		pVtx[2].pos = D3DXVECTOR3(-(SHADOW_SIZE / 2), 0.0f, -(SHADOW_SIZE / 2));
	//		pVtx[3].pos = D3DXVECTOR3((SHADOW_SIZE / 2), 0.0f, -(SHADOW_SIZE / 2));
	//	}
	//	pVtx += 4;
	//}
	////頂点バッファをアンロックする
	//g_pVtxBuffShadow->Unlock();
}

//======================================================
//	影の描画処理
//======================================================
void DrawShadow(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		if (g_aShadow[nCnt].bUse == true)
		{//影を使用している場合
			D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aShadow[nCnt].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCnt].rot.y, g_aShadow[nCnt].rot.x, g_aShadow[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCnt].mtxWorld, &g_aShadow[nCnt].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCnt].pos.x, g_aShadow[nCnt].pos.y, g_aShadow[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCnt].mtxWorld, &g_aShadow[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCnt].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureShadow);

			//プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				nCnt * 4,	//描画する最初の頂点インデックス
				2);			//プリミティブ（ポリゴン）数
		}
	}
	//減算合成の設定を元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//テクスチャを元に戻す
	pDevice->SetTexture(0, NULL);
}

//======================================================
//	影の設定処理
//======================================================
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadiusX, float fRadiusZ)
{
	int nCntShadow;

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == false)
		{//影が使用されていない場合
			g_aShadow[nCntShadow].pos = pos;
			g_aShadow[nCntShadow].rot = rot;
			g_aShadow[nCntShadow].fRadiusX = fRadiusX;	//横幅
			g_aShadow[nCntShadow].fRadiusZ = fRadiusZ;	//縦幅
			g_aShadow[nCntShadow].bUse = true;

			break;
		}
	}
	return nCntShadow;
}

//======================================================
//	影の位置の更新処理
//======================================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	g_aShadow[nIdxShadow].pos = pos;	//位置
	g_aShadow[nIdxShadow].rot = rot;	//向き
}