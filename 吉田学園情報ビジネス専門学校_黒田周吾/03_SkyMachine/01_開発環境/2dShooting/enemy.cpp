#include "main.h"
#include "enemy.h"
#include "bullet.h"
#include "player.h"
#include "explosion.h"
#include "game_logo.h"
#include "sound.h"
#include <stdlib.h>
#include <time.h>
#include "score.h"
#include "particle.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureEnemy[MAX_ENEMY_TYPE] = { NULL };	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;		//頂点バッファへのポインタ
Enemy g_aEnemy[MAX_ENEMY];							//エネミーの情報
int g_nEnemyCounter;								//エネミー出現カウンター
int g_nEnemyCounter2;								//エネミー出現カウンター2
bool g_PlayerSpawn;									//プレイヤーが登場したかどうか
int g_nCntScore2;									//スコア加算用
bool g_bVictory;									//ゲームに勝ったかどうか

//======================================================
//		エネミー(敵)の初期化処理
//======================================================
void InitEnemy(void)
{
	int nCntEnemy;
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Enemy001.png", &g_pTextureEnemy[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Enemy002.png", &g_pTextureEnemy[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Enemy003.png", &g_pTextureEnemy[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Enemy004.png", &g_pTextureEnemy[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Boss000.png", &g_pTextureEnemy[4]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);

	//エネミー(敵)情報の設定
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].posEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].moveEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nLife = 30;
		g_aEnemy[nCntEnemy].bMoveCnt = false;
		g_aEnemy[nCntEnemy].nSize = 50.0f;
		g_aEnemy[nCntEnemy].nMoveType = 0;
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].nCntAttack = 0;
	}

	g_nEnemyCounter = 0;		//エネミー出現カウンターのリセット
	g_nEnemyCounter2 = 0;		//エネミー出現カウンター2のリセット
	g_PlayerSpawn = false;		//プレイヤーが登場したかどうか
	g_bVictory = false;
	g_nCntScore2 = 0;

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].posEnemy.x - g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].posEnemy.y - g_aEnemy[nCntEnemy].nSize, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].posEnemy.x + g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].posEnemy.y - g_aEnemy[nCntEnemy].nSize, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].posEnemy.x - g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].posEnemy.y + g_aEnemy[nCntEnemy].nSize, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].posEnemy.x + g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].posEnemy.y + g_aEnemy[nCntEnemy].nSize, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;  //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//======================================================
//		敵(エネミー)終了処理
//======================================================
void UninitEnemy(void)
{
	//サウンド停止
	StopSound();

	//テクスチャの破棄
	for (int nCnt = 0; nCnt < MAX_ENEMY_TYPE; nCnt++)
	{
		if (g_pTextureEnemy[nCnt] != NULL)
		{
			g_pTextureEnemy[nCnt]->Release();
			g_pTextureEnemy[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//======================================================
//		敵(エネミー)更新処理
//======================================================
void UpdateEnemy(void)
{
	int nCntEnemy;
	Player *pPlayer;	//プレイヤーのポインタ

	//プレイヤー情報の取得
	pPlayer = GetPlayer();

	//乱数の種を設定
	srand((unsigned int)time(0));

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);
	
	//-------------------------------------------
	//		エネミー出現処理
	//-------------------------------------------
	if (g_PlayerSpawn == false)
	{
		g_nEnemyCounter++;		//エネミー登場カウンターを加算する
		if (g_nEnemyCounter == 80)
		{
			g_PlayerSpawn = true;
			g_nEnemyCounter = 0;
		}
	}

	if (g_PlayerSpawn == true)
	{//プレイヤーが登場したら
		g_nEnemyCounter++;		//エネミー出現カウンターを加算する
		if (g_nEnemyCounter2 < 5)
		{
			if (g_nEnemyCounter == 30)
			{//エネミーを出す(1巡目)
				SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 5, 0.0f), 0, 0, 30,50.0f);
				g_nEnemyCounter = 0;
				g_nEnemyCounter2++;
			}
		}
		if (5 <= g_nEnemyCounter2 && g_nEnemyCounter2 < 12)
		{
			if (g_nEnemyCounter == 40)
			{//エネミーを出す(2巡目)
				if (g_nEnemyCounter2 > 6)
				{
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, rand() % SCREEN_HEIGHT + 1.0f , 0.0f), 1, 1, 60, 50.0f);
				}
				g_nEnemyCounter = 0;
				g_nEnemyCounter2++;
			}
		}
		if (12 <= g_nEnemyCounter2 && g_nEnemyCounter2 < 19)
		{
			if (g_nEnemyCounter == 40)
			{//エネミーを出す(3巡目)
				if (g_nEnemyCounter2 > 13)
				{
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 4, 0.0f), 2, 2, 30, 40.0f);
				}
				g_nEnemyCounter = 0;
				g_nEnemyCounter2++;
			}
		}
		if (19 <= g_nEnemyCounter2 && g_nEnemyCounter2 < 22)
		{
			if (g_nEnemyCounter == 40)
			{//エネミーを出す(4巡目)
				if (g_nEnemyCounter2 > 20)
				{
					for (int nCnt=1; nCnt < 7; nCnt++)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 100.0f*nCnt, 0.0f), 1, 3, 60, 50.0f);
					}
				}
				g_nEnemyCounter = 0;
				g_nEnemyCounter2++;
			}
		}
		if (22 <= g_nEnemyCounter2 && g_nEnemyCounter2 < 32)
		{
			if (g_nEnemyCounter == 40)
			{//エネミーを出す(5巡目)(中ボス)
				if (g_nEnemyCounter2 > 30)
				{
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 2, 0.0f), 3, 4, 500, 200.0f);
				}
				g_nEnemyCounter = 0;
				g_nEnemyCounter2++;
			}
		}
		if (33 <= g_nEnemyCounter2 && g_nEnemyCounter2 < 38)
		{
			if (g_nEnemyCounter == 60)
			{//エネミーを出す(6巡目)
				if (g_nEnemyCounter2 > 30)
				{
					for (int nCnt = 1; nCnt < 6; nCnt++)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 120.0f*nCnt, 0.0f), 0, 5, 20, 50.0f);
					}
				}
				g_nEnemyCounter = 0;
				g_nEnemyCounter2++;
			}
		}
		if (38 <= g_nEnemyCounter2 && g_nEnemyCounter2 < 48)
		{
			if (g_nEnemyCounter == 60)
			{//エネミーを出す(7巡目)
				if (g_nEnemyCounter2 > 41)
				{
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 200.0f, SCREEN_HEIGHT, 0.0f), 1, 6, 20, 50.0f);		//後列
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 400.0f, 0.0f, 0.0f), 1, 7, 30, 50.0f);				//前列
				}
				g_nEnemyCounter = 0;
				g_nEnemyCounter2++;
			}
		}
		if (48 <= g_nEnemyCounter2 && g_nEnemyCounter2 < 62)
		{//エネミーを出す(8巡目)(一列目)
			if (g_nEnemyCounter == 20)
			{
				if (g_nEnemyCounter2 > 57)
				{
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - 100.0f, 0.0f), 2, 8, 20, 50.0f);
				}
				g_nEnemyCounter = 0;
				g_nEnemyCounter2++;
			}
		}
		if (62 <= g_nEnemyCounter2 && g_nEnemyCounter2 < 69)
		{//エネミーを出す(8巡目)(二列目)
			if (g_nEnemyCounter == 20)
			{
				if (g_nEnemyCounter2 > 64)
				{
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 5, 0.0f), 2, 8, 20, 50.0f);
				}
				g_nEnemyCounter = 0;
				g_nEnemyCounter2++;
			}
		}
		if (69 <= g_nEnemyCounter2 && g_nEnemyCounter2 < 76)
		{//エネミーを出す(8巡目)(三列目)
			if (g_nEnemyCounter == 20)
			{
				if (g_nEnemyCounter2 > 71)
				{
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 2, 0.0f), 2, 8, 20, 50.0f);
				}
				g_nEnemyCounter = 0;
				g_nEnemyCounter2++;
			}
		}
		if (76 <= g_nEnemyCounter2 && g_nEnemyCounter2 < 82)
		{//エネミーを出す(9巡目)
			if (g_nEnemyCounter == 70)
			{
				if (g_nEnemyCounter2 > 76)
				{
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f), 2, 9, 20, 50.0f);
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, 0.0f, 0.0f), 2, 9, 20, 50.0f);
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 100.0f, 0.0f), 2, 9, 20, 50.0f);

					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), 2, 10, 20, 50.0f);
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, SCREEN_HEIGHT, 0.0f), 2, 10, 20, 50.0f);
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - 100.0f, 0.0f), 2, 10, 20, 50.0f);
				}
				g_nEnemyCounter = 0;
				g_nEnemyCounter2++;
			}
		}
		if (82 <= g_nEnemyCounter2 && g_nEnemyCounter2 < 83)
		{//プレイヤーにボス警告を出す
			if (g_nEnemyCounter == 30)
			{
				Setlogo(1);
				g_nEnemyCounter = 0;
				g_nEnemyCounter2++;
			}
		}
		if (83 <= g_nEnemyCounter2 && g_nEnemyCounter2 < 90)
		{
			if (g_nEnemyCounter == 40)
			{//エネミーを出す(10巡目)ボス戦
				if (g_nEnemyCounter2 > 88)
				{
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 2, 0.0f), 4, 11, 1500, 200.0f);
				}
				g_nEnemyCounter = 0;
				g_nEnemyCounter2++;
			}
		}
	}

	//-------------------------------------------
	//		エネミー出現処理ここまで
	//-------------------------------------------

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//敵が出現している

		 //-------------------------------------------
		 //		エネミー移動処理
		 //-------------------------------------------

			if (g_aEnemy[nCntEnemy].nMoveType == 0)
			{//エネミーの移動処理(1巡目)
				if (g_aEnemy[nCntEnemy].posEnemy.x > SCREEN_WIDTH /1.8f && g_aEnemy[nCntEnemy].posEnemy.y == SCREEN_HEIGHT / 5)
				{
					g_aEnemy[nCntEnemy].moveEnemy = D3DXVECTOR3(-8.0f, 0.0f, 0.0f);
				}
				if (g_aEnemy[nCntEnemy].posEnemy.x <= SCREEN_WIDTH / 1.8f)
				{
					g_aEnemy[nCntEnemy].moveEnemy = D3DXVECTOR3(6.0f, 7.0f, 0.0f);
				}
				if (g_aEnemy[nCntEnemy].posEnemy.y >= SCREEN_HEIGHT -400.0f)
				{
					g_aEnemy[nCntEnemy].moveEnemy = D3DXVECTOR3(7.0f, 0.0f, 0.0f);
				}
			}
			if (g_aEnemy[nCntEnemy].nMoveType == 1)
			{//エネミーの移動処理(2巡目)
				g_aEnemy[nCntEnemy].moveEnemy.x = -5.0f;
				g_aEnemy[nCntEnemy].moveEnemy.y = (pPlayer->posPlayer.y - g_aEnemy[nCntEnemy].posEnemy.y) / 100.0f;
			}
			if (g_aEnemy[nCntEnemy].nMoveType == 2)
			{//エネミーの移動処理(3巡目)
				g_aEnemy[nCntEnemy].moveEnemy.x = -6.0f;
				if (g_aEnemy[nCntEnemy].bMoveCnt == false)
				{
					if (g_aEnemy[nCntEnemy].posEnemy.y >= SCREEN_HEIGHT / 4)
					{
						g_aEnemy[nCntEnemy].moveEnemy.y = -5.0f;
					}
					else
					{
						g_aEnemy[nCntEnemy].bMoveCnt = true;
					}
				}
				if (g_aEnemy[nCntEnemy].bMoveCnt == true)
				{
					if (g_aEnemy[nCntEnemy].posEnemy.y <= SCREEN_HEIGHT -200.0f)
					{
						g_aEnemy[nCntEnemy].moveEnemy.y = 5.0f;
					}
					else
					{
						g_aEnemy[nCntEnemy].bMoveCnt = false;
					}
				}
			}
			if (g_aEnemy[nCntEnemy].nMoveType == 3)
			{//エネミーの移動処理(4巡目)
				if (g_aEnemy[nCntEnemy].posEnemy.x >= SCREEN_WIDTH - 300.0f)
				{
					g_aEnemy[nCntEnemy].moveEnemy = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
				}
				else
				{
					g_aEnemy[nCntEnemy].moveEnemy = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
				}
			}
			if (g_aEnemy[nCntEnemy].nMoveType == 4)
			{//エネミーの移動処理(5巡目)(中ボス)
				if (g_aEnemy[nCntEnemy].posEnemy.x >= SCREEN_WIDTH - 300.0f)
				{
					g_aEnemy[nCntEnemy].moveEnemy = D3DXVECTOR3(-5.0f, 0.0f, 0.0f);
				}
				else
				{
					g_aEnemy[nCntEnemy].moveEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
			if (g_aEnemy[nCntEnemy].nMoveType == 5)
			{//エネミーの移動処理(6巡目)
				if (g_aEnemy[nCntEnemy].posEnemy.x >= 0.0f )
				{
					g_aEnemy[nCntEnemy].moveEnemy = D3DXVECTOR3(-3.0f, 0.0f, 0.0f);
				}
			}
			if (g_aEnemy[nCntEnemy].nMoveType == 6)
			{//エネミーの移動処理(7巡目)(後列)
				g_aEnemy[nCntEnemy].moveEnemy = D3DXVECTOR3(0.0f, -3.0f, 0.0f);
			}
			if (g_aEnemy[nCntEnemy].nMoveType == 7)
			{//エネミーの移動処理(7巡目)(前列)
				g_aEnemy[nCntEnemy].moveEnemy = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
			}
			if (g_aEnemy[nCntEnemy].nMoveType == 8)
			{//エネミーの移動処理(8巡目)
				g_aEnemy[nCntEnemy].moveEnemy = D3DXVECTOR3(-8.0f, 0.0f, 0.0f);
			}
			if (g_aEnemy[nCntEnemy].nMoveType == 9)
			{//エネミーの移動処理(9巡目)(左下↙へ)
				g_aEnemy[nCntEnemy].moveEnemy = D3DXVECTOR3(-6.0f, 6.0f, 0.0f);
			}
			if (g_aEnemy[nCntEnemy].nMoveType == 10)
			{//エネミーの移動処理(9巡目)(左上↖へ)
				g_aEnemy[nCntEnemy].moveEnemy = D3DXVECTOR3(-6.0f, -6.0f, 0.0f);
			}
			if (g_aEnemy[nCntEnemy].nMoveType == 11)
			{//エネミーの移動処理(10巡目)ボス戦
				if (g_aEnemy[nCntEnemy].bMoveCnt == false)
				{
					if (g_aEnemy[nCntEnemy].posEnemy.y >= SCREEN_HEIGHT / 4)
					{
						g_aEnemy[nCntEnemy].moveEnemy.x = 0.0f;
						g_aEnemy[nCntEnemy].moveEnemy.y = -5.0f;
					}
					else
					{
						g_aEnemy[nCntEnemy].bMoveCnt = true;
					}
				}
				if (g_aEnemy[nCntEnemy].bMoveCnt == true)
				{
					if (g_aEnemy[nCntEnemy].posEnemy.y <= SCREEN_HEIGHT - 200.0f)
					{
						g_aEnemy[nCntEnemy].moveEnemy.x = 0.0f;
						g_aEnemy[nCntEnemy].moveEnemy.y = 5.0f;
					}
					else
					{
						g_aEnemy[nCntEnemy].bMoveCnt = false;
					}
				}
				if (g_aEnemy[nCntEnemy].posEnemy.x >= SCREEN_WIDTH - 200.0f)
				{
					g_aEnemy[nCntEnemy].moveEnemy.x = -3.0f;
				}
			}

			//-------------------------------------------
			//		エネミー移動処理ここまで
			//-------------------------------------------

			//敵を動かす
			g_aEnemy[nCntEnemy].posEnemy += g_aEnemy[nCntEnemy].moveEnemy;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].posEnemy.x - g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].posEnemy.y - g_aEnemy[nCntEnemy].nSize, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].posEnemy.x + g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].posEnemy.y - g_aEnemy[nCntEnemy].nSize, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].posEnemy.x - g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].posEnemy.y + g_aEnemy[nCntEnemy].nSize, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].posEnemy.x + g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].posEnemy.y + g_aEnemy[nCntEnemy].nSize, 0.0f);

			if (g_aEnemy[nCntEnemy].posEnemy.x < 0.0f || g_aEnemy[nCntEnemy].posEnemy.x > SCREEN_WIDTH || g_aEnemy[nCntEnemy].posEnemy.y < 0.0f || g_aEnemy[nCntEnemy].posEnemy.y > SCREEN_HEIGHT)
			{
				g_aEnemy[nCntEnemy].bUse = false;  //使用していない状態にする
				g_aEnemy[nCntEnemy].nCounterState = 0;
			}

			if (g_aEnemy[nCntEnemy].bUse == true)
			{
				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:			//エネミーの状態が通常の場合
					break;
				case ENEMYSTATE_DAMAGE:			//エネミーがダメージ状態の場合
					g_aEnemy[nCntEnemy].nCounterState--;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					}
					break;
				}
			}
			if (pPlayer->bcontrol2 == true)
			{
				if (g_aEnemy[nCntEnemy].nType == 0 || g_aEnemy[nCntEnemy].nType == 1 || g_aEnemy[nCntEnemy].nType == 2)
				{
					//攻撃間隔カウンターの加算
					g_aEnemy[nCntEnemy].nCntAttack++;
					//敵の攻撃処理
					if (g_aEnemy[nCntEnemy].nCntAttack >= 80)
					{
						SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].posEnemy.x, g_aEnemy[nCntEnemy].posEnemy.y,
							atan2f(-(pPlayer->posPlayer.x - g_aEnemy[nCntEnemy].posEnemy.x), -(pPlayer->posPlayer.y - g_aEnemy[nCntEnemy].posEnemy.y))), 1);
						g_aEnemy[nCntEnemy].nCntAttack = 0;
					}
				}
				if (g_aEnemy[nCntEnemy].nType == 3|| g_aEnemy[nCntEnemy].nType == 4)
				{
					//攻撃間隔カウンターの加算
					g_aEnemy[nCntEnemy].nCntAttack++;
					//敵の攻撃処理
					if (g_aEnemy[nCntEnemy].nCntAttack >=60)
					{
						for (int nCntpos = 0; nCntpos < 4; nCntpos++)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].posEnemy.x, g_aEnemy[nCntEnemy].posEnemy.y, D3DX_PI * (0.85f + nCntpos * 0.03f)), 1);
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].posEnemy.x, g_aEnemy[nCntEnemy].posEnemy.y, D3DX_PI * (0.6f + nCntpos * 0.03f)), 1);
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].posEnemy.x, g_aEnemy[nCntEnemy].posEnemy.y, D3DX_PI * (0.45f + nCntpos * 0.03f)), 1);
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].posEnemy.x, g_aEnemy[nCntEnemy].posEnemy.y, D3DX_PI * (0.3f + nCntpos * 0.03f)), 1);
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].posEnemy.x, g_aEnemy[nCntEnemy].posEnemy.y, D3DX_PI * (0.15f + nCntpos * 0.03f)), 1);
							g_aEnemy[nCntEnemy].nCntAttack = 0;
						}
					}
				}
			}
			if (pPlayer->bUse == true)
			{//プレイヤーがいたら
				if (pPlayer->posPlayer.x + PLAYER_SIZE_X / 2 > pVtx[0].pos.x
					&& pPlayer->posPlayer.x - PLAYER_SIZE_X / 2 < pVtx[1].pos.x
					&&pPlayer->posPlayer.y + PLAYER_SIZE_Y / 2 > pVtx[1].pos.y
					&& pPlayer->posPlayer.y - PLAYER_SIZE_Y / 2 < pVtx[2].pos.y)
				{//プレイヤーが敵に当たったら(↑ここで当たり判定調整)
					HitPlayer();
				}
			}
		}
		pVtx += 4;  //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();
}

