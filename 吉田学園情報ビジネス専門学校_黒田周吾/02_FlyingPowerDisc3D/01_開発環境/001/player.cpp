//========================================================
//
//					  プレイヤー
//					AUTHOR:越本愛彪,黒田周吾
//
//========================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "meshfield.h"
#include "bullet.h"
#include "model.h"
#include "sound.h"

//======================================================
//	マクロ定義
//======================================================
#define PLAYER_MOVE				(1.5f)		//プレイヤーの移動量
#define PLAYER_MOVEUP			(5.0f)		//プレイヤーのジャンプ力
#define PLAYER_HEIGHT			(8.0f)		//プレイヤーの高さ
#define PLAYER_MOVEDOWN			(0.3f)		//プレイヤーへの重力
#define PLAYER_ROTSPEED			(0.1f)		//プレイヤーの回転の減衰量
#define PLAYER_SLIDINGSPEED		(2.0f)		//プレイヤースライディング時の加速量
#define PLAYER_SIDINGTIME		(10)		//スライディングの持続時間
#define PLAYER_SLIDING_COOLTIME (90)		//スライディングのクールタイム

//-----------------------------------------------------------------------------
//	構造体の定義
//-----------------------------------------------------------------------------
//モデル情報の構造体
typedef struct
{
	LPD3DXMESH pMesh = NULL;		//メッシュ情報へのポインタ
	LPD3DXBUFFER pBuffMat = NULL;	//マテリアル情報へのポインタ
	DWORD nNumMat = 0;				//マテリアル情報の数
	bool bUse;						//使用しているかどうか
}PlayerPartsInfo;

//======================================================
//	グローバル宣言
//======================================================
PlayerPartsInfo g_aPartsInfo[MAX_PARTS];		//パーツ(モデル)の情報
Player g_player[MAX_PLAYER];					//プレイヤーの情報
int g_nJudge;
bool g_bGameSet;								//ゲームが終わったかどうか

//======================================================
//	プレイヤーの初期化処理
//======================================================
void InitPlayer(void)
{
	//プレイヤーのパーツ(モデル)情報の初期化
	ZeroMemory(&g_aPartsInfo, sizeof(g_aPartsInfo)); 
	//プレイヤー情報の初期化
	ZeroMemory(&g_player, sizeof(g_player));

	for (int nCntPlayer = 0, nCntPosPlayer = 1; nCntPlayer < MAX_PLAYER; nCntPlayer++, nCntPosPlayer *= -1)
	{
		//影の設定
		g_player[nCntPlayer].nIdxShadow = SetShadow(D3DXVECTOR3(g_player[nCntPlayer].pos.x, 1.0f, g_player[nCntPlayer].pos.z), g_player[nCntPlayer].rot, 30.0f, 30.0f);
	}
	g_nJudge = 0;
	g_bGameSet = false;
}

//======================================================
//	プレイヤーの終了処理
//======================================================
void UninitPlayer(void)
{
	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		//メッシュの破棄
		if (g_aPartsInfo[nCnt].pMesh != NULL)
		{
			g_aPartsInfo[nCnt].pMesh->Release();
			g_aPartsInfo[nCnt].pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_aPartsInfo[nCnt].pBuffMat != NULL)
		{
			g_aPartsInfo[nCnt].pBuffMat->Release();
			g_aPartsInfo[nCnt].pBuffMat = NULL;
		}
	}
}

