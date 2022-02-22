#include "main.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "enemy.h"
#include "life.h"
#include "game_logo.h"
#include "explosion.h"
#include "sound.h"
#include "fade.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	//頂点バッファへのポインタ
Player g_Player;									//プレイヤーの情報
bool g_bPlayerlogo;									//プレイヤーのロゴ表示カウント(開幕のみ)

//======================================================
//		プレイヤーの初期化処理
//======================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,"data/TEXTURE/Player001.png",&g_pTexturePlayer);

	g_Player.nCounterAnimPlayer = 0;		//カウンターを初期化する
	g_Player.nPatternAnimPlayer = 0;		//パターンNo.を初期化する
	g_Player.nCntAttack = 0;				//攻撃間隔のカウンター
	g_Player.nHitCnt = 0;					//プレイヤーのヒットカウントを初期化する
	g_Player.bUse = true;					//プレイヤー使用状態を初期化する
	g_Player.bcontrol = false;				//プレイヤー操作状態を初期化する
	g_Player.bcontrol2 = true;				//プレイヤー操作状態2を初期化する
	g_Player.nCntRespawn = 0;				//プレイヤーの復活カウンターを初期化する
	g_Player.nCntRespawn2 = 0;				//プレイヤーの復活カウンター2を初期化する
	g_Player.bInvincible = false;			//プレイヤーの点滅切り替え判定を初期化する
	g_Player.bDie = false;					//プレイヤーの死亡状態をリセットする
	g_Player.bEndGame = false;				//ゲーム終了判定状態を初期化する
	g_Player.nCntEndGame = 0;				//ゲーム終了遷移カウンターを初期化する
	g_Player.bVictory = false;				//プレイヤーの勝利判定をリセットする
	g_Player.posPlayer = D3DXVECTOR3(-50.0f, SCREEN_HEIGHT / 2, 0.0f);		//位置を初期化する
	g_Player.movePlayer = D3DXVECTOR3(0.0f,0.0f,0.0f);						//移動量を初期化する

	g_bPlayerlogo = true;

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

	//頂点座標の設定
	pVtx[0].pos.x = g_Player.posPlayer.x + sinf(-D3DX_PI*0.75f)*1.50f;
	pVtx[0].pos.y = g_Player.posPlayer.y + cosf(-D3DX_PI*0.75f)*1.50f;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Player.posPlayer.x + sinf(D3DX_PI*0.75f)*1.50f;
	pVtx[1].pos.y = g_Player.posPlayer.y + cosf(D3DX_PI*0.75f)*1.50f;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Player.posPlayer.x + sinf(-0.75f)*1.50f;
	pVtx[2].pos.y = g_Player.posPlayer.y + cosf(-0.75f)*1.50f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Player.posPlayer.x + sinf(0.75f)*1.50f;
	pVtx[3].pos.y = g_Player.posPlayer.y + cosf(0.75f)*1.50f;
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
	//サウンド停止
	StopSound();

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
	if (g_Player.bcontrol2 == true)
	{
		if (g_Player.bcontrol == true)
		{
			if (GetKeyboardPress(DIK_A) == true)
			{//Aキーが押された
				if (GetKeyboardPress(DIK_W) == true)
				{
					g_Player.posPlayer.x += sinf(-D3DX_PI * 0.75f)*PLAYER_MOVE;
					g_Player.posPlayer.y += cosf(-D3DX_PI * 0.75f)*PLAYER_MOVE;
				}
				else if (GetKeyboardPress(DIK_S) == true)
				{
					g_Player.posPlayer.x += sinf(-D3DX_PI * 0.25f)*PLAYER_MOVE;
					g_Player.posPlayer.y += cosf(-D3DX_PI * 0.25f)*PLAYER_MOVE;
				}
				else
				{
					g_Player.posPlayer.x += sinf(-D3DX_PI * 0.5f)*PLAYER_MOVE;
					g_Player.posPlayer.y += cosf(-D3DX_PI * 0.5f)*PLAYER_MOVE;
				}
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{//Dキーが押された
				if (GetKeyboardPress(DIK_W) == true)
				{
					g_Player.posPlayer.x += sinf(D3DX_PI *0.75f)*PLAYER_MOVE;
					g_Player.posPlayer.y += cosf(D3DX_PI *0.75f)*PLAYER_MOVE;
				}
				else if (GetKeyboardPress(DIK_S) == true)
				{
					g_Player.posPlayer.x += sinf(D3DX_PI * 0.25f)*PLAYER_MOVE;
					g_Player.posPlayer.y += cosf(D3DX_PI * 0.25f)*PLAYER_MOVE;
				}
				else
				{
					g_Player.posPlayer.x += sinf(D3DX_PI * 0.5f)*PLAYER_MOVE;
					g_Player.posPlayer.y += cosf(D3DX_PI * 0.5f)*PLAYER_MOVE;
				}
			}
			else if (GetKeyboardPress(DIK_W) == true)
			{//Wキーが押された
				g_Player.posPlayer.x += sinf(D3DX_PI * 1.0f) * PLAYER_MOVE;
				g_Player.posPlayer.y += cosf(D3DX_PI * 1.0f) * PLAYER_MOVE;

			}
			else if (GetKeyboardPress(DIK_S) == true)
			{//Sキーが押された
				g_Player.posPlayer.x += sinf(D3DX_PI * 0.0f) * PLAYER_MOVE;
				g_Player.posPlayer.y += cosf(D3DX_PI * 0.0f) * PLAYER_MOVE;
			}

			if (GetKeyboardPress(DIK_SPACE) == true)
			{//SPACEキーを押された
				g_Player.nCntAttack++;		//攻撃間隔のカウンター
				if (g_Player.nCntAttack > 10)
				{
					//弾の設定
					SetBullet(D3DXVECTOR3(g_Player.posPlayer.x + 30, g_Player.posPlayer.y + 15, g_Player.posPlayer.z), 0);
					g_Player.nCntAttack = 0;

					//サウンド再生
					PlaySound(SOUND_LABEL_SE_SHOT);
				}
			}
			else
			{//SPACEキーが押されていない
				g_Player.nCntAttack = 10;
			}

			//画面外へのはみ出し防止処理
			if (g_Player.posPlayer.x - PLAYER_SIZE_X <= 0)
			{
				g_Player.posPlayer.x = PLAYER_SIZE_X;
			}
			else if (g_Player.posPlayer.x + PLAYER_SIZE_X >= SCREEN_WIDTH)
			{
				g_Player.posPlayer.x = SCREEN_WIDTH - PLAYER_SIZE_X;
			}
			if (g_Player.posPlayer.y - PLAYER_SIZE_Y <= 0)
			{
				g_Player.posPlayer.y = PLAYER_SIZE_Y;
			}
			else if (g_Player.posPlayer.y + PLAYER_SIZE_Y >= SCREEN_HEIGHT)
			{
				g_Player.posPlayer.y = SCREEN_HEIGHT - PLAYER_SIZE_Y;
			}
		}
	}

	if (g_Player.bcontrol == false)
	{//プレイヤーが操作できない場合
		g_Player.nCntRespawn++;			//リスポーンカウンターを加算

		if (g_Player.nCntRespawn >= 60)
		{//カウンターが一定値を超えたら
			if (g_Player.posPlayer.x < SCREEN_WIDTH / 4.0f)
			{
				g_Player.posPlayer.x += 7.0f;		//プレイヤーを左から出現させる

				if (g_Player.posPlayer.x >= SCREEN_WIDTH / 5.0f)
				{//プレイヤーがリスポーンし終わったら
					g_Player.bcontrol = true;		//プレイヤーを操作可能にする
					g_Player.bcontrol2 = true;
					g_Player.nCntRespawn = 0;
				}
			}
		}
	}

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_Player.posPlayer.x - PLAYER_SIZE_X, g_Player.posPlayer.y - PLAYER_SIZE_Y, g_Player.posPlayer.z);
	pVtx[1].pos = D3DXVECTOR3(g_Player.posPlayer.x + PLAYER_SIZE_X, g_Player.posPlayer.y - PLAYER_SIZE_Y, g_Player.posPlayer.z);
	pVtx[2].pos = D3DXVECTOR3(g_Player.posPlayer.x - PLAYER_SIZE_X, g_Player.posPlayer.y + PLAYER_SIZE_Y, g_Player.posPlayer.z);
	pVtx[3].pos = D3DXVECTOR3(g_Player.posPlayer.x + PLAYER_SIZE_X, g_Player.posPlayer.y + PLAYER_SIZE_Y, g_Player.posPlayer.z);

	g_Player.nCounterAnimPlayer++;		//カウンターを加算
	if (g_Player.nCounterAnimPlayer > 3)
	{
		g_Player.nCounterAnimPlayer = 0;
		g_Player.nPatternAnimPlayer++;

		if (g_Player.nPatternAnimPlayer >= 2)
		{//パターンNo.を初期値に戻す
			g_Player.nPatternAnimPlayer = 0;
		}

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((0.5f*g_Player.nPatternAnimPlayer), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.5f*(g_Player.nPatternAnimPlayer + 1)), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.5f*g_Player.nPatternAnimPlayer), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.5f*(g_Player.nPatternAnimPlayer + 1)), 1.0f);
	}

	//状態判定
	switch (g_Player.State)
	{
	case PLAYERSTATE_NORMAL:
		break;
	case PLAYERSTATE_RESPAWN:
		if (g_Player.bInvincible == false)
		{
			g_Player.nCntRespawn2++;
			g_Player.nCounterState++;
			if (g_Player.nCounterState == 3)
			{
				g_Player.bInvincible = true;
			}
			if (g_Player.nCntRespawn2 > 180)
			{
				g_Player.bUse = true;
				g_Player.nCntRespawn2 = 0;
				g_Player.State = PLAYERSTATE_NORMAL;
			}
		}
		break;
	}

	if (g_Player.bEndGame == true)
	{
		g_Player.nCntEndGame++;
		if (g_Player.nCntEndGame == 180)
		{
			DiePlayer();
		}
	}

	if (g_bPlayerlogo == true)
	{
		Setlogo(0);
		g_bPlayerlogo = false;
	}

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

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePlayer);

	switch (g_Player.State)
	{
	case PLAYERSTATE_NORMAL:
		//ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,											//描画する最初の頂点インデックス
			2);											//プリミティブ(ポリゴン)数
		break;
	case PLAYERSTATE_RESPAWN:
		if (g_Player.bInvincible == true)
		{
			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				0,											//描画する最初の頂点インデックス
				2);											//プリミティブ(ポリゴン)数

			g_Player.nCounterState--;

			if (g_Player.nCounterState == 0)
			{
				g_Player.bInvincible = false;
			}
		}
		break;
	}
}