//======================================================
//		敵(エネミー)描画処理
//======================================================
void DrawEnemy(void)
{
	int nCntEnemy;
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//弾が使用されている
		 //テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEnemy[g_aEnemy[nCntEnemy].nType]);
			//プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				nCntEnemy * 4,								//描画する最初の頂点インデックス
				2);											//プリミティブ（ポリゴン）数
		}
	}
}

//======================================================
//		エネミーの設定処理
//======================================================
void SetEnemy(D3DXVECTOR3 pos, int nType,int nMoveType,int nLife,float nSize)
{
	int nCntEnemy;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{//弾が使用されている
			g_aEnemy[nCntEnemy].posEnemy = pos;
			g_aEnemy[nCntEnemy].nSize = nSize;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].posEnemy.x - g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].posEnemy.y - g_aEnemy[nCntEnemy].nSize, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].posEnemy.x + g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].posEnemy.y - g_aEnemy[nCntEnemy].nSize, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].posEnemy.x - g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].posEnemy.y + g_aEnemy[nCntEnemy].nSize, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].posEnemy.x + g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].posEnemy.y + g_aEnemy[nCntEnemy].nSize, 0.0f);

			g_aEnemy[nCntEnemy].nCntAttack = 100;
			g_aEnemy[nCntEnemy].nLife = nLife;
			g_aEnemy[nCntEnemy].nType = nType;
			g_aEnemy[nCntEnemy].nMoveType = nMoveType;
			g_aEnemy[nCntEnemy].bUse = true;	//使用している状態にする

			break;
		}
		pVtx += 4;  //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();
}

