//=============================================================================
//
// 姫の処理 [princess.h]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#ifndef _PRINCESS_H_
#define _PRINCESS_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define PRINCESS_MAX	(6)	//姫の総パーツ数

//=============================================================================
// 姫の状態
//=============================================================================
typedef enum
{
	PRINCESSSTATE_NEUTRAL,	//ニュートラル
	PRINCESSSTATE_MOVE,		//移動
	PRINCESSSTATE_ATTACK,	//攻撃
	PRINCESSSTATE_WIN,		//勝利
	PRINCESSSTATE_DETH,		//死亡
	PRINCESSSTATE_MAX		//情報の最大数
} PRINCESSSTATE;

//=============================================================================
// モデルの構造体
//=============================================================================
typedef struct
{
	LPD3DXMESH		pMesh = NULL;		//メッシュ情報のポインタ
	LPD3DXBUFFER	pBuffMat = NULL;	//マテリアル情報へのポインタ
	DWORD			nNumMat = 0;		//マテリアル情報の数
	D3DXVECTOR3		pos;				//位置
	D3DXVECTOR3		rot;				//向き
	D3DXVECTOR3		rotMove;			//回転移動量
	D3DXMATRIX		mtxWorld;			//ワールドマトリックス
	int				nIdxModelParent;	//親モデルのインデックス
	char			*pFilename = NULL;	// ファイル名
} PRINCESSMODEL;

//=============================================================================
// 姫の構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;						//位置
	D3DXVECTOR3	posOld;						//前回位置
	D3DXVECTOR3	rot;						//向き
	D3DXVECTOR3	rotDest;					//到達させたい角度
	D3DXVECTOR3	rotmove;					//回転移動量
	D3DXVECTOR3	move;						//移動
	D3DXMATRIX	mtxWorld;					//ワールドマトリックス
	float		fRadius;					//半径
	int			nLife;						//体力
	int			nCntFrame;					//現在のフレーム数
	int			nCntKeyPrincess;			//現在のキー数
	int			nCntState;					//状態カウンター
	bool		bDamege;					//攻撃を受けた
	bool		bPinch;						//危険か否か
	PRINCESSMODEL	aModel[PRINCESS_MAX];	//モデル情報
	PRINCESSSTATE	state;					//姫の状態
} PRINCESS;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitPrincess(void);
void UninitPrincess(void);
void UpdatePrincess(void);
void DrawPrincess(void);

PRINCESS *GetPrincess(void);
void PrincessMotion(void);
void MovePrincess(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove);
bool CollisionPrincess(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Radius,int nType);
void LoadPrincess(void);
#endif