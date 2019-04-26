//=============================================================================
//
// 障害物処理 [obstacle.h]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define OBSTACLE_MAX		(30)		//モデル数

//=============================================================================
// モデルの構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;			//位置
	D3DXVECTOR3	rot;			//向き
	D3DXVECTOR3 move;			//移動
	D3DXVECTOR3 rotDest;		//到達させたい角度
	D3DXMATRIX	mtxWorldObstacle;	//ワールドマトリックス
	int			nIdxShadow;		//影のID
	int			nLife;			//耐久力
	int			nCntState;		//状態カウンター
	D3DXVECTOR3	vtxMin;			//最小値
	D3DXVECTOR3	vtxMax;			//最大値
	bool		bUse;			//使用状態
	bool		bHit;			//攻撃を受けた
}OBSTACLE;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitObstacle(void);
void UninitObstacle(void);
void UpdateObstacle(void);
void DrawObstacle(void);

OBSTACLE *GetObstacle(void);
void SetObstacle(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void DeleteObstacle(int Damege,int nCntObstacle);
void DeleteALLObstacle(void);

bool CollisionObstacle(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3*pMove,float Radius, int nCntbasin);
#endif
