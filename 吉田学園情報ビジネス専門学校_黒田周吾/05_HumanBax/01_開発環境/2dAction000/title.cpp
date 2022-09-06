#include "main.h"
#include "title.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "player.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureTitle[MAX_TITLE] = { NULL };			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle[MAX_TITLE] = { NULL };		//頂点バッファへのポインタ
bool g_GameStartPush;			//スタート演出の繰り返し防止

//タイトル演出用プレイヤー情報
D3DXVECTOR3 g_PlayerPos;		//位置を初期化する
D3DXVECTOR3 g_PlayerMove;		//移動量を初期化する
int g_PlayerDirectionMove;		//プレイヤーの向き初期化する
int g_PlayerCounterAnim;		//カウンターを初期化する
int g_PlayerPatternAnim;		//パターンNo.を初期化する
int g_nCntSetPlayer;			//プレイヤーのループ用変数

//======================================================
//		タイトル画面の初期化処理
//======================================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/title000.png",
		&g_pTextureTitle[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/title001.png",
		&g_pTextureTitle[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/title002.png",
		&g_pTextureTitle[3]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/player000.png",
		&g_pTextureTitle[4]);

	//頂点バッファの生成
	for (int nCnt = 0; nCnt < MAX_TITLE; nCnt++)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffTitle[nCnt],
			NULL);
	}

	g_PlayerPos = D3DXVECTOR3(SCREEN_WIDTH + 200.0f , 470.0f, 0.0f);	//位置を初期化する
	g_PlayerMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//移動量を初期化する
	g_nCntSetPlayer = 0;			//プレイヤーのループ用変数
	g_PlayerDirectionMove = 1;		//プレイヤーの向き初期化する
	g_PlayerCounterAnim = 0;		//カウンターを初期化する
	g_PlayerPatternAnim = 0;		//パターンNo.を初期化する

	g_GameStartPush = false;		//スタート演出繰り返し防止の初期化

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle[0]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定
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

	//頂点バッファをアンロック
	g_pVtxBuffTitle[0]->Unlock();


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle[1]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定
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

	//頂点バッファをアンロック
	g_pVtxBuffTitle[1]->Unlock();


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle[2]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定
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

	//頂点バッファをアンロック
	g_pVtxBuffTitle[2]->Unlock();


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle[3]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定
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

	//頂点バッファをアンロック
	g_pVtxBuffTitle[3]->Unlock();


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle[4]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定(中心座標は下辺の中心)
	pVtx[0].pos.x = g_PlayerPos.x - PLAYER_SIZE_X / 2;
	pVtx[0].pos.y = g_PlayerPos.y - PLAYER_SIZE_Y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_PlayerPos.x + PLAYER_SIZE_X / 2;
	pVtx[1].pos.y = g_PlayerPos.y - PLAYER_SIZE_Y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_PlayerPos.x - PLAYER_SIZE_X / 2;
	pVtx[2].pos.y = g_PlayerPos.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_PlayerPos.x + PLAYER_SIZE_X / 2;
	pVtx[3].pos.y = g_PlayerPos.y;
	pVtx[3].pos.z = 0.0f;

	//rhwの設定(1.0fで固定)
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
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffTitle[4]->Unlock();

	//サウンド再生
	PlaySound(SOUND_LABEL_BGM000);
}

//======================================================
//		タイトル画面の終了処理
//======================================================
void UninitTitle(void)
{
	for (int nCnt = 0; nCnt < MAX_TITLE; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTextureTitle[nCnt] != NULL)
		{
			g_pTextureTitle[nCnt]->Release();
			g_pTextureTitle[nCnt] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < MAX_TITLE; nCnt++)
	{
		//頂点バッファの破棄
		if (g_pVtxBuffTitle[nCnt] != NULL)
		{
			g_pVtxBuffTitle[nCnt]->Release();
			g_pVtxBuffTitle[nCnt] = NULL;
		}
	}
}

//======================================================
//		タイトル画面の更新処理
//======================================================
void UpdateTitle(void)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle[4]->Lock(0, 0, (void**)&pVtx, 0);

	if (g_GameStartPush == false)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//エンターキーを押されたら
			//モード移行
			SetFade(MODE_TUTORIAL);

			//スタート演出の繰り返し防止
			g_GameStartPush = true;
		}
	}

	g_nCntSetPlayer++;
	if (g_nCntSetPlayer == 900)
	{
		g_PlayerPos = D3DXVECTOR3(SCREEN_WIDTH + 200.0f, 470.0f, 0.0f);
		g_nCntSetPlayer = 0;
	}

	g_PlayerMove.x += sinf(-D3DX_PI * 0.5f)*(PLAYER_MOVE - 0.3f);
	g_PlayerMove.y += cosf(-D3DX_PI * 0.5f)*(PLAYER_MOVE - 0.3f);

	g_PlayerCounterAnim++;		//カウンターを加算
	if (g_PlayerCounterAnim > 6)
	{
		g_PlayerCounterAnim = 0;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.25f * g_PlayerPatternAnim, 0.5f * g_PlayerDirectionMove);
		pVtx[1].tex = D3DXVECTOR2(0.25f * (g_PlayerPatternAnim + 1), 0.5f * g_PlayerDirectionMove);
		pVtx[2].tex = D3DXVECTOR2(0.25f * g_PlayerPatternAnim, 0.5f * (g_PlayerDirectionMove + 1));
		pVtx[3].tex = D3DXVECTOR2(0.25f * (g_PlayerPatternAnim + 1), 0.5f * (g_PlayerDirectionMove + 1));

		g_PlayerPatternAnim++;

		if (g_PlayerPatternAnim >= 4)
		{//パターンNo.を初期値に戻す
			g_PlayerPatternAnim = 0;
		}
	}

	//位置更新
	g_PlayerPos += g_PlayerMove;

	//移動量更新(減衰)
	g_PlayerMove.x += -g_PlayerMove.x * 0.1f;			//(目的の値 - 現在の値) * 減衰係数

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle[4]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定(中心座標は下辺の中心)
	pVtx[0].pos.x = g_PlayerPos.x - PLAYER_SIZE_X / 2;
	pVtx[0].pos.y = g_PlayerPos.y - PLAYER_SIZE_Y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_PlayerPos.x + PLAYER_SIZE_X / 2;
	pVtx[1].pos.y = g_PlayerPos.y - PLAYER_SIZE_Y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_PlayerPos.x - PLAYER_SIZE_X / 2;
	pVtx[2].pos.y = g_PlayerPos.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_PlayerPos.x + PLAYER_SIZE_X / 2;
	pVtx[3].pos.y = g_PlayerPos.y;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロック
	g_pVtxBuffTitle[4]->Unlock();
}

//======================================================
//		タイトル画面の描画処理
//======================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_TITLE; nCnt++)
	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffTitle[nCnt], 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[nCnt]);

		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,		//描画する最初の頂点インデックス
			2);		//プリミティブ（ポリゴン）数
	}
}