//===================================================================
//
//	カメラ処理[camera.cpp]
//	Author:黒田 周吾
//
//===================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include "camera.h"
#include "input.h"
#include "player.h"

//======================================================
//	グローバル変数
//======================================================
Camera g_camera[MAX_CAMERA];	//カメラ情報

//======================================================
//	カメラの初期化処理
//======================================================
void InitCamera(void)
{
	float fAngle = 0.0f;

	for (int nCntCamera = 0; nCntCamera < MAX_CAMERA; nCntCamera++, fAngle += D3DX_PI)
	{
		//視点・注視点・上方向を設定する
		g_camera[nCntCamera].posV = D3DXVECTOR3(0.0f, 130.0f, -230.0f);
		g_camera[nCntCamera].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera[nCntCamera].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//固定
		g_camera[nCntCamera].posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera[nCntCamera].posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//注視点までの距離
		D3DXVECTOR3 Distance = D3DXVECTOR3((g_camera[nCntCamera].posV.x + g_camera[nCntCamera].posR.x),
			(g_camera[nCntCamera].posV.y + g_camera[nCntCamera].posR.y),
			(g_camera[nCntCamera].posV.z + g_camera[nCntCamera].posR.z));

		//向き・視点から注視点までの距離を初期化する
		g_camera[nCntCamera].rot = D3DXVECTOR3(atan2f(Distance.z, Distance.y), fAngle, 0.0f);
		g_camera[nCntCamera].fDistance = sqrtf(Distance.x * Distance.x + Distance.z * Distance.z);
		g_camera[nCntCamera].fDistance = sqrtf(Distance.y * Distance.y + g_camera[nCntCamera].fDistance * g_camera[nCntCamera].fDistance);
	}
}

//======================================================
//	カメラの終了処理
//======================================================
void UninitCamera(void)
{

}

//======================================================
//	カメラの更新処理
//======================================================
void UpdateCamera(void)
{
	//プレイヤー情報の取得
	Player *pPlayer = GetPlayer();

	for (int nCntCamera = 0; nCntCamera < MAX_CAMERA; nCntCamera++, pPlayer++)
	{
		//プレイヤー追従
		g_camera[nCntCamera].posRDest.x = (pPlayer->pos.x) + sinf(pPlayer->rot.x) * sinf(pPlayer->rot.y);
		g_camera[nCntCamera].posRDest.z = (pPlayer->pos.z) + sinf(pPlayer->rot.x) * cosf(pPlayer->rot.y);
		g_camera[nCntCamera].posRDest.y = (pPlayer->pos.y + 50.0f) + cosf(pPlayer->rot.x);

		g_camera[nCntCamera].posVDest.x = pPlayer->pos.x + sinf(g_camera[nCntCamera].rot.x) * sinf(g_camera[nCntCamera].rot.y) * g_camera[nCntCamera].fDistance;
		g_camera[nCntCamera].posVDest.z = pPlayer->pos.z + sinf(g_camera[nCntCamera].rot.x) * cosf(g_camera[nCntCamera].rot.y) * g_camera[nCntCamera].fDistance;
		g_camera[nCntCamera].posVDest.y = (pPlayer->pos.y + 50.0f) + cosf(g_camera[nCntCamera].rot.x) * g_camera[nCntCamera].fDistance;

		//カメラ位置の更新(目的の位置への更新)
		g_camera[nCntCamera].posR += (g_camera[nCntCamera].posRDest - g_camera[nCntCamera].posR) * 0.1f;
		g_camera[nCntCamera].posV += (g_camera[nCntCamera].posVDest - g_camera[nCntCamera].posV) * 0.1f;

		//角度の正規化
		if (g_camera[nCntCamera].rot.y > D3DX_PI)
		{
			g_camera[nCntCamera].rot.y -= D3DX_PI * 2;
		}
		if (g_camera[nCntCamera].rot.y < -D3DX_PI)
		{
			g_camera[nCntCamera].rot.y += D3DX_PI * 2;
		}
	}
}

//======================================================
//	カメラの設定処理
//======================================================
void SetCamera(int nCntCamera)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera[nCntCamera].mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_camera[nCntCamera].mtxView, &g_camera[nCntCamera].posV, &g_camera[nCntCamera].posR, &g_camera[nCntCamera].vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera[nCntCamera].mtxView);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_camera[nCntCamera].mtxProjection,
		D3DXToRadian(45.0f),
		(float)(SCREEN_WIDTH / 2) / (float)SCREEN_HEIGHT,
		10.0f,
		6400.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera[nCntCamera].mtxProjection);
}

//======================================================
//	カメラの情報を取得
//======================================================
Camera *GetCamera(void)
{
	return &g_camera[0];
}