//======================================================
//	プレイヤーの更新処理
//======================================================
void UpdatePlayer(void)
{
	//前回の位置の保存
	g_player[PLAYER_1].posOld = g_player[PLAYER_1].pos;
	g_player[PLAYER_2].posOld = g_player[PLAYER_2].pos;

	if (!g_bGameSet)
	{//ゲームが終わったら操作不能になる

	//--------------
	//プレイヤー[1]
	//--------------

	/*ディスクをキャッチした状態*/
		if (g_player[PLAYER_1].State == PLAYERSTATE_CATCH)
		{
			g_player[PLAYER_1].rotDest.y = D3DX_PI;

			if (g_player[PLAYER_1].MotionIdx.nMotionIdx != 3)
			{//投擲モーションの再生
				g_player[PLAYER_1].MotionIdx.nMotionIdx = 3;
				g_player[PLAYER_1].MotionIdx.nKeySetIdx = 0;
				g_player[PLAYER_1].MotionIdx.nFrame = 0;
			}
			g_player[PLAYER_1].nCntThrow++;
			if (g_player[PLAYER_1].nCntThrow >= 30)
			{
				g_player[PLAYER_1].State = PLAYERSTATE_HAVEDISC;
				g_player[PLAYER_1].nCntThrow = 0;		//リセット
			}
		}

		/*ディスクを持っている状態*/
		if (g_player[PLAYER_1].State == PLAYERSTATE_HAVEDISC)
		{
			if (GetJoypadTrigger(JOYKEY_A, PLAYER_1) && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1).z > 0.5f && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1).x < -0.5f)
			{//左前投げ
				g_player[PLAYER_1].Throw = PLAYERTHROW_STRAIGHT_LEFT;
				g_player[PLAYER_1].State = PLAYERSTATE_THROW;
				g_nJudge = PLAYER_1;
			}
			else if (GetJoypadTrigger(JOYKEY_A, PLAYER_1) && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1).z > 0.5f  && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1).x > 0.5f)
			{//右前投げ
				g_player[PLAYER_1].Throw = PLAYERTHROW_STRAIGHT_RIGHT;
				g_player[PLAYER_1].State = PLAYERSTATE_THROW;
				g_nJudge = PLAYER_1;
			}
			else if (GetJoypadTrigger(JOYKEY_A, PLAYER_1) && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1).x < -0.5f)
			{//左投げ
				g_player[PLAYER_1].Throw = PLAYERTHROW_LEFT;
				g_player[PLAYER_1].State = PLAYERSTATE_THROW;
				g_nJudge = PLAYER_1;
			}
			else if (GetJoypadTrigger(JOYKEY_A, PLAYER_1) && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1).x > 0.5f)
			{//右投げ
				g_player[PLAYER_1].Throw = PLAYERTHROW_RIGHT;
				g_player[PLAYER_1].State = PLAYERSTATE_THROW;
				g_nJudge = PLAYER_1;
			}
			else if (GetJoypadTrigger(JOYKEY_A, PLAYER_1) && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1).z > 0.5f)
			{//前投げ
				g_player[PLAYER_1].Throw = PLAYERTHROW_STRAIGHT;
				g_player[PLAYER_1].State = PLAYERSTATE_THROW;
				g_nJudge = PLAYER_1;
			}
		}

		/*ディスクを投げている状態*/
		if (g_player[PLAYER_1].State == PLAYERSTATE_THROW)
		{
			g_player[PLAYER_1].rotDest.y = D3DX_PI;

			if (g_player[PLAYER_1].MotionIdx.nMotionIdx != 2)
			{//投擲モーションの再生
				g_player[PLAYER_1].MotionIdx.nMotionIdx = 2;
				g_player[PLAYER_1].MotionIdx.nKeySetIdx = 0;
				g_player[PLAYER_1].MotionIdx.nFrame = 0;
			}
			g_player[PLAYER_1].nCntThrow++;
			if (g_player[PLAYER_1].nCntThrow >= 20)
			{
				g_player[PLAYER_1].State = PLAYERSTATE_NORMAL;		//通常の状態に戻す
				if (g_player[PLAYER_1].Throw == PLAYERTHROW_STRAIGHT)
				{//正面投げ
					SetBullet(D3DXVECTOR3(g_player[PLAYER_1].pos.x, g_player[PLAYER_1].pos.y, g_player[PLAYER_1].pos.z), D3DXVECTOR3(0.0f, 0.0f, 7.0f), 0);
					//サウンド再生
					PlaySound(SOUND_LABEL_SE_SHOT);
				}
				if (g_player[PLAYER_1].Throw == PLAYERTHROW_STRAIGHT_RIGHT)
				{//右前投げ
					SetBullet(D3DXVECTOR3(g_player[PLAYER_1].pos.x, g_player[PLAYER_1].pos.y, g_player[PLAYER_1].pos.z), D3DXVECTOR3(2.0f, 0.0f, 5.0f), 0);
					//サウンド再生
					PlaySound(SOUND_LABEL_SE_SHOT);
				}
				if (g_player[PLAYER_1].Throw == PLAYERTHROW_RIGHT)
				{//右投げ
					SetBullet(D3DXVECTOR3(g_player[PLAYER_1].pos.x, g_player[PLAYER_1].pos.y, g_player[PLAYER_1].pos.z), D3DXVECTOR3(5.0f, 0.0f, 2.0f), 0);
					//サウンド再生
					PlaySound(SOUND_LABEL_SE_SHOT);
				}
				if (g_player[PLAYER_1].Throw == PLAYERTHROW_STRAIGHT_LEFT)
				{//左前投げ
					SetBullet(D3DXVECTOR3(g_player[PLAYER_1].pos.x, g_player[PLAYER_1].pos.y, g_player[PLAYER_1].pos.z), D3DXVECTOR3(-2.0f, 0.0f, 5.0f), 0);
					//サウンド再生
					PlaySound(SOUND_LABEL_SE_SHOT);
				}
				if (g_player[PLAYER_1].Throw == PLAYERTHROW_LEFT)
				{//左投げ
					SetBullet(D3DXVECTOR3(g_player[PLAYER_1].pos.x, g_player[PLAYER_1].pos.y, g_player[PLAYER_1].pos.z), D3DXVECTOR3(-5.0f, 0.0f, 2.0f), 0);
					//サウンド再生
					PlaySound(SOUND_LABEL_SE_SHOT);
				}
				g_player[PLAYER_1].nCntThrow = 0;		//リセット
			}
		}

		/*通常*/
		if (g_player[PLAYER_1].State == PLAYERSTATE_NORMAL)
		{
			//スライディングの持続時間を戻す
			g_player[PLAYER_1].nSlidingTime = PLAYER_SIDINGTIME;

			//スライディングのクールタイムを減少させる
			g_player[PLAYER_1].nSlidingCoolTime--;

			//移動
			if (GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1) && GetJoypadTrigger(JOYKEY_A, PLAYER_1) && g_player[PLAYER_1].nSlidingCoolTime <= 0)
			{//移動+Aボタンでスライディング

				//スティック情報から移動量を取得
				D3DXVECTOR3 vector = GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1);

				while (g_player[PLAYER_1].nSlidingTime >= 0)
				{
					//スライディングの持続時間を減少させる
					g_player[PLAYER_1].nSlidingTime--;
					g_player[PLAYER_1].move += vector * PLAYER_SLIDINGSPEED;
				}

				//クールタイムの再設定
				g_player[PLAYER_1].nSlidingCoolTime = PLAYER_SLIDING_COOLTIME;
			}
			else if (GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1).x > 0.0f || GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1).z > 0.0f ||
				GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1).x < 0.0f || GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1).z < 0.0f)
			{//左スティック入力中の場合
			 //左スティック移動量を加算
				g_player[PLAYER_1].move += GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1);
				//プレイヤーの向き反映
				g_player[PLAYER_1].rotDest.y = atan2f(-g_player[PLAYER_1].move.x, -g_player[PLAYER_1].move.z);
			}
		}

		//ステージの当たり判定
		if (g_player[PLAYER_1].pos.z + (PLAYER_WIDTH / 2) >= 0.0f)
		{//奥壁
			g_player[PLAYER_1].pos.z = -(PLAYER_WIDTH / 2);
		}
		else if (g_player[PLAYER_1].pos.z - (PLAYER_WIDTH / 2) <= -FIELD_SIZE_Z / 2)
		{//手前壁
			g_player[PLAYER_1].pos.z = (-FIELD_SIZE_Z / 2) + (PLAYER_WIDTH / 2);
		}

		//--------------
		//プレイヤー[2]
		//--------------
		/*ディスクをキャッチした状態*/
		if (g_player[PLAYER_2].State == PLAYERSTATE_CATCH)
		{
			g_player[PLAYER_2].rotDest.y = 0.0f;

			if (g_player[PLAYER_2].MotionIdx.nMotionIdx != 3)
			{//投擲モーションの再生
				g_player[PLAYER_2].MotionIdx.nMotionIdx = 3;
				g_player[PLAYER_2].MotionIdx.nKeySetIdx = 0;
				g_player[PLAYER_2].MotionIdx.nFrame = 0;
			}
			g_player[PLAYER_2].nCntThrow++;
			if (g_player[PLAYER_2].nCntThrow >= 30)
			{
				g_player[PLAYER_2].State = PLAYERSTATE_HAVEDISC;
				g_player[PLAYER_2].nCntThrow = 0;		//リセット
			}
		}

		/*ディスクを持っている状態*/
		if (g_player[PLAYER_2].State == PLAYERSTATE_HAVEDISC)
		{
			if (GetJoypadTrigger(JOYKEY_A, PLAYER_2) && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2).z < -0.5f && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2).x < -0.5f)
			{//左前投げ
				g_player[PLAYER_2].Throw = PLAYERTHROW_STRAIGHT_LEFT;
				g_player[PLAYER_2].State = PLAYERSTATE_THROW;
				g_nJudge = PLAYER_2;
			}
			else if (GetJoypadTrigger(JOYKEY_A, PLAYER_2) && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2).z < -0.5f  && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2).x > 0.5f)
			{//右前投げ
				g_player[PLAYER_2].Throw = PLAYERTHROW_STRAIGHT_RIGHT;
				g_player[PLAYER_2].State = PLAYERSTATE_THROW;
				g_nJudge = PLAYER_2;
			}
			else if (GetJoypadTrigger(JOYKEY_A, PLAYER_2) && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2).x < -0.5f)
			{//左投げ
				g_player[PLAYER_2].Throw = PLAYERTHROW_LEFT;
				g_player[PLAYER_2].State = PLAYERSTATE_THROW;
				g_nJudge = PLAYER_2;
			}
			else if (GetJoypadTrigger(JOYKEY_A, PLAYER_2) && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2).x > 0.5f)
			{//右投げ
				g_player[PLAYER_2].Throw = PLAYERTHROW_RIGHT;
				g_player[PLAYER_2].State = PLAYERSTATE_THROW;
				g_nJudge = PLAYER_2;
			}
			else if (GetJoypadTrigger(JOYKEY_A, PLAYER_2) && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2).z < -0.5f)
			{//前投げ
				g_player[PLAYER_2].Throw = PLAYERTHROW_STRAIGHT;
				g_player[PLAYER_2].State = PLAYERSTATE_THROW;
				g_nJudge = PLAYER_2;
			}
		}

		/*ディスクを投げている状態*/
		if (g_player[PLAYER_2].State == PLAYERSTATE_THROW)
		{
			if (g_player[PLAYER_2].MotionIdx.nMotionIdx != 2)
			{//投擲モーションの再生
				g_player[PLAYER_2].MotionIdx.nMotionIdx = 2;
				g_player[PLAYER_2].MotionIdx.nKeySetIdx = 0;
				g_player[PLAYER_2].MotionIdx.nFrame = 0;
			}
			g_player[PLAYER_2].nCntThrow++;
			if (g_player[PLAYER_2].nCntThrow >= 20)
			{
				g_player[PLAYER_2].State = PLAYERSTATE_NORMAL;		//通常の状態に戻す
				if (g_player[PLAYER_2].Throw == PLAYERTHROW_STRAIGHT)
				{//正面投げ
					SetBullet(D3DXVECTOR3(g_player[PLAYER_2].pos.x, g_player[PLAYER_2].pos.y, g_player[PLAYER_2].pos.z), D3DXVECTOR3(0.0f, 0.0f, -7.0f), 0);
				}
				if (g_player[PLAYER_2].Throw == PLAYERTHROW_STRAIGHT_RIGHT)
				{//右前投げ
					SetBullet(D3DXVECTOR3(g_player[PLAYER_2].pos.x, g_player[PLAYER_2].pos.y, g_player[PLAYER_2].pos.z), D3DXVECTOR3(2.0f, 0.0f, -5.0f), 0);
				}
				if (g_player[PLAYER_2].Throw == PLAYERTHROW_RIGHT)
				{//右投げ
					SetBullet(D3DXVECTOR3(g_player[PLAYER_2].pos.x, g_player[PLAYER_2].pos.y, g_player[PLAYER_2].pos.z), D3DXVECTOR3(5.0f, 0.0f, -2.0f), 0);
				}
				if (g_player[PLAYER_2].Throw == PLAYERTHROW_STRAIGHT_LEFT)
				{//左前投げ
					SetBullet(D3DXVECTOR3(g_player[PLAYER_2].pos.x, g_player[PLAYER_2].pos.y, g_player[PLAYER_2].pos.z), D3DXVECTOR3(-2.0f, 0.0f, -5.0f), 0);
				}
				if (g_player[PLAYER_2].Throw == PLAYERTHROW_LEFT)
				{//左投げ
					SetBullet(D3DXVECTOR3(g_player[PLAYER_2].pos.x, g_player[PLAYER_2].pos.y, g_player[PLAYER_2].pos.z), D3DXVECTOR3(-5.0f, 0.0f, -2.0f), 0);
				}
				g_player[PLAYER_2].nCntThrow = 0;		//リセット
			}
		}

		/*通常*/
		if (g_player[PLAYER_2].State == PLAYERSTATE_NORMAL)
		{
			//スライディングの持続時間を戻す
			g_player[PLAYER_2].nSlidingTime = PLAYER_SIDINGTIME;

			//スライディングのクールタイムを減少させる
			g_player[PLAYER_2].nSlidingCoolTime--;

			//移動
			if (GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2) && GetJoypadTrigger(JOYKEY_A, PLAYER_2) && g_player[PLAYER_2].nSlidingCoolTime <= 0)
			{//移動+Aボタンでスライディング

				 //スティック情報から移動量を取得
				D3DXVECTOR3 vector = GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2);

				while (g_player[PLAYER_2].nSlidingTime >= 0)
				{
					//スライディングの持続時間を減少させる
					g_player[PLAYER_2].nSlidingTime--;
					g_player[PLAYER_2].move += vector * PLAYER_SLIDINGSPEED;
				}

				//クールタイムの再設定
				g_player[PLAYER_2].nSlidingCoolTime = PLAYER_SLIDING_COOLTIME;
			}
			else if (GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2).x > 0.0f || GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2).z > 0.0f ||
				GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2).x < 0.0f || GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2).z < 0.0f)
			{//左スティック入力中の場合
			 //左スティック移動量を加算
				g_player[PLAYER_2].move += GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2);
				//プレイヤーの向き反映
				g_player[PLAYER_2].rotDest.y = atan2f(-g_player[PLAYER_2].move.x, -g_player[PLAYER_2].move.z);
			}
		}

		//ステージの当たり判定
		if (g_player[PLAYER_2].pos.z + (PLAYER_WIDTH / 2) >= FIELD_SIZE_Z / 2)
		{//奥壁
			g_player[PLAYER_2].pos.z = (FIELD_SIZE_Z / 2) - (PLAYER_WIDTH / 2);
		}
		else if (g_player[PLAYER_2].pos.z - (PLAYER_WIDTH / 2) <= 0.0f)
		{//手前壁
			g_player[PLAYER_2].pos.z = (PLAYER_WIDTH / 2);
		}
	}

	//--------------
	//	  共通
	//--------------
	//慣性の追加
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_player[nCntPlayer].move.y--;

		//位置に移動量を加算
		g_player[nCntPlayer].pos += g_player[nCntPlayer].move;

		g_player[nCntPlayer].move.x += (-g_player[nCntPlayer].move.x) * 0.5f;
		g_player[nCntPlayer].move.z += (-g_player[nCntPlayer].move.z) * 0.5f;

		////回転の正規化
		//目的の角度が大きすぎたり小さすぎたりした時の計算
		if (g_player[nCntPlayer].rotDest.y - g_player[nCntPlayer].rot.y > D3DX_PI)
		{
			g_player[nCntPlayer].rotDest.y -= D3DX_PI * 2.0f;
		}
		else if (g_player[nCntPlayer].rotDest.y - g_player[nCntPlayer].rot.y < -D3DX_PI)
		{
			g_player[nCntPlayer].rotDest.y += D3DX_PI * 2.0f;
		}

		//慣性の計算
		g_player[nCntPlayer].rot.y += (g_player[nCntPlayer].rotDest.y - g_player[nCntPlayer].rot.y) * PLAYER_ROTSPEED;

		//計算結果が大きすぎたり小さすぎたりした時の調整
		if (g_player[nCntPlayer].rot.y < -D3DX_PI)
		{
			g_player[nCntPlayer].rot.y += D3DX_PI * 2.0f;
		}
		else if (g_player[nCntPlayer].rot.y > D3DX_PI)
		{
			g_player[nCntPlayer].rot.y -= D3DX_PI * 2.0f;
		}

		//壁・床の当たり判定処理
		if (g_player[nCntPlayer].pos.x - (PLAYER_WIDTH / 2) <= -FIELD_SIZE_X / 2)
		{//左壁
			g_player[nCntPlayer].pos.x = (-FIELD_SIZE_X / 2) + (PLAYER_WIDTH / 2);
		}
		else if (g_player[nCntPlayer].pos.x + (PLAYER_WIDTH / 2) >= FIELD_SIZE_X / 2)
		{//右壁
			g_player[nCntPlayer].pos.x = (FIELD_SIZE_X / 2) - (PLAYER_WIDTH / 2);
		}
		if (g_player[nCntPlayer].pos.y - (PLAYER_HEIGHT / 2) <= 0.0f)
		{//床との当たり判定
			g_player[nCntPlayer].pos.y = (PLAYER_HEIGHT / 2);
		}

		//影の位置更新
		SetPositionShadow(g_player[nCntPlayer].nIdxShadow, D3DXVECTOR3(g_player[nCntPlayer].pos.x, 1.0f, g_player[nCntPlayer].pos.z), g_player[nCntPlayer].rot);

		if (g_player[nCntPlayer].State != PLAYERSTATE_THROW && g_player[nCntPlayer].State != PLAYERSTATE_CATCH)
		{//プレイヤーがディスクを投げている、かつキャッチ中でない場合
			if (g_player[nCntPlayer].move.x >= 0.8f
				|| g_player[nCntPlayer].move.x <= -0.8f
				|| g_player[nCntPlayer].move.z >= 0.8f
				|| g_player[nCntPlayer].move.z <= -0.8f)
			{//プレイヤーが移動中
				if (g_player[nCntPlayer].MotionIdx.nMotionIdx != 1)
				{//ダッシュモーションの再生
					g_player[nCntPlayer].MotionIdx.nMotionIdx = 1;
					g_player[nCntPlayer].MotionIdx.nKeySetIdx = 0;
					g_player[nCntPlayer].MotionIdx.nFrame = 0;
				}
			}
			else
			{//止まっている場合
				if (g_player[nCntPlayer].MotionIdx.nMotionIdx != 0)
				{//ニュートラルモーションの再生
					g_player[nCntPlayer].MotionIdx.nMotionIdx = 0;
					g_player[nCntPlayer].MotionIdx.nKeySetIdx = 0;
					g_player[nCntPlayer].MotionIdx.nFrame = 0;
				}
			}
		}
		if(g_bGameSet)
		{
			if (g_player[nCntPlayer].State == PLAYERSTATE_WIN)
			{
				if (g_player[nCntPlayer].MotionIdx.nMotionIdx != 4)
				{//勝利モーションの再生
					g_player[nCntPlayer].MotionIdx.nMotionIdx = 4;
					g_player[nCntPlayer].MotionIdx.nKeySetIdx = 0;
					g_player[nCntPlayer].MotionIdx.nFrame = 0;
				}
			}
			else if (g_player[nCntPlayer].State == PLAYERSTATE_LOSE)
			{
				if (g_player[nCntPlayer].MotionIdx.nMotionIdx != 5)
				{//敗北モーションの再生
					g_player[nCntPlayer].MotionIdx.nMotionIdx = 5;
					g_player[nCntPlayer].MotionIdx.nKeySetIdx = 0;
					g_player[nCntPlayer].MotionIdx.nFrame = 0;
				}
			}
		}

		//モーション再生
		MotionPlayer(nCntPlayer);
	}

		//ディスクとの当たり判定
		if (g_nJudge == PLAYER_1)
		{//プレイヤー[1]が投げたディスクはプレイヤー[2]と当たり判定を行う
			CollisionDisc(PLAYER_2);
		}
		else if (g_nJudge == PLAYER_2)
		{//プレイヤー[2]が投げたディスクはプレイヤー[1]と当たり判定を行う
			CollisionDisc(PLAYER_1);
		}
}

