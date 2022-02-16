#include "main.h"
#include "player.h"
#include "input.h"
#include "block.h"
#include "switch.h"
#include "particle.h"
#include "flag.h"
#include "sound.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	//頂点バッファへのポインタ
Player g_Player;									//プレイヤーの情報

//======================================================
//		プレイヤーの初期化処理
//======================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player000.png", &g_pTexturePlayer);

	g_Player.nCounterAnim = 0;		//カウンターを初期化する
	g_Player.nPatternAnim = 0;		//パターンNo.を初期化する
	g_Player.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 400.0f, 0.0f);		//位置を初期化する
	g_Player.posOld = D3DXVECTOR3(200.0f, 400.0f, 0.0f);			//前回の位置を初期化する
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//移動量を初期化する
	g_Player.BlockMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//現在乗っているブロック番号の初期化
	g_Player.bUse = true;											//使用状況を初期化する
	g_Player.bIsJumping = false
		;										//プレイヤーがジャンプ中判定を初期化する
	g_Player.nDirectionMove = 0;									//プレイヤーの向き初期化する

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定(中心座標は下辺の中心)
	pVtx[0].pos.x = g_Player.pos.x - PLAYER_SIZE_X / 2;
	pVtx[0].pos.y = g_Player.pos.y - PLAYER_SIZE_Y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Player.pos.x + PLAYER_SIZE_X / 2;
	pVtx[1].pos.y = g_Player.pos.y - PLAYER_SIZE_Y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Player.pos.x - PLAYER_SIZE_X / 2;
	pVtx[2].pos.y = g_Player.pos.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Player.pos.x + PLAYER_SIZE_X / 2;
	pVtx[3].pos.y = g_Player.pos.y;
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

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//======================================================
//		プレイヤーの終了処理
//======================================================
void UninitPlayer(void)
{
	//テクスチャの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	} 
}

