//=============================================================================
//
// 魔法処理 [wave.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "wave.h"
#include"game.h"
#include "input.h"
#include "joystick.h"
#include "princess.h"
#include "sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define WAVE_TEXTURE	"data/TEXTURE/white.png"	//読み込むテクスチャ名
#define WAVE_HEIGHT			(25.0f)
#define WAVE_RADIUS			(30.0f)
#define WAVE_RADIUS_MAX		(WAVE_RADIUS * 5)

#define WAVE_Y			(1)		//縦分割数
#define WAVE_XZ			(15)	//分割数
#define SECOND			(60)	//秒

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexWave(LPDIRECT3DDEVICE9 pDevice);


//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffWave = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureWave = NULL;	//テクスチャのポインタ
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffWave = NULL;	//インデックスバッファへのポインタ

WAVE g_wave;
D3DXVECTOR3 g_waveMove[WAVE_XZ];

//=============================================================================
// 初期化処理
//=============================================================================
void InitWave(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 位置・回転の初期設定
		g_wave.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_wave.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_wave.col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		g_wave.state = WAVE_STATE_CHARGE;
		g_wave.nNumVertex = (WAVE_XZ + 1) * (WAVE_Y + 1);
		g_wave.nNumPolygon = WAVE_XZ * WAVE_Y * 2;
		g_wave.nNumIndex = (WAVE_XZ + 1) * (WAVE_Y + 1);
		g_wave.nCharge = 0;
		g_wave.fRadius = WAVE_RADIUS;
		g_wave.fHeight = WAVE_HEIGHT;
		g_wave.bUse = false;

		for (int nCntMove = 0; nCntMove < WAVE_XZ; nCntMove++)
		{
			g_waveMove[nCntMove] = D3DXVECTOR3(0.0f, 0.5f, 0.0f);
		}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		WAVE_TEXTURE,
		&g_pTextureWave);

	// 頂点情報の作成
	MakeVertexWave(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitWave(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffWave != NULL)
	{
		g_pVtxBuffWave->Release();
		g_pVtxBuffWave = NULL;
	}

	//インデックスバッファの開放
	if (g_pIdxBuffWave != NULL)
	{
		g_pIdxBuffWave->Release();
		g_pIdxBuffWave = NULL;
	}

	//テクスチャの開放
		if (g_pTextureWave != NULL)
		{					 
			g_pTextureWave->Release();
			g_pTextureWave = NULL;
		}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateWave(void)
{
	PRINCESS *pPrincess;
	pPrincess = GetPrincess();

		if (g_wave.state == WAVE_STATE_CHARGE)
		{
			g_wave.nCharge++;
			if (g_wave.nCharge % (SECOND/2) == 0)
			{
				PlaySound(SOUND_LABEL_SE_CHARGE);
			}
			if (g_wave.nCharge > SECOND*5)
			{
				g_wave.state = WAVE_STATE_CHARGEMAX;
				PlaySound(SOUND_LABEL_SE_CHARGEMAX);

			}
		}
	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffWave->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	//頂点の設定
	for (int nCntY = 0; nCntY <= WAVE_Y; nCntY++)
	{
		for (int nCntXZ = 0; nCntXZ <= WAVE_XZ; nCntXZ++, pVtx++)
		{
			pVtx[0].pos.x = sinf((2 * D3DX_PI / WAVE_XZ) * nCntXZ) * g_wave.fRadius;
			pVtx[0].pos.z = cosf((2 * D3DX_PI / WAVE_XZ) * nCntXZ) * g_wave.fRadius;

			if (nCntXZ % 2 == 1 && nCntY > 0)
			{
				pVtx[0].pos.y += g_waveMove[nCntXZ].y;

				if (pVtx[0].pos.y > WAVE_HEIGHT + 10.0f || pVtx[0].pos.y < WAVE_HEIGHT - 10.0f)
				{
					g_waveMove[nCntXZ].y *= -1;
				}
			}
				switch (g_wave.state)
				{
				case WAVE_STATE_CHARGE:
					g_wave.col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
					break;

				case WAVE_STATE_CHARGEMAX:
					g_wave.col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
						if ((GetKeyTrigger(DIK_B) || GetJoystickTrigger(PAD_Y)))
						{
							pPrincess->state = PRINCESSSTATE_ATTACK;
							pPrincess->nCntFrame = 0;
							pPrincess->nCntKeyPrincess = 0;
							g_wave.state = WAVE_STATE_ATTACK;
							PlaySound(SOUND_LABEL_SE_MAGIC);
						}
					break;

				case WAVE_STATE_ATTACK:
					g_wave.col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
					g_wave.fRadius += 0.1f;
					if (g_wave.fRadius > WAVE_RADIUS_MAX)
					{
						g_wave.state = WAVE_STATE_CHARGE;
						pPrincess->state = PRINCESSSTATE_NEUTRAL;
						g_wave.nCharge = 0;
						g_wave.fRadius = WAVE_RADIUS;
					}
					break;
				default:
					break;
			}
			if (nCntY > 0)
			{
				g_wave.col.a = 0.0f;
			}

			if (*GetGameState() == GAMESTATE_GAMEOVER)
			{
				g_wave.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			}

			pVtx[0].col = g_wave.col;
			g_wave.rot.y += 0.0005f;
		}
	}

	//頂点データをアンロック
	g_pVtxBuffWave->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawWave(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス


	//裏側をカリングしない
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// レンダーステート(加算合成処理)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_wave.mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_wave.rot.y, g_wave.rot.x, g_wave.rot.z);

	D3DXMatrixMultiply(&g_wave.mtxWorld, &g_wave.mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_wave.pos.x, g_wave.pos.y, g_wave.pos.z);

	D3DXMatrixMultiply(&g_wave.mtxWorld, &g_wave.mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_wave.mtxWorld);

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffWave, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデバイスのデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffWave);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureWave);

	if (g_wave.bUse)
	{
		//ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			0, 0,
			g_wave.nNumVertex,		//頂点数
			0,
			g_wave.nNumPolygon);	//描画するプリミティブ数
	}
	// 裏面(左回り)をカリングする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//通常モードに戻す
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// レンダーステート(通常ブレンド処理)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexWave(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_wave.nNumVertex,	//確保するバッファサイズ(サイズ*頂点数)
								D3DUSAGE_WRITEONLY,		//固定
								FVF_VERTEX_3D,			//頂点フォーマット
								D3DPOOL_MANAGED,		//固定
								&g_pVtxBuffWave,
								NULL);

	//インデックスバッファを生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_wave.nNumIndex ,	//確保するバッファのサイズ(サイズ*必要なインデックス数)
								D3DUSAGE_WRITEONLY,
								D3DFMT_INDEX16,		//一つ当たり16ビット
								D3DPOOL_MANAGED,
								&g_pIdxBuffWave,
								NULL);

	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffWave->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！
		//頂点の設定
		for (int nCntY = 0; nCntY <= WAVE_Y; nCntY++)
		{
			for (int nCntXZ = 0; nCntXZ <= WAVE_XZ; nCntXZ++, pVtx++)
			{
				pVtx[0].pos = D3DXVECTOR3(sinf((2 * D3DX_PI / WAVE_XZ) * nCntXZ) * g_wave.fRadius,	//x
					g_wave.fHeight* nCntY,															//y
					cosf((2 * D3DX_PI / WAVE_XZ) * nCntXZ) * g_wave.fRadius);						//z

				pVtx[0].nor = pVtx[0].pos;
				D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);
				if (nCntY > 0)
				{
					g_wave.col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.0f);
				}
				else
				{
					g_wave.col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
				}
				pVtx[0].col = g_wave.col;
				pVtx[0].tex = D3DXVECTOR2((1.0f / WAVE_XZ) * nCntXZ, (1.0f / WAVE_Y) * nCntY);
			}
		}
	//頂点データをアンロック
	g_pVtxBuffWave->Unlock();

	WORD *pIdx;		//インデックスデータへのポインタ
	
	//インデックスデータをロックし、インデックスデータへのポインタを取得
	g_pIdxBuffWave->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntIdxY = 0; nCntIdxY < WAVE_Y; nCntIdxY++)
		{
			//通常インデックス
			for (int nCntIdxXZ = 0; nCntIdxXZ <= WAVE_XZ; nCntIdxXZ++)
			{
				pIdx[0] = nCntIdxXZ + ((WAVE_XZ + 1) * nCntIdxY);	//頂点の番号(インデックス)を代入
				pIdx[1] = (WAVE_XZ + 1) + nCntIdxXZ + ((WAVE_XZ + 1) * nCntIdxY);

				pIdx += 2;
			}
		}
	//頂点データをアンロック
	g_pIdxBuffWave->Unlock();
}