//======================================================
//	プレイヤーの描画処理
//======================================================
void DrawPlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (g_player[nCntPlayer].bUse)
		{
			//デバイスの取得
			LPDIRECT3DDEVICE9 pDevice = GetDevice();

			D3DXMATRIX mtxRotParent;		//親のワールドマトリックス
			D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

			//ワールドマトリックスの初期化（親）
			D3DXMatrixIdentity(&mtxRotParent);

			//プレイヤーの向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_player[nCntPlayer].rot.y, g_player[nCntPlayer].rot.x, g_player[nCntPlayer].rot.z);

			D3DXMatrixMultiply(&mtxRotParent, &mtxRotParent, &mtxRot);

			//プレイヤーの位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_player[nCntPlayer].pos.x, g_player[nCntPlayer].pos.y, g_player[nCntPlayer].pos.z);

			D3DXMatrixMultiply(&mtxRotParent, &mtxRotParent, &mtxTrans);

			for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
			{
				//プレイヤーパーツ名の短縮用ポインタ
				PlayerParts *pPart = &g_player[nCntPlayer].aParts[nCntParts];

				if (pPart->bUse)
				{
					D3DMATERIAL9 matDef;	//現在のマテリアル保存用
					D3DXMATERIAL *pMat;		//マテリアルデータへのポインタ

					//ワールドマトリックスの初期化（子）
					D3DXMatrixIdentity(&g_player[nCntPlayer].aParts[pPart->nIndex].mtxWorld);

					//向きを反映
					D3DXMatrixRotationYawPitchRoll(&mtxRot,
						pPart->rot.y, pPart->rot.x, pPart->rot.z);

					D3DXMatrixMultiply(&g_player[nCntPlayer].aParts[pPart->nIndex].mtxWorld, &g_player[nCntPlayer].aParts[pPart->nIndex].mtxWorld, &mtxRot);

					//位置を反映
					D3DXMatrixTranslation(&mtxTrans,
						pPart->pos.x, pPart->pos.y, pPart->pos.z);

					D3DXMatrixMultiply(&g_player[nCntPlayer].aParts[pPart->nIndex].mtxWorld, &g_player[nCntPlayer].aParts[pPart->nIndex].mtxWorld, &mtxTrans);

					//自分の親モデルのマトリックスとの掛け算
					if (g_player[nCntPlayer].aParts[nCntParts].nParent < 0)
					{
						//モデルのマトリックス ＊ 親のワールドマトリックス
						D3DXMatrixMultiply(&g_player[nCntPlayer].aParts[pPart->nIndex].mtxWorld, &g_player[nCntPlayer].aParts[pPart->nIndex].mtxWorld, &mtxRotParent);
					}
					else
					{
						//モデルのマトリックス ＊ 親のワールドマトリックス
						D3DXMatrixMultiply(&g_player[nCntPlayer].aParts[pPart->nIndex].mtxWorld, &g_player[nCntPlayer].aParts[pPart->nIndex].mtxWorld, &g_player[nCntPlayer].aParts[pPart->nParent].mtxWorld);
					}

					if (g_aPartsInfo[pPart->nIndex].pMesh == NULL)
					{//パーツのモデルがNULLだったら
						continue;		//for文の最初に戻る
					}
	
					//ワールドマトリックスの設定
					pDevice->SetTransform(D3DTS_WORLD, &g_player[nCntPlayer].aParts[pPart->nIndex].mtxWorld);

					//現在のマテリアルを保持
					pDevice->GetMaterial(&matDef);

					//マテリアルデータへのポインタを取得
					pMat = (D3DXMATERIAL*)g_aPartsInfo[pPart->nIndex].pBuffMat->GetBufferPointer();

					for (int nCntMat = 0; nCntMat < (int)g_aPartsInfo[pPart->nIndex].nNumMat; nCntMat++)
					{
						//マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

						//モデルパーツの描画
						g_aPartsInfo[pPart->nIndex].pMesh->DrawSubset(nCntMat);
					}

					//保持していたマテリアルを戻す
					pDevice->SetMaterial(&matDef);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	モデルタイプ読み込み処理
//-----------------------------------------------------------------------------
HRESULT LoadPlayerType(HWND hWnd, char *sXFilePath)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntPatn = 0; nCntPatn < MAX_PARTS; nCntPatn++)
	{
		if (!g_aPartsInfo[nCntPatn].bUse)
		{
			//Xファイルの読み込み
			D3DXLoadMeshFromX(sXFilePath,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_aPartsInfo[nCntPatn].pBuffMat,
				NULL,
				&g_aPartsInfo[nCntPatn].nNumMat,
				&g_aPartsInfo[nCntPatn].pMesh);

			g_aPartsInfo[nCntPatn].bUse = true;

			return S_OK;
		}
	}

	MessageBox(hWnd, "読み込み可能モデル数を超えています", "警告！", MB_ICONWARNING);
	return E_FAIL;
}

//-----------------------------------------------------------------------------
//	プレイヤーのセット処理
//-----------------------------------------------------------------------------
void SetPlayer(Player player, D3DXVECTOR3 InitPos, int nCnt)
{
	g_player[nCnt] = player;		//プレイヤーの情報
	g_player[nCnt].pos = InitPos;	//プレイヤーの初期位置
}

//-----------------------------------------------------------------------------
//	モーション再生処理
//-----------------------------------------------------------------------------
void MotionPlayer(int nCntPlayer)
{
	int nMotion = g_player[nCntPlayer].MotionIdx.nMotionIdx;
	int nKey = g_player[nCntPlayer].MotionIdx.nKeySetIdx;
	int nFrame = g_player[nCntPlayer].aMotion[nMotion].nFrame[nKey];

	if (nFrame <= 0)
	{//テキストで設定されたフレーム数が0以下だった場合
		nFrame = 1;
	}

	//モーション再生処理
	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		if (g_player[nCntPlayer].aParts[nCnt].bUse)
		{
			//位置更新（ローカル座標）
			g_player[nCntPlayer].aParts[nCnt].pos += g_player[nCntPlayer].aMotion[nMotion].aKey[nKey].pos[nCnt] / (float)g_player[nCntPlayer].aMotion[nMotion].nFrame[nKey];

			//角度更新
			g_player[nCntPlayer].aParts[nCnt].rot += (g_player[nCntPlayer].aParts[nCnt].baseRot + g_player[nCntPlayer].aMotion[nMotion].aKey[nKey].rot[nCnt] - g_player[nCntPlayer].aParts[nCnt].rot) / (float)g_player[nCntPlayer].aMotion[nMotion].nFrame[nKey];
		}
	}

	//フレームの加算
	g_player[nCntPlayer].MotionIdx.nFrame++;

	if (g_player[nCntPlayer].aMotion[nMotion].nFrame[nKey] <= g_player[nCntPlayer].MotionIdx.nFrame)
	{//フレーム数が設定の値を超えたら

	 //再生中のキー数の加算
		g_player[nCntPlayer].MotionIdx.nKeySetIdx++;

		//フレームの初期化
		g_player[nCntPlayer].MotionIdx.nFrame = 0;

		if (g_player[nCntPlayer].aMotion[nMotion].nNumKey <= g_player[nCntPlayer].MotionIdx.nKeySetIdx)
		{//再生中のキー数が設定の値を超えたら
			if (g_player[nCntPlayer].aMotion[nMotion].nLoop == 1)
			{//モーションがループ設定だったら
				g_player[nCntPlayer].MotionIdx.nKeySetIdx = 0;
			}
			else
			{//現在再生中のモーションからニュートラルモーションに変更
				g_player[nCntPlayer].MotionIdx.nKeySetIdx = 0;
				g_player[nCntPlayer].MotionIdx.nMotionIdx = 0;
			}
		}
	}
}

