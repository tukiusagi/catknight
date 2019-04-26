//=============================================================================
//
// 敵処理 [enemy.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "enemy.h"
#include "input.h"
#include "joystick.h"
#include "camera.h"
#include "shadow.h"
#include "meshfield.h"
#include "line.h"
#include "princess.h"
#include "player.h"
#include "game.h"
#include "boss.h"
#include "lifebar.h"
#include "life.h"
#include "wave.h"
#include "obstacle.h"
#include "effect.h"
#include "score.h"
#include "sound.h"
#include "particle.h"
#include "enemywave.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define ENEMY_TEXTURE	"data/TEXTURE/Suede-bk.jpg"	//読み込むテクスチャ名
#define ENEMY_TEXTURE2	"data/TEXTURE/white.png"	//読み込むテクスチャ名
#define ENEMY_SHADOW	(15.0f)			//影の大きさ
#define ENEMY_ROTSPEAD	(0.1f)			//回転速度
#define ENEMY_KEY_MAX	(4)				//キーの最大数
#define MAX_CHARA		(256)			//読み込む文字の最大数
#define ENEMY_SCORE		(300)			//読み込む文字の最大数

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
	KEY	Key[ENEMY_MAX];	//キー要素[パーツ数]
} KEY_INFO;

//モーション情報
typedef struct
{
	bool		bLoop;		//ループの有無
	int			nNumKey;	//キー数
	KEY_INFO	KeyInfo[ENEMY_KEY_MAX];	//キー情報[キーの数]
} MOTION_INFO;

//=============================================================================
// ロード用の構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos[ENEMY_MAX];	//位置
	D3DXVECTOR3	rot[ENEMY_MAX];	//角度
	char		cEnemyFileNeme[ENEMY_MAX][MAX_CHARA];	//モデルファイル名
	int			nParent[ENEMY_MAX];	//親の番号
	int			nNumEnemy;			//読み込むモデル数
	int			nNumEnemyParts;		//読み込むパーツ数
	int			nMaxLife;			//最大体力
	float		fSpeed;				//速度
	float		fRadius;			//半径
	D3DXVECTOR3	Collpos;			//位置(当たり判定)
	int			nCollParent;		//親の番号(当たり判定)
	float		fCollRadius;		//半径(当たり判定)
	int			nStart;				//開始するフレーム
	int			nEnd;				//終了するフレーム
}LOADENEMY;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
#ifdef _DEBUG
void DrawMaxEnemy(void);
#endif

//=============================================================================
// グローバル変数
//=============================================================================
ENEMY				g_aEnemy[MAX_ENEMY];					//敵情報
LOADENEMY	g_LoadEnemy;
LPDIRECT3DTEXTURE9	g_pTextureEnemy[6] = {};	//テクスチャのポインタ
int g_nEnemyMax = 0;		//敵の総数

MOTION_INFO	g_aEnemyMotionInfo[ENEMYSTATE_MAX];

#ifdef _DEBUG
LPD3DXFONT	g_pFontEnemy = NULL;			// フォントへのポインタ
int			g_nCountEnemy = 0;		//敵の総数
#endif

//=============================================================================
// 初期化処理
//=============================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_nEnemyMax = 0;

	LoadEnemy();

	// 位置・向きの初期設定
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NEUTRAL;
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].bHit = false;
		g_aEnemy[nCntEnemy].bDamege = false;
		g_aEnemy[nCntEnemy].bStop = false;
		g_aEnemy[nCntEnemy].nCntFrameEnemy = 0;
		g_aEnemy[nCntEnemy].nCntKeyEnemy = 0;
		g_aEnemy[nCntEnemy].nCntAttack = 0;
		g_aEnemy[nCntEnemy].nCntState = 0;
		g_aEnemy[nCntEnemy].fRadius = g_LoadEnemy.fRadius;
		g_aEnemy[nCntEnemy].fSpeed = g_LoadEnemy.fSpeed;
		g_aEnemy[nCntEnemy].nLife = g_LoadEnemy.nMaxLife;

		g_aEnemy[nCntEnemy].CollisionEnemy.nIdxModelParent = g_LoadEnemy.nCollParent;
		g_aEnemy[nCntEnemy].CollisionEnemy.pos = g_LoadEnemy.Collpos;
		g_aEnemy[nCntEnemy].CollisionEnemy.fRadius = g_LoadEnemy.fCollRadius;
		g_aEnemy[nCntEnemy].CollisionEnemy.nStartFrame = g_LoadEnemy.nStart;
		g_aEnemy[nCntEnemy].CollisionEnemy.nEndFrame = g_LoadEnemy.nEnd;
		g_aEnemy[nCntEnemy].CollisionEnemy.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].CollisionEnemy.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// Xファイルの読み込み
		for (int nCntParts = 0; nCntParts < g_LoadEnemy.nNumEnemyParts; nCntParts++)
		{
			g_aEnemy[nCntEnemy].aModel[nCntParts].nIdxModelParent = g_LoadEnemy.nParent[nCntParts];
			g_aEnemy[nCntEnemy].aModel[nCntParts].pos = g_LoadEnemy.pos[nCntParts];
			g_aEnemy[nCntEnemy].aModel[nCntParts].rot = g_LoadEnemy.rot[nCntParts];
			g_aEnemy[nCntEnemy].aModel[nCntParts].pFilename = g_LoadEnemy.cEnemyFileNeme[nCntParts];

			D3DXLoadMeshFromX(g_aEnemy[nCntEnemy].aModel[nCntParts].pFilename,	//読み込むファイル名
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_aEnemy[nCntEnemy].aModel[nCntParts].pBuffMat,
				NULL,						
				&g_aEnemy[nCntEnemy].aModel[nCntParts].nNumMat,
				&g_aEnemy[nCntEnemy].aModel[nCntParts].pMesh);
		}
	}

