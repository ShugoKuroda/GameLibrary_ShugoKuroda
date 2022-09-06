//===================================================================
//
//	敵処理[enemy.cpp]
//	Author:黒田 周吾
//
//===================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include "player.h"
#include "enemy.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "meshfield.h"
#include "bullet.h"
#include "model.h"
#include "sound.h"
#include "function.h"
#include "particle.h"
#include "life.h"
#include "score.h"
#include "game.h"

//======================================================
//	マクロ定義
//======================================================
#define ENEMY_WIDTH			(50.0f)		//敵の幅
#define ENEMY_HEIGHT		(2.0f)		//敵の高さ
#define ENEMY_MOVE			(0.7f)		//敵の移動量
#define ENEMY_MOVEDOWN		(0.3f)		//敵への重力
#define ENEMY_MOVEUP		(8.0f)		//敵のジャンプ力

//-----------------------------------------------------------------------------
//	構造体の定義
//-----------------------------------------------------------------------------
//モーション番号情報の構造体
typedef struct
{
	int nFrame;					//現在のフレーム数
	int nKeySetIdx;				//再生中のキーセット番号
	int nMotionIdx;				//再生中のモーション番号
}EnemyMotionIdx;

//モデル情報の構造体
typedef struct
{
	LPD3DXMESH pMesh = NULL;		//メッシュ情報へのポインタ
	LPD3DXBUFFER pBuffMat = NULL;	//マテリアル情報へのポインタ
	DWORD nNumMat = 0;				//マテリアル情報の数
	bool bUse;						//使用しているかどうか
}EnemyPartsInfo;

//======================================================
//	グローバル宣言
//======================================================
D3DXMATRIX g_EnemyPartsmtxWorld[MAX_ENEMY_PARTS];	//ワールドマトリックス(敵のパーツ)
EnemyPartsInfo g_aEnemyPartsInfo[MAX_ENEMY_PARTS];	//モデル(パーツ)の情報
EnemyMotionIdx g_EnemyMotionIdx;					//敵のモーション番号情報
Enemy g_aEnemy[MAX_ENEMY];							//敵の情報
EnemyInfo g_EnemyInfo;								//敵のモデル情報

//======================================================
//	プレイヤーの初期化処理
//======================================================
void InitEnemy(void)
{
	//プレイヤーのワールドマトリックスの初期化
	ZeroMemory(&g_EnemyPartsmtxWorld, sizeof(g_EnemyPartsmtxWorld));
	//プレイヤー構造体の初期化
	ZeroMemory(&g_aEnemyPartsInfo, sizeof(g_aEnemyPartsInfo));
	ZeroMemory(&g_aEnemy, sizeof(g_aEnemy));
	//プレイヤーのモーション番号情報の初期化
	ZeroMemory(&g_EnemyMotionIdx, sizeof(EnemyMotionIdx));

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		//影の設定
		g_aEnemy[nCnt].nIdxShadow = SetShadow(D3DXVECTOR3(g_aEnemy[nCnt].pos.x, 1.0f, g_aEnemy[nCnt].pos.z), g_aEnemy[nCnt].rot, 30.0f, 30.0f);
	}

	//敵の初期配置
	SetEnemy();
}

//======================================================
//	プレイヤーの終了処理
//======================================================
void UninitEnemy(void)
{
	for (int nCnt = 0; nCnt < MAX_ENEMY_PARTS; nCnt++)
	{
		//メッシュの破棄
		if (g_aEnemyPartsInfo[nCnt].pMesh != NULL)
		{
			g_aEnemyPartsInfo[nCnt].pMesh->Release();
			g_aEnemyPartsInfo[nCnt].pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_aEnemyPartsInfo[nCnt].pBuffMat != NULL)
		{
			g_aEnemyPartsInfo[nCnt].pBuffMat->Release();
			g_aEnemyPartsInfo[nCnt].pBuffMat = NULL;
		}
	}
}

