//=============================================================================
//
//	マウス入力処理のヘッダーファイル[inputMouse.h]
//	Author : 黒田 周吾
//
//=============================================================================
#ifndef _INPUT_MOUSE_H_		//このマクロ定義がされなかったら
#define _INPUT_MOUSE_H_		//二重インクルード防止のマクロ定義

//マウスボタンの番号
typedef enum
{
	CLICK_LEFT = 0,			//左クリック
	CLICK_RIGHT,			//右クリック
	CLICK_WHEEL,			//ホイールクリック(中央クリック)
	CLICK_MAX
}CLICKMOUSE;

//======================================================
//	プロトタイプ宣言
//======================================================
HRESULT InitMouse(HINSTANCE hinstance, HWND hWnd);
void UninitMouse(void);
void UpdateMouse(void);
bool GetMousePress(int nMouse);
bool GetMouseTrigger(int nMouse);
DIMOUSESTATE *GetMouseMove(void);

#endif