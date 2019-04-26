//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "joystick.h"
#include "camera.h"
#include "shadow.h"
#include "meshfield.h"
#include "line.h"
#include "princess.h"
#include "enemy.h"
#include"game.h"
#include "boss.h"
#include "life.h"
#include "effect.h"
#include "obstacle.h"
#include "trajectory.h"
#include "combo.h"
#include "sound.h"
#include "particle.h"
#include "wave.h"
#include "magic.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define PLAYER_TEXTURE	"data/TEXTURE/white.jpg"	//読み込むテクスチャ名
#define PLAYER_SHADOW		(15.0f)			//影の大きさ
#define PLAYER_ROTSPEAD		(0.1f)			//回転速度
#define PLAYER_KEY_MAX		(4)				//キーの最大数

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
	KEY	Key[PLAYER_MAX];	//キー要素[パーツ数]
} KEY_INFO;

//モーション情報
typedef struct
{
	bool		bLoop;		//ループの有無
	int			nNumKey;	//キー数
	KEY_INFO	KeyInfo[PLAYER_KEY_MAX];	//キー情報[キーの数]
} MOTION_INFO;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
#ifdef _DEBUG
void DrawPos(void);
#endif

//=============================================================================
// グローバル変数
//=============================================================================
PLAYER				g_Player;			//プレイヤー情報
COLLISION			g_Collision;		//当たり判定情報
MESH_ORBIT			g_Mesh;
LPDIRECT3DTEXTURE9	g_pTexture[6] = {};	//テクスチャのポインタ
int	g_nIdxShadow;		//影のID
int g_NumModel = 0;		//読み込むモデル数
int g_nNumParts = 0;	//読み込むパーツ数
int g_nCntframe = 0;	//現在のフレーム数
int g_nCntKey = 0;		//現在のキー数
char acModelFileName[PLAYER_MAX][MAX_CHARA];	//モデルファイル名格納用

#ifdef _DEBUG
LPD3DXFONT			g_pFontPL = NULL;			// フォントへのポインタ
D3DXVECTOR3			g_PLpos;
#endif

MOTION_INFO	g_aMotionInfo[PLAYERSTATE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ファイル読み込み
	LoadPlayer();

	// 位置・向きの初期設定
	g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, -40.0f);
	g_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.nLife = g_Player.nMaxLife;
	g_Player.state = PLAYERSTATE_NEUTRAL;
	g_Player.bAttack = false;
	g_Player.bDamege = false;
	g_Player.nCntState = 0;

	g_Collision.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Collision.rot= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//影の作成
	g_nIdxShadow = SetSadow(g_Player.pos, g_Player.rot, PLAYER_SHADOW, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));

	// Xファイルの読み込み
	for (int nCntModel = 0; nCntModel < g_NumModel; nCntModel++)
	{
		D3DXLoadMeshFromX(g_Player.aModel[nCntModel].pFilename,	//読み込むファイル名
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_Player.aModel[nCntModel].pBuffMat,
			NULL,
			&g_Player.aModel[nCntModel].nNumMat,
			&g_Player.aModel[nCntModel].pMesh);
	}


	//テクスチャの読み込み
	for (int nCntTexture = 0; nCntTexture < 6; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice,
			PLAYER_TEXTURE,
			&g_pTexture[nCntTexture]);
	}

	//軌跡の設定
	SetTrajectory(g_Mesh.pos[0], g_Mesh.pos[1], &g_Player.aModel[g_Mesh.nParent].mtxWorld, D3DXCOLOR(1.0f,0.5f,0.0f,1.0f));

#ifdef _DEBUG
	// デバッグ表示用フォントを設定
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "ＭＳ ゴシック", &g_pFontPL);
#endif

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < PLAYER_MAX; nCntPlayer++)
	{
		// メッシュの開放
		if (g_Player.aModel[nCntPlayer].pMesh != NULL)
		{
			g_Player.aModel[nCntPlayer].pMesh->Release();
			g_Player.aModel[nCntPlayer].pMesh = NULL;
		}

		// マテリアルの開放
		if (g_Player.aModel[nCntPlayer].pBuffMat != NULL)
		{
			g_Player.aModel[nCntPlayer].pBuffMat->Release();
			g_Player.aModel[nCntPlayer].pBuffMat = NULL;
		}
	}

	//テクスチャの開放
	for (int nCntTex = 0; nCntTex < 6; nCntTex++)
	{
		if (g_pTexture[nCntTex] != NULL)
		{
			g_pTexture[nCntTex]->Release();
			g_pTexture[nCntTex] = NULL;
		}
	}
#ifdef _DEBUG
	if (g_pFontPL != NULL)
	{// デバッグ表示用フォントの開放
		g_pFontPL->Release();
		g_pFontPL = NULL;
	}
