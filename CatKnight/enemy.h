//=============================================================================
//
// 敵処理 [enemy.h]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define ENEMY_MAX			(6)		//敵の総パーツ数
#define MAX_ENEMY			(60)	//敵の最大数
#define DAMEGE_WAITTIME		(10)	//ダメージ状態時間
#define PARALYZE_WAITTIME	(180)	//マヒ状態時間
#define ATTACK_TIMING		(180)	//攻撃タイミング

//=============================================================================
// 敵の状態
//=============================================================================
typedef enum
{
	ENEMYSTATE_NEUTRAL,	//ニュートラル
	ENEMYSTATE_MOVE,	//歩行
	ENEMYSTATE_ATTACK,	//攻撃
	ENEMYSTATE_HIT,		//ダメージ
	ENEMYSTATE_DETH,	//死亡
	ENEMYSTATE_MAX		//情報の最大数
} ENEMYSTATE;

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
} ENEMYMODEL;

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
}COLLISIONENEMY;

//=============================================================================
// 敵の構造体
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
	ENEMYMODEL	aModel[ENEMY_MAX];	//モデル情報
	ENEMYSTATE	state;				//敵の状態
	bool		bUse;				//使用状態
	bool		bHit;				//PLと当たった
	bool		bDamege;			//攻撃を受けた
	bool		bStop;				//スタン状態
	int			nIdxShadowEnemy;	//影のID
	int			nCntFrameEnemy;		//現在のフレーム数
	int			nCntKeyEnemy;		//現在のキー数
	int			nCntAttack;			//攻撃タイミング
	int			nCntState;			//状態カウンター
	COLLISIONENEMY	CollisionEnemy;		//当たり判定情報
} ENEMY;


//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

void CalculateDist(float &fDistX, float &fDistZ, float &fDistans, D3DXVECTOR3 pos);
void MoveEnemy(float fDistX, float fDistZ, float fDistans, int nCntEnemy);
void AttackEnemy(int nCntEnemy);
void HitEnemy(int nCntEnemy, int Damege, D3DXVECTOR3 *pPos);
void SetEnemy(D3DXVECTOR3 pos);
void DeleteEnemey(void);
ENEMY *GetEnemy(void);
int *GetNumEnemy(void);
void EnemyMotion(int nCntEnemy);
bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Radius,int nCntEnemy);

#endif