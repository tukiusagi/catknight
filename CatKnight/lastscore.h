//=============================================================================
//
// �X�R�A���� [lastscore.h]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#ifndef _LASTSCORE_H_
#define _LASTSCORE_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitLastScore(void);
void UninitLastScore(void);
void UpdateLastScore(void);
void DrawLastScore(void);

void AddLastScore(int nValue);
int	GetLastScore(void);
void SetLastScore(bool Set);
#endif