#endif

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *pCamera;
	pCamera = GetCamera();

	g_Player.posOld = g_Player.pos;
	g_Collision.posOld = g_Collision.pos;

	if (g_Player.bDamege)
	{
		g_Player.nCntState++;
		if (g_Player.nCntState % 10 == 0)
		{	//ダメージ状態解除
			g_Player.bDamege = false;
		}
	}

	//フレーム増加
	if (g_Player.state == PLAYERSTATE_DETH && g_nCntKey >= 1)
	{
		//死亡時は増加しない
	}
	else
	{
		g_nCntframe++;
		if (g_nCntframe > g_aMotionInfo[g_Player.state].KeyInfo[g_nCntKey].nFrame)
		{
			g_nCntframe = 0;
			g_nCntKey++;
			if (g_nCntKey >= g_aMotionInfo[g_Player.state].nNumKey)
			{
				g_nCntKey = 0;
			}
		}
	}

	//モーションの切り替え
	switch (*GetGameState())
	{
	case GAMESTATE_CLEAR:
		if (g_Player.state != PLAYERSTATE_ATTACK01 && g_Player.state != PLAYERSTATE_ATTACK02)
		{
			g_Player.state = PLAYERSTATE_WIN;
		}
		break;
	case GAMESTATE_GAMEOVER:
		if (g_Player.state != PLAYERSTATE_DETH)
		{
			g_Player.state = PLAYERSTATE_DETH;
			g_nCntframe = 0;
			g_nCntKey = 0;
		}
		break;
	default:
		if (g_Player.state != PLAYERSTATE_ATTACK01 && g_Player.state != PLAYERSTATE_ATTACK02 && g_Player.state != PLAYERSTATE_HIT)
		{
			if (g_Player.move.x > 0.5f || g_Player.move.z > 0.5f ||
				g_Player.move.x < -0.5f || g_Player.move.z < -0.5f)
			{
				g_Player.state = PLAYERSTATE_MOVE;
			}
			else
			{
				g_Player.state = PLAYERSTATE_NEUTRAL;
			}
		}
		break;
	}

	if (g_Player.state != PLAYERSTATE_DETH)
	{
		if (g_Player.state != PLAYERSTATE_ATTACK01 && g_Player.state != PLAYERSTATE_ATTACK02)
		{
			//任意のキーが押されたかどうか
			if (GetKeyboardPress(DIK_A) || GetJoystickPress(PAD_LEFT_L))
			{
				g_Player.state = PLAYERSTATE_MOVE;
				if (GetKeyboardPress(DIK_W) || GetJoystickPress(PAD_UP_L))
				{	//左上
					g_Player.move.x += sinf(-D3DX_PI * 0.75f - pCamera->rot.y) * g_Player.fSpeed;	//加速値
					g_Player.move.z -= cosf(-D3DX_PI * 0.75f - pCamera->rot.y) * g_Player.fSpeed;
					g_Player.rotDest.y = D3DX_PI * 0.75f + pCamera->rot.y;
				}
				else if (GetKeyboardPress(DIK_S) || GetJoystickPress(PAD_DOWN_L))
				{	//左下
					g_Player.move.x += sinf(-D3DX_PI * 0.25f - pCamera->rot.y) * g_Player.fSpeed;
					g_Player.move.z -= cosf(-D3DX_PI * 0.25f - pCamera->rot.y) * g_Player.fSpeed;
					g_Player.rotDest.y = D3DX_PI * 0.25f + pCamera->rot.y;
				}
				else
				{	//左
					g_Player.move.x += sinf(-D3DX_PI * 0.5f + pCamera->rot.y) * g_Player.fSpeed;
					g_Player.move.z += cosf(-D3DX_PI * 0.5f + pCamera->rot.y) * g_Player.fSpeed;
					g_Player.rotDest.y = D3DX_PI * 0.5f + pCamera->rot.y;
				}
			}
			else if (GetKeyboardPress(DIK_D) || GetJoystickPress(PAD_RIGHT_L))
			{
				g_Player.state = PLAYERSTATE_MOVE;

				if (GetKeyboardPress(DIK_W) || GetJoystickPress(PAD_UP_L))
				{	//右上
					g_Player.move.x += sinf(D3DX_PI * 0.75f - pCamera->rot.y) * g_Player.fSpeed;
					g_Player.move.z -= cosf(D3DX_PI * 0.75f - pCamera->rot.y) * g_Player.fSpeed;
					g_Player.rotDest.y = -D3DX_PI * 0.75f + pCamera->rot.y;
				}
				else if (GetKeyboardPress(DIK_S) || GetJoystickPress(PAD_DOWN_L))
				{	//右下
					g_Player.move.x += sinf(D3DX_PI * 0.25f - pCamera->rot.y) * g_Player.fSpeed;
					g_Player.move.z -= cosf(D3DX_PI * 0.25f - pCamera->rot.y) * g_Player.fSpeed;
					g_Player.rotDest.y = -D3DX_PI * 0.25f + pCamera->rot.y;
				}
				else
				{	//右
					g_Player.move.x += sinf(D3DX_PI * 0.5f + pCamera->rot.y) * g_Player.fSpeed;
					g_Player.move.z += cosf(D3DX_PI * 0.5f + pCamera->rot.y) * g_Player.fSpeed;
					g_Player.rotDest.y = -D3DX_PI * 0.5f + pCamera->rot.y;
				}
			}
			else if (GetKeyboardPress(DIK_W) || GetJoystickPress(PAD_UP_L))
			{	//奥
				g_Player.state = PLAYERSTATE_MOVE;

				g_Player.move.x -= sinf(D3DX_PI * 1.0f + pCamera->rot.y) * g_Player.fSpeed;
				g_Player.move.z -= cosf(D3DX_PI * 1.0f + pCamera->rot.y) * g_Player.fSpeed;
				g_Player.rotDest.y = D3DX_PI * 1.0f + pCamera->rot.y;
			}
			else if (GetKeyboardPress(DIK_S) || GetJoystickPress(PAD_DOWN_L))
			{	//手前
				g_Player.state = PLAYERSTATE_MOVE;

				g_Player.move.x -= sinf(D3DX_PI * 0.0f + pCamera->rot.y) * g_Player.fSpeed;
				g_Player.move.z -= cosf(D3DX_PI * 0.0f + pCamera->rot.y) * g_Player.fSpeed;
				g_Player.rotDest.y = D3DX_PI * 0.0f + pCamera->rot.y;
			}
		}
		if (g_Player.state != PLAYERSTATE_WIN && (GetKeyTrigger(DIK_SPACE)|| GetJoystickTrigger(PAD_B)))
		{	//攻撃
			if (g_Player.state == PLAYERSTATE_ATTACK01)
			{	//二段階目
				g_Player.state = PLAYERSTATE_ATTACK02;
				g_nCntframe = 0;
				g_nCntKey = 0;
			}
			if (g_Player.state == PLAYERSTATE_ATTACK02 && g_nCntKey >= 1)
			{	//一段階目に戻る
				g_Player.state = PLAYERSTATE_ATTACK01;
				g_nCntframe = 0;
				g_nCntKey = 0;
			}
			if ( g_Player.state != PLAYERSTATE_ATTACK01 && g_Player.state != PLAYERSTATE_ATTACK02)
			{	//一段階
				g_Player.state = PLAYERSTATE_ATTACK01;
				g_nCntframe = 0;
				g_nCntKey = 0;
			}

			PlaySound(SOUND_LABEL_SE_SWORD);
			g_Player.bAttack = false;
		}
	}
