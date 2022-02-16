//===================================================================
//
//		カメラ処理[camera.cpp]
//		Author:黒田 周吾
//
//===================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include "input.h"
#include "inputMouse.h"
#include "camera.h"
#include "player.h"
#include "model.h"

//======================================================
//	グローバル変数
//======================================================
Camera g_camera;	//カメラ情報
float g_AngleX;

//======================================================
//	カメラの初期化処理
//======================================================
void InitCamera(void)
{
	//視点・注視点・上方向を設定する
	g_camera.posV = D3DXVECTOR3(0.0f, 30.0f, -150.0f);
	g_camera.posR = D3DXVECTOR3(0.0f, 90.0f, 0.0f);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//固定
	g_camera.posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//注視点までの距離
	D3DXVECTOR3 Distance = D3DXVECTOR3((g_camera.posV.x + g_camera.posR.x), (g_camera.posV.y + g_camera.posR.y), (g_camera.posV.z + g_camera.posR.z));

	//向き・視点から注視点までの距離を初期化する
	g_AngleX = atan2f(Distance.z, Distance.y);
	g_camera.rot = D3DXVECTOR3(g_AngleX, 0.0f, 0.0f);
	g_camera.fDistance = sqrtf(Distance.x * Distance.x + Distance.z * Distance.z);
	g_camera.fDistance = sqrtf(Distance.y * Distance.y + g_camera.fDistance * g_camera.fDistance);
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

	//プレイヤー情報の取得
	Model *pModel = GetModel();

	//マウスの移動量情報の取得
	DIMOUSESTATE *pMouse = GetMouseMove();

	//if (GetKeyboardPress(DIK_V) == true)
	//{//ロックオン
	//	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++, pModel++)
	//	{
	//		if (pModel->bUse)
	//		{
	//			g_camera.posRDest.x = pModel->pos.x + sinf(pModel->rot.x) * sinf(pModel->rot.y);
	//			g_camera.posRDest.z = pModel->pos.z + sinf(pModel->rot.x) * cosf(pModel->rot.y);
	//			g_camera.posRDest.y = pModel->pos.y + cosf(pModel->rot.x);

	//			D3DXVECTOR3 vecCamera = (pPlayer->pos - g_camera.posR) * 1.5f;
	//			D3DXMATRIX mtxRot;

	//			//カメラのの向きを反映
	//			D3DXMatrixRotationYawPitchRoll(&mtxRot, -D3DX_PI / 12.0f, 0.0f, 0.0f);

	//			D3DXVec3TransformCoord(&vecCamera, &vecCamera, &mtxRot);

	//			g_camera.posVDest = g_camera.posR + vecCamera;
	//		}
	//	}
	//}
	if (!pPlayer->bIsDie)
	{//プレイヤーが死亡したらカメラ操作、追従をOFFにする
		//注視点をマウス操作で変える
		g_camera.rot.y += (D3DX_PI / 180.0f) * (pMouse->lX * 0.25f);
		g_camera.rot.x += (D3DX_PI / 180.0f) * (pMouse->lY * 0.25f);
		if (g_camera.rot.x >= (D3DX_PI / 8.0f) - 0.4f)
		{
			g_camera.rot.x = (D3DX_PI / 8.0f) - 0.4f;
		}
		else if (g_camera.rot.x <= -1.8f)
		{
			g_camera.rot.x = -1.8f;
		}
		g_camera.posV.x = g_camera.posR.x + sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z + sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.y = g_camera.posR.y + cosf(g_camera.rot.x) * g_camera.fDistance;

		//プレイヤー追従
		g_camera.posRDest.x = pPlayer->pos.x + sinf(pPlayer->rot.x) * sinf(pPlayer->rot.y);
		g_camera.posRDest.z = pPlayer->pos.z + sinf(pPlayer->rot.x) * cosf(pPlayer->rot.y);
		g_camera.posRDest.y = (pPlayer->pos.y + 50.0f) + cosf(pPlayer->rot.x);
		g_camera.posVDest.x = pPlayer->pos.x + sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posVDest.z = pPlayer->pos.z + sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posVDest.y = (pPlayer->pos.y + 50.0f) + cosf(g_camera.rot.x) * g_camera.fDistance;
	}

	//カメラ位置の更新
	g_camera.posR.x += (g_camera.posRDest.x - g_camera.posR.x) * 0.3f;
	g_camera.posR.z += (g_camera.posRDest.z - g_camera.posR.z) * 0.3f;
	g_camera.posR.y += (g_camera.posRDest.y - g_camera.posR.y) * 0.3f;
	g_camera.posV.x += (g_camera.posVDest.x - g_camera.posV.x) * 0.3f;
	g_camera.posV.z += (g_camera.posVDest.z - g_camera.posV.z) * 0.3f;
	g_camera.posV.y += (g_camera.posVDest.y - g_camera.posV.y) * 0.3f;

	//角度の正規化
	if (g_camera.rot.y > D3DX_PI)
	{
		g_camera.rot.y -= D3DX_PI * 2;
	}
	if (g_camera.rot.y < -D3DX_PI)
	{
		g_camera.rot.y += D3DX_PI * 2;
	}
}

//======================================================
//	カメラの設定処理
//======================================================
void SetCamera(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_camera.mtxView, &g_camera.posV, &g_camera.posR, &g_camera.vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		6400.0f);			//視界の設定

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);
}

//======================================================
//	カメラの情報を取得
//======================================================
Camera *GetCamera(void)
{
	return &g_camera;
}