//=============================================================================
//
// ���C������ [line.h]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#ifndef _LINE_H_
#define _LINE_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitLine(void);
void UninitLine(void);
void UpdateLine(void);
void DrawLine(void);

void SetLine(D3DXVECTOR3 pos, D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR col);
void DeleteLine(void);

#endif
