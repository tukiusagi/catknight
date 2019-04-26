//=============================================================================
//
// ライフメーター処理 [lifebar.h]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#ifndef _LIFEBAR_H_
#define _LIFEBAR_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitLifebar(void);
void UninitLifebar(void);
void UpdateLifebar(void);
void DrawLifebar(void);

void DecreaseLifebar(int nValue);
void IncreaseLifebar(int nlife);

float GetLifeber(void);
#endif
