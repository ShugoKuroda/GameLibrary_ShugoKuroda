//===================================================================
//
//		エフェクト処理[effect.cpp]
//		Author:黒田 周吾
//
//===================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include "effect.h"
#include "input.h"

//======================================================
//	マクロ定義
//======================================================	
#define MAX_EFFECT		(1240)		//エフェクトの最大数
#define MAX_EFFECT_TEX  (3)			//エフェクトのテクスチャ数
#define EFFECT_SIZE		(8.0f)		//エフェクトのサイズ
#define EFFECT_LIFE		(10)		//エフェクトの寿命

//======================================================
//	エフェクト構造体の定義
//======================================================
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 scale;		//サイズ
	D3DXVECTOR3 move;		//移動量
	D3DXCOLOR col;			//色
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nLife;				//寿命
	int nType;				//種類
	bool bUse;				//使用しているかどうか
}Effect;

//======================================================
//	グローバル変数
//======================================================
LPDIRECT3DTEXTURE9 g_pTextureEffect[MAX_EFFECT_TEX] = { NULL };		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;					//頂点バッファへのポインタ
Effect g_aEffect[MAX_EFFECT];										//エフェクトの情報

//======================================================
//	エフェクトの初期化処理
//======================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/shadow000.jpg", &g_pTextureEffect[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/shadow001.png", &g_pTextureEffect[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/hit.png", &g_pTextureEffect[2]);

	//エフェクト構造体の初期化
	ZeroMemory(&g_aEffect, sizeof(g_aEffect));

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3((-EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.x, (EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3((EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.x, (EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3((-EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.x, (-EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3((EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.x, (-EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.y, 0.0f);

		//各頂点の法線の設定(ベクトルの大きさは１にする必要がある)
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャの座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点座標をアンロック
	g_pVtxBuffEffect->Unlock();
}

//=======================================================
//	エフェクトの終了処理
//=======================================================
void UninitEffect(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < MAX_EFFECT_TEX; nCnt++)
	{
		if (g_pTextureEffect[nCnt] != NULL)
		{
			g_pTextureEffect[nCnt]->Release();
			g_pTextureEffect[nCnt] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//=========================================================
//	エフェクトの更新処理
//=========================================================
void UpdateEffect(void)
{
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//エフェクトのライフ減少
			g_aEffect[nCntEffect].scale.x -= 0.05f;
			g_aEffect[nCntEffect].scale.y -= 0.05f;
			g_aEffect[nCntEffect].nLife--;

			//頂点カラーの設定
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3((-EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.x, (EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.x, (EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((-EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.x, (-EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.x, (-EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.y, 0.0f);

			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].bUse = false;
			}
		}
		pVtx += 4;
	}
	//頂点座標をアンロック
	g_pVtxBuffEffect->Unlock();
}

//===============================================================
//	エフェクトの描画処理
//===============================================================
void DrawEffect(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DXMATRIX mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxView;		//ビューマトリックス

	//Zバッファを有効にする
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		if (g_aEffect[nCnt].bUse == true)
		{//弾が使用されている場合
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEffect[nCnt].mtxWorld);

			//ビューマトリックスの設定
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//カメラの逆行列を設定
			g_aEffect[nCnt].mtxWorld._11 = mtxView._11;
			g_aEffect[nCnt].mtxWorld._12 = mtxView._21;
			g_aEffect[nCnt].mtxWorld._13 = mtxView._31;
			g_aEffect[nCnt].mtxWorld._21 = mtxView._12;
			g_aEffect[nCnt].mtxWorld._22 = mtxView._22;
			g_aEffect[nCnt].mtxWorld._23 = mtxView._32;
			g_aEffect[nCnt].mtxWorld._31 = mtxView._13;
			g_aEffect[nCnt].mtxWorld._32 = mtxView._23;
			g_aEffect[nCnt].mtxWorld._33 = mtxView._33;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCnt].pos.x, g_aEffect[nCnt].pos.y, g_aEffect[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCnt].mtxWorld, &g_aEffect[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCnt].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect[g_aEffect[nCnt].nType]);

			//プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				nCnt * 4,	//描画する最初の頂点インデックス
				2);			//プリミティブ（ポリゴン）数
		}
	}
	//ライトを有効に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//テクスチャを元に戻す
	pDevice->SetTexture(0, NULL);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Zバッファを元に戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//===========================================================
//	エフェクトの設定処理
//===========================================================
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 scale, int nType)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].scale = scale;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3((-EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.x, (+EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((+EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.x, (+EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((-EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.x, (-EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((+EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.x, (-EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.y, 0.0f);

			g_aEffect[nCntEffect].nType = nType;
			g_aEffect[nCntEffect].nLife = EFFECT_LIFE;
			g_aEffect[nCntEffect].bUse = true;

			break;
		}
		pVtx += 4;
	}
	//頂点座標をアンロック
	g_pVtxBuffEffect->Unlock();
}