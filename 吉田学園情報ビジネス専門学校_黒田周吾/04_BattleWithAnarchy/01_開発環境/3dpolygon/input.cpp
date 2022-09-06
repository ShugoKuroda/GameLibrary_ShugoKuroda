//=============================================================================
//
//	キーボード入力処理 [input.cpp]
//	Author : 黒田 周吾
//
//=============================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include "input.h"

//======================================================
//	マクロ定義
//======================================================
#define NUM_KEY_MAX		(256)		//キーの最大数

//======================================================
//	グローバル変数
//======================================================
LPDIRECTINPUT8 g_pInput = NULL;					//DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;		//入力デバイス(キーボード)へのポインタ
BYTE g_aKeyStatePress[NUM_KEY_MAX];				//キーボードのプレス情報
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];			//キーボードのトリガー情報

//============================================
//	キーボードの初期化処理
//============================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス(キーボード)の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//============================================
//	キーボードの終了処理
//============================================
void UninitKeyboard(void)
{
	//入力デバイス(キーボード)の破棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//DirectInputオブジェクトの破棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//============================================
//	キーボードの更新処理
//============================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyStatePress[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];		//トリガー情報の保存
			g_aKeyStatePress[nCntKey] = aKeyState[nCntKey];															//プレス情報の保存														//キーボードのプレス情報を保存
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();		//キーボードへのアクセス権を獲得
	}
}

//============================================
//	キーボードのプレス情報を取得
//============================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyStatePress[nKey] & 0x80) ? true : false;		//三項演算子(条件式)?A:B;(条件式が真ならAを返し、偽ならBを返す)
}

//============================================
//	トリガー情報の取得
//============================================
bool GetKeyboardTrigger(int nKey)
{
	return(g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}