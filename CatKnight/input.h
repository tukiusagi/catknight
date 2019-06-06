//=============================================================================
//
// ���͏��� [input.h]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyTrigger (int nKey);

#endif
