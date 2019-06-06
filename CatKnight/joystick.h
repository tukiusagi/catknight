//=============================================================================
//
// ゲームパッド入力処理 [joystick.h]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// ボタンの種類
//=============================================================================
typedef enum
{
	PAD_X = 0,
	PAD_Y,
	PAD_A,
	PAD_B,
	PAD_L,
	PAD_R,
	PAD_ZL,
	PAD_ZR,
	PAD_LSTICKBUTTON,
	PAD_RSTICKBUTTON,
	PAD_BACK,
	PAD_START,
	PAD_GUIDE,
	PAD_RIGHT_L,	//左スティック
	PAD_LEFT_L,		
	PAD_UP_L,		
	PAD_DOWN_L,		
	PAD_RIGHT_R,	//右スティック
	PAD_LEFT_R,		
	PAD_UP_R,		
	PAD_DOWN_R,		
	PAD_MAX
}PADTYPE;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitJoystick(HINSTANCE hInstance, HWND hWnd);
void UninitJoystick(void);
BOOL UpdateJoystick(void);
bool GetJoystickPress(int nKey);
bool GetJoystickTrigger (int nKey);
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE*pdidInstance, VOID*pContext);
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE*pdidoi, VOID*pContext);

#endif
