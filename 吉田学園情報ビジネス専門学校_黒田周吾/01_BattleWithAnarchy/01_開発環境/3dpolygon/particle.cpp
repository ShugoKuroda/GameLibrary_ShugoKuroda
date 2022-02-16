//===================================================================
//
//	パーティクル処理[particle.cpp]
//	Author:黒田 周吾
//
//===================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include "particle.h"

//======================================================
//	マクロ定義
//======================================================	
#define MAX_PARTICLE		(1240)		//パーティクルの最大数
#define MAX_PARTICLE_TEX	(2)			//パーティクルのテクスチャ数
#define PARTICLE_SIZE		(8.0f)		//パーティクルのサイズ
#define PARTICLE_LIFE		(60)		//パーティクルの寿命
#define PARTICLE_MOVEDOWN	(0.1f)		//パーティクルの重力

//======================================================
//	パーティクル構造体の定義
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
}Particle;

//======================================================
//	グローバル変数
//======================================================
LPDIRECT3DTEXTURE9 g_pTextureParticle[MAX_PARTICLE_TEX] = { NULL };		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;						//頂点バッファへのポインタ
Particle g_aParticle[MAX_PARTICLE];		//パーティクルの情報

//======================================================
//	パーティクルの初期化処理
//======================================================
void InitParticle(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/shadow001.png", &g_pTextureParticle[0]);		//星形
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/shadow000.jpg", &g_pTextureParticle[1]);		//丸型

	//パーティクル構造体の初期化
	ZeroMemory(&g_aParticle, sizeof(g_aParticle));

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_PARTICLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_PARTICLE; nCntEffect++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3((-PARTICLE_SIZE / 2) * g_aParticle[nCntEffect].scale.x, (PARTICLE_SIZE / 2) * g_aParticle[nCntEffect].scale.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3((PARTICLE_SIZE / 2) * g_aParticle[nCntEffect].scale.x, (PARTICLE_SIZE / 2) * g_aParticle[nCntEffect].scale.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3((-PARTICLE_SIZE / 2) * g_aParticle[nCntEffect].scale.x, (-PARTICLE_SIZE / 2) * g_aParticle[nCntEffect].scale.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3((PARTICLE_SIZE / 2) * g_aParticle[nCntEffect].scale.x, (-PARTICLE_SIZE / 2) * g_aParticle[nCntEffect].scale.y, 0.0f);

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
	g_pVtxBuffParticle->Unlock();
}

//=======================================================
//	パーティクルの終了処理
//=======================================================
void UninitParticle(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < MAX_PARTICLE_TEX; nCnt++)
	{
		if (g_pTextureParticle[nCnt] != NULL)
		{
			g_pTextureParticle[nCnt]->Release();
			g_pTextureParticle[nCnt] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}

//=========================================================
//	パーティクルの更新処理
//=========================================================
void UpdateParticle(void)
{
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse)
		{//パーティクルが使用中の場合

			//移動量の加算
			g_aParticle[nCntParticle].move.y -= PARTICLE_MOVEDOWN;		//重力負荷をかける
			g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;

			//エフェクトのライフ減少
			g_aParticle[nCntParticle].scale.x -= 0.01f;
			g_aParticle[nCntParticle].scale.y -= 0.01f;
			g_aParticle[nCntParticle].nLife--;

			//減衰
			g_aParticle[nCntParticle].move.x -= g_aParticle[nCntParticle].move.x * 0.1f;

			//頂点カラーの設定
			pVtx[0].col = g_aParticle[nCntParticle].col;
			pVtx[1].col = g_aParticle[nCntParticle].col;
			pVtx[2].col = g_aParticle[nCntParticle].col;
			pVtx[3].col = g_aParticle[nCntParticle].col;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3((-PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.x, (PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.x, (PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((-PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.x, (-PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.x, (-PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.y, 0.0f);

			if (g_aParticle[nCntParticle].nLife <= 0)
			{
				g_aParticle[nCntParticle].bUse = false;
			}
		}
		pVtx += 4;
	}
	//頂点座標をアンロック
	g_pVtxBuffParticle->Unlock();
}

//===============================================================
//	パーティクルの描画処理
//===============================================================
void DrawParticle(void)
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

	for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
	{
		if (g_aParticle[nCnt].bUse == true)
		{//弾が使用されている場合
			 //ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aParticle[nCnt].mtxWorld);

			//ビューマトリックスの設定
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//カメラの逆行列を設定
			g_aParticle[nCnt].mtxWorld._11 = mtxView._11;
			g_aParticle[nCnt].mtxWorld._12 = mtxView._21;
			g_aParticle[nCnt].mtxWorld._13 = mtxView._31;
			g_aParticle[nCnt].mtxWorld._21 = mtxView._12;
			g_aParticle[nCnt].mtxWorld._22 = mtxView._22;
			g_aParticle[nCnt].mtxWorld._23 = mtxView._32;
			g_aParticle[nCnt].mtxWorld._31 = mtxView._13;
			g_aParticle[nCnt].mtxWorld._32 = mtxView._23;
			g_aParticle[nCnt].mtxWorld._33 = mtxView._33;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aParticle[nCnt].pos.x, g_aParticle[nCnt].pos.y, g_aParticle[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aParticle[nCnt].mtxWorld, &g_aParticle[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aParticle[nCnt].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureParticle[g_aParticle[nCnt].nType]);

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
//	パーティクルの設定処理
//===========================================================
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR3 scale, int nType)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{
			g_aParticle[nCntParticle].pos = pos;		//位置
			g_aParticle[nCntParticle].move = move;		//移動量
			g_aParticle[nCntParticle].col = col;		//色
			g_aParticle[nCntParticle].scale = scale;	//大きさ

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3((-PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.x, (+PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((+PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.x, (+PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((-PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.x, (-PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((+PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.x, (-PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.y, 0.0f);

			g_aParticle[nCntParticle].nType = nType;			//種類
			g_aParticle[nCntParticle].nLife = PARTICLE_LIFE;	//寿命
			g_aParticle[nCntParticle].bUse = true;

			break;
		}
		pVtx += 4;
	}
	//頂点座標をアンロック
	g_pVtxBuffParticle->Unlock();
}