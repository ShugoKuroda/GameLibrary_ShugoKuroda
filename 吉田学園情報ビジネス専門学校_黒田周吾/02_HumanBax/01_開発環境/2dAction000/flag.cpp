#include "main.h"
#include "flag.h"
#include "particle.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureGameFlag[MAX_FLAG] = { NULL };	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameFlag = NULL;			//頂点バッファへのポインタ
FLAG g_aGameFlag[4];		//中間地点の情報
int nPoint;

//======================================================
//		中間地点の初期化処理
//======================================================
void InitGameFlag(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	////テクスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice,
	//	"data/TEXTURE/flag000.png",
	//	&g_pTextureGameFlag[0]);
	//D3DXCreateTextureFromFile(pDevice,
	//	"data/TEXTURE/Exit000.png",
	//	&g_pTextureGameFlag[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameFlag,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameFlag->Lock(0, 0, (void**)&pVtx, 0);

	//各項目の初期化
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		g_aGameFlag[nCnt].pos = D3DXVECTOR3(SCREEN_WIDTH + 400.0f, SCREEN_HEIGHT / 2, 0.0f);
		g_aGameFlag[nCnt].nType = 0;
		g_aGameFlag[nCnt].bUse = false;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aGameFlag[nCnt].pos.x - FLAG_SIZE_X, g_aGameFlag[nCnt].pos.y - FLAG_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aGameFlag[nCnt].pos.x + FLAG_SIZE_X, g_aGameFlag[nCnt].pos.y - FLAG_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aGameFlag[nCnt].pos.x - FLAG_SIZE_X, g_aGameFlag[nCnt].pos.y + FLAG_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aGameFlag[nCnt].pos.x + FLAG_SIZE_X, g_aGameFlag[nCnt].pos.y + FLAG_SIZE_Y, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffGameFlag->Unlock();
}

//======================================================
//		中間地点の終了処理
//======================================================
void UninitGameFlag(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < MAX_FLAG; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTextureGameFlag[nCnt] != NULL)
		{
			g_pTextureGameFlag[nCnt]->Release();
			g_pTextureGameFlag[nCnt] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffGameFlag != NULL)
	{
		g_pVtxBuffGameFlag->Release();
		g_pVtxBuffGameFlag = NULL;
	}
}

//======================================================
//		中間地点の更新処理
//======================================================
void UpdateGameFlag(void)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameFlag->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (g_aGameFlag[nCnt].bUse == true)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aGameFlag[nCnt].pos.x - FLAG_SIZE_X, g_aGameFlag[nCnt].pos.y - FLAG_SIZE_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aGameFlag[nCnt].pos.x + FLAG_SIZE_X, g_aGameFlag[nCnt].pos.y - FLAG_SIZE_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aGameFlag[nCnt].pos.x - FLAG_SIZE_X, g_aGameFlag[nCnt].pos.y + FLAG_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aGameFlag[nCnt].pos.x + FLAG_SIZE_X, g_aGameFlag[nCnt].pos.y + FLAG_SIZE_Y, 0.0f);
		}
		pVtx += 4;		//頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffGameFlag->Unlock();
}

//======================================================
//		中間地点の描画処理
//======================================================
void DrawGameFlag(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGameFlag, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureGameFlag[g_aGameFlag[nCnt].nType]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			nCnt * 4,				//描画する最初の頂点インデックス
			2);						//プリミティブ（ポリゴン）数
	}
}

//======================================================
//		中間地点の設定処理
//======================================================
void SetGameFlag(D3DXVECTOR3 pos, int nType)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameFlag->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (g_aGameFlag[nCnt].bUse == false)
		{
			g_aGameFlag[nCnt].pos = pos;
			g_aGameFlag[nCnt].nType = nType;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aGameFlag[nCnt].pos.x - FLAG_SIZE_X, g_aGameFlag[nCnt].pos.y - FLAG_SIZE_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aGameFlag[nCnt].pos.x + FLAG_SIZE_X, g_aGameFlag[nCnt].pos.y - FLAG_SIZE_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aGameFlag[nCnt].pos.x - FLAG_SIZE_X, g_aGameFlag[nCnt].pos.y + FLAG_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aGameFlag[nCnt].pos.x + FLAG_SIZE_X, g_aGameFlag[nCnt].pos.y + FLAG_SIZE_Y, 0.0f);

			g_aGameFlag[nCnt].bUse = true;

			break;
		}
	}
	//頂点バッファをアンロック
	g_pVtxBuffGameFlag->Unlock();
}

//======================================================
//		中間地点の設定処理
//======================================================
void CollisionFlag(D3DXVECTOR3 *pPos, float Width, float Height)
{
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (pPos->x + (Width / 2) >= g_aGameFlag[nCnt].pos.x - FLAG_SIZE_X
			&& pPos->x - (Width / 2) <= g_aGameFlag[nCnt].pos.x + FLAG_SIZE_X
			&& pPos->y >= g_aGameFlag[nCnt].pos.y - FLAG_SIZE_Y
			&& pPos->y - Height <= g_aGameFlag[nCnt].pos.y + FLAG_SIZE_Y)
		{
			for (int nCnt = 0; nCnt < 15; nCnt++)
			{
				SetParticle(g_aGameFlag[nCnt].pos);
			}
			if (nCnt == 0)
			{
				nPoint = 1;
			}
			if (nCnt == 1)
			{
				nPoint = 2;
			}
			if (nCnt == 2)
			{
				nPoint = 3;
			}
		}
	}
}

int FlagStage(void)
{
	return nPoint;
}

//======================================================
//		プレイヤーの情報を取得
//======================================================
FLAG *GetFlag(void)
{
	return &g_aGameFlag[0];
}