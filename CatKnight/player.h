//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define PLAYER_MAX		(7)		//プレイヤーの総パーツ数
#define MAX_CHARA		(256)	//読み込む文字の最大数
#define LATEST_SPEED	(0.5f)	//移動速度の最低値
#define GRAVITY			(0.8f)	//重力
#define INERTIA			(0.25f)	//慣性

//=============================================================================
// プレイヤーの状態
//=============================================================================
typedef enum
{
	PLAYERSTATE_NEUTRAL,		//ニュートラル
	PLAYERSTATE_HOLDNEUTRAL,	//手つなぎニュートラル
	PLAYERSTATE_MOVE,			//歩行
	PLAYERSTATE_HOLDMOVE,		//手つなぎ歩行
	PLAYERSTATE_ATTACK01,		//攻撃01
	PLAYERSTATE_ATTACK02,		//攻撃02
	PLAYERSTATE_HIT,			//ダメージ
	PLAYERSTATE_DETH,			//死亡
	PLAYERSTATE_WIN,			//勝利
	PLAYERSTATE_MAX				//情報の最大数
} PLAYERSTATE;

//=============================================================================
// モデルの構造体
//=============================================================================
typedef struct
{
	LPD3DXMESH		pMesh = NULL;		//メッシュ情報のポインタ
	LPD3DXBUFFER	pBuffMat = NULL;	//マテリアル情報へのポインタ
	DWORD			nNumMat = 0;		//マテリアル情報の数
	D3DXVECTOR3		posOld;				//前回位置
	D3DXVECTOR3		rotMove;			//回転移動量
	D3DXMATRIX		mtxWorld;			//ワールドマトリックス
} PLAYERMODEL;

//=============================================================================
// プレイヤーの構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;				//位置
	D3DXVECTOR3	posOld;				//前回位置
	D3DXVECTOR3	rot;				//向き
	D3DXVECTOR3	rotDest;			//到達させたい角度
	D3DXVECTOR3	rotmove;			//回転移動量
	D3DXVECTOR3	move;				//移動
	D3DXMATRIX	mtxWorld;			//ワールドマトリックス
	float		fRadius;			//半径
	int			nLife;				//体力
	PLAYERMODEL	aModel[PLAYER_MAX];	//モデル情報
	PLAYERSTATE	state;				//プレイヤーの状態
	bool		bUse;				//使用状態
	bool		bAttack;			//攻撃状態
	bool		bHold;				//手をつないでいるか
	int			nCntState;			//状態カウンター
	bool		bDamege;			//攻撃を受けた
} PLAYER;

//=============================================================================
// 当たり判定の構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3	posOld;				//前回位置
	D3DXVECTOR3	rot;				//向き
	D3DXVECTOR3	move;				//移動
	D3DXMATRIX	mtxWorld;			//ワールドマトリックス
}COLLISION;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Radius);	//プレイヤーとの当たり判定
void CollisionWall(D3DXVECTOR3 *pPos, float fRadius);	//壁めり込み判定
void RotationDir(D3DXVECTOR3 &rotmove, D3DXVECTOR3 &rotDest, D3DXVECTOR3 &rot, float fSpeed);
void OperationMove(D3DXVECTOR3 *pMove, float fSpeed);	//移動
void SetPlayer(void);		//プレイヤーの設定
PLAYER *GetPlayer(void);	//プレイヤーの取得
void PlayMotion(void);		//モーション再生
void HitPlayer(int Damege, D3DXVECTOR3 *pPos);	//ダメージ判定
#endif