//=============================================================================
//
// メッシュ(球)処理 [meshcylinder.cpp]
// Author : SHUGO KURODA
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "meshcylinder.h"

//-----------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------
#define CYLINDER_MAX				(256)		//球体の最大数

//-----------------------------------------------------------------------------
//構造体の定義
//-----------------------------------------------------------------------------
//メッシュ構造体
typedef struct
{
	D3DXVECTOR3 pos;					//位置
	D3DXVECTOR3 rot;					//向き
	D3DXCOLOR col;						//カラー
	float fRadiusX; float fRadiusZ;		//各横縦幅
	int nMeshX; int nMeshZ;				//縦横のメッシュ化の数
	int nPolygon;						//ポリゴン数
	int nTop;							//頂点
	int nIdx;							//インデックス数
	D3DXMATRIX mtxWorld;				//ワールドマトリックス
	TexturetCylinder Texturetstate;	//テクスチャの状態
	bool bUse;							//使用しているかどうか
}CylinderStructure;

//-----------------------------------------------------------------------------
//グローバル変数宣言
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCylinder = NULL;//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffCylinder = NULL;	//インデックスバッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureCylinder[CYLINDER_TEXTURE_MAX] = { NULL };//テクスチャへのポインタ
CylinderStructure g_aCylinder[CYLINDER_MAX];//メッシュ構造体

//*****************************************************************************
//	メッシュ初期化
//*****************************************************************************
void InitMeshCylinder(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//メッシュのテクスチャの初期化
	ZeroMemory(&g_pTextureCylinder[0], sizeof(g_pTextureCylinder));

	//テクスチャの読み込み(0番目はNULL)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/sky000.jpg",
		&g_pTextureCylinder[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * CYLINDER_MAX,//確保する頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffCylinder,
		NULL);

	//頂点のインデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * 4 * CYLINDER_MAX,//確保する頂点の数
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pIdxBuffCylinder,
		NULL);

	//メッシュ構造体の初期化
	ZeroMemory(&g_aCylinder[0], sizeof(g_aCylinder));

	//初期メッシュの配置
	SetCylinder(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		3000.0f, 3000.0f, 10, 10, CYLINDER_TEXTURE_1);
}

//*****************************************************************************
//	メッシュの終了処理
//*****************************************************************************
void UninitMeshCylinder(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < CYLINDER_TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureCylinder[nCnt] != NULL)
		{
			g_pTextureCylinder[nCnt]->Release();
			g_pTextureCylinder[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffCylinder != NULL)
	{
		g_pVtxBuffCylinder->Release();
		g_pVtxBuffCylinder = NULL;
	}

	//インデックスバッファの破棄
	if (g_pIdxBuffCylinder != NULL)
	{
		g_pIdxBuffCylinder->Release();
		g_pIdxBuffCylinder = NULL;
	}
}

//*****************************************************************************
//	メッシュの更新処理
//*****************************************************************************
void UpdateMeshCylinder(void)
{
	for (int nCnt = 0; nCnt < CYLINDER_MAX; nCnt++)
	{
		if (g_aCylinder[nCnt].bUse)
		{//使用していたら
			g_aCylinder[nCnt].rot.x += 0.001f;
		}
	}
}