#ifdef _DEBUG
	// デバッグ表示用フォントを設定
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "ＭＳ ゴシック", &g_pFontEnemy);
#endif

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		for (int nCntParts = 0; nCntParts < g_LoadEnemy.nNumEnemyParts; nCntParts++)
		{
			// メッシュの開放
			if (g_aEnemy[nCntEnemy].aModel[nCntParts].pMesh != NULL)
			{
				g_aEnemy[nCntEnemy].aModel[nCntParts].pMesh->Release();
				g_aEnemy[nCntEnemy].aModel[nCntParts].pMesh = NULL;
			}

			// マテリアルの開放
			if (g_aEnemy[nCntEnemy].aModel[nCntParts].pBuffMat != NULL)
			{
				g_aEnemy[nCntEnemy].aModel[nCntParts].pBuffMat->Release();
				g_aEnemy[nCntEnemy].aModel[nCntParts].pBuffMat = NULL;
			}
		}
	}

#ifdef _DEBUG
	if (g_pFontEnemy != NULL)
	{// デバッグ表示用フォントの開放
		g_pFontEnemy->Release();
		g_pFontEnemy = NULL;
	}
#endif

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	CAMERA *pCamera;
	pCamera = GetCamera();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse)
		{
			g_aEnemy[nCntEnemy].nCntFrameEnemy++;
			g_aEnemy[nCntEnemy].nCntAttack++;

			g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;
			g_aEnemy[nCntEnemy].CollisionEnemy.posOld = g_aEnemy[nCntEnemy].CollisionEnemy.pos;

			if (g_aEnemy[nCntEnemy].bDamege)
			{	//ダメージ状態の解除
				g_aEnemy[nCntEnemy].nCntState++;
				if (g_aEnemy[nCntEnemy].nCntState % 10 == 0)
				{
					g_aEnemy[nCntEnemy].bDamege = false;
					g_aEnemy[nCntEnemy].nCntState = 0;
				}
			}
			else if (g_aEnemy[nCntEnemy].bStop)
			{	//マヒ状態の解除
				g_aEnemy[nCntEnemy].nCntState++;
				if (g_aEnemy[nCntEnemy].nCntState % (60 * 3) == 0)
				{
					g_aEnemy[nCntEnemy].bStop = false;
					g_aEnemy[nCntEnemy].nCntState = 0;
				}
			}

			//フレーム増加
			if (g_aEnemy[nCntEnemy].bStop && g_aEnemy[nCntEnemy].nCntKeyEnemy >= 1)
			{	//マヒ状態ではストップ
			}
			else if (g_aEnemy[nCntEnemy].nCntFrameEnemy > g_aEnemyMotionInfo[g_aEnemy[nCntEnemy].state].KeyInfo[g_aEnemy[nCntEnemy].nCntKeyEnemy].nFrame)
			{
				g_aEnemy[nCntEnemy].nCntFrameEnemy = 0;
				g_aEnemy[nCntEnemy].nCntKeyEnemy++;	//キー増加

				if (g_aEnemy[nCntEnemy].nCntKeyEnemy >= g_aEnemyMotionInfo[g_aEnemy[nCntEnemy].state].nNumKey)
				{
					g_aEnemy[nCntEnemy].nCntKeyEnemy = 0;	//キーリセット
				}
			}

			//モーションの切り替え
			if (g_aEnemy[nCntEnemy].state != ENEMYSTATE_ATTACK && g_aEnemy[nCntEnemy].state != ENEMYSTATE_DETH && g_aEnemy[nCntEnemy].state != ENEMYSTATE_HIT)
			{
				if (g_aEnemy[nCntEnemy].move.x > 0.5f || g_aEnemy[nCntEnemy].move.z > 0.5f ||
					g_aEnemy[nCntEnemy].move.x < -0.5f || g_aEnemy[nCntEnemy].move.z < -0.5f)
				{	//速度があるときは移動
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_MOVE;
				}
				else
				{	//通常はニュートラルモーション
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NEUTRAL;
				}
			}

			if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_ATTACK)
			{	//エフェクト表示
				SetEffect(D3DXVECTOR3(g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld._41, g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld._42, g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld._43),
					D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), g_aEnemy[nCntEnemy].CollisionEnemy.fRadius + 5, 10);
			}

			//重力
			g_aEnemy[nCntEnemy].move.y -= 0.8f;

			//自分との距離を比べて近い方に寄り付く
			float fDistX, fDistZ, fDistans;
			float fDistXPL, fDistZPL, fDistansPL;
			float fDistXPrincess, fDistZPrincess, fDistansPrincess;
			PLAYER *pPlayer;
			pPlayer = GetPlayer();
			PRINCESS *pPrincess;
			pPrincess = GetPrincess();

			if (*GetMode() == MODE_GAME)
			{
				//プレイヤーとの距離計算
				fDistXPL = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;
				fDistZPL = pPlayer->pos.z - g_aEnemy[nCntEnemy].pos.z;
				fDistansPL = (float)sqrt(fDistXPL * fDistXPL + fDistZPL * fDistZPL);
				//姫との距離計算
				fDistXPrincess = pPrincess->pos.x - g_aEnemy[nCntEnemy].pos.x;
				fDistZPrincess = pPrincess->pos.z - g_aEnemy[nCntEnemy].pos.z;
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
			}
			//移動
			if (CollisionPlayer(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, g_aEnemy[nCntEnemy].fRadius) ||
				CollisionPrincess(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, g_aEnemy[nCntEnemy].fRadius, 1))
			{	//姫や騎士に接触しているときは動かない
				if (*GetMode() == MODE_GAME)
				{
					if (g_aEnemy[nCntEnemy].state != ENEMYSTATE_ATTACK)
					{

						if ((rand() % 180 == 0 || g_aEnemy[nCntEnemy].nCntAttack % 180 == 0) && g_aEnemy[nCntEnemy].state != ENEMYSTATE_DETH)
						{	//3秒に1回くらいの確率で攻撃
							g_aEnemy[nCntEnemy].state = ENEMYSTATE_ATTACK;
							g_aEnemy[nCntEnemy].nCntFrameEnemy = 0;
							g_aEnemy[nCntEnemy].nCntKeyEnemy = 0;
							g_aEnemy[nCntEnemy].nCntAttack = 0;
							PlaySound(SOUND_LABEL_SE_NAIL);
						}
					}
					else
					{	//攻撃時
						if (pPrincess->state != PRINCESSSTATE_DETH &&
							CollisionPrincess(&D3DXVECTOR3(g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld._41, g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld._42, g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld._43), &g_aEnemy[nCntEnemy].CollisionEnemy.posOld, g_aEnemy[nCntEnemy].CollisionEnemy.fRadius, 1) &&
							(g_aEnemy[nCntEnemy].CollisionEnemy.nStartFrame <= g_aEnemy[nCntEnemy].nCntFrameEnemy && g_aEnemy[nCntEnemy].CollisionEnemy.nEndFrame >= g_aEnemy[nCntEnemy].nCntFrameEnemy))
						{	//姫体力減少
							DecreaseLifebar(3);
						}
						else if (pPlayer->state != PLAYERSTATE_DETH &&
							CollisionPlayer(&D3DXVECTOR3(g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld._41, g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld._42, g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld._43), &g_aEnemy[nCntEnemy].CollisionEnemy.posOld, g_aEnemy[nCntEnemy].CollisionEnemy.fRadius) &&
							(g_aEnemy[nCntEnemy].CollisionEnemy.nStartFrame <= g_aEnemy[nCntEnemy].nCntFrameEnemy && g_aEnemy[nCntEnemy].CollisionEnemy.nEndFrame >= g_aEnemy[nCntEnemy].nCntFrameEnemy))
						{	//プレイヤー体力減少
							HitPlayer(1, &g_aEnemy[nCntEnemy].pos);
						}
					}
				}
			}
			else
			{
				if (g_aEnemy[nCntEnemy].state != ENEMYSTATE_ATTACK && !g_aEnemy[nCntEnemy].bStop && *GetMode() == MODE_GAME)
				{
					//移動
					MoveEnemy(fDistX, fDistZ, fDistans, nCntEnemy);
				}
			}

			OBSTACLE *pObstacle = GetObstacle();

			//障害物判定
			for (int nCntObstacle = 0; nCntObstacle < OBSTACLE_MAX; nCntObstacle++, pObstacle++)
			{
				if (pObstacle->bUse)
				{
					if (CollisionObstacle(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move, g_aEnemy[nCntEnemy].fRadius, nCntObstacle))
					{
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						fDistX = pObstacle->pos.x - g_aEnemy[nCntEnemy].pos.x;
						fDistZ = pObstacle->pos.z - g_aEnemy[nCntEnemy].pos.z;

						if (g_aEnemy[nCntEnemy].state != ENEMYSTATE_ATTACK && g_aEnemy[nCntEnemy].nCntAttack % 60 == 0 && g_aEnemy[nCntEnemy].state != ENEMYSTATE_DETH)
						{	//障害物破壊
							g_aEnemy[nCntEnemy].state = ENEMYSTATE_ATTACK;
							g_aEnemy[nCntEnemy].nCntFrameEnemy = 0;
							g_aEnemy[nCntEnemy].nCntKeyEnemy = 0;
							DeleteObstacle(1, nCntObstacle);
							PlaySound(SOUND_LABEL_SE_NAIL);
						}
					}
				}
			}

			//移動方向を向く
			if (g_aEnemy[nCntEnemy].state != ENEMYSTATE_ATTACK && !g_aEnemy[nCntEnemy].bStop && *GetMode() == MODE_GAME)
			{
				g_aEnemy[nCntEnemy].rotDest.y = atan2f(fDistX, fDistZ) + D3DX_PI;
			}

			//回転
			if (g_aEnemy[nCntEnemy].rotmove.y = g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y)
			{
				if (g_aEnemy[nCntEnemy].rotmove.y < -D3DX_PI)
				{
					g_aEnemy[nCntEnemy].rotmove.y += D3DX_PI * 2;
				}
				else if (g_aEnemy[nCntEnemy].rotmove.y > D3DX_PI)
				{
					g_aEnemy[nCntEnemy].rotmove.y -= D3DX_PI * 2;
				}
			}
			if (g_aEnemy[nCntEnemy].rot.y += g_aEnemy[nCntEnemy].rotmove.y * ENEMY_ROTSPEAD)
			{
				if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
				{
					g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
				}
				else if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
				{
					g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
				}
			}

			//慣性
			g_aEnemy[nCntEnemy].move.x += (0.0f - g_aEnemy[nCntEnemy].move.x)*0.25f;
			g_aEnemy[nCntEnemy].move.z += (0.0f - g_aEnemy[nCntEnemy].move.z)*0.25f;


			//移動
			g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
			g_aEnemy[nCntEnemy].pos.z += g_aEnemy[nCntEnemy].move.z;
			g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y;


			WAVE *pWave = GetWave();

			if (pWave->state == WAVE_STATE_ATTACK)
			{
				if (CollisionWave(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move, g_aEnemy[nCntEnemy].fRadius))
				{	//魔法当たり判定
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_HIT;
					g_aEnemy[nCntEnemy].nCntFrameEnemy = 0;
					g_aEnemy[nCntEnemy].nCntKeyEnemy = 0;
					g_aEnemy[nCntEnemy].bStop = true;
				}
			}

			//当たり判定
			pObstacle = GetObstacle();

			for (int nCntObstacle = 0; nCntObstacle < OBSTACLE_MAX; nCntObstacle++, pObstacle++)
			{
				if (pObstacle->bUse)
				{
					CollisionObstacle(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move, g_aEnemy[nCntEnemy].fRadius, nCntObstacle);
				}
			}


			if (g_aEnemy[nCntEnemy].pos.y < 0)
			{	//地面にめり込まないように
				g_aEnemy[nCntEnemy].pos.y = 0;
				g_aEnemy[nCntEnemy].move.y = 0;
			}
			//左右壁判定
			if (g_aEnemy[nCntEnemy].pos.x > MESHFIELD_SIZE* MESHFIELD_W * 0.5f - g_aEnemy[nCntEnemy].fRadius)
			{
				g_aEnemy[nCntEnemy].pos.x = MESHFIELD_SIZE* MESHFIELD_W * 0.5f - g_aEnemy[nCntEnemy].fRadius;
			}
			if (g_aEnemy[nCntEnemy].pos.x < -MESHFIELD_SIZE* MESHFIELD_W * 0.5f + g_aEnemy[nCntEnemy].fRadius)
			{
				g_aEnemy[nCntEnemy].pos.x = -MESHFIELD_SIZE* MESHFIELD_W * 0.5f + g_aEnemy[nCntEnemy].fRadius;
			}

			//奥手前壁判定
			if (g_aEnemy[nCntEnemy].pos.z > MESHFIELD_SIZE* MESHFIELD_D * 0.5f - g_aEnemy[nCntEnemy].fRadius)
			{
				g_aEnemy[nCntEnemy].pos.z = MESHFIELD_SIZE* MESHFIELD_D * 0.5f - g_aEnemy[nCntEnemy].fRadius;
			}
			if (g_aEnemy[nCntEnemy].pos.z < -MESHFIELD_SIZE* MESHFIELD_D * 0.5f + g_aEnemy[nCntEnemy].fRadius)
			{
				g_aEnemy[nCntEnemy].pos.z = -MESHFIELD_SIZE * MESHFIELD_D * 0.5f + g_aEnemy[nCntEnemy].fRadius;
			}

			//モーション再生
			if (g_aEnemy[nCntEnemy].bStop && g_aEnemy[nCntEnemy].nCntKeyEnemy >= 1)
			{
			}
			else
			{
				EnemyMotion(nCntEnemy);
			}
			//影の位置を設定
			SetPositionSadow(g_aEnemy[nCntEnemy].nIdxShadowEnemy, g_aEnemy[nCntEnemy].pos, ENEMY_SHADOW + ((0.0f - g_aEnemy[nCntEnemy].pos.y)*-0.1f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f - ((0.0f - g_aEnemy[nCntEnemy].pos.y)*-0.008f)));
		}
	}