#ifdef _DEBUG
	if (GetKeyTrigger(DIK_J))
	{	// ジャンプ
		g_Player.move.y += 10.0f;
	}
#endif

	//回転
	if (g_Player.rotmove.y = g_Player.rotDest.y - g_Player.rot.y)
	{
		if (g_Player.rotmove.y < -D3DX_PI)
		{
			g_Player.rotmove.y += D3DX_PI * 2;
		}
		else if (g_Player.rotmove.y > D3DX_PI)
		{
			g_Player.rotmove.y -= D3DX_PI * 2;
		}
	}
	if (g_Player.rot.y += g_Player.rotmove.y * PLAYER_ROTSPEAD)
	{
		if (g_Player.rot.y > D3DX_PI)
		{
			g_Player.rot.y -= D3DX_PI * 2;
		}
		else if (g_Player.rot.y < -D3DX_PI)
		{
			g_Player.rot.y += D3DX_PI * 2;
		}
	}

	//重力
	g_Player.move.y -= 0.8f;

	//慣性
	g_Player.move.x += (0.0f - g_Player.move.x)*0.25f;
	g_Player.move.z += (0.0f - g_Player.move.z)*0.25f;

	//移動
	g_Player.pos.x += g_Player.move.x;
	g_Player.pos.z += g_Player.move.z;
	g_Player.pos.y += g_Player.move.y;

	//当たり判定
	ENEMY *pEnemy;
	pEnemy = GetEnemy();
	float fCollision = 5.0f;
	CollisionPrincess(&g_Player.pos, &g_Player.posOld, g_Player.fRadius, 0);

	OBSTACLE *pObstacle = GetObstacle();
	//障害物
	for (int nCntObstacle = 0; nCntObstacle < OBSTACLE_MAX; nCntObstacle++, pObstacle++)
	{
		if (pObstacle->bUse)
		{
			CollisionObstacle(&g_Player.pos, &g_Player.posOld, &g_Player.move, g_Player.fRadius, nCntObstacle);
		}
	}

	BOSS *pBoss;
	pBoss = GetBoss();
	//ボス
	for (int nCntBoss = 0; nCntBoss < MAX_ENEMY; nCntBoss++, pBoss++)
	{
		if (pBoss->bUse)
		{
			if ((g_Player.state == PLAYERSTATE_ATTACK01 || g_Player.state == PLAYERSTATE_ATTACK02) &&
				(!pBoss->bDamege && pBoss->state != BOSSSTATE_DETH) &&
				(g_Collision.nStartFrame <= g_nCntframe) && (g_nCntframe <= g_Collision.nEndFrame))
			{
				if (CollisionBoss(&D3DXVECTOR3(g_Collision.mtxWorld._41, g_Collision.mtxWorld._42, g_Collision.mtxWorld._43), &g_Collision.posOld, g_Collision.fRadius,nCntBoss))
				{
					//ボス体力減少
					HitBoss(1, &g_Player.pos,nCntBoss);
					AddCombo(1);	//コンボ増加
					g_Player.bAttack = true;
				}
			}
		}
	}
	//敵
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse)
		{
			if ((g_Player.state == PLAYERSTATE_ATTACK01 || g_Player.state == PLAYERSTATE_ATTACK02) &&
				(pEnemy->state != ENEMYSTATE_DETH) &&
				(g_Collision.nStartFrame <= g_nCntframe) && (g_nCntframe <= g_Collision.nEndFrame))
			{
				if (CollisionEnemy(&D3DXVECTOR3(g_Collision.mtxWorld._41, g_Collision.mtxWorld._42, g_Collision.mtxWorld._43), &g_Collision.posOld, g_Collision.fRadius, nCntEnemy))
				{	//敵体力減少
					HitEnemy(nCntEnemy, 1, &g_Player.pos);
					if (*GetGameState() == GAMESTATE_NORMAL)
					{
						AddCombo(1);	//コンボ増加
					}
					g_Player.bAttack = true;
				}
			}
		}
	}

	if (g_Player.pos.y < 0)
	{	//地面にめり込まないように
		g_Player.pos.y = 0;
		g_Player.move.y = 0;
	}
	//左右壁判定
	if (g_Player.pos.x > MESHFIELD_SIZE * MESHFIELD_W * 0.5f - g_Player.fRadius)
	{
		g_Player.pos.x = MESHFIELD_SIZE * MESHFIELD_W * 0.5f - g_Player.fRadius;
	}
	if (g_Player.pos.x < -MESHFIELD_SIZE * MESHFIELD_W * 0.5f + g_Player.fRadius)
	{
		g_Player.pos.x = -MESHFIELD_SIZE * MESHFIELD_W * 0.5f + g_Player.fRadius;
	}
	//奥手前壁判定
	if (g_Player.pos.z > MESHFIELD_SIZE * MESHFIELD_D * 0.5f - g_Player.fRadius)
	{
		g_Player.pos.z = MESHFIELD_SIZE * MESHFIELD_D * 0.5f - g_Player.fRadius;
	}
	if (g_Player.pos.z < -MESHFIELD_SIZE * MESHFIELD_D * 0.5f + g_Player.fRadius)
	{
		g_Player.pos.z = -MESHFIELD_SIZE * MESHFIELD_D * 0.5f + g_Player.fRadius;
	}

	//モーション再生
	if (g_Player.state == PLAYERSTATE_DETH && g_nCntKey >= 1)
	{

	}
	else
	{
		PlayMotion();
	}

	//影の位置を設定
	SetPositionSadow(g_nIdxShadow, g_Player.pos, PLAYER_SHADOW + ((0.0f - g_Player.pos.y)*-0.1f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f - ((0.0f - g_Player.pos.y)*-0.008f)));

