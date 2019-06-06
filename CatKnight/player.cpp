//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "joystick.h"
#include "xController.h"

#include "camera.h"
#include "shadow.h"
#include "meshfield.h"
#include "line.h"
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
#include "princess.h"
#include "fileload.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define PLAYER_TEXTURE	"data/TEXTURE/white.jpg"	//読み込むテクスチャ名
#define PLAYER_FILENAME "data/motion_Cat.txt"	//モデルファイル名
#define PLAYER_SHADOW		(15.0f)			//影の大きさ
#define PLAYER_ROTSPEED		(0.1f)			//回転速度
#define PLAYER_KEY_MAX		(4)				//キーの最大数
#define PLAYER_RANGE		(20)

//=============================================================================
// プロトタイプ宣言
//=============================================================================
#ifdef _DEBUG
void DrawPos(void);
#endif
void SwitchMotion();		//モーション切り替え

//=============================================================================
// グローバル変数
//=============================================================================
PLAYER				g_Player;			//プレイヤー情報
COLLISION			g_Collision;		//当たり判定情報
LPDIRECT3DTEXTURE9	g_pTexture[6] = {};	//テクスチャのポインタ
int	g_nIdxShadow;		//影のID
int g_nCntframe = 0;	//現在のフレーム数
int g_nCntKey = 0;		//現在のキー数
char acModelFileName[PLAYER_MAX][MAX_CHARA];	//モデルファイル名格納用

#ifdef _DEBUG
LPD3DXFONT			g_pFontPL = NULL;			// フォントへのポインタ
D3DXVECTOR3			g_PLpos;
#endif

LOAD *pLoad = GetLoad(CHARA_PLAYER);

