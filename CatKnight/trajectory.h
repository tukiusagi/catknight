#pragma once
//=============================================================================
//
// 軌跡処理 [trajectory.h]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#ifndef _TRAJECTORY_H_
#define _TRAJECTORY_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TRAJECTORY_POLYGON			(10)				//最大ポリゴン数

//=============================================================================
// 軌跡の構造体
//=============================================================================
typedef struct
{
	D3DXMATRIX	mtxWorld;		//ワールドマトリックス
	D3DXMATRIX	aMtxPoint[2];	//オフセットのマトリックス
	D3DXMATRIX	*pMtxParent;	//親のマトリックス
	D3DXVECTOR3	offset[2];		//オフセット
	D3DXVECTOR3	pos[TRAJECTORY_POLYGON * 2 + 2];			//頂点の位置情報（頂点数分）
	D3DXVECTOR3	rot[2];			//向き
	D3DXCOLOR	col;			//色
	int			nNumVertex;		//総頂点数
	int			nNumPolygon;	//総ポリゴン数
	bool		bUse;			//使用しているか
}MESH_TRAJECTORY;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitTrajectory(void);
void UninitTrajectory(void);
void UpdateTrajectory(void);
void DrawTrajectory(void);
void SetTrajectory(D3DXVECTOR3 offset00, D3DXVECTOR3 offset01, D3DXMATRIX *mtxParent, D3DXCOLOR	col);

MESH_TRAJECTORY *GetTrajectory();
#endif
