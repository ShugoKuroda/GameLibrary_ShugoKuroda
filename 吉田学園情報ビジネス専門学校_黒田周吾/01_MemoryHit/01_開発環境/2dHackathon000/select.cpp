#include "select.h"
#include "input.h"
#include "Frame.h"
#include "game.h"
#include "item.h"
#include "sound.h"
#include "particle.h"
#include "effect.h"

//マクロ定義	
#define SIZE_SELECT		(50.0f)	//選択指のサイズ
#define SPEED_SELECT	(5.0f)	//選択指のスピード

//
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
}Select;

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureSelect = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelect = NULL;	//頂点バッファへのポインタ
Select g_Select;

//----------------------------------------
//  選択の初期化設定処理
//----------------------------------------
void InitSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\select_001.png",
		&g_pTextureSelect);

	//頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffSelect,
			NULL);

	VERTEX_2D * pVtx;		//頂点情報へのポインタ

	//位置の初期化
	g_Select.pos = D3DXVECTOR3((SCREEN_WIDTH / 2.0f), (SCREEN_HEIGHT / 2.0f), 0.0f);

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標設定
	pVtx[0].pos = D3DXVECTOR3(g_Select.pos.x - SIZE_SELECT, g_Select.pos.y - SIZE_SELECT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Select.pos.x + SIZE_SELECT, g_Select.pos.y - SIZE_SELECT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Select.pos.x - SIZE_SELECT, g_Select.pos.y + SIZE_SELECT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Select.pos.x + SIZE_SELECT, g_Select.pos.y + SIZE_SELECT, 0.0f);

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

	//頂点バッファをアンロックする
	g_pVtxBuffSelect->Unlock();
}

//----------------------------------------
//  選択の終了処理
//----------------------------------------
void UninitSelect(void)
{
	//指テクスチャの破棄
	if (g_pTextureSelect != NULL)
	{
		g_pTextureSelect->Release();
		g_pTextureSelect = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffSelect != NULL)
	{
		g_pVtxBuffSelect->Release();
		g_pVtxBuffSelect = NULL;
	}
}

