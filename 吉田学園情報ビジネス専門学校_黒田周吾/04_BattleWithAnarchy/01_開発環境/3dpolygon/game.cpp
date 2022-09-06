//=============================================================================
//
//	ゲーム画面処理 [game.cpp]
//	Author : 黒田 周吾
//
//=============================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include <stdio.h>
#include "load.h"
#include "game.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "input.h"
#include "shadow.h"
#include "wall.h"
#include "billboard.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "bullet.h"
#include "effect.h"
#include "player.h"
#include "line.h"
#include "sound.h"
#include "fade.h"
#include "pause.h"
#include "enemy.h"
#include "particle.h"
#include "score.h"
#include "time.h"
#include "ui.h"
#include "life.h"
#include "logo.h"

//======================================================
//	グローバル変数
//======================================================
LogoCount g_Logo;		//ロゴ出現情報

//======================================================
//	ゲーム画面の初期化処理
//======================================================
void InitGame(void)
{
	//ウインドウハンドルの取得
	HWND hWnd = GetHWnd();

	//ロゴ情報の初期化
	ZeroMemory(&g_Logo, sizeof(LogoCount));

	//ポーズ画面の初期化処理
	InitPause();

	//UI画面の初期化処理
	InitUI();

	//スコア画面の初期化処理
	InitScore();

	//タイマーの初期化処理
	InitTime();

	//ライフの初期化処理
	InitLife();

	//ロゴの初期化
	InitLogo();

	//ラインの初期化処理
	//InitLine();

	//影の初期化処理
	InitShadow();

	//モデルの初期化処理
	InitModel();

	//プレイヤーの初期化処理
	InitPlayer();

	//敵の初期化処理
	InitEnemy();

	//ロード処理
	LoadSetModel(hWnd);
	LoadSetPlayer(hWnd);
	LoadSetEnemy(hWnd);

	//メッシュフィールドの初期化処理
	InitMeshField();

	//図形(円柱)の初期化処理
	InitMeshCylinder();

	//ライトの初期化処理
	InitLight();

	//ビルボードの初期化処理
	//InitBillboard();

	//弾の初期化処理
	InitBullet();

	//エフェクトの初期化処理
	InitEffect();

	//パーティクルの初期化処理
	InitParticle();

	//カメラの初期化処理
	InitCamera();

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

	//ポーズの終了処理
	UninitPause();

	//UI画面の終了処理
	UninitUI();

	//スコアの終了処理
	UninitScore();

	//タイマーの終了処理
	UninitTime();

	//ライフの終了処理
	UninitLife();

	//ロゴの終了処理
	UninitLogo();

	//影の終了処理
	UninitShadow();

	//メッシュフィールドの終了処理
	UninitMeshField();

	//図形(円柱)の終了処理
	UninitMeshCylinder();

	//カメラの終了処理
	UninitCamera();

	//ライトの終了処理
	UninitLight();

	//ラインの終了処理
	//UninitLine();

	//プレイヤーの終了処理
	UninitPlayer();

	//敵の終了処理
	UninitEnemy();

	//モデルの終了処理
	UninitModel();

	//ビルボードの終了処理
	//UninitBillboard();

	//弾の終了処理
	UninitBullet();

	//エフェクトの終了処理
	UninitEffect();

	//パーティクルの終了処理
	UninitParticle();
}

//======================================================
//	ゲーム画面の更新処理
//======================================================
void UpdateGame(void)
{
	if (!g_Logo.bNot)
	{
		g_Logo.nCnt++;
		if (g_Logo.nCnt == 1)
		{
			SetLogo(0);
		}
		if (g_Logo.nCnt == 91)
		{
			SetLogo(1);
			g_Logo.bNot = true;
		}
	}

	if (!UpdatePause())
	{//ポーズ中でなければ

		//影の更新処理
		UpdateShadow();

		//UIの更新処理
		UpdateUI();

		//スコアの更新処理
		UpdateScore();

		//タイマーの更新処理
		UpdateTime();

		//ライフの更新処理
		UpdateLife();

		//ロゴの更新処理
		UpdateLogo();

		//メッシュフィールドの更新処理
		UpdateMeshField();

		//図形(円柱)の更新処理
		UpdateMeshCylinder();

		//カメラの更新処理
		UpdateCamera();

		//ライトの更新処理
		UpdateLight();

		//モデルの更新処理
		UpdateModel();

		//ラインの更新処理
		//UpdateLine();

		//プレイヤーの更新処理
		UpdatePlayer();

		//敵の更新処理
		UpdateEnemy();

		//ビルボードの更新処理
		//UpdateBillboard();

		//弾の更新処理
		UpdateBullet();

		//エフェクトの更新処理
		UpdateEffect();

		//パーティクルの更新処理
		UpdateParticle();
	}
}

//======================================================
//	ゲーム画面の描画処理
//======================================================
void DrawGame(void)
{
	//メッシュフィールドの描画処理
	DrawMeshField();

	//影の描画処理
	DrawShadow();

	//ビルボードの描画処理
	//5DrawBillboard();

	//図形(円柱)の描画処理
	DrawMeshCylinder();

	//モデルの描画処理
	DrawModel();

	//ラインの描画処理
	//DrawLine();

	//プレイヤーの描画処理
	DrawPlayer();

	//敵の描画処理
	DrawEnemy();

	//弾の描画処理
	DrawBullet();

	//エフェクトの描画処理
	DrawEffect();

	//パーティクルの描画処理
	DrawParticle();

	//カメラの設定処理
	SetCamera();

	//UI画面の描画処理
	DrawUI();

	//ロゴの描画処理
	DrawLogo();

	//ライフの描画処理
	DrawLife();

	//タイマーの描画処理
	DrawTime();

	//スコアの描画処理
	DrawScore();

	//ポーズ画面の描画処理
	DrawPause();
}

//======================================================
//	ロゴ出現情報を取得
//======================================================
LogoCount *GetLogoCount(void)
{
	return &g_Logo;
}