//======================================================
//		プレイヤーの更新処理
//======================================================
void UpdatePlayer(void)
{
	//各ポインターを宣言
	Block *pBlock;

	//各情報の取得
	pBlock = GetBlock();

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);
	if (g_Player.bUse == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{//Aキーが押された
			g_Player.move.x += sinf(-D3DX_PI * 0.5f)*PLAYER_MOVE;
			g_Player.move.y += cosf(-D3DX_PI * 0.5f)*PLAYER_MOVE;
			g_Player.nDirectionMove = 1;
		}

		if (GetKeyboardPress(DIK_D) == true)
		{//Dキーが押された
			g_Player.move.x += sinf(D3DX_PI * 0.5f)*PLAYER_MOVE;
			g_Player.move.y += cosf(D3DX_PI * 0.5f)*PLAYER_MOVE;
			g_Player.nDirectionMove = 0;
		}

		if (g_Player.bIsJumping == false)
		{
			if (GetKeyboardTrigger(DIK_SPACE) == true)
			{//スペースキーが押された
				g_Player.move.y = PLAYER_MOVEUP;

				g_Player.bIsJumping = true;

				if (g_Player.nPatternAnim == 0 || g_Player.nPatternAnim == 2)
				{
					g_Player.nPatternAnim++;
				}

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.25f * g_Player.nPatternAnim, 0.5f * g_Player.nDirectionMove);
				pVtx[1].tex = D3DXVECTOR2(0.25f * (g_Player.nPatternAnim + 1), 0.5f * g_Player.nDirectionMove);
				pVtx[2].tex = D3DXVECTOR2(0.25f * g_Player.nPatternAnim, 0.5f * (g_Player.nDirectionMove + 1));
				pVtx[3].tex = D3DXVECTOR2(0.25f * (g_Player.nPatternAnim + 1), 0.5f * (g_Player.nDirectionMove + 1));

				if (g_Player.nPatternAnim >= 4)
				{//パターンNo.を初期値に戻す
					g_Player.nPatternAnim = 0;
				}
			}

			if (g_Player.move.x >= 1.0f || g_Player.move.x <= -1.0f)
			{//プレイヤーが移動中
				g_Player.nCounterAnim++;		//カウンターを加算
				if (g_Player.nCounterAnim > 6)
				{
					g_Player.nCounterAnim = 0;

					// テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(0.25f * g_Player.nPatternAnim, 0.5f * g_Player.nDirectionMove);
					pVtx[1].tex = D3DXVECTOR2(0.25f * (g_Player.nPatternAnim + 1), 0.5f * g_Player.nDirectionMove);
					pVtx[2].tex = D3DXVECTOR2(0.25f * g_Player.nPatternAnim, 0.5f * (g_Player.nDirectionMove + 1));
					pVtx[3].tex = D3DXVECTOR2(0.25f * (g_Player.nPatternAnim + 1), 0.5f * (g_Player.nDirectionMove + 1));

					g_Player.nPatternAnim++;

					if (g_Player.nPatternAnim >= 4)
					{//パターンNo.を初期値に戻す
						g_Player.nPatternAnim = 0;
					}
				}
			}
			else
			{//プレイヤーが停止中の場合
				if (g_Player.nPatternAnim == 1 || g_Player.nPatternAnim == 3)
				{
					g_Player.nPatternAnim++;
				}
				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.25f * g_Player.nPatternAnim, 0.5f * g_Player.nDirectionMove);
				pVtx[1].tex = D3DXVECTOR2(0.25f * (g_Player.nPatternAnim + 1), 0.5f * g_Player.nDirectionMove);
				pVtx[2].tex = D3DXVECTOR2(0.25f * g_Player.nPatternAnim, 0.5f * (g_Player.nDirectionMove + 1));
				pVtx[3].tex = D3DXVECTOR2(0.25f * (g_Player.nPatternAnim + 1), 0.5f * (g_Player.nDirectionMove + 1));

				if (g_Player.nPatternAnim >= 4)
				{//パターンNo.を初期値に戻す
					g_Player.nPatternAnim = 0;
				}
			}
		}

		//前回の位置を保存
		g_Player.posOld = g_Player.pos;

		//重力負荷をかける
		g_Player.move.y += PLAYER_MOVEDOWN;

		//横スクロール処理
		GameScroll(0);

		//位置更新
		g_Player.pos += g_Player.move;
		if (g_Player.bIsJumping == false)
		{
			g_Player.pos.x += g_Player.BlockMove.x;
		}

		//ブロックの当たり判定
		g_Player.bIsJumping = !CollisionBlock(&g_Player.pos, &g_Player.posOld, &g_Player.move, &g_Player.BlockMove, PLAYER_SIZE_X, PLAYER_SIZE_Y, 2);
		CollisionSwitch(&g_Player.pos, &g_Player.posOld, &g_Player.move, PLAYER_SIZE_X, PLAYER_SIZE_Y);

		//移動量更新(減衰)
		g_Player.move.x += -g_Player.move.x * 0.1f;			//(目的の値 - 現在の値) * 減衰係数

		//画面下の死亡処理
		if (g_Player.pos.y >= SCREEN_HEIGHT + PLAYER_SIZE_Y)
		{
			//パーティクル処理
			for (int nCnt = 0; nCnt < 20; nCnt++)
			{
				SetParticle(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_SIZE_Y, g_Player.pos.z));
			}
			g_Player.bUse = false;

			//サウンド再生
			PlaySound(SOUND_LABEL_SE_EXPLOSION);
		}
	}

	//頂点座標の設定(中心座標は下辺の中心)
	pVtx[0].pos.x = g_Player.pos.x - PLAYER_SIZE_X / 2;
	pVtx[0].pos.y = g_Player.pos.y - PLAYER_SIZE_Y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Player.pos.x + PLAYER_SIZE_X / 2;
	pVtx[1].pos.y = g_Player.pos.y - PLAYER_SIZE_Y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Player.pos.x - PLAYER_SIZE_X / 2;
	pVtx[2].pos.y = g_Player.pos.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Player.pos.x + PLAYER_SIZE_X / 2;
	pVtx[3].pos.y = g_Player.pos.y;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//======================================================
