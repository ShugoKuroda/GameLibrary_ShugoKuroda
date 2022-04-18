//=============================================================================
//
//	ロード処理 [load.cpp]
//	Author : SHUGO KURODA
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

//======================================================
// マクロ定義
//======================================================
#define MAX_CHAR (256)			//一行の最大読み取り文字数

//-----------------------------------------------------------------------------
//モデルのセットロード
//-----------------------------------------------------------------------------
HRESULT LoadSetModel(HWND hWnd)
{
	FILE *pFile = NULL;				//ファイルポインター宣言
	char cScanf[MAX_CHAR];			//一行分読み取る変数
	char cScanfHead[MAX_CHAR];		//頭の文字を読み取る変数

	//ファイルを開く
	pFile = fopen("model.txt", "r");

	if (pFile == NULL)
	{//ファイルを開けなかった場合
		MessageBox(hWnd, "モデルテキストを開けませんでした", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	//文字列の読み取りループ処理
	while (fgets(cScanf, MAX_CHAR, pFile) != NULL)
	{
		//文字列の分析
		sscanf(cScanf, "%s", &cScanfHead);

		if (strcmp(&cScanfHead[0], "MODEL_FILENAME") == 0)
		{//Xファイルの相対パス用

			char sPath[MAX_CHAR];		//相対パス保存用

			//一行の文字列から相対パスの読み取り
			sscanf(cScanf, "%s = %s", &cScanfHead, &sPath[0]);

			//Xファイルの読み込み
			LoadModelType(hWnd, &sPath[0]);
		}
		else if (strcmp(&cScanfHead[0], "MODELSET") == 0)
		{//モデルの配置用
			Model model;				//モデル構造体情報の格納変数
			memset(&model, 0, (sizeof(Model)));		//初期化
			float fShadowX, fShadowZ;	//影の大きさ情報の格納変数
			int nCheck;					//影を使用するかどうか

			//モデル配置に必要な情報読み取りループ処理
			while (fgets(cScanf, MAX_CHAR, pFile) != NULL)
			{
				//文字列の分析
				sscanf(cScanf, "%s", &cScanfHead);

				if (strcmp(&cScanfHead[0], "TYPE") == 0)
				{//モデルの種類用
					sscanf(cScanf, "%s = %d", &cScanfHead, &model.nType);
				}
				else if (strcmp(&cScanfHead[0], "PLAYER") == 0)
				{//プレイヤーのゴール番号用(0はゴールじゃない)
					sscanf(cScanf, "%s = %d", &cScanfHead, &model.nPlayerGoal);
				}
				else if (strcmp(&cScanfHead[0], "POS") == 0)
				{//POS用
					sscanf(cScanf, "%s = %f%f%f", &cScanfHead, &model.pos.x, &model.pos.y, &model.pos.z);
				}
				else if (strcmp(&cScanfHead[0], "ROT") == 0)
				{//ROT用
					sscanf(cScanf, "%s = %f%f%f", &cScanfHead, &model.rot.x, &model.rot.y, &model.rot.z);
				}
				else if (strcmp(&cScanfHead[0], "SHADOW") == 0)
				{//影のセット
					sscanf(cScanf, "%s = %d%f%f", &cScanfHead, &nCheck, &fShadowX, &fShadowZ);
				}
				else if (strcmp(&cScanfHead[0], "END_MODELSET") == 0)
				{//モデルのセットに必要な情報を読み切った時
					break;
				}
			}

			if (nCheck >= 1)
			{//nCheckが1以上の場合、影を設定する
				model.nIdxShadow = SetShadow(D3DXVECTOR3(model.pos.x, 0.1f, model.pos.z), model.rot, fShadowX, fShadowZ);
			}

			//モデル配置
			SetModel(hWnd, model);
		}
		else if (strcmp(&cScanfHead[0], "TEXT_END") == 0)
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
	FILE *pFile = NULL;					//ファイルポインター宣言

	Player aPlayer;				//プレイヤー情報の保存用
	char cScanf[MAX_CHAR];		//一行分読み取るための変数
	char cScanfHead[MAX_CHAR];	//頭の文字を読み取るための変数
	int nMotionIdx = 0;			//モーション番号

	//プレイヤー情報の初期化
	ZeroMemory(&aPlayer, sizeof(Player));

	//ファイルを開く
	pFile = fopen("motion.txt", "r");

	if (pFile == NULL)
	{//開けなかった時用
		MessageBox(hWnd, "モーションテキストを開けませんでした", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	//文字列の読み取りループ処理
	while (fgets(cScanf, MAX_CHAR, pFile) != NULL)
	{
		//文字読み込み用変数の初期化
		memset(&cScanfHead, 0, sizeof(cScanfHead));
		//文字列の分析
		sscanf(cScanf, "%s", &cScanfHead);

		if (strcmp(&cScanfHead[0], "MODEL_FILENAME") == 0)
		{//Xファイルの相対パス用

			char sPath[MAX_CHAR];		//相対パス保存用

			//一行の文字列から相対パスの読み取り
			sscanf(cScanf, "%s = %s", &cScanfHead, &sPath[0]);

			//Xファイルの読み込み
			LoadPlayerType(hWnd, &sPath[0]);
		}
		else if (strcmp(&cScanfHead[0], "CHARACTERSET") == 0)
		{//プレイヤーの配置用

			//プレイヤー情報の読み取りループ処理
			while (fgets(cScanf, MAX_CHAR, pFile) != NULL)
			{
				//文字列の分析
				sscanf(cScanf, "%s", &cScanfHead);

				if (strcmp(&cScanfHead[0], "NUM_PARTS") == 0)
				{//パーツ数
				 //文字列の分析
					sscanf(cScanf, "%s = %d", &cScanfHead, &aPlayer.nNumParts);
				}
				else if (strcmp(&cScanfHead[0], "MOVE") == 0)
				{//移動量
				 //文字列の分析
					sscanf(cScanf, "%s = %f", &cScanfHead, &aPlayer.fMove);
				}
				else if (strcmp(&cScanfHead[0], "JUMP") == 0)
				{//ジャンプ量
				 //文字列の分析
					sscanf(cScanf, "%s = %f", &cScanfHead, &aPlayer.fJump);
				}
				else if (strcmp(&cScanfHead[0], "PARTSSET") == 0)
				{
					//インデックス保存用
					int nIndex = 0;

					//プレイヤーパーツ情報の読み取りループ処理
					while (fgets(cScanf, MAX_CHAR, pFile) != NULL)
					{
						//文字列の分析
						sscanf(cScanf, "%s", &cScanfHead);

						if (strcmp(&cScanfHead[0], "INDEX") == 0)
						{//パーツ番号
						 //文字列の分析
							sscanf(cScanf, "%s = %d", &cScanfHead, &nIndex);
							aPlayer.aParts[nIndex].nIndex = nIndex;
							aPlayer.aParts[nIndex].bUse = true;
						}
						else if (strcmp(&cScanfHead[0], "PARENT") == 0)
						{//現在のパーツの親
						 //文字列の分析
							sscanf(cScanf, "%s = %d", &cScanfHead, &aPlayer.aParts[nIndex].nParent);
						}
						else if (strcmp(&cScanfHead[0], "POS") == 0)
						{//位置
						 //文字列の分析
							sscanf(cScanf, "%s = %f%f%f", &cScanfHead, &aPlayer.aParts[nIndex].pos.x, &aPlayer.aParts[nIndex].pos.y, &aPlayer.aParts[nIndex].pos.z);
						}
						else if (strcmp(&cScanfHead[0], "ROT") == 0)
						{//回転(角度)
						 //文字列の分析
							sscanf(cScanf, "%s = %f%f%f", &cScanfHead, &aPlayer.aParts[nIndex].rot.x, &aPlayer.aParts[nIndex].rot.y, &aPlayer.aParts[nIndex].rot.z);
							aPlayer.aParts[nIndex].baseRot = aPlayer.aParts[nIndex].rot;
						}
						else if (strcmp(&cScanfHead[0], "END_PARTSSET") == 0)
						{//パーツ読み込み終了
							break;
						}
					}
				}
				else if (strcmp(&cScanfHead[0], "END_CHARACTERSET") == 0)
				{//プレイヤー読み込み終了
					break;
				}
			}
		}
		else if (strcmp(&cScanfHead[0], "MOTIONSET") == 0)
		{//モーション設定用

			//キーセット番号の保存用変数
			int nKeySetIdx = 0;
			//モーション情報の読み取りループ処理
			while (fgets(cScanf, MAX_CHAR, pFile) != NULL)
			{
				//文字列の分析
				sscanf(cScanf, "%s", &cScanfHead);

				if (strcmp(&cScanfHead[0], "LOOP") == 0)
				{//ループ設定
				 //文字列の分析
					sscanf(cScanf, "%s = %d", &cScanfHead, &aPlayer.aMotion[nMotionIdx].nLoop);
				}
				else if (strcmp(&cScanfHead[0], "NUM_KEY") == 0)
				{//キーの数
				 //文字列の分析
					sscanf(cScanf, "%s = %d", &cScanfHead, &aPlayer.aMotion[nMotionIdx].nNumKey);
				}
				else if (strcmp(&cScanfHead[0], "KEYSET") == 0)
				{
					//モデルパーツのセット番号
					int nPartsIdx = 0;

					//キーセット情報の読み取りループ処理
					while (fgets(cScanf, MAX_CHAR, pFile) != NULL)
					{
						//文字列の分析
						sscanf(cScanf, "%s", &cScanfHead);

						if (strcmp(&cScanfHead[0], "FRAME") == 0)
						{//ループ設定
							//文字列の分析
							sscanf(cScanf, "%s = %d", &cScanfHead, &aPlayer.aMotion[nMotionIdx].nFrame[nKeySetIdx]);
							if (aPlayer.aMotion[nMotionIdx].nFrame[nKeySetIdx] == 0)
							{//再生フレーム数が0の場合1にする
								aPlayer.aMotion[nMotionIdx].nFrame[nKeySetIdx] = 1;
							}
						}
						if (strcmp(&cScanfHead[0], "KEY") == 0)
						{//ループ設定
						 //キー情報の読み取りループ処理
							while (fgets(cScanf, MAX_CHAR, pFile) != NULL)
							{
								//文字列の分析
								sscanf(cScanf, "%s", &cScanfHead);

								if (strcmp(&cScanfHead[0], "POS") == 0)
								{//モーション再生中の位置情報読み込み
									//文字列の分析
									sscanf(cScanf, "%s = %f%f%f", &cScanfHead,
										&aPlayer.aMotion[nMotionIdx].aKey[nKeySetIdx].pos[nPartsIdx].x,
										&aPlayer.aMotion[nMotionIdx].aKey[nKeySetIdx].pos[nPartsIdx].y,
										&aPlayer.aMotion[nMotionIdx].aKey[nKeySetIdx].pos[nPartsIdx].z);
								}
								else if (strcmp(&cScanfHead[0], "ROT") == 0)
								{//モーション再生中の回転情報読み込み
									//文字列の分析
									sscanf(cScanf, "%s = %f%f%f", &cScanfHead,
										&aPlayer.aMotion[nMotionIdx].aKey[nKeySetIdx].rot[nPartsIdx].x,
										&aPlayer.aMotion[nMotionIdx].aKey[nKeySetIdx].rot[nPartsIdx].y,
										&aPlayer.aMotion[nMotionIdx].aKey[nKeySetIdx].rot[nPartsIdx].z);
								}
								else if (strcmp(&cScanfHead[0], "END_KEY") == 0)
								{
									nPartsIdx++;
									break;
								}
							}
						}
						else if (strcmp(&cScanfHead[0], "END_KEYSET") == 0)
						{
							nKeySetIdx++;
							break;
						}
					}
				}
				else if (strcmp(&cScanfHead[0], "END_MOTIONSET") == 0)
				{
					nMotionIdx++;
					break;
				}
			}
		}
		else if (strcmp(&cScanfHead[0], "END_SCRIPT") == 0)
		{//テキストファイルを読み切った時
			break;
		}
	}

	//プレイヤーを使用している状態にする
	aPlayer.bUse = true;

	//プレイヤー情報の設定
	SetPlayer(aPlayer, D3DXVECTOR3(0.0f, 0.0f, -(FIELD_SIZE_Z - PLAYER_WIDTH)), 0);
	SetPlayer(aPlayer, D3DXVECTOR3(0.0f, 0.0f, FIELD_SIZE_Z - PLAYER_WIDTH), 1);

	//ファイルを閉じる
	fclose(pFile);

	return S_OK;
}