#ifdef _DEBUG
	g_nCountEnemy = g_nEnemyMax;
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans, mtxParent;	// 計算用マトリックス
	D3DXMATERIAL		*pMat;	//現在のマテリアル保存用
	D3DMATERIAL9		matDef;	//マテリアルデータへのポインタ
	D3DMATERIAL9		matKeep;	//保存用

//敵の設定
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);

		D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);

		D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);

		//モデルの設定
		for (int nCntParts = 0; nCntParts < g_LoadEnemy.nNumEnemyParts; nCntParts++)
		{
			//親の識別
			if (g_aEnemy[nCntEnemy].aModel[nCntParts].nIdxModelParent == -1)
			{
				mtxParent = g_aEnemy[nCntEnemy].mtxWorld;
			}
			else
			{
				mtxParent = g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].aModel[nCntParts].nIdxModelParent].mtxWorld;
			}
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].aModel[nCntParts].rot.y, g_aEnemy[nCntEnemy].aModel[nCntParts].rot.x, g_aEnemy[nCntEnemy].aModel[nCntParts].rot.z);

			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld, &g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].aModel[nCntParts].pos.x, g_aEnemy[nCntEnemy].aModel[nCntParts].pos.y, g_aEnemy[nCntEnemy].aModel[nCntParts].pos.z);

			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld, &g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld, &g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld, &mtxParent);	//ワールド行列＝ワールド行列＊親のワールド行列
			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアル情報に対するポインタを取得
			pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].aModel[nCntParts].pBuffMat->GetBufferPointer();

			// テクスチャの設定をするならここ(1枚の時)

			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].aModel[nCntParts].nNumMat; nCntMat++)
			{
				if (g_aEnemy[nCntEnemy].bUse == true)
				{
					// テクスチャの設定をするならここ(複数枚の時)
					pDevice->SetTexture(0, g_pTextureEnemy[nCntMat]);

					matKeep = pMat[nCntMat].MatD3D;	//情報の保存

					if (g_aEnemy[nCntEnemy].bDamege)
					{	//ダメージ時赤色に変える
						pMat[nCntMat].MatD3D.Diffuse.r = 255;
					}
					else if (g_aEnemy[nCntEnemy].bStop)
					{	//マヒ状態で黄色に変える
						pMat[nCntMat].MatD3D.Diffuse.r = 155;	
						pMat[nCntMat].MatD3D.Diffuse.g = 155;	
					}

					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					pMat[nCntMat].MatD3D = matKeep;	//色を戻す

					// 描画
					g_aEnemy[nCntEnemy].aModel[nCntParts].pMesh->DrawSubset(nCntMat);	//格子状態
				}
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
		//当たり判定の設定
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].CollisionEnemy.rot.y, g_aEnemy[nCntEnemy].CollisionEnemy.rot.x, g_aEnemy[nCntEnemy].CollisionEnemy.rot.z);

		D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld, &g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].CollisionEnemy.pos.x, g_aEnemy[nCntEnemy].CollisionEnemy.pos.y, g_aEnemy[nCntEnemy].CollisionEnemy.pos.z);

		D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld, &g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

		//親子反映
		D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld, &g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld, &g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].CollisionEnemy.nIdxModelParent].mtxWorld);	//ワールド行列＝ワールド行列＊親のワールド行列

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld);
	}

