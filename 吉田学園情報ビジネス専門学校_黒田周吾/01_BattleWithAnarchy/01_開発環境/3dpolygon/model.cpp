//===================================================================
//
//	モデル処理[model.cpp]
//	Author:黒田 周吾
//
//===================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "meshfield.h"
#include "load.h"
#include "enemy.h"

//======================================================
//	マクロ定義
//======================================================
#define MODEL_MOVE		(1.4f)		//モデルの移動量
#define MAX_MODEL_TYPE	(10)		//モデルの最大種類数

//======================================================
//	構造体定義
//======================================================
//モデルの構造体
typedef struct
{
	LPD3DXMESH pMeshModel = NULL;		//メッシュ情報へのポインタ
	LPD3DXBUFFER pBuffMatModel = NULL;	//マテリアル情報へのポインタ
	DWORD nNumMatModel = 0;				//マテリアル情報の数
	bool bUse;							//使用しているかどうか
	LPDIRECT3DTEXTURE9 g_pTextureMeshField[5] = { NULL };			//テクスチャへのポインタ
}ModelType;

//======================================================
//	グローバル宣言
//======================================================
Model g_aModel[MAX_MODEL];					//モデルの情報
ModelType g_aModelXFile[MAX_MODEL_TYPE];	//モデルの種類情報

//======================================================
//	モデルの初期化処理
//======================================================
void InitModel(void)
{
	//構造体の要素を初期化
	ZeroMemory(&g_aModel, sizeof(g_aModel));
	ZeroMemory(&g_aModelXFile, sizeof(g_aModelXFile));
}

//======================================================
//	モデルの終了処理
//======================================================
void UninitModel(void)
{
	for (int nCnt = 0; nCnt < MAX_MODEL_TYPE; nCnt++)
	{
		//メッシュの破棄
		if (g_aModelXFile[nCnt].pMeshModel != NULL)
		{
			g_aModelXFile[nCnt].pMeshModel->Release();
			g_aModelXFile[nCnt].pMeshModel = NULL;
		}
		//マテリアルの破棄
		if (g_aModelXFile[nCnt].pBuffMatModel != NULL)
		{
			g_aModelXFile[nCnt].pBuffMatModel->Release();
			g_aModelXFile[nCnt].pBuffMatModel = NULL;
		}
	}
}

//======================================================
//	モデルの更新処理
//======================================================
void UpdateModel(void)
{
	for (int nCntVtx = 0; nCntVtx < MAX_MODEL_TYPE; nCntVtx++)
	{
		for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
		{
			//前回の位置を保存
			g_aModel[nCnt].posOld = g_aModel[nCnt].pos;

			//当たり判定
			//CollisionPlayer(&g_aModel[nCnt].pos, &g_aModel[nCnt].posOld, &g_aModel[nCnt].Airmove, g_aModel[nCnt].fWidth, g_aModel[nCnt].fHeight);
		}
	}
}

//======================================================
//	モデルの描画処理
//======================================================
void DrawModel(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		if (g_aModel[nCnt].bUse)
		{
			D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
			D3DMATERIAL9 matDef;			//現在のマテリアル保存用
			D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aModel[nCnt].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_aModel[nCnt].rot.y, g_aModel[nCnt].rot.x, g_aModel[nCnt].rot.z);

			D3DXMatrixMultiply(&g_aModel[nCnt].mtxWorld, &g_aModel[nCnt].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_aModel[nCnt].pos.x, g_aModel[nCnt].pos.y, g_aModel[nCnt].pos.z);

			D3DXMatrixMultiply(&g_aModel[nCnt].mtxWorld, &g_aModel[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aModel[nCnt].mtxWorld);

			//現在のマテリアルを保持
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aModelXFile[g_aModel[nCnt].nType].pBuffMatModel->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aModelXFile[g_aModel[nCnt].nType].nNumMatModel; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_aModelXFile[g_aModel[nCnt].nType].g_pTextureMeshField[nCntMat]);

				//モデルパーツの描画
				g_aModelXFile[g_aModel[nCnt].nType].pMeshModel->DrawSubset(nCntMat);

				//テクスチャの設定を元に戻す
				pDevice->SetTexture(0, NULL);
			}
			//保持していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//-----------------------------------------------------------------------------
