//===================================================================
//
//		壁の処理[wall.cpp]
//		Author:黒田 周吾
//
//===================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include "wall.h"
#include "meshfield.h"

//======================================================
//	マクロ定義
//======================================================
#define MAX_WALL			(16)		//影の最大数

//======================================================
//	グローバル宣言
//======================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;		//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;			//テクスチャへのポインタ
Wall g_aWall[MAX_WALL];		//壁の情報

//======================================================
//	壁の初期化処理
//======================================================
void InitWall(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wall001.jpg", &g_pTextureWall);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	//各情報の初期化
	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		g_aWall[nCnt].pos = D3DXVECTOR3(0.0f, 30.0f, 0.0f);
		g_aWall[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCnt].bUse = false;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-(WALL_WIDTH / 2), 0.0f,(WALL_HEIGHT / 2));
		pVtx[1].pos = D3DXVECTOR3((WALL_WIDTH / 2), 0.0f,(WALL_HEIGHT / 2));
		pVtx[2].pos = D3DXVECTOR3(-(WALL_WIDTH / 2), 0.0f,-(WALL_HEIGHT / 2));
		pVtx[3].pos = D3DXVECTOR3(+(WALL_WIDTH / 2), 0.0f,-(WALL_HEIGHT / 2));

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
	g_pVtxBuffWall->Unlock();

	//壁配置
	SetWall(D3DXVECTOR3(-(MESHFIELD_SIZE / 2), WALL_HEIGHT / 2, 0.0f), D3DXVECTOR3(D3DX_PI / 2, 0.0f, -D3DX_PI / 2));
	SetWall(D3DXVECTOR3((MESHFIELD_SIZE / 2), WALL_HEIGHT / 2, 0.0f), D3DXVECTOR3(D3DX_PI / 2, 0.0f, D3DX_PI / 2));
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT / 2, (MESHFIELD_SIZE / 2)), D3DXVECTOR3(D3DX_PI / 2, 0.0f, -D3DX_PI));
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT / 2, -(MESHFIELD_SIZE / 2)), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f));
}

//======================================================
//	壁の終了処理
//======================================================
void UninitWall(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
	//テクスチャの破棄
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}
}

//======================================================
//	壁の更新処理
//======================================================
void UpdateWall(void)
{
}

//======================================================
//	壁の描画処理
//======================================================
void DrawWall(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		if (g_aWall[nCnt].bUse == true)
		{//影が使用されている場合
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aWall[nCnt].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCnt].rot.y, g_aWall[nCnt].rot.x, g_aWall[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aWall[nCnt].mtxWorld, &g_aWall[nCnt].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCnt].pos.x, g_aWall[nCnt].pos.y, g_aWall[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aWall[nCnt].mtxWorld, &g_aWall[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCnt].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureWall);

			//プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				nCnt * 4,	//描画する最初の頂点インデックス
				2);			//プリミティブ（ポリゴン）数

		}
	}
	//テクスチャを元に戻す
	pDevice->SetTexture(0, NULL);
}

//======================================================
//	壁の設定処理
//======================================================
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		if (g_aWall[nCnt].bUse == false)
		{//影が使用されていない場合
			g_aWall[nCnt].pos = pos;
			g_aWall[nCnt].rot = rot;
			g_aWall[nCnt].bUse = true;

			break;
		}
	}
}