#ifdef _DEBUG
	// 敵総数表示
	DrawMaxEnemy();
#endif

}

//=============================================================================
// 敵の設定
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int Life)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (!g_aEnemy[nCntEnemy].bUse)	//敵が使用されていない場合
		{
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].rot = rot;
			g_aEnemy[nCntEnemy].nLife = Life;
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_NEUTRAL;
			g_aEnemy[nCntEnemy].bUse = true;

			//影の作成
			g_aEnemy[nCntEnemy].nIdxShadowEnemy = SetSadow(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].rot, ENEMY_SHADOW, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));

			g_nEnemyMax++;	//敵の総数をカウント
			break;
		}
	}
}

//=============================================================================
// 敵の消去
//=============================================================================
void DeleteEnemey(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse)	//敵が使用されている場合
		{
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_NEUTRAL;
			g_aEnemy[nCntEnemy].bUse = false;
			g_aEnemy[nCntEnemy].bHit = false;
			g_aEnemy[nCntEnemy].bDamege = false;
			g_aEnemy[nCntEnemy].bStop = false;
			g_nEnemyMax--;
			DeleteShadow(g_aEnemy[nCntEnemy].nIdxShadowEnemy);

		}
	}

}
//=============================================================================
// 敵の移動関数
//=============================================================================
void MoveEnemy(float fDistX, float fDistZ, float fDistans, int nCntEnemy)
{
	float fSpeed = g_aEnemy[nCntEnemy].fSpeed;
	//追尾
	if (*GetEwave() <= 3)
	{	//序盤は移動速度半減
		fSpeed = g_aEnemy[nCntEnemy].fSpeed / 2.0f;
	}
	g_aEnemy[nCntEnemy].move.x += (fDistX / fDistans * fSpeed);
	g_aEnemy[nCntEnemy].move.z += (fDistZ / fDistans * fSpeed);

	//移動方向を向く
	g_aEnemy[nCntEnemy].rotDest.y = atan2f(fDistX, fDistZ)+ D3DX_PI;

	//回転
	if (g_aEnemy[nCntEnemy].rotmove.y = g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y)
	{
		if (g_aEnemy[nCntEnemy].rotmove.y < -D3DX_PI)
		{
			g_aEnemy[nCntEnemy].rotmove.y += D3DX_PI * 2;
		}
		else if (g_aEnemy[nCntEnemy].rotmove.y > D3DX_PI)
		{
			g_aEnemy[nCntEnemy].rotmove.y -= D3DX_PI * 2;
		}
	}
	if (g_aEnemy[nCntEnemy].rot.y += g_aEnemy[nCntEnemy].rotmove.y * ENEMY_ROTSPEAD)
	{
		if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
		{
			g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
		}
		else if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
		{
			g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
		}
	}
	
}

//=============================================================================
// 当たり判定
//=============================================================================
bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Radius, int nCntEnemy)
{
	bool bLand = false;
	float fDistance = (pPos->x - g_aEnemy[nCntEnemy].pos.x)*(pPos->x - g_aEnemy[nCntEnemy].pos.x) + (pPos->z - g_aEnemy[nCntEnemy].pos.z)*(pPos->z - g_aEnemy[nCntEnemy].pos.z);	//距離
	float fLength = (Radius + g_aEnemy[nCntEnemy].fRadius)*(Radius + g_aEnemy[nCntEnemy].fRadius);	//長さ
	float fDiff = (Radius + g_aEnemy[nCntEnemy].fRadius) - (float)sqrt(fDistance);	//差
	D3DXVECTOR3 vec = D3DXVECTOR3((pPos->x - g_aEnemy[nCntEnemy].pos.x), 0.0f, (pPos->z - g_aEnemy[nCntEnemy].pos.z));	//ベクトル
	D3DXVec3Normalize(&vec, &vec);

	if (fDistance < fLength)
	{
		bLand = true;

		pPos->x = pPosOld->x + vec.x * fDiff;
		pPos->z = pPosOld->z + vec.z * fDiff;
	}

	return bLand;
}

//=============================================================================
//	モーションの再生
//=============================================================================
void EnemyMotion(int nCntEnemy)
{
	//パーツ数分
	for (int nCntMotion = 0; nCntMotion < g_LoadEnemy.nNumEnemyParts; nCntMotion++)
	{
		KEY *pKey, *pNextKey;
		int nNextKey = g_aEnemy[nCntEnemy].nCntKeyEnemy + 1;

		if (nNextKey >= g_aEnemyMotionInfo[g_aEnemy[nCntEnemy].state].nNumKey)
		{
			if (g_aEnemyMotionInfo[g_aEnemy[nCntEnemy].state].bLoop)
			{
				nNextKey = 0;
			}
			else
			{
				if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_DETH)
				{	//死亡時
					if (g_aEnemy[nCntEnemy].bUse)
					{
						g_aEnemy[nCntEnemy].bUse = false;
						if (*GetMode() == MODE_GAME)
						{
							AddScore(ENEMY_SCORE);	//スコア加算
						}
						g_nEnemyMax--;
						DeleteShadow(g_aEnemy[nCntEnemy].nIdxShadowEnemy);
					}
				}
				else
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NEUTRAL;
					g_aEnemy[nCntEnemy].nCntFrameEnemy = 0;
					g_aEnemy[nCntEnemy].nCntKeyEnemy = 0;
				}
			}
		}
		pKey = &g_aEnemyMotionInfo[g_aEnemy[nCntEnemy].state].KeyInfo[g_aEnemy[nCntEnemy].nCntKeyEnemy].Key[nCntMotion];	//現在のキー	
		pNextKey = &g_aEnemyMotionInfo[g_aEnemy[nCntEnemy].state].KeyInfo[nNextKey].Key[nCntMotion];	//次のキー		

		//回転
		g_aEnemy[nCntEnemy].aModel[nCntMotion].rotMove = (pNextKey->rot - pKey->rot)*((float)g_aEnemy[nCntEnemy].nCntFrameEnemy / (float)g_aEnemyMotionInfo[g_aEnemy[nCntEnemy].state].KeyInfo[g_aEnemy[nCntEnemy].nCntKeyEnemy].nFrame);
		g_aEnemy[nCntEnemy].aModel[nCntMotion].rot = pKey->rot + g_aEnemy[nCntEnemy].aModel[nCntMotion].rotMove;
	}
}