//セットモデル処理
//-----------------------------------------------------------------------------
HRESULT SetModel(HWND hWnd, Model Model)
{
	int nNumVix;		//頂点数
	DWORD sizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		if (!g_aModel[nCnt].bUse)
		{//モデルが使用されていない場合
			g_aModel[nCnt].pos = Model.pos;
			g_aModel[nCnt].rot = Model.rot;
			g_aModel[nCnt].nType = Model.nType;
			g_aModel[nCnt].bUse = true;

			//頂点数の取得
			nNumVix = g_aModelXFile[g_aModel[nCnt].nType].pMeshModel->GetNumVertices();

			//頂点フォーマット取得
			sizeFVF = D3DXGetFVFVertexSize(g_aModelXFile[g_aModel[nCnt].nType].pMeshModel->GetFVF());

			//頂点ロック
			g_aModelXFile[g_aModel[nCnt].nType].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			//すべての頂点POSの取得
			for (int nCntVtx = 0; nCntVtx < nNumVix; nCntVtx++)
			{
				//頂点座標の代入
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				if (g_aModel[nCnt].vtxMax.x < vtx.x)
				{//X
					g_aModel[nCnt].vtxMax.x = vtx.x;
				}
				else if (g_aModel[nCnt].vtxMin.x > vtx.x)
				{
					g_aModel[nCnt].vtxMin.x = vtx.x;
				}

				if (g_aModel[nCnt].vtxMax.y < vtx.y)
				{//Y
					g_aModel[nCnt].vtxMax.y = vtx.y;
				}
				else if (g_aModel[nCnt].vtxMin.y > vtx.y)
				{
					g_aModel[nCnt].vtxMin.y = vtx.y;
				}

				if (g_aModel[nCnt].vtxMax.z < vtx.z)
				{//Z
					g_aModel[nCnt].vtxMax.z = vtx.z;
				}
				else if (g_aModel[nCnt].vtxMin.z > vtx.z)
				{
					g_aModel[nCnt].vtxMin.z = vtx.z;
				}

				//頂点フォーマットのサイズ分ポインタを進める
				pVtxBuff += sizeFVF;
			}

			//ラインを配置
			//SetLine(g_Model[nCnt].pos, g_Model[nCnt].vtxMax, g_Model[nCnt].vtxMin, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

			//頂点アンロック
			g_aModelXFile[g_aModel[nCnt].nType].pMeshModel->UnlockVertexBuffer();
			return S_OK;
		}
	}
	MessageBox(hWnd, "モデルの表示限界を超えました", "警告！", MB_ICONWARNING);
	return E_FAIL;
}

//-----------------------------------------------------------------------------
//モデルタイプ読み込み処理
//-----------------------------------------------------------------------------
HRESULT LoadModelType(HWND hWnd, char *sXFilePath)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスのポインタ

	for (int nCnt = 0; nCnt < MAX_MODEL_TYPE; nCnt++)
	{
		if (!g_aModelXFile[nCnt].bUse)
		{
			//Xファイルの読み込み
			D3DXLoadMeshFromX(sXFilePath,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_aModelXFile[nCnt].pBuffMatModel,
				NULL,
				&g_aModelXFile[nCnt].nNumMatModel,
				&g_aModelXFile[nCnt].pMeshModel);

			//テクスチャの読み込み処理
			LoadModelTexture(nCnt);

			g_aModelXFile[nCnt].bUse = true;

			return S_OK;
		}
	}

	MessageBox(hWnd, "読み込み可能モデル数を超えています", "警告！", MB_ICONWARNING);
	return E_FAIL;
}

//-----------------------------------------------------------------------------
//モデルタイプ読み込み処理
//-----------------------------------------------------------------------------
void LoadModelTexture(int nCnt)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスのポインタ

	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_aModelXFile[nCnt].pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_aModelXFile[nCnt].nNumMatModel; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		if (pMat[nCntMat].pTextureFilename != NULL && !strcmp(pMat[nCntMat].pTextureFilename, "") == 0)
		{//テクスチャがあった場合
			//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aModelXFile[nCnt].g_pTextureMeshField[nCntMat]);
		}
	}
}

