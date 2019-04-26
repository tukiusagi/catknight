//=============================================================================
//
// 障害物処理 [obstacle.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "obstacle.h"
#include "shadow.h"
#include "line.h"
#include "sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define OBSTACLE_FILE			"data/MODEL/Rubble2.x"					//読み込むファイル名
#define OBSTACLE_TEXTURENAME	"data/TEXTURE/stone_00037.jpg"			//読み込むテクスチャ名

#define OBSTACLE_TEXTURE	(2)		//テクスチャ数
#define OBSTACLE_LIFE		(2)		//テクスチャ数

//=============================================================================
// グローバル変数
//=============================================================================
LPD3DXMESH				g_pMeshObstacle = NULL;		//メッシュ情報のポインタ
LPD3DXBUFFER			g_pBuffMatObstacle = NULL;		//マテリアル情報へのポインタ
DWORD					g_nNumMatObstacle = 0;			//マテリアル情報の数
LPDIRECT3DTEXTURE9		g_pTextureObstacle[OBSTACLE_TEXTURE] = {};	//テクスチャのポインタ

OBSTACLE g_aObstacle[OBSTACLE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
void InitObstacle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 位置・向きの初期設定

	for (int nCntbasin = 0; nCntbasin < OBSTACLE_MAX; nCntbasin++)
	{
			g_aObstacle[nCntbasin].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aObstacle[nCntbasin].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aObstacle[nCntbasin].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aObstacle[nCntbasin].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aObstacle[nCntbasin].nLife = OBSTACLE_LIFE;
			g_aObstacle[nCntbasin].nCntState = 0;
			g_aObstacle[nCntbasin].bUse = false;
			g_aObstacle[nCntbasin].bHit = false;

	}

	// Xファイルの読み込み
	D3DXLoadMeshFromX(OBSTACLE_FILE,	//読み込むファイル名
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatObstacle,
		NULL,
		&g_nNumMatObstacle,
		&g_pMeshObstacle);

	//テクスチャの読み込み
	for (int nCnt = 0; nCnt < OBSTACLE_TEXTURE; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice,
			OBSTACLE_TEXTURENAME,
			&g_pTextureObstacle[nCnt]);
	}

	//モデルの頂点座標の最大値・最小値の設定
	int		nNumVertices;	//頂点数
	DWORD	sizeFVF;		//頂点フォーマットのサイズ
	BYTE	*pVertexBuffer;	//頂点バッファへのポインタ

	//初期化
	for (int nCntbasin = 0; nCntbasin < OBSTACLE_MAX; nCntbasin++)
	{
		g_aObstacle[nCntbasin].vtxMin = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
		g_aObstacle[nCntbasin].vtxMax = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);

		//頂点数を取得
		nNumVertices = g_pMeshObstacle->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_pMeshObstacle->GetFVF());

		//頂点バッファのロック
		g_pMeshObstacle->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

		//頂点の最大・最小値の取得
		for (int nCntVtx = 0; nCntVtx < nNumVertices; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;
			//すべての頂点情報を比較して最小値・最大値を抜き出す
			//xの最小と最大
			if (g_aObstacle[nCntbasin].vtxMin.x > vtx.x)
			{
				g_aObstacle[nCntbasin].vtxMin.x = vtx.x;
			}
			if (g_aObstacle[nCntbasin].vtxMax.x < vtx.x)
			{
				g_aObstacle[nCntbasin].vtxMax.x = vtx.x;
			}
			//yの最小と最大
			if (g_aObstacle[nCntbasin].vtxMin.y > vtx.y)
			{
				g_aObstacle[nCntbasin].vtxMin.y = vtx.y;
			}
			if (g_aObstacle[nCntbasin].vtxMax.y < vtx.y)
			{
				g_aObstacle[nCntbasin].vtxMax.y = vtx.y;
			}

			//zの最小と最大
			if (g_aObstacle[nCntbasin].vtxMin.z > vtx.z)
			{
				g_aObstacle[nCntbasin].vtxMin.z = vtx.z;
			}
			if (g_aObstacle[nCntbasin].vtxMax.z < vtx.z)
			{
				g_aObstacle[nCntbasin].vtxMax.z = vtx.z;
			}
			pVertexBuffer += sizeFVF;	//サイズ分ポインタを進める
		}

		//頂点バッファのアンロック
		g_pMeshObstacle->UnlockVertexBuffer();
	}

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitObstacle(void)
{
	// メッシュの開放
	if (g_pMeshObstacle != NULL)
	{
		g_pMeshObstacle->Release();
		g_pMeshObstacle = NULL;
	}

	// マテリアルの開放
	if (g_pBuffMatObstacle != NULL)
	{
		g_pBuffMatObstacle->Release();
		g_pBuffMatObstacle = NULL;
	}

	//テクスチャの開放
	for (int nCntTex = 0; nCntTex < OBSTACLE_TEXTURE; nCntTex++)
	{
		if (g_pTextureObstacle[nCntTex] != NULL)
		{
			g_pTextureObstacle[nCntTex]->Release();
			g_pTextureObstacle[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateObstacle(void)
{
	for (int nCntObstacle = 0; nCntObstacle < OBSTACLE_MAX; nCntObstacle++)
	{
		if (g_aObstacle[nCntObstacle].bHit)
		{
			g_aObstacle[nCntObstacle].nCntState++;
			if (g_aObstacle[nCntObstacle].nCntState % 10 == 0)
			{
				g_aObstacle[nCntObstacle].bHit = false;
			}
		}
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawObstacle(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	D3DXMATRIX		mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATERIAL	*pMat;				//現在のマテリアル保存用
	D3DMATERIAL9	matDef;				//マテリアルデータへのポインタ
	D3DMATERIAL9	matKeep;	//保存用

	for (int nCntbasin = 0; nCntbasin < OBSTACLE_MAX; nCntbasin++)
	{
		if (g_aObstacle[nCntbasin].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aObstacle[nCntbasin].mtxWorldObstacle);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aObstacle[nCntbasin].rot.y, g_aObstacle[nCntbasin].rot.x, g_aObstacle[nCntbasin].rot.z);

			D3DXMatrixMultiply(&g_aObstacle[nCntbasin].mtxWorldObstacle, &g_aObstacle[nCntbasin].mtxWorldObstacle, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aObstacle[nCntbasin].pos.x, g_aObstacle[nCntbasin].pos.y, g_aObstacle[nCntbasin].pos.z);

			D3DXMatrixMultiply(&g_aObstacle[nCntbasin].mtxWorldObstacle, &g_aObstacle[nCntbasin].mtxWorldObstacle, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aObstacle[nCntbasin].mtxWorldObstacle);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアル情報に対するポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatObstacle->GetBufferPointer();

			// テクスチャの設定をするならここ(1枚の時)

			for (int nCntMat = 0; nCntMat < (int)g_nNumMatObstacle; nCntMat++)
			{
				// テクスチャの設定をするならここ(複数枚の時)
				pDevice->SetTexture(0, g_pTextureObstacle[nCntMat]);

				matKeep = pMat[nCntMat].MatD3D;	//情報の保存

				if (g_aObstacle[nCntbasin].bHit)
				{
					pMat[nCntMat].MatD3D.Diffuse.r = 255;	//赤色に変える
				}
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				pMat[nCntMat].MatD3D = matKeep;	//色を戻す

				// 描画
				g_pMeshObstacle->DrawSubset(nCntMat);	//格子状態
			}
			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// 障害物の設定
//=============================================================================
void SetObstacle(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntbasin = 0; nCntbasin < OBSTACLE_MAX; nCntbasin++)
	{
		if (!g_aObstacle[nCntbasin].bUse)
		{
			g_aObstacle[nCntbasin].pos = pos;
			g_aObstacle[nCntbasin].rot = rot;
			g_aObstacle[nCntbasin].bUse = true;

#ifdef _DEBUG
			//ラインの描画
			SetLine(g_aObstacle[nCntbasin].pos,	D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMax.x, g_aObstacle[nCntbasin].vtxMin.y, g_aObstacle[nCntbasin].vtxMax.z), D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMax.x, g_aObstacle[nCntbasin].vtxMax.y, g_aObstacle[nCntbasin].vtxMax.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			SetLine(g_aObstacle[nCntbasin].pos, D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMin.x, g_aObstacle[nCntbasin].vtxMin.y, g_aObstacle[nCntbasin].vtxMin.z), D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMin.x, g_aObstacle[nCntbasin].vtxMax.y, g_aObstacle[nCntbasin].vtxMin.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			SetLine(g_aObstacle[nCntbasin].pos, D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMin.x, g_aObstacle[nCntbasin].vtxMin.y, g_aObstacle[nCntbasin].vtxMax.z), D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMin.x, g_aObstacle[nCntbasin].vtxMax.y, g_aObstacle[nCntbasin].vtxMax.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			SetLine(g_aObstacle[nCntbasin].pos, D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMax.x, g_aObstacle[nCntbasin].vtxMin.y, g_aObstacle[nCntbasin].vtxMin.z), D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMax.x, g_aObstacle[nCntbasin].vtxMax.y, g_aObstacle[nCntbasin].vtxMin.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

			SetLine(g_aObstacle[nCntbasin].pos, D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMax.x, g_aObstacle[nCntbasin].vtxMax.y, g_aObstacle[nCntbasin].vtxMax.z), D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMax.x, g_aObstacle[nCntbasin].vtxMax.y, g_aObstacle[nCntbasin].vtxMin.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			SetLine(g_aObstacle[nCntbasin].pos, D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMin.x, g_aObstacle[nCntbasin].vtxMax.y, g_aObstacle[nCntbasin].vtxMin.z), D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMin.x, g_aObstacle[nCntbasin].vtxMax.y, g_aObstacle[nCntbasin].vtxMax.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			SetLine(g_aObstacle[nCntbasin].pos, D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMin.x, g_aObstacle[nCntbasin].vtxMax.y, g_aObstacle[nCntbasin].vtxMax.z), D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMax.x, g_aObstacle[nCntbasin].vtxMax.y, g_aObstacle[nCntbasin].vtxMax.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			SetLine(g_aObstacle[nCntbasin].pos, D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMax.x, g_aObstacle[nCntbasin].vtxMax.y, g_aObstacle[nCntbasin].vtxMin.z), D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMin.x, g_aObstacle[nCntbasin].vtxMax.y, g_aObstacle[nCntbasin].vtxMin.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif
			break;
		}
	}
}

//=============================================================================
// 情報の取得
//=============================================================================
OBSTACLE *GetObstacle(void)
{
	return &g_aObstacle[0];
}

//=============================================================================
// 耐久力の減少
//=============================================================================
void DeleteObstacle(int Damage,int nCntObstacle)
{
	g_aObstacle[nCntObstacle].nLife -= Damage;
	g_aObstacle[nCntObstacle].bHit = true;

	if (g_aObstacle[nCntObstacle].nLife <= 0)
	{
		PlaySound(SOUND_LABEL_SE_BREAK);
		g_aObstacle[nCntObstacle].bUse = false;
		g_aObstacle[nCntObstacle].bHit = false;
	}
}

//=============================================================================
// 障害物の全消去
//=============================================================================
void DeleteALLObstacle(void)
{
	for (int nCntbasin = 0; nCntbasin < OBSTACLE_MAX; nCntbasin++)
	{
		if (g_aObstacle[nCntbasin].bUse)
		{
			g_aObstacle[nCntbasin].bUse = false;
			g_aObstacle[nCntbasin].bHit = false;
			g_aObstacle[nCntbasin].nLife = OBSTACLE_LIFE;

		}
	}
	DeleteLine();
}

//=============================================================================
// 当たり判定
//=============================================================================
bool CollisionObstacle(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3*pMove, float Radius,int nCntbasin)
{
	bool bLand = false;
	//for (int nCntbasin = 0; nCntbasin < OBSTACLE_MAX; nCntbasin++)
	//{
		if (g_aObstacle[nCntbasin].bUse)
		{
			if (pPos->x + Radius >= g_aObstacle[nCntbasin].pos.x + g_aObstacle[nCntbasin].vtxMin.x &&
				pPos->x - Radius <= g_aObstacle[nCntbasin].pos.x + g_aObstacle[nCntbasin].vtxMax.x)
			{
				if (pPosOld->z - Radius >= g_aObstacle[nCntbasin].pos.z + g_aObstacle[nCntbasin].vtxMax.z &&
					pPos->z - Radius <= g_aObstacle[nCntbasin].pos.z + g_aObstacle[nCntbasin].vtxMax.z)
				{	//奥から来た時
					bLand = true;
					pPos->z = g_aObstacle[nCntbasin].pos.z + g_aObstacle[nCntbasin].vtxMax.z + Radius;
					//pMove->z = 0;
				}
				else if (pPosOld->z + Radius <= g_aObstacle[nCntbasin].pos.z + g_aObstacle[nCntbasin].vtxMin.z &&
					pPos->z + Radius >= g_aObstacle[nCntbasin].pos.z + g_aObstacle[nCntbasin].vtxMin.z)
				{	//手前から来た時
					bLand = true;
					pPos->z = g_aObstacle[nCntbasin].pos.z + g_aObstacle[nCntbasin].vtxMin.z - Radius;
					//pMove->z = 0;
				}
			}
			if (pPos->z + Radius >= g_aObstacle[nCntbasin].pos.z + g_aObstacle[nCntbasin].vtxMin.z &&
				pPos->z - Radius <= g_aObstacle[nCntbasin].pos.z + g_aObstacle[nCntbasin].vtxMax.z)
			{
				if (pPosOld->x - Radius >= g_aObstacle[nCntbasin].pos.x + g_aObstacle[nCntbasin].vtxMax.x &&
					pPos->x - Radius <= g_aObstacle[nCntbasin].pos.x + g_aObstacle[nCntbasin].vtxMax.x)
				{	//右から来た時
					bLand = true;
					pPos->x = g_aObstacle[nCntbasin].pos.x + g_aObstacle[nCntbasin].vtxMax.x + Radius;
					//pMove->x = 0;
				}
				else if (pPosOld->x + Radius <= g_aObstacle[nCntbasin].pos.x + g_aObstacle[nCntbasin].vtxMin.x &&
					pPos->x + Radius >= g_aObstacle[nCntbasin].pos.x + g_aObstacle[nCntbasin].vtxMin.x)
				{	//左から来た時
					bLand = true;
					pPos->x = g_aObstacle[nCntbasin].pos.x + g_aObstacle[nCntbasin].vtxMin.x - Radius;
					//pMove->x = 0;
				}
			}
		}
	//}
	return bLand;
}