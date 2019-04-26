//=============================================================================
//
// ゲームパッド入力処理 [joystick.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include "joystick.h"
#include"input.h"	

//=============================================================================
// マクロ定義
//=============================================================================
#define	NUM_BUTTON_MAX			(128)	// ボタンの最大数

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECTINPUT8 g_pJoystick = NULL;				//DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevJoystick = NULL;		//入力デバイス（キーボード）へのポインタ
DIDEVCAPS g_diDevCaps;
BYTE g_aButtonState[NUM_BUTTON_MAX];			//キーボード入力情報ワーク
BYTE g_aButtonStateTrigger[NUM_BUTTON_MAX];		//キーボード入力トリガー
BYTE g_aPad[PAD_MAX];

//=============================================================================
// ジョイスティックの初期化
//=============================================================================
HRESULT InitJoystick(HINSTANCE hInstance, HWND hWnd)
{

	HRESULT hr;
	int nCntPad;

	for (nCntPad = 0; nCntPad < PAD_MAX; nCntPad++)
	{
		g_aPad[nCntPad] = false;
	}

	// DirectInputオブジェクトの作成
	if (FAILED(hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pJoystick, NULL)))
	{
		return E_FAIL;
	}
	
	// 入力デバイスの作成
	if (FAILED(hr = g_pJoystick->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}
	
	if (g_pDevJoystick == NULL)
	{
		return false;
	}
	// データフォーマットを設定
	if (FAILED(hr = g_pDevJoystick->SetDataFormat(&c_dfDIJoystick2)))
	{
		return E_FAIL;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	if (FAILED(hr = g_pDevJoystick->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// デバイスの能力の取得
	hr = g_pDevJoystick->GetCapabilities(&g_diDevCaps);

	//十字キーの範囲指定
	hr = g_pDevJoystick->EnumObjects(EnumAxesCallback, (VOID*)hWnd, DIDFT_AXIS);

	hr = g_pDevJoystick->Poll();
	if (FAILED(hr))
	{
		hr = g_pDevJoystick->Acquire();
		while (hr==DIERR_INPUTLOST)
		{
			hr = g_pDevJoystick->Acquire();
		}
	}
	return S_OK;
}

//=============================================================================
// ジョイスティックの終了処理
//=============================================================================
void UninitJoystick(void)
{
	// 入力デバイス(ジョイスティック)の開放
	if (g_pDevJoystick != NULL)
	{
		g_pDevJoystick->Unacquire();
		g_pDevJoystick->Release();
		g_pDevJoystick = NULL;
	}
	
	// DirectInputオブジェクトの開放
	if (g_pJoystick != NULL)
	{
		g_pJoystick->Release();
		g_pJoystick = NULL;
	}
}

//=============================================================================
// ジョイスティックの更新処理
//=============================================================================
BOOL UpdateJoystick(void)
{
	HRESULT	hr;
	DIJOYSTATE2	js;
	int nCntButton;

	if (NULL == g_pDevJoystick)
	{
		return FALSE;
	}

	hr = g_pDevJoystick->Poll();
	if (FAILED(hr))
	{
		hr = g_pDevJoystick->Acquire();
	}

	hr = g_pDevJoystick->GetDeviceState(sizeof(DIJOYSTATE2), &js);
	if (SUCCEEDED(hr))
	{
		//ボタン
		if (js.rgbButtons[0] & 0x80)
		{
			g_aPad[PAD_X] = true;
		}
		else
		{
			g_aPad[PAD_X] = false;
		}
		if (js.rgbButtons[1] & 0x80)
		{
			g_aPad[PAD_Y] = true;
		}
		else
		{
			g_aPad[PAD_Y] = false;
		}
		 if (js.rgbButtons[2] & 0x80)
		{
			g_aPad[PAD_A] = true;
		}
		 else
		 {
			 g_aPad[PAD_A] = false;
		 }
		 if (js.rgbButtons[3] & 0x80)
		{
			g_aPad[PAD_B] = true;
		}
		 else
		 {
			 g_aPad[PAD_B] = false;
		 }
		 if (js.rgbButtons[4] & 0x80)
		{
			g_aPad[PAD_L] = true;
		}
		 else
		 {
			 g_aPad[PAD_L] = false;
		 }
		 if (js.rgbButtons[5] & 0x80)
		{
			g_aPad[PAD_R] = true;
		}
		 else
		 {
			 g_aPad[PAD_R] = false;
		 }
		 if (js.rgbButtons[6] & 0x80)
		{
			g_aPad[PAD_ZL] = true;
		}
		 else
		{
			 g_aPad[PAD_ZL] = false;
		}
		if (js.rgbButtons[7] & 0x80)
		{
			g_aPad[PAD_ZR] = true;
		}
		else
		{
			g_aPad[PAD_ZR] = false;
		}

		if (js.rgbButtons[8] & 0x80)
		{
			g_aPad[PAD_LSTICKBUTTON] = true;
		}
		if (js.rgbButtons[9] & 0x80)
		{
			g_aPad[PAD_RSTICKBUTTON] = true;
		}
		if (js.rgbButtons[10] & 0x80)
		{
			g_aPad[PAD_BACK] = true;
		}
		if (js.rgbButtons[11] & 0x80)
		{
			g_aPad[PAD_START] = true;
		}
		else
		{
			g_aPad[PAD_START] = false;
		}
		if (js.rgbButtons[12] & 0x80)
		{
			g_aPad[PAD_GUIDE] = true;
		}
		else
		{
			g_aPad[PAD_GUIDE] = false;
		}

		//左スティック
		//左右
		if (js.lX >= 800)
		{
			g_aPad[PAD_RIGHT_L] = true;
		}
		else
		{
			g_aPad[PAD_RIGHT_L] = false;
		}
		if (js.lX <= -800)
		{
			g_aPad[PAD_LEFT_L] = true;
		}
		else
		{
			g_aPad[PAD_LEFT_L] = false;
		}
		//上下
		if (js.lY <= -800)
		{
			g_aPad[PAD_UP_L] = true;
		}
		else
		{
			g_aPad[PAD_UP_L] = false;
		}

		if (js.lY >= 800)
		{
			g_aPad[PAD_DOWN_L] = true;
		}
		else
		{
			g_aPad[PAD_DOWN_L] = false;
		}
		
		//右スティック
		//左右
		if (js.lZ >= 800)
		{
			g_aPad[PAD_RIGHT_R] = true;
		}
		else
		{
			g_aPad[PAD_RIGHT_R] = false;
		}
		if (js.lZ <= -800)
		{
			g_aPad[PAD_LEFT_R] = true;
		}
		else
		{
			g_aPad[PAD_LEFT_R] = false;
		}
		//上下
		if (js.lRz <= -800)
		{
			g_aPad[PAD_UP_R] = true;
		}
		else
		{
			g_aPad[PAD_UP_R] = false;
		}

		if (js.lRz >= 800)
		{
			g_aPad[PAD_DOWN_R] = true;
		}
		else
		{
			g_aPad[PAD_DOWN_R] = false;
		}


		for (nCntButton = 0; nCntButton < PAD_MAX; nCntButton++)
		{
			g_aButtonStateTrigger[nCntButton] = (g_aButtonState[nCntButton] ^ g_aPad[nCntButton]) & g_aPad[nCntButton];	//キートリガー情報保存
			g_aButtonState[nCntButton] = g_aPad[nCntButton];
		}
	}
	return hr;
}

//=============================================================================
// ボタンのプレス状態を取得
//=============================================================================
bool GetJoystickPress(int nKey)
{
	return (g_aPad[nKey]) ? true : false;	//三項演算子
}

//=============================================================================
// トリガー処理
//=============================================================================
bool GetJoystickTrigger(int nKey)
{
	return (g_aButtonStateTrigger[nKey]) ? true : false;	//三項演算子
}

//=============================================================================
// コールバック関数
//=============================================================================
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE*pdidInstance, VOID*pContext)
{
	HRESULT hr;

	hr = g_pJoystick->CreateDevice(pdidInstance->guidInstance, &g_pDevJoystick, NULL);
	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}

	return DIENUM_STOP;
}

//=============================================================================
// キー設定コールバック関数
//=============================================================================
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE*pdidoi, VOID*pContext)
{
	HRESULT hr;
	DIPROPRANGE diprg;

	diprg.diph.dwSize		= sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow		= DIPH_BYID;
	diprg.diph.dwObj		= pdidoi->dwType;
	diprg.lMin				= 0 - 1000;
	diprg.lMax				= 0 + 1000;

	hr = g_pDevJoystick->SetProperty(DIPROP_RANGE,&diprg.diph);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}