//		プレイヤーの描画処理
//======================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_Player.bUse == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePlayer);

		//ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,											//描画する最初の頂点インデックス
			2);											//プリミティブ(ポリゴン)数
	}
}

//======================================================
//		動くブロックの当たり判定
//======================================================
bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float Width, float Height)
{
	//ブロックのポインターを宣言
	Block *pBlock;

	//ブロックの情報を取得
	pBlock = GetBlock();

	bool bIsMoving = false;		//動いているかどうか

	//上からブロックに着地した場合
	if (pPos->x + (Width / 2) > g_Player.pos.x - (PLAYER_SIZE_X / 2)
		&& pPos->x - (Width / 2) < g_Player.pos.x + (PLAYER_SIZE_X / 2)
		&& pPosOld->y - (Height / 2) <= g_Player.pos.y
		&& pPos->y - (Height / 2) > g_Player.pos.y)
	{//プレイヤーを下降させる
		g_Player.posOld = g_Player.pos;
		g_Player.pos.y = pPos->y - (Height / 2);
		CollisionBlock(&g_Player.pos, &g_Player.posOld, &g_Player.move, &g_Player.BlockMove, PLAYER_SIZE_X, PLAYER_SIZE_Y, 0);
	}

	//上からブロックに着地した場合
	if (pPos->x + (Width / 2) > g_Player.pos.x - (PLAYER_SIZE_X / 2)
		&& pPos->x - (Width / 2) < g_Player.pos.x + (PLAYER_SIZE_X / 2)
		&& pPos->y - (Height / 2) < g_Player.pos.y
		&& pPosOld->y - (Height / 2) >= g_Player.pos.y)
	{//プレイヤーを上昇させる
		g_Player.pos.y = pPos->y - (Height / 2);
	}

	//下から動くブロックに挟まった場合
	if (pPos->x + (Width / 2) > g_Player.pos.x - (PLAYER_SIZE_X / 2)
		&& pPos->x - (Width / 2) < g_Player.pos.x + (PLAYER_SIZE_X / 2)
		&& pPos->y + (Height / 2) > g_Player.pos.y - PLAYER_SIZE_Y
		&& pPosOld->y + (Height / 2) <= g_Player.pos.y - PLAYER_SIZE_Y)
	{
		if (g_Player.bIsJumping == false)
		{
			//パーティクル処理
			for (int nCnt = 0; nCnt < 20; nCnt++)
			{
				SetParticle(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - (PLAYER_SIZE_Y / 2), g_Player.pos.z));
			}
			g_Player.bUse = false;
			g_Player.pos = D3DXVECTOR3(-100.0f, -100.0f, 0.0f);
			//サウンド再生
			PlaySound(SOUND_LABEL_SE_EXPLOSION);
		}
	}

	//右からブロックにぶつかった時場合
	if (pPosOld->x + (Width / 2) <= g_Player.pos.x - (PLAYER_SIZE_X / 2)
		&& pPos->x + (Width / 2) > g_Player.pos.x - (PLAYER_SIZE_X / 2)
		&& pPos->y - (Height / 2) < g_Player.pos.y
		&& pPos->y + (Height / 2) > g_Player.pos.y - PLAYER_SIZE_Y)
	{//ブロックの右に押し出す
		g_Player.pos.x = pPos->x + (Width / 2) + (PLAYER_SIZE_X / 2);

		//横スクロール処理
		GameScroll(1);
	}
	
	//左からブロックにぶつかった時場合
	if (pPosOld->x - (Width / 2) >= g_Player.pos.x + (PLAYER_SIZE_X / 2)
		&& pPos->x - (Width / 2) < g_Player.pos.x + (PLAYER_SIZE_X / 2)
		&& pPos->y - (Height / 2) < g_Player.pos.y
		&& pPos->y + (Height / 2) > g_Player.pos.y - PLAYER_SIZE_Y)
	{//ブロックの左に押し出す
		g_Player.pos.x = pPos->x - (Width / 2) - (PLAYER_SIZE_X / 2);

		//位置更新
		GameScroll(1);
	}
	return bIsMoving;
}

