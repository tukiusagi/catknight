//=============================================================================
//
// タイトル処理[tutorial.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"tutorial.h"	//インクルードファイル
#include "input.h"	
#include "joystick.h"
#include "fade.h"
#include"logo.h"
#include "sound.h"
#include"pressstart.h"
#include "camera.h"
#include "player.h"
#include "shadow.h"
#include "effect.h"
#include "princess.h"
#include "enemy.h"
#include "magic.h"
#include "wave.h"
#include "trajectory.h"
#include "meshfield.h"
#include "meshwall.h"
#include "meshcylinder.h"
#include "ground.h"
#include "light.h"
#include "particle.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TUTORIAL_NAME	"data/TEXTURE/チュートリアル.png"	//読み込むテクスチャ名
#define TUTORIAL_POS	(0.0f)
#define TUTORIAL_SIZE_X	(1280)
#define TUTORIAL_SIZE_Y	(209)

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexTutorial(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// チュートリアルの状態
//=============================================================================
typedef enum
{
	TUTORIAL_NORMAL,		//通常
	TUTORIAL_GETBOTTON,	//ボタンを押された
	TUTORIAL_MAX
} TUTORIAL;

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureTutorial = NULL;			//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;	//頂点バッファへのポインタ
TUTORIAL g_Tutorialstate;

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
void InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();	//デバイスの取得
	g_Tutorialstate = TUTORIAL_NORMAL;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TUTORIAL_NAME,
		&g_pTextureTutorial);

	MakeVertexTutorial(pDevice);

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

	//姫の初期化
	InitPrincess();

	// プレイヤーの初期化
	InitPlayer();

	//敵の初期化
	InitEnemy();
	SetEnemy(D3DXVECTOR3(0, 0.0f, 100), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100);

	//プレススタートの初期化
	InitStart();
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void UninitTutorial(void)
{
	//プレススタートの終了
	UninitStart();

	//エネミーの消去
	DeleteEnemey();

	//パーティクルの終了
	UninitParticle();

	//テクスチャの開放
	if (g_pTextureTutorial != NULL)
	{					  
		g_pTextureTutorial->Release();
		g_pTextureTutorial = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}

}

//=============================================================================
// ポリゴンの更新処理
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

	//プレススタートの更新
	UpdateStart();

	if (g_Tutorialstate == TUTORIAL_NORMAL && (GetKeyTrigger(DIK_RETURN) || GetJoystickTrigger(PAD_START)))	//Enterで終了
	{
		PlaySound(SOUND_LABEL_SE_CAT);
		g_Tutorialstate = TUTORIAL_GETBOTTON;
		SetFade(MODE_GAME);
	}
}

//=============================================================================
// ポリゴンの描画処理
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

	//パーティクルの描画
	DrawParticle();

	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);				//頂点フォーマットの設定

	pDevice->SetTexture(0, g_pTextureTutorial);	//テクスチャの設定

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,					//開始するインデックス
		2);					//プリミティブの数(ポリゴンの数)

//プレススタートの描画
	DrawStart();
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexTutorial(LPDIRECT3DDEVICE9 pDevice)
{
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	//確保するバッファサイズ(サイズ*頂点数)
		D3DUSAGE_WRITEONLY,		//固定
		FVF_VERTEX_2D,			//頂点フォーマット
		D3DPOOL_MANAGED,		//固定
		&g_pVtxBuffTutorial,
		NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

														//頂点の設定
	pVtx[0].pos = D3DXVECTOR3(TUTORIAL_POS, SCREEN_HEIGHT- TUTORIAL_SIZE_Y, 0.0f);	//ｚは0.0ｆで固定
	pVtx[1].pos = D3DXVECTOR3(TUTORIAL_POS + TUTORIAL_SIZE_X, SCREEN_HEIGHT - TUTORIAL_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TUTORIAL_POS, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TUTORIAL_POS + TUTORIAL_SIZE_X, SCREEN_HEIGHT, 0.0f);

	pVtx[0].rhw = 1.0f;	//1.0fで固定
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データをアンロック
	g_pVtxBuffTutorial->Unlock();
}