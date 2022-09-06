//========================================================
//
//						ビルボード
//					AUTHOR:越本愛彪
//
//========================================================
#include "main.h"
#include "billboard.h"
#include "player.h"

//マクロ定義
#define BILLBOARD_SIZE (10)												//ビルボードのサイズ
#define MAX_BILLBOARD (256)												//ビルボードの最大数
#define MAX_TEXTURE (16)												//テクスチャの最大数

//ビルボードの構造体
typedef struct
{
	D3DXVECTOR3 pos;													//位置
	D3DXVECTOR3 rot;													//向き
	D3DXMATRIX mtxWorld;												//ワールドマトリックス
	LPDIRECT3DTEXTURE9 pTexture;										//ビルボードのテクスチャのポインタ
	float nSize;														//サイズ
	int nTextureType;													//テクスチャの種類
	int nCntTime;														//表示する時間
	bool bUse;															//使用されているかどうか
} Billboard;

//グローバル変数	
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;						//ビルボードの頂点バッファへのポインタ
Billboard g_aBillboard[MAX_BILLBOARD];									//ビルボードの情報を格納
int g_nCntBillTime;														//ビルボードが表示されるまでのカウンター

//========================================================
//					ビルボード初期化処理
//========================================================
void InitBillboard(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ビルボードの情報の初期化
	for (int nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		g_aBillboard[nCntBill].pTexture = NULL;
		g_aBillboard[nCntBill].nTextureType = -1;
		g_aBillboard[nCntBill].nSize = 0;
		g_aBillboard[nCntBill].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の初期化
		g_aBillboard[nCntBill].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向きの初期化
	}
	g_nCntBillTime = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffBillboard, NULL);

	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロック
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		//各頂点位置の設定
		pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_SIZE, BILLBOARD_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(BILLBOARD_SIZE, BILLBOARD_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_SIZE, -BILLBOARD_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BILLBOARD_SIZE, -BILLBOARD_SIZE, 0.0f);

		//各頂点の法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//各頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファのアンロック
	g_pVtxBuffBillboard->Unlock();
}

//========================================================
//					ビルボード終了処理
//========================================================
void UninitBillboard(void)
{
	//テクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
	{
		if (g_aBillboard[nCntTexture].pTexture != NULL)
		{
			g_aBillboard[nCntTexture].pTexture->Release();
			g_aBillboard[nCntTexture].pTexture = NULL;
		}
	}

	//頂点バッファの解放
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//========================================================
//					ビルボード更新処理
//========================================================
void UpdateBillboard(void)
{
	if (g_nCntBillTime < 20)
	{
		g_nCntBillTime++;
		if (g_nCntBillTime >= 20)
		{
			SetBillboard(D3DXVECTOR3(0.0f, 100.0f, FIELD_SIZE_Z-200.0f), 20.0f, 180, "data/TEXTURE/5pts000.png");
			SetBillboard(D3DXVECTOR3(70.0f, 100.0f, FIELD_SIZE_Z - 200.0f), 20.0f, 180, "data/TEXTURE/3pts000.png");
			SetBillboard(D3DXVECTOR3(-70.0f, 100.0f, FIELD_SIZE_Z - 200.0f), 20.0f, 180, "data/TEXTURE/3pts000.png");
			SetBillboard(D3DXVECTOR3(0.0f, 100.0f, -FIELD_SIZE_Z + 200.0f), 20.0f, 180, "data/TEXTURE/5pts000.png");
			SetBillboard(D3DXVECTOR3(70.0f, 100.0f, -FIELD_SIZE_Z + 200.0f), 20.0f, 180, "data/TEXTURE/3pts000.png");
			SetBillboard(D3DXVECTOR3(-70.0f, 100.0f, -FIELD_SIZE_Z + 200.0f), 20.0f, 180, "data/TEXTURE/3pts000.png");
		}
	}

	for (int nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		if (g_aBillboard[nCntBill].nCntTime != -1)
		{//表示カウンターが-1の場合、常に表示する

			if (g_aBillboard[nCntBill].nCntTime <= 0)
			{//カウンター0で非表示	
				g_aBillboard[nCntBill].bUse = false;
			}
			else
			{
				g_aBillboard[nCntBill].nCntTime--;		//表示カウンター減算
			}
		}
	}
}

//========================================================
//					ビルボード描画処理
//========================================================
void DrawBillboard(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;

	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Zバッファ法の有効化
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	//Zテストの設定
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	for (int nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		if (g_aBillboard[nCntBill].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBillboard[nCntBill].mtxWorld);

			D3DXMATRIX mtxView;
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//カメラの逆行列を設定
			g_aBillboard[nCntBill].mtxWorld._11 = mtxView._11;
			g_aBillboard[nCntBill].mtxWorld._12 = mtxView._21;
			g_aBillboard[nCntBill].mtxWorld._13 = mtxView._31;
			g_aBillboard[nCntBill].mtxWorld._21 = mtxView._12;
			g_aBillboard[nCntBill].mtxWorld._22 = mtxView._22;
			g_aBillboard[nCntBill].mtxWorld._23 = mtxView._32;
			g_aBillboard[nCntBill].mtxWorld._31 = mtxView._13;
			g_aBillboard[nCntBill].mtxWorld._32 = mtxView._23;
			g_aBillboard[nCntBill].mtxWorld._33 = mtxView._33;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBillboard[nCntBill].pos.x, g_aBillboard[nCntBill].pos.y, g_aBillboard[nCntBill].pos.z);
			D3DXMatrixMultiply(&g_aBillboard[nCntBill].mtxWorld, &g_aBillboard[nCntBill].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard[nCntBill].mtxWorld);

			//頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_aBillboard[nCntBill].pTexture);

			//αテストを有効
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//ビルボードの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

			//αテストを無効に戻す
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		}
	}

	//Zテストの設定
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//Zバッファ法を有効に戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	//カメラを有効に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	pDevice->SetTexture(0, NULL);
}

//========================================================
//					ビルボードの設定処理
//========================================================
void SetBillboard(D3DXVECTOR3 pos, float fSize, int nCntTime, char *pFilename)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロック
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		if (g_aBillboard[nCntBillboard].bUse == false)
		{
			//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pFilename, &g_aBillboard[nCntBillboard].pTexture);

			g_aBillboard[nCntBillboard].nSize = fSize;
			g_aBillboard[nCntBillboard].pos = pos;
			g_aBillboard[nCntBillboard].nCntTime = nCntTime;

			//各頂点位置の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aBillboard[nCntBillboard].nSize, g_aBillboard[nCntBillboard].nSize, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBillboard[nCntBillboard].nSize, g_aBillboard[nCntBillboard].nSize, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aBillboard[nCntBillboard].nSize, -g_aBillboard[nCntBillboard].nSize, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBillboard[nCntBillboard].nSize, -g_aBillboard[nCntBillboard].nSize, 0.0f);

			g_aBillboard[nCntBillboard].bUse = true;

			break;
		}
		pVtx += 4;
	}
	//頂点バッファのアンロック
	g_pVtxBuffBillboard->Unlock();

}
