//===================================================================
//
//		ライン処理[line.cpp]
//		Author:黒田 周吾
//
//===================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include "model.h"
#include "line.h"

//======================================================
//	グローバル宣言
//======================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLine = NULL;		//頂点バッファへのポインタ
Line g_aLine[MAX_LINE];		//モデルの情報

//======================================================
//	ラインの初期化処理
//======================================================
void InitLine(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_LINE) * 2 * MAX_LINE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_LINE,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLine,
		NULL);

	//構造体の要素を初期化
	ZeroMemory(&g_aLine[0], sizeof(Line));

	VERTEX_LINE *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLine->Lock(0, 0, (void**)&pVtx, 0);

	//各情報の初期化
	for (int nCnt = 0; nCnt < MAX_LINE; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx += 2;
	}
	//頂点バッファをアンロック
	g_pVtxBuffLine->Unlock();

	//壁配置
	SetLine(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetLine(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetLine(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetLine(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//======================================================
//	ラインの終了処理
//======================================================
void UninitLine(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffLine != NULL)
	{
		g_pVtxBuffLine->Release();
		g_pVtxBuffLine = NULL;
	}
}

//======================================================
//	ラインの更新処理
//======================================================
void UpdateLine(void)
{
}

//======================================================
//	ラインの描画処理
//======================================================
void DrawLine(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	for (int nCnt = 0; nCnt < MAX_LINE; nCnt++)
	{
		if (g_aLine[nCnt].bUse == true)
		{//影が使用されている場合
		 //ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aLine[nCnt].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aLine[nCnt].rot.y, g_aLine[nCnt].rot.x, g_aLine[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aLine[nCnt].mtxWorld, &g_aLine[nCnt].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aLine[nCnt].pos.x, g_aLine[nCnt].pos.y, g_aLine[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aLine[nCnt].mtxWorld, &g_aLine[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aLine[nCnt].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffLine, 0, sizeof(VERTEX_LINE));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_LINE);

			//プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_LINELIST,		//プリミティブの種類
				nCnt * 2,	//描画する最初の頂点インデックス
				1);			//プリミティブ（ポリゴン）数
		}
	}
}

//======================================================
//	ラインの設定処理
//======================================================
void SetLine(D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin)
{
	for (int nCnt = 0; nCnt < MAX_LINE; nCnt++)
	{
		if (g_aLine[nCnt].bUse == false)
		{//弾が使用されていない場合
			g_aLine[nCnt].pos = VtxMax;
			g_aLine[nCnt].rot = VtxMin;
			g_aLine[nCnt].bUse = true;
			break;
		}
	}
}

//======================================================
//	ラインの情報を取得
//======================================================
Line *GetLine(void)
{
	return &g_aLine[0];
}