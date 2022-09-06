//========================================================
//
//					得点時の処理
//					AUTHOR:越本愛彪
//
//========================================================
#include "player.h"
#include "score.h"
#include "bullet.h"
#include "set.h"
#include "ui.h"
#include "fade.h"
#include "sound.h"
//#include "game.h"
//#include "result.h"
//#include "fade.h"

//マクロ定義
#define NUM_SCORE (2)											//得点の桁
#define ONE_SENT  (60)											//1フレームを1/60秒としたときの1秒

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;			//得点のテクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		//得点の頂点情報へのポインタ
Score g_aScore[MAX_PLAYER];							//得点の情報を格納
bool g_bUse;				//得点の表示・非表示を管理
int g_nCntScoreTime;		//スコアの表示時間カウンター
int g_nCntChange;			//最初に投げる人を交代する用
int g_nSetPlayer1;			//Set数記憶用(1P)
int g_nSetPlayer2;			//Set数記憶用(2P)
int g_nCntGameEnd;			//ゲームが終わり、画面遷移するまでのカウンター
	
//========================================================
//					得点の初期化処理
//========================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureScore);

	//グローバル変数の初期化
	g_bUse = false;						//表示していない状態にする
	g_nCntScoreTime = 0;
	g_nCntChange = PLAYER_1;
	g_nSetPlayer1 = 0;
	g_nSetPlayer2 = 0;
	g_nCntGameEnd = 0;
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_aScore[nCntPlayer].nScore = 0;							//スコアの初期化
		g_aScore[nCntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置の初期化
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (NUM_SCORE * MAX_PLAYER),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	//プレイヤー[1]の得点
	for (int nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(320.0f + (80.0f * nCntScore), 250.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(400.0f + (80.0f * nCntScore), 250.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(320.0f + (80.0f * nCntScore), 430.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(400.0f + (80.0f * nCntScore), 430.0f, 0.0f);

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

		//テクスチャの座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	//プレイヤー[2]の得点
	for (int nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(800.0f + (80.0f * nCntScore), 250.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(880.0f + (80.0f * nCntScore), 250.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(800.0f + (80.0f * nCntScore), 430.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(880.0f + (80.0f * nCntScore), 430.0f, 0.0f);

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

		//テクスチャの座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	//頂点座標をアンロック
	g_pVtxBuffScore->Unlock();
}

//========================================================
//					得点の終了処理
//========================================================
void UninitScore(void)
{
	//テクスチャの破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//========================================================
//					得点の更新処理
//========================================================
void UpdateScore(void)
{
	if (g_bUse)
	{//得点が表示されていたら
		g_nCntScoreTime++;		//表示カウンター加算
		if (g_nCntScoreTime >= 150)
		{//カウンターが一定以上
			if (g_aScore[PLAYER_1].nScore >= 15)
			{//1Pの点が15点以上
				if (g_nCntScoreTime == 151)
				{//セットカウントを表示
					SetCounter(PLAYER_1);
					SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.4f, 0.0f), D3DXVECTOR2(200.0f, 170.0f), 150, 7);		//SetCount表記
					g_nSetPlayer1++;
					if (g_nSetPlayer1 >= 2)
					{//Set数が2以上
						GameSet(PLAYER_1, PLAYER_2);
						g_nCntScoreTime = 301;
						SetUI(D3DXVECTOR3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 1.4f, 0.0f), D3DXVECTOR2(200.0f, 170.0f), 150, 5);		//win表記
						SetUI(D3DXVECTOR3(SCREEN_WIDTH / 1.25f, SCREEN_HEIGHT / 1.4f, 0.0f), D3DXVECTOR2(200.0f, 170.0f), 150, 6);		//lose表記
					}
				}
				if (g_nCntScoreTime == 300)
				{//スコア、ゲーム状態をリセット
					ResetGame();
					g_aScore[PLAYER_1].nScore = 0;
					g_aScore[PLAYER_2].nScore = 0;
					g_nCntScoreTime = 0;
				}
				if (g_nCntScoreTime == 450)
				{//タイトルに戻る
					SetFade(MODE_TITLE);
					g_nCntScoreTime = 0;
				}
			}
			else if (g_aScore[PLAYER_2].nScore >= 15)
			{//2Pの点が15点以上
				if (g_nCntScoreTime == 151)
				{//セットカウントを表示
					SetCounter(PLAYER_2);
					SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.4f, 0.0f), D3DXVECTOR2(200.0f, 170.0f), 150, 7);		//SetCount表記
					g_nSetPlayer2++;
					if (g_nSetPlayer2 >= 2)
					{//Set数が2以上
						GameSet(PLAYER_2, PLAYER_1);
						g_nCntScoreTime = 301;
						SetUI(D3DXVECTOR3(SCREEN_WIDTH / 1.25f, SCREEN_HEIGHT / 1.4f, 0.0f), D3DXVECTOR2(200.0f, 170.0f), 150, 5);		//win表記
						SetUI(D3DXVECTOR3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 1.4f, 0.0f), D3DXVECTOR2(200.0f, 170.0f), 150, 6);		//lose表記
					}
				}
				if (g_nCntScoreTime >= 300)
				{//スコア、ゲーム状態をリセット
					ResetGame();
					g_aScore[PLAYER_1].nScore = 0;
					g_aScore[PLAYER_2].nScore = 0;
					g_nCntScoreTime = 0;
				}
				if (g_nCntScoreTime == 450)
				{//タイトルに戻る
					SetFade(MODE_TITLE);
					g_nCntScoreTime = 0;
				}
			}
			else
			{//ゲーム状態をリセット
				ResetGame();
				g_nCntScoreTime = 0;
			}
		}
	}
}

//========================================================
//					得点の描画処理
//========================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntScore = 0; nCntScore < NUM_SCORE * MAX_PLAYER; nCntScore++)
	{
		if (g_bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureScore);

			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,								//プリミティブの種類
				4 * nCntScore,
				2);													//プリミティブの数	
		}
	}
}