//*****************************************************************************
//	メッシュの描画処理
//*****************************************************************************
void DrawMeshCylinder(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//カリングOFF
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (int nCnt = 0; nCnt < CYLINDER_MAX; nCnt++)
	{
		if (g_aCylinder[nCnt].bUse)
		{
			D3DXMATRIX mtxRot, mtxTrans;		//計算用のマトリックス

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aCylinder[nCnt].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_aCylinder[nCnt].rot.y, g_aCylinder[nCnt].rot.x, g_aCylinder[nCnt].rot.z);

			D3DXMatrixMultiply(&g_aCylinder[nCnt].mtxWorld, &g_aCylinder[nCnt].mtxWorld, &mtxRot);

			//位置の反映
			D3DXMatrixTranslation(&mtxTrans, g_aCylinder[nCnt].pos.x, g_aCylinder[nCnt].pos.y, g_aCylinder[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aCylinder[nCnt].mtxWorld, &g_aCylinder[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aCylinder[nCnt].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffCylinder, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffCylinder);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureCylinder[g_aCylinder[nCnt].Texturetstate]);

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				0,							//描画する最初の頂点インデックス
				0,							//インデックスの最小値
				g_aCylinder[nCnt].nTop,		//頂点の数
				0,							//インデックスの最初の数
				g_aCylinder[nCnt].nPolygon);		//プリミティブ（ポリゴン）数
		}
	}
	//カリングを元に戻す(ON)
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//-----------------------------------------------------------------------------
//	セットメッシュ処理
//-----------------------------------------------------------------------------
void SetCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadiusX, float fRadiusZ, int nMeshX, int nMeshZ, TexturetCylinder Texturetstate)
{
	VERTEX_3D *pVtx = NULL;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCylinder->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < CYLINDER_MAX; nCnt++)
	{
		if (!g_aCylinder[nCnt].bUse)
		{//条件が偽だったら

			//構造体を使用状態にする
			g_aCylinder[nCnt].bUse = true;
			g_aCylinder[nCnt].pos = pos;
			g_aCylinder[nCnt].rot = rot;
			g_aCylinder[nCnt].col = col;
			g_aCylinder[nCnt].fRadiusX = fRadiusX;
			g_aCylinder[nCnt].fRadiusZ = fRadiusZ;
			g_aCylinder[nCnt].nMeshX = nMeshX;
			g_aCylinder[nCnt].nMeshZ = nMeshZ;
			g_aCylinder[nCnt].Texturetstate = Texturetstate;
			g_aCylinder[nCnt].nTop = (nMeshX + 1) * (nMeshZ + 1);
			g_aCylinder[nCnt].nIdx = (nMeshX + 1) * 2 * nMeshZ + (nMeshZ - 1) * 2;
			g_aCylinder[nCnt].nPolygon = nMeshX * nMeshZ * 2 + (nMeshZ - 1) * 4;

			for (int nCntZ = 0; nCntZ <= nMeshZ; nCntZ++)
			{
				float fData = fRadiusZ * cosf((D3DX_PI/ nMeshZ * nCntZ) - D3DX_PI / 2);
				for (int nCntX = 0; nCntX <= nMeshX; nCntX++)
				{
					pVtx[0].pos.x = fData * sinf(D3DX_PI * 2 / nMeshX * nCntX);
					pVtx[0].pos.y = fRadiusZ * sinf((D3DX_PI / nMeshZ  * nCntZ) - D3DX_PI / 2);
					pVtx[0].pos.z = fData * cosf(D3DX_PI * 2 / nMeshX * nCntX);

					//各頂点の法線の設定（ベクトルの大きさは１にする必要がある）
					pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

					//頂点カラーの設定
					pVtx[0].col = g_aCylinder[nCnt].col;

					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2((1.0f / nMeshX) * nCntX, (1.0f / nMeshZ) * nCntZ);

					pVtx++; //データを1つ分進める
				}
			}

			//インデックスバッファをロック
			WORD* pIdx;
			g_pIdxBuffCylinder->Lock(0, 0, (void**)&pIdx, 0);

			for (int nCntZ = 0; nCntZ <= nMeshZ; nCntZ++)
			{
				for (int nCntX = 0; nCntX <= nMeshX; nCntX++)
				{
					//インデックスの設定
					pIdx[0] = (WORD)(nCntX + (nMeshX + 1) * (nCntZ + 1));
					pIdx[1] = (WORD)(nCntX + (nMeshX + 1) * nCntZ);
					pIdx += 2;//インデックスのデータを２すすめる
				}

				if (nCntZ < nMeshZ - 1)
				{
					//インデックスの設定
					pIdx[0] = pIdx[-1];
					pIdx[1] = (WORD)((nMeshX + 1) * (nCntZ + 2));
					pIdx += 2;//インデックスのデータを２すすめる
				}
			}
			g_pIdxBuffCylinder->Unlock();
			break;
		}

		pVtx += 4; //データを４つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffCylinder->Unlock();
}