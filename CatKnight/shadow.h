//=============================================================================
//
// �e���� [shadow.h]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

int SetSadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSize, D3DXCOLOR col);
void DeleteShadow(int IdxShadow);
void SetPositionSadow(int IdxShadow, D3DXVECTOR3 pos, float fSize, D3DXCOLOR col);

#endif