#ifdef _DEBUG
	g_PLpos = g_Player.pos;
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans, mtxParent;	// 計算用マトリックス
	D3DXMATERIAL		*pMat;	//現在のマテリアル保存用
	D3DMATERIAL9		matDef;	//マテリアルデータへのポインタ
	D3DMATERIAL9		matKeep;	//保存用

//プレイヤーの設定
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Player.mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);

	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);

	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Player.mtxWorld);

	//モデルの設定
	for (int nCntPlayer = 0; nCntPlayer < g_nNumParts; nCntPlayer++)
	{
		//親の識別
		if (g_Player.aModel[nCntPlayer].nIdxModelParent == -1)
		{
			mtxParent = g_Player.mtxWorld;
		}
		else
		{
			mtxParent = g_Player.aModel[g_Player.aModel[nCntPlayer].nIdxModelParent].mtxWorld;
		}
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Player.aModel[nCntPlayer].mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.aModel[nCntPlayer].rot.y, g_Player.aModel[nCntPlayer].rot.x, g_Player.aModel[nCntPlayer].rot.z);

		D3DXMatrixMultiply(&g_Player.aModel[nCntPlayer].mtxWorld, &g_Player.aModel[nCntPlayer].mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Player.aModel[nCntPlayer].pos.x, g_Player.aModel[nCntPlayer].pos.y, g_Player.aModel[nCntPlayer].pos.z);

		D3DXMatrixMultiply(&g_Player.aModel[nCntPlayer].mtxWorld, &g_Player.aModel[nCntPlayer].mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

		//親子反映
		D3DXMatrixMultiply(&g_Player.aModel[nCntPlayer].mtxWorld, &g_Player.aModel[nCntPlayer].mtxWorld, &mtxParent);	//ワールド行列＝ワールド行列＊親のワールド行列

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Player.aModel[nCntPlayer].mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_Player.aModel[nCntPlayer].pBuffMat->GetBufferPointer();

		// テクスチャの設定をするならここ(1枚の時)

		for (int nCntMat = 0; nCntMat < (int)g_Player.aModel[nCntPlayer].nNumMat; nCntMat++)
		{
			// テクスチャの設定をするならここ(複数枚の時)
			pDevice->SetTexture(0, g_pTexture[nCntMat]);

			pMat[nCntMat].MatD3D.Ambient.r = pMat[nCntMat].MatD3D.Diffuse.r / 10;
			pMat[nCntMat].MatD3D.Ambient.g = pMat[nCntMat].MatD3D.Diffuse.g / 10;
			pMat[nCntMat].MatD3D.Ambient.b = pMat[nCntMat].MatD3D.Diffuse.b / 10;


			matKeep = pMat[nCntMat].MatD3D;	//保存

			if (g_Player.bDamege)
			{	//ダメージ状態で赤色に変更
				pMat[nCntMat].MatD3D.Diffuse.r = 255;
			}

			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			pMat[nCntMat].MatD3D = matKeep;	//色を戻す

			// 描画
			g_Player.aModel[nCntPlayer].pMesh->DrawSubset(nCntMat);	//格子状態
		}

		// マテリアルをデフォルトに戻す
		pDevice->SetMaterial(&matDef);
	}
