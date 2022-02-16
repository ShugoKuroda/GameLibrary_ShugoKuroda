//===================================================================
//
//	プレイヤー処理[player.cpp]
//	Author:黒田 周吾
//
//===================================================================

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
#include "inputMouse.h"
#include "function.h"
#include "enemy.h"
#include "particle.h"
#include "score.h"
#include "logo.h"

//======================================================
//	マクロ定義
//======================================================
#define PLAYER_WIDTH			(50.0f)		//プレイヤーの幅
#define PLAYER_HEIGHT			(8.0f)		//プレイヤーの高さ
#define PLAYER_MOVEDOWN			(0.3f)		//プレイヤーへの重力
#define PLAYER_MOVEATTACK		(1.2f)		//攻撃中の前移動
#define PLAYER_SWORDCOLLISION	(50.0f)		//プレイヤーの剣の当たり判定サイズ(球)

//-----------------------------------------------------------------------------
//	構造体の定義
//-----------------------------------------------------------------------------
//モーション番号情報の構造体
typedef struct
{
	int nFrame;					//現在のフレーム数
	int nKeySetIdx;				//再生中のキーセット番号
	int nMotionIdx;				//再生中のモーション番号
}PlayerMotionIdx;

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
D3DXMATRIX g_PlayerPartsmtxWorld[MAX_PARTS];	//ワールドマトリックス(プレイヤーのパーツ)
PlayerPartsInfo g_aPartsInfo[MAX_PARTS];		//モデル(パーツ)の情報
PlayerMotionIdx	g_PlayerMotionIdx;				//プレイヤーのモーション番号情報
Player g_Player;		//プレイヤーの情報

