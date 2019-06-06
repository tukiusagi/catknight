//=============================================================================
//
// 姫の処理 [princess.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "princess.h"
#include "input.h"
#include "joystick.h"
#include "xController.h"

#include "camera.h"
#include "shadow.h"
#include "meshfield.h"
#include "line.h"
#include"game.h"
#include "enemy.h"
#include "wave.h"
#include "sound.h"
#include "magic.h"
#include "fileload.h"
#include "magicGage.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define PRINCESS_TEXTURE	"data/TEXTURE/mizuiro.jpg"	//読み込むテクスチャ名
#define PRINCESS_TEXTURE2	"data/TEXTURE/white2.jpg"	//読み込むテクスチャ名
#define PRINCESS_FILENAME	"data/motion_Princess.txt"	//読み込むファイル名

#define PRINCESS_SHADOW		(15.0f)			//影の大きさ
#define PRINCESS_ROTSPEAD	(0.1f)			//回転速度
#define PRINCESS_KEY_MAX	(4)				//キーの最大数
#define PRINCESS_TEX_MAX	(6)				//テクスチャの最大数
#define MAX_CHARA			(256)			//読み込む文字の最大数
#define INTERBAL_FRAME		(10)			//補完フレーム

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void SwitchMotionPrincess();			//モーション切り替え

//=============================================================================
// グローバル変数
//=============================================================================
PRINCESS			g_Princess;					//姫の情報
LPDIRECT3DTEXTURE9	g_pTexturePrincess[PRINCESS_TEX_MAX] = {};	//テクスチャのポインタ
int	g_nIdxShadowPrincess;		//影のID
char g_PrincessFileNeme[PRINCESS_MAX][MAX_CHARA];

LOAD *pLoadPrincess = GetLoad(CHARA_PRINCESS);
int g_nFrame = 40;
bool g_bMotion;