//----------------------------------------
//  選択指の移動処理
//----------------------------------------
void MoveSelect(void)
{
	if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT,PLAYER_1) == true)
	{//Aキーが押された
		SetEffect(D3DXVECTOR3(g_Select.pos.x, g_Select.pos.y, 0.0f), D3DXCOLOR(0.0f, 0.5f, 0.5f, 1.0f), 40.0f, 10, EFFECT_EFE1, 1);
		if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP, PLAYER_1) == true)
		{//Wキーと押した
			if (g_Select.pos.y >= SIZE_SELECT && g_Select.pos.x >= SIZE_SELECT)
			{
				g_Select.move.x += sinf(-D3DX_PI / 4.0f * 3.0f) * 0.8f;
				g_Select.move.y += cosf(-D3DX_PI / 4.0f * 3.0f) * 0.8f;
			}
		}
		else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN, PLAYER_1) == true)
		{//Sキーと押した
			if (g_Select.pos.y <= SCREEN_HEIGHT - SIZE_SELECT && g_Select.pos.x >= SIZE_SELECT)
			{
				g_Select.move.x += sinf(-D3DX_PI / 4.0f * 1.0f) * 0.8f;
				g_Select.move.y += cosf(-D3DX_PI / 4.0f * 1.0f) * 0.8f;
			}
		}
		else
		{//Aキーのみ
			if (g_Select.pos.x >= SIZE_SELECT)
			{
				g_Select.move.x += sinf(-D3DX_PI / 2.0f) * 0.8f;
				g_Select.move.y += cosf(-D3DX_PI / 2.0f) * 0.8f;
			}
		}
	}
	else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT, PLAYER_1) == true)
	{//Dキーが押された
		SetEffect(D3DXVECTOR3(g_Select.pos.x, g_Select.pos.y, 0.0f), D3DXCOLOR(0.0f, 0.5f, 0.5f, 1.0f), 40.0f, 10, EFFECT_EFE1, 1);
		if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP, PLAYER_1) == true)
		{//Wキーと押した
			if (g_Select.pos.x <= SCREEN_WIDTH - SIZE_SELECT && g_Select.pos.y >= SIZE_SELECT)
			{
				g_Select.move.x += sinf(D3DX_PI / 4.0f * 3.0f) * 0.8f;
				g_Select.move.y += cosf(D3DX_PI / 4.0f * 3.0f) * 0.8f;
			}
		}
		else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN, PLAYER_1) == true)
		{//Sキーと押した
			if (g_Select.pos.x <= SCREEN_WIDTH - SIZE_SELECT && g_Select.pos.y <= SCREEN_HEIGHT - SIZE_SELECT)
			{
				g_Select.move.x += sinf(D3DX_PI / 4.0f * 1.0f) * 0.8f;
				g_Select.move.y += cosf(D3DX_PI / 4.0f * 1.0f) * 0.8f;
			}
		}
		else
		{//Dキーのみ
			if (g_Select.pos.x <= SCREEN_WIDTH - SIZE_SELECT)
			{
				g_Select.move.x += sinf(D3DX_PI / 2.0f) * 0.8f;
				g_Select.move.y += cosf(D3DX_PI / 2.0f) * 0.8f;
			}
		}
	}
	else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN, PLAYER_1) == true)
	{//Sキーが押された
		SetEffect(D3DXVECTOR3(g_Select.pos.x, g_Select.pos.y, 0.0f), D3DXCOLOR(0.0f, 0.5f, 0.5f, 1.0f), 40.0f, 10, EFFECT_EFE1, 1);
		if (g_Select.pos.y <= SCREEN_HEIGHT - SIZE_SELECT)
		{
			g_Select.move.x += sinf(0.0f) * 0.8f;
			g_Select.move.y += cosf(0.0f) * 0.8f;
		}
	}
	else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP, PLAYER_1) == true)
	{//Wキーが押された
		SetEffect(D3DXVECTOR3(g_Select.pos.x, g_Select.pos.y, 0.0f), D3DXCOLOR(0.0f, 0.5f, 0.5f, 1.0f), 40.0f, 10, EFFECT_EFE1, 1);
		if (g_Select.pos.y >= SIZE_SELECT)
		{
			g_Select.move.x += sinf(D3DX_PI) * 0.8f;
			g_Select.move.y += cosf(D3DX_PI) * 0.8f;
		}
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A, PLAYER_1) == true)
	{
		SelectFrame();
		SelectItem();

		//サウンド再生
		PlaySound(SOUND_LABEL_SE_SELECT1);
	}

	//位置を更新
	g_Select.pos.x += g_Select.move.x;
	g_Select.pos.y += g_Select.move.y;

	//移動量を更新(減衰させる)
	g_Select.move.x += (0.0f - g_Select.move.x) * 0.08f;
	g_Select.move.y += (0.0f - g_Select.move.y) * 0.08f;

	VERTEX_2D * pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標設定
	pVtx[0].pos = D3DXVECTOR3(g_Select.pos.x - SIZE_SELECT, g_Select.pos.y - SIZE_SELECT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Select.pos.x + SIZE_SELECT, g_Select.pos.y - SIZE_SELECT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Select.pos.x - SIZE_SELECT, g_Select.pos.y + SIZE_SELECT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Select.pos.x + SIZE_SELECT, g_Select.pos.y + SIZE_SELECT, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffSelect->Unlock();
}

//----------------------------------------
//　選択の更新処理
//----------------------------------------
void UpdateSelect(void)
{
	VERTEX_2D * pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);

	//移動処理
	MoveSelect();

	//選択肢とアイテムの当たり判定
	CollisionFrame(g_Select.pos, SIZE_SELECT);
	CollisionItem(g_Select.pos, SIZE_SELECT);

	//頂点座標設定
	pVtx[0].pos = D3DXVECTOR3(g_Select.pos.x - SIZE_SELECT, g_Select.pos.y - SIZE_SELECT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Select.pos.x + SIZE_SELECT, g_Select.pos.y - SIZE_SELECT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Select.pos.x - SIZE_SELECT, g_Select.pos.y + SIZE_SELECT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Select.pos.x + SIZE_SELECT, g_Select.pos.y + SIZE_SELECT, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffSelect->Unlock();
}

//----------------------------------------
//  選択の描画処理
//----------------------------------------
void DrawSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffSelect, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureSelect);

		//ポリゴンの設定
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

}