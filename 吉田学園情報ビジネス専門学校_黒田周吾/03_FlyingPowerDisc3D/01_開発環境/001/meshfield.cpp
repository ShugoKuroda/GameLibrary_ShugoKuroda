//===================================================================
//
//		メッシュフィールド処理[meshfield.cpp]
//		Author:黒田 周吾
//
//===================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include "meshfield.h"
#include "input.h"

//======================================================
//	マクロ定義
//======================================================
#define MESHFIELD_X_BLOCK			(7)			//X方向のブロック数
#define MESHFIELD_Z_BLOCK			(7)			//Z方向のブロック数
#define MESHFIELD_VERTEX_NUM		((MESHFIELD_X_BLOCK + 1) * (MESHFIELD_Z_BLOCK + 1))			//頂点数
#define MESHFIELD_INDEX_NUM			((MESHFIELD_X_BLOCK + 1) * 2 * MESHFIELD_Z_BLOCK + (MESHFIELD_Z_BLOCK - 1) * 2)		//インデックス数
#define MESHFIELD_PRIMITIVE_NUM		(MESHFIELD_X_BLOCK * MESHFIELD_Z_BLOCK * 2 + (MESHFIELD_Z_BLOCK - 1) * 4)			//ポリゴン数

//======================================================
//	グローバル宣言
//======================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;		//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureMeshField = NULL;			//テクスチャへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;		//インデックスバッファへのポインタ
D3DXVECTOR3 g_posMeshField;		//位置
D3DXVECTOR3 g_rotMeshField;		//向き
D3DXMATRIX	g_mtxWorldMeshField;	//ワールドマトリックス

//======================================================
//	メッシュフィールドの初期化処理
//======================================================
void InitMeshField(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHFIELD_VERTEX_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/glass000.jpg", &g_pTextureMeshField);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHFIELD_INDEX_NUM,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL);

	VERTEX_3D *pVtx = NULL;		//頂点情報へのポインタ
	WORD *pIdx;			//インデックス情報へのポインタ

	g_posMeshField = D3DXVECTOR3(-MESHFIELD_SIZE / 4, 0.0f, MESHFIELD_SIZE / 4);
	g_rotMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	//各頂点座標の設定
	for (int nCntBlockZ = 0; nCntBlockZ < MESHFIELD_Z_BLOCK + 1; nCntBlockZ++)
	{//Z方向の座標設定
		for (int nCntBlockX = 0; nCntBlockX < MESHFIELD_X_BLOCK + 1; pVtx++, nCntBlockX++)
		{//X方向の座標設定
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_posMeshField.x + (MESHFIELD_SIZE / MESHFIELD_X_BLOCK) * nCntBlockX, g_posMeshField.y, g_posMeshField.z - (MESHFIELD_SIZE / MESHFIELD_Z_BLOCK) * nCntBlockZ);
			//各頂点の法線の設定(ベクトルの大きさは１にする必要がある)
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2((1.0f / MESHFIELD_X_BLOCK) * nCntBlockX, (1.0f / MESHFIELD_Z_BLOCK) * nCntBlockZ);
		}
	}

	//頂点バッファをアンロック
	g_pVtxBuffMeshField->Unlock();

	int nCntIdxX;
	int nCntIdxZ;

	//インデックスバッファをロック
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	//インデックスの設定
	for (nCntIdxZ = 0; nCntIdxZ < MESHFIELD_Z_BLOCK; pIdx += 2, nCntIdxZ++)
	{//Z方向のインデックス設定
		for (nCntIdxX = 0; nCntIdxX < MESHFIELD_X_BLOCK + 1; pIdx += 2, nCntIdxX++)
		{//X方向のインデックス設定
			pIdx[0] = nCntIdxX + ((MESHFIELD_X_BLOCK + 1) * nCntIdxZ) + MESHFIELD_X_BLOCK + 1;
			pIdx[1] = nCntIdxX + ((MESHFIELD_X_BLOCK + 1) * nCntIdxZ);
		}
		//縮退ポリゴン用のインデックス設定
		if (nCntIdxZ <= MESHFIELD_Z_BLOCK - 2)
		{//最後のインデックス番号ではない時
			pIdx[0] = (nCntIdxX - 1) + ((MESHFIELD_X_BLOCK + 1) * nCntIdxZ);
			pIdx[1] = (nCntIdxX + (MESHFIELD_X_BLOCK + 1)) + ((MESHFIELD_X_BLOCK + 1) * nCntIdxZ);
		}
	}
	//インデックスバッファのアンロック
	g_pIdxBuffMeshField->Unlock();
}

//======================================================
//	メッシュフィールドの終了処理
//======================================================
void UninitMeshField(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}
	//テクスチャの破棄
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}
	//インデックスバッファの解放
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//======================================================
//	メッシュフィールドの更新処理
//======================================================
void UpdateMeshField(void)
{
	//デバイスの取得
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();

//	int nCntMesh;
	VERTEX_3D *pVtx = NULL;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	//頂点バッファをアンロック
	g_pVtxBuffMeshField->Unlock();
}

//======================================================
//	メッシュフィールドの描画処理
//======================================================
void DrawMeshField(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshField);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshField.y, g_rotMeshField.x, g_rotMeshField.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posMeshField.x, g_posMeshField.y, g_posMeshField.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshField);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshField);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshField);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,			//描画する最初のインデックス
		0,			//バッファの最小値
		MESHFIELD_VERTEX_NUM,		//描画する最初の頂点インデックス
		0,			//インデックスの最初の数
		MESHFIELD_PRIMITIVE_NUM);	//プリミティブ（ポリゴン）数
}