//=============================================================================
//
// ロード処理 [load.cpp]
// Author : SHUGO KURODA
//
//=============================================================================

//======================================================
// インクルード
//======================================================
#include "main.h"
#include <stdio.h>
#include "player.h"
#include "model.h"
#include "load.h"
#include "shadow.h"
#include "enemy.h"

//======================================================
// マクロ定義
//======================================================
#define LINE_MAX_READING_LENGTH (256)			//一行の最大読み取り文字数

//-----------------------------------------------------------------------------
//モデルのセットロード
//-----------------------------------------------------------------------------
HRESULT LoadSetModel(HWND hWnd)
{
	FILE *pFile = NULL;			//ファイルポインター宣言

	char cBff[LINE_MAX_READING_LENGTH];			//一行分読み取る変数
	char cBffHead[LINE_MAX_READING_LENGTH];		//頭の文字を読み取る変数
	
	//ファイルを開く
	pFile = fopen("model.txt", "r");

	if (pFile == NULL)
	{//ファイルを開けなかった場合
		MessageBox(hWnd, "モデルテキストを開けませんでした", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}
	
	//文字列の読み取りループ処理
	while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
	{
		//文字列の分析
		sscanf(cBff, "%s", &cBffHead);

		if (strcmp(&cBffHead[0], "MODEL_FILENAME") == 0)
		{//Xファイルの相対パス用

			char sPath[LINE_MAX_READING_LENGTH];		//相対パス保存用

			//一行の文字列から相対パスの読み取り
			sscanf(cBff, "%s = %s", &cBffHead, &sPath[0]);

			//Xファイルの読み込み
			LoadModelType(hWnd, &sPath[0]);
		}
		else if (strcmp(&cBffHead[0], "MODELSET") == 0)
		{//モデルの配置用
			Model Model;				//モデル構造体情報の格納変数
			float fShadowX, fShadowZ;	//影の大きさ情報の格納変数
			int nCheck;					//影を使用するかどうか

			//モデル配置に必要な情報読み取りループ処理
			while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
			{
				//文字列の分析
				sscanf(cBff, "%s", &cBffHead);

				if (strcmp(&cBffHead[0], "TYPE") == 0)
				{//モデルの種類用
					sscanf(cBff, "%s = %d", &cBffHead, &Model.nType);
				}
				else if (strcmp(&cBffHead[0], "POS") == 0)
				{//POS用
					sscanf(cBff, "%s = %f%f%f", &cBffHead, &Model.pos.x, &Model.pos.y, &Model.pos.z);
				}
				else if (strcmp(&cBffHead[0], "ROT") == 0)
				{//ROT用
					sscanf(cBff, "%s = %f%f%f", &cBffHead, &Model.rot.x, &Model.rot.y, &Model.rot.z);
				}
				else if (strcmp(&cBffHead[0], "SHADOW") == 0)
				{//影のセット
					sscanf(cBff, "%s = %d%f%f", &cBffHead, &nCheck, &fShadowX, &fShadowZ);
				}
				else if (strcmp(&cBffHead[0], "END_MODELSET") == 0)
				{//モデルのセットに必要な情報を読み切った時
					break;
				}
			}

			if (nCheck >= 1)
			{//nCheckが1以上の場合、影を設定する
				Model.nIdxShadow = SetShadow(D3DXVECTOR3(Model.pos.x, 0.1f, Model.pos.z), Model.rot, fShadowX, fShadowZ);
			}

			//モデル配置
			SetModel(hWnd,Model);
		}
		else if (strcmp(&cBffHead[0], "TEXT_END") == 0)
		{//テキストファイルを読み切った時
			break;
		}
	}

	//ファイルを閉じる
	fclose(pFile);

	return S_OK;
}

//-----------------------------------------------------------------------------
//プレイヤーのセットロード
//-----------------------------------------------------------------------------
HRESULT LoadSetPlayer(HWND hWnd)
{
	FILE *pFile = NULL;			//ファイルポインター宣言

	char cBff[LINE_MAX_READING_LENGTH];		//一行分読み取るための変数
	char cBffHead[LINE_MAX_READING_LENGTH];	//頭の文字を読み取るための変数
	Player *pPlayer = GetPlayer();			//プレイヤー情報の取得
	int nMotionIdx = 0;			//モーション番号

	//ファイルを開く
	pFile = fopen("motion.txt", "r");

	if (pFile == NULL)
	{//開けなかった時用
		MessageBox(hWnd, "モーションテキストを開けませんでした", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	//文字列の読み取りループ処理
	while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
	{
		//文字読み込み用変数の初期化
		memset(&cBffHead, 0, sizeof(cBffHead));
		//文字列の分析
		sscanf(cBff, "%s", &cBffHead);

		if (strcmp(&cBffHead[0], "MODEL_FILENAME") == 0)
		{//Xファイルの相対パス用

			char sPath[LINE_MAX_READING_LENGTH];		//相対パス保存用

			//一行の文字列から相対パスの読み取り
			sscanf(cBff, "%s = %s", &cBffHead, &sPath[0]);

			//Xファイルの読み込み
			LoadPlayerType(hWnd, &sPath[0]);
		}
		else if (strcmp(&cBffHead[0], "CHARACTERSET") == 0)
		{//プレイヤーの配置用

			//プレイヤー情報の読み取りループ処理
			while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
			{
				//文字列の分析
				sscanf(cBff, "%s", &cBffHead);

				if (strcmp(&cBffHead[0], "NUM_PARTS") == 0)
				{//パーツ数
					//文字列の分析
					sscanf(cBff, "%s = %d", &cBffHead,&pPlayer->nNumParts);
				}
				else if (strcmp(&cBffHead[0], "MOVE") == 0)
				{//移動量
					//文字列の分析
					sscanf(cBff, "%s = %f", &cBffHead, &pPlayer->fMove);
				}
				else if (strcmp(&cBffHead[0], "JUMP") == 0)
				{//ジャンプ量
					//文字列の分析
					sscanf(cBff, "%s = %f", &cBffHead, &pPlayer->fJump);
				}
				else if (strcmp(&cBffHead[0], "PARTSSET") == 0)
				{
					//インデックス保存用
					int nIndex = 0;

					//プレイヤーパーツ情報の読み取りループ処理
					while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
					{
						//文字列の分析
						sscanf(cBff, "%s", &cBffHead);

						if (strcmp(&cBffHead[0], "INDEX") == 0)
						{//パーツ番号
							//文字列の分析
							sscanf(cBff, "%s = %d", &cBffHead, &nIndex);
							pPlayer->aParts[nIndex].nIndex = nIndex;
							pPlayer->aParts[nIndex].bUse = true;
						}
						else if (strcmp(&cBffHead[0], "PARENT") == 0)
						{//現在のパーツの親
							//文字列の分析
							sscanf(cBff, "%s = %d", &cBffHead, &pPlayer->aParts[nIndex].nParent);
						}
						else if (strcmp(&cBffHead[0], "POS") == 0)
						{//位置
							//文字列の分析
							sscanf(cBff, "%s = %f%f%f", &cBffHead, &pPlayer->aParts[nIndex].pos.x, &pPlayer->aParts[nIndex].pos.y, &pPlayer->aParts[nIndex].pos.z);
						}
						else if (strcmp(&cBffHead[0], "ROT") == 0)
						{//回転(角度)
							//文字列の分析
							sscanf(cBff, "%s = %f%f%f", &cBffHead, &pPlayer->aParts[nIndex].rot.x, &pPlayer->aParts[nIndex].rot.y, &pPlayer->aParts[nIndex].rot.z);
							pPlayer->aParts[nIndex].baseRot = pPlayer->aParts[nIndex].rot;
						}
						else if (strcmp(&cBffHead[0], "END_PARTSSET") == 0)
						{//パーツ読み込み終了
							break;
						}
					}
				}
				else if (strcmp(&cBffHead[0], "END_CHARACTERSET") == 0)
				{//プレイヤー読み込み終了
					break;
				}
			}
		}
		else if (strcmp(&cBffHead[0], "MOTIONSET") == 0)
		{//モーション設定用

			//キーセット番号の保存用変数
			int nKeySetIdx = 0;
			 //モーション情報の読み取りループ処理
			while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
			{
				//文字列の分析
				sscanf(cBff, "%s", &cBffHead);

				if (strcmp(&cBffHead[0], "LOOP") == 0)
				{//ループ設定
					//文字列の分析
					sscanf(cBff, "%s = %d", &cBffHead, &pPlayer->aMotion[nMotionIdx].nLoop);
				}
				else if (strcmp(&cBffHead[0], "NUM_KEY") == 0)
				{//キーの数
					//文字列の分析
					sscanf(cBff, "%s = %d", &cBffHead, &pPlayer->aMotion[nMotionIdx].nNumKey);
				}
				else if (strcmp(&cBffHead[0], "KEYSET") == 0)
				{
					//モデルパーツのセット番号
					int nPartsIdx = 0;

					//キーセット情報の読み取りループ処理
					while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
					{
						//文字列の分析
						sscanf(cBff, "%s", &cBffHead);

						if (strcmp(&cBffHead[0], "FRAME") == 0)
						{//ループ設定
							//文字列の分析
							sscanf(cBff, "%s = %d", &cBffHead, &pPlayer->aMotion[nMotionIdx].nFrame[nKeySetIdx]);
							if (pPlayer->aMotion[nMotionIdx].nFrame[nKeySetIdx] == 0)
							{//再生フレーム数が0の場合1にする
								pPlayer->aMotion[nMotionIdx].nFrame[nKeySetIdx] = 1;
							}
						}
						if (strcmp(&cBffHead[0], "KEY") == 0)
						{//ループ設定
							//キー情報の読み取りループ処理
							while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
							{
								//文字列の分析
								sscanf(cBff, "%s", &cBffHead);

								if (strcmp(&cBffHead[0], "POS") == 0)
								{//モーション再生中の位置情報読み込み
									//文字列の分析
									sscanf(cBff, "%s = %f%f%f", &cBffHead,
										&pPlayer->aMotion[nMotionIdx].aKey[nKeySetIdx].pos[nPartsIdx].x,
										&pPlayer->aMotion[nMotionIdx].aKey[nKeySetIdx].pos[nPartsIdx].y,
										&pPlayer->aMotion[nMotionIdx].aKey[nKeySetIdx].pos[nPartsIdx].z);
								}
								else if (strcmp(&cBffHead[0], "ROT") == 0)
								{//モーション再生中の回転情報読み込み
									//文字列の分析
									sscanf(cBff, "%s = %f%f%f", &cBffHead,
										&pPlayer->aMotion[nMotionIdx].aKey[nKeySetIdx].rot[nPartsIdx].x,
										&pPlayer->aMotion[nMotionIdx].aKey[nKeySetIdx].rot[nPartsIdx].y,
										&pPlayer->aMotion[nMotionIdx].aKey[nKeySetIdx].rot[nPartsIdx].z);
								}
								else if (strcmp(&cBffHead[0], "END_KEY") == 0)
								{
									nPartsIdx++;
									break;
								}
							}
						}
						else if (strcmp(&cBffHead[0], "END_KEYSET") == 0)
						{
							nKeySetIdx++;
							break;
						}
					}
				}
				else if (strcmp(&cBffHead[0], "END_MOTIONSET") == 0)
				{
					nMotionIdx++;
					break;
				}
			}
		}
		else if (strcmp(&cBffHead[0], "END_SCRIPT") == 0)
		{//テキストファイルを読み切った時
			break;
		}
	}

	//プレイヤーを使用している状態にする
	pPlayer->bUse = true;

	//ファイルを閉じる
	fclose(pFile);

	return S_OK;
}

//-----------------------------------------------------------------------------
//プレイヤーのセットロード
//-----------------------------------------------------------------------------
HRESULT LoadSetEnemy(HWND hWnd)
{
	FILE *pFile = NULL;			//ファイルポインター宣言

	char cBff[LINE_MAX_READING_LENGTH];			//一行分読み取るための変数
	char cBffHead[LINE_MAX_READING_LENGTH];		//頭の文字を読み取るための変数
	EnemyInfo *pEnemy = GetEnemyInfo();			//敵情報の取得
	int nMotionIdx = 0;							//モーション番号

	//ファイルを開く
	pFile = fopen("motion_wolf.txt", "r");

	if (pFile == NULL)
	{//開けなかった時用
		MessageBox(hWnd, "モーションテキストを開けませんでした", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	//文字列の読み取りループ処理
	while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
	{
		//文字列の分析
		sscanf(cBff, "%s", &cBffHead);

		if (strcmp(&cBffHead[0], "MODEL_FILENAME") == 0)
		{//Xファイルの相対パス用

			char sPath[LINE_MAX_READING_LENGTH];		//相対パス保存用

			//一行の文字列から相対パスの読み取り
			sscanf(cBff, "%s = %s", &cBffHead, &sPath[0]);

			//Xファイルの読み込み
			LoadEnemyType(hWnd, &sPath[0]);
		}
		else if (strcmp(&cBffHead[0], "CHARACTERSET") == 0)
		{//プレイヤーの配置用

			//プレイヤー情報の読み取りループ処理
			while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
			{
				//文字列の分析
				sscanf(cBff, "%s", &cBffHead);

				if (strcmp(&cBffHead[0], "NUM_PARTS") == 0)
				{//パーツ数
				 //文字列の分析
					sscanf(cBff, "%s = %d", &cBffHead, &pEnemy->nNumParts);
				}
				else if (strcmp(&cBffHead[0], "MOVE") == 0)
				{//移動量
				 //文字列の分析
					sscanf(cBff, "%s = %f", &cBffHead, &pEnemy->fMove);
				}
				else if (strcmp(&cBffHead[0], "JUMP") == 0)
				{//ジャンプ量
				 //文字列の分析
					sscanf(cBff, "%s = %f", &cBffHead, &pEnemy->fJump);
				}
				else if (strcmp(&cBffHead[0], "PARTSSET") == 0)
				{
					//インデックス保存用
					int nIndex = 0;

					//プレイヤーパーツ情報の読み取りループ処理
					while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
					{
						//文字列の分析
						sscanf(cBff, "%s", &cBffHead);

						if (strcmp(&cBffHead[0], "INDEX") == 0)
						{//パーツ番号
						 //文字列の分析
							sscanf(cBff, "%s = %d", &cBffHead, &nIndex);
							pEnemy->aParts[nIndex].nIndex = nIndex;
							pEnemy->aParts[nIndex].bUse = true;
						}
						else if (strcmp(&cBffHead[0], "PARENT") == 0)
						{//現在のパーツの親
						 //文字列の分析
							sscanf(cBff, "%s = %d", &cBffHead, &pEnemy->aParts[nIndex].nParent);
						}
						else if (strcmp(&cBffHead[0], "POS") == 0)
						{//位置
						 //文字列の分析
							sscanf(cBff, "%s = %f%f%f", &cBffHead, &pEnemy->aParts[nIndex].pos.x, &pEnemy->aParts[nIndex].pos.y, &pEnemy->aParts[nIndex].pos.z);
						}
						else if (strcmp(&cBffHead[0], "ROT") == 0)
						{//回転(角度)
						 //文字列の分析
							sscanf(cBff, "%s = %f%f%f", &cBffHead, &pEnemy->aParts[nIndex].rot.x, &pEnemy->aParts[nIndex].rot.y, &pEnemy->aParts[nIndex].rot.z);
						}
						else if (strcmp(&cBffHead[0], "END_PARTSSET") == 0)
						{//パーツ読み込み終了
							break;
						}
					}
				}
				else if (strcmp(&cBffHead[0], "END_CHARACTERSET") == 0)
				{//プレイヤー読み込み終了
					break;
				}
			}
		}
		else if (strcmp(&cBffHead[0], "MOTIONSET") == 0)
		{//モーション設定用

		 //キーセット番号の保存用変数
			int nKeySetIdx = 0;
			//モーション情報の読み取りループ処理
			while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
			{
				//文字列の分析
				sscanf(cBff, "%s", &cBffHead);

				if (strcmp(&cBffHead[0], "LOOP") == 0)
				{//ループ設定
				 //文字列の分析
					sscanf(cBff, "%s = %d", &cBffHead, &pEnemy->aMotion[nMotionIdx].nLoop);
				}
				else if (strcmp(&cBffHead[0], "NUM_KEY") == 0)
				{//キーの数
				 //文字列の分析
					sscanf(cBff, "%s = %d", &cBffHead, &pEnemy->aMotion[nMotionIdx].nNumKey);
				}
				else if (strcmp(&cBffHead[0], "KEYSET") == 0)
				{
					//モデルパーツのセット番号
					int nPartsIdx = 0;

					//キーセット情報の読み取りループ処理
					while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
					{
						//文字列の分析
						sscanf(cBff, "%s", &cBffHead);

						if (strcmp(&cBffHead[0], "FRAME") == 0)
						{//ループ設定
						 //文字列の分析
							sscanf(cBff, "%s = %d", &cBffHead, &pEnemy->aMotion[nMotionIdx].nFrame[nKeySetIdx]);
							if (pEnemy->aMotion[nMotionIdx].nFrame[nKeySetIdx] == 0)
							{//再生フレーム数が0の場合1にする
								pEnemy->aMotion[nMotionIdx].nFrame[nKeySetIdx] = 1;
							}
						}
						if (strcmp(&cBffHead[0], "KEY") == 0)
						{//ループ設定
						 //キー情報の読み取りループ処理
							while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
							{
								//文字列の分析
								sscanf(cBff, "%s", &cBffHead);

								if (strcmp(&cBffHead[0], "POS") == 0)
								{//モーション再生中の位置情報読み込み
								 //文字列の分析
									sscanf(cBff, "%s = %f%f%f", &cBffHead,
										&pEnemy->aMotion[nMotionIdx].aKey[nKeySetIdx].pos[nPartsIdx].x,
										&pEnemy->aMotion[nMotionIdx].aKey[nKeySetIdx].pos[nPartsIdx].y,
										&pEnemy->aMotion[nMotionIdx].aKey[nKeySetIdx].pos[nPartsIdx].z);
								}
								else if (strcmp(&cBffHead[0], "ROT") == 0)
								{//モーション再生中の回転情報読み込み
									//文字列の分析
									sscanf(cBff, "%s = %f%f%f", &cBffHead,
										&pEnemy->aMotion[nMotionIdx].aKey[nKeySetIdx].rot[nPartsIdx].x,
										&pEnemy->aMotion[nMotionIdx].aKey[nKeySetIdx].rot[nPartsIdx].y,
										&pEnemy->aMotion[nMotionIdx].aKey[nKeySetIdx].rot[nPartsIdx].z);
								}
								else if (strcmp(&cBffHead[0], "END_KEY") == 0)
								{
									nPartsIdx++;
									break;
								}
							}
						}
						else if (strcmp(&cBffHead[0], "END_KEYSET") == 0)
						{
							nKeySetIdx++;
							break;
						}
					}
				}
				else if (strcmp(&cBffHead[0], "END_MOTIONSET") == 0)
				{
					nMotionIdx++;
					break;
				}
			}
		}
		else if (strcmp(&cBffHead[0], "END_SCRIPT") == 0)
		{//テキストファイルを読み切った時
			break;
		}
	}

	//ファイルを閉じる
	fclose(pFile);

	return S_OK;
}