//当たり判定の設定
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Collision.mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Collision.rot.y, g_Collision.rot.x, g_Collision.rot.z);

	D3DXMatrixMultiply(&g_Collision.mtxWorld, &g_Collision.mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Collision.pos.x, g_Collision.pos.y, g_Collision.pos.z);

	D3DXMatrixMultiply(&g_Collision.mtxWorld, &g_Collision.mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

	//親子反映
	D3DXMatrixMultiply(&g_Collision.mtxWorld, &g_Collision.mtxWorld, &g_Player.aModel[g_Collision.nIdxModelParent].mtxWorld);	//ワールド行列＝ワールド行列＊親のワールド行列

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Collision.mtxWorld);

#ifdef _DEBUG
	// PLpos表示
	DrawPos();
#endif

}

//=============================================================================
// プレイヤーのセット
//=============================================================================
void SetPlayer(void)
{
	g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, -40.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.nLife = g_Player.nMaxLife;
	g_Player.state = PLAYERSTATE_NEUTRAL;

}

//=============================================================================
// プレイヤー情報の取得
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

//=============================================================================
// 当たり判定
//=============================================================================
bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Radius)
{
	bool bLand = false;
	float fDistance = (pPos->x - g_Player.pos.x)*(pPos->x - g_Player.pos.x) + (pPos->z - g_Player.pos.z)*(pPos->z - g_Player.pos.z);	//2点間の距離
	float fLength = (Radius + g_Player.fRadius)*(Radius + g_Player.fRadius);	//半径
	float fDiff = (Radius + g_Player.fRadius) - (float)sqrt(fDistance);	//差
	D3DXVECTOR3 vec = D3DXVECTOR3((pPos->x - g_Player.pos.x), 0.0f, (pPos->z - g_Player.pos.z));	//ベクトル
	D3DXVec3Normalize(&vec, &vec);

	if (fDistance <= fLength)
	{
		bLand = true;

		pPos->x = pPos->x + vec.x * fDiff;
		pPos->z = pPos->z + vec.z * fDiff;
	}

	return bLand;
}

//=============================================================================
// ダメージ処理
//=============================================================================
void HitPlayer(int Damege, D3DXVECTOR3 *pPos)
{
	D3DXVECTOR3 vec = D3DXVECTOR3((g_Player.pos.x - pPos->x), 0.0f, (g_Player.pos.z - pPos->z));	//ベクトル
	D3DXVec3Normalize(&vec, &vec);

	DecreaseLife(1);
	//ヒットエフェクト
	float fAngle = 0;
	float fSpeed = 5;
	for (int nCnt = 0; nCnt < 20; nCnt++)
	{
		fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;	//-3.14～3.14の範囲
		D3DXVECTOR3 move = D3DXVECTOR3(sinf(fAngle)*fSpeed, cosf(fAngle)*fSpeed, 0.0f);
		SetParticle(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y + 20, g_Player.pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, 10, 10);
	}
	PlaySound(SOUND_LABEL_SE_DAMAGE);

	if (g_Player.state != PLAYERSTATE_DETH)
	{	//通常ヒット
		g_Player.state = PLAYERSTATE_HIT;
		g_nCntframe = 0;
		g_nCntKey = 0;
		g_Player.bDamege = true;
		g_Player.move.x = g_Player.move.x + vec.x * 10;
		g_Player.move.z = g_Player.move.z + vec.z * 10;
	}

	if (g_Player.nLife <= 0)
	{	//死亡時
		g_Player.state = PLAYERSTATE_DETH;
		g_nCntframe = 0;
		g_nCntKey = 0;

		g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, -30.0f);
		g_Player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player.nLife = g_Player.nMaxLife;
		IncreaseLife(g_Player.nMaxLife);
	}

}

