//=============================================================================
//
// メッシュ筒処理 [meshcylinder.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "meshcylinder.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MEDHCYLINDER_TEXTURE00	"data/TEXTURE/sky001.jpg"	//読み込むテクスチャ名
#define MEDHCYLINDER_HEIGHT		(1000.0f)
#define MEDHCYLINDER_RADIUS		(1000.0f)

#define MESHCYLINDER_Y			(1)		//縦分割数
#define MESHCYLINDER_XZ			(10)	//分割数

#define MESHCYLINDER_MAX			(1)	//筒の最大数
#define MEDHCYLINDER_TEXTURE_MAX	(2)		//テクスチャの最大数

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexMeshCylinder(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// メッシュ筒の構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;			//位置
	D3DXVECTOR3	rot;			//向き
	D3DXMATRIX	mtxWorld;		//ワールドマトリックス
	int			nNumVertex;		//総頂点数
	int			nNumIndex;		//総インデックス数
	int			nNumPolygon;	//総ポリゴン数
	int			nType;			//種類
	float		fRadius;		//半径
	float		fHeight;		//高さ
	bool		bUse;			//使用状態
}MESHCYLINDER;

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffMeshCylinder = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureMeshCylinder[MEDHCYLINDER_TEXTURE_MAX] = {};	//テクスチャのポインタ
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffMeshCylinder = NULL;	//インデックスバッファへのポインタ

MESHCYLINDER g_meshcylinder[MESHCYLINDER_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
void InitMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 位置・回転の初期設定
	for (int nCntCylinder = 0; nCntCylinder < MESHCYLINDER_MAX; nCntCylinder++)
	{
		g_meshcylinder[nCntCylinder].pos = D3DXVECTOR3(0.0f, 800.0f, 0.0f);
		g_meshcylinder[nCntCylinder].rot = D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f);
		g_meshcylinder[nCntCylinder].nNumVertex = (MESHCYLINDER_XZ + 1) * (MESHCYLINDER_Y + 1);
		g_meshcylinder[nCntCylinder].nNumPolygon = MESHCYLINDER_XZ * MESHCYLINDER_Y * 2;
		g_meshcylinder[nCntCylinder].nNumIndex = (MESHCYLINDER_XZ + 1) * (MESHCYLINDER_Y + 1);
		g_meshcylinder[nCntCylinder].fRadius = MEDHCYLINDER_RADIUS;
		g_meshcylinder[nCntCylinder].fHeight = MEDHCYLINDER_HEIGHT;
		g_meshcylinder[nCntCylinder].bUse = false;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		MEDHCYLINDER_TEXTURE00,
		&g_pTextureMeshCylinder[0]);

	// 頂点情報の作成
	MakeVertexMeshCylinder(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshCylinder(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffMeshCylinder != NULL)
	{
		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}

	//インデックスバッファの開放
	if (g_pIdxBuffMeshCylinder != NULL)
	{
		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}

	//テクスチャの開放
	for (int nCnt = 0; nCnt < MEDHCYLINDER_TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureMeshCylinder[nCnt] != NULL)
		{						 
			g_pTextureMeshCylinder[nCnt]->Release();
			g_pTextureMeshCylinder[nCnt] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshCylinder(void)
{
	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	for (int nCntCylinder = 0; nCntCylinder < MESHCYLINDER_MAX; nCntCylinder++)
	{
		if (g_meshcylinder[nCntCylinder].bUse)
		{
			pVtx += nCntCylinder * 22;

			//頂点の設定
			for (int nCntY = 0; nCntY <= MESHCYLINDER_Y; nCntY++)
			{
				for (int nCntXZ = 0; nCntXZ <= MESHCYLINDER_XZ; nCntXZ++, pVtx++)
				{
					pVtx[0].tex.x += 0.0001f;
				}
			}
		}
	}
	//頂点データをアンロック
	g_pVtxBuffMeshCylinder->Unlock();


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	MESHCYLINDER *pMeshCylinder;
	pMeshCylinder = &g_meshcylinder[0];

	for (int nCntCylinder = 0; nCntCylinder < MESHCYLINDER_MAX; nCntCylinder++, pMeshCylinder++)
	{
		if (pMeshCylinder->bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pMeshCylinder->mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMeshCylinder->rot.y, pMeshCylinder->rot.x, pMeshCylinder->rot.z);

			D3DXMatrixMultiply(&pMeshCylinder->mtxWorld, &pMeshCylinder->mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pMeshCylinder->pos.x, pMeshCylinder->pos.y, pMeshCylinder->pos.z);

			D3DXMatrixMultiply(&pMeshCylinder->mtxWorld, &pMeshCylinder->mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pMeshCylinder->mtxWorld);

			//頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデバイスのデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffMeshCylinder);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureMeshCylinder[pMeshCylinder->nType]);

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				0, 0,
				pMeshCylinder->nNumVertex,		//頂点数
				0,
				pMeshCylinder->nNumPolygon);	//描画するプリミティブ数
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexMeshCylinder(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_meshcylinder[0].nNumVertex * MESHCYLINDER_MAX,	//確保するバッファサイズ(サイズ*頂点数)
								D3DUSAGE_WRITEONLY,		//固定
								FVF_VERTEX_3D,			//頂点フォーマット
								D3DPOOL_MANAGED,		//固定
								&g_pVtxBuffMeshCylinder,
								NULL);

	//インデックスバッファを生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_meshcylinder[0].nNumIndex * MESHCYLINDER_MAX,	//確保するバッファのサイズ(サイズ*必要なインデックス数)
								D3DUSAGE_WRITEONLY,
								D3DFMT_INDEX16,		//一つ当たり16ビット
								D3DPOOL_MANAGED,
								&g_pIdxBuffMeshCylinder,
								NULL);

	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！
	for (int nCntCylinder = 0; nCntCylinder < MESHCYLINDER_MAX; nCntCylinder++)
	{
		//頂点の設定
		for (int nCntY = 0; nCntY <= MESHCYLINDER_Y; nCntY++)
		{
			for (int nCntXZ = 0; nCntXZ <= MESHCYLINDER_XZ; nCntXZ++, pVtx++)
			{
				pVtx[0].pos = D3DXVECTOR3(sinf((2 * D3DX_PI / MESHCYLINDER_XZ) * nCntXZ) * g_meshcylinder[nCntCylinder].fRadius,	//x
					g_meshcylinder[nCntCylinder].fHeight* nCntY,															//y
					cosf((2 * D3DX_PI / MESHCYLINDER_XZ) * nCntXZ) * g_meshcylinder[nCntCylinder].fRadius);						//z

				pVtx[0].nor = pVtx[0].pos;
				D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[0].tex = D3DXVECTOR2((1.0f / MESHCYLINDER_XZ) * nCntXZ, (1.0f / MESHCYLINDER_Y) * nCntY);
			}
		}
	}
	//頂点データをアンロック
	g_pVtxBuffMeshCylinder->Unlock();

	WORD *pIdx;		//インデックスデータへのポインタ
	
	//インデックスデータをロックし、インデックスデータへのポインタを取得
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntCylinder = 0; nCntCylinder < MESHCYLINDER_MAX; nCntCylinder++)
	{
		for (int nCntIdxY = 0; nCntIdxY < MESHCYLINDER_Y; nCntIdxY++)
		{
			//通常インデックス
			for (int nCntIdxXZ = 0; nCntIdxXZ <= MESHCYLINDER_XZ; nCntIdxXZ++)
			{
				pIdx[0] = nCntIdxXZ + ((MESHCYLINDER_XZ + 1) * nCntIdxY);	//頂点の番号(インデックス)を代入
				pIdx[1] = (MESHCYLINDER_XZ + 1) + nCntIdxXZ + ((MESHCYLINDER_XZ + 1) * nCntIdxY);

				pIdx += 2;
			}
		}
	}
	//頂点データをアンロック
	g_pIdxBuffMeshCylinder->Unlock();
}

