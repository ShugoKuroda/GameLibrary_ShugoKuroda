#include "main.h"
#include "switch.h"
#include "block.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureSwitch[MAX_SWITCH_TEX] = { NULL };			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSwitch[MAX_SWITCH_TEX] = { NULL };		//頂点バッファへのポインタ
Switch g_aSwitch[MAX_SWITCH];

//======================================================
//		スイッチ画面の初期化処理
//======================================================
void InitSwitch(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/switch000.png",
		&g_pTextureSwitch[0]);

	//頂点バッファの生成
	for (int nCnt = 0; nCnt < MAX_SWITCH_TEX; nCnt++)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SWITCH,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffSwitch[nCnt],
			NULL);
	}

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSwitch[0]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SWITCH; nCnt++)
	{
		g_aSwitch[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSwitch[nCnt].bUse = false;
		g_aSwitch[nCnt].nPushCnt = 0;
		g_aSwitch[nCnt].fWidth = 0.0f;
		g_aSwitch[nCnt].fHeight = 0.0f;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aSwitch[nCnt].fWidth, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, g_aSwitch[nCnt].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aSwitch[nCnt].fWidth, g_aSwitch[nCnt].fHeight, 0.0f);

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
	g_pVtxBuffSwitch[0]->Unlock();
}

//======================================================
//		スイッチ画面の終了処理
//======================================================
void UninitSwitch(void)
{
	for (int nCnt = 0; nCnt < MAX_SWITCH_TEX; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTextureSwitch[nCnt] != NULL)
		{
			g_pTextureSwitch[nCnt]->Release();
			g_pTextureSwitch[nCnt] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < MAX_SWITCH_TEX; nCnt++)
	{
		//頂点バッファの破棄
		if (g_pVtxBuffSwitch[nCnt] != NULL)
		{
			g_pVtxBuffSwitch[nCnt]->Release();
			g_pVtxBuffSwitch[nCnt] = NULL;
		}
	}
}

//======================================================
//		スイッチ画面の更新処理
//======================================================
void UpdateSwitch(void)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSwitch[0]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SWITCH; nCnt++)
	{
		if (g_aSwitch[nCnt].bUse == true)
		{
			if (g_aSwitch[nCnt].nPushCnt == 1)
			{
				g_aSwitch[nCnt].nPushCnt = OpenGate(nCnt);
			}

			//頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(g_aSwitch[nCnt].pos.x - (g_aSwitch[nCnt].fWidth / 2), g_aSwitch[nCnt].pos.y - (g_aSwitch[nCnt].fHeight / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aSwitch[nCnt].pos.x + (g_aSwitch[nCnt].fWidth / 2), g_aSwitch[nCnt].pos.y - (g_aSwitch[nCnt].fHeight / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aSwitch[nCnt].pos.x - (g_aSwitch[nCnt].fWidth / 2), g_aSwitch[nCnt].pos.y + (g_aSwitch[nCnt].fHeight / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aSwitch[nCnt].pos.x + (g_aSwitch[nCnt].fWidth / 2), g_aSwitch[nCnt].pos.y + (g_aSwitch[nCnt].fHeight / 2), 0.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		pVtx += 4;		//頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffSwitch[0]->Unlock();
}

//======================================================
//		スイッチ画面の描画処理
//======================================================
void DrawSwitch(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_SWITCH_TEX; nCnt++)
	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffSwitch[nCnt], 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureSwitch[nCnt]);

		for (int nCnt = 0; nCnt < MAX_SWITCH; nCnt++)
		{
			if (g_aSwitch[nCnt].bUse == true)
			{
				//プレイヤーの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
					nCnt * 4,		//描画する最初の頂点インデックス
					2);		//プリミティブ（ポリゴン）数
			}
		}
	}
}

//======================================================
//		スイッチの設定処理
//======================================================
void SetSwitch(D3DXVECTOR3 pos, float Width, float Height)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSwitch[0]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SWITCH; nCnt++)
	{
		if (g_aSwitch[nCnt].bUse == false)
		{//ブロックが使用されていない場合

			g_aSwitch[nCnt].pos = pos;
			g_aSwitch[nCnt].fWidth = Width;
			g_aSwitch[nCnt].fHeight = Height;

			//頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(g_aSwitch[nCnt].pos.x - (g_aSwitch[nCnt].fWidth / 2), g_aSwitch[nCnt].pos.y - (g_aSwitch[nCnt].fHeight / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aSwitch[nCnt].pos.x + (g_aSwitch[nCnt].fWidth / 2), g_aSwitch[nCnt].pos.y - (g_aSwitch[nCnt].fHeight / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aSwitch[nCnt].pos.x - (g_aSwitch[nCnt].fWidth / 2), g_aSwitch[nCnt].pos.y + (g_aSwitch[nCnt].fHeight / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aSwitch[nCnt].pos.x + (g_aSwitch[nCnt].fWidth / 2), g_aSwitch[nCnt].pos.y + (g_aSwitch[nCnt].fHeight / 2), 0.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			g_aSwitch[nCnt].bUse = true;

			break;
		}
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffSwitch[0]->Unlock();
}

//======================================================
//		スイッチの当たり判定
//======================================================
void CollisionSwitch(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float Width, float Height)
{
	for (int nCnt = 0; nCnt < MAX_SWITCH; nCnt++)
	{
		if (g_aSwitch[nCnt].bUse == true)
		{
			if (g_aSwitch[nCnt].nPushCnt == 0)
			{
				if (pPos->x + (Width / 2) > g_aSwitch[nCnt].pos.x - (g_aSwitch[nCnt].fWidth / 2)
					&& pPos->x - (Width / 2) < g_aSwitch[nCnt].pos.x + (g_aSwitch[nCnt].fWidth / 2)
					&& pPosOld->y <= g_aSwitch[nCnt].pos.y - (g_aSwitch[nCnt].fHeight / 2)
					&& pPos->y > g_aSwitch[nCnt].pos.y - (g_aSwitch[nCnt].fHeight / 2))
				{//上からブロックに着地した場合
					pPos->y = g_aSwitch[nCnt].pos.y - (g_aSwitch[nCnt].fHeight / 2);
					pMove->y = 0.0f;
					g_aSwitch[nCnt].pos.y += 10.0f;
					g_aSwitch[nCnt].nPushCnt = 1;
				}
			}
		}
	}
}



//======================================================
//		スイッチの情報を取得
//======================================================
Switch *GetSwitch(void)
{
	return &g_aSwitch[0];
}