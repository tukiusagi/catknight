//=============================================================================
//
// 魔法攻撃処理 [wave.h]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#ifndef _WAVE_H_
#define _WAVE_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define WAVE_RADIUS_MAX		(150.0f)	//広がる最大値
#define WAVE_SPREAD_TIME	(1*60)	//広がる速さ

//=============================================================================
// 魔法の状態
//=============================================================================
typedef enum
{
	WAVE_STATE_CHARGE,	//チャージ中
	WAVE_STATE_CHARGEMAX,	//チャージ終了
	WAVE_STATE_ATTACK,	//攻撃
	WAVE_STATE_MAX		//情報の最大数
} WAVE_STATE;

//=============================================================================
//構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;			//位置
	D3DXVECTOR3	rot;			//向き
	D3DXVECTOR3	move;			//移動
	D3DXMATRIX	mtxWorld;		//ワールドマトリックス
	WAVE_STATE	state;			//状態
	D3DXCOLOR	col;			//色
	int			nNumVertex;		//総頂点数
	int			nNumIndex;		//総インデックス数
	int			nNumPolygon;	//総ポリゴン数
	int			nCharge;		//チャージ
	int			nInterval;		//インターバル
	float		fRadius;		//半径
	float		fHeight;		//高さ
	bool		bUse;			//使用状態
}WAVE;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitWave(void);
void UninitWave(void);
void UpdateWave(void);
void DrawWave(void);

void DeleteCharge(void);
bool CollisionWave(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float Radius);
WAVE *GetWave(void);
void SetWave(D3DXVECTOR3 pos);
void DeleteWave(void);

#endif