//======================================================
//		プレイヤーの死亡処理
//======================================================
bool DiePlayer(void)
{
	return g_Player.bUse;
}

//======================================================
//		画面スクロール処理
//======================================================
void GameScroll(int nType)
{
	//各ポインターを宣言
	Block *pBlock;
	Switch *pSwitch;
	FLAG *pFlag;

	//各情報の取得
	pBlock = GetBlock();
	pSwitch = GetSwitch();
	pFlag = GetFlag();

	//位置更新
	if (g_Player.pos.x > SCREEN_WIDTH * 0.7f)
	{//右端に行こうとした場合
		g_Player.pos.x = SCREEN_WIDTH * 0.7f;
		for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++, pBlock++)
		{
			pBlock->pos.x -= g_Player.move.x;

			////デバッグ中
			//pBlock->pos.x -= PLAYER_MOVE;

			if (nType == 1)
			{//リフトで移動する場合
				pBlock->pos.x -= 4.0f;
			}

			else if (nType == 2)
			{//可動ブロック移動時
				pBlock->pos.x -= 3.0f;
			}
		}
		for (int nCnt = 0; nCnt < MAX_SWITCH; nCnt++, pSwitch++)
		{
			pSwitch->pos.x -= g_Player.move.x;

			////デバッグ中
			//pSwitch->pos.x -= PLAYER_MOVE;

			if (nType == 1)
			{//リフトで移動する場合
				pSwitch->pos.x -= 4.0f;
			}

			else if (nType == 2)
			{//可動ブロック移動時
				pSwitch->pos.x -= 3.0f;
			}
		}
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pFlag->pos.x -= g_Player.move.x;

			////デバッグ中
			//pSwitch->pos.x -= PLAYER_MOVE;

			if (nType == 1)
			{//リフトで移動する場合
				pFlag->pos.x -= 4.0f;
			}

			else if (nType == 2)
			{//可動ブロック移動時
				pFlag->pos.x -= 3.0f;
			}
		}
	}

	else if (g_Player.pos.x < SCREEN_WIDTH * 0.3f)
	{//左端に行こうとした場合
		g_Player.pos.x = SCREEN_WIDTH * 0.3f;
		for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++, pBlock++)
		{
			pBlock->pos.x -= g_Player.move.x;

			////デバッグ中
			//pBlock->pos.x += PLAYER_MOVE;

			if (nType == 1)
			{//リフトで移動する場合
				pBlock->pos.x += 4.0f;
			}

			else if (nType == 2)
			{//可動ブロック移動時
				pBlock->pos.x += 3.0f;
			}
		}
		for (int nCnt = 0; nCnt < MAX_SWITCH; nCnt++, pSwitch++)
		{
			pSwitch->pos.x -= g_Player.move.x;

			////デバッグ中
			//pSwitch->pos.x += PLAYER_MOVE;

			if (nType == 1)
			{//リフトで移動する場合
				pSwitch->pos.x += 4.0f;
			}

			else if (nType == 2)
			{//可動ブロック移動時
				pSwitch->pos.x += 3.0f;
			}
		}
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pFlag->pos.x -= g_Player.move.x;

			////デバッグ中
			//pSwitch->pos.x -= PLAYER_MOVE;

			if (nType == 1)
			{//リフトで移動する場合
				pFlag->pos.x += 4.0f;
			}

			else if (nType == 2)
			{//可動ブロック移動時
				pFlag->pos.x += 3.0f;
			}
		}
	}
}

//======================================================
//		プレイヤーの情報を取得
//======================================================
Player *GetPlayer(void)
{
	return &g_Player;
}