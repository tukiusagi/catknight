//=============================================================================
//
// ゲーム処理[game.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"game.h"	//インクルードファイル
#include "camera.h"
#include "light.h"
#include "player.h"
#include "input.h"
#include "joystick.h"
#include "shadow.h"
#include "meshfield.h"
#include "meshwall.h"
#include "effect.h"
#include "fade.h"
#include "line.h"
#include "princess.h"
#include "enemy.h"
#include "life.h"
#include "logo.h"
#include "meshcylinder.h"
#include "ground.h"
#include "lifebar.h"
#include "pause.h"
#include "boss.h"
#include "magic.h"
#include "wave.h"
#include "obstacle.h"
#include "trajectory.h"
#include "score.h"
#include "combo.h"
#include "enemywave.h"
#include "sound.h"
#include"help.h"
#include "particle.h"

//=============================================================================
// グローバル変数
//=============================================================================
GAMESTATE g_gameState = GAMESTATE_NONE;	//ゲーム状態
int g_nCounterGameState = 0;			//ゲーム状態カウンター
int g_nLastScore;							//最終スコア

//=============================================================================
// 初期化処理
//=============================================================================
void InitGame(void)
{
	srand((unsigned int)time(NULL));

	g_gameState = GAMESTATE_NORMAL;	//ゲーム状態
	g_nCounterGameState = 0;
	g_nLastScore = 0;

	//ポーズの初期化
	InitPause();

	//ラインの初期化
	InitLine();

	//オブジェクトの初期化
	InitObstacle();
	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		SetObstacle(D3DXVECTOR3(rand() % 220 + 80,
			0.0f,
			rand() % int(MESHFIELD_SIZE * MESHFIELD_D * 0.9f) - int(MESHFIELD_SIZE * MESHFIELD_D * 0.4f)),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		SetObstacle(D3DXVECTOR3(-rand() % 220 - 80,
			0.0f,
			rand() % int(MESHFIELD_SIZE * MESHFIELD_D*0.9f) - int(MESHFIELD_SIZE * MESHFIELD_D * 0.4f)),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}


	//敵の初期化
	//InitEnemy();
	SetEnemy(D3DXVECTOR3(300, 0.0f, 300), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
	SetEnemy(D3DXVECTOR3(-300, 0.0f, 300), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
	SetEnemy(D3DXVECTOR3(300, 0.0f, -300), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
	SetEnemy(D3DXVECTOR3(-300, 0.0f, -300), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);

	//チャージリセット
	DeleteCharge();

	//プレイヤーの初期設定
	SetPlayer();

	//ボスの初期化
	InitBoss();

	//パーティクルの初期化
	InitParticle();

	//PLライフの初期化
	InitLife();

	//姫ライフの初期化
	InitLifebar();

	//スコアの初期化
	InitScore();
	InitCombo();
	InitEwave();

	//ヘルプの初期化
	InitHelp();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	//エネミーの消去
	DeleteEnemey();

	//ボスの終了
	UninitBoss();

	//オブジェクトの終了
	UninitObstacle();
	DeleteALLObstacle();

	//PLライフの終了
	UninitLife();

	//ヘルプの終了
	UninitHelp();

	//ポーズの終了
	UninitPause();

	//ラインの終了
	UninitLine();

	//ロゴの終了
	DeleteLogo();

	//パーティクルの終了
	UninitParticle();

	SetScore(false);
	SetCombo(false);
	SetEwaveresult(false);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:
		// カメラの更新処理
		UpdateCamera();

		// ライトの更新処理
		UpdateLight();

		//シリンダーの更新
		UpdateMeshCylinder();

		//影の更新
		UpdateShadow();

		//魔法陣の更新
		UpdateMagic();

		//地面の更新
		UpdateGround();

		//フィールドの更新
		UpdateMeshField();

		//メッシュ壁の更新
		UpdateMeshWall();

		//モデルの更新
		UpdateObstacle();

		//姫の更新
		UpdatePrincess();

		//プレイヤーの更新
		UpdatePlayer();

		//敵の更新
		UpdateEnemy();

		//ボスの更新
		UpdateBoss();

		//エフェクトの更新
		UpdateEffect();
		UpdateTrajectory();

		//魔法の更新
		UpdateWave();

		//ラインの更新
		UpdateLine();

		//パーティクルの更新
		UpdateParticle();

		//PLライフの更新
		UpdateLife();

		//姫ライフの更新
		UpdateLifebar();

		//ヘルプの更新
		UpdateHelp();

		//スコアの更新
		UpdateScore();
		UpdateCombo();
		UpdateEwave();

		if (GetKeyTrigger(DIK_P) || GetJoystickTrigger(PAD_START))
		{
			g_gameState = GAMESTATE_PAUSE;
			PlaySound(SOUND_LABEL_SE_PAUSE);
		}
		break;

	case GAMESTATE_CLEAR:
		// カメラの更新処理
		UpdateCamera();

		//魔法陣の更新
		UpdateMagic();

		//魔法の更新
		UpdateWave();

		//エフェクトの更新
		UpdateEffect();
		UpdateTrajectory();

		//姫の更新
		UpdatePrincess();

		//敵の更新
		UpdateEnemy();

		//ボスの更新
		UpdateBoss();

		//プレイヤーの更新
		UpdatePlayer();

		//パーティクルの更新
		UpdateParticle();

		g_nCounterGameState++;
		if ((GetKeyTrigger(DIK_RETURN) || GetJoystickTrigger(PAD_START)))	//Enterで終了
		{
			SetFade(MODE_RESULT);
			g_gameState = GAMESTATE_NONE;
		}
		if (g_nCounterGameState >= COUNTER_MAX * 5)
		{
			SetFade(MODE_RESULT);
			g_gameState = GAMESTATE_NONE;
		}
		break;

	case GAMESTATE_GAMEOVER:
		// カメラの更新処理
		UpdateCamera();

		//魔法陣の更新
		UpdateMagic();

		//エフェクトの更新
		UpdateEffect();
		UpdateTrajectory();

		//姫の更新
		UpdatePrincess();

		//敵の更新
		UpdateEnemy();

		//ボスの更新
		UpdateBoss();

		//プレイヤーの更新
		UpdatePlayer();

		//パーティクルの更新
		UpdateParticle();

		//ヘルプの更新
		UpdateHelp();

		g_nCounterGameState++;
		if ((GetKeyTrigger(DIK_RETURN) || GetJoystickTrigger(PAD_START)))	//Enterで終了
		{
			SetFade(MODE_RESULT);
			g_gameState = GAMESTATE_NONE;
		}
		if (g_nCounterGameState >= COUNTER_MAX * 5)
		{
			SetFade(MODE_RESULT);
			g_gameState = GAMESTATE_NONE;
		}
		break;

	case GAMESTATE_PAUSE:
		//ポーズの更新
		UpdatePause();
		break;

	default:
		break;
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame(void)
{
	// カメラの設定
	SetCamera();

	//地面の描画
	DrawGround();

	//フィールドの描画
	DrawMeshField();

	//メッシュ壁の描画
	DrawMeshWall();

	//影の描画
	DrawShadow();

	//魔法陣の描画
	DrawMagic();

	//姫の描画
	DrawPrincess();

	//敵の描画
	DrawEnemy();

	//ボスの描画
	DrawBoss();

	//プレイヤーの描画
	DrawPlayer();

	//モデルの描画
	DrawObstacle();

	//シリンダーの描画
	DrawMeshCylinder();

	//ラインの描画
	DrawLine();

	//魔法の描画
	DrawWave();

	//エフェクトの描画
	DrawEffect();
	DrawTrajectory();

	//パーティクルの描画
	DrawParticle();

	//PLライフの描画
	DrawLife();

	//姫ライフの描画
	DrawLifebar();

	//ヘルプの描画
	DrawHelp();

	//スコアの描画
	DrawScore();
	DrawCombo();
	DrawEwave();

	switch (g_gameState)
	{
	case GAMESTATE_PAUSE:
		//ポーズの描画
		DrawPause();
		break;
	default:
		break;
	}
}

//=============================================================================
// ゲーム状態の設定
//=============================================================================
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterGameState = 0;
}

//=============================================================================
// ゲーム状態の取得
//=============================================================================
GAMESTATE *GetGameState(void)
{
	return &g_gameState;
}