//=============================================================================
//	モーションの再生
//=============================================================================
void PlayMotion(void)
{
	//パーツ数分
	for (int nCntMotion = 0; nCntMotion < PLAYER_MAX; nCntMotion++)
	{
		KEY *pKey, *pNextKey;
		int nNextKey = g_nCntKey + 1;

		//ループするかどうか
		if (nNextKey >= g_aMotionInfo[g_Player.state].nNumKey)
		{
			if (g_aMotionInfo[g_Player.state].bLoop)
			{
				nNextKey = 0;
			}
			else
			{
				if (!g_Player.bAttack && *GetGameState() == GAMESTATE_NORMAL)
				{
					DeleteCombo();	//コンボの途切れ
				}
				g_Player.state = PLAYERSTATE_NEUTRAL;
				g_nCntframe = 0;
				g_nCntKey = 0;
			}
		}

		pKey = &g_aMotionInfo[g_Player.state].KeyInfo[g_nCntKey].Key[nCntMotion];		//現在のキー
		pNextKey = &g_aMotionInfo[g_Player.state].KeyInfo[nNextKey].Key[nCntMotion];	//次のキー

		//回転
		g_Player.aModel[nCntMotion].rotMove = (pNextKey->rot - pKey->rot)*((float)g_nCntframe / (float)g_aMotionInfo[g_Player.state].KeyInfo[g_nCntKey].nFrame);

		g_Player.aModel[nCntMotion].rot = pKey->rot + g_Player.aModel[nCntMotion].rotMove;
	}
}

#ifdef _DEBUG
//=============================================================================
// pos表示処理
//=============================================================================
void DrawPos(void)
{
	RECT rect = { 0, 50, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "pos：%3.2f %3.2f %3.2f\n", g_PLpos.x, g_PLpos.y, g_PLpos.z);

	// テキスト描画
	g_pFontPL->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}
#endif

