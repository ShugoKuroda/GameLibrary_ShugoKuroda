//=============================================================================
//
// アイテム処理 [item.cpp]
// Author : 黒田 周吾
//
//=============================================================================

//======================================================
// インクルード
//======================================================
#include "main.h"
#include "item.h"
#include "particle.h"
#include "fade.h"
#include "sound.h"
#include <stdio.h>
#include "score.h"

//======================================================
// グローバル変数
//======================================================
LPDIRECT3DTEXTURE9 g_pTextureItem[MAX_ITEM_TEX] = { NULL };			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;		//頂点バッファへのポインタ
Item g_aItem[MAX_ITEM];							//ブロックの情報

//================================================
//	ブロックの初期化処理
//================================================
void InitItem(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Item_001.png", &g_pTextureItem[0]);		//通常アイテム1
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Item_002.png", &g_pTextureItem[1]);		//通常アイテム2
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Item_003.png", &g_pTextureItem[2]);		//通常アイテム3
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Item_007.png", &g_pTextureItem[3]);		//通常アイテム4
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Item_004_SR.png", &g_pTextureItem[4]);		//レアアイテム1
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Item_005_SR.png", &g_pTextureItem[5]);		//レアアイテム2
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Item_007_SR.png", &g_pTextureItem[6]);		//レアアイテム3

	//ブロックの情報の初期化
	ZeroMemory(&g_aItem, sizeof(g_aItem));

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_ITEM; nCntBlock++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntBlock].pos.x, g_aItem[nCntBlock].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntBlock].pos.x, g_aItem[nCntBlock].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntBlock].pos.x, g_aItem[nCntBlock].pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntBlock].pos.x, g_aItem[nCntBlock].pos.y, 0.0f);

		//rhwの設定(1.0fで固定)
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

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

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();
}

//======================================================
//	ブロックの終了処理
//======================================================
void UninitItem(void)
{
	for (int nCnt = 0; nCnt > MAX_ITEM_TEX; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTextureItem != NULL)
		{
			g_pTextureItem[nCnt]->Release();
			g_pTextureItem[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}

//======================================================
//	ブロックの更新処理
//======================================================
void UpdateItem(void)
{
}

//======================================================
//	ブロックの描画処理
//======================================================
void DrawItem(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBlock = 0; nCntBlock < MAX_ITEM; nCntBlock++)
	{
		if (g_aItem[nCntBlock].bUse == true)
		{//弾が使用されている

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureItem[g_aItem[nCntBlock].nType]);

			//プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				nCntBlock * 4,								//描画する最初の頂点インデックス
				2);		//プリミティブ（ポリゴン）数
		}
	}
}

//======================================================
//	ブロックの設定処理
//======================================================
void SetItem(D3DXVECTOR3 pos, int nType, float ItemPosX, float ItemPosY)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_ITEM; nCnt++)
	{
		if (g_aItem[nCnt].bUse == false)
		{//ブロックが使用されていない場合

			g_aItem[nCnt].pos = pos;
			g_aItem[nCnt].nType = nType;
			g_aItem[nCnt].BoxNum = D3DXVECTOR2(ItemPosX, ItemPosY);

			//頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(g_aItem[nCnt].pos.x - (ITEM_SIZE / 2), g_aItem[nCnt].pos.y - (ITEM_SIZE / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[nCnt].pos.x + (ITEM_SIZE / 2), g_aItem[nCnt].pos.y - (ITEM_SIZE / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[nCnt].pos.x - (ITEM_SIZE / 2), g_aItem[nCnt].pos.y + (ITEM_SIZE / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[nCnt].pos.x + (ITEM_SIZE / 2), g_aItem[nCnt].pos.y + (ITEM_SIZE / 2), 0.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			g_aItem[nCnt].bUse = true;

			break;
		}
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();
}

//----------------------------------------
//　アイテムの当たり判定
//----------------------------------------
void CollisionItem(D3DXVECTOR3 pos, float size)
{
	VERTEX_2D * pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntFrame = 0; nCntFrame < MAX_ITEM; nCntFrame++)
	{
		if (g_aItem[nCntFrame].bUse == true)
		{
			if (g_aItem[nCntFrame].pos.x + ITEM_SIZE >= pos.x &&
				g_aItem[nCntFrame].pos.x - ITEM_SIZE <= pos.x &&
				g_aItem[nCntFrame].pos.y - ITEM_SIZE <= pos.y &&
				g_aItem[nCntFrame].pos.y + ITEM_SIZE >= pos.y)
			{
				g_aItem[nCntFrame].state = ITEMSTATE_ON;
			}
			else
			{
				g_aItem[nCntFrame].state = ITEMSTATE_OFF;
			}
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();
}

//----------------------------------------
//　アイテムの当たり判定
//----------------------------------------
void SelectItem(void)
{
	bool bPenalty = true;		//ペナルティ判定用

	for (int nCnt = 0; nCnt < MAX_ITEM; nCnt++)
	{
		if (g_aItem[nCnt].bUse && !g_aItem[nCnt].bSelect)
		{
			if (g_aItem[nCnt].state == ITEMSTATE_ON)
			{
				if (g_aItem[nCnt].nType == 4 || g_aItem[nCnt].nType == 5 || g_aItem[nCnt].nType == 6)
				{//金色のアイテム
					AddScore(3);
					bPenalty = false;		//ペナルティを受けない
					g_aItem[nCnt].bSelect = true;		//重複して選択しないようにする
				}
				else
				{//普通のアイテム
					AddScore(1);
					bPenalty = false;		//ペナルティを受けない
					g_aItem[nCnt].bSelect = true;		//重複して選択しないようにする
				}
			}
		}
		else if (g_aItem[nCnt].bUse && g_aItem[nCnt].bSelect)
		{
			bPenalty = false;		//ペナルティを受けない
		}
	}

	if (bPenalty)
	{//ペナルティ判定が有効なら
		AddScore(-3);
	}
}

//======================================================
//	ブロックの情報を取得
//======================================================
Item *GetItem(void)
{
	return &g_aItem[0];
}