//=============================================================================
// 初期化処理
//=============================================================================
void InitPrincess(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//データのロード
	LoadFile(PRINCESS_FILENAME, CHARA_PRINCESS);
	g_bMotion = false;

	// 位置・向きの初期設定
	g_Princess.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Princess.state = PRINCESSSTATE_NEUTRAL;
	g_Princess.stateOld = PRINCESSSTATE_NEUTRAL;

	g_Princess.bDamege = false;
	g_Princess.bPinch = false;
	g_Princess.bHold = false;

	g_Princess.nMaxLife = pLoadPrincess->nMaxLife;
	g_Princess.nCntState = 0;
	g_Princess.nCntFrame = 0;
	g_Princess.nCntKeyPrincess = 0;

	//影の作成
	g_nIdxShadowPrincess = SetSadow(g_Princess.pos, g_Princess.rot, PRINCESS_SHADOW, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
	if (*GetMode() != MODE_TITLE)
	{
		SetMagic(g_Princess.pos);
	}

	// Xファイルの読み込み
	for (int nCntModel = 0; nCntModel < pLoadPrincess->nNumModel; nCntModel++)
	{
		D3DXLoadMeshFromX(pLoadPrincess->cFileNeme[nCntModel],	//読み込むファイル名
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_Princess.aModel[nCntModel].pBuffMat,
			NULL,
			&g_Princess.aModel[nCntModel].nNumMat,
			&g_Princess.aModel[nCntModel].pMesh);

	}

	//テクスチャの読み込み
	for (int nCntTex = 0; nCntTex < PRINCESS_TEX_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			PRINCESS_TEXTURE2,
			&g_pTexturePrincess[nCntTex]);
	}

	D3DXCreateTextureFromFile(pDevice,
		PRINCESS_TEXTURE,
		&g_pTexturePrincess[2]);

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPrincess(void)
{
	for (int nCntPrincess = 0; nCntPrincess < pLoadPrincess->nNumModel; nCntPrincess++)
	{
		// メッシュの開放
		if (g_Princess.aModel[nCntPrincess].pMesh != NULL)
		{
			g_Princess.aModel[nCntPrincess].pMesh->Release();
			g_Princess.aModel[nCntPrincess].pMesh = NULL;
		}

		// マテリアルの開放
		if (g_Princess.aModel[nCntPrincess].pBuffMat != NULL)
		{
			g_Princess.aModel[nCntPrincess].pBuffMat->Release();
			g_Princess.aModel[nCntPrincess].pBuffMat = NULL;
		}
	}

	//テクスチャの開放
	for (int nCntTex = 0; nCntTex < PRINCESS_TEX_MAX; nCntTex++)
	{
		if (g_pTexturePrincess[nCntTex] != NULL)
		{
			g_pTexturePrincess[nCntTex]->Release();
			g_pTexturePrincess[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePrincess(void)
{
	WAVE *pWave = GetWave();
	g_Princess.bPinch = false;
	g_Princess.posOld = g_Princess.pos;

	if (g_Princess.bDamege)
	{	//ダメージ状態
		g_Princess.nCntState++;
		if (g_Princess.nCntState % 10 == 0)
		{
			g_Princess.bDamege = false;	//状態解除
		}
	}

	//フレーム
	if (g_Princess.state == PRINCESSSTATE_DETH && g_Princess.nCntKeyPrincess >= 1)
	{	//死亡時には増加しない
	}
	else
	{	//フレーム増加
		g_Princess.nCntFrame++;
		if (!g_bMotion)
		{
			if (g_Princess.nCntFrame > pLoadPrincess->MotionInfo[g_Princess.state].KeyInfo[g_Princess.nCntKeyPrincess].nFrame)
			{
				g_Princess.nCntFrame = 0;
				g_Princess.nCntKeyPrincess++;	//キー増加
				if (g_Princess.nCntKeyPrincess >= pLoadPrincess->MotionInfo[g_Princess.state].nNumKey)
				{
					g_Princess.nCntKeyPrincess = 0;
				}
			}
		}
		else
		{
			if (g_Princess.nCntFrame > g_nFrame)
			{
				g_Princess.nCntFrame = 0;
				g_Princess.stateOld = g_Princess.state;
			}
		}
	}

	if (*GetMode() == MODE_GAME || *GetMode() == MODE_TUTORIAL)
	{
		if (g_Princess.bHold)
		{	//移動中は魔法を消す
			DeleteMagic();
			DeleteWave();
		}
		else
		{	//魔法セット
			SetMagic(g_Princess.pos);
			SetWave(g_Princess.pos);
		}
	}

	//モーションの切り替え
	if (*GetMode() == MODE_GAME)
	{
		SwitchMotionPrincess();
	}
	else
	{
		if (g_Princess.move.x > LATEST_SPEED || g_Princess.move.z > LATEST_SPEED ||
			g_Princess.move.x < -LATEST_SPEED || g_Princess.move.z < -LATEST_SPEED)
		{	//速度がある
			g_Princess.state = PRINCESSSTATE_MOVE;
		}
		else if (g_Princess.state != PRINCESSSTATE_ATTACK && g_Princess.state != PRINCESSSTATE_WIN)
		{	//通常時
			if (g_Princess.bHold)
			{	//手をつないでる
				g_Princess.state = PRINCESSSTATE_HOLDNEUTRAL;
			}
			else
			{	//1人
				g_Princess.state = PRINCESSSTATE_NEUTRAL;
			}
		}
	}

	if (pWave->state == WAVE_STATE_CHARGEMAX)
	{

		if ((GetKeyTrigger(DIK_B) || GetJoystickTrigger(PAD_Y) || GetxControllerTrigger(XINPUT_PAD_Y)))
		{	//ボタンを押したら魔法
			MAGIC *pMagic = GetMagic();
			g_Princess.state = PRINCESSSTATE_ATTACK;
			g_Princess.nCntFrame = 0;
			g_Princess.nCntKeyPrincess = 0;
			pWave->state = WAVE_STATE_ATTACK;
			pWave->col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
			pMagic->col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
			DecreaseMagicGage();
			PlaySound(SOUND_LABEL_SE_MAGIC);
		}
	}

	//回転
	RotationDir(g_Princess.rotmove, g_Princess.rotDest, g_Princess.rot, PRINCESS_ROTSPEAD);

	//重力
	g_Princess.move.y -= GRAVITY;

	//慣性
	g_Princess.move.x += (0.0f - g_Princess.move.x)*INERTIA;
	g_Princess.move.z += (0.0f - g_Princess.move.z)*INERTIA;

	//移動
	g_Princess.pos.x += g_Princess.move.x;
	g_Princess.pos.z += g_Princess.move.z;
	g_Princess.pos.y += g_Princess.move.y;


	if (g_Princess.pos.y < 0)
	{	//地面にめり込まないように
		g_Princess.pos.y = 0;
		g_Princess.move.y = 0;
	}
	//左右壁判定
	CollisionWall(&g_Princess.pos, pLoadPrincess->fRadius);

	if (g_Princess.state == PRINCESSSTATE_DETH && g_Princess.nCntKeyPrincess >= 1)
	{
	}
	else
	{
		//モーション再生
		PrincessMotion();
	}

	//影の位置を設定
	SetPositionSadow(g_nIdxShadowPrincess, g_Princess.pos, PRINCESS_SHADOW + ((0.0f - g_Princess.pos.y)*-0.1f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f - ((0.0f - g_Princess.pos.y)*-0.008f)));
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPrincess(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	D3DXMATRIX		mtxRot, mtxTrans, mtxParent;	// 計算用マトリックス
	D3DXMATERIAL	*pMat;	//現在のマテリアル保存用
	D3DMATERIAL9	matDef;	//マテリアルデータへのポインタ
	D3DMATERIAL9		matKeep;	//保存用

//姫の設定
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Princess.mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Princess.rot.y, g_Princess.rot.x, g_Princess.rot.z);

	D3DXMatrixMultiply(&g_Princess.mtxWorld, &g_Princess.mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Princess.pos.x, g_Princess.pos.y, g_Princess.pos.z);

	D3DXMatrixMultiply(&g_Princess.mtxWorld, &g_Princess.mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Princess.mtxWorld);

//モデルの設定
	for (int nCntPrincess = 0; nCntPrincess < pLoadPrincess->nNumParts; nCntPrincess++)
	{
		//親の識別
		if (pLoadPrincess->nParent[nCntPrincess] == -1)
		{
			mtxParent = g_Princess.mtxWorld;
		}
		else
		{
			mtxParent = g_Princess.aModel[pLoadPrincess->nParent[nCntPrincess]].mtxWorld;
		}
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Princess.aModel[nCntPrincess].mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pLoadPrincess->rot[nCntPrincess].y, pLoadPrincess->rot[nCntPrincess].x, pLoadPrincess->rot[nCntPrincess].z);

		D3DXMatrixMultiply(&g_Princess.aModel[nCntPrincess].mtxWorld, &g_Princess.aModel[nCntPrincess].mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pLoadPrincess->pos[nCntPrincess].x, pLoadPrincess->pos[nCntPrincess].y, pLoadPrincess->pos[nCntPrincess].z);

		D3DXMatrixMultiply(&g_Princess.aModel[nCntPrincess].mtxWorld, &g_Princess.aModel[nCntPrincess].mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

		D3DXMatrixMultiply(&g_Princess.aModel[nCntPrincess].mtxWorld, &g_Princess.aModel[nCntPrincess].mtxWorld, &mtxParent);	//ワールド行列＝ワールド行列＊親のワールド行列
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Princess.aModel[nCntPrincess].mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_Princess.aModel[nCntPrincess].pBuffMat->GetBufferPointer();
		// テクスチャの設定をするならここ(1枚の時)

		for (int nCntMat = 0; nCntMat < (int)g_Princess.aModel[nCntPrincess].nNumMat; nCntMat++)
		{
			// テクスチャの設定をするならここ(複数枚の時)
			pDevice->SetTexture(0, g_pTexturePrincess[nCntMat]);

			pMat[nCntMat].MatD3D.Ambient.r = pMat[nCntMat].MatD3D.Diffuse.r/12;
			pMat[nCntMat].MatD3D.Ambient.g = pMat[nCntMat].MatD3D.Diffuse.g/12;
			pMat[nCntMat].MatD3D.Ambient.b = pMat[nCntMat].MatD3D.Diffuse.b/12;

			matKeep = pMat[nCntMat].MatD3D;	//保存

			if (g_Princess.bDamege)
			{
				pMat[nCntMat].MatD3D.Diffuse.r = 255;	//赤色変更
			}

			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			pMat[nCntMat].MatD3D = matKeep;	//色を戻す

			// 描画
			g_Princess.aModel[nCntPrincess].pMesh->DrawSubset(nCntMat);	//格子状態
		}

		// マテリアルをデフォルトに戻す
		pDevice->SetMaterial(&matDef);
	}
}

//=============================================================================
// 姫の設定
//=============================================================================
void SetPrincess(void)
{
	g_Princess.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.state = PRINCESSSTATE_NEUTRAL;
	g_Princess.stateOld = PRINCESSSTATE_NEUTRAL;

	g_Princess.bDamege = false;
	g_Princess.bPinch = false;
	g_Princess.bHold = false;

	DeleteMagic();
	DeleteWave();
	SetMagic(g_Princess.pos);
	SetWave(g_Princess.pos);

}

//=============================================================================
// 姫情報の取得
//=============================================================================
PRINCESS *GetPrincess(void)
{
	return &g_Princess;
}

//=============================================================================
//	モーションの切り替え
//=============================================================================
void SwitchMotionPrincess()
{
	switch (*GetGameState())
	{
	case GAMESTATE_CLEAR:
		//喜び
		g_Princess.state = PRINCESSSTATE_WIN;
		break;
	case GAMESTATE_GAMEOVER:
		//死
		g_Princess.state = PRINCESSSTATE_DETH;
		break;
	default:
		if (g_Princess.move.x > LATEST_SPEED || g_Princess.move.z > LATEST_SPEED ||
			g_Princess.move.x < -LATEST_SPEED || g_Princess.move.z < -LATEST_SPEED)
		{	//速度がある
			g_Princess.state = PRINCESSSTATE_MOVE;
		}
		else if (g_Princess.state != PRINCESSSTATE_ATTACK && g_Princess.state != PRINCESSSTATE_WIN)
		{	//通常時
			if (g_Princess.bHold)
			{	//手をつないでる
				g_Princess.state = PRINCESSSTATE_HOLDNEUTRAL;
			}
			else
			{	//1人
				g_Princess.state = PRINCESSSTATE_NEUTRAL;
			}
		}
		break;
	}
}
//=============================================================================
//	モーションの再生
//=============================================================================
void PrincessMotion(void)
{
	//前回のモーションと今回のモーションが違ったら何フレームか賭けて変化する

	//パーツ数分
	for (int nCntMotion = 0; nCntMotion < pLoadPrincess->nNumParts; nCntMotion++)
	{
		KEY *pKey, *pNextKey;
		int nNextKey = g_Princess.nCntKeyPrincess + 1;
		if (g_Princess.state != g_Princess.stateOld)
		{
			nNextKey = 0;
		}

		if (nNextKey >= pLoadPrincess->MotionInfo[g_Princess.state].nNumKey)
		{
			if (pLoadPrincess->MotionInfo[g_Princess.state].bLoop)
			{
				nNextKey = 0;
			}
			else
			{
				g_Princess.stateOld = g_Princess.state;
				g_Princess.state = PRINCESSSTATE_NEUTRAL;
				nNextKey = 0;
				g_Princess.nCntFrame = 0;
				g_Princess.nCntKeyPrincess = 0;
			}
		}
		if (g_Princess.state != g_Princess.stateOld)
		{
			g_bMotion = true;
			g_nFrame = INTERBAL_FRAME;
			pKey = &pLoadPrincess->MotionInfo[g_Princess.stateOld].KeyInfo[pLoadPrincess->MotionInfo[g_Princess.stateOld].nNumKey-1].Key[nCntMotion];		//最後のキー	
			pNextKey = &pLoadPrincess->MotionInfo[g_Princess.state].KeyInfo[nNextKey].Key[nCntMotion];	//次のキー		
		}
		else
		{
			g_bMotion = false;
			g_nFrame = pLoadPrincess->MotionInfo[g_Princess.state].KeyInfo[g_Princess.nCntKeyPrincess].nFrame;
			pKey = &pLoadPrincess->MotionInfo[g_Princess.state].KeyInfo[g_Princess.nCntKeyPrincess].Key[nCntMotion];		//現在のキー	
			pNextKey = &pLoadPrincess->MotionInfo[g_Princess.state].KeyInfo[nNextKey].Key[nCntMotion];	//次のキー		
		}

		//回転
		g_Princess.aModel[nCntMotion].rotMove = (pNextKey->rot - pKey->rot)*((float)g_Princess.nCntFrame / (float)g_nFrame);
		pLoadPrincess->rot[nCntMotion] = pKey->rot + g_Princess.aModel[nCntMotion].rotMove;
	}
}

//=============================================================================
// 連れ歩き処理
//=============================================================================
void HoldPrincess(PLAYER *pPlayer, float fDistX, float fDistZ)
{
	float fLength = (pPlayer->fRadius + pLoadPrincess->fRadius);
	D3DXVECTOR3 vec = -pPlayer->move;	//プレイヤーの移動方向と反対方向のベクトル
	D3DXVec3Normalize(&vec, &vec);
	g_Princess.bHold = true;

	//姫をプレイヤーの後ろにセット
	if (vec.x >= 0.1f || vec.z >= 0.1f || vec.x <= -0.1f || vec.z <= -0.1f)
	{
		g_Princess.pos.x = pPlayer->pos.x + vec.x * fLength;
		g_Princess.pos.z = pPlayer->pos.z + vec.z * fLength;
	}

	//一緒に動く
	g_Princess.move = pPlayer->move;
	g_Princess.rotDest.y = atan2f(fDistX, fDistZ);
}

//=============================================================================
// 当たり判定
//=============================================================================
bool CollisionPrincess(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Radius, int nType)
{
	bool bLand = false;
	float fDistance = (pPos->x - g_Princess.pos.x)*(pPos->x - g_Princess.pos.x) + (pPos->z - g_Princess.pos.z)*(pPos->z - g_Princess.pos.z);	//2点間の距離
	float fLength = (Radius + pLoadPrincess->fRadius)*(Radius + pLoadPrincess->fRadius);	//半径
	float fDiff = (Radius + pLoadPrincess->fRadius) - (float)sqrt(fDistance);	//差
	D3DXVECTOR3 vec = D3DXVECTOR3((pPos->x - g_Princess.pos.x), 0.0f, (pPos->z - g_Princess.pos.z));	//ベクトル
	D3DXVec3Normalize(&vec, &vec);

	if (fDistance <= fLength)
	{
		if (nType == 1)
		{	//敵が近くにいたらピンチ
			g_Princess.bPinch = true;
		}
		bLand = true;
		pPos->x = pPos->x + vec.x * fDiff;
		pPos->z = pPos->z + vec.z * fDiff;
	}

	return bLand;
}
