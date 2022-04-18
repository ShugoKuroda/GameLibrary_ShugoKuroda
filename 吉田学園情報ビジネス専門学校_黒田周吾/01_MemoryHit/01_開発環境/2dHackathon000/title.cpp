//=============================================================================
//
//	タイトル処理 [title.cpp]
//	Author : 黒田 周吾
//
//=============================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include "title.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "game.h"

//======================================================
//	マクロ定義
//======================================================
#define MAX_TITLE			(5)			//タイトル画面で必要な画像数
#define TITLE_SIZE_X		(900.0f)	//タイトルの大きさX
#define TITLE_SIZE_Y		(150.0f)	//タイトルの大きさY

//======================================================
//	グローバル変数
//======================================================
LPDIRECT3DTEXTURE9 g_pTextureTitle[MAX_TITLE] = { NULL };		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;					//頂点バッファへのポインタ
Title g_aTitle[MAX_TITLE];		//タイトル情報
MenuInfo g_Menu;				//メニュー選択に必要な情報

//======================================================
//	タイトル画面の初期化処理
//======================================================
void InitTitle(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/TITLE.jpg",
		&g_pTextureTitle[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Title000.png",
		&g_pTextureTitle[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/TitlePress000.png",
		&g_pTextureTitle[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/TitleMenu000.png",
		&g_pTextureTitle[3]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/TitleMenu002.png",
		&g_pTextureTitle[4]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TITLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	//構造体情報の初期化
	ZeroMemory(&g_aTitle, sizeof(g_aTitle));
	ZeroMemory(&g_Menu, sizeof(MenuInfo));

	SetTitle(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));			//背景
	SetTitle(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / -4.0f, 0.0f), D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.5f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	//タイトル
	SetTitle(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 1.4f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH / 3.0f, SCREEN_HEIGHT / 6.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));	//PressEnter
	SetTitle(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 1.6f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH / 3.0f, SCREEN_HEIGHT / 7.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));	//選択肢(3Dゲーム)
	SetTitle(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 1.2f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH / 3.0f, SCREEN_HEIGHT / 7.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));	//選択肢(チュートリアル)

	g_Menu.nSelect = 3;
	
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_TITLE; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aTitle[nCnt].pos.x - (g_aTitle[nCnt].size.x / 2), g_aTitle[nCnt].pos.y - (g_aTitle[nCnt].size.y / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTitle[nCnt].pos.x + (g_aTitle[nCnt].size.x / 2), g_aTitle[nCnt].pos.y - (g_aTitle[nCnt].size.y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTitle[nCnt].pos.x - (g_aTitle[nCnt].size.x / 2), g_aTitle[nCnt].pos.y + (g_aTitle[nCnt].size.y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTitle[nCnt].pos.x + (g_aTitle[nCnt].size.x / 2), g_aTitle[nCnt].pos.y + (g_aTitle[nCnt].size.y / 2), 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(g_aTitle[nCnt].color);
		pVtx[1].col = D3DXCOLOR(g_aTitle[nCnt].color);
		pVtx[2].col = D3DXCOLOR(g_aTitle[nCnt].color);
		pVtx[3].col = D3DXCOLOR(g_aTitle[nCnt].color);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;		//頂点情報のポインタを進める
	}

	//頂点バッファをアンロック
	g_pVtxBuffTitle->Unlock();

	//サウンド再生
	PlaySound(SOUND_LABEL_BGM000);
}

//======================================================
//	タイトル画面の終了処理
//======================================================
void UninitTitle(void)
{
	//サウンド停止
	StopSound();

	for (int nCnt = 0; nCnt < MAX_TITLE; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTextureTitle[nCnt] != NULL)
		{
			g_pTextureTitle[nCnt]->Release();
			g_pTextureTitle[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

//======================================================
//	タイトル画面の更新処理
//======================================================
void UpdateTitle(void)
{
	//--------------------------
	//	メニュー選択画面
	//--------------------------
	if (g_Menu.nCntEnter)
	{//選択肢メニュー
		if (GetKeyboardTrigger(DIK_UP) || GetKeyboardTrigger(DIK_W) || GetJoypadTrigger(JOYKEY_UP, PLAYER_1))
		{//↑キー押下
			g_Menu.nSelect--;

			//選択肢の色リセット(暗くする)
			g_aTitle[TITLENUM_SELECTGAME].color = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
			g_aTitle[TITLENUM_SELECTTUTORIAL].color = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

			if (g_Menu.nSelect < TITLENUM_SELECTGAME)
			{//選択肢が上限まで行ったら、下に戻す
				g_Menu.nSelect = TITLENUM_SELECTTUTORIAL;
			}

			//サウンド再生
			PlaySound(SOUND_LABEL_SE_SELECT1);
		}
		else if (GetKeyboardTrigger(DIK_DOWN) || GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN, PLAYER_1))
		{//↓キー押下
			g_Menu.nSelect++;

			//選択肢の色リセット(暗くする)
			g_aTitle[TITLENUM_SELECTGAME].color = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
			g_aTitle[TITLENUM_SELECTTUTORIAL].color = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

			if (g_Menu.nSelect > TITLENUM_SELECTTUTORIAL)
			{//選択肢が下限まで行ったら、上に戻す
				g_Menu.nSelect = TITLENUM_SELECTGAME;
			}

			//サウンド再生
			PlaySound(SOUND_LABEL_SE_SELECT1);
		}
		else if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) || GetJoypadTrigger(JOYKEY_A, PLAYER_1) || GetJoypadTrigger(JOYKEY_START, PLAYER_1))
		{//Enterキー押下
			if (g_Menu.nSelect == TITLENUM_SELECTGAME && !g_Menu.bGameStartPush)
			{//ゲームスタートの項目を選択

				//サウンド再生
				PlaySound(SOUND_LABEL_SE_SELECT2);

				//画面遷移
				SetFade(MODE_GAME);

				//スタート演出の繰り返し防止
				g_Menu.bGameStartPush = true;
			}
			else if (g_Menu.nSelect == TITLENUM_SELECTTUTORIAL && !g_Menu.bGameStartPush)
			{//チュートリアル項目を選択

				//サウンド再生
				PlaySound(SOUND_LABEL_SE_SELECT2);

				//画面遷移
				SetFade(MODE_TUTORIAL);

				//スタート演出の繰り返し防止
				g_Menu.bGameStartPush = true;
			}
		}
		//選択項目を明るくする
		g_aTitle[g_Menu.nSelect].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	//--------------------------
	//	PressEnter画面
	//--------------------------
	else if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_A, PLAYER_1) || GetJoypadTrigger(JOYKEY_START, PLAYER_1) && !g_Menu.nCntEnter)
	{//Enterキー、Aキー、STARTキー押下(PressEnterが表示されている間)
		if (g_aTitle[TITLENUM_TITLE].pos.y < SCREEN_HEIGHT / 4.0f)
		{//タイトルが所定の位置まで来ていない場合
			g_aTitle[TITLENUM_TITLE].pos.y = SCREEN_HEIGHT / 4.0f;		//タイトルを所定の位置に設定する
		}
		else
		{
			//PreesEnterを消す
			g_aTitle[TITLENUM_PRESSENTER].bUse = false;

			//選択肢を出現させる
			g_aTitle[TITLENUM_SELECTGAME].color = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
			g_aTitle[TITLENUM_SELECTTUTORIAL].color = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

			//選択肢メニューに移行する
			g_Menu.nCntEnter = 1;

			//サウンド再生
			PlaySound(SOUND_LABEL_SE_SELECT2);
		}
	}
	else if (!g_Menu.nCntEnter)
	{
		if (g_aTitle[TITLENUM_TITLE].pos.y >= SCREEN_HEIGHT / 4.0f)
		{//タイトルが所定の位置まで来た場合

			//PressEnterを点滅させる
			if (g_Menu.nCntColorA == 0)
			{//α値減算
				g_aTitle[TITLENUM_PRESSENTER].color.a -= 0.03f;
				if (g_aTitle[TITLENUM_PRESSENTER].color.a <= 0.0f)
				{
					g_Menu.nCntColorA = 1;
				}
			}
			else if (g_Menu.nCntColorA == 1)
			{//α値加算
				g_aTitle[TITLENUM_PRESSENTER].color.a += 0.03f;
				if (g_aTitle[TITLENUM_PRESSENTER].color.a >= 1.0f)
				{
					g_Menu.nCntColorA = 0;
				}
			}
		}
		else
		{//タイトルを上から出す(降ろす)
			g_aTitle[TITLENUM_TITLE].pos += g_aTitle[TITLENUM_TITLE].move;
		}
	}

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_TITLE; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aTitle[nCnt].pos.x - (g_aTitle[nCnt].size.x / 2), g_aTitle[nCnt].pos.y - (g_aTitle[nCnt].size.y / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTitle[nCnt].pos.x + (g_aTitle[nCnt].size.x / 2), g_aTitle[nCnt].pos.y - (g_aTitle[nCnt].size.y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTitle[nCnt].pos.x - (g_aTitle[nCnt].size.x / 2), g_aTitle[nCnt].pos.y + (g_aTitle[nCnt].size.y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTitle[nCnt].pos.x + (g_aTitle[nCnt].size.x / 2), g_aTitle[nCnt].pos.y + (g_aTitle[nCnt].size.y / 2), 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(g_aTitle[nCnt].color);
		pVtx[1].col = D3DXCOLOR(g_aTitle[nCnt].color);
		pVtx[2].col = D3DXCOLOR(g_aTitle[nCnt].color);
		pVtx[3].col = D3DXCOLOR(g_aTitle[nCnt].color);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;		//頂点情報のポインタを進める
	}

	//頂点バッファをアンロック
	g_pVtxBuffTitle->Unlock();
}

//======================================================
//	タイトル画面の描画処理
//======================================================
void DrawTitle(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_TITLE; nCnt++)
	{
		if (g_aTitle[nCnt].bUse)
		{
			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureTitle[nCnt]);

			//プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				nCnt * 4,	//描画する最初の頂点インデックス
				2);			//プリミティブ（ポリゴン）数
		}
	}
}

//======================================================
//	タイトルの設定処理
//======================================================
void SetTitle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 size, D3DXCOLOR color)
{
	for (int nCnt = 0; nCnt < MAX_TITLE; nCnt++)
	{
		if (!g_aTitle[nCnt].bUse)
		{
			g_aTitle[nCnt].pos = pos;			//位置を設定
			g_aTitle[nCnt].move = move;			//移動量を設定
			g_aTitle[nCnt].size = size;			//サイズ(大きさ)を設定
			g_aTitle[nCnt].color = color;		//色を設定
			g_aTitle[nCnt].bUse = true;			//使用状態にする

			break;
		}
	}
}