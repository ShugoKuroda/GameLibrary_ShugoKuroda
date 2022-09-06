//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0(タイトル曲)
	SOUND_LABEL_BGM001,			// BGM1(ゲーム曲)
	SOUND_LABEL_SE_SHOT,		// ディスクを投げる音
	SOUND_LABEL_SE_REVERSE,		// ディスクの反射音
	SOUND_LABEL_SE_KANKYAKU,	// 歓声音
	SOUND_LABEL_SE_VICTORY,		// 勝利SE
	SOUND_LABEL_SE_SELECT,		// 選択音(決定音)
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif