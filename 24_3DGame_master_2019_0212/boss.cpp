//=============================================================================
//
// ボス処理 [boss.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "boss.h"
#include "input.h"
#include "joystick.h"
#include "camera.h"
#include "shadow.h"
#include "meshfield.h"
#include "line.h"
#include "princess.h"
#include "player.h"
#include"game.h"
#include "lifebar.h"
#include "life.h"
#include "wave.h"
#include "obstacle.h"
#include "effect.h"
#include "score.h"
#include "enemywave.h"
#include "enemy.h"
#include "sound.h"
#include "particle.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define BOSS_TEXTURE	"data/TEXTURE/Suede-bk.jpg"	//読み込むテクスチャ名
#define BOSS_TEXTURE2	"data/TEXTURE/white.png"	//読み込むテクスチャ名
#define BOSS_SHADOW	(15.0f)			//影の大きさ
#define BOSS_ROTSPEAD	(0.1f)			//回転速度
#define BOSS_KEY_MAX	(4)				//キーの最大数
#define BOSS_WAVE_MAX	(1)				//wave最大数
#define MAX_CHARA		(256)			//読み込む文字の最大数

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
	int nFrame;			//フレーム数
	KEY	Key[BOSS_MAX];	//キー要素[パーツ数]
} KEY_INFO;

//モーション情報
typedef struct
{
	bool		bLoop;		//ループの有無
	int			nNumKey;	//キー数
	KEY_INFO	KeyInfo[BOSS_KEY_MAX];	//キー情報[キーの数]
} MOTION_INFO;

//=============================================================================
// ロード用の構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos[BOSS_MAX];	//位置
	D3DXVECTOR3	rot[BOSS_MAX];	//角度
	char		cBossFileNeme[BOSS_MAX][MAX_CHARA];	//モデルファイル名
	int			nParent[BOSS_MAX];	//親の番号
	int			nNumBoss = 0;		//読み込むモデル数
	int			nNumBossParts = 0;	//読み込むパーツ数
	float		fSpeed;		//速度
	float		fRadius;	//半径
	int			nMaxLife;	//最大体力
	D3DXVECTOR3	Collpos;			//位置(当たり判定)
	int			nCollParent;		//親の番号(当たり判定)
	float		fCollRadius;		//半径(当たり判定)
	int			nStart;				//開始するフレーム
	int			nEnd;				//終了するフレーム
}LOADBOSS;

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9	g_pTextureBoss[6] = {};	//テクスチャのポインタ
BOSS				g_aBoss[MAX_BOSS];		//ボス情報
LOADBOSS			g_LoadBoss;				//読み込み用
MOTION_INFO	g_aBossMotionInfo[BOSSSTATE_MAX];	//モーション情報
int g_NumBoss = 0;	//ボス数

//=============================================================================
// 初期化処理
//=============================================================================
void InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 位置・向きの初期設定
	LoadBoss();
	g_NumBoss = 0;
	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		g_aBoss[nCntBoss].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].fRadius = g_LoadBoss.fRadius;
		g_aBoss[nCntBoss].fSpeed = g_LoadBoss.fSpeed;
		g_aBoss[nCntBoss].nLife = g_LoadBoss.nMaxLife;
		g_aBoss[nCntBoss].state = BOSSSTATE_NEUTRAL;
		g_aBoss[nCntBoss].bUse = false;
		g_aBoss[nCntBoss].bHit = false;
		g_aBoss[nCntBoss].bDamege = false;
		g_aBoss[nCntBoss].bStop = false;
		g_aBoss[nCntBoss].nCntState = 0;
		g_aBoss[nCntBoss].nCntFrameBoss = 0;
		g_aBoss[nCntBoss].nCntKeyBoss = 0;
		g_aBoss[nCntBoss].nCntAttack = 0;

		g_aBoss[nCntBoss].CollisionBoss.nIdxModelParent = g_LoadBoss.nCollParent;
		g_aBoss[nCntBoss].CollisionBoss.pos = g_LoadBoss.Collpos;
		g_aBoss[nCntBoss].CollisionBoss.fRadius = g_LoadBoss.fCollRadius;
		g_aBoss[nCntBoss].CollisionBoss.nStartFrame = g_LoadBoss.nStart;
		g_aBoss[nCntBoss].CollisionBoss.nEndFrame = g_LoadBoss.nEnd;
		g_aBoss[nCntBoss].CollisionBoss.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].CollisionBoss.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// Xファイルの読み込み
		for (int nCntParts = 0; nCntParts < g_LoadBoss.nNumBossParts; nCntParts++)
		{
			g_aBoss[nCntBoss].aModel[nCntParts].nIdxModelParent = g_LoadBoss.nParent[nCntParts];
			g_aBoss[nCntBoss].aModel[nCntParts].pos = g_LoadBoss.pos[nCntParts];
			g_aBoss[nCntBoss].aModel[nCntParts].rot = g_LoadBoss.rot[nCntParts];
			g_aBoss[nCntBoss].aModel[nCntParts].pFilename = g_LoadBoss.cBossFileNeme[nCntParts];

			D3DXLoadMeshFromX(g_aBoss[nCntBoss].aModel[nCntParts].pFilename,	//読み込むファイル名
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_aBoss[nCntBoss].aModel[nCntParts].pBuffMat,
				NULL,
				&g_aBoss[nCntBoss].aModel[nCntParts].nNumMat,
				&g_aBoss[nCntBoss].aModel[nCntParts].pMesh);
		}
	}

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBoss(void)
{
	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		DeleteShadow(g_aBoss[nCntBoss].nIdxShadowBoss);
		for (int nCntParts = 0; nCntParts < g_LoadBoss.nNumBossParts; nCntParts++)
		{
			// メッシュの開放
			if (g_aBoss[nCntBoss].aModel[nCntParts].pMesh != NULL)
			{
				g_aBoss[nCntBoss].aModel[nCntParts].pMesh->Release();
				g_aBoss[nCntBoss].aModel[nCntParts].pMesh = NULL;
			}

			// マテリアルの開放
			if (g_aBoss[nCntBoss].aModel[nCntParts].pBuffMat != NULL)
			{
				g_aBoss[nCntBoss].aModel[nCntParts].pBuffMat->Release();
				g_aBoss[nCntBoss].aModel[nCntParts].pBuffMat = NULL;
			}
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBoss(void)
{
	CAMERA *pCamera;
	pCamera = GetCamera();
	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (g_aBoss[nCntBoss].bUse)
		{
			g_aBoss[nCntBoss].nCntAttack++;
			g_aBoss[nCntBoss].nCntFrameBoss++;

			g_aBoss[nCntBoss].posOld = g_aBoss[nCntBoss].pos;
			g_aBoss[nCntBoss].CollisionBoss.posOld = g_aBoss[nCntBoss].CollisionBoss.pos;

			if (g_aBoss[nCntBoss].bDamege)
			{	//ダメージを受けたら赤く変わる
				g_aBoss[nCntBoss].nCntState++;
				if (g_aBoss[nCntBoss].nCntState % 10 == 0)
				{	//色を戻す
					g_aBoss[nCntBoss].bDamege = false;
				}
			}
			else if (g_aBoss[nCntBoss].bStop)
			{	//マヒ状態
				g_aBoss[nCntBoss].nCntState++;
				if (g_aBoss[nCntBoss].nCntState % (60 * 3) == 0)
				{	//状態解除
					g_aBoss[nCntBoss].bStop = false;
					g_aBoss[nCntBoss].nCntState = 0;
				}
			}

			//フレーム増加
			if (g_aBoss[nCntBoss].bStop && g_aBoss[nCntBoss].nCntKeyBoss >= 1)
			{	//マヒ状態ではストップ
			}
			else if (g_aBoss[nCntBoss].nCntFrameBoss > g_aBossMotionInfo[g_aBoss[nCntBoss].state].KeyInfo[g_aBoss[nCntBoss].nCntKeyBoss].nFrame)
			{
				g_aBoss[nCntBoss].nCntFrameBoss = 0;
				g_aBoss[nCntBoss].nCntKeyBoss++;	//キー増加

				if (g_aBoss[nCntBoss].nCntKeyBoss >= g_aBossMotionInfo[g_aBoss[nCntBoss].state].nNumKey)
				{
					g_aBoss[nCntBoss].nCntKeyBoss = 0;	//キーリセット
				}
			}

			//モーションの切り替え
			if (g_aBoss[nCntBoss].state != BOSSSTATE_ATTACK && g_aBoss[nCntBoss].state != BOSSSTATE_DETH && g_aBoss[nCntBoss].state != BOSSSTATE_HIT)
			{
				if (g_aBoss[nCntBoss].move.x > 0.5f || g_aBoss[nCntBoss].move.z > 0.5f ||
					g_aBoss[nCntBoss].move.x < -0.5f || g_aBoss[nCntBoss].move.z < -0.5f)
				{	//速度があるときは移動
					g_aBoss[nCntBoss].state = BOSSSTATE_MOVE;
				}
				else
				{	//通常はニュートラルモーション
					g_aBoss[nCntBoss].state = BOSSSTATE_NEUTRAL;
				}
			}

			if (g_aBoss[nCntBoss].state == BOSSSTATE_ATTACK)
			{	//エフェクト表示
				SetEffect(D3DXVECTOR3(g_aBoss[nCntBoss].CollisionBoss.mtxWorld._41, g_aBoss[nCntBoss].CollisionBoss.mtxWorld._42, g_aBoss[nCntBoss].CollisionBoss.mtxWorld._43),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), g_aBoss[nCntBoss].CollisionBoss.fRadius + 5, 10);
			}

			//重力
			g_aBoss[nCntBoss].move.y -= 0.8f;


			//自分との距離を比べて近い方に寄り付く
			float fDistX, fDistZ, fDistans;
			float fDistXPL, fDistZPL, fDistansPL;
			float fDistXPrincess, fDistZPrincess, fDistansPrincess;

			PLAYER *pPlayer;
			pPlayer = GetPlayer();
			fDistXPL = pPlayer->pos.x - g_aBoss[nCntBoss].pos.x;
			fDistZPL = pPlayer->pos.z - g_aBoss[nCntBoss].pos.z;
			fDistansPL = (float)sqrt(fDistXPL * fDistXPL + fDistZPL * fDistZPL);

			PRINCESS *pPrincess;
			pPrincess = GetPrincess();
			fDistXPrincess = pPrincess->pos.x - g_aBoss[nCntBoss].pos.x;
			fDistZPrincess = pPrincess->pos.z - g_aBoss[nCntBoss].pos.z;
			fDistansPrincess = (float)sqrt(fDistXPrincess * fDistXPrincess + fDistZPrincess * fDistZPrincess);

			if (fDistansPL > fDistansPrincess)
			{	//姫に近寄る
				fDistans = fDistansPrincess;
				fDistX = fDistXPrincess;
				fDistZ = fDistZPrincess;
			}
			else
			{	//騎士に近寄る
				fDistans = fDistansPL;
				fDistX = fDistXPL;
				fDistZ = fDistZPL;
			}

			//移動
			if (CollisionPlayer(&g_aBoss[nCntBoss].pos, &g_aBoss[nCntBoss].posOld, g_aBoss[nCntBoss].fRadius) ||
				CollisionPrincess(&g_aBoss[nCntBoss].pos, &g_aBoss[nCntBoss].posOld, g_aBoss[nCntBoss].fRadius,1))
			{	//姫や騎士に接触しているときは動かない
				if (g_aBoss[nCntBoss].state != BOSSSTATE_ATTACK)
				{
					if ((rand() % 180 == 0 || g_aBoss[nCntBoss].nCntAttack % 180 == 0) && g_aBoss[nCntBoss].state != BOSSSTATE_DETH)
					{	//3秒に1回くらいの確率で攻撃
						g_aBoss[nCntBoss].state = BOSSSTATE_ATTACK;
						g_aBoss[nCntBoss].nCntFrameBoss = 0;
						g_aBoss[nCntBoss].nCntKeyBoss = 0;
						PlaySound(SOUND_LABEL_SE_NAIL);
					}
				}
				else
				{
					if (CollisionPrincess(&D3DXVECTOR3(g_aBoss[nCntBoss].CollisionBoss.mtxWorld._41, g_aBoss[nCntBoss].CollisionBoss.mtxWorld._42, g_aBoss[nCntBoss].CollisionBoss.mtxWorld._43), &g_aBoss[nCntBoss].CollisionBoss.posOld, g_aBoss[nCntBoss].CollisionBoss.fRadius,1) &&
						(g_aBoss[nCntBoss].CollisionBoss.nStartFrame <= g_aBoss[nCntBoss].nCntFrameBoss && g_aBoss[nCntBoss].CollisionBoss.nEndFrame >= g_aBoss[nCntBoss].nCntFrameBoss))
					{	//姫体力減少
						DecreaseLifebar(5);
					}
					else if (CollisionPlayer(&D3DXVECTOR3(g_aBoss[nCntBoss].CollisionBoss.mtxWorld._41, g_aBoss[nCntBoss].CollisionBoss.mtxWorld._42, g_aBoss[nCntBoss].CollisionBoss.mtxWorld._43), &g_aBoss[nCntBoss].CollisionBoss.posOld, g_aBoss[nCntBoss].CollisionBoss.fRadius) &&
						(g_aBoss[nCntBoss].CollisionBoss.nStartFrame <= g_aBoss[nCntBoss].nCntFrameBoss && g_aBoss[nCntBoss].CollisionBoss.nEndFrame >= g_aBoss[nCntBoss].nCntFrameBoss))
					{	//プレイヤー体力減少
						HitPlayer(3, &g_aBoss[nCntBoss].pos);
					}
				}
			}
			else
			{
				//移動
				if (g_aBoss[nCntBoss].state != BOSSSTATE_ATTACK && !g_aBoss[nCntBoss].bStop)
				{
					MoveBoss(fDistX, fDistZ, fDistans,nCntBoss);
				}
			}

			OBSTACLE *pObstacle = GetObstacle();

			//障害物破壊
			for (int nCntObstacle = 0; nCntObstacle < OBSTACLE_MAX; nCntObstacle++, pObstacle++)
			{
				if (pObstacle->bUse)
				{
					if (CollisionObstacle(&g_aBoss[nCntBoss].pos, &g_aBoss[nCntBoss].posOld, &g_aBoss[nCntBoss].move, g_aBoss[nCntBoss].fRadius, nCntObstacle))
					{
						g_aBoss[nCntBoss].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						fDistX = pObstacle->pos.x - g_aBoss[nCntBoss].pos.x;
						fDistZ = pObstacle->pos.z - g_aBoss[nCntBoss].pos.z;

						if (g_aBoss[nCntBoss].state != BOSSSTATE_ATTACK && g_aBoss[nCntBoss].nCntAttack % 60 == 0)
						{
							g_aBoss[nCntBoss].state = BOSSSTATE_ATTACK;
							g_aBoss[nCntBoss].nCntFrameBoss = 0;
							g_aBoss[nCntBoss].nCntKeyBoss = 0;
							DeleteObstacle(5, nCntObstacle);
							PlaySound(SOUND_LABEL_SE_NAIL);
						}
					}
				}
			}

			//移動方向を向く
			if (g_aBoss[nCntBoss].state != BOSSSTATE_ATTACK && !g_aBoss[nCntBoss].bStop)
			{
				g_aBoss[nCntBoss].rotDest.y = atan2f(fDistX, fDistZ) + D3DX_PI;
			}
			//回転
			if (g_aBoss[nCntBoss].rotmove.y = g_aBoss[nCntBoss].rotDest.y - g_aBoss[nCntBoss].rot.y)
			{
				if (g_aBoss[nCntBoss].rotmove.y < -D3DX_PI)
				{
					g_aBoss[nCntBoss].rotmove.y += D3DX_PI * 2;
				}
				else if (g_aBoss[nCntBoss].rotmove.y > D3DX_PI)
				{
					g_aBoss[nCntBoss].rotmove.y -= D3DX_PI * 2;
				}
			}
			if (g_aBoss[nCntBoss].rot.y += g_aBoss[nCntBoss].rotmove.y * BOSS_ROTSPEAD)
			{
				if (g_aBoss[nCntBoss].rot.y > D3DX_PI)
				{
					g_aBoss[nCntBoss].rot.y -= D3DX_PI * 2;
				}
				else if (g_aBoss[nCntBoss].rot.y < -D3DX_PI)
				{
					g_aBoss[nCntBoss].rot.y += D3DX_PI * 2;
				}
			}

			//慣性
			g_aBoss[nCntBoss].move.x += (0.0f - g_aBoss[nCntBoss].move.x)*0.25f;
			g_aBoss[nCntBoss].move.z += (0.0f - g_aBoss[nCntBoss].move.z)*0.25f;

			//移動
			g_aBoss[nCntBoss].pos.x += g_aBoss[nCntBoss].move.x;
			g_aBoss[nCntBoss].pos.z += g_aBoss[nCntBoss].move.z;
			g_aBoss[nCntBoss].pos.y += g_aBoss[nCntBoss].move.y;

			WAVE *pWave = GetWave();

			//魔法当たり判定
			if (pWave->state == WAVE_STATE_ATTACK)
			{
				if (CollisionWave(&g_aBoss[nCntBoss].pos, &g_aBoss[nCntBoss].posOld, &g_aBoss[nCntBoss].move, g_aBoss[nCntBoss].fRadius))
				{
					g_aBoss[nCntBoss].state = BOSSSTATE_HIT;
					g_aBoss[nCntBoss].nCntFrameBoss = 0;
					g_aBoss[nCntBoss].nCntKeyBoss = 0;
					g_aBoss[nCntBoss].bStop = true;
				}
			}

			//障害物当たり判定
			pObstacle = GetObstacle();

			for (int nCntObstacle = 0; nCntObstacle < OBSTACLE_MAX; nCntObstacle++, pObstacle++)
			{
				if (pObstacle->bUse)
				{
					CollisionObstacle(&g_aBoss[nCntBoss].pos, &g_aBoss[nCntBoss].posOld, &g_aBoss[nCntBoss].move, g_aBoss[nCntBoss].fRadius, nCntObstacle);
				}
			}


			if (g_aBoss[nCntBoss].pos.y < 0)
			{	//地面にめり込まないように
				g_aBoss[nCntBoss].pos.y = 0;
				g_aBoss[nCntBoss].move.y = 0;
			}
			//左右壁判定
			if (g_aBoss[nCntBoss].pos.x > MESHFIELD_SIZE* MESHFIELD_W * 0.5f - g_aBoss[nCntBoss].fRadius)
			{
				g_aBoss[nCntBoss].pos.x = MESHFIELD_SIZE* MESHFIELD_W * 0.5f - g_aBoss[nCntBoss].fRadius;
			}
			if (g_aBoss[nCntBoss].pos.x < -MESHFIELD_SIZE* MESHFIELD_W * 0.5f + g_aBoss[nCntBoss].fRadius)
			{
				g_aBoss[nCntBoss].pos.x = -MESHFIELD_SIZE* MESHFIELD_W * 0.5f + g_aBoss[nCntBoss].fRadius;
			}

			//奥手前壁判定
			if (g_aBoss[nCntBoss].pos.z > MESHFIELD_SIZE* MESHFIELD_D * 0.5f - g_aBoss[nCntBoss].fRadius)
			{
				g_aBoss[nCntBoss].pos.z = MESHFIELD_SIZE* MESHFIELD_D * 0.5f - g_aBoss[nCntBoss].fRadius;
			}
			if (g_aBoss[nCntBoss].pos.z < -MESHFIELD_SIZE* MESHFIELD_D * 0.5f + g_aBoss[nCntBoss].fRadius)
			{
				g_aBoss[nCntBoss].pos.z = -MESHFIELD_SIZE * MESHFIELD_D * 0.5f + g_aBoss[nCntBoss].fRadius;
			}

			//モーション再生
			if (g_aBoss[nCntBoss].bStop && g_aBoss[nCntBoss].nCntKeyBoss >= 1)
			{
			}
			else
			{
				BossMotion(nCntBoss);
			}

			//影の位置を設定
			SetPositionSadow(g_aBoss[nCntBoss].nIdxShadowBoss, g_aBoss[nCntBoss].pos, BOSS_SHADOW + ((0.0f - g_aBoss[nCntBoss].pos.y)*-0.1f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f - ((0.0f - g_aBoss[nCntBoss].pos.y)*-0.008f)));
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans, mtxParent;	// 計算用マトリックス
	D3DXMATERIAL		*pMat;	//現在のマテリアル保存用
	D3DMATERIAL9		matDef;	//マテリアルデータへのポインタ
	D3DMATERIAL9		matKeep;	//保存用
	BOSS *pBoss = GetBoss();

	//ボスの設定
	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++, pBoss++)
	{
		if (pBoss->bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pBoss->mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pBoss->rot.y, pBoss->rot.x, pBoss->rot.z);

			D3DXMatrixMultiply(&pBoss->mtxWorld, &pBoss->mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pBoss->pos.x, pBoss->pos.y, pBoss->pos.z);

			D3DXMatrixMultiply(&pBoss->mtxWorld, &pBoss->mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pBoss->mtxWorld);

			//モデルの設定
			for (int nCntParts = 0; nCntParts < g_LoadBoss.nNumBossParts; nCntParts++)
			{
				//親の識別
				if (pBoss->aModel[nCntParts].nIdxModelParent == -1)
				{
					mtxParent = pBoss->mtxWorld;
				}
				else
				{
					mtxParent = pBoss->aModel[pBoss->aModel[nCntParts].nIdxModelParent].mtxWorld;
				}
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&pBoss->aModel[nCntParts].mtxWorld);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, pBoss->aModel[nCntParts].rot.y, pBoss->aModel[nCntParts].rot.x, pBoss->aModel[nCntParts].rot.z);

				D3DXMatrixMultiply(&pBoss->aModel[nCntParts].mtxWorld, &pBoss->aModel[nCntParts].mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

				// 位置を反映
				D3DXMatrixTranslation(&mtxTrans, pBoss->aModel[nCntParts].pos.x, pBoss->aModel[nCntParts].pos.y, pBoss->aModel[nCntParts].pos.z);

				D3DXMatrixMultiply(&pBoss->aModel[nCntParts].mtxWorld, &pBoss->aModel[nCntParts].mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

				D3DXMatrixMultiply(&pBoss->aModel[nCntParts].mtxWorld, &pBoss->aModel[nCntParts].mtxWorld, &mtxParent);	//ワールド行列＝ワールド行列＊親のワールド行列
				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &pBoss->aModel[nCntParts].mtxWorld);

				// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);

				// マテリアル情報に対するポインタを取得
				pMat = (D3DXMATERIAL*)pBoss->aModel[nCntParts].pBuffMat->GetBufferPointer();

				// テクスチャの設定をするならここ(1枚の時)

				for (int nCntMat = 0; nCntMat < (int)pBoss->aModel[nCntParts].nNumMat; nCntMat++)
				{
					if (pBoss->bUse == true)
					{
						// テクスチャの設定をするならここ(複数枚の時)
						pDevice->SetTexture(0, g_pTextureBoss[nCntMat]);

						matKeep = pMat[nCntMat].MatD3D;	//保存

						if (pBoss->bDamege)
						{
							pMat[nCntMat].MatD3D.Diffuse.r = 255;	//赤色変更
						}
						else if (g_aBoss[nCntBoss].bStop)
						{	//黄色に変える
							pMat[nCntMat].MatD3D.Diffuse.r = 155;
							pMat[nCntMat].MatD3D.Diffuse.g = 155;
						}

						// マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

						pMat[nCntMat].MatD3D = matKeep;	//色を戻す

						// 描画
						pBoss->aModel[nCntParts].pMesh->DrawSubset(nCntMat);	//格子状態
					}
				}

				// マテリアルをデフォルトに戻す
				pDevice->SetMaterial(&matDef);
			}
			//当たり判定の設定
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBoss[nCntBoss].CollisionBoss.mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBoss[nCntBoss].CollisionBoss.rot.y, g_aBoss[nCntBoss].CollisionBoss.rot.x, g_aBoss[nCntBoss].CollisionBoss.rot.z);

			D3DXMatrixMultiply(&g_aBoss[nCntBoss].CollisionBoss.mtxWorld, &g_aBoss[nCntBoss].CollisionBoss.mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBoss[nCntBoss].CollisionBoss.pos.x, g_aBoss[nCntBoss].CollisionBoss.pos.y, g_aBoss[nCntBoss].CollisionBoss.pos.z);

			D3DXMatrixMultiply(&g_aBoss[nCntBoss].CollisionBoss.mtxWorld, &g_aBoss[nCntBoss].CollisionBoss.mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

			//親子反映
			D3DXMatrixMultiply(&g_aBoss[nCntBoss].CollisionBoss.mtxWorld, &g_aBoss[nCntBoss].CollisionBoss.mtxWorld, &pBoss->aModel[g_aBoss[nCntBoss].CollisionBoss.nIdxModelParent].mtxWorld);	//ワールド行列＝ワールド行列＊親のワールド行列

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBoss[nCntBoss].CollisionBoss.mtxWorld);
		}
	}
}

//=============================================================================
// ボスの設定
//=============================================================================
void SetBoss(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (!g_aBoss[nCntBoss].bUse)
		{
			g_aBoss[nCntBoss].pos = pos;
			g_aBoss[nCntBoss].rot = rot;
			g_aBoss[nCntBoss].state = BOSSSTATE_NEUTRAL;
			g_aBoss[nCntBoss].nLife = g_LoadBoss.nMaxLife;
			g_aBoss[nCntBoss].bUse = true;
			g_NumBoss++;
			//影の作成
			g_aBoss[nCntBoss].nIdxShadowBoss = SetSadow(g_aBoss[nCntBoss].pos, g_aBoss[nCntBoss].rot, BOSS_SHADOW, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
			break;
		}
	}
}

//=============================================================================
// ボスの移動関数
//=============================================================================
void MoveBoss(float fDistX, float fDistZ, float fDistans,int nCntBoss)
{
		//追尾
		g_aBoss[nCntBoss].move.x += (fDistX / fDistans * g_aBoss[nCntBoss].fSpeed);
		g_aBoss[nCntBoss].move.z += (fDistZ / fDistans * g_aBoss[nCntBoss].fSpeed);

		//移動方向を向く
		g_aBoss[nCntBoss].rotDest.y = atan2f(fDistX, fDistZ) + D3DX_PI;

		//回転
		if (g_aBoss[nCntBoss].rotmove.y = g_aBoss[nCntBoss].rotDest.y - g_aBoss[nCntBoss].rot.y)
		{
			if (g_aBoss[nCntBoss].rotmove.y < -D3DX_PI)
			{
				g_aBoss[nCntBoss].rotmove.y += D3DX_PI * 2;
			}
			else if (g_aBoss[nCntBoss].rotmove.y > D3DX_PI)
			{
				g_aBoss[nCntBoss].rotmove.y -= D3DX_PI * 2;
			}
		}
		if (g_aBoss[nCntBoss].rot.y += g_aBoss[nCntBoss].rotmove.y * BOSS_ROTSPEAD)
		{
			if (g_aBoss[nCntBoss].rot.y > D3DX_PI)
			{
				g_aBoss[nCntBoss].rot.y -= D3DX_PI * 2;
			}
			else if (g_aBoss[nCntBoss].rot.y < -D3DX_PI)
			{
				g_aBoss[nCntBoss].rot.y += D3DX_PI * 2;
			}
		}
}

//=============================================================================
// 当たり判定
//=============================================================================
bool CollisionBoss(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Radius,int nCntBoss)
{
	bool bLand = false;
	float fDistance = (pPos->x - g_aBoss[nCntBoss].pos.x)*(pPos->x - g_aBoss[nCntBoss].pos.x) + (pPos->z - g_aBoss[nCntBoss].pos.z)*(pPos->z - g_aBoss[nCntBoss].pos.z);
	float fLength = (Radius + g_aBoss[nCntBoss].fRadius)*(Radius + g_aBoss[nCntBoss].fRadius);
	float fDiff = (Radius + g_aBoss[nCntBoss].fRadius) - (float)sqrt(fDistance);	//差
	D3DXVECTOR3 vec = D3DXVECTOR3((pPos->x - g_aBoss[nCntBoss].pos.x), 0.0f, (pPos->z - g_aBoss[nCntBoss].pos.z));	//ベクトル
	D3DXVec3Normalize(&vec, &vec);

	if (g_aBoss[nCntBoss].bUse)
	{
		if (fDistance < fLength)
		{
			bLand = true;
			pPos->x = pPosOld->x + vec.x * fDiff;
			pPos->z = pPosOld->z + vec.z * fDiff;
		}
	}
	return bLand;
}

//=============================================================================
//	モーションの再生
//=============================================================================
void BossMotion(int nCntBoss)
{
	//パーツ数分
	for (int nCntMotion = 0; nCntMotion < g_LoadBoss.nNumBossParts; nCntMotion++)
	{
		KEY *pKey, *pNextKey;
		int nNextKey = g_aBoss[nCntBoss].nCntKeyBoss + 1;

		if (nNextKey >= g_aBossMotionInfo[g_aBoss[nCntBoss].state].nNumKey)
		{
			if (g_aBossMotionInfo[g_aBoss[nCntBoss].state].bLoop)
			{
				nNextKey = 0;
			}
			else
			{
				if (g_aBoss[nCntBoss].state == BOSSSTATE_DETH /*&& *GetGameState() == GAMESTATE_NORMAL*/)
				{
					g_aBoss[nCntBoss].bUse = false;
					DeleteShadow(g_aBoss[nCntBoss].nIdxShadowBoss);
				}
				else
				{
					g_aBoss[nCntBoss].state = BOSSSTATE_NEUTRAL;
					g_aBoss[nCntBoss].nCntFrameBoss = 0;
					g_aBoss[nCntBoss].nCntKeyBoss = 0;
				}
			}
		}
		pKey = &g_aBossMotionInfo[g_aBoss[nCntBoss].state].KeyInfo[g_aBoss[nCntBoss].nCntKeyBoss].Key[nCntMotion];	//現在のキー	
		pNextKey = &g_aBossMotionInfo[g_aBoss[nCntBoss].state].KeyInfo[nNextKey].Key[nCntMotion];	//次のキー		

		//回転
		g_aBoss[nCntBoss].aModel[nCntMotion].rotMove = (pNextKey->rot - pKey->rot)*((float)g_aBoss[nCntBoss].nCntFrameBoss / (float)g_aBossMotionInfo[g_aBoss[nCntBoss].state].KeyInfo[g_aBoss[nCntBoss].nCntKeyBoss].nFrame);
		g_aBoss[nCntBoss].aModel[nCntMotion].rot = pKey->rot + g_aBoss[nCntBoss].aModel[nCntMotion].rotMove;
	}
}

//=============================================================================
// ダメージ処理
//=============================================================================
void HitBoss(int Damege, D3DXVECTOR3 *pPos, int nCntBoss)
{
	D3DXVECTOR3 vec = D3DXVECTOR3((g_aBoss[nCntBoss].pos.x - pPos->x), 0.0f, (g_aBoss[nCntBoss].pos.z - pPos->z));	//ベクトル
	D3DXVec3Normalize(&vec, &vec);

	if (g_aBoss[nCntBoss].bUse)
	{
		g_aBoss[nCntBoss].nLife -= Damege;
		g_aBoss[nCntBoss].state = BOSSSTATE_HIT;
		g_aBoss[nCntBoss].nCntFrameBoss = 0;
		g_aBoss[nCntBoss].nCntKeyBoss = 0;
		g_aBoss[nCntBoss].bDamege = true;
		g_aBoss[nCntBoss].bStop = false;
		PlaySound(SOUND_LABEL_SE_DAMAGE);

		//パーティクル発生
		float fAngle = 0;
		float fSpeed = 5;
		for (int nCnt = 0; nCnt < 20; nCnt++)
		{
			fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;	//0～3.14を出す　*-1で正負を決める
			D3DXVECTOR3 move = D3DXVECTOR3(sinf(fAngle)*fSpeed, cosf(fAngle)*fSpeed, 0.0f);
			SetParticle(D3DXVECTOR3(g_aBoss[nCntBoss].pos.x, g_aBoss[nCntBoss].aModel[0].pos.y + 20, g_aBoss[nCntBoss].pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, 10, 10);
		}
		//はじき飛ぶ
		g_aBoss[nCntBoss].move.x = g_aBoss[nCntBoss].move.x + vec.x * 10;
		g_aBoss[nCntBoss].move.z = g_aBoss[nCntBoss].move.z + vec.z * 10;

		if (g_aBoss[nCntBoss].nLife <= 0)
		{	//死亡
			AddScore(1500);
			g_aBoss[nCntBoss].state = BOSSSTATE_DETH;
			g_aBoss[nCntBoss].nCntFrameBoss = 0;
			g_aBoss[nCntBoss].nCntKeyBoss = 0;
			g_NumBoss--;

		}
	}
}

//=============================================================================
// ボス情報の取得
//=============================================================================
BOSS *GetBoss(void)
{
	return &g_aBoss[0];
}

int GetBossUse(void)
{
	return g_NumBoss;
}
//=============================================================================
//	ファイル読み込み
//=============================================================================
void LoadBoss(void)
{
	//ファイルポイント
	FILE *pFile;

	//変数宣言
	char *pStrCur = NULL;	//文字列の先頭へのポインタ
	char cLine[MAX_CHARA];	//文字列読み込み用
	char cStr[MAX_CHARA];	//文字列抜き出し用
	char *cPunctuation;				//文字区切り用
	int nCnt = 0;			//空白カウンター
	int nCntModel = 0;		//モデル番号
	int nCntMotion = 0;		//モーション番号
	int nCntKey = 0;		//現在キー
	int nCntParts;			//パーツ番号

	pFile = fopen("data/motion_boss.txt", "r");

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
				g_LoadBoss.nNumBoss = atoi(cStr);	//atoi　char型をint型に変換する
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
					strcpy(g_LoadBoss.cBossFileNeme[nCntModel], pStrCur);
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
							g_LoadBoss.fSpeed = (float)atof(cStr);	//atof　char型をfloat型に変換する
						}
						if (memcmp(&cLine[nCnt], "RADIUS = ", strlen("RADIUS = ")) == 0)
						{	//半径
							pStrCur += strlen("RADIUS = ");
							strcpy(cStr, pStrCur);
							g_LoadBoss.fRadius = (float)atof(cStr);
						}
						if (memcmp(&cLine[nCnt], "LIFE = ", strlen("LIFE = ")) == 0)
						{	//体力
							pStrCur += strlen("LIFE = ");
							strcpy(cStr, pStrCur);
							g_LoadBoss.nMaxLife = atoi(cStr);
						}
						if (memcmp(&cLine[nCnt], "NUM_PARTS = ", strlen("NUM_PARTS = ")) == 0)
						{	//読み込むパーツ数
							pStrCur += strlen("NUM_PARTS = ");
							strcpy(cStr, pStrCur);
							g_LoadBoss.nNumBossParts = atoi(cStr);
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
									g_LoadBoss.nParent[nCntParts] = atoi(cStr);
								}
								if (memcmp(&cLine[nCnt], "POS = ", strlen("POS = ")) == 0)
								{	//位置
									pStrCur += strlen("POS = ");
									strcpy(cStr, pStrCur);

									cPunctuation = strtok(cStr, " ");	//文字を空白ごとに区切って分解
									g_LoadBoss.pos[nCntParts].x = (float)atof(cPunctuation);

									cPunctuation = strtok(NULL, " ");
									g_LoadBoss.pos[nCntParts].y = (float)atof(cPunctuation);

									cPunctuation = strtok(NULL, " ");
									g_LoadBoss.pos[nCntParts].z = (float)atof(cPunctuation);
								}
								if (memcmp(&cLine[nCnt], "ROT = ", strlen("ROT = ")) == 0)
								{	//角度
									pStrCur += strlen("ROT = ");
									strcpy(cStr, pStrCur);
									cPunctuation = strtok(cStr, " ");
									g_LoadBoss.rot[nCntParts].x = (float)atof(cPunctuation);
									cPunctuation = strtok(NULL, " ");
									g_LoadBoss.rot[nCntParts].y = (float)atof(cPunctuation);
									cPunctuation = strtok(NULL, " ");
									g_LoadBoss.rot[nCntParts].z = (float)atof(cPunctuation);
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
						g_aBossMotionInfo[nCntMotion].bLoop = atoi(cStr);
					}
					if (memcmp(&cLine[nCnt], "NUM_KEY = ", strlen("NUM_KEY = ")) == 0)
					{	//キー数
						pStrCur += strlen("NUM_KEY = ");
						strcpy(cStr, pStrCur);
						g_aBossMotionInfo[nCntMotion].nNumKey = atoi(cStr);
					}
					if (memcmp(&cLine[nCnt], "COLLISION = ", strlen("COLLISION = ")) == 0)
					{	//当たり判定
						pStrCur += strlen("COLLISION = ");
						strcpy(cStr, pStrCur);

						//親の番号
						cPunctuation = strtok(cStr, " ");
						g_LoadBoss.nCollParent = atoi(cPunctuation);
						//オフセット値
						//X
						cPunctuation = strtok(NULL, " ");
						g_LoadBoss.Collpos.x = (float)atof(cPunctuation);
						//Y
						cPunctuation = strtok(NULL, " ");
						g_LoadBoss.Collpos.y = (float)atof(cPunctuation);
						//Z
						cPunctuation = strtok(NULL, " ");
						g_LoadBoss.Collpos.z = (float)atof(cPunctuation);
						//半径
						cPunctuation = strtok(NULL, " ");
						g_LoadBoss.fCollRadius = (float)atof(cPunctuation);
						//当たり判定を開始するフレーム数
						cPunctuation = strtok(NULL, " ");
						g_LoadBoss.nStart = atoi(cPunctuation);
						//終了するフレーム数
						cPunctuation = strtok(NULL, " ");
						g_LoadBoss.nEnd = atoi(cPunctuation);
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
								g_aBossMotionInfo[nCntMotion].KeyInfo[nCntKey].nFrame = atoi(cStr);
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
										g_aBossMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos.x = (float)atof(cPunctuation);
										cPunctuation = strtok(NULL, " ");
										g_aBossMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos.y = (float)atof(cPunctuation);
										cPunctuation = strtok(NULL, " ");
										g_aBossMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos.z = (float)atof(cPunctuation);
									}
									if (memcmp(&cLine[nCnt], "ROT = ", strlen("ROT = ")) == 0)
									{	//角度
										pStrCur += strlen("ROT = ");
										strcpy(cStr, pStrCur);
										cPunctuation = strtok(cStr, " ");
										g_aBossMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot.x = (float)atof(cPunctuation);
										cPunctuation = strtok(NULL, " ");
										g_aBossMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot.y = (float)atof(cPunctuation);
										cPunctuation = strtok(NULL, " ");
										g_aBossMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot.z = (float)atof(cPunctuation);
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