//=============================================================================
//	ファイル読み込み
//=============================================================================
void LoadPlayer(void)
{
	//ファイルポイント
	FILE *pFile;

	//変数宣言
	char *pStrCur = NULL;	//文字列の先頭へのポインタ
	char cLine[MAX_CHARA];	//文字列読み込み用
	char cStr[MAX_CHARA];	//文字列抜き出し用
	char *cPunctuation;		//文字区切り用
	int nCnt = 0;			//空白カウンター
	int nCntModel = 0;		//モデル番号
	int nCntMotion = 0;		//モーション番号
	int nCntKey = 0;		//現在キー
	int nCntParts;			//パーツ番号

	pFile = fopen("data/motion_Cat.txt", "r");

	//開けた
	if (pFile != NULL)
	{
		while (memcmp(cStr, "SCRIPT", strlen("SCRIPT")) != 0)	//指定の文字列を見つけるまで読み込む
		{
			//テキスト1行読み込むのに↓3行1セットが必要
			fgets(cLine, MAX_CHARA, pFile);	//1行256文字読み込み
			pStrCur = &cLine[0];			//文字数整理
			strcpy(cStr, pStrCur);			//pStrCurで読み取った文字をcStrに格納
		}
		while (memcmp(cStr, "END_SCRIPT", strlen("END_SCRIPT")) != 0)
		{
			fgets(cLine, MAX_CHARA, pFile);
			pStrCur = &cLine[0];
			strcpy(cStr, pStrCur);
		//読み込むモデル数
			if (memcmp(cStr, "NUM_MODEL = ", strlen("NUM_MODEL = ")) == 0)
			{
				pStrCur += strlen("NUM_MODEL = ");	//文字列の自動計算
				strcpy(cStr, pStrCur);
				g_NumModel = atoi(cStr);	//atoi　char型をint型に変換する
			}

		//モデルファイル名の読み込み
			if (memcmp(cStr, "MODEL_FILENAME = ", strlen("MODEL_FILENAME = ")) == 0)
			{
				for (int nCntStr = 0; nCntStr < MAX_CHARA; nCntStr++)
				{
					if (cStr[nCntStr] == '\t')	//TABを見つけたらそこで文字列を切る
					{
						cStr[nCntStr] = '\0';
						strcpy(pStrCur, cStr);
					}
				}
				pStrCur += strlen("MODEL_FILENAME = ");
				strcpy(acModelFileName[nCntModel], pStrCur);
				g_Player.aModel[nCntModel].pFilename = acModelFileName[nCntModel];
				nCntModel++;
			}
		//キャラクター情報
			if (memcmp(cStr, "CHARACTERSET", strlen("CHARACTERSET")) == 0)
			{
				while (memcmp(cStr, "END_CHARACTERSET", strlen("END_CHARACTERSET")) != 0)
				{
					fgets(cLine, MAX_CHARA, pFile);
					nCnt = 0;
					while (1)
					{	//文字の前に存在する空白やタブをカットする
						if (cLine[nCnt] == ' ' || cLine[nCnt] == '\t')
						{
							nCnt++;
						}
						else
						{
							break;
						}
					}
					pStrCur = &cLine[nCnt];
					strcpy(cStr, pStrCur);

					if (memcmp(&cLine[nCnt], "MOVE = ", strlen("MOVE = ")) == 0)
					{	//移動速度
						pStrCur += strlen("MOVE = ");
						strcpy(cStr, pStrCur);
						g_Player.fSpeed = (float)atof(cStr);	//atof　char型をfloat型に変換する
					}
					if (memcmp(&cLine[nCnt], "RADIUS = ", strlen("RADIUS = ")) == 0)
					{	//半径
						pStrCur += strlen("RADIUS = ");
						strcpy(cStr, pStrCur);
						g_Player.fRadius = (float)atof(cStr);
					}
					if (memcmp(&cLine[nCnt], "LIFE = ", strlen("LIFE = ")) == 0)
					{	//体力
						pStrCur += strlen("LIFE = ");
						strcpy(cStr, pStrCur);
						g_Player.nMaxLife = atoi(cStr);
					}
					if (memcmp(&cLine[nCnt], "NUM_PARTS = ", strlen("NUM_PARTS = ")) == 0)
					{	//読み込むパーツ数
						pStrCur += strlen("NUM_PARTS = ");
						strcpy(cStr, pStrCur);
						g_nNumParts = atoi(cStr);
					}
				//キャラパーツ情報
					if (memcmp(&cLine[nCnt], "PARTSSET", strlen("PARTSSET")) == 0)
					{
						while (memcmp(&cLine[nCnt], "END_PARTSSET", strlen("END_PARTSSET")) != 0)
						{
							fgets(cLine, MAX_CHARA, pFile);
							nCnt = 0;
							while (1)
							{
								if (cLine[nCnt] == ' ' || cLine[nCnt] == '\t')
								{
									nCnt++;
								}
								else
								{
									break;
								}
							}
							pStrCur = &cLine[nCnt];
							strcpy(cStr, pStrCur);

							if (memcmp(&cLine[nCnt], "INDEX = ", strlen("INDEX = ")) == 0)
							{	//これは何番のパーツか
								pStrCur += strlen("INDEX = ");
								strcpy(cStr, pStrCur);
								nCntParts = atoi(cStr);
							}
							if (memcmp(&cLine[nCnt], "PARENT = ", strlen("PARENT = ")) == 0)
							{	//親の番号
								pStrCur += strlen("PARENT = ");
								strcpy(cStr, pStrCur);
								g_Player.aModel[nCntParts].nIdxModelParent = atoi(cStr);
							}
							if (memcmp(&cLine[nCnt], "POS = ", strlen("POS = ")) == 0)
							{	//位置
								pStrCur += strlen("POS = ");
								strcpy(cStr, pStrCur);

								cPunctuation = strtok(cStr, " ");	//文字を空白ごとに区切って分解
								g_Player.aModel[nCntParts].pos.x = (float)atof(cPunctuation);

								cPunctuation = strtok(NULL, " ");
								g_Player.aModel[nCntParts].pos.y = (float)atof(cPunctuation);

								cPunctuation = strtok(NULL, " ");
								g_Player.aModel[nCntParts].pos.z = (float)atof(cPunctuation);
							}
							if (memcmp(&cLine[nCnt], "ROT = ", strlen("ROT = ")) == 0)
							{	//角度
								pStrCur += strlen("ROT = ");
								strcpy(cStr, pStrCur);
								cPunctuation = strtok(cStr, " ");
								g_Player.aModel[nCntParts].rot.x = (float)atof(cPunctuation);
								cPunctuation = strtok(NULL, " ");
								g_Player.aModel[nCntParts].rot.y = (float)atof(cPunctuation);
								cPunctuation = strtok(NULL, " ");
								g_Player.aModel[nCntParts].rot.z = (float)atof(cPunctuation);
							}
						}
					}
				}
			}
		//モーション情報
			if (memcmp(cStr, "MOTIONSET", strlen("MOTIONSET")) == 0)
			{
				nCntKey = 0;	//キーカウンターリセット
				while (memcmp(&cLine[nCnt], "END_MOTIONSET", strlen("END_MOTIONSET")) != 0)
				{
					fgets(cLine, MAX_CHARA, pFile);
					nCnt = 0;
					while (1)
					{
						if (cLine[nCnt] == ' ' || cLine[nCnt] == '\t')
						{
							nCnt++;
						}
						else
						{
							break;
						}
					}
					pStrCur = &cLine[nCnt];
					strcpy(cStr, pStrCur);

					if (memcmp(&cLine[nCnt], "LOOP = ", strlen("LOOP = ")) == 0)
					{	//ループの有無
						pStrCur += strlen("LOOP = ");
						strcpy(cStr, pStrCur);
						g_aMotionInfo[nCntMotion].bLoop = atoi(cStr);
					}
					if (memcmp(&cLine[nCnt], "NUM_KEY = ", strlen("NUM_KEY = ")) == 0)
					{	//キー数
						pStrCur += strlen("NUM_KEY = ");
						strcpy(cStr, pStrCur);
						g_aMotionInfo[nCntMotion].nNumKey = atoi(cStr);
					}
					if (memcmp(&cLine[nCnt], "COLLISION = ", strlen("COLLISION = ")) == 0)
					{	//当たり判定
						pStrCur += strlen("COLLISION = ");
						strcpy(cStr, pStrCur);

						//親の番号
						cPunctuation = strtok(cStr, " ");
						g_Collision.nIdxModelParent = atoi(cPunctuation);
						//オフセット値
						//X
						cPunctuation = strtok(NULL, " ");
						g_Collision.pos.x = (float)atof(cPunctuation);	
						//Y
						cPunctuation = strtok(NULL, " ");
						g_Collision.pos.y = (float)atof(cPunctuation);	
						//Z
						cPunctuation = strtok(NULL, " ");
						g_Collision.pos.z = (float)atof(cPunctuation);	
						//半径
						cPunctuation = strtok(NULL, " ");
						g_Collision.fRadius = (float)atof(cPunctuation);
						//当たり判定を開始するフレーム数
						cPunctuation = strtok(NULL, " ");
						g_Collision.nStartFrame = atoi(cPunctuation);
						//終了するフレーム数
						cPunctuation = strtok(NULL, " ");
						g_Collision.nEndFrame = atoi(cPunctuation);
					}
					if (memcmp(&cLine[nCnt], "ORBIT = ", strlen("ORBIT = ")) == 0)
					{	//軌跡
						pStrCur += strlen("ORBIT = ");
						strcpy(cStr, pStrCur);

						//親の番号
						cPunctuation = strtok(cStr, " ");
						g_Mesh.nParent = atoi(cPunctuation);
						//オフセット値
						//X
						cPunctuation = strtok(NULL, " ");
						g_Mesh.pos[0].x = (float)atof(cPunctuation);
						//Y
						cPunctuation = strtok(NULL, " ");
						g_Mesh.pos[0].y = (float)atof(cPunctuation);
						//Z
						cPunctuation = strtok(NULL, " ");
						g_Mesh.pos[0].z = (float)atof(cPunctuation);
					}

				//キー情報
					if (memcmp(&cLine[nCnt], "KEYSET", strlen("KEYSET")) == 0)
					{
						nCntParts = 0;	//パーツカウンターリセット
						while (memcmp(&cLine[nCnt], "END_KEYSET", strlen("END_KEYSET")) != 0)
						{
							fgets(cLine, MAX_CHARA, pFile);
							nCnt = 0;
							while (1)
							{
								if (cLine[nCnt] == ' ' || cLine[nCnt] == '\t')
								{
									nCnt++;
								}
								else
								{
									break;
								}
							}
							pStrCur = &cLine[nCnt];
							strcpy(cStr, pStrCur);

							if (memcmp(&cLine[nCnt], "FRAME = ", strlen("FRAME = ")) == 0)
							{	//フレーム数
								pStrCur += strlen("FRAME = ");
								strcpy(cStr, pStrCur);
								g_aMotionInfo[nCntMotion].KeyInfo[nCntKey].nFrame = atoi(cStr);
							}
						//キー要素
							if (memcmp(&cLine[nCnt], "KEY", strlen("KEY")) == 0)
							{
								while (memcmp(&cLine[nCnt], "END_KEY", strlen("END_KEY")) != 0)
								{
									fgets(cLine, MAX_CHARA, pFile);
									nCnt = 0;
									while (1)
									{
										if (cLine[nCnt] == ' ' || cLine[nCnt] == '\t')
										{
											nCnt++;
										}
										else
										{
											break;
										}
									}
									pStrCur = &cLine[nCnt];
									strcpy(cStr, pStrCur);

									if (memcmp(&cLine[nCnt], "POS = ", strlen("POS = ")) == 0)
									{	//位置
										pStrCur += strlen("POS = ");
										strcpy(cStr, pStrCur);
										cPunctuation = strtok(cStr, " ");
										g_aMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos.x = (float)atof(cPunctuation);
										cPunctuation = strtok(NULL, " ");
										g_aMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos.y = (float)atof(cPunctuation);
										cPunctuation = strtok(NULL, " ");
										g_aMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos.z = (float)atof(cPunctuation);
									}
									if (memcmp(&cLine[nCnt], "ROT = ", strlen("ROT = ")) == 0)
									{	//角度
										pStrCur += strlen("ROT = ");
										strcpy(cStr, pStrCur);
										cPunctuation = strtok(cStr, " ");
										g_aMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot.x = (float)atof(cPunctuation);
										cPunctuation = strtok(NULL, " ");
										g_aMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot.y = (float)atof(cPunctuation);
										cPunctuation = strtok(NULL, " ");
										g_aMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot.z = (float)atof(cPunctuation);
									}
								}
								nCntParts++;	//次のパーツへ
							}
						}
						nCntKey++;	//次のキーへ
					}
				}
				nCntMotion++;	//次のモーションへ
			}
		}
	}
}
