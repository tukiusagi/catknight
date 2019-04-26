//=============================================================================
//
// リザルト処理[result.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"result.h"
#include "input.h"	
#include "joystick.h"
#include "fade.h"
#include"logo.h"
#include"resultlogo.h"
#include "combo.h"
#include "score.h"
#include "enemywave.h"
#include "lastscore.h"
#include "sound.h"
#include "camera.h"
#include "player.h"
#include "effect.h"
#include "enemy.h"
#include "lifebar.h"
#include "magic.h"
#include "wave.h"
#include "trajectory.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SECOND (60)	//秒数

//=============================================================================
// リザルトの状態
//=============================================================================
typedef enum
{
	RESULTSTATE_NONE,		//通常
	RESULTSTATE_SCORE,		//スコア
	RESULTSTATE_COMBO,		//コンボ
	RESULTSTATE_WAVE,		//ウェーブ
	RESULTSTATE_LAST,		//最終スコア
	RESULTSTATE_NEXT,		//ランキングへ
	RESULTSTATE_MAX
} RESULTSTATE;

//=============================================================================
// リザルトの状態
//=============================================================================
typedef enum
{
	RESULT_NORMAL,		//通常
	RESULT_GETBOTTON,	//ボタンを押された
	RESULT_MAX
} RESULT;

//=============================================================================
// グローバル変数
//=============================================================================
RESULT ResultState;
RESULTSTATE g_NowState;
int g_nCntResult;
int g_Combo;
int g_nNumScore;
int g_Wave;

//=============================================================================
// 初期化処理
//=============================================================================
void InitResult(void)
{
	ResultState = RESULT_NORMAL;
	g_NowState = RESULTSTATE_NONE;

	g_nCntResult = 0;

	//最大コンボの取得
	DeleteCombo();
	g_Combo=GetCombo();
	AddCombo(g_Combo);

	AddScore(GetLifeber() * 100);
	g_nNumScore = GetScore();
	g_Wave = *GetEwave();

	//最終スコアの初期化
	InitLastScore();
	AddLastScore(g_nNumScore + g_Combo * 100 + g_Wave * 500);

	//結果発表の初期化
	InitReLogo();
	SetLogo(LOGO_RESULT);
}

//=============================================================================
// 終了処理
//=============================================================================
int UninitResult(void)
{
	// カメラの終了処理
	UninitCamera();

	//敵の終了
	UninitEnemy();

	// プレイヤーの終了
	UninitPlayer();

	//エフェクトの終了
	UninitEffect();
	UninitTrajectory();

	//魔法陣の終了
	UninitMagic();

	//魔法の終了
	UninitWave();

	//姫ライフの終了
	UninitLifebar();

	//結果発表の終了
	UninitReLogo();

	//スコアの終了
	UninitScore();
	UninitCombo();
	UninitEwave();
	UninitLastScore();

	//ロゴの終了
	DeleteLogo();

	return (g_nNumScore + g_Combo * 100 + g_Wave * 500);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	//ロゴの更新
	UpdateReLogo();

	//スコアの更新
	UpdateScore();
	UpdateCombo();
	UpdateEwave();
	UpdateLastScore();

	g_nCntResult++;
	//リザルト表示
	if (g_NowState == RESULTSTATE_NONE && ResultState != RESULT_GETBOTTON && (g_nCntResult == SECOND * 2 || (GetKeyTrigger(DIK_SPACE) || GetJoystickTrigger(PAD_B))))
	{
		SetScore(true);
		PlaySound(SOUND_LABEL_SE_PAUSE);
		ResultState = RESULT_GETBOTTON;
		g_NowState = RESULTSTATE_SCORE;
	}
	if (g_NowState == RESULTSTATE_SCORE && ResultState != RESULT_GETBOTTON && (g_nCntResult == SECOND * 3 || (GetKeyTrigger(DIK_SPACE) || GetJoystickTrigger(PAD_B))))
	{
		SetCombo(true);
		PlaySound(SOUND_LABEL_SE_PAUSE);
		ResultState = RESULT_GETBOTTON;
		g_NowState = RESULTSTATE_COMBO;
	}
	if (g_NowState == RESULTSTATE_COMBO && ResultState != RESULT_GETBOTTON && (g_nCntResult == SECOND * 4 || (GetKeyTrigger(DIK_SPACE) || GetJoystickTrigger(PAD_B))))
	{
		SetEwaveresult(true);
		PlaySound(SOUND_LABEL_SE_PAUSE);
		ResultState = RESULT_GETBOTTON;
		g_NowState = RESULTSTATE_WAVE;
	}
	if (g_NowState == RESULTSTATE_WAVE && ResultState != RESULT_GETBOTTON && (g_nCntResult == SECOND * 5 || (GetKeyTrigger(DIK_SPACE) || GetJoystickTrigger(PAD_B))))
	{
		SetLastScore(true);
		PlaySound(SOUND_LABEL_SE_CAT);
		ResultState = RESULT_GETBOTTON;
		g_NowState = RESULTSTATE_LAST;
	}
	if (g_NowState == RESULTSTATE_LAST && ResultState != RESULT_GETBOTTON && (GetKeyTrigger(DIK_SPACE) || GetJoystickTrigger(PAD_B)))	//Enterで終了
	{
		SetFade(MODE_RANKING);
		ResultState = RESULT_GETBOTTON;
	}
	else if (g_nCntResult > SECOND * 10)
	{
		SetFade(MODE_RANKING);
		ResultState = RESULT_GETBOTTON;
		g_nCntResult = 0;
	}

	ResultState = RESULT_NORMAL;
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void DrawResult(void)
{
	//ロゴの描画
	DrawReLogo();

	//スコアの描画
	DrawCombo();
	DrawScore();
	DrawEwave();
	DrawLastScore();

}