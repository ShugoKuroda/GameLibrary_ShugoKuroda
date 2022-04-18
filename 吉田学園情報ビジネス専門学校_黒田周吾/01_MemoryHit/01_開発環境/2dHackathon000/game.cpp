//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : 黒田 周吾
//
//=============================================================================

//======================================================
// インクルード
//======================================================
#include "main.h"
#include "game.h"
#include "bg.h"
#include "time.h"
#include "sound.h"
#include "fade.h"
#include "input.h"
#include "particle.h"
#include "pause.h"
#include "game_logo.h"
#include "item.h"
#include "shadow.h"
#include "Frame.h"
#include "select.h"
#include <stdlib.h>
#include <time.h>
#include "score.h"
#include "time.h"
#include "effect.h"

//======================================================
// グローバル変数
//======================================================
int g_nCntSelect;			//選択画面を出すまでのカウンター

//======================================================
// ゲーム画面の初期化処理
//======================================================
void InitGame(void)
{
	//カウンターリセット
	g_nCntSelect = 0;

	//乱数の種を生成する
	srand((unsigned int)time(NULL));

	//背景の初期化処理
	InitBg();

	//ブロックの初期化処理
	InitItem();

	//---------------------------------------------------------
	//	アイテム配置
	//---------------------------------------------------------
	for (int nCntSet = 0; nCntSet <= 10; nCntSet++)
	{
		float posX = (float)(rand() % 5);
		float posY = (float)(rand() % 5);
		SetItem(D3DXVECTOR3(350.0f + (650.0f / 5) * posX, 100.0f + (650.0f / 5) * posY, 0.0f), rand() % 7, posX, posY);
	}

	//---------------------------------------------------------
	//	アイテム配置ここまで
	//---------------------------------------------------------

	//ポーズの初期化処理
	InitPause();

	//パーティクルの初期化処理
	InitParticle();

	//ゲーム中のロゴ表示初期化
	InitLogo();

	//ゲーム中のフェード初期化
	InitShadow();

	//選択処理の初期化
	InitFrame();
	InitSelect();

	//スコアの初期化
	InitScore();

	//タイマーの初期化
	InitTime();

	//エフェクトの初期化
	InitEffect();

	//サウンド再生
	PlaySound(SOUND_LABEL_BGM001);
}

//======================================================
//	ゲーム画面の終了処理
//======================================================
void UninitGame(void)
{
	//サウンド停止
	StopSound();

	//背景の終了処理
	UninitBg();

	//ブロックの終了処理
	UninitItem();

	//パーティクルの終了処理
	UninitParticle();

	//ポーズの終了処理
	UninitPause();

	//ゲーム中のロゴ表示終了処理
	UninitLogo();

	//ゲーム中のフェード終了
	UninitShadow();

	//選択処理の終了
	UninitFrame();
	UninitSelect();

	//タイマーの終了
	UninitTime();

	//エフェクトの終了
	UninitEffect();
}

//======================================================
//	ゲーム画面の更新処理
//======================================================
void UpdateGame(void)
{
	if (!UpdatePause())
	{//ポーズ中でなければ

		//背景の更新処理
		UpdateBg();

		//ブロックの更新処理
		UpdateItem();

		//パーティクルの更新処理
		UpdateParticle();

		//ゲーム中のロゴ表示更新
		UpdateLogo();

		//ゲーム中のフェード更新
		UpdateShadow();

		//スコア処理の更新
		UpdateScore();

		g_nCntSelect++;

		if (g_nCntSelect == 399)
		{
			SetTime(D3DXVECTOR3(SCREEN_WIDTH - 200, 90.0f, 0.0f), D3DXCOLOR(0.0f, 0.5f, 0.5f, 1.0f), 10);
		}
		if (g_nCntSelect >= 400)
		{
			if (!FinishGame())
			{
				//選択処理の更新
				UpdateFrame();
				UpdateSelect();

				//エフェクト処理の更新
				UpdateEffect();
			}

			//タイマー処理の更新
			UpdateTime();

			g_nCntSelect = 400;
		}
	}
}

//======================================================
//	ゲーム画面の描画処理
//======================================================
void DrawGame(void)
{
	//背景の描画処理
	DrawBg();

	//アイテムの描画処理
	DrawItem();

	//ゲーム中のフェード初期化
	DrawShadow();

	//パーティクルの描画処理
	DrawParticle();

	//ポーズの描画処理
	DrawPause();

	//ゲーム中のロゴ表示描画
	DrawLogo();

	if (g_nCntSelect >= 400)
	{
		//エフェクトの描画処理
		DrawEffect();

		//選択処理の描画処理
		DrawFrame();
		DrawSelect();

		//タイマーの描画処理
		DrawTime();
	}

	if (FinishGame())
	{
		//アイテムの描画処理
		DrawItem();

		//スコアの描画処理
		DrawScore();
	}
}