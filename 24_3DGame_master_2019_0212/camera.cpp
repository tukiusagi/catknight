//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "joystick.h"
#include "player.h"
#include"game.h"	

//=============================================================================
// マクロ定義
//=============================================================================
#define CAMERA_HEIGHT	(150.0f)	//高さ
#define CAMERA_DEPTH	(-200.0f)	//奥行
#define CAMERA_MOVE		(0.1f)		//移動量
#define CAMERA_INERTIA	(0.4f)		//慣性
#define CAMERA_ROLL		(0.03f)		//回転量
#define CAMERA_DEST		(5.0f)		//目的までの距離
#define CAMERA_ROTMOVE	(0.01f)		//係数

//=============================================================================
// グローバル変数
//=============================================================================
CAMERA g_camera;
int g_nCntMove;

//=============================================================================
// カメラの初期化処理
//=============================================================================
void InitCamera(void)
{
	g_camera.posV = D3DXVECTOR3(0.0f, CAMERA_HEIGHT, CAMERA_DEPTH);	//現在の視点
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//現在の注視点
	g_camera.posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//目的の視点
	g_camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//目的の注視点
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//上方向ベクトル
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//回転
	g_camera.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//目的の回転

	g_camera.fLength = sqrtf(((g_camera.posR.x - g_camera.posV.x)*(g_camera.posR.x - g_camera.posV.x)) + ((g_camera.posR.z - g_camera.posV.z)*(g_camera.posR.z - g_camera.posV.z)));

	g_nCntMove = 0;
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{
	
}

//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	if (*GetMode() == MODE_TITLE || *GetMode() == MODE_RANKING)
	{
		g_camera.posV = D3DXVECTOR3(0.0f, 130, -300);
		g_camera.posR = D3DXVECTOR3(0.0f, 30.0f, 0.0f);	
		g_camera.fLength = sqrtf(((g_camera.posR.x - g_camera.posV.x)*(g_camera.posR.x - g_camera.posV.x)) + ((g_camera.posR.z - g_camera.posV.z)*(g_camera.posR.z - g_camera.posV.z)));

		//視点右旋回
		g_camera.rot.y -= CAMERA_ROLL*0.1f;
		if (g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2;
		}
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fLength;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fLength;

	}
	else
	{
		//追従移動
		//目標注視点
		if (pPlayer->move.x < 0)
		{
			g_camera.posRDest.x = pPlayer->pos.x + sinf(pPlayer->rot.y) * CAMERA_DEST * pPlayer->move.x;
		}
		else
		{
			g_camera.posRDest.x = pPlayer->pos.x - sinf(pPlayer->rot.y) * CAMERA_DEST * pPlayer->move.x;
		}
		if (pPlayer->move.z < 0)
		{
			g_camera.posRDest.z = pPlayer->pos.z + cosf(pPlayer->rot.y) * CAMERA_DEST * pPlayer->move.z;
		}
		else
		{
			g_camera.posRDest.z = pPlayer->pos.z - cosf(pPlayer->rot.y) * CAMERA_DEST * pPlayer->move.z;
		}
		g_camera.posRDest.y = pPlayer->pos.y + 5;

		//目標視点
		g_camera.posVDest.x = pPlayer->pos.x - sinf(g_camera.rot.y) * g_camera.fLength;
		g_camera.posVDest.z = pPlayer->pos.z - cosf(g_camera.rot.y) * g_camera.fLength;
		g_camera.posVDest.y = pPlayer->pos.y + CAMERA_HEIGHT;


#ifdef _DEBUG
		if (GetKeyboardPress(DIK_C) || GetJoystickPress(PAD_ZR))
		{	//視点右旋回
			g_camera.rot.y -= CAMERA_ROLL;
			if (g_camera.rot.y < -D3DX_PI)
			{
				g_camera.rot.y += D3DX_PI * 2;
			}
			g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fLength;
			g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fLength;
		}
		else if (GetKeyboardPress(DIK_Z) || GetJoystickPress(PAD_ZL))
		{	//左旋回
			g_camera.rot.y += CAMERA_ROLL;
			if (g_camera.rot.y > D3DX_PI)
			{
				g_camera.rot.y -= D3DX_PI * 2;
			}
			g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fLength;
			g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fLength;
		}

#endif

		//注視点
		g_camera.posR.x += (g_camera.posRDest.x - g_camera.posR.x)*CAMERA_MOVE;
		g_camera.posR.z += (g_camera.posRDest.z - g_camera.posR.z)*CAMERA_MOVE;
		g_camera.posR.y += (g_camera.posRDest.y - g_camera.posR.y)*0.03f;


		//視点
		g_camera.posV.x += (g_camera.posVDest.x - g_camera.posV.x)*CAMERA_MOVE;
		g_camera.posV.z += (g_camera.posVDest.z - g_camera.posV.z)*CAMERA_MOVE;
		g_camera.posV.y += (g_camera.posVDest.y - g_camera.posV.y)*0.03f;
	}
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
								D3DXToRadian(45.0f),						//視野角
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	//アスペクト比
								10.0f,										//NearZ値
								10000.0f);									//FarZ値

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&g_camera.mtxView,
						&g_camera.posV,		//カメラの視点
						&g_camera.posR,		//カメラの注視点
						&g_camera.vecU);	//カメラの上方向ベクトル

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//=============================================================================
// カメラの取得
//=============================================================================
CAMERA *GetCamera(void)
{
	return&g_camera;
}