//======================================================
//		プレイヤーの情報を取得
//======================================================
Player *GetPlayer(void)
{
	return &g_Player;
}

//======================================================
//		プレイヤーのヒット処理
//======================================================
void HitPlayer(void)
{
	//ヒットカウント
	g_Player.nHitCnt++;

	//爆発四散させる
	SetExplosion(g_Player.posPlayer, 100.0f);

	//ライフ表示の減少
	DownLifePlayer();

	if (g_Player.nHitCnt <= 2)
	{
		//サウンド再生
		PlaySound(SOUND_LABEL_SE_PLAYERHIT);

		//プレイヤーを画面外に出す
		g_Player.bUse = false;
		g_Player.bcontrol = false;
		g_Player.bcontrol2 = false;
		g_Player.posPlayer = D3DXVECTOR3(-50.0f, SCREEN_HEIGHT / 2, 0.0f);

		g_Player.State = PLAYERSTATE_RESPAWN;
		g_Player.nCounterState = 0;
	}

	//プレイヤーの死亡判定
	if (g_Player.nHitCnt == 3)
	{
		//サウンド停止
		StopSound();

		//サウンド再生
		PlaySound(SOUND_LABEL_SE_PLAYERHIT);

		g_Player.bUse = false;
		g_Player.bcontrol2 = false;
		g_Player.posPlayer = D3DXVECTOR3(-50.0f, SCREEN_HEIGHT / 2, 0.0f);

		g_Player.bEndGame = true;
	}
}

//======================================================
//		プレイヤーの死亡判定処理
//======================================================
void DiePlayer(void)
{
	g_Player.bDie = true;
	SetFade(MODE_RESULT);
}