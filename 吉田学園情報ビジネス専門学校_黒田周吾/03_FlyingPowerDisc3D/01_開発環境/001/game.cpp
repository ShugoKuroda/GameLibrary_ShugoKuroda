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
#include "load.h"
#include "game.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "input.h"
#include "shadow.h"
#include "billboard.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "bullet.h"
#include "effect.h"
#include "player.h"
#include "sound.h"
#include "fade.h"
#include "pause.h"
#include "time.h"
#include "score.h"
#include "ui.h"
#include "set.h"

//======================================================
//	ゲーム画面の初期化処理
//======================================================
void InitGame(void)
{
	//ウインドウハンドルの取得
	HWND hWnd = GetHWnd();

	//ポーズ画面の初期化処理
	InitPause();

	//得点処理の初期化
	InitScore();

	//セットカウント処理の初期化
	InitSet();

	//UIの初期化
	InitUI();

	//影の初期化処理
	InitShadow();

	//モデルの初期化処理
	InitModel();

	//プレイヤーの初期化処理
	InitPlayer();

	//ロード処理
	LoadSetModel(hWnd);
	LoadSetPlayer(hWnd);

	//メッシュフィールドの初期化処理
	InitMeshField();

	//図形(円柱)の初期化処理
	InitMeshCylinder();

	//ライトの初期化処理
	InitLight();

	//ビルボードの初期化処理
	InitBillboard();

	//弾の初期化処理
	InitBullet();

	//エフェクトの初期化処理
	InitEffect();

	//カメラの初期化処理
	InitCamera();

	//サウンド再生
	PlaySound(SOUND_LABEL_BGM001);

	//ゲームのリセット
	ResetGame();
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

	//得点の終了処理
	UninitScore();

	//セットカウント処理の終了
	UninitSet();

	//UIの終了処理
	UninitUI();

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

	//プレイヤーの終了処理
	UninitPlayer();

	//モデルの終了処理
	UninitModel();

	//ビルボードの終了処理
	UninitBillboard();

	//弾の終了処理
	UninitBullet();

	//エフェクトの終了処理
	UninitEffect();
}

//======================================================
//	ゲーム画面の更新処理
//======================================================
void UpdateGame(void)
{
	if (!UpdatePause())
	{//ポーズ中でなければ

		//UIの更新処理
		UpdateUI();

		//得点の更新処理
		UpdateScore();

		//セットカウント処理の更新処理
		UpdateSet();

		//影の更新処理
		UpdateShadow();

		//メッシュフィールドの更新処理
		UpdateMeshField();

		//カメラの更新処理
		UpdateCamera();

		//ライトの更新処理
		UpdateLight();

		//モデルの更新処理
		UpdateModel();

		//プレイヤーの更新処理
		UpdatePlayer();

		//ビルボードの更新処理
		UpdateBillboard();

		//弾の更新処理
		UpdateBullet();

		//エフェクトの更新処理
		UpdateEffect();
	}
}

//======================================================
//	ゲーム画面の描画処理
//======================================================
void DrawGame(int nCntCamera)
{
	//カメラの設定
	SetCamera(nCntCamera);

	//メッシュフィールドの描画処理
	DrawMeshField();

	//影の描画処理
	DrawShadow();

	//ビルボードの描画処理
	DrawBillboard();

	//図形(円柱)の描画処理
	DrawMeshCylinder();

	//弾の描画処理
	DrawBullet();

	//プレイヤーの描画処理
	DrawPlayer();

	//モデルの描画処理
	DrawModel(nCntCamera);

	//エフェクトの描画処理
	DrawEffect();

	//得点の描画
	DrawScore();

	//セットカウントの描画
	DrawSet();

	//ポーズ画面の描画処理
	DrawPause();

	//UIの描画
	DrawUI();
}