//========================================================
//				ディスクとの当たり判定
//========================================================
void CollisionDisc(int nIdxPlayer)
{
	Bullet *pBullet = GetBullet();

	if (pBullet->bUse == true)
	{//ディスクが使用されているとき
		if (pBullet->pos.x + 10.0f > g_player[nIdxPlayer].pos.x - PLAYER_COLLISION &&
			pBullet->pos.x - 10.0f < g_player[nIdxPlayer].pos.x + PLAYER_COLLISION &&
			pBullet->pos.z + 10.0f > g_player[nIdxPlayer].pos.z - PLAYER_COLLISION &&
			pBullet->pos.z - 10.0f < g_player[nIdxPlayer].pos.z + PLAYER_COLLISION)
		{
			g_player[nIdxPlayer].State = PLAYERSTATE_CATCH;		//プレイヤーがディスクをキャッチした状態にする
			pBullet->bUse = false;								//弾を使用していない状態にする
			pBullet->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量をリセット
		}
	}
}

//========================================================
//		最初にディスクを投げるプレイヤーを交代
//========================================================
void DiscChange(int nIdxPlayer)
{
	g_nJudge = nIdxPlayer;
}

//========================================================
//		ゲームセット処理
//========================================================
void GameSet(int nWinPlayer, int nLosePlayer)
{
	g_bGameSet = true;
	g_player[nWinPlayer].State = PLAYERSTATE_WIN;
	g_player[nLosePlayer].State = PLAYERSTATE_LOSE;

	//サウンド再生
	PlaySound(SOUND_LABEL_SE_VICTORY);
}

//======================================================
//	プレイヤーの情報を取得
//======================================================
Player *GetPlayer(void)
{
	return &g_player[0];
}