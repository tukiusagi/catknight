//=============================================================================
//
// ランキング処理 [rank.h]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#ifndef _RANK_H_
#define _RANK_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitRank(void);
void UninitRank(void);
void UpdateRank(void);
void DrawRank(void);

void AddRank(int nScore);

#endif
