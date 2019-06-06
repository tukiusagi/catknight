//=============================================================================
//
// メッシュ壁処理 [meshwall.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "meshwall.h"
#include "meshfield.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MESHWALL_TEXTURE	"data/TEXTURE/Free Texture Seamless Brick 09-28-10_07.jpg"	//読み込むテクスチャ名
#define MESHWALL_MAX		(20)			//壁の最大数

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexMeshWall(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// メッシュ壁の構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;			//位置
	D3DXVECTOR3	rot;			//向き
	D3DXMATRIX	mtxWorld;		//ワールドマトリックス
	int			nNumVertex;		//総頂点数
	int			nNumIndex;		//総インデックス数
	int			nNumPolygon;	//総ポリゴン数
	bool		bUse;			//使用しているか
}MESHWALL;

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffMeshWall = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureMeshWall = NULL;	//テクスチャのポインタ
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffMeshWall = NULL;	//インデックスバッファへのポインタ

MESHWALL g_ameshwall[MESHWALL_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
void InitMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 位置・回転の初期設定
	for (int nCntWall = 0; nCntWall < MESHWALL_MAX; nCntWall++)
	{
		g_ameshwall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ameshwall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ameshwall[nCntWall].nNumVertex = (MESHWALL_W + 1) * (MESHWALL_H + 1);
		g_ameshwall[nCntWall].nNumPolygon = (2 * MESHWALL_H * MESHWALL_W) + 4 * (MESHWALL_H - 1);
		g_ameshwall[nCntWall].nNumIndex = (2 * MESHWALL_H) * (MESHWALL_W + 1) + 2 * (MESHWALL_H - 1);
		g_ameshwall[nCntWall].bUse = false;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		MESHWALL_TEXTURE,
							&g_pTextureMeshWall);

	// 頂点情報の作成
	MakeVertexMeshWall(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshWall(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffMeshWall != NULL)
	{
		g_pVtxBuffMeshWall->Release();
		g_pVtxBuffMeshWall = NULL;
	}

	//インデックスバッファの開放
	if (g_pIdxBuffMeshWall != NULL)
	{
		g_pIdxBuffMeshWall->Release();
		g_pIdxBuffMeshWall = NULL;
	}

	//テクスチャの開放
	if (g_pTextureMeshWall != NULL)
	{
		g_pTextureMeshWall->Release();
		g_pTextureMeshWall = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshWall(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	MESHWALL *pMeshWall;

	pMeshWall = &g_ameshwall[0];

	for (int nCntWall = 0; nCntWall < MESHWALL_MAX; nCntWall++, pMeshWall++)
	{
		if (pMeshWall->bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pMeshWall->mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMeshWall->rot.y, pMeshWall->rot.x, pMeshWall->rot.z);

			D3DXMatrixMultiply(&pMeshWall->mtxWorld, &pMeshWall->mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pMeshWall->pos.x, pMeshWall->pos.y, pMeshWall->pos.z);

			D3DXMatrixMultiply(&pMeshWall->mtxWorld, &pMeshWall->mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pMeshWall->mtxWorld);

			//頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pVtxBuffMeshWall, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデバイスのデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffMeshWall);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureMeshWall);

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				0, 0,
				pMeshWall->nNumVertex,		//頂点数
				0,
				pMeshWall->nNumPolygon);	//描画するプリミティブ数
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexMeshWall(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_ameshwall[0].nNumVertex * MESHWALL_MAX,	//確保するバッファサイズ(サイズ*頂点数)
								D3DUSAGE_WRITEONLY,		//固定
								FVF_VERTEX_3D,			//頂点フォーマット
								D3DPOOL_MANAGED,		//固定
								&g_pVtxBuffMeshWall,
								NULL);

	//インデックスバッファを生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_ameshwall[0].nNumIndex * MESHWALL_MAX,	//確保するバッファのサイズ(サイズ*必要なインデックス数)
								D3DUSAGE_WRITEONLY,
								D3DFMT_INDEX16,		//一つ当たり16ビット
								D3DPOOL_MANAGED,
								&g_pIdxBuffMeshWall,
								NULL);

	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffMeshWall->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	for (int nCntWall = 0; nCntWall < MESHWALL_MAX; nCntWall++)
	{
		//頂点の設定
		for (int nCntY = 0; nCntY < MESHWALL_H + 1; nCntY++)
		{
			for (int nCntW = 0; nCntW < MESHWALL_W + 1; nCntW++,pVtx++)
			{
				pVtx[0].pos = D3DXVECTOR3((-MESHWALL_WIDTH * 0.5f * MESHWALL_W) + (MESHWALL_WIDTH * nCntW),				//x
											(MESHWALL_HEIGHT * 0.5f * (MESHWALL_H * 2)) - (MESHWALL_HEIGHT * nCntY),	//y
											0.0f);																		//z
				pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[0].tex = D3DXVECTOR2((1.0f / MESHWALL_W + 1) * nCntW, (1.0f / MESHWALL_H + 1) * nCntY);
			}
		}
	}

	//頂点データをアンロック
	g_pVtxBuffMeshWall->Unlock();

	WORD *pIdx;		//インデックスデータへのポインタ
	
	//インデックスデータをロックし、インデックスデータへのポインタを取得
	g_pIdxBuffMeshWall->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntWall = 0; nCntWall < MESHWALL_MAX; nCntWall++)
	{
		for (int nCntIdxH = 0; nCntIdxH < MESHWALL_H; nCntIdxH++)
		{
			//縮退分
			if (nCntIdxH > 0)
			{
				pIdx[0] = (MESHWALL_W + 1) + (MESHWALL_W + 1) * nCntIdxH;
				pIdx++;
			}

			//通常インデックス
			for (int nCntIdxW = 0; nCntIdxW < MESHWALL_W + 1; nCntIdxW++)
			{
				pIdx[0] = (MESHWALL_W + 1 + nCntIdxW) + ((MESHWALL_W + 1) * nCntIdxH);	//頂点の番号(インデックス)を代入
				pIdx[1] = nCntIdxW + ((MESHWALL_W + 1) * nCntIdxH);

				pIdx += 2;
			}
			//縮退分
			if (nCntIdxH < (MESHWALL_H - 1))
			{
				pIdx[0] = MESHWALL_W + (MESHWALL_W + 1) * nCntIdxH;
				pIdx++;
			}

		}
	}
	//頂点データをアンロック
	g_pIdxBuffMeshWall->Unlock();
}

//=============================================================================
// 壁の作成
//=============================================================================
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntWall = 0; nCntWall < MESHWALL_MAX; nCntWall++)
	{
		if (!g_ameshwall[nCntWall].bUse)
		{
			g_ameshwall[nCntWall].pos = pos;
			g_ameshwall[nCntWall].rot = rot;
			g_ameshwall[nCntWall].bUse = true;

			break;
		}
	}
}