//=============================================================================
// チャージリセット
//=============================================================================
void DeleteCharge(void)
{
	g_wave.nCharge = 0;
	g_wave.state = WAVE_STATE_CHARGE;
}

//=============================================================================
// 当たり判定
//=============================================================================
bool CollisionWave(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove,float Radius)
{
	bool bLand = false;
	float fDistance = (pPos->x - g_wave.pos.x)*(pPos->x - g_wave.pos.x) + (pPos->z - g_wave.pos.z)*(pPos->z - g_wave.pos.z);
	float fLength = (Radius + g_wave.fRadius)*(Radius + g_wave.fRadius);
	float fDiff = (Radius + g_wave.fRadius) - (float)sqrt(fDistance);	//差
	D3DXVECTOR3 vec = D3DXVECTOR3((pPos->x - g_wave.pos.x), 0.0f, (pPos->z - g_wave.pos.z));	//ベクトル
	D3DXVec3Normalize(&vec, &vec);

	if (fDistance < fLength)
	{
		bLand = true;

		pPos->x = pPos->x + vec.x * fDiff;
		pPos->z = pPos->z + vec.z * fDiff;
	}

	return bLand;
}

//=============================================================================
// 情報の取得
//=============================================================================
WAVE *GetWave(void)
{
	return &g_wave;
}

//=============================================================================
// 魔法の設定
//=============================================================================
void SetWave(D3DXVECTOR3 pos)
{
	g_wave.pos = pos;
	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffWave->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！
													//頂点の設定
	for (int nCntY = 0; nCntY <= WAVE_Y; nCntY++)
	{
		for (int nCntXZ = 0; nCntXZ <= WAVE_XZ; nCntXZ++, pVtx++)
		{
			pVtx[0].pos = D3DXVECTOR3(sinf((2 * D3DX_PI / WAVE_XZ) * nCntXZ) * g_wave.fRadius,	//x
				g_wave.fHeight* nCntY,															//y
				cosf((2 * D3DX_PI / WAVE_XZ) * nCntXZ) * g_wave.fRadius);						//z

			pVtx[0].nor = pVtx[0].pos;
			D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);
			if (nCntY > 0)
			{
				g_wave.col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.0f);
			}
			else
			{
				g_wave.col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
			}
			pVtx[0].col = g_wave.col;
			pVtx[0].tex = D3DXVECTOR2((1.0f / WAVE_XZ) * nCntXZ, (1.0f / WAVE_Y) * nCntY);
		}
	}
	//頂点データをアンロック
	g_pVtxBuffWave->Unlock();

	g_wave.bUse = true;
}
