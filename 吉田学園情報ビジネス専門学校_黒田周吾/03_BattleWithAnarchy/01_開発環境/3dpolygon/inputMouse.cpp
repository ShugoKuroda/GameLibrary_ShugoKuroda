//=============================================================================
//
//	マウス入力処理 [inputMouse.cpp]
//	Author : 黒田 周吾
//
//=============================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include "inputMouse.h"

//======================================================
//	マクロ定義
//======================================================
#define NUM_MOUSE_MAX		(3)		//マウスキーの最大数

//======================================================
//	グローバル変数
//======================================================
LPDIRECTINPUT8 g_pDInput = NULL;				//DirectInputオブジェクト
LPDIRECTINPUTDEVICE8 g_pDIMouse = NULL;			//マウスデバイス
DIMOUSESTATE g_MouseStatePress;					//マウスのプレス情報
DIMOUSESTATE g_MouseStateTrigger;				//マウスのトリガー情報

//============================================
//	マウスの初期化処理
//============================================
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pDInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス(マウス)の生成
	if (FAILED(g_pDInput->CreateDevice(GUID_SysMouse, &g_pDIMouse, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_pDIMouse->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_pDIMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return E_FAIL;
	}

	//マウスへのアクセス権を獲得
	g_pDIMouse->Acquire();

	return S_OK;
}

//============================================
//	マウスの終了処理
//============================================
void UninitMouse(void)
{
	//入力デバイス(マウス)の破棄
	if (g_pDIMouse != NULL)
	{
		g_pDIMouse->Unacquire();
		g_pDIMouse->Release();
		g_pDIMouse = NULL;
	}

	//DirectInputオブジェクトの破棄
	if (g_pDInput != NULL)
	{
		g_pDInput->Release();
		g_pDInput = NULL;
	}
}

//============================================
//	キーボードの更新処理
//============================================
void UpdateMouse(void)
{
	DIMOUSESTATE aMouseState;
	int nCntMouse;

	//入力デバイスからデータを取得
	if (SUCCEEDED(g_pDIMouse->GetDeviceState(sizeof(aMouseState), &aMouseState)))
	{
		for (nCntMouse = 0; nCntMouse < NUM_MOUSE_MAX; nCntMouse++)
		{
			g_MouseStateTrigger.rgbButtons[nCntMouse] = (g_MouseStatePress.rgbButtons[nCntMouse] ^ aMouseState.rgbButtons[nCntMouse]) & aMouseState.rgbButtons[nCntMouse];		//トリガー情報の保存
		}
		g_MouseStatePress = aMouseState;			//プレス情報の保存														//キーボードのプレス情報を保存
	}
	else
	{
		g_pDIMouse->Acquire();		//キーボードへのアクセス権を獲得
	}
}

//============================================
//	マウスのプレス情報を取得
//============================================
bool GetMousePress(int nMouse)
{
	return (g_MouseStatePress.rgbButtons[nMouse] & 0x80) ? true : false;		//三項演算子(条件式)?A:B;(条件式が真ならAを返し、偽ならBを返す)
}

//============================================
//	マウスのトリガー情報の取得
//============================================
bool GetMouseTrigger(int nMouse)
{
	return(g_MouseStateTrigger.rgbButtons[nMouse] & 0x80) ? true : false;
}

//============================================
//	マウスの移動量情報の取得
//============================================
DIMOUSESTATE *GetMouseMove(void)
{
	return &g_MouseStatePress;
}