//=============================================================================
// 初期化処理
//=============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ファイル読み込み
	LoadFile(PLAYER_FILENAME, CHARA_PLAYER);

	// 位置・向きの初期設定
	g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, -40.0f);
	g_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.nLife = pLoad->nMaxLife;
	g_Player.fRadius = pLoad->fRadius;
	g_Player.state = PLAYERSTATE_NEUTRAL;
	g_Player.bAttack = false;
	g_Player.bDamege = false;
	g_Player.nCntState = 0;

	g_Collision.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Collision.rot= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//影の作成
	g_nIdxShadow = SetSadow(g_Player.pos, g_Player.rot, PLAYER_SHADOW, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));

	// Xファイルの読み込み
	for (int nCntModel = 0; nCntModel < pLoad->nNumModel; nCntModel++)
	{
		D3DXLoadMeshFromX(pLoad->cFileNeme[nCntModel],	//読み込むファイル名
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
	SetTrajectory(pLoad->Meshpos[0], pLoad->Meshpos[1], &g_Player.aModel[pLoad->nMeshParent].mtxWorld, D3DXCOLOR(1.0f,0.5f,0.0f,1.0f));

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
	for (int nCntParts = 0; nCntParts < PLAYER_MAX; nCntParts++)
	{
		// メッシュの開放
		if (g_Player.aModel[nCntParts].pMesh != NULL)
		{
			g_Player.aModel[nCntParts].pMesh->Release();
			g_Player.aModel[nCntParts].pMesh = NULL;
		}

		// マテリアルの開放
		if (g_Player.aModel[nCntParts].pBuffMat != NULL)
		{
			g_Player.aModel[nCntParts].pBuffMat->Release();
			g_Player.aModel[nCntParts].pBuffMat = NULL;
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
	PRINCESS *pPrincess = GetPrincess();

	g_Player.posOld = g_Player.pos;
	g_Collision.posOld = pLoad->Collpos;

	if (g_Player.bDamege)
	{
		g_Player.nCntState++;
		if (g_Player.nCntState % 10 == 0)
		{	//ダメージ状態解除
			g_Player.bDamege = false;
		}
	}

	MESH_TRAJECTORY *pTrajectory = GetTrajectory();
	if (g_Player.state == PLAYERSTATE_ATTACK01 || g_Player.state == PLAYERSTATE_ATTACK02)
	{
		pTrajectory->bUse = true;
	}
	else
	{
		pTrajectory->bUse = false;
	}

	//フレーム増加
	if (g_Player.state == PLAYERSTATE_DETH && g_nCntKey >= 1)
	{
		//死亡時は増加しない
	}
	else
	{
		g_nCntframe++;
		if (g_nCntframe > pLoad->MotionInfo[g_Player.state].KeyInfo[g_nCntKey].nFrame)
		{
			g_nCntframe = 0;
			g_nCntKey++;
			if (g_nCntKey >= pLoad->MotionInfo[g_Player.state].nNumKey)
			{
				g_nCntKey = 0;
			}
		}
	}

	//モーションの切り替え
	SwitchMotion();

	if (g_Player.state != PLAYERSTATE_DETH)
	{
		if (g_Player.state != PLAYERSTATE_ATTACK01 && g_Player.state != PLAYERSTATE_ATTACK02)
		{
			//任意のキーが押されたかどうか
			OperationMove(&g_Player.move, pLoad->fSpeed);
		}
		if (!pPrincess->bHold && g_Player.state != PLAYERSTATE_WIN && (GetKeyTrigger(DIK_SPACE) || GetJoystickTrigger(PAD_B) || GetxControllerTrigger(XINPUT_PAD_B)))
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

		float fDistX, fDistZ;
		float fDistans = 100000;
		fDistX = pPrincess->pos.x - g_Player.pos.x;
		fDistZ = pPrincess->pos.z - g_Player.pos.z;
		fDistans = (float)sqrt(fDistX * fDistX + fDistZ * fDistZ);

		if (pLoad->fRadius + PLAYER_RANGE > fDistans && (GetKeyboardPress(DIK_N) || GetJoystickPress(PAD_X) || GetxControllerPress(XINPUT_PAD_X)))
		{	//プレイヤーの近くに姫がいたら
			HoldPrincess(&g_Player,fDistX,fDistZ);
			g_Player.bHold = true;
		}
		else
		{
			g_Player.bHold = false;
			pPrincess->bHold = false;
		}
	}
#ifdef _DEBUG
	if (GetKeyTrigger(DIK_J))
	{	// ジャンプ
		g_Player.move.y += 10.0f;
	}
#endif

	//回転
	RotationDir(g_Player.rotmove, g_Player.rotDest, g_Player.rot, PLAYER_ROTSPEED);

	//重力
	g_Player.move.y -= GRAVITY;

	//慣性
	g_Player.move.x += (0.0f - g_Player.move.x) * INERTIA;
	g_Player.move.z += (0.0f - g_Player.move.z) * INERTIA;

	//移動
	g_Player.pos.x += g_Player.move.x;
	g_Player.pos.z += g_Player.move.z;
	g_Player.pos.y += g_Player.move.y;

	//当たり判定
	CollisionPrincess(&g_Player.pos, &g_Player.posOld, pLoad->fRadius, 0);

	//障害物
	OBSTACLE *pObstacle = GetObstacle();
	for (int nCntObstacle = 0; nCntObstacle < OBSTACLE_MAX; nCntObstacle++, pObstacle++)
	{
		CollisionObstacle(&g_Player.pos, &g_Player.posOld, &g_Player.move, pLoad->fRadius, nCntObstacle);
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
				(pLoad->nStart <= g_nCntframe) && (g_nCntframe <= pLoad->nEnd))
			{
				if (CollisionBoss(&D3DXVECTOR3(g_Collision.mtxWorld._41, g_Collision.mtxWorld._42, g_Collision.mtxWorld._43), &g_Collision.posOld, pLoad->fCollRadius,nCntBoss))
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
	ENEMY *pEnemy;
	pEnemy = GetEnemy();
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse)
		{
			if ((g_Player.state == PLAYERSTATE_ATTACK01 || g_Player.state == PLAYERSTATE_ATTACK02) &&
				(pEnemy->state != ENEMYSTATE_DETH) &&
				(pLoad->nStart <= g_nCntframe) && (g_nCntframe <= pLoad->nEnd))
			{
				if (CollisionEnemy(&D3DXVECTOR3(g_Collision.mtxWorld._41, g_Collision.mtxWorld._42, g_Collision.mtxWorld._43), &g_Collision.posOld, pLoad->fCollRadius, nCntEnemy))
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

	//壁めり込み判定
	CollisionWall(&g_Player.pos, pLoad->fRadius);

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
	for (int nCntParts = 0; nCntParts < pLoad->nNumParts; nCntParts++)
	{
		//親の識別
		if (pLoad->nParent[nCntParts] == -1)
		{
			mtxParent = g_Player.mtxWorld;
		}
		else
		{
			mtxParent = g_Player.aModel[pLoad->nParent[nCntParts]].mtxWorld;
		}
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Player.aModel[nCntParts].mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pLoad->rot[nCntParts].y, pLoad->rot[nCntParts].x, pLoad->rot[nCntParts].z);

		D3DXMatrixMultiply(&g_Player.aModel[nCntParts].mtxWorld, &g_Player.aModel[nCntParts].mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pLoad->pos[nCntParts].x, pLoad->pos[nCntParts].y, pLoad->pos[nCntParts].z);

		D3DXMatrixMultiply(&g_Player.aModel[nCntParts].mtxWorld, &g_Player.aModel[nCntParts].mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

		//親子反映
		D3DXMatrixMultiply(&g_Player.aModel[nCntParts].mtxWorld, &g_Player.aModel[nCntParts].mtxWorld, &mtxParent);	//ワールド行列＝ワールド行列＊親のワールド行列

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Player.aModel[nCntParts].mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_Player.aModel[nCntParts].pBuffMat->GetBufferPointer();

		// テクスチャの設定をするならここ(1枚の時)

		for (int nCntMat = 0; nCntMat < (int)g_Player.aModel[nCntParts].nNumMat; nCntMat++)
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
			g_Player.aModel[nCntParts].pMesh->DrawSubset(nCntMat);	//格子状態
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
	D3DXMatrixTranslation(&mtxTrans, pLoad->Collpos.x, pLoad->Collpos.y, pLoad->Collpos.z);

	D3DXMatrixMultiply(&g_Collision.mtxWorld, &g_Collision.mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

	//親子反映
	D3DXMatrixMultiply(&g_Collision.mtxWorld, &g_Collision.mtxWorld, &g_Player.aModel[pLoad->nCollParent].mtxWorld);	//ワールド行列＝ワールド行列＊親のワールド行列

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
	g_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.nLife = pLoad->nMaxLife;
	g_Player.state = PLAYERSTATE_NEUTRAL;

}

//=============================================================================
// 操作入力
//=============================================================================
void OperationMove(D3DXVECTOR3 *pMove,float fSpeed)
{
	CAMERA *pCamera;
	pCamera = GetCamera();

	if (GetKeyboardPress(DIK_A) || GetJoystickPress(PAD_LEFT_L) || GetxControllerPress(XINPUT_PAD_LEFT_L))
	{
		if (GetKeyboardPress(DIK_W) || GetJoystickPress(PAD_UP_L) || GetxControllerPress(XINPUT_PAD_UP_L))
		{	//左上
			pMove->x += sinf(-D3DX_PI * 0.75f - pCamera->rot.y) * fSpeed;	//加速値
			pMove->z -= cosf(-D3DX_PI * 0.75f - pCamera->rot.y) * fSpeed;
			g_Player.rotDest.y = D3DX_PI * 0.75f + pCamera->rot.y;
		}
		else if (GetKeyboardPress(DIK_S) || GetJoystickPress(PAD_DOWN_L) || GetxControllerPress(XINPUT_PAD_DOWN_L))
		{	//左下
			pMove->x += sinf(-D3DX_PI * 0.25f - pCamera->rot.y) * fSpeed;
			pMove->z -= cosf(-D3DX_PI * 0.25f - pCamera->rot.y) * fSpeed;
			g_Player.rotDest.y = D3DX_PI * 0.25f + pCamera->rot.y;
		}
		else
		{	//左
			pMove->x += sinf(-D3DX_PI * 0.5f + pCamera->rot.y) * fSpeed;
			pMove->z += cosf(-D3DX_PI * 0.5f + pCamera->rot.y) * fSpeed;
			g_Player.rotDest.y = D3DX_PI * 0.5f + pCamera->rot.y;
		}
	}
	else if (GetKeyboardPress(DIK_D) || GetJoystickPress(PAD_RIGHT_L) || GetxControllerPress(XINPUT_PAD_RIGHT_L))
	{
		if (GetKeyboardPress(DIK_W) || GetJoystickPress(PAD_UP_L))
		{	//右上
			pMove->x += sinf(D3DX_PI * 0.75f - pCamera->rot.y) * fSpeed;
			pMove->z -= cosf(D3DX_PI * 0.75f - pCamera->rot.y) * fSpeed;
			g_Player.rotDest.y = -D3DX_PI * 0.75f + pCamera->rot.y;
		}
		else if (GetKeyboardPress(DIK_S) || GetJoystickPress(PAD_DOWN_L) || GetxControllerPress(XINPUT_PAD_DOWN_L))
		{	//右下
			pMove->x += sinf(D3DX_PI * 0.25f - pCamera->rot.y) * fSpeed;
			pMove->z -= cosf(D3DX_PI * 0.25f - pCamera->rot.y) * fSpeed;
			g_Player.rotDest.y = -D3DX_PI * 0.25f + pCamera->rot.y;
		}
		else
		{	//右
			pMove->x += sinf(D3DX_PI * 0.5f + pCamera->rot.y) * fSpeed;
			pMove->z += cosf(D3DX_PI * 0.5f + pCamera->rot.y) * fSpeed;
			g_Player.rotDest.y = -D3DX_PI * 0.5f + pCamera->rot.y;
		}
	}
	else if (GetKeyboardPress(DIK_W) || GetJoystickPress(PAD_UP_L) || GetxControllerPress(XINPUT_PAD_UP_L))
	{	//奥
		pMove->x -= sinf(D3DX_PI * 1.0f + pCamera->rot.y) * fSpeed;
		pMove->z -= cosf(D3DX_PI * 1.0f + pCamera->rot.y) * fSpeed;
		g_Player.rotDest.y = D3DX_PI * 1.0f + pCamera->rot.y;
	}
	else if (GetKeyboardPress(DIK_S) || GetJoystickPress(PAD_DOWN_L) || GetxControllerPress(XINPUT_PAD_DOWN_L))
	{	//手前
		pMove->x -= sinf(D3DX_PI * 0.0f + pCamera->rot.y) * fSpeed;
		pMove->z -= cosf(D3DX_PI * 0.0f + pCamera->rot.y) * fSpeed;
		g_Player.rotDest.y = D3DX_PI * 0.0f + pCamera->rot.y;
	}
}

//=============================================================================
// プレイヤー情報の取得
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

//=============================================================================
// 向きの回転
//=============================================================================
void RotationDir(D3DXVECTOR3 &rotmove, D3DXVECTOR3 &rotDest, D3DXVECTOR3 &rot, float fSpeed)
{
	if (rotmove.y = rotDest.y - rot.y)
	{
		if (rotmove.y < -D3DX_PI)
		{
			rotmove.y += D3DX_PI * 2;
		}
		else if (rotmove.y > D3DX_PI)
		{
			rotmove.y -= D3DX_PI * 2;
		}
	}
	if (rot.y += rotmove.y * fSpeed)
	{
		if (rot.y > D3DX_PI)
		{
			rot.y -= D3DX_PI * 2;
		}
		else if (rot.y < -D3DX_PI)
		{
			rot.y += D3DX_PI * 2;
		}
	}

}

//=============================================================================
// 当たり判定
//=============================================================================
bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Radius)
{
	bool bLand = false;
	float fDistance = (pPos->x - g_Player.pos.x)*(pPos->x - g_Player.pos.x) + (pPos->z - g_Player.pos.z)*(pPos->z - g_Player.pos.z);	//2点間の距離
	float fLength = (Radius + pLoad->fRadius)*(Radius + pLoad->fRadius);	//半径
	float fDiff = (Radius + pLoad->fRadius) - (float)sqrt(fDistance);	//差
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
// 壁めり込み判定
//=============================================================================
void CollisionWall(D3DXVECTOR3 *pPos,float fRadius)
{
	//左右壁判定
	if (pPos->x > MESHFIELD_SIZE * MESHFIELD_W * 0.5f - fRadius)
	{
		pPos->x = MESHFIELD_SIZE * MESHFIELD_W * 0.5f - fRadius;
	}
	if (pPos->x < -MESHFIELD_SIZE * MESHFIELD_W * 0.5f + fRadius)
	{
		pPos->x = -MESHFIELD_SIZE * MESHFIELD_W * 0.5f + fRadius;
	}
	//奥手前壁判定
	if (pPos->z > MESHFIELD_SIZE * MESHFIELD_D * 0.5f - fRadius)
	{
		pPos->z = MESHFIELD_SIZE * MESHFIELD_D * 0.5f - fRadius;
	}
	if (pPos->z < -MESHFIELD_SIZE * MESHFIELD_D * 0.5f + fRadius)
	{
		pPos->z = -MESHFIELD_SIZE * MESHFIELD_D * 0.5f + fRadius;
	}
}
//=============================================================================
// ダメージ処理
//=============================================================================
void HitPlayer(int Damege, D3DXVECTOR3 *pPos)
{
	D3DXVECTOR3 vec = D3DXVECTOR3((g_Player.pos.x - pPos->x), 0.0f, (g_Player.pos.z - pPos->z));	//ベクトル
	D3DXVec3Normalize(&vec, &vec);

	DecreaseLife(Damege);
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
		g_Player.nLife = pLoad->nMaxLife;
		IncreaseLife(pLoad->nMaxLife);
	}

}

//=============================================================================
//	モーションの切り替え
//=============================================================================
void SwitchMotion()
{
	switch (*GetGameState())
	{
	case GAMESTATE_CLEAR:
		if (g_Player.state != PLAYERSTATE_ATTACK01 && g_Player.state != PLAYERSTATE_ATTACK02)
		{
			if (g_Player.move.x > LATEST_SPEED || g_Player.move.z > LATEST_SPEED ||
				g_Player.move.x < -LATEST_SPEED || g_Player.move.z < -LATEST_SPEED)
			{
				if (g_Player.bHold)
				{
					g_Player.state = PLAYERSTATE_HOLDMOVE;
				}
				else
				{
					g_Player.state = PLAYERSTATE_MOVE;
				}
			}
			else
			{
				g_Player.state = PLAYERSTATE_WIN;
			}
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
			if (g_Player.move.x > LATEST_SPEED || g_Player.move.z > LATEST_SPEED ||
				g_Player.move.x < -LATEST_SPEED || g_Player.move.z < -LATEST_SPEED)
			{
				if (g_Player.bHold)
				{
					g_Player.state = PLAYERSTATE_HOLDMOVE;
				}
				else
				{
					g_Player.state = PLAYERSTATE_MOVE;
				}
			}
			else
			{
				if (g_Player.bHold)
				{
					g_Player.state = PLAYERSTATE_HOLDNEUTRAL;
				}
				else
				{
					g_Player.state = PLAYERSTATE_NEUTRAL;
				}
			}
		}
		break;
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
		if (nNextKey >= pLoad->MotionInfo[g_Player.state].nNumKey)
		{
			if (pLoad->MotionInfo[g_Player.state].bLoop)
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

		pKey = &pLoad->MotionInfo[g_Player.state].KeyInfo[g_nCntKey].Key[nCntMotion];		//現在のキー
		pNextKey = &pLoad->MotionInfo[g_Player.state].KeyInfo[nNextKey].Key[nCntMotion];	//次のキー

		//回転
		g_Player.aModel[nCntMotion].rotMove = (pNextKey->rot - pKey->rot)*((float)g_nCntframe / (float)pLoad->MotionInfo[g_Player.state].KeyInfo[g_nCntKey].nFrame);

		pLoad->rot[nCntMotion] = pKey->rot + g_Player.aModel[nCntMotion].rotMove;
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