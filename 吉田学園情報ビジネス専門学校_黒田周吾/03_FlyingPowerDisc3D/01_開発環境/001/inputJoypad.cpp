//=============================================================================
//
//	ジョイパッド入力処理 [inputJoypad.cpp]
//	Author : 黒田 周吾
//
//=============================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include "inputJoypad.h"

//======================================================
//	マクロ定義
//======================================================
#define MAX_PLAYER		(2)		//最大プレイヤー人数

//======================================================
//	グローバル変数
//======================================================
XINPUT_STATE g_JoyKeyState[MAX_PLAYER];				//ジョイパットのプレス情報
XINPUT_STATE g_JoyKeyStateTrigger[MAX_PLAYER];		//ジョイパットのトリガー情報
D3DXVECTOR3 g_JoyStickPos[MAX_PLAYER];				//ジョイスティックの傾き
XINPUT_VIBRATION g_JoyMoter[MAX_PLAYER];			//ジョイパッドのモーター
int g_nTime[MAX_PLAYER];							//振動持続時間
int g_nStrength[MAX_PLAYER];						//振動の強さ(0 - 65535)

//======================================================
//	ジョイパッドの初期化処理
//======================================================
HRESULT InitJoypad(HINSTANCE hInstance, HWND hWnd)
{
	//XInputのステートを設定（有効にする）
	XInputEnable(true);

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		//メモリーのクリア
		memset(&g_JoyKeyState[nCnt], 0, sizeof(XINPUT_STATE));
		memset(&g_JoyKeyStateTrigger[nCnt], 0, sizeof(XINPUT_STATE));

		//ジョイパッドの振動制御の０クリア
		ZeroMemory(&g_JoyMoter[nCnt], sizeof(XINPUT_VIBRATION));

		//振動制御用の初期化
		g_nStrength[nCnt] = 0;
		g_nTime[nCnt] = 0;
	}

	return S_OK;
}

//======================================================
//	ジョイパッドの終了処理
//======================================================
void UninitJoypad(void)
{
	//XInputのステートを無効にする
	XInputEnable(false);
}

//======================================================
//	ジョイパッドの更新処理
//======================================================
void UpdateJoypad(void)
{
	XINPUT_STATE JoyKeyState[MAX_PLAYER];		//ジョイパッド入力情報

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		//ジョイパッドの状態を取得
		if (XInputGetState(nCnt, &JoyKeyState[nCnt]) == ERROR_SUCCESS)
		{
			g_JoyKeyStateTrigger[nCnt].Gamepad.wButtons
				= ~g_JoyKeyState[nCnt].Gamepad.wButtons
				& JoyKeyState[nCnt].Gamepad.wButtons;		//トリガー情報を保存
			g_JoyKeyState[nCnt] = JoyKeyState[nCnt];		//プレス処理
		}

		//ジョイパッドの振動
		g_JoyMoter[nCnt].wLeftMotorSpeed = g_nStrength[nCnt];
		g_JoyMoter[nCnt].wRightMotorSpeed = g_nStrength[nCnt];
		XInputSetState(nCnt, &g_JoyMoter[nCnt]);

		if (g_nTime[nCnt] > 0)
		{
			g_nTime[nCnt]--;
		}
		else
		{
			g_nStrength[nCnt] = 0;
			g_nTime[nCnt] = 0;
		}
	}
}

//======================================================
//	ジョイパッドのプレス処理
//======================================================
bool GetJoypadPress(JOYKEY Key, int nPlayer)
{
	return (g_JoyKeyState[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//======================================================
//	ジョイパッドのトリガー処理
//======================================================
bool GetJoypadTrigger(JOYKEY Key, int nPlayer)
{
	return (g_JoyKeyStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//======================================================
//	ジョイパット（スティックプレス）処理
//======================================================
D3DXVECTOR3 GetJoypadStick(JOYKEY Key, int nPlayer)
{
	switch (Key)
	{
	case JOYKEY_LEFT_STICK:
		g_JoyStickPos[nPlayer] = D3DXVECTOR3(g_JoyKeyState[nPlayer].Gamepad.sThumbLX / 32767.0f, -g_JoyKeyState[nPlayer].Gamepad.sThumbLY / 32767.0f, 0.0f);
		break;
	case JOYKEY_RIGHT_STICK:
		g_JoyStickPos[nPlayer] = D3DXVECTOR3(g_JoyKeyState[nPlayer].Gamepad.sThumbRX / 32767.0f, -g_JoyKeyState[nPlayer].Gamepad.sThumbRY / 32767.0f, 0.0f);
		break;
	}

	return g_JoyStickPos[nPlayer];
}

//======================================================
//	ジョイパット（トリガーペダル）処理
//======================================================
int GetJoypadTriggerPedal(JOYKEY Key, int nPlayer)
{
	int nJoypadTriggerPedal = 0;
	switch (Key)
	{
	case JOYKEY_LEFT_TRIGGER:
		nJoypadTriggerPedal = g_JoyKeyState[nPlayer].Gamepad.bLeftTrigger;
		break;
	case JOYKEY_RIGHT_TRIGGER:
		nJoypadTriggerPedal = g_JoyKeyState[nPlayer].Gamepad.bRightTrigger;
		break;
	}

	return nJoypadTriggerPedal;
}

//======================================================
//	コントローラーの振動制御
//======================================================
void JoypadVibration(int nTime, WORD nStrength, int nPlayer)
{
	g_nTime[nPlayer] = nTime;			//振動持続時間
	g_nStrength[nPlayer] = nStrength;	//振動の強さ
}