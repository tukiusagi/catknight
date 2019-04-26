//=============================================================================
//
// メッシュ壁処理 [meshwall.h]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MESHWALL_WIDTH		(100.0f)		//横幅
#define MESHWALL_HEIGHT		(100.0f)		//高さ
#define MESHWALL_W			(7)	//Xブロック数
#define MESHWALL_H			(1)				//Yブロック数

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitMeshWall(void);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif
