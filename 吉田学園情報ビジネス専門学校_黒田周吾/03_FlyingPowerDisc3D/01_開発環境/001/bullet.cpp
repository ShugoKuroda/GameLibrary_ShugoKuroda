//===================================================================
//
//	ディスク処理[bullet.cpp]
//	Author:黒田 周吾
//
//===================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include "input.h"
#include "bullet.h"
#include "effect.h"
#include "meshfield.h"
#include "player.h"
#include "score.h"
#include "sound.h"

//======================================================
//	マクロ定義
//======================================================
#define MAX_BULLET			(1)			//弾の最大数
#define BULLET_TYPE			(1)			//弾の種類

//================================================
//	グローバル変数
//================================================
BulletInfo g_BulletInfo;			//弾のモデル(Xファイル)情報
Bullet g_aBullet[MAX_BULLET];		//弾の情報

//================================================
//	弾の初期化処理
//================================================
void InitBullet(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//Xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/model/disc000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_BulletInfo.g_pBuffMatModel,
		NULL,
		&g_BulletInfo.g_nNumMatModel,
		&g_BulletInfo.g_pMeshModel);

	//弾情報の初期化
	ZeroMemory(&g_aBullet, sizeof(g_aBullet));
}

//======================================================
//	弾の終了処理
//======================================================
void UninitBullet(void)
{
	//メッシュの破棄
	if (g_BulletInfo.g_pMeshModel != NULL)
	{
		g_BulletInfo.g_pMeshModel->Release();
		g_BulletInfo.g_pMeshModel = NULL;
	}
	//マテリアルの破棄
	if (g_BulletInfo.g_pBuffMatModel != NULL)
	{
		g_BulletInfo.g_pBuffMatModel->Release();
		g_BulletInfo.g_pBuffMatModel = NULL;
	}
}

//======================================================
//	弾の更新処理
//======================================================
void UpdateBullet(void)
{
	//発射処理
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse)
		{//弾が使用されている場合

			//弾の位置の更新
			switch (g_aBullet[nCntBullet].nType)
			{
			case 0:
				//位置を更新処理
				g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
				g_aBullet[nCntBullet].pos.z += g_aBullet[nCntBullet].move.z;
				break;

				//case 1:
				//	//位置を更新処理
				//	g_aBullet[nCntBullet].pos.x -= g_aBullet[nCntBullet].move.x;
				//	g_aBullet[nCntBullet].pos.z -= g_aBullet[nCntBullet].move.z;
				//	break;

			default:
				break;
			}

			//壁・床の反射処理
			if (g_aBullet[nCntBullet].pos.x <= -FIELD_SIZE_X / 2)
			{//左壁
				g_aBullet[nCntBullet].move = ReflectionBullet(g_aBullet[nCntBullet].move, D3DXVECTOR3(1.0f, 0.0f, 0.0f));
			}
			else if (g_aBullet[nCntBullet].pos.x >= FIELD_SIZE_X / 2)
			{//右壁
				g_aBullet[nCntBullet].move = ReflectionBullet(g_aBullet[nCntBullet].move, D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
			}
			if (g_aBullet[nCntBullet].pos.z > (FIELD_SIZE_Z / 2) + 30.0f)
			{//奥壁
				SetPoint(PLAYER_1, g_aBullet[nCntBullet].pos);
				g_aBullet[nCntBullet].pos.z = (FIELD_SIZE_Z / 2) + 30.0f;
				g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
			else if (g_aBullet[nCntBullet].pos.z < (-FIELD_SIZE_Z / 2) - 30.0f)
			{//手前壁
				SetPoint(PLAYER_2, g_aBullet[nCntBullet].pos);
				g_aBullet[nCntBullet].pos.z = (-FIELD_SIZE_Z / 2) - 30.0f;
				g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}
}

//======================================================
//	弾の描画処理
//======================================================
void DrawBullet(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	//弾のワールドマトリックス設定
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse)
		{//弾が使用されている場合

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBullet[nCntBullet].rot.y, g_aBullet[nCntBullet].rot.x, g_aBullet[nCntBullet].rot.z);
			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);
		}
	}

	//現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_BulletInfo.g_pBuffMatModel->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)g_BulletInfo.g_nNumMatModel; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//モデルパーツの描画
		g_BulletInfo.g_pMeshModel->DrawSubset(nCntMat);
	}

	//保持していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//======================================================
//	弾の設定処理
//======================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int Type)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (!g_aBullet[nCntBullet].bUse)
		{//弾が使用されていない場合
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].nType = Type;
			g_aBullet[nCntBullet].bUse = true;

			break;
		}
	}
}

//======================================================
//	弾の反射処理
//======================================================
D3DXVECTOR3 ReflectionBullet(const D3DXVECTOR3 move, const D3DXVECTOR3 nor)
{
	D3DXVECTOR3 vecMove = nor * -(D3DXVec3Dot(&move, &nor));	//内積を求める(弾の移動ベクトルと、標準化した壁の法線ベクトル)
	vecMove = move + (vecMove * 2.0f);				//反射ベクトルを求める

	//サウンド再生
	PlaySound(SOUND_LABEL_SE_REVERSE);

	return vecMove;
}
//======================================================
//	弾の情報を取得
//======================================================
Bullet *GetBullet(void)
{
	return &g_aBullet[0];
}