//======================================================
//		エネミー情報の取得
//======================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//======================================================
//		敵のヒット処理
//======================================================
void HitEnemy(int nCntEnemy, int nDamage)
{
	Player *pPlayer;	//プレイヤーのポインタ

	//プレイヤー情報の取得
	pPlayer = GetPlayer();

	g_aEnemy[nCntEnemy].nLife -= nDamage;

	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{//敵の体力がなくなった
		if (g_aEnemy[nCntEnemy].nType == 0 || g_aEnemy[nCntEnemy].nType == 1 || g_aEnemy[nCntEnemy].nType == 2)
		{
			g_aEnemy[nCntEnemy].bUse = false;
			g_aEnemy[nCntEnemy].nCounterState = 0;

			//爆発処理
			for (int nCnt = 0; nCnt < 30; nCnt++)
			{
				SetParticle(g_aEnemy[nCntEnemy].posEnemy);
			}

			//サウンド再生
			PlaySound(SOUND_LABEL_SE_EXPLOSION);

			g_nCntScore2 += 200;

			SetScore(g_nCntScore2);
		}
		if (g_aEnemy[nCntEnemy].nType == 3)
		{
			g_aEnemy[nCntEnemy].bUse = false;
			g_aEnemy[nCntEnemy].nCounterState = 0;

			//爆発処理
			SetExplosion(g_aEnemy[nCntEnemy].posEnemy, 300.0f);

			//サウンド再生
			PlaySound(SOUND_LABEL_SE_EXPLOSION);

			g_nEnemyCounter = 0;
			g_nEnemyCounter2++;

			g_nCntScore2 += 1000;

			SetScore(g_nCntScore2);
		}
		if (g_aEnemy[nCntEnemy].nType == 4)
		{
			g_aEnemy[nCntEnemy].bUse = false;
			g_aEnemy[nCntEnemy].nCounterState = 0;

			//爆発処理
			SetExplosion(g_aEnemy[nCntEnemy].posEnemy, 300.0f);

			//サウンド停止
			StopSound();

			//サウンド再生
			PlaySound(SOUND_LABEL_SE_EXPLOSION);

			g_nEnemyCounter = 0;
			g_nEnemyCounter2++;

			g_nCntScore2 += 2000;

			SetScore(g_nCntScore2);

			pPlayer->bEndGame = true;
			pPlayer->bVictory = true;
		}
	}
	else
	{
		//サウンド再生
		PlaySound(SOUND_LABEL_SE_HIT);

		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nCntEnemy].nCounterState = 6;

		VERTEX_2D *pVtx;		//頂点情報へのポインタ

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラーの設定
		pVtx += 4 * nCntEnemy;

		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//頂点バッファをアンロック
		g_pVtxBuffEnemy->Unlock();
	}
}