//=============================================================================
// 筒の作成
//=============================================================================
void SetMeshCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float radius, float height, int type)
{
	for (int nCntCylinder = 0; nCntCylinder < MESHCYLINDER_MAX; nCntCylinder++)
	{
		if (!g_meshcylinder[nCntCylinder].bUse)
		{
			g_meshcylinder[nCntCylinder].pos = pos;
			g_meshcylinder[nCntCylinder].rot = rot;
			g_meshcylinder[nCntCylinder].fRadius = radius;
			g_meshcylinder[nCntCylinder].fHeight = height;
			g_meshcylinder[nCntCylinder].nType = type;

			VERTEX_3D *pVtx;

			//頂点データの範囲をロックし、頂点バッファへのポインタを取得
			g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

			pVtx += nCntCylinder * 22;

			//頂点の設定
			for (int nCntY = 0; nCntY <= MESHCYLINDER_Y; nCntY++)
			{
				for (int nCntXZ = 0; nCntXZ <= MESHCYLINDER_XZ; nCntXZ++, pVtx++)
				{
					pVtx[0].pos = D3DXVECTOR3(sinf((2 * D3DX_PI / MESHCYLINDER_XZ) * nCntXZ) * g_meshcylinder[nCntCylinder].fRadius,	//x
						g_meshcylinder[nCntCylinder].fHeight* nCntY,															//y
						cosf((2 * D3DX_PI / MESHCYLINDER_XZ) * nCntXZ) * g_meshcylinder[nCntCylinder].fRadius);						//z
				}
			}

			//頂点データをアンロック
			g_pVtxBuffMeshCylinder->Unlock();

			g_meshcylinder[nCntCylinder].bUse = true;
			break;
		}
	}
}