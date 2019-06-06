//=============================================================================
//
// タイトル処理[title.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"title.h"	//インクルードファイル
#include "input.h"	
#include "joystick.h"
#include "xController.h"

#include "fade.h"
#include "camera.h"
#include "light.h"
#include "meshfield.h"
#include "meshwall.h"
#include "shadow.h"
#include"logo.h"
#include "princess.h"
#include"pressstart.h"
#include "meshcylinder.h"
#include "ground.h"
#include "sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SECOND (60)	//秒数

//=============================================================================
// タイトルの状態
//=============================================================================
typedef enum
{
	TITLE_NORMAL,		//通常
	TITLE_GETBOTTON,	//ボタンを押された
	TITLE_MAX
} TITLE;

//=============================================================================
// グローバル変数
//=============================================================================
TITLE TitleState;
int g_nCntTitle;

//=============================================================================
// 初期化処理
//=============================================================================
void InitTitle(void)
{
	TitleState = TITLE_NORMAL;
	g_nCntTitle = 0;

	// カメラの初期化処理
	InitCamera();

	// ライトの初期化処理
	InitLight();

	//シリンダーの初期化
	InitMeshCylinder();
	SetMeshCylinder(D3DXVECTOR3(0.0f, 800.0f, 0.0f), D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f), 1000.0f, 1000.0f, 0);

	//地面の初期化
	InitGround();

	//フィールドの初期化
	InitMeshField();

	//メッシュ壁の初期化
	InitMeshWall();
	SetMeshWall(D3DXVECTOR3(0.0f, 0.0f, MESHFIELD_SIZE* 0.5f * MESHFIELD_D), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetMeshWall(D3DXVECTOR3(MESHFIELD_SIZE* 0.5f * MESHFIELD_W, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f));
	SetMeshWall(D3DXVECTOR3(-MESHFIELD_SIZE* 0.5f * MESHFIELD_W, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI / 2, 0.0f));
	SetMeshWall(D3DXVECTOR3(0.0f, 0.0f, -MESHFIELD_SIZE* 0.5f * MESHFIELD_D), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

	//影の初期化
	InitShadow();

	//姫の初期化
	InitPrincess();

	SetLogo(LOGO_TITLE);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	// カメラの終了処理
	UninitCamera();

	//ロゴの終了
	DeleteLogo();

	//プレススタートの終了
	UninitStart();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
	// カメラの更新処理
	UpdateCamera();

	// ライトの更新処理
	UpdateLight();

	//影の更新
	UpdateShadow();

	//シリンダーの更新
	UpdateMeshCylinder();

	//地面の更新
	UpdateGround();

	//フィールドの更新
	UpdateMeshField();

	//メッシュ壁の更新
	UpdateMeshWall();

	//姫の更新
	UpdatePrincess();

	//プレススタートの更新
	UpdateStart();

	g_nCntTitle++;
	if (TitleState == TITLE_NORMAL && (GetKeyTrigger(DIK_RETURN)||GetJoystickTrigger(PAD_START) || GetxControllerTrigger(XINPUT_PAD_START)))	//Enterで終了
	{
		PlaySound(SOUND_LABEL_SE_CAT);
		SetFade(MODE_TUTORIAL);
		TitleState = TITLE_GETBOTTON;
	}
	else if (g_nCntTitle > SECOND * 5)
	{
		SetFade(MODE_RANKING);
		g_nCntTitle = 0;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
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

	//影の描画
	DrawShadow();

	//姫の描画
	DrawPrincess();

	//プレススタートの描画
	DrawStart();
}