//========================================================
//					得点の設定処理
//========================================================
void SetPoint(int nIdxPlayer, D3DXVECTOR3 Bulletpos)
{
	VERTEX_2D *pVtx;
	float aPosTexU[MAX_PLAYER][NUM_SCORE];							//各桁の数値を格納

	g_bUse = true;		//スコアを使用状態にする

	if (nIdxPlayer == 0)
	{
		if (Bulletpos.x > FIELD_SIZE_X / 5 || Bulletpos.x < -FIELD_SIZE_X / 5)
		{//弾の位置が両端
			g_aScore[PLAYER_1].nScore += 3;

			//サウンド再生
			PlaySound(SOUND_LABEL_SE_KANKYAKU);
		}
		else
		{//それ以外
			g_aScore[PLAYER_1].nScore += 5;

			//サウンド再生
			PlaySound(SOUND_LABEL_SE_KANKYAKU);
		}
	}
	else if (nIdxPlayer == 1)
	{
		if (Bulletpos.x > FIELD_SIZE_X / 5 || Bulletpos.x < -FIELD_SIZE_X / 5)
		{//弾の位置が両端
			g_aScore[PLAYER_2].nScore += 3;

			//サウンド再生
			PlaySound(SOUND_LABEL_SE_KANKYAKU);
		}
		else
		{//それ以外
			g_aScore[PLAYER_2].nScore += 5;

			//サウンド再生
			PlaySound(SOUND_LABEL_SE_KANKYAKU);
		}
	}

	//プレイヤー[1]
	aPosTexU[0][0] = ((g_aScore[0].nScore % 100) / 10) * 0.1f;
	aPosTexU[0][1] = (g_aScore[0].nScore % 10) * 0.1f;

	//プレイヤー[2]
	aPosTexU[1][0] = ((g_aScore[1].nScore % 100) / 10) * 0.1f;
	aPosTexU[1][1] = (g_aScore[1].nScore % 10) * 0.1f;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	//プレイヤー[1]の得点
	for (int nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + aPosTexU[0][nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aPosTexU[0][nCntScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aPosTexU[0][nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aPosTexU[0][nCntScore], 1.0f);

		pVtx += 4;
	}

	//プレイヤー[2]の得点
	for (int nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + aPosTexU[1][nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aPosTexU[1][nCntScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aPosTexU[1][nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aPosTexU[1][nCntScore], 1.0f);

		pVtx += 4;
	}

	//頂点座標をアンロック
	g_pVtxBuffScore->Unlock();
}

//========================================================
//					ゲームのリセット処理
//========================================================
void ResetGame(void)
{
	Bullet *pBullet = GetBullet();		//ディスク情報の取得
	Player *pPlayer = GetPlayer();		//プレイヤー情報の取得

	g_bUse = false;				//得点表示を消す
	g_nCntScoreTime = 0;		//カウンターリセット

	//弾をリセット(再射出)する
	pBullet->bUse = false;
	if (g_nCntChange == PLAYER_1)
	{//1Pに再射出
		SetBullet(D3DXVECTOR3(FIELD_SIZE_X / 2, 30.0f, 0.0f), D3DXVECTOR3(-2.0f, 0.0f, -3.0f), 0);
		g_nCntChange++;
		DiscChange(PLAYER_2);
	}

	else if (g_nCntChange == PLAYER_2)
	{//2Pに再射出
		SetBullet(D3DXVECTOR3(FIELD_SIZE_X / 2, 30.0f, 0.0f), D3DXVECTOR3(-2.0f, 0.0f, 3.0f), 0);
		g_nCntChange--;
		DiscChange(PLAYER_1);
	}

	//1Pの状態と位置を初期化
	pPlayer->pos = D3DXVECTOR3(0.0f, 0.0f, -(FIELD_SIZE_Z - PLAYER_WIDTH));
	pPlayer->State = PLAYERSTATE_NORMAL;
	pPlayer++;

	//2Pの状態と位置を初期化
	pPlayer->pos = D3DXVECTOR3(0.0f, 0.0f, FIELD_SIZE_Z - PLAYER_WIDTH);
	pPlayer->State = PLAYERSTATE_NORMAL;
}

//========================================================
//					リザルトタイムの表示処理
//========================================================
//void DrawResultTime(int nResultTime)
//{
//	VERTEX_2D *pVtx;
//	float aPosTexU[NUM_SCORE];									//各桁の数値を収納
//
//	//各桁の数値を計算
//	aPosTexU[0] = ((nResultTime % 1000) / 100) * 0.1f;
//	aPosTexU[1] = ((nResultTime % 100) / 10) * 0.1f;
//	aPosTexU[2] = (nResultTime % 10) * 0.1f;
//
//	//頂点バッファをロックし、頂点情報へのポインタを取得
//	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (int nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
//	{
//		//頂点座標の設定
//		pVtx[0].pos = D3DXVECTOR3(570.0f + (60.0f * nCntScore), 220.0f, 0.0f);
//		pVtx[1].pos = D3DXVECTOR3(630.0f + (60.0f * nCntScore), 220.0f, 0.0f);
//		pVtx[2].pos = D3DXVECTOR3(570.0f + (60.0f * nCntScore), 300.0f, 0.0f);
//		pVtx[3].pos = D3DXVECTOR3(630.0f + (60.0f * nCntScore), 300.0f, 0.0f);
//
//		//頂点カラーの設定
//		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//		//テクスチャの設定
//		pVtx[0].tex = D3DXVECTOR2(0.0f + aPosTexU[nCntScore], 0.0f);
//		pVtx[1].tex = D3DXVECTOR2(0.1f + aPosTexU[nCntScore], 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(0.0f + aPosTexU[nCntScore], 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(0.1f + aPosTexU[nCntScore], 1.0f);
//
//		pVtx += 4;
//	}
//
//	//頂点座標をアンロック
//	g_pVtxBuffScore->Unlock();
//
//	DrawScore();
//}