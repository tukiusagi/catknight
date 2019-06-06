//=============================================================================
//
// X�C���v�b���� [xController.h]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#ifndef _XCONTROLLER_H_
#define _XCONTROLLER_H_

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
	XINPUT_PAD_X = 0,
	XINPUT_PAD_B,
	XINPUT_PAD_A,
	XINPUT_PAD_Y,
	XINPUT_PAD_L,
	XINPUT_PAD_R,
	XINPUT_PAD_ZL,
	XINPUT_PAD_ZR,
	XINPUT_PAD_LSTICKBUTTON,
	XINPUT_PAD_RSTICKBUTTON,
	XINPUT_PAD_BACK,
	XINPUT_PAD_START,
	XINPUT_PAD_GUIDE,
	XINPUT_PAD_RIGHT_L,	//���X�e�B�b�N
	XINPUT_PAD_LEFT_L,		
	XINPUT_PAD_UP_L,		
	XINPUT_PAD_DOWN_L,		
	XINPUT_PAD_RIGHT_R,	//�E�X�e�B�b�N
	XINPUT_PAD_LEFT_R,		
	XINPUT_PAD_UP_R,		
	XINPUT_PAD_DOWN_R,		
	XINPUT_PAD_MAX
}XINPUT_PADTYPE;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitxController();
void UninitxController(void);
void UpdatexController(void);
HRESULT GetxController();
bool GetxControllerPress(int nKey);
bool GetxControllerTrigger (int nKey);

#endif
