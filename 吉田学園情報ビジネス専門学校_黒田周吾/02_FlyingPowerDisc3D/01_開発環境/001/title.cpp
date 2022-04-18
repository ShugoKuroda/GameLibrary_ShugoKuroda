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
#include "player.h"

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
bool g_bGameStartPush;			//スタート演出の繰り返し防止
int g_nCounterAnimBg;			//背景アニメーション再生までのカウンター
int g_nPatternAnimBg;			//背景アニメーションのフレーム番号
int g_nCntColorA;				//α値の設定カウンター
int g_nSelect;					//ゲームの選択項目カウンター
int g_nCntEnter;

//======================================================
//	タイトル画面の初期化処理
//======================================================
void InitTitle(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/TitleBg000.png",
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

	//タイトル情報の初期化
	ZeroMemory(&g_aTitle, sizeof(g_aTitle));

	SetTitle(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));			//背景
	SetTitle(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / -4.0f, 0.0f), D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.5f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	//タイトル
	SetTitle(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 1.4f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH / 3.0f, SCREEN_HEIGHT / 6.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));	//PressEnter
	SetTitle(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 1.6f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH / 3.0f, SCREEN_HEIGHT / 7.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));	//選択肢(3Dゲーム)
	SetTitle(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 1.2f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH / 3.0f, SCREEN_HEIGHT / 7.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));	//選択肢(チュートリアル)

	g_nCounterAnimBg = 0;
	g_nPatternAnimBg = 0;
	g_bGameStartPush = false;		//スタート演出繰り返し防止の初期化
	g_nCntColorA = 0;
	g_nSelect = 3;
	g_nCntEnter = 0;
	
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
	if (GetKeyboardTrigger(DIK_RETURN) && !g_nCntEnter || GetJoypadTrigger(JOYKEY_A, PLAYER_1) && !g_nCntEnter)
	{//Enterキー押下(PressEnterが表示されている間)
		if (g_aTitle[TITLENUM_TITLE].pos.y < SCREEN_HEIGHT / 4.0f)
		{//タイトルが所定の位置まで来ていない場合
			g_aTitle[TITLENUM_TITLE].pos.y = SCREEN_HEIGHT / 4.0f;		//タイトルを所定の位置に設定する
		}
		else
		{
			//PreesEnterを消す
			g_aTitle[TITLENUM_PRESSENTER].bUse = false;

			//選択肢を出現させる
			g_aTitle[TITLENUM_SELECT3DGAME].color = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
			g_aTitle[TITLENUM_SELECT2DGAME].color = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
			g_aTitle[TITLENUM_SELECTTUTORIAL].color = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

			//選択肢メニューに移行する
			g_nCntEnter = 1;
		}
	}
	else if (g_nCntEnter)
	{//選択肢メニュー
		if (GetKeyboardTrigger(DIK_UP) || GetKeyboardTrigger(DIK_W)||GetJoypadTrigger(JOYKEY_UP,PLAYER_1))
		{//↑キー押下
			g_nSelect--;

			//選択肢の色リセット(暗くする)
			g_aTitle[TITLENUM_SELECT3DGAME].color = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
			g_aTitle[TITLENUM_SELECT2DGAME].color = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
			g_aTitle[TITLENUM_SELECTTUTORIAL].color = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

			if (g_nSelect < 3)
			{//上限まで行ったら、下に戻す
				g_nSelect = 4;
			}
		}
		else if (GetKeyboardTrigger(DIK_DOWN) || GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN, PLAYER_1))
		{//↓キー押下
			g_nSelect++;

			//選択肢の色リセット(暗くする)
			g_aTitle[TITLENUM_SELECT3DGAME].color = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
			g_aTitle[TITLENUM_SELECT2DGAME].color = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
			g_aTitle[TITLENUM_SELECTTUTORIAL].color = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

			if (g_nSelect > 4)
			{//下限まで行ったら、上に戻す
				g_nSelect = 3;
			}
		}
		else if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) || GetJoypadTrigger(JOYKEY_A, PLAYER_1))
		{//Enterキー押下
			if (g_nSelect == 3)
			{//3Dモード
				//サウンド再生
				PlaySound(SOUND_LABEL_SE_SELECT);

				SetFade(MODE_GAME);

				//スタート演出の繰り返し防止
				g_bGameStartPush = true;
			}
			else if (g_nSelect == 4)
			{//チュートリアル

				//サウンド再生
				PlaySound(SOUND_LABEL_SE_SELECT);

				SetFade(MODE_TUTORIAL);

				//スタート演出の繰り返し防止
				g_bGameStartPush = true;
			}
		}
		//選択項目を明るくする
		g_aTitle[g_nSelect].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (!g_nCntEnter)
	{
		if (g_aTitle[TITLENUM_TITLE].pos.y >= SCREEN_HEIGHT / 4.0f)
		{//タイトルが所定の位置まで来た場合

			//PressEnterを点滅させる
			if (g_nCntColorA == 0)
			{//α値減算
				g_aTitle[TITLENUM_PRESSENTER].color.a -= 0.03f;
				if (g_aTitle[TITLENUM_PRESSENTER].color.a <= 0.0f)
				{
					g_nCntColorA = 1;
				}
			}
			else if (g_nCntColorA == 1)
			{//α値加算
				g_aTitle[TITLENUM_PRESSENTER].color.a += 0.03f;
				if (g_aTitle[TITLENUM_PRESSENTER].color.a >= 1.0f)
				{
					g_nCntColorA = 0;
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

	//g_nCounterAnimBg++;		//カウンターを加算

	//if (g_nCounterAnimBg % 2 == 0)
	//{
	//	g_nCounterAnimBg = 0;
	//	g_nPatternAnimBg = (g_nPatternAnimBg + 1) % 1000;		//パターンNo.を更新

	//	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//							//頂点バッファをロックし、頂点情報へのポインタを取得
	//	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	//	// テクスチャ座標の設定
	//	pVtx[0].tex = D3DXVECTOR2(0.002f * (g_nPatternAnimBg % 1000), 0.0f);
	//	pVtx[1].tex = D3DXVECTOR2(0.002f * ((g_nPatternAnimBg % 1000) + 500), 0.0f);
	//	pVtx[2].tex = D3DXVECTOR2(0.002f * (g_nPatternAnimBg % 1000), 1.0f);
	//	pVtx[3].tex = D3DXVECTOR2(0.002f * ((g_nPatternAnimBg % 1000) + 500), 1.0f);

	//	//頂点バッファをアンロック
	//	g_pVtxBuffTitle->Unlock();
	//}
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