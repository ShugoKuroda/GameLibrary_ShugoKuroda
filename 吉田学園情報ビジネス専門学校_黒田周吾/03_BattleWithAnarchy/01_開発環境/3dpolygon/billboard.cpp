//===================================================================
//
//		ビルボード処理[billboard.cpp]
//		Author:黒田 周吾
//
//===================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include "billboard.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"

//======================================================
//	マクロ定義
//======================================================
#define MAX_BILLBOARD		(128)		//ビルボードの最大数
#define BILLBOARD_WIDTH		(30.0f)		//影の幅
#define BILLBOARD_HEIGHT	(50.0f)		//影の高さ

//======================================================
//	ビルボードの構造体
//======================================================
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//回転
	int nIdxShadow;		//影の番号
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	bool bUse;		//使用しているかどうか
}billboard;

//======================================================
//	グローバル宣言
//======================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;		//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;			//テクスチャへのポインタ
billboard g_abillboard[MAX_BILLBOARD];		//ビルボードの情報
int g_nZBuffer;		//Zバッファの切り替えフラグ

//======================================================
//	ビルボードの初期化処理
//======================================================
void InitBillboard(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/friend000.png", &g_pTextureBillboard);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	//各情報の初期化
	g_nZBuffer = 0;

	//各項目の初期化
	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		g_abillboard[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_abillboard[nCnt].bUse = false;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_abillboard[nCnt].pos.x - (BILLBOARD_WIDTH / 2), g_abillboard[nCnt].pos.y + (BILLBOARD_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_abillboard[nCnt].pos.x + (BILLBOARD_WIDTH / 2), g_abillboard[nCnt].pos.y + (BILLBOARD_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_abillboard[nCnt].pos.x - (BILLBOARD_WIDTH / 2), g_abillboard[nCnt].pos.y - (BILLBOARD_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_abillboard[nCnt].pos.x + (BILLBOARD_WIDTH / 2), g_abillboard[nCnt].pos.y - (BILLBOARD_HEIGHT / 2), 0.0f);

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

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//ビルボード配置
	SetBillboard(D3DXVECTOR3(190.0f, 30.0f, 0.0f));
	SetBillboard(D3DXVECTOR3(150.0f, 30.0f, 0.0f));
	SetBillboard(D3DXVECTOR3(100.0f, 30.0f, 0.0f));
	SetBillboard(D3DXVECTOR3(50.0f, 30.0f, 0.0f));
	SetBillboard(D3DXVECTOR3(0.0f, 30.0f, 0.0f));
	SetBillboard(D3DXVECTOR3(-50.0f, 30.0f, 0.0f));

	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		if (g_abillboard[nCnt].bUse == true)
		{//ビルボードが使用されている場合、影を設定する
			g_abillboard[nCnt].nIdxShadow = SetShadow(D3DXVECTOR3(g_abillboard[nCnt].pos.x, 1.0f, g_abillboard[nCnt].pos.z), g_abillboard[nCnt].rot, 20.0f, 20.0f);
		}
	}

	//頂点バッファをアンロック
	g_pVtxBuffBillboard->Unlock();
}

//======================================================
//	ビルボードの終了処理
//======================================================
void UninitBillboard(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
	//テクスチャの破棄
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}
}

//======================================================
//	ビルボードの更新処理
//======================================================
void UpdateBillboard(void)
{
	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		if (g_abillboard[nCnt].bUse)
		{
			//影の位置更新
			SetPositionShadow(g_abillboard[nCnt].nIdxShadow, D3DXVECTOR3(g_abillboard[nCnt].pos.x, 1.0f, g_abillboard[nCnt].pos.z), g_abillboard[nCnt].rot);
		}
	}
}

//======================================================
//	ビルボードの描画処理
//======================================================
void DrawBillboard(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DXMATRIX mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxView;		//ビューマトリックス

	if (g_nZBuffer == 1)
	{//Zバッファを有効にする
		pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}
	if (g_nZBuffer == 0)
	{//Zバッファを無効にする
		pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		if (g_abillboard[nCnt].bUse == true)
		{//ビルボードが使用されている場合
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_abillboard[nCnt].mtxWorld);

			//ビューマトリックスの設定
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//カメラの逆行列を設定
			g_abillboard[nCnt].mtxWorld._11 = mtxView._11;
			//g_abillboard[nCnt].mtxWorld._12 = mtxView._21;
			g_abillboard[nCnt].mtxWorld._13 = mtxView._31;
			//g_abillboard[nCnt].mtxWorld._21 = mtxView._12;
			//g_abillboard[nCnt].mtxWorld._22 = mtxView._22;
			//g_abillboard[nCnt].mtxWorld._23 = mtxView._32;
			g_abillboard[nCnt].mtxWorld._31 = mtxView._13;
			//g_abillboard[nCnt].mtxWorld._32 = mtxView._23;
			g_abillboard[nCnt].mtxWorld._33 = mtxView._33;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_abillboard[nCnt].pos.x, g_abillboard[nCnt].pos.y, g_abillboard[nCnt].pos.z);
			D3DXMatrixMultiply(&g_abillboard[nCnt].mtxWorld, &g_abillboard[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_abillboard[nCnt].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBillboard);

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

	//Zバッファを無効にする
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//======================================================
//	ビルボードの設定処理
//======================================================
void SetBillboard(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		if (g_abillboard[nCnt].bUse == false)
		{//影が使用されていない場合
			g_abillboard[nCnt].pos = pos;
			g_abillboard[nCnt].bUse = true;

			break;
		}
	}
}