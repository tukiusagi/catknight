//=============================================================================
//
// ボス処理 [boss.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "boss.h"
#include "enemy.h"
#include "shadow.h"
#include "princess.h"
#include "player.h"
#include "wave.h"
#include "obstacle.h"
#include "effect.h"
#include "score.h"
#include "sound.h"
#include "particle.h"
#include "enemywave.h"
#include "fileload.h"
#include "lifePrincess.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define BOSS_FILENAME	"data/motion_boss.txt"		//読み込むテクスチャ名

#define BOSS_SHADOW			(15.0f)	//影の大きさ
#define BOSS_ROTSPEAD		(0.1f)	//回転速度
#define BOSS_KEY_MAX		(4)		//キーの最大数
#define BOSS_WAVE_MAX		(1)		//wave最大数
#define MAX_CHARA			(256)	//読み込む文字の最大数
#define BOSS_SCORE			(1500)	//入るスコア
#define ATTACK_PRINCESS		(5);

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9	g_pTextureBoss[6] = {};	//テクスチャのポインタ
BOSS				g_aBoss[MAX_BOSS];		//ボス情報
int					g_NumBoss = 0;			//ボス数
LOAD *pLoadBoss = GetLoad(CHARA_BOSS);		//ファイル情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 位置・向きの初期設定
	LoadFile(BOSS_FILENAME, CHARA_BOSS);
	g_NumBoss = 0;

	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		g_aBoss[nCntBoss].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].fRadius = pLoadBoss->fRadius;
		g_aBoss[nCntBoss].fSpeed = pLoadBoss->fSpeed;
		g_aBoss[nCntBoss].nLife = pLoadBoss->nMaxLife;
		g_aBoss[nCntBoss].nAttack = pLoadBoss->nAttack;
		g_aBoss[nCntBoss].state = BOSSSTATE_NEUTRAL;
		g_aBoss[nCntBoss].bUse = false;
		g_aBoss[nCntBoss].bHit = false;
		g_aBoss[nCntBoss].bDamege = false;
		g_aBoss[nCntBoss].bStop = false;
		g_aBoss[nCntBoss].nCntState = 0;
		g_aBoss[nCntBoss].nCntFrameBoss = 0;
		g_aBoss[nCntBoss].nCntKeyBoss = 0;
		g_aBoss[nCntBoss].nCntAttack = 0;

		g_aBoss[nCntBoss].CollisionBoss.nIdxModelParent = pLoadBoss->nCollParent;
		g_aBoss[nCntBoss].CollisionBoss.pos = pLoadBoss->Collpos;
		g_aBoss[nCntBoss].CollisionBoss.fRadius = pLoadBoss->fCollRadius;
		g_aBoss[nCntBoss].CollisionBoss.nStartFrame = pLoadBoss->nStart;
		g_aBoss[nCntBoss].CollisionBoss.nEndFrame = pLoadBoss->nEnd;
		g_aBoss[nCntBoss].CollisionBoss.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].CollisionBoss.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// Xファイルの読み込み
		for (int nCntParts = 0; nCntParts < pLoadBoss->nNumParts; nCntParts++)
		{
			g_aBoss[nCntBoss].aModel[nCntParts].nIdxModelParent = pLoadBoss->nParent[nCntParts];
			g_aBoss[nCntBoss].aModel[nCntParts].pos = pLoadBoss->pos[nCntParts];
			g_aBoss[nCntBoss].aModel[nCntParts].rot = pLoadBoss->rot[nCntParts];
			g_aBoss[nCntBoss].aModel[nCntParts].pFilename = pLoadBoss->cFileNeme[nCntParts];

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
		for (int nCntParts = 0; nCntParts < pLoadBoss->nNumParts; nCntParts++)
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
	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (g_aBoss[nCntBoss].bUse)
		{
			g_aBoss[nCntBoss].nCntAttack++;	//攻撃タイミングカウンター

			g_aBoss[nCntBoss].posOld = g_aBoss[nCntBoss].pos;
			g_aBoss[nCntBoss].CollisionBoss.posOld = g_aBoss[nCntBoss].CollisionBoss.pos;

			if (g_aBoss[nCntBoss].bDamege)
			{	//ダメージを受けたら赤く変わる
				g_aBoss[nCntBoss].nCntState++;
				if (g_aBoss[nCntBoss].nCntState % DAMEGE_WAITTIME == 0)
				{	//色を戻す
					g_aBoss[nCntBoss].bDamege = false;
				}
			}
			else if (g_aBoss[nCntBoss].bStop)
			{	//マヒ状態
				g_aBoss[nCntBoss].nCntState++;
				if (g_aBoss[nCntBoss].nCntState % PARALYZE_WAITTIME == 0)
				{	//状態解除
					g_aBoss[nCntBoss].bStop = false;
					g_aBoss[nCntBoss].nCntState = 0;
				}
			}

			//フレーム増加]
			g_aBoss[nCntBoss].nCntFrameBoss++;

			if (g_aBoss[nCntBoss].bStop && g_aBoss[nCntBoss].nCntKeyBoss >= 1)
			{	//マヒ状態ではストップ
			}
			else if (g_aBoss[nCntBoss].nCntFrameBoss > pLoadBoss->MotionInfo[g_aBoss[nCntBoss].state].KeyInfo[g_aBoss[nCntBoss].nCntKeyBoss].nFrame)
			{
				g_aBoss[nCntBoss].nCntFrameBoss = 0;
				g_aBoss[nCntBoss].nCntKeyBoss++;	//キー増加

				if (g_aBoss[nCntBoss].nCntKeyBoss >= pLoadBoss->MotionInfo[g_aBoss[nCntBoss].state].nNumKey)
				{
					g_aBoss[nCntBoss].nCntKeyBoss = 0;	//キーリセット
				}
			}

			//モーションの切り替え
			if (g_aBoss[nCntBoss].state != BOSSSTATE_ATTACK && g_aBoss[nCntBoss].state != BOSSSTATE_DETH && g_aBoss[nCntBoss].state != BOSSSTATE_HIT)
			{
				if (g_aBoss[nCntBoss].move.x > LATEST_SPEED || g_aBoss[nCntBoss].move.z > LATEST_SPEED ||
					g_aBoss[nCntBoss].move.x < -LATEST_SPEED || g_aBoss[nCntBoss].move.z < -LATEST_SPEED)
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
			g_aBoss[nCntBoss].move.y -= GRAVITY;

			//自分との距離を比べて近い方に寄り付く
			float fDistX, fDistZ, fDistans;
			CalculateDist(fDistX, fDistZ, fDistans, g_aBoss[nCntBoss].pos);

			//移動
			if (CollisionPlayer(&g_aBoss[nCntBoss].pos, &g_aBoss[nCntBoss].posOld, g_aBoss[nCntBoss].fRadius) ||
				CollisionPrincess(&g_aBoss[nCntBoss].pos, &g_aBoss[nCntBoss].posOld, g_aBoss[nCntBoss].fRadius,1))
			{	//姫や騎士に接触しているとき
				AttackBoss(nCntBoss);
			}
			else
			{
				//移動
				if (g_aBoss[nCntBoss].state != BOSSSTATE_ATTACK && !g_aBoss[nCntBoss].bStop)
				{
					MoveBoss(fDistX, fDistZ, fDistans,nCntBoss);
				}
			}

			//障害物破壊
			OBSTACLE *pObstacle = GetObstacle();
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
			RotationDir(g_aBoss[nCntBoss].rotmove, g_aBoss[nCntBoss].rotDest, g_aBoss[nCntBoss].rot, BOSS_ROTSPEAD);

			//慣性
			g_aBoss[nCntBoss].move.x += (0.0f - g_aBoss[nCntBoss].move.x)*INERTIA;
			g_aBoss[nCntBoss].move.z += (0.0f - g_aBoss[nCntBoss].move.z)*INERTIA;

			//移動
			g_aBoss[nCntBoss].pos.x += g_aBoss[nCntBoss].move.x;
			g_aBoss[nCntBoss].pos.z += g_aBoss[nCntBoss].move.z;
			g_aBoss[nCntBoss].pos.y += g_aBoss[nCntBoss].move.y;

			//魔法当たり判定
			WAVE *pWave = GetWave();
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
			ENEMY *pEnemy = GetEnemy();
			for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++)
			{	//敵との当たり判定
				if (pEnemy->bUse)
				{
					CollisionEnemy(&g_aBoss[nCntBoss].pos, &g_aBoss[nCntBoss].posOld, g_aBoss[nCntBoss].fRadius, nCnt);
				}
			}
			BOSS *pBoss = GetBoss();
			for (int nCnt = 0; nCnt < MAX_BOSS; nCnt++,pBoss++)
			{
				if (pBoss->bUse && nCnt != nCntBoss)
				{
					CollisionBoss(&g_aBoss[nCntBoss].pos, &g_aBoss[nCntBoss].posOld, g_aBoss[nCntBoss].fRadius, nCnt);
				}
			}

			if (g_aBoss[nCntBoss].pos.y < 0)
			{	//地面にめり込まないように
				g_aBoss[nCntBoss].pos.y = 0;
				g_aBoss[nCntBoss].move.y = 0;
			}

			//壁当たり判定
			CollisionWall(&g_aBoss[nCntBoss].pos, g_aBoss[nCntBoss].fRadius);

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
			for (int nCntParts = 0; nCntParts < pLoadBoss->nNumParts; nCntParts++)
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
void SetBoss(D3DXVECTOR3 pos)
{
	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (!g_aBoss[nCntBoss].bUse)
		{
			g_aBoss[nCntBoss].pos = pos;
			g_aBoss[nCntBoss].state = BOSSSTATE_NEUTRAL;
			g_aBoss[nCntBoss].nLife = pLoadBoss->nMaxLife;
			g_aBoss[nCntBoss].bUse = true;
			g_NumBoss++;
			//影の作成
			g_aBoss[nCntBoss].nIdxShadowBoss = SetSadow(g_aBoss[nCntBoss].pos, g_aBoss[nCntBoss].rot, BOSS_SHADOW, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
			break;
		}
	}
}

//=============================================================================
// ボスの削除
//=============================================================================
void DeleteBoss()
{
	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (g_aBoss[nCntBoss].bUse)
		{
			g_aBoss[nCntBoss].bUse = false;
			g_aBoss[nCntBoss].bHit = false;
			g_aBoss[nCntBoss].bDamege = false;
			g_aBoss[nCntBoss].bStop = false;
			g_aBoss[nCntBoss].state = BOSSSTATE_NEUTRAL;
			DeleteShadow(g_aBoss[nCntBoss].nIdxShadowBoss);
		}
	}
	g_NumBoss = 0;
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
// 攻撃
//=============================================================================
void AttackBoss(int nCntBoss)
{
	if (g_aBoss[nCntBoss].state != BOSSSTATE_ATTACK)
	{
		if ((rand() % ATTACK_TIMING == 0 || g_aBoss[nCntBoss].nCntAttack % ATTACK_TIMING == 0) && g_aBoss[nCntBoss].state != BOSSSTATE_DETH)
		{	//3秒に1回くらいの確率で攻撃
			g_aBoss[nCntBoss].state = BOSSSTATE_ATTACK;
			g_aBoss[nCntBoss].nCntFrameBoss = 0;
			g_aBoss[nCntBoss].nCntKeyBoss = 0;
			PlaySound(SOUND_LABEL_SE_NAIL);
		}
	}
	else
	{
		if (CollisionPrincess(&D3DXVECTOR3(g_aBoss[nCntBoss].CollisionBoss.mtxWorld._41, g_aBoss[nCntBoss].CollisionBoss.mtxWorld._42, g_aBoss[nCntBoss].CollisionBoss.mtxWorld._43), &g_aBoss[nCntBoss].CollisionBoss.posOld, g_aBoss[nCntBoss].CollisionBoss.fRadius, 1) &&
			(g_aBoss[nCntBoss].CollisionBoss.nStartFrame <= g_aBoss[nCntBoss].nCntFrameBoss && g_aBoss[nCntBoss].CollisionBoss.nEndFrame >= g_aBoss[nCntBoss].nCntFrameBoss))
		{	//姫体力減少
			DecreasePLife(g_aBoss[nCntBoss].nAttack);
		}
		else if (CollisionPlayer(&D3DXVECTOR3(g_aBoss[nCntBoss].CollisionBoss.mtxWorld._41, g_aBoss[nCntBoss].CollisionBoss.mtxWorld._42, g_aBoss[nCntBoss].CollisionBoss.mtxWorld._43), &g_aBoss[nCntBoss].CollisionBoss.posOld, g_aBoss[nCntBoss].CollisionBoss.fRadius) &&
			(g_aBoss[nCntBoss].CollisionBoss.nStartFrame <= g_aBoss[nCntBoss].nCntFrameBoss && g_aBoss[nCntBoss].CollisionBoss.nEndFrame >= g_aBoss[nCntBoss].nCntFrameBoss))
		{	//プレイヤー体力減少
			HitPlayer(g_aBoss[nCntBoss].nAttack / 2, &g_aBoss[nCntBoss].pos);
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
	for (int nCntMotion = 0; nCntMotion < pLoadBoss->nNumParts; nCntMotion++)
	{
		KEY *pKey, *pNextKey;
		int nNextKey = g_aBoss[nCntBoss].nCntKeyBoss + 1;

		if (nNextKey >= pLoadBoss->MotionInfo[g_aBoss[nCntBoss].state].nNumKey)
		{
			if (pLoadBoss->MotionInfo[g_aBoss[nCntBoss].state].bLoop)
			{
				nNextKey = 0;
			}
			else
			{
				if (g_aBoss[nCntBoss].state == BOSSSTATE_DETH)
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
		pKey = &pLoadBoss->MotionInfo[g_aBoss[nCntBoss].state].KeyInfo[g_aBoss[nCntBoss].nCntKeyBoss].Key[nCntMotion];	//現在のキー	
		pNextKey = &pLoadBoss->MotionInfo[g_aBoss[nCntBoss].state].KeyInfo[nNextKey].Key[nCntMotion];	//次のキー		

		//回転
		g_aBoss[nCntBoss].aModel[nCntMotion].rotMove = (pNextKey->rot - pKey->rot)*((float)g_aBoss[nCntBoss].nCntFrameBoss / (float)pLoadBoss->MotionInfo[g_aBoss[nCntBoss].state].KeyInfo[g_aBoss[nCntBoss].nCntKeyBoss].nFrame);
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
			AddScore(BOSS_SCORE);
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

//=============================================================================
// ボス総数の取得
//=============================================================================
int GetBossNum(void)
{
	return g_NumBoss;
}