//======================================================
//	プレイヤーの更新処理
//======================================================
void UpdateEnemy(void)
{
	//プレイヤー情報の取得
	Player *pPlayer = GetPlayer();

	//ロゴ出現情報の取得
	LogoCount *pLogoCount = GetLogoCount();

	g_EnemyInfo.nCntSpawn++;		//敵出現カウンターの加算
	if (g_EnemyInfo.nCntSpawn >= 600)
	{//カウンターが一定数(10秒)
		SetEnemy();			//敵が出現
		g_EnemyInfo.nCntSpawn = 0;			//カウンターリセット
	}

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse)
		{//敵が使用されている

			//前回の位置を保存
			g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;

			if (pLogoCount->bNot)
			{
				if (g_aEnemy[nCntEnemy].nLife > 0)
				{//敵の体力がある場合
					g_aEnemy[nCntEnemy].nCntAttack++;		//攻撃カウンターの加算

					if (g_aEnemy[nCntEnemy].bIsDamage)
					{//敵がダメージ状態の場合
						g_aEnemy[nCntEnemy].nCntDamage++;			//ダメージ状態切り替えカウンターの加算

						g_aEnemy[nCntEnemy].move.x += g_aEnemy[nCntEnemy].moveKnockBack.x * ENEMY_MOVE;
						g_aEnemy[nCntEnemy].move.z += g_aEnemy[nCntEnemy].moveKnockBack.z * ENEMY_MOVE;

						if (g_EnemyMotionIdx.nMotionIdx != 2)
						{//吹っ飛びモーションの再生
							g_EnemyMotionIdx.nMotionIdx = 2;
							g_EnemyMotionIdx.nKeySetIdx = 0;
							g_EnemyMotionIdx.nFrame = 0;
						}
						if (g_aEnemy[nCntEnemy].nCntDamage == 1)
						{
							g_aEnemy[nCntEnemy].move.y = (g_EnemyInfo.fJump / 3);
						}
						if (g_aEnemy[nCntEnemy].nCntDamage >= 30)
						{//カウンターが一定以上	
							g_aEnemy[nCntEnemy].bIsDamage = false;		//プレイヤーのダメージ状態を解く
							g_aEnemy[nCntEnemy].nCntDamage = 0;			//ダメージカウンターのリセット
						}
					}

					//敵の攻撃判定
					else if (SphereCollision(pPlayer->pos, g_aEnemy[nCntEnemy].pos, ENEMY_COLLISION - 20.0f, PLAYER_COLLISION))
					{//プレイヤーに近づいたら
						if (g_aEnemy[nCntEnemy].nCntAttack >= 180 && !g_aEnemy[nCntEnemy].bIsAttacking)
						{//一定間隔で攻撃
							g_aEnemy[nCntEnemy].bIsAttacking = true;		//攻撃フラグを立てる

							g_aEnemy[nCntEnemy].move.y = g_EnemyInfo.fJump;			//ジャンプ
							g_aEnemy[nCntEnemy].bIsJumping = true;		//ジャンプ判定を有効にする

							if (g_EnemyMotionIdx.nMotionIdx != 2)
							{//攻撃モーションの再生
								g_EnemyMotionIdx.nMotionIdx = 2;
								g_EnemyMotionIdx.nKeySetIdx = 0;
								g_EnemyMotionIdx.nFrame = 0;
							}
						}
						else
						{
							g_aEnemy[nCntEnemy].move.x = 0.0f;
							g_aEnemy[nCntEnemy].move.z = 0.0f;
						}
						if (g_aEnemy[nCntEnemy].nCntAttack >= 185 && g_aEnemy[nCntEnemy].bIsAttacking)
						{//攻撃モーションを終えたら
							if (!pPlayer->bIsDamage)
							{//プレイヤーがダメージ状態ではない時
								//敵の攻撃判定
								if (SphereCollision(pPlayer->pos, g_aEnemy[nCntEnemy].pos, ENEMY_COLLISION - 20.0f, PLAYER_COLLISION))
								{//プレイヤーが攻撃範囲内だったら
									pPlayer->bIsDamage = true;

									//プレイヤーのライフ減少
									AddLife(-20);

									//プレイヤーと敵の距離差分を保存(目的の位置 - 現在の位置)
									D3DXVECTOR3 vecToEnemy = pPlayer->pos - g_aEnemy[nCntEnemy].pos;

									//敵からプレイヤーへのベクトル(移動量)に変換する
									D3DXVec3Normalize(&vecToEnemy, &vecToEnemy);

									//移動量の更新
									pPlayer->moveKnockBack.x = vecToEnemy.x * (pPlayer->fMove);
									pPlayer->moveKnockBack.z = vecToEnemy.z * (pPlayer->fMove);

									//プレイヤーの向き更新
									pPlayer->rotDest.y = atan2f(vecToEnemy.x, vecToEnemy.z);

									//サウンド再生
									PlaySound(SOUND_LABEL_SE_DAMAGE);

									//パーティクル処理
									for (int nCnt = 0; nCnt < 20; nCnt++)
									{
										SetParticle(D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z),		//位置
											D3DXVECTOR3(sinf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f,		//移動量
												tanf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f,
												cosf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f),
											D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f),		//色
											D3DXVECTOR3(3.0f, 3.0f, 3.0f),			//大きさ
											0);			//種類
									}
								}
							}
						}
						if (g_aEnemy[nCntEnemy].nCntAttack >= 210 && g_aEnemy[nCntEnemy].bIsAttacking)
						{//攻撃モーションを終えたら
							g_aEnemy[nCntEnemy].nCntAttack = 0;			//攻撃カウンターをリセット
							g_aEnemy[nCntEnemy].bIsAttacking = false;	//攻撃フラグを無効化
						}
					}
					else
					{//プレイヤーが近くにいなかったら
						//プレイヤーを追従する処理
						HomingEnemy(nCntEnemy);
						g_aEnemy[nCntEnemy].bIsAttacking = false;		//攻撃フラグの無効化
					}

					g_aEnemy[nCntEnemy].nCntPlayerAttack++;			//プレイヤーの多段剣攻撃防止カウンターの加算
					if (g_aEnemy[nCntEnemy].nCntPlayerAttack >= 20)
					{//カウンターが一定数
						g_aEnemy[nCntEnemy].bIsSwordHit = false;		//敵の剣攻撃Hit状態を解除
						g_aEnemy[nCntEnemy].nCntPlayerAttack = 0;		//カウンターリセット
					}
				}
			}

			g_aEnemy[nCntEnemy].move.y -= ENEMY_MOVEDOWN;		//重力負荷をかける
			g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;		//位置更新

			//移動量の減衰(目的の値 - 現在の値) * 減衰係数
			g_aEnemy[nCntEnemy].move.x += -g_aEnemy[nCntEnemy].move.x * 0.2f;
			g_aEnemy[nCntEnemy].move.z += -g_aEnemy[nCntEnemy].move.z * 0.2f;

			if (g_aEnemy[nCntEnemy].nLife <= 0)
			{//敵の体力が0以下
				g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量のリセット
				g_aEnemy[nCntEnemy].nCntDie++;		//死亡カウンターの加算
				if (g_EnemyMotionIdx.nMotionIdx != 3)
				{//やられモーションの再生
					g_EnemyMotionIdx.nMotionIdx = 3;
					g_EnemyMotionIdx.nKeySetIdx = 0;
					g_EnemyMotionIdx.nFrame = 0;
				}
				if (g_aEnemy[nCntEnemy].nCntDie >= 60)
				{//死亡カウンターが一定数以上
					//パーティクル処理
					for (int nCnt = 0; nCnt < 20; nCnt++)
					{
						SetParticle(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z),		//位置
							D3DXVECTOR3(sinf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f,		//移動量
								tanf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f,
								cosf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f),
							D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f),		//色
							D3DXVECTOR3(3.0f, 3.0f, 3.0f),			//大きさ
							0);				//大きさ
					}
					AddScore(3);			//スコア加算
					g_aEnemy[nCntEnemy].bUse = false;		//敵を消す
					g_aEnemy[nCntEnemy].nCntDie = 0;			//カウンターリセット

					//サウンド再生
					PlaySound(SOUND_LABEL_SE_EXPLOSION);
				}
			}
			else if (!g_aEnemy[nCntEnemy].bIsDamage)
			{//敵がダメージ状態でない場合
				if (!g_aEnemy[nCntEnemy].bIsAttacking)
				{//敵が攻撃中でない場合
					if (g_aEnemy[nCntEnemy].move.x > 0.1f
						|| g_aEnemy[nCntEnemy].move.x < -0.1f
						|| g_aEnemy[nCntEnemy].move.z > 0.1f
						|| g_aEnemy[nCntEnemy].move.z < -0.1f)
					{//敵が移動中
						if (g_EnemyMotionIdx.nMotionIdx != 1)
						{//移動モーションの再生
							g_EnemyMotionIdx.nMotionIdx = 1;
							g_EnemyMotionIdx.nKeySetIdx = 0;
							g_EnemyMotionIdx.nFrame = 0;
						}
					}
					else
					{//止まっている場合
						if (g_EnemyMotionIdx.nMotionIdx != 0)
						{//ニュートラルモーションの再生
							g_EnemyMotionIdx.nMotionIdx = 0;
							g_EnemyMotionIdx.nKeySetIdx = 0;
							g_EnemyMotionIdx.nFrame = 0;
						}
					}
				}
			}

			//角度の更新
			if (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
			{
				g_aEnemy[nCntEnemy].rotDest.y -= D3DX_PI * 2;
			}
			if (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
			{
				g_aEnemy[nCntEnemy].rotDest.y += D3DX_PI * 2;
			}
			g_aEnemy[nCntEnemy].rot.y += (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y) * 0.1f;

			//角度の正規化
			if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
			{
				g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
			}
			if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
			{
				g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
			}

			//ブロックの当たり判定
			g_aEnemy[nCntEnemy].bIsJumping = !CollisionModel(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move, ENEMY_WIDTH, ENEMY_HEIGHT, 1);

			//壁・床の当たり判定処理
			if (g_aEnemy[nCntEnemy].pos.x - (ENEMY_WIDTH / 2) <= -2000.0f / 2)
			{//左壁
				g_aEnemy[nCntEnemy].pos.x = (-2000.0f / 2) + (ENEMY_WIDTH / 2);
			}
			else if (g_aEnemy[nCntEnemy].pos.x + (ENEMY_WIDTH / 2) >= 2000.0f / 2)
			{//右壁
				g_aEnemy[nCntEnemy].pos.x = (2000.0f / 2) - (ENEMY_WIDTH / 2);
			}
			if (g_aEnemy[nCntEnemy].pos.z + (ENEMY_WIDTH / 2) >= 2000.0f / 2)
			{//奥壁
				g_aEnemy[nCntEnemy].pos.z = (2000.0f / 2) - (ENEMY_WIDTH / 2);
			}
			else if (g_aEnemy[nCntEnemy].pos.z - (ENEMY_WIDTH / 2) <= -2000.0f / 2)
			{//手前壁
				g_aEnemy[nCntEnemy].pos.z = (-2000.0f / 2) + (ENEMY_WIDTH / 2);
			}
			if (g_aEnemy[nCntEnemy].pos.y - (ENEMY_HEIGHT / 2) <= 0.0f)
			{//床
				g_aEnemy[nCntEnemy].pos.y = (ENEMY_HEIGHT / 2);
				g_aEnemy[nCntEnemy].move.y = 0.0f;		//移動量Yの初期化
				g_aEnemy[nCntEnemy].bIsJumping = false;
			}

			//影の位置更新
			SetPositionShadow(g_aEnemy[nCntEnemy].nIdxShadow, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, 1.0f, g_aEnemy[nCntEnemy].pos.z), g_aEnemy[nCntEnemy].rot);

			//モーション再生
			MotionEnemy();
		}
	}
}