//=============================================================================
// ダメージ処理
//=============================================================================
void HitEnemy(int nCntEnemy, int Damege, D3DXVECTOR3 *pPos)
{
	D3DXVECTOR3 vec = D3DXVECTOR3((g_aEnemy[nCntEnemy].pos.x - pPos->x), 0.0f, (g_aEnemy[nCntEnemy].pos.z - pPos->z));	//ベクトル
	D3DXVec3Normalize(&vec, &vec);

	g_aEnemy[nCntEnemy].nLife -= Damege;
	g_aEnemy[nCntEnemy].bStop = false;
	g_aEnemy[nCntEnemy].bDamege = true;

	//ヒットエフェクト
	float fAngle = 0;
	float fSpeed = 5;
	for (int nCnt = 0; nCnt < 20; nCnt++)
	{
		fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;	//0〜3.14を出す
		D3DXVECTOR3 move = D3DXVECTOR3(sinf(fAngle)*fSpeed, cosf(fAngle)*fSpeed, 0.0f);
		SetParticle(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].aModel[0].pos.y+20, g_aEnemy[nCntEnemy].pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, 10, 10);
	}

	PlaySound(SOUND_LABEL_SE_DAMAGE);

	//はじき飛ぶ
	g_aEnemy[nCntEnemy].move.x = g_aEnemy[nCntEnemy].move.x + vec.x * 10;
	g_aEnemy[nCntEnemy].move.z = g_aEnemy[nCntEnemy].move.z + vec.z * 10;

	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DETH;
		g_aEnemy[nCntEnemy].nCntFrameEnemy = 0;
		g_aEnemy[nCntEnemy].nCntKeyEnemy = 0;

	}
}

