#include "game.h"
#include "player.h"
#include "back.h"
#include "bullet.h"
#include "enemy.h"
#include "score.h"
#include "explosion.h"
#include "life.h"
#include "game_logo.h"
#include "sound.h"
#include "fade.h"
#include "input.h"
#include "particle.h"

//グローバル変数
bool g_bPause = false;		//ゲームのポーズ(一時停止)判定

//======================================================
//		ゲーム全体の初期化処理
//======================================================
void InitGame(void)
{
	g_bPause = false;		//ポーズ機能の初期化

	//背景の初期化処理
	InitBack();

	//プレイヤーの初期化処理
	InitPlayer();

	//エネミーの初期化
	InitEnemy();

	//弾の初期化
	InitBullet();

	//残機の初期化
	InitLife();

	//ゲーム中ロゴの初期化
	InitLogo();

	//スコアの初期化
	InitScore();

	//爆発初期化
	InitExplosion();

	//パーティクルの初期化
	InitParticle();

	//サウンド再生
	PlaySound(SOUND_LABEL_BGM001);
}

//======================================================
//		ゲーム全体の終了処理
//======================================================
void UninitGame(void)
{
	//サウンド停止
	StopSound();

	//背景の終了処理
	UninitBack();

	//プレイヤーの終了処理
	UninitPlayer();

	//エネミー終了処理
	UninitEnemy();

	//弾の終了処理
	UninitBullet();

	//残機の終了処理
	UninitLife();

	//ゲーム中ロゴの終了処理
	UninitLogo();

	//スコアの終了処理
	UninitScore();

	//爆発終了処理
	UninitExplosion();

	//パーティクルの終了処理
	UninitParticle();
}

//======================================================
//		ゲーム全体の更新処理
//======================================================
void UpdateGame(void)
{
	Player *pPlayer;	//プレイヤーのポインタ

	//プレイヤー情報を取得
	pPlayer = GetPlayer();

	if (GetKeyboardTrigger(DIK_P) == true)
	{//P(ポーズ)キーが押された
		g_bPause = g_bPause ? false : true;		//ポーズメニューの切り替え
	}

	if (pPlayer->bDie == true)
	{//プレイヤーが死亡した
		g_bPause = true;		//ゲームを止める
	}

	if (g_bPause == false)
	{//ポーズ中でなければ
		//背景の更新処理
		UpdateBack();

		//プレイヤーの更新処理
		UpdatePlayer();

		//エネミー更新処理
		UpdateEnemy();

		//弾の更新処理
		UpdateBullet();

		//スコアの更新処理
		UpdateScore();

		//爆発更新処理
		UpdateExplosion();

		//ゲーム中ロゴの更新処理
		UpdateLogo();

		//パーティクルの更新処理
		UpdateParticle();
	}
}

//======================================================
//		ゲーム全体の描画処理
//======================================================
void DrawGame(void)
{
	//背景の描画処理
	DrawBack();

	//プレイヤーの描画処理
	DrawPlayer();

	//エネミー描画処理
	DrawEnemy();

	//弾の描画処理
	DrawBullet();

	//残機の描画処理
	DrawLife();

	//スコアの描画処理
	DrawScore();

	//爆発描画処理
	DrawExplosion();

	//ゲーム中ロゴ描画処理
	DrawLogo();

	//パーティクルの描画処理
	DrawParticle();
}