//======================================================
//	プレイヤーの初期化処理
//======================================================
void InitPlayer(void)
{
	//プレイヤーのワールドマトリックスの初期化
	ZeroMemory(&g_PlayerPartsmtxWorld, sizeof(g_PlayerPartsmtxWorld));
	//プレイヤー構造体の初期化
	ZeroMemory(&g_aPartsInfo, sizeof(g_aPartsInfo)); 
	ZeroMemory(&g_Player, sizeof(Player));
	//プレイヤーのモーション番号情報の初期化
	ZeroMemory(&g_PlayerMotionIdx, sizeof(PlayerMotionIdx));

	//プレイヤーの初期位置設定
	g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, -200.0f);

	//影の設定
	g_Player.nIdxShadow = SetShadow(D3DXVECTOR3(g_Player.pos.x, 1.0f, g_Player.pos.z), g_Player.rot, 30.0f, 30.0f);
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
	//前回の位置を保存
	g_Player.posOld = g_Player.pos;

	//カメラ情報の取得
	Camera *pCamera = GetCamera();

	//敵情報の取得
	Enemy *pEnemy = GetEnemy();

	//剣先の位置情報を取得(ワールドマトリックスからワールド座標を取得)
	D3DXVECTOR3 SwordPos = D3DXVECTOR3(
		g_PlayerPartsmtxWorld[g_Player.aParts[17].nIndex]._41,
		g_PlayerPartsmtxWorld[g_Player.aParts[17].nIndex]._42,
		g_PlayerPartsmtxWorld[g_Player.aParts[17].nIndex]._43);

	if (g_Player.bUse)
	{
		if (!g_Player.bIsDie)
		{//プレイヤーが死亡状態のときは操作できない
			if (!g_Player.bIsDamage)
			{//プレイヤーがダメージ状態の時は操作できない
				if (!g_Player.bIsAttackingSword)
				{//プレイヤーが近接攻撃中の場合は操作できない
					if (GetKeyboardPress(DIK_LSHIFT) == true)
					{//ダッシュ
						g_Player.bIsDashing = true;			//ダッシュ判定を有効にする
						g_Player.fMoveDash = 0.4f;			//移動量を増加
					}
					else
					{//ダッシュ判定を無効にする
						g_Player.bIsDashing = false;
						g_Player.fMoveDash = 0.0f;
					}

					if (GetKeyboardPress(DIK_A) == true)
					{//左移動
						if (GetKeyboardPress(DIK_W) == true)
						{
							g_Player.move.x += sinf(pCamera->rot.y - (D3DX_PI / 4)) * (g_Player.fMove + g_Player.fMoveDash);
							g_Player.move.z += cosf(pCamera->rot.y - (D3DX_PI / 4)) * (g_Player.fMove + g_Player.fMoveDash);
							g_Player.rotDest.y = (D3DX_PI / 1.5f) + pCamera->rot.y;
						}
						else if (GetKeyboardPress(DIK_S) == true)
						{
							g_Player.move.x += sinf(pCamera->rot.y - (D3DX_PI / 1.5f)) * (g_Player.fMove + g_Player.fMoveDash);
							g_Player.move.z += cosf(pCamera->rot.y - (D3DX_PI / 1.5f)) * (g_Player.fMove + g_Player.fMoveDash);
							g_Player.rotDest.y = (D3DX_PI / 4) + pCamera->rot.y;
						}
						else
						{
							g_Player.move.x += sinf(pCamera->rot.y - (D3DX_PI / 2)) * (g_Player.fMove + g_Player.fMoveDash);
							g_Player.move.z += cosf(pCamera->rot.y - (D3DX_PI / 2)) * (g_Player.fMove + g_Player.fMoveDash);
							g_Player.rotDest.y = (D3DX_PI / 2) + pCamera->rot.y;
						}
					}

					else if (GetKeyboardPress(DIK_D) == true)
					{//右移動
						if (GetKeyboardPress(DIK_W) == true)
						{
							g_Player.move.x -= sinf(pCamera->rot.y - (D3DX_PI / 1.5f)) * (g_Player.fMove + g_Player.fMoveDash);
							g_Player.move.z -= cosf(pCamera->rot.y - (D3DX_PI / 1.5f)) * (g_Player.fMove + g_Player.fMoveDash);
							g_Player.rotDest.y = (-D3DX_PI / 1.5f) + pCamera->rot.y;
						}
						else if (GetKeyboardPress(DIK_S) == true)
						{
							g_Player.move.x -= sinf(pCamera->rot.y - (D3DX_PI / 4)) * (g_Player.fMove + g_Player.fMoveDash);
							g_Player.move.z -= cosf(pCamera->rot.y - (D3DX_PI / 4)) * (g_Player.fMove + g_Player.fMoveDash);
							g_Player.rotDest.y = (-D3DX_PI / 4) + pCamera->rot.y;
						}
						else
						{
							g_Player.move.x -= sinf(pCamera->rot.y - (D3DX_PI / 2)) * (g_Player.fMove + g_Player.fMoveDash);
							g_Player.move.z -= cosf(pCamera->rot.y - (D3DX_PI / 2)) * (g_Player.fMove + g_Player.fMoveDash);
							g_Player.rotDest.y = (-D3DX_PI / 2) + pCamera->rot.y;
						}
					}

					else if (GetKeyboardPress(DIK_W) == true)
					{//前移動
						g_Player.move.x += sinf(pCamera->rot.y) * (g_Player.fMove + g_Player.fMoveDash);
						g_Player.move.z += cosf(pCamera->rot.y) * (g_Player.fMove + g_Player.fMoveDash);
						g_Player.rotDest.y = D3DX_PI + pCamera->rot.y;
					}

					else if (GetKeyboardPress(DIK_S) == true)
					{//後移動
						g_Player.move.x -= sinf(pCamera->rot.y) * (g_Player.fMove + g_Player.fMoveDash);
						g_Player.move.z -= cosf(pCamera->rot.y) * (g_Player.fMove + g_Player.fMoveDash);
						g_Player.rotDest.y = 0.0f + pCamera->rot.y;
					}

					if (!g_Player.bIsJumping)
					{//プレイヤーがジャンプ中でない場合
						if (GetKeyboardTrigger(DIK_SPACE) == true)
						{//ジャンプ
							g_Player.move.y = g_Player.fJump;

							//プレイヤーをジャンプ中判定にする
							g_Player.bIsJumping = true;
						}
					}
				}
			}
			if (g_Player.bIsDamage)
			{//プレイヤーがダメージ状態の場合
				g_Player.nCntDamage++;			//ダメージ状態切り替えカウンターの加算

				g_Player.move.x += g_Player.moveKnockBack.x;
				g_Player.move.z += g_Player.moveKnockBack.z;

				if (g_PlayerMotionIdx.nMotionIdx != 9)
				{//吹っ飛びモーションの再生
					g_PlayerMotionIdx.nMotionIdx = 9;
					g_PlayerMotionIdx.nKeySetIdx = 0;
					g_PlayerMotionIdx.nFrame = 0;
				}
				if (g_Player.nCntDamage == 1)
				{
					g_Player.move.y = (g_Player.fJump / 3);
				}
				if (g_Player.nCntDamage >= 30)
				{//カウンターが一定以上	
					g_Player.bIsDamage = false;			//プレイヤーのダメージ状態を解く
					g_Player.nCntDamage = 0;			//ダメージカウンターのリセット
				}
			}
			if (!g_Player.bIsDamage)
			{//被ダメ中は攻撃または武器切り替えができない
				if (GetMouseTrigger(CLICK_RIGHT) == true)
				{//右クリック時(トリガー)
					switch (g_Player.Weapon)
					{//武器の切り替え
					case PLAYERWEAPON_GUN:			//プレイヤーが銃を所持している場合
						g_Player.Weapon = PLAYERWEAPON_SWORD;			//剣に切り替える
						g_Player.AttackCombo = PLAYERATTACK_1COMBO;		//コンボ状況の初期化
						g_Player.bIsAttacking = false;					//攻撃フラグ無効
						break;
					case PLAYERWEAPON_SWORD:		//プレイヤーが剣を所持している場合
						//銃に切り替える
						g_Player.Weapon = PLAYERWEAPON_GUN;
						g_Player.bIsAttackingSword = false;				//近接攻撃フラグ無効
						break;
					}
				}
				switch (g_Player.Weapon)
				{//武器ごとの攻撃
				case PLAYERWEAPON_GUN:			//プレイヤーが銃を所持している場合
					if (GetMousePress(CLICK_LEFT) == true)
					{//左クリック時(プレス)
						g_Player.nCntShot++;				//射撃間隔のカウンター
						g_Player.bIsAttacking = true;		//プレイヤーが攻撃中のフラグを立てる(他のモーション誤再生防止)

						if (g_PlayerMotionIdx.nMotionIdx != 5)
						{//射撃モーションの再生
							g_PlayerMotionIdx.nMotionIdx = 5;
							g_PlayerMotionIdx.nKeySetIdx = 0;
							g_PlayerMotionIdx.nFrame = 0;
						}

						if (g_Player.nCntShot > 20)
						{//射撃カウンターが一定以上で射撃

							g_Player.nCntShot = 0;		//射撃カウンターリセット

							 //弾の設置(移動量(ベクトル)はプレイヤーの向き)
							SetBullet(D3DXVECTOR3((sinf(g_Player.rot.y) * -42.0f) + g_Player.pos.x, g_Player.pos.y + 57.0f, (cosf(g_Player.rot.y) * -42.0f) + g_Player.pos.z),
								D3DXVECTOR3(sinf(g_Player.rot.y) * -BULLET_MOVE, 0.0f, cosf(g_Player.rot.y) * -BULLET_MOVE),
								0);

							//サウンド再生
							PlaySound(SOUND_LABEL_SE_SHOT);
						}
						//パーティクルの設置(バックパックの噴射口の火)
						SetParticle(D3DXVECTOR3((sinf(g_Player.rot.y) * 10.0f) + g_Player.pos.x, g_Player.pos.y + 50.0f, (cosf(g_Player.rot.y) * 10.0f) + g_Player.pos.z),	//位置
							D3DXVECTOR3(sinf((float)(rand() % 629) / 200) * (float)(rand() % 629 - 314) / 100.0f,		//移動量
								-(float)(rand() % 629) / 200.0f,
								cosf((float)(rand() % 629) / 200) * (float)(rand() % 629 - 314) / 100.0f),
							D3DXCOLOR(1.0f, 0.6f, 0.6f, 1.0f),		//色
							D3DXVECTOR3(0.7f, 0.7f, 0.7f),			//大きさ
							1);			//種類
					}
					else
					{//左クリックされていない
						g_Player.nCntShot = 0;			//カウンターのリセット
						g_Player.bIsAttacking = false;	//プレイヤーの攻撃判定を無効にする
					}
					break;
				case PLAYERWEAPON_SWORD:		//プレイヤーが剣を所持している場合
					if (GetMouseTrigger(CLICK_LEFT) == true)
					{//左クリック時(トリガー)
						g_Player.bIsAttacking = true;			//プレイヤーが攻撃中のフラグを立てる(他のモーション誤再生防止)
						g_Player.bIsAttackingSword = true;		//プレイヤーが近接攻撃中のフラグを立てる(攻撃中のキー入力無効用)
						switch (g_Player.AttackCombo)
						{
						case PLAYERATTACK_1COMBO:		//0コンボの場合
							if (g_PlayerMotionIdx.nMotionIdx != 6)
							{//近接攻撃モーションの再生(一撃目)
								g_PlayerMotionIdx.nMotionIdx = 6;
								g_PlayerMotionIdx.nKeySetIdx = 0;
								g_PlayerMotionIdx.nFrame = 0;
								g_Player.nCntAttack = 0;			//カウンターリセット
							}
							//コンボ状態を進める(二撃目)
							g_Player.AttackCombo = PLAYERATTACK_2COMBO;
							break;
						case PLAYERATTACK_2COMBO:			//1コンボの場合
							if (g_Player.nCntAttack >= 20)
							{//連続攻撃防止カウンター
								if (g_PlayerMotionIdx.nMotionIdx != 7)
								{//近接攻撃モーションの再生(二撃目)
									g_PlayerMotionIdx.nMotionIdx = 7;
									g_PlayerMotionIdx.nKeySetIdx = 0;
									g_PlayerMotionIdx.nFrame = 0;
									g_Player.nCntAttack = 0;			//カウンターリセット
								}
								//コンボ状態を進める(三撃目)
								g_Player.AttackCombo = PLAYERATTACK_3COMBO;
							}
							break;
						case PLAYERATTACK_3COMBO:			//2コンボの場合
							if (g_Player.nCntAttack >= 20)
							{//連続攻撃防止カウンター
								if (g_PlayerMotionIdx.nMotionIdx != 8)
								{//近接攻撃モーションの再生(三撃目)
									g_PlayerMotionIdx.nMotionIdx = 8;
									g_PlayerMotionIdx.nKeySetIdx = 0;
									g_PlayerMotionIdx.nFrame = 0;
									g_Player.nCntAttack = 0;			//カウンターリセット
								}
								//コンボ状態を繰り返し状態に移行する
								g_Player.AttackCombo = PLAYERATTACK_LOOP;
							}
							break;
						case PLAYERATTACK_LOOP:			//コンボが終わった場合
							if (g_Player.nCntAttack >= 30)
							{
								//コンボ状態を最初に戻す(一撃目)
								g_Player.AttackCombo = PLAYERATTACK_1COMBO;
							}
						}
					}
					if (g_Player.bIsAttacking)
					{//プレイヤーが攻撃中の場合
						g_Player.nCntAttack++;
						if (g_Player.nCntAttack >= 15 && g_Player.nCntAttack < 20)
						{//攻撃モーション内で剣を振り下ろしている時
							g_Player.move.x += sinf(g_Player.rot.y) * -PLAYER_MOVEATTACK;
							g_Player.move.z += cosf(g_Player.rot.y) * -PLAYER_MOVEATTACK;

							for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++)
							{
								if (pEnemy->bUse)
								{//敵が使用されている場合
									if (!pEnemy->bIsSwordHit)
									{//剣攻撃が1回当たったら、次の攻撃まで攻撃が当たらない
										if (SphereCollision(SwordPos, pEnemy->pos, PLAYER_SWORDCOLLISION, ENEMY_COLLISION))
										{//敵とプレイヤーの剣の当たり判定

											//サウンド再生
											PlaySound(SOUND_LABEL_SE_HIT);

											pEnemy->bIsSwordHit = true;			//敵を剣攻撃Hit状態にする
											pEnemy->nCntPlayerAttack = 0;		//敵の多段剣攻撃防止カウンターのリセット

											pEnemy->bIsDamage = true;		//敵をダメージ状態にする

											//敵とプレイヤーの距離差分を保存(目的の位置 - 現在の位置)
											D3DXVECTOR3 vecToPlayer = pEnemy->pos - g_Player.pos;

											//敵からプレイヤーへのベクトル(移動量)に変換する
											D3DXVec3Normalize(&vecToPlayer, &vecToPlayer);

											//移動量の更新
											pEnemy->moveKnockBack.x = vecToPlayer.x;
											pEnemy->moveKnockBack.z = vecToPlayer.z;

											//プレイヤーの向き更新
											pEnemy->rotDest.y = atan2f(vecToPlayer.x, vecToPlayer.z);

											if (g_PlayerMotionIdx.nMotionIdx == 6 || g_PlayerMotionIdx.nMotionIdx == 7)
											{//プレイヤーの攻撃が1、2段階目の場合
												pEnemy->nLife -= 20.0f;		//敵の体力減少

												//スコア加算処理
												AddScore(0);

												//パーティクル処理
												for (int nCnt = 0; nCnt < 20; nCnt++)
												{
													SetParticle(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y, pEnemy->pos.z),		//位置
														D3DXVECTOR3(sinf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f,		//移動量
															tanf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f,
															cosf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f),
														D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f),		//色
														D3DXVECTOR3(1.0f, 1.0f, 1.0f),			//大きさ
														0);				//大きさ
												}
											}
											else if (g_PlayerMotionIdx.nMotionIdx == 8)
											{//プレイヤーの攻撃が3段階目の場合
												pEnemy->nLife -= 30.0f;		//敵の体力減少

												//スコア加算
												AddScore(1);

												//パーティクル処理
												for (int nCnt = 0; nCnt < 20; nCnt++)
												{
													SetParticle(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y, pEnemy->pos.z),		//位置
														D3DXVECTOR3(sinf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f,		//移動量
															tanf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f,
															cosf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f),
														D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f),		//色
														D3DXVECTOR3(3.0f, 3.0f, 3.0f),			//大きさ
														0);		//種類
												}
											}
										}
									}
								}
							}
						}
						if (g_Player.nCntAttack >= 40)
						{//一定時間経ったら(コンボ中に攻撃しなかったら)
							g_Player.bIsAttacking = false;			//攻撃フラグ無効
							g_Player.bIsAttackingSword = false;		//近接攻撃フラグ無効
							g_Player.nCntAttack = 0;		//カウンターリセット
							g_Player.AttackCombo = PLAYERATTACK_1COMBO;			//コンボ状況をリセット
						}
					}
					break;
				}
			}
			g_Player.move.y -= PLAYER_MOVEDOWN;		//重力負荷をかける
			g_Player.pos += g_Player.move;			//位置更新

			//ブロックの当たり判定
			g_Player.bIsJumping = !CollisionModel(&g_Player.pos, &g_Player.posOld, &g_Player.move, PLAYER_WIDTH, PLAYER_HEIGHT, 0);

			//移動量の減衰(目的の値 - 現在の値) * 減衰係数
			g_Player.move.x += -g_Player.move.x * 0.2f;
			g_Player.move.z += -g_Player.move.z * 0.2f;

			//壁・床の当たり判定処理
			if (g_Player.pos.x - (PLAYER_WIDTH / 2) <= -2000.0f / 2)
			{//左壁
				g_Player.pos.x = (-2000.0f / 2) + (PLAYER_WIDTH / 2);
			}
			else if (g_Player.pos.x + (PLAYER_WIDTH / 2) >= 2000.0f / 2)
			{//右壁
				g_Player.pos.x = (2000.0f / 2) - (PLAYER_WIDTH / 2);
			}
			if (g_Player.pos.z + (PLAYER_WIDTH / 2) >= 2000.0f / 2)
			{//奥壁
				g_Player.pos.z = (2000.0f / 2) - (PLAYER_WIDTH / 2);
			}
			else if (g_Player.pos.z - (PLAYER_WIDTH / 2) <= -2000.0f / 2)
			{//手前壁
				g_Player.pos.z = (-2000.0f / 2) + (PLAYER_WIDTH / 2);
			}
			if (g_Player.pos.y - (PLAYER_HEIGHT / 2) <= 0.0f)
			{//床
				g_Player.pos.y = (PLAYER_HEIGHT / 2);
				g_Player.bIsJumping = false;
				g_Player.move.y = 0.0f;			//移動量Yの初期化
			}
		}

		if (g_Player.bIsDie)
		{//プレイヤーが死亡したら
			g_Player.nCntDie++;		//カウンター加算
			if (g_PlayerMotionIdx.nMotionIdx != 10)
			{//やられモーションの再生
				g_PlayerMotionIdx.nMotionIdx = 10;
				g_PlayerMotionIdx.nKeySetIdx = 0;
				g_PlayerMotionIdx.nFrame = 0;
			}
			if (g_Player.nCntDie <= 100)
			{//一定時間下に沈ませる
				g_Player.pos.y -= 0.35f;
			}
			if (g_Player.nCntDie == 101)
			{//モーション再生が終わったら
				g_Player.bUse = false;		//プレイヤーを消す

				//パーティクル処理
				for (int nCnt = 0; nCnt < 20; nCnt++)
				{
					SetParticle(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z),		//位置
						D3DXVECTOR3(sinf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f,		//移動量
							tanf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f,
							cosf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f),
						D3DXCOLOR(0.0f, 0.5f, 0.5f, 1.0f),		//色
						D3DXVECTOR3(3.0f, 3.0f, 3.0f),			//大きさ
						0);			//種類

					//サウンド再生
					PlaySound(SOUND_LABEL_SE_EXPLOSION);
				}

				//プレイヤーの敗北ロゴを出現させる
				SetLogo(3);
			}
		}

		if (!g_Player.bIsDamage)
		{//プレイヤーがダメージ状態でない場合
			if (!g_Player.bIsAttacking)
			{//プレイヤーが攻撃中でない場合
				if (g_Player.bIsJumping)
				{//プレイヤーがジャンプ中の場合
					if (g_PlayerMotionIdx.nMotionIdx != 3)
					{//ジャンプモーションの再生
						g_PlayerMotionIdx.nMotionIdx = 3;
						g_PlayerMotionIdx.nKeySetIdx = 0;
						g_PlayerMotionIdx.nFrame = 0;
					}
				}
				else if (!g_Player.bIsJumping)
				{//プレイヤーがジャンプ中でない場合
					if (g_Player.move.x > 1.0f
						|| g_Player.move.x < -1.0f
						|| g_Player.move.z > 1.0f
						|| g_Player.move.z < -1.0f)
					{//プレイヤーが移動中
						if (g_Player.bIsDashing)
						{//プレイヤーがダッシュ中の場合
							if (g_PlayerMotionIdx.nMotionIdx != 2)
							{//ダッシュモーションの再生
								g_PlayerMotionIdx.nMotionIdx = 2;
								g_PlayerMotionIdx.nKeySetIdx = 0;
								g_PlayerMotionIdx.nFrame = 0;
							}
						}
						else if (!g_Player.bIsDashing)
						{//プレイヤーがダッシュしていない場合
							if (g_PlayerMotionIdx.nMotionIdx != 1)
							{//歩行モーションの再生
								g_PlayerMotionIdx.nMotionIdx = 1;
								g_PlayerMotionIdx.nKeySetIdx = 0;
								g_PlayerMotionIdx.nFrame = 0;
							}
						}
					}
					else
					{//止まっている場合
						if (g_PlayerMotionIdx.nMotionIdx != 0)
						{//ニュートラルモーションの再生
							g_PlayerMotionIdx.nMotionIdx = 0;
							g_PlayerMotionIdx.nKeySetIdx = 0;
							g_PlayerMotionIdx.nFrame = 0;
						}
					}
				}
			}
		}

		//角度の更新
		if (g_Player.rotDest.y - g_Player.rot.y > D3DX_PI)
		{
			g_Player.rotDest.y -= D3DX_PI * 2;
		}
		if (g_Player.rotDest.y - g_Player.rot.y < -D3DX_PI)
		{
			g_Player.rotDest.y += D3DX_PI * 2;
		}
		g_Player.rot.y += (g_Player.rotDest.y - g_Player.rot.y) * 0.1f;

		//角度の正規化
		if (g_Player.rot.y > D3DX_PI)
		{
			g_Player.rot.y -= D3DX_PI * 2;
		}
		if (g_Player.rot.y < -D3DX_PI)
		{
			g_Player.rot.y += D3DX_PI * 2;
		}

		//影の位置更新
		SetPositionShadow(g_Player.nIdxShadow, D3DXVECTOR3(g_Player.pos.x, 1.0f, g_Player.pos.z), g_Player.rot);

		//モーション再生
		MotionPlayer();
	}
}

