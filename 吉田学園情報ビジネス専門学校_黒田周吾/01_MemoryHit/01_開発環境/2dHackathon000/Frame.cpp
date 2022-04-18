#include "Frame.h"
#include "input.h"

//マクロ定義	
#define MAX_X	(5)	//横の枠数
#define MAX_Y	(5)	//縦の枠数
#define MAX_FRAME	(MAX_X * MAX_Y)	//必要枠数

typedef enum
{
	STATE_OFF = 0,
	STATE_ON
}Frame_State;

typedef enum
{
	TYPE_01 = 0,
	TYPE_02
}Frame_Type;

//
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 size;
	bool bUse;
	Frame_State state;
	Frame_Type type;
}Frame;

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureFrame[2] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFrame = NULL;		//頂点バッファへのポインタ
Frame g_aFrame[MAX_FRAME];

//----------------------------------------
//  枠の初期化設定処理
//----------------------------------------
void InitFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\waku_001.png",
		&g_pTextureFrame[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\waku_002.png",
		&g_pTextureFrame[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_FRAME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFrame,
		NULL);

	//位置の初期化
	for (int nCnt = 0; nCnt < MAX_FRAME; nCnt++)
	{
		g_aFrame[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFrame[nCnt].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFrame[nCnt].bUse = false;
		g_aFrame[nCnt].state = STATE_OFF;
		g_aFrame[nCnt].type = TYPE_01;
	}

	VERTEX_2D * pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffFrame->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_FRAME; nCnt++)
	{
		//頂点座標設定
		pVtx[0].pos = D3DXVECTOR3(g_aFrame[nCnt].pos.x - g_aFrame[nCnt].size.x, g_aFrame[nCnt].pos.y - g_aFrame[nCnt].size.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aFrame[nCnt].pos.x + g_aFrame[nCnt].size.x, g_aFrame[nCnt].pos.y - g_aFrame[nCnt].size.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aFrame[nCnt].pos.x - g_aFrame[nCnt].size.x, g_aFrame[nCnt].pos.y + g_aFrame[nCnt].size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aFrame[nCnt].pos.x + g_aFrame[nCnt].size.x, g_aFrame[nCnt].pos.y + g_aFrame[nCnt].size.y, 0.0f);

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

		pVtx += 4;			//頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffFrame->Unlock();
}

//----------------------------------------
//  枠の終了処理
//----------------------------------------
void UninitFrame(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (g_pTextureFrame[nCnt] != NULL)
		{
			g_pTextureFrame[nCnt]->Release();
			g_pTextureFrame[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffFrame != NULL)
	{
		g_pVtxBuffFrame->Release();
		g_pVtxBuffFrame = NULL;
	}
}

void SetFrame(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	for (int nCnt = 0; nCnt < MAX_FRAME; nCnt++)
	{
		if (g_aFrame[nCnt].bUse == false)
		{
			g_aFrame[nCnt].pos = pos;
			g_aFrame[nCnt].size = size;
			g_aFrame[nCnt].bUse = true;

			break;
		}
	}
}

//----------------------------------------
//  枠の配置処理
//----------------------------------------
void SetFramepos(void)
{
	float SIZE_X = 650.0f / MAX_X;
	float SIZE_Y = 650.0f / MAX_Y;

	for (int nCntY = 0; nCntY < MAX_Y; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_X; nCntX++)
		{
			SetFrame(D3DXVECTOR3(350.0f + SIZE_X * nCntX, 100.0f + SIZE_Y * nCntY, 0.0f), D3DXVECTOR3(60.0f, 60.0f, 0.0f));
		}
	}
}

//----------------------------------------
//　枠の更新処理
//----------------------------------------
void UpdateFrame(void)
{
	VERTEX_2D * pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffFrame->Lock(0, 0, (void**)&pVtx, 0);

	SetFramepos();

	for (int nCnt = 0; nCnt < MAX_FRAME; nCnt++)
	{
		if (g_aFrame[nCnt].bUse == true)
		{
			//頂点座標設定
			pVtx[0].pos = D3DXVECTOR3(g_aFrame[nCnt].pos.x - g_aFrame[nCnt].size.x, g_aFrame[nCnt].pos.y - g_aFrame[nCnt].size.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aFrame[nCnt].pos.x + g_aFrame[nCnt].size.x, g_aFrame[nCnt].pos.y - g_aFrame[nCnt].size.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aFrame[nCnt].pos.x - g_aFrame[nCnt].size.x, g_aFrame[nCnt].pos.y + g_aFrame[nCnt].size.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aFrame[nCnt].pos.x + g_aFrame[nCnt].size.x, g_aFrame[nCnt].pos.y + g_aFrame[nCnt].size.y, 0.0f);

			if (g_aFrame[nCnt].state == STATE_OFF)
			{
				//頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			}
			else if (g_aFrame[nCnt].state == STATE_ON)
			{
				//頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			}
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffFrame->Unlock();

}

//----------------------------------------
//  枠の描画処理
//----------------------------------------
void DrawFrame(void)
{
	// デバイス関係
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	pDevice = GetDevice();			//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFrame, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntFrame = 0; nCntFrame < MAX_FRAME; nCntFrame++)
	{
		if (g_aFrame[nCntFrame].bUse == true)
		{//敵が使用されている
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureFrame[g_aFrame[nCntFrame].type]);

			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntFrame * 4,
				2);
		}
	}
}

//----------------------------------------
//　枠の当たり判定
//----------------------------------------
void CollisionFrame(D3DXVECTOR3 pos, float size)
{
	VERTEX_2D * pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffFrame->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntFrame = 0; nCntFrame < MAX_FRAME; nCntFrame++)
	{
		if (g_aFrame[nCntFrame].bUse == true)
		{
			if (g_aFrame[nCntFrame].pos.x + g_aFrame[nCntFrame].size.x >= pos.x &&
				g_aFrame[nCntFrame].pos.x - g_aFrame[nCntFrame].size.x <= pos.x &&
				g_aFrame[nCntFrame].pos.y - g_aFrame[nCntFrame].size.y <= pos.y &&
				g_aFrame[nCntFrame].pos.y + g_aFrame[nCntFrame].size.y >= pos.y)
			{
				g_aFrame[nCntFrame].state = STATE_ON;
			}
			else
			{
				g_aFrame[nCntFrame].state = STATE_OFF;
			}
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffFrame->Unlock();
}

//----------------------------------------
//　枠の当たり判定
//----------------------------------------
void SelectFrame(void)
{
	for (int nCnt = 0; nCnt < MAX_FRAME; nCnt++)
	{
		if (g_aFrame[nCnt].bUse == true)
		{
			if (g_aFrame[nCnt].state == STATE_ON)
			{
				g_aFrame[nCnt].type = TYPE_02;
			}
		}
	}
}
