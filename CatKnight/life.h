//=============================================================================
//
// 残りライフ処理 [life.h]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// 構造体定義
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	bool		bUse;	//表示しているかどうか
}LIFE;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitLife(void);
void UninitLife(void);
void UpdateLife(void);
void DrawLife(void);

void DecreaseLife(int nValue);
void IncreaseLife(int nlife);
#endif
