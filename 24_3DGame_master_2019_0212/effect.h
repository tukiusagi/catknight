//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife);

#endif
