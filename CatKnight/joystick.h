//=============================================================================
//
// �Q�[���p�b�h���͏��� [joystick.h]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �{�^���̎��
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
	PAD_RIGHT_L,	//���X�e�B�b�N
	PAD_LEFT_L,		
	PAD_UP_L,		
	PAD_DOWN_L,		
	PAD_RIGHT_R,	//�E�X�e�B�b�N
	PAD_LEFT_R,		
	PAD_UP_R,		
	PAD_DOWN_R,		
	PAD_MAX
}PADTYPE;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitJoystick(HINSTANCE hInstance, HWND hWnd);
void UninitJoystick(void);
BOOL UpdateJoystick(void);
bool GetJoystickPress(int nKey);
bool GetJoystickTrigger (int nKey);
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE*pdidInstance, VOID*pContext);
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE*pdidoi, VOID*pContext);

#endif