//======================================================
//	プレイヤーの描画処理
//======================================================
void DrawPlayer(void)
{
	if (g_Player.bUse)
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		D3DXMATRIX mtxRotParent;		//親のワールドマトリックス
		D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

		//ワールドマトリックスの初期化（親）
		D3DXMatrixIdentity(&mtxRotParent);

		//プレイヤーの向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);

		D3DXMatrixMultiply(&mtxRotParent, &mtxRotParent, &mtxRot);

		//プレイヤーの位置を反映
		D3DXMatrixTranslation(&mtxTrans,
			g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);

		D3DXMatrixMultiply(&mtxRotParent, &mtxRotParent, &mtxTrans);

		switch (g_Player.Weapon)
		{
		case PLAYERWEAPON_GUN:			//プレイヤーが銃を所持している場合
			//剣の描画をやめ、銃を描画する
			g_Player.aParts[15].bUse = false;
			g_Player.aParts[16].bUse = true;
			break;
		case PLAYERWEAPON_SWORD:		//プレイヤーが剣を所持している場合
			//銃の描画をやめ、剣を描画する
			g_Player.aParts[15].bUse = true;
			g_Player.aParts[16].bUse = false;
			break;
		}

		for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
		{
			if (g_Player.aParts[nCnt].bUse)
			{
				D3DMATERIAL9 matDef;			//現在のマテリアル保存用
				D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

				//ワールドマトリックスの初期化（子）
				D3DXMatrixIdentity(&g_PlayerPartsmtxWorld[g_Player.aParts[nCnt].nIndex]);

				//向きを反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot,
					g_Player.aParts[nCnt].rot.y, g_Player.aParts[nCnt].rot.x, g_Player.aParts[nCnt].rot.z);

				D3DXMatrixMultiply(&g_PlayerPartsmtxWorld[g_Player.aParts[nCnt].nIndex], &g_PlayerPartsmtxWorld[g_Player.aParts[nCnt].nIndex], &mtxRot);

				//位置を反映
				D3DXMatrixTranslation(&mtxTrans,
					g_Player.aParts[nCnt].pos.x, g_Player.aParts[nCnt].pos.y, g_Player.aParts[nCnt].pos.z);

				D3DXMatrixMultiply(&g_PlayerPartsmtxWorld[g_Player.aParts[nCnt].nIndex], &g_PlayerPartsmtxWorld[g_Player.aParts[nCnt].nIndex], &mtxTrans);

				//自分の親モデルのマトリックスとの掛け算
				if (g_Player.aParts[nCnt].nParent < 0)
				{
					//モデルのマトリックス ＊ 親のワールドマトリックス
					D3DXMatrixMultiply(&g_PlayerPartsmtxWorld[g_Player.aParts[nCnt].nIndex], &g_PlayerPartsmtxWorld[g_Player.aParts[nCnt].nIndex], &mtxRotParent);
				}
				else
				{
					//モデルのマトリックス ＊ 親のワールドマトリックス
					D3DXMatrixMultiply(&g_PlayerPartsmtxWorld[g_Player.aParts[nCnt].nIndex], &g_PlayerPartsmtxWorld[g_Player.aParts[nCnt].nIndex], &g_PlayerPartsmtxWorld[g_Player.aParts[nCnt].nParent]);
				}

				if (g_aPartsInfo[g_Player.aParts[nCnt].nIndex].pMesh == NULL)
				{//パーツのモデルがNULLだったら
					continue;		//for文の最初に戻る
				}

				//ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_PlayerPartsmtxWorld[g_Player.aParts[nCnt].nIndex]);
				
				//現在のマテリアルを保持
				pDevice->GetMaterial(&matDef);

				//マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_aPartsInfo[g_Player.aParts[nCnt].nIndex].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aPartsInfo[g_Player.aParts[nCnt].nIndex].nNumMat; nCntMat++)
				{
					//マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
					 
					//モデルパーツの描画
					g_aPartsInfo[g_Player.aParts[nCnt].nIndex].pMesh->DrawSubset(nCntMat);
				}

				//保持していたマテリアルを戻す
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}

//-----------------------------------------------------------------------------
//モデルタイプ読み込み処理
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
//プレイヤーのセット処理
//-----------------------------------------------------------------------------
void SetPlayer(Player Player)
{
	g_Player = Player;
}

//-----------------------------------------------------------------------------
//モーション再生処理
//-----------------------------------------------------------------------------
void MotionPlayer(void)
{
	int nMotion = g_PlayerMotionIdx.nMotionIdx;
	int nKey = g_PlayerMotionIdx.nKeySetIdx;
	int nFrame = g_Player.aMotion[nMotion].nFrame[nKey];

	if (nFrame <= 0)
	{//テキストで設定されたフレーム数が0以下だった場合
		nFrame = 1;
	}

	//モーション再生処理
	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		if (g_Player.aParts[nCnt].bUse)
		{
			//位置更新（ローカル座標）
			g_Player.aParts[nCnt].pos += g_Player.aMotion[nMotion].aKey[nKey].pos[nCnt] / (float)g_Player.aMotion[nMotion].nFrame[nKey];

			//角度更新
			g_Player.aParts[nCnt].rot += (g_Player.aParts[nCnt].baseRot + g_Player.aMotion[nMotion].aKey[nKey].rot[nCnt] - g_Player.aParts[nCnt].rot) / (float)g_Player.aMotion[nMotion].nFrame[nKey];
		}
	}

	//フレームの加算
	g_PlayerMotionIdx.nFrame++;

	if (g_Player.aMotion[nMotion].nFrame[nKey] <= g_PlayerMotionIdx.nFrame)
	{//フレーム数が設定の値を超えたら

		//再生中のキー数の加算
		g_PlayerMotionIdx.nKeySetIdx++;

		//フレームの初期化
		g_PlayerMotionIdx.nFrame = 0;

		if (g_Player.aMotion[nMotion].nNumKey <= g_PlayerMotionIdx.nKeySetIdx)
		{//再生中のキー数が設定の値を超えたら
			if (g_Player.aMotion[nMotion].nLoop == 1)
			{
				g_PlayerMotionIdx.nKeySetIdx = 0;
			}
			else
			{//現在再生中のモーションからニュートラルモーションに変更
				g_PlayerMotionIdx.nKeySetIdx = 0;
				g_PlayerMotionIdx.nMotionIdx = 0;
			}
		}
	}
}

