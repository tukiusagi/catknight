//=============================================================================
//
// ファイルロード処理 [fileload.h]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#ifndef _FILELOAD_H_
#define _FILELOAD_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_CHARA	(256)
#define MAX_PARTS	(10)
#define MAX_KEY		(4)
#define MAX_MOTION	(10)

//=============================================================================
// キャラの種類
//=============================================================================
typedef enum
{
	CHARA_NONE = -1,
	CHARA_PLAYER,
	CHARA_PRINCESS,
	CHARA_ENEMY,
	CHARA_BOSS,
	CHARA_MAX
}CHARA_TYPE;

//=============================================================================
// モーション管理
//=============================================================================
//キー要素
typedef struct
{
	D3DXVECTOR3	pos;	//位置
	D3DXVECTOR3	rot;	//向き
} KEY;

//キー情報
typedef struct
{
	int nFrame;				//フレーム数
	KEY	Key[MAX_PARTS];		//キー要素[パーツ数]
} KEY_INFO;

//モーション情報
typedef struct
{
	bool		bLoop;		//ループの有無
	int			nNumKey;	//キー数
	KEY_INFO	KeyInfo[MAX_KEY];	//キー情報[キーの数]
} MOTION_INFO;

//=============================================================================
// 構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos[MAX_PARTS];						//位置[パーツ数]
	D3DXVECTOR3	rot[MAX_PARTS];						//角度[パーツ数]
	char		cFileNeme[MAX_PARTS][MAX_CHARA];	//モデルファイル名[パーツ数]
	int			nParent[MAX_PARTS];					//親の番号[パーツ数]
	int			nNumModel = 0;						//読み込むモデル数
	int			nNumParts = 0;						//読み込むパーツ数
	float		fSpeed;								//速度
	float		fRadius;							//半径
	int			nAttack;							//攻撃力
	int			nMaxLife;							//最大体力
	D3DXVECTOR3	Collpos;							//位置(当たり判定)
	int			nCollParent;						//親の番号(当たり判定)
	float		fCollRadius;						//半径(当たり判定)
	int			nStart;								//開始するフレーム
	int			nEnd;								//終了するフレーム
	D3DXVECTOR3	Meshpos[2];							//位置(軌跡)
	int			nMeshParent;						//親の番号(軌跡)
	MOTION_INFO MotionInfo[MAX_MOTION];				//モーション情報[モーション数]
}LOAD;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void LoadFile(char *FileName, int nCharaType);	//ファイルの読み込み
void LoadLine(FILE *pFile,  char *cLine, char *cStr);	//一行読み込み
void CutTub(char *cStr);	//tab消し

void LoadChara(FILE *pFile, char *cLine, char *cStr, int nCharaType);	//キャラ情報
void LoadParts(FILE *pFile, char *cLine, char *cStr, int nCharaType);	//パーツ情報
void LoadMotion(FILE *pFile, char *cLine, char *cStr, int nCntMotion, int nCharaType);	//モーション情報
void LoadKey(FILE *pFile, char *cLine, char *cStr, int nCntMotion, int nCntKey, int nCntParts, int nCharaType);	//キー情報
void LoadXYZ(D3DXVECTOR3 *xyz, char *cStr, char *cPunctuation);	//XYZ情報

LOAD *GetLoad(int nCntType);

#endif