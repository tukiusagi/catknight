//=============================================================================
//
// ランキング画面処理[ranking.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"ranking.h"
#include "input.h"	
#include "joystick.h"
#include "fade.h"
#include "camera.h"
#include "light.h"
#include "meshfield.h"
#include "meshwall.h"
#include"logo.h"
#include "meshcylinder.h"
#include "ground.h"
#include "rank.h"
#include "princess.h"
#include "shadow.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SECOND (60)

//=============================================================================
// ランキングの状態
//=============================================================================
typedef enum
{
	RANK_NORMAL,
	RANK_GETBOTTON,
	RANK_MAX
} RANKING;

//=============================================================================
// グローバル変数
//=============================================================================
RANKING RankState;
int g_nCntRanking;

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
void InitRanking(void)
{
	RankState = RANK_NORMAL;
	g_nCntRanking = 0;

	// カメラの初期化処理
	InitCamera();

	//ランキングの初期化
	InitRank();

	SetLogo(LOGO_RANKING);
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void UninitRanking(void)
{
	// カメラの終了処理
	UninitCamera();

	// ライトの終了処理
	UninitLight();

	//影の終了
	UninitShadow();

	//姫の終了
	UninitPrincess();

	//シリンダーの終了
	UninitMeshCylinder();

	//地面の終了
	UninitGround();

	//フィールドの終了
	UninitMeshField();

	//メッシュ壁の終了
	UninitMeshWall();

	//ロゴの終了
	DeleteLogo();

	//ランキングの終了
	UninitRank();
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void UpdateRanking(void)
{
	// カメラの更新処理
	UpdateCamera();

	// ライトの更新処理
	UpdateLight();

	//シリンダーの更新
	UpdateMeshCylinder();

	//地面の更新
	UpdateGround();

	//フィールドの更新
	UpdateMeshField();

	//メッシュ壁の更新
	UpdateMeshWall();

	//ランキングの更新
	UpdateRank();

	g_nCntRanking++;
	if (RankState == RANK_NORMAL && (GetKeyTrigger(DIK_SPACE)|| GetJoystickTrigger(PAD_B)))	//Enterで終了
	{
		SetFade(MODE_TITLE);
		RankState = RANK_GETBOTTON;
	}
	else if (g_nCntRanking > SECOND * 5)
	{
		SetFade(MODE_TITLE);
		g_nCntRanking = 0;
	}
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void DrawRanking(void)
{
	// カメラの設定
	SetCamera();

	//シリンダーの描画
	DrawMeshCylinder();

	//地面の描画
	DrawGround();

	//フィールドの描画
	DrawMeshField();

	//メッシュ壁の描画
	DrawMeshWall();

	//ランキングの描画
	DrawRank();
}