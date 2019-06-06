//=============================================================================
//
// ボス処理 [boss.h]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define BOSS_MAX	(6)	//敵の総パーツ数
#define MAX_BOSS	(5)				//敵の最大数

//=============================================================================
// 敵の状態
//=============================================================================
typedef enum
{
	BOSSSTATE_NEUTRAL,	//ニュートラル
	BOSSSTATE_MOVE,		//歩行
	BOSSSTATE_ATTACK,	//攻撃
	BOSSSTATE_HIT,		//ダメージ
	BOSSSTATE_DETH,		//死亡
	BOSSSTATE_MAX		//情報の最大数
} BOSSSTATE;

//=============================================================================
// モデルの構造体
//=============================================================================
typedef struct
{
	LPD3DXMESH		pMesh = NULL;		//メッシュ情報のポインタ
	LPD3DXBUFFER	pBuffMat = NULL;	//マテリアル情報へのポインタ
	DWORD			nNumMat = 0;		//マテリアル情報の数
	D3DXVECTOR3		pos;				//位置
	D3DXVECTOR3		posMove;			//位置移動量
	D3DXVECTOR3		rot;				//向き
	D3DXVECTOR3		rotMove;			//回転移動量
	D3DXMATRIX		mtxWorld;			//ワールドマトリックス
	int				nIdxModelParent;	//親モデルのインデックス
	char			*pFilename = NULL;	// ファイル名
} BOSSMODEL;

//=============================================================================
// 当たり判定の構造体
//=============================================================================
typedef struct
{
	int			nIdxModelParent;	//親モデルのインデックス
	D3DXVECTOR3	pos;				//オフセット値
	D3DXVECTOR3	posOld;				//前回位置
	D3DXVECTOR3	rot;				//向き
	D3DXVECTOR3	move;				//移動
	float		fRadius;			//半径
	D3DXMATRIX	mtxWorld;			//ワールドマトリックス
	int			nStartFrame;		//開始するフレーム
	int			nEndFrame;			//終了するフレーム
}COLLISIONBOSS;

//=============================================================================
// ボスの構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;				//位置
	D3DXVECTOR3	posOld;				//前回位置
	D3DXVECTOR3	rot;				//向き
	D3DXVECTOR3	rotDest;			//到達させたい角度
	D3DXVECTOR3	rotmove;			//回転移動量
	D3DXVECTOR3	move;				//移動
	float		fSpeed;				//移動速度
	D3DXMATRIX	mtxWorld;			//ワールドマトリックス
	float		fRadius;			//半径
	int			nLife;				//体力
	int			nAttack;			//攻撃力
	BOSSMODEL	aModel[BOSS_MAX];	//モデル情報
	BOSSSTATE	state;				//敵の状態
	bool		bUse;				//使用状態
	bool		bHit;				//PLと当たった
	bool		bDamege;			//攻撃を受けた
	bool		bStop;				//スタン状態
	int			nCntState;			//状態カウンター
	int			nIdxShadowBoss;		//影のID
	int			nCntFrameBoss;		//現在のフレーム数
	int			nCntKeyBoss;		//現在のキー数
	int			nCntAttack;			//攻撃タイミング
	COLLISIONBOSS	CollisionBoss;		//当たり判定情報
} BOSS;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);

void MoveBoss(float fDistX, float fDistZ, float fDistans,int nCntBoss);
void AttackBoss(int nCntBoss);
void HitBoss(int Damege, D3DXVECTOR3 *pPos,int nCntBoss);
void SetBoss(D3DXVECTOR3 pos);
void DeleteBoss();
BOSS *GetBoss(void);
int GetBossNum(void);
void BossMotion(int nCntBoss);
bool CollisionBoss(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Radius, int nCntBoss);
#endif