//-----------------------------------------------------------------------------
//モデルとの当たり判定処理
//-----------------------------------------------------------------------------
bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float Width, float Height,int nType)
{
	bool bIsLanding = false;		//プレイヤーが着地しているか

	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		if (g_aModel[nCnt].bUse)
		{
			if (pPosOld->x <= g_aModel[nCnt].pos.x + g_aModel[nCnt].vtxMin.x		//前回の位置(X軸)がX軸の最小値以下
				&& pPos->x >= g_aModel[nCnt].pos.x + g_aModel[nCnt].vtxMin.x		//現在の位置(X軸)がX軸の最小値以上
				&& pPos->z <= g_aModel[nCnt].pos.z + g_aModel[nCnt].vtxMax.z		//Z軸の最大値より内側
				&& pPos->z >= g_aModel[nCnt].pos.z + g_aModel[nCnt].vtxMin.z		//Z軸の最小値より内側
				&& pPos->y <= g_aModel[nCnt].pos.y + g_aModel[nCnt].vtxMax.y		//Y軸の最大値より内側
				&& pPos->y >= g_aModel[nCnt].pos.y + g_aModel[nCnt].vtxMin.y)		//Y軸の最小値より内側
			{//左側の当たり判定(-X)
				pPos->x = g_aModel[nCnt].pos.x + g_aModel[nCnt].vtxMin.x;
				if (nType == 1)
				{//敵だったら
					pMove->y = 6.0f;		//ジャンプさせる
					bIsLanding = false;		//ジャンプ状態にする
					pPos->y += pMove->y;	//移動量の更新(Y)
				}
			}
			else if (pPosOld->x >= g_aModel[nCnt].pos.x + g_aModel[nCnt].vtxMax.x	//前回の位置(X軸)がX軸の最小値以上
				&& pPos->x <= g_aModel[nCnt].pos.x + g_aModel[nCnt].vtxMax.x		//現在の位置(X軸)がX軸の最小値以下
				&& pPos->z <= g_aModel[nCnt].pos.z + g_aModel[nCnt].vtxMax.z		//Z軸の最大値より内側
				&& pPos->z >= g_aModel[nCnt].pos.z + g_aModel[nCnt].vtxMin.z		//Z軸の最小値より内側
				&& pPos->y <= g_aModel[nCnt].pos.y + g_aModel[nCnt].vtxMax.y		//Y軸の最大値より内側
				&& pPos->y >= g_aModel[nCnt].pos.y + g_aModel[nCnt].vtxMin.y)		//Y軸の最小値より内側
			{//右側の当たり判定(+X)
				pPos->x = g_aModel[nCnt].pos.x + g_aModel[nCnt].vtxMax.x;
				if (nType == 1)
				{//敵だったら
					pMove->y = 6.0f;		//ジャンプさせる
					bIsLanding = false;		//ジャンプ状態にする
					pPos->y += pMove->y;	//移動量の更新(Y)
				}
			}

			if (pPosOld->z <= g_aModel[nCnt].pos.z + g_aModel[nCnt].vtxMin.z		//前回の位置(Z軸)がZ軸の最小値以下
				&& pPos->z >= g_aModel[nCnt].pos.z + g_aModel[nCnt].vtxMin.z		//現在の位置(Z軸)がZ軸の最小値以上
				&& pPos->x <= g_aModel[nCnt].pos.x + g_aModel[nCnt].vtxMax.x		//X軸の最大値より内側
				&& pPos->x >= g_aModel[nCnt].pos.x + g_aModel[nCnt].vtxMin.x		//X軸の最小値より内側
				&& pPos->y <= g_aModel[nCnt].pos.y + g_aModel[nCnt].vtxMax.y		//Y軸の最大値より内側
				&& pPos->y >= g_aModel[nCnt].pos.y + g_aModel[nCnt].vtxMin.y)		//Y軸の最小値より内側
			{//手前側の当たり判定(-Z)
				pPos->z = g_aModel[nCnt].pos.z + g_aModel[nCnt].vtxMin.z;
				if (nType == 1)
				{//敵だったら
					pMove->y = 6.0f;		//ジャンプさせる
					bIsLanding = false;		//ジャンプ状態にする
					pPos->y += pMove->y;	//移動量の更新(Y)
				}
			}
			else if (pPosOld->z >= g_aModel[nCnt].pos.z + g_aModel[nCnt].vtxMax.z	//前回の位置(Z軸)がZ軸の最小値以上
				&& pPos->z <= g_aModel[nCnt].pos.z + g_aModel[nCnt].vtxMax.z		//現在の位置(Z軸)がZ軸の最小値以下
				&& pPos->x <= g_aModel[nCnt].pos.x + g_aModel[nCnt].vtxMax.x		//X軸の最大値より内側
				&& pPos->x >= g_aModel[nCnt].pos.x + g_aModel[nCnt].vtxMin.x		//X軸の最小値より内側
				&& pPos->y <= g_aModel[nCnt].pos.y + g_aModel[nCnt].vtxMax.y		//Y軸の最大値より内側
				&& pPos->y >= g_aModel[nCnt].pos.y + g_aModel[nCnt].vtxMin.y)		//Y軸の最小値より内側
			{//奥側の当たり判定(+Z)
				pPos->z = g_aModel[nCnt].pos.z + g_aModel[nCnt].vtxMax.z;
				if (nType == 1)
				{//敵だったら
					pMove->y = 6.0f;		//ジャンプさせる
					bIsLanding = false;		//ジャンプ状態にする
					pPos->y += pMove->y;	//移動量の更新(Y)
				}
			}

			if (pPosOld->y >= g_aModel[nCnt].pos.y + g_aModel[nCnt].vtxMax.y		//前回の位置(Y軸)がY軸の最小値以上
				&& pPos->y <= g_aModel[nCnt].pos.y + g_aModel[nCnt].vtxMax.y		//現在の位置(Y軸)がY軸の最小値以下
				&& pPos->x <= g_aModel[nCnt].pos.x + g_aModel[nCnt].vtxMax.x		//X軸の最大値より内側
				&& pPos->x >= g_aModel[nCnt].pos.x + g_aModel[nCnt].vtxMin.x		//X軸の最小値より内側
				&& pPos->z <= g_aModel[nCnt].pos.z + g_aModel[nCnt].vtxMax.z		//Z軸の最大値より内側
				&& pPos->z >= g_aModel[nCnt].pos.z + g_aModel[nCnt].vtxMin.z)		//Z軸の最小値より内側
			{//上側の当たり判定(-Y)
				pPos->y = g_aModel[nCnt].pos.y + g_aModel[nCnt].vtxMax.y;
				pMove->y = 0.0f;		//Yの移動量を0にする
				bIsLanding = true;		//着地状態にする
			}
			else if (pPosOld->y <= g_aModel[nCnt].pos.y + g_aModel[nCnt].vtxMin.y	//前回の位置(Y軸)がY軸の最小値以下
				&& pPos->y >= g_aModel[nCnt].pos.y + g_aModel[nCnt].vtxMin.y		//現在の位置(Y軸)がY軸の最小値以上
				&& pPos->x <= g_aModel[nCnt].pos.x + g_aModel[nCnt].vtxMax.x		//X軸の最大値より内側
				&& pPos->x >= g_aModel[nCnt].pos.x + g_aModel[nCnt].vtxMin.x		//X軸の最小値より内側
				&& pPos->z <= g_aModel[nCnt].pos.z + g_aModel[nCnt].vtxMax.z		//Z軸の最大値より内側
				&& pPos->z >= g_aModel[nCnt].pos.z + g_aModel[nCnt].vtxMin.z)		//Z軸の最小値より内側
			{//下側の当たり判定(+Y)
				pPos->y = g_aModel[nCnt].pos.y + g_aModel[nCnt].vtxMin.y;
			}
		}
	}
	return bIsLanding;
}

//======================================================
//	モデルの情報を取得
//======================================================
Model *GetModel(void)
{
	return &g_aModel[0];
}