//======================================================
//	プレイヤーの描画処理
//======================================================
void DrawEnemy(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse)
		{
			D3DXMATRIX mtxRotParent;		//親のワールドマトリックス
			D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

			//ワールドマトリックスの初期化（親）
			D3DXMatrixIdentity(&mtxRotParent);

			//プレイヤーの向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);

			D3DXMatrixMultiply(&mtxRotParent, &mtxRotParent, &mtxRot);

			//プレイヤーの位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);

			D3DXMatrixMultiply(&mtxRotParent, &mtxRotParent, &mtxTrans);

			for (int nCnt = 0; nCnt < MAX_ENEMY_PARTS; nCnt++)
			{
				if (g_EnemyInfo.aParts[nCnt].bUse)
				{
					D3DMATERIAL9 matDef;			//現在のマテリアル保存用
					D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

					//ワールドマトリックスの初期化（子）
					D3DXMatrixIdentity(&g_EnemyPartsmtxWorld[g_EnemyInfo.aParts[nCnt].nIndex]);

					//向きを反映
					D3DXMatrixRotationYawPitchRoll(&mtxRot,
						g_EnemyInfo.aParts[nCnt].rot.y, g_EnemyInfo.aParts[nCnt].rot.x, g_EnemyInfo.aParts[nCnt].rot.z);

					D3DXMatrixMultiply(&g_EnemyPartsmtxWorld[g_EnemyInfo.aParts[nCnt].nIndex], &g_EnemyPartsmtxWorld[g_EnemyInfo.aParts[nCnt].nIndex], &mtxRot);

					//位置を反映
					D3DXMatrixTranslation(&mtxTrans,
						g_EnemyInfo.aParts[nCnt].pos.x, g_EnemyInfo.aParts[nCnt].pos.y, g_EnemyInfo.aParts[nCnt].pos.z);

					D3DXMatrixMultiply(&g_EnemyPartsmtxWorld[g_EnemyInfo.aParts[nCnt].nIndex], &g_EnemyPartsmtxWorld[g_EnemyInfo.aParts[nCnt].nIndex], &mtxTrans);

					//自分の親モデルのマトリックスとの掛け算
					if (g_EnemyInfo.aParts[nCnt].nParent < 0)
					{
						//モデルのマトリックス ＊ 親のワールドマトリックス
						D3DXMatrixMultiply(&g_EnemyPartsmtxWorld[g_EnemyInfo.aParts[nCnt].nIndex], &g_EnemyPartsmtxWorld[g_EnemyInfo.aParts[nCnt].nIndex], &mtxRotParent);
					}
					else
					{
						//モデルのマトリックス ＊ 親のワールドマトリックス
						D3DXMatrixMultiply(&g_EnemyPartsmtxWorld[g_EnemyInfo.aParts[nCnt].nIndex], &g_EnemyPartsmtxWorld[g_EnemyInfo.aParts[nCnt].nIndex], &g_EnemyPartsmtxWorld[g_EnemyInfo.aParts[nCnt].nParent]);
					}

					//ワールドマトリックスの設定
					pDevice->SetTransform(D3DTS_WORLD, &g_EnemyPartsmtxWorld[g_EnemyInfo.aParts[nCnt].nIndex]);

					//現在のマテリアルを保持
					pDevice->GetMaterial(&matDef);

					//マテリアルデータへのポインタを取得
					pMat = (D3DXMATERIAL*)g_aEnemyPartsInfo[g_EnemyInfo.aParts[nCnt].nIndex].pBuffMat->GetBufferPointer();

					for (int nCntMat = 0; nCntMat < (int)g_aEnemyPartsInfo[g_EnemyInfo.aParts[nCnt].nIndex].nNumMat; nCntMat++)
					{
						//マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

						//モデルパーツの描画
						g_aEnemyPartsInfo[g_EnemyInfo.aParts[nCnt].nIndex].pMesh->DrawSubset(nCntMat);
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
HRESULT LoadEnemyType(HWND hWnd, char *sXFilePath)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntPatn = 0; nCntPatn < MAX_ENEMY_PARTS; nCntPatn++)
	{
		if (!g_aEnemyPartsInfo[nCntPatn].bUse)
		{
			//Xファイルの読み込み
			D3DXLoadMeshFromX(sXFilePath,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_aEnemyPartsInfo[nCntPatn].pBuffMat,
				NULL,
				&g_aEnemyPartsInfo[nCntPatn].nNumMat,
				&g_aEnemyPartsInfo[nCntPatn].pMesh);

			g_aEnemyPartsInfo[nCntPatn].bUse = true;

			return S_OK;
		}
	}

	MessageBox(hWnd, "読み込み可能モデル数を超えています", "警告！", MB_ICONWARNING);
	return E_FAIL;
}

//-----------------------------------------------------------------------------
//	敵の設定処理
//-----------------------------------------------------------------------------
void SetEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (!g_aEnemy[nCntEnemy].bUse)
		{//使用されていない
			g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の初期化
			g_aEnemy[nCntEnemy].bUse = true;		//出現させる
			g_aEnemy[nCntEnemy].nLife = 100;		//体力の設定
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//	モーション再生処理
//-----------------------------------------------------------------------------
void MotionEnemy(void)
{
	int nMotion = g_EnemyMotionIdx.nMotionIdx;
	int nKey = g_EnemyMotionIdx.nKeySetIdx;
	int nFrame = g_EnemyInfo.aMotion[nMotion].nFrame[nMotion];

	if (nFrame <= 0)
	{//テキストで設定されたフレーム数が0以下だった場合
		nFrame = 1;
	}

	//モーション再生処理
	for (int nCnt = 0; nCnt < MAX_ENEMY_PARTS; nCnt++)
	{
		if (g_EnemyInfo.aParts[nCnt].bUse)
		{
			//位置更新（ローカル座標）
			g_EnemyInfo.aParts[nCnt].pos += g_EnemyInfo.aMotion[nMotion].aKey[nKey].pos[nCnt] / (float)g_EnemyInfo.aMotion[nMotion].nFrame[nKey];

			//角度更新
			g_EnemyInfo.aParts[nCnt].rot += (g_EnemyInfo.aMotion[nMotion].aKey[nKey].rot[nCnt] - g_EnemyInfo.aParts[nCnt].rot) / (float)g_EnemyInfo.aMotion[nMotion].nFrame[nKey];
		}
	}

	//フレームの加算
	g_EnemyMotionIdx.nFrame++;

	if (g_EnemyInfo.aMotion[nMotion].nFrame[nMotion] <= g_EnemyMotionIdx.nFrame)
	{//フレーム数が設定の値を超えたら

		//再生中のキー数の加算
		g_EnemyMotionIdx.nKeySetIdx++;

		//フレームの初期化
		g_EnemyMotionIdx.nFrame = 0;

		if (g_EnemyInfo.aMotion[nMotion].nNumKey <= g_EnemyMotionIdx.nKeySetIdx)
		{//再生中のキー数が設定の値を超えたら
			if (g_EnemyInfo.aMotion[nMotion].nLoop == 1)
			{
				g_EnemyMotionIdx.nKeySetIdx = 0;
			}
			else
			{//現在再生中のモーションからニュートラルモーションに変更
				g_EnemyMotionIdx.nKeySetIdx = 0;
				g_EnemyMotionIdx.nMotionIdx = 0;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	敵の追従処理
//-----------------------------------------------------------------------------
void HomingEnemy(int nCnt)
{
	//プレイヤー情報の取得
	Player *pPlayer = GetPlayer();

	//プレイヤーと敵の距離差分を保存(目的の位置 - 現在の位置)
	D3DXVECTOR3 vecToPlayer = pPlayer->pos - g_aEnemy[nCnt].pos;

	//敵からプレイヤーへのベクトル(移動量)に変換する
	D3DXVec3Normalize(&vecToPlayer, &vecToPlayer);

	//移動量の更新
	g_aEnemy[nCnt].move.x = vecToPlayer.x * g_EnemyInfo.fMove;
	g_aEnemy[nCnt].move.z = vecToPlayer.z * g_EnemyInfo.fMove;

	//モデルの向き更新
	g_aEnemy[nCnt].rotDest.y = atan2f(vecToPlayer.x, vecToPlayer.z) + D3DX_PI;
}

//======================================================
//	敵の情報を取得
//======================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//======================================================
//	敵の情報を取得
//======================================================
EnemyInfo *GetEnemyInfo(void)
{
	return &g_EnemyInfo;
}