//=============================================================================
// 敵情報の取得
//=============================================================================
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//=============================================================================
// 敵数の取得
//=============================================================================
int *GetNumEnemy(void)
{
	return &g_nEnemyMax;
}

#ifdef _DEBUG
//=============================================================================
// pos表示処理
//=============================================================================
void DrawMaxEnemy(void)
{
	RECT rect = { 0, 100, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "敵数：%d\n", g_nCountEnemy);

	// テキスト描画
	g_pFontEnemy->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}
#endif

//=============================================================================
//	ファイル読み込み
//=============================================================================
void LoadEnemy(void)
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

	pFile = fopen("data/motion_mouse.txt", "r");

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
				g_LoadEnemy.nNumEnemy = atoi(cStr);	//atoi　char型をint型に変換する
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
					strcpy(g_LoadEnemy.cEnemyFileNeme[nCntModel], pStrCur);
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
							g_LoadEnemy.fSpeed = (float)atof(cStr);	//atof　char型をfloat型に変換する
						}
						if (memcmp(&cLine[nCnt], "RADIUS = ", strlen("RADIUS = ")) == 0)
						{	//半径
							pStrCur += strlen("RADIUS = ");
							strcpy(cStr, pStrCur);
							g_LoadEnemy.fRadius = (float)atof(cStr);
						}
						if (memcmp(&cLine[nCnt], "LIFE = ", strlen("LIFE = ")) == 0)
						{	//体力
							pStrCur += strlen("LIFE = ");
							strcpy(cStr, pStrCur);
							g_LoadEnemy.nMaxLife = atoi(cStr);
						}
						if (memcmp(&cLine[nCnt], "NUM_PARTS = ", strlen("NUM_PARTS = ")) == 0)
						{	//読み込むパーツ数
							pStrCur += strlen("NUM_PARTS = ");
							strcpy(cStr, pStrCur);
							g_LoadEnemy.nNumEnemyParts = atoi(cStr);
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
									g_LoadEnemy.nParent[nCntParts] = atoi(cStr);
								}
								if (memcmp(&cLine[nCnt], "POS = ", strlen("POS = ")) == 0)
								{	//位置
									pStrCur += strlen("POS = ");
									strcpy(cStr, pStrCur);

									cPunctuation = strtok(cStr, " ");	//文字を空白ごとに区切って分解
									g_LoadEnemy.pos[nCntParts].x = (float)atof(cPunctuation);

									cPunctuation = strtok(NULL, " ");
									g_LoadEnemy.pos[nCntParts].y = (float)atof(cPunctuation);

									cPunctuation = strtok(NULL, " ");
									g_LoadEnemy.pos[nCntParts].z = (float)atof(cPunctuation);
								}
								if (memcmp(&cLine[nCnt], "ROT = ", strlen("ROT = ")) == 0)
								{	//角度
									pStrCur += strlen("ROT = ");
									strcpy(cStr, pStrCur);
									cPunctuation = strtok(cStr, " ");
									g_LoadEnemy.rot[nCntParts].x = (float)atof(cPunctuation);
									cPunctuation = strtok(NULL, " ");
									g_LoadEnemy.rot[nCntParts].y = (float)atof(cPunctuation);
									cPunctuation = strtok(NULL, " ");
									g_LoadEnemy.rot[nCntParts].z = (float)atof(cPunctuation);
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
						g_aEnemyMotionInfo[nCntMotion].bLoop = atoi(cStr);
					}
					if (memcmp(&cLine[nCnt], "NUM_KEY = ", strlen("NUM_KEY = ")) == 0)
					{	//キー数
						pStrCur += strlen("NUM_KEY = ");
						strcpy(cStr, pStrCur);
						g_aEnemyMotionInfo[nCntMotion].nNumKey = atoi(cStr);
					}
					if (memcmp(&cLine[nCnt], "COLLISION = ", strlen("COLLISION = ")) == 0)
					{	//当たり判定
						pStrCur += strlen("COLLISION = ");
						strcpy(cStr, pStrCur);

						//親の番号
						cPunctuation = strtok(cStr, " ");
						g_LoadEnemy.nCollParent = atoi(cPunctuation);
						//オフセット値
						//X
						cPunctuation = strtok(NULL, " ");
						g_LoadEnemy.Collpos.x = (float)atof(cPunctuation);
						//Y
						cPunctuation = strtok(NULL, " ");
						g_LoadEnemy.Collpos.y = (float)atof(cPunctuation);
						//Z
						cPunctuation = strtok(NULL, " ");
						g_LoadEnemy.Collpos.z = (float)atof(cPunctuation);
						//半径
						cPunctuation = strtok(NULL, " ");
						g_LoadEnemy.fCollRadius = (float)atof(cPunctuation);
						//当たり判定を開始するフレーム数
						cPunctuation = strtok(NULL, " ");
						g_LoadEnemy.nStart = atoi(cPunctuation);
						//終了するフレーム数
						cPunctuation = strtok(NULL, " ");
						g_LoadEnemy.nEnd = atoi(cPunctuation);
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
								g_aEnemyMotionInfo[nCntMotion].KeyInfo[nCntKey].nFrame = atoi(cStr);
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
										g_aEnemyMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos.x = (float)atof(cPunctuation);
										cPunctuation = strtok(NULL, " ");
										g_aEnemyMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos.y = (float)atof(cPunctuation);
										cPunctuation = strtok(NULL, " ");
										g_aEnemyMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos.z = (float)atof(cPunctuation);
									}
									if (memcmp(&cLine[nCnt], "ROT = ", strlen("ROT = ")) == 0)
									{	//角度
										pStrCur += strlen("ROT = ");
										strcpy(cStr, pStrCur);
										cPunctuation = strtok(cStr, " ");
										g_aEnemyMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot.x = (float)atof(cPunctuation);
										cPunctuation = strtok(NULL, " ");
										g_aEnemyMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot.y = (float)atof(cPunctuation);
										cPunctuation = strtok(NULL, " ");
										g_aEnemyMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot.z = (float)atof(cPunctuation);
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