////======================================================
////	モデルの当たり判定処理
////======================================================
//bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Width, float Height)
//{
//	bool bIsMoving = false;		//動いているかどうか
//
//	//上からブロックに着地した場合
//	if (pPos->x + (Width / 2) > g_Player.pos.x - (PLAYER_WIDTH / 2)
//		&& pPos->x - (Width / 2) < g_Player.pos.x + (PLAYER_WIDTH / 2)
//		&& pPosOld->y - (Height / 2) <= g_Player.pos.y
//		&& pPos->y - (Height / 2) > g_Player.pos.y)
//	{//プレイヤーを下降させる
//		g_Player.pos.y = pPos->y - (Height / 2);
//	}
//
//	//上からブロックに着地した場合
//	if (pPos->x + (Width / 2) > g_Player.pos.x - (PLAYER_WIDTH / 2)
//		&& pPos->x - (Width / 2) < g_Player.pos.x + (PLAYER_WIDTH / 2)
//		&& pPos->y - (Height / 2) < g_Player.pos.y
//		&& pPosOld->y - (Height / 2) >= g_Player.pos.y)
//	{//プレイヤーを上昇させる
//		g_Player.pos.y = pPos->y - (Height / 2);
//	}
//
//	//右からブロックにぶつかった時場合
//	if (pPosOld->x + (Width / 2) <= g_Player.pos.x - (PLAYER_WIDTH / 2)
//		&& pPos->x + (Width / 2) > g_Player.pos.x - (PLAYER_WIDTH / 2)
//		&& pPos->y - (Height / 2) < g_Player.pos.y
//		&& pPos->y + (Height / 2) > g_Player.pos.y - PLAYER_HEIGHT)
//	{//ブロックの右に押し出す
//		g_Player.pos.x = pPos->x + (Width / 2) + (PLAYER_WIDTH / 2);
//	}
//
//	//左からブロックにぶつかった時場合
//	if (pPosOld->x - (Width / 2) >= g_Player.pos.x + (PLAYER_WIDTH / 2)
//		&& pPos->x - (Width / 2) < g_Player.pos.x + (PLAYER_WIDTH / 2)
//		&& pPos->y - (Height / 2) < g_Player.pos.y
//		&& pPos->y + (Height / 2) > g_Player.pos.y - PLAYER_HEIGHT)
//	{//ブロックの左に押し出す
//		g_Player.pos.x = pPos->x - (Width / 2) - (PLAYER_WIDTH / 2);
//	}
//	return bIsMoving;
//}

//======================================================
//	プレイヤーの情報を取得
//======================================================
Player *GetPlayer(void)
{
	return &g_Player;
}