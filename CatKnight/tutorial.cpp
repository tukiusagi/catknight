//=============================================================================
//
// チュートリアル処理[tutorial.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"tutorial.h"	//インクルードファイル
#include"manual.h"

#include "input.h"	
#include "joystick.h"
#include "xController.h"

#include"pressstart.h"
#include"logo.h"
#include "fade.h"
#include "sound.h"

#include "camera.h"
#include "light.h"

#include "meshfield.h"
#include "meshwall.h"
#include "meshcylinder.h"
#include "ground.h"

#include "player.h"
#include "princess.h"
#include "enemy.h"

#include "shadow.h"
#include "effect.h"
#include "magic.h"
#include "wave.h"
#include "trajectory.h"
#include "particle.h"
#include "magicGage.h"
#include "lifePrincess.h"

//=============================================================================
// チュートリアルの状態
//=============================================================================
typedef enum
{
	TUTORIAL_NORMAL,	//通常
	TUTORIAL_GETBOTTON,	//ボタンを押された
	TUTORIAL_MAX
} TUTORIAL;

//=============================================================================
// グローバル変数
//=============================================================================
TUTORIAL g_Tutorialstate;

//=============================================================================
// 初期化処理
//=============================================================================
void InitTutorial(void)
{
	g_Tutorialstate = TUTORIAL_NORMAL;

	// カメラの初期化処理
	InitCamera();

	//影の初期化
	InitShadow();

	//魔法陣の初期化
	InitMagic();

	//魔法の初期化
	InitWave();
	SetWave(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//エフェクトの初期化
	InitEffect();
	InitTrajectory();

	//パーティクルの初期化
	InitParticle();

	// プレイヤーの初期化
	InitPlayer();

	//姫の初期化
	InitPrincess();

	//敵の初期化
	InitEnemy();
	SetEnemy(D3DXVECTOR3(0, 0.0f, 100));

	//姫ライフの初期化
	InitPLife();

	//魔法ゲージの初期化
	InitMagicGage();

	//プレススタートの初期化
	InitStart();

	//操作説明
	InitManual();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTutorial(void)
{
	//プレススタートの終了
	UninitStart();

	//エネミーの消去
	DeleteEnemey();

	////パーティクルの終了
	//UninitParticle();

	//操作説明
	UninitManual();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTutorial(void)
{
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

	//姫の更新
	UpdatePrincess();

	//プレイヤーの更新
	UpdatePlayer();

	//敵の更新
	UpdateEnemy();

	//エフェクトの更新
	UpdateEffect();
	UpdateTrajectory();

	//パーティクルの更新
	UpdateParticle();

	//魔法の更新
	UpdateWave();

	//姫ライフの更新
	UpdatePLife();

	//魔法ゲージの更新
	UpdateMagicGage();

	//プレススタートの更新
	UpdateStart();

	//操作説明
	UpdateManual();

	if (g_Tutorialstate == TUTORIAL_NORMAL && (GetKeyTrigger(DIK_RETURN) || GetJoystickTrigger(PAD_START) || GetxControllerTrigger(XINPUT_PAD_START)))	//Enterで終了
	{
		PlaySound(SOUND_LABEL_SE_CAT);
		g_Tutorialstate = TUTORIAL_GETBOTTON;
		SetFade(MODE_GAME);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTutorial(void)
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

	//プレイヤーの描画
	DrawPlayer();

	//シリンダーの描画
	DrawMeshCylinder();

	//魔法の描画
	DrawWave();

	//エフェクトの描画
	DrawEffect();
	DrawTrajectory();

	//姫ライフの描画
	DrawPLife();

	//魔法ゲージの描画
	DrawMagicGage();

	//パーティクルの描画
	DrawParticle();

	//操作説明
	DrawManual();

	//プレススタートの描画
	DrawStart();
}