//=============================================================================
//
//	入力処理のヘッダーファイル[input.h]
//	Author : 黒田 周吾
//
//=============================================================================
#ifndef _INPUT_H_		//このマクロ定義がされなかったら
#define _INPUT_H_		//二重インクルード防止のマクロ定義

//======================================================
//	プロトタイプ宣言
//======================================================
HRESULT InitKeyboard(HINSTANCE hinstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);

#endif