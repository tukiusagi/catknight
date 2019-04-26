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
#define PLAYER_MAX	(7)		//プレイヤーの総パーツ数
#define MAX_CHARA	(256)	//読み込む文字の最大数

//=============================================================================
// プレイヤーの状態
//=============================================================================
typedef enum
{
	PLAYERSTATE_NEUTRAL,	//ニュートラル
	PLAYERSTATE_MOVE,		//歩行
	PLAYERSTATE_ATTACK01,	//攻撃01
	PLAYERSTATE_ATTACK02,	//攻撃02
	PLAYERSTATE_HIT,		//ダメージ
	PLAYERSTATE_DETH,		//死亡
	PLAYERSTATE_WIN,		//勝利
	PLAYERSTATE_MAX			//情報の最大数
} PLAYERSTATE;

//=============================================================================
// モデルの構造体
//=============================================================================
typedef struct
{
	LPD3DXMESH		pMesh = NULL;		//メッシュ情報のポインタ
	LPD3DXBUFFER	pBuffMat = NULL;	//マテリアル情報へのポインタ
	DWORD			nNumMat = 0;		//マテリアル情報の数
	D3DXVECTOR3		pos;				//位置
	D3DXVECTOR3		posOld;				//前回位置
	D3DXVECTOR3		rot;				//向き
	D3DXVECTOR3		rotMove;			//回転移動量
	D3DXMATRIX		mtxWorld;			//ワールドマトリックス
	int				nIdxModelParent;	//親モデルのインデックス
	char			*pFilename = NULL;	// ファイル名
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
	float		fSpeed;				//移動速度
	D3DXMATRIX	mtxWorld;			//ワールドマトリックス
	float		fRadius;			//半径
	int			nLife;				//体力
	int			nMaxLife;			//最大体力
	PLAYERMODEL	aModel[PLAYER_MAX];	//モデル情報
	PLAYERSTATE	state;				//プレイヤーの状態
	bool		bUse;				//使用状態
	bool		bAttack;			//攻撃状態
	int			nCntState;			//状態カウンター
	bool		bDamege;			//攻撃を受けた
} PLAYER;

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
}COLLISION;

//=============================================================================
// 軌跡の構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos[2];			//2点のオフセット値
	int			nParent;		//親の番号
}MESH_ORBIT;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Radius);

PLAYER *GetPlayer(void);
void PlayMotion(void);
void HitPlayer(int Damege, D3DXVECTOR3 *pPos);
void LoadPlayer(void);
void SetPlayer(void);
#endif