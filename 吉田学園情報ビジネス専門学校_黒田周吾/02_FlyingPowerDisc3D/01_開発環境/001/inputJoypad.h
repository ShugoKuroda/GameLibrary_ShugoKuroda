//=============================================================================
//
// ジョイパッド入力処理 [inputJoypad.h]
// Author: SHUGO KURODA
//
//=============================================================================
#ifndef _INPUT_JOYPAD_H_		//このマクロ定義がされなかったら
#define _INPUT_JOYPAD_H_		//2重インクルード防止のマクロ定義

//----------------------------------------------------------------------------
//	インクルード
//----------------------------------------------------------------------------
#include "main.h"

//----------------------------------------------------------------------------
//	ジョイパッド列挙型
//----------------------------------------------------------------------------
typedef enum
{
	JOYKEY_UP = 0,				//十字ボタン上
	JOYKEY_DOWN,				//十字ボタン下
	JOYKEY_LEFT,				//十字ボタン左
	JOYKEY_RIGHT,				//十字ボタン右
	JOYKEY_START,				//スタートボタン
	JOYKEY_BACK,				//バックボタン
	JOYKEY_LEFT_THUMB,			//左スティック押込み
	JOYKEY_RIGHT_THUMB,			//右スティック押込み
	JOYKEY_LEFT_SHOULDER,		//L１ボタン
	JOYKEY_RIGHT_SHOULDER,		//R１ボタン
	JOYKEY_LEFT_TRIGGER,		//L２トリガー
	JOYKEY_RIGHT_TRIGGER,		//R２トリガー
	JOYKEY_A,					//Aボタン
	JOYKEY_B,					//Bボタン
	JOYKEY_X,					//Xボタン
	JOYKEY_Y,					//Yボタン
	JOYKEY_LEFT_STICK,			//左スティック
	JOYKEY_RIGHT_STICK,			//右スティック
	JOYKEY_MAX
}JOYKEY;

//======================================================
//	プロトタイプ宣言
//======================================================
HRESULT InitJoypad(HINSTANCE hinstance, HWND hWnd);
void UninitJoypad(void);
void UpdateJoypad(void);
bool GetJoypadPress(JOYKEY Key, int nPlayer);
bool GetJoypadTrigger(JOYKEY Key, int nPlayer);
D3DXVECTOR3 GetJoypadStick(JOYKEY Key, int nPlayer);
int GetJoypadTriggerPedal(JOYKEY Key, int nPlayer);
void JoypadVibration(int nTime, WORD nStrength, int nPlayer);

#endif

/*************************************************************************************************************

//main.hに追加分

//インクルード
#include "dinput.h"						//入力処理に必要
#include "Xinput.h"						//ジョイパット処理

//main.cppに追加分

//インクルード
#include "input.h"

//初期化処理に以下のコードを入力

//入力処理の初期化処理
if (FAILED(InitInput(hInstance, hWnd)))
{
return E_FAIL;
}

//終了処理に以下のコードを入力

//入力処理の終了処理
UninitInput();

//更新処理に以下のコードを入力

//入力処理の更新処理
UpdateInput();

//各関数の説明

//--------------------------------------------------------------------------------------------------------
//プロトタイプ宣言キーボード
//--------------------------------------------------------------------------------------------------------

{//引数にキーのマクロを入れそれが押されたかどうかをboolで返す
bool GetKeyboardPress(int nKey);			//キーボードプレス処理
bool GetKeyboardTrigger(int nKey);			//キーボードトリガー処理
bool GetKeyboardAllPress(void);				//キーボード全キープレス処理
bool GetKeyboardAllTrigger(void);			//キーボード全キートリガー処理
}

//--------------------------------------------------------------------------------------------------------
//プロトタイプ宣言ジョイパット
//--------------------------------------------------------------------------------------------------------

{//引数にキーの列挙型を入れそれが押されたかどうかをboolで返す
bool GetJoypadPress(JOYKEY Key, int nPlayer);					//ジョイパッドプレス処理
bool GetJoypadTrigger(JOYKEY Key, int nPlayer);					//ジョイパッドトリガー処理
}

//返す値　スティックを倒せば倒すほど(1.0f or -1.0f)に近く
//スティックの遊びはifの条件式に組む
//例　if(GetJoypadStick(JOYKEY_RIGHT_STICK,0)  > 0.5f)
//    {//右に半分より多く倒したら

{第一引数はキー（スティック）,第二引数はプレイヤー番号
D3DXVECTOR3 GetJoypadStick(JOYKEY Key, int nPlayer);			//ジョイパッドスティック処理
}

//返す値　ペダルがどれだけ押し込まれたかをintで返す
//ペダルの遊びはifの条件式に組む
//例　if(GetJoypadTriggerPedal(JOYKEY_RIGHT_TRIGGER,0) > 100))
//    {//右ペダルを100より多く倒したら

{第一引数はキー（スティック）,第二引数はプレイヤー番号
int GetJoypadTriggerPedal(JOYKEY Key, int nPlayer);				//ジョイパッドトリガーペダル処理
}

{第一引数は揺らす時間, 第二引数はどれくらいの強さで揺らすか, 第三引数はプレイヤー番号
void JoypadVibration(int nTime, WORD nStrength, int nPlayer);	//ジョイパッド振動制御
}

//--------------------------------------------------------------------------------------------------------
//プロトタイプ宣言マウス
//--------------------------------------------------------------------------------------------------------

{//引数にキーの列挙型を入れそれが押されたかどうかをboolで返す
bool GetMousePress(MOUSE mouse);			//マウスのプレス処理
bool GetMouseTrigger(MOUSE mouse);			//マウスのトリガー処理
}

//返す値　マウスのposを返す値
{
D3DXVECTOR3 GetMouse(void);					//マウスの画面内の位置
}

{//引数無し、返す値は奥に回すと＋で返り手前に回すとーで返ってきます
int GetMouseWheel(void);					//マウスのホイールの動き処理
}
************************************************************************************************************/