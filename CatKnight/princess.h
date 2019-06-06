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
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define PRINCESS_MAX	(6)	//姫の総パーツ数

//=============================================================================
// 姫の状態
//=============================================================================
typedef enum
{
	PRINCESSSTATE_NEUTRAL,		//ニュートラル
	PRINCESSSTATE_HOLDNEUTRAL,	//手つなぎニュートラル
	PRINCESSSTATE_MOVE,			//移動
	PRINCESSSTATE_ATTACK,		//攻撃
	PRINCESSSTATE_WIN,			//勝利
	PRINCESSSTATE_DETH,			//死亡
	PRINCESSSTATE_MAX			//情報の最大数
} PRINCESSSTATE;

//=============================================================================
// モデルの構造体
//=============================================================================
typedef struct
{
	LPD3DXMESH		pMesh = NULL;		//メッシュ情報のポインタ
	LPD3DXBUFFER	pBuffMat = NULL;	//マテリアル情報へのポインタ
	DWORD			nNumMat = 0;		//マテリアル情報の数
	D3DXVECTOR3		rotMove;			//回転移動量
	D3DXMATRIX		mtxWorld;			//ワールドマトリックス
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
	int			nCntFrame;					//現在のフレーム数
	int			nCntKeyPrincess;			//現在のキー数
	int			nCntState;					//状態カウンター
	int			nMaxLife;
	bool		bDamege;					//攻撃を受けた
	bool		bPinch;						//危険か否か
	bool		bHold;						//手をつないでいるかどうか
	PRINCESSMODEL	aModel[PRINCESS_MAX];	//モデル情報
	PRINCESSSTATE	state;					//姫の状態
	PRINCESSSTATE	stateOld;				//前回の姫の状態

} PRINCESS;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitPrincess(void);
void UninitPrincess(void);
void UpdatePrincess(void);
void DrawPrincess(void);

void SetPrincess(void);			//姫の設置
PRINCESS *GetPrincess(void);	//姫情報取得
void PrincessMotion(void);		//モーション再生
void HoldPrincess(PLAYER *pPlayer, float fDistX, float fDistZ);	//連れ歩き
bool CollisionPrincess(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Radius,int nType);	//当たり判定
#endif