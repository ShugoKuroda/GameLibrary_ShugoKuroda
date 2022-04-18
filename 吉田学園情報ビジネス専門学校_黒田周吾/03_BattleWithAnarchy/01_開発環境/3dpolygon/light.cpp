//===================================================================
//
//		ライト処理[light.cpp]
//		Author:黒田 周吾
//
//===================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include "light.h"

//======================================================
//	マクロ定義
//======================================================
#define MAX_LIGHT	(2)		//ライトの最大数

//======================================================
//	グローバル変数
//======================================================
D3DLIGHT9 g_aLight[MAX_LIGHT];		//ライト情報

//======================================================
//	ライトの初期化処理
//======================================================
void InitLight(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;		//ライト方向ベクトル

	//ライトをクリアする
	ZeroMemory(&g_aLight[0], sizeof(g_aLight));

	////ライト１の設定
	//ライトの種類を設定
	g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光を設定
	g_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//ライトの方向を設定
	vecDir = D3DXVECTOR3(-0.2f, -0.8f, 0.4f);

	//正規化する(大きさ１のベクトルにする)
	D3DXVec3Normalize(&vecDir, &vecDir);
	g_aLight[0].Direction = vecDir;

	//ライトを設定 
	pDevice->SetLight(0, &g_aLight[0]);

	//ライトを有効にする
	pDevice->LightEnable(0, TRUE);


	////ライト２の設定
	//ライトの種類を設定
	g_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光を設定
	g_aLight[1].Diffuse = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);

	//ライトの方向を設定
	vecDir = D3DXVECTOR3(0.2f, -0.1f, -0.8f);

	//正規化する(大きさ１のベクトルにする)
	D3DXVec3Normalize(&vecDir, &vecDir);
	g_aLight[1].Direction = vecDir;

	//ライトを設定
	pDevice->SetLight(1, &g_aLight[1]);

	//ライトを有効にする
	pDevice->LightEnable(1, TRUE);
}

//======================================================
//	ライトの終了処理
//======================================================
void UninitLight(void)
{

}

//======================================================
//	ライトの更新処理
//======================================================
void UpdateLight(void)
{

}
