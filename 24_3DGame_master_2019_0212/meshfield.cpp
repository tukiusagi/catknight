//=============================================================================
//
// フィールド処理 [meshfield.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "meshfield.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MESHFIELD_TEXTURE		"data/TEXTURE/field.jpg"	//読み込むテクスチャ名

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// ポリゴンの構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;			//位置
	D3DXVECTOR3	rot;			//向き
	D3DXMATRIX	mtxWorld;		//ワールドマトリックス
	int			nNumVertex;		//総頂点数
	int			nNumIndex;		//総インデックス数
	int			nNumPolygon;	//総ポリゴン数
}MESHFIELD;

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffMeshField = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureMeshField = NULL;	//テクスチャのポインタ
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffMeshField = NULL;	//インデックスバッファへのポインタ

MESHFIELD g_meshfield;

//=============================================================================
// 初期化処理
//=============================================================================
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 位置・回転の初期設定
	g_meshfield.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_meshfield.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_meshfield.nNumVertex = (MESHFIELD_W + 1) * (MESHFIELD_D + 1);
	g_meshfield.nNumPolygon = (2 * MESHFIELD_D * MESHFIELD_W) + 4 * (MESHFIELD_D - 1);
	g_meshfield.nNumIndex = (2 * MESHFIELD_D) * (MESHFIELD_W + 1) + 2 * (MESHFIELD_D - 1);
		
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		MESHFIELD_TEXTURE,
							&g_pTextureMeshField);

	// 頂点情報の作成
	MakeVertexMeshField(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshField(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	//インデックスバッファの開放
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}

	//テクスチャの開放
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshField(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_meshfield.mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_meshfield.rot.y, g_meshfield.rot.x, g_meshfield.rot.z);

	D3DXMatrixMultiply(&g_meshfield.mtxWorld, &g_meshfield.mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_meshfield.pos.x, g_meshfield.pos.y, g_meshfield.pos.z);

	D3DXMatrixMultiply(&g_meshfield.mtxWorld, &g_meshfield.mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_meshfield.mtxWorld);

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデバイスのデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshField);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshField);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
							0,0,
							g_meshfield.nNumVertex,		//頂点数
							0,
							g_meshfield.nNumPolygon);	//描画するプリミティブ数				
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_meshfield.nNumVertex,	//確保するバッファサイズ(サイズ*頂点数)
								D3DUSAGE_WRITEONLY,		//固定
								FVF_VERTEX_3D,			//頂点フォーマット
								D3DPOOL_MANAGED,		//固定
								&g_pVtxBuffMeshField,
								NULL);

	//インデックスバッファを生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_meshfield.nNumIndex,	//確保するバッファのサイズ(サイズ*必要なインデックス数)
								D3DUSAGE_WRITEONLY,
								D3DFMT_INDEX16,		//一つ当たり16ビット
								D3DPOOL_MANAGED,
								&g_pIdxBuffMeshField,
								NULL);

	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	//頂点の設定
	for (int nCntD = 0; nCntD < MESHFIELD_D + 1; nCntD++)
	{
		for (int nCntW = 0; nCntW < MESHFIELD_W + 1; nCntW++,pVtx++)
		{
			pVtx[0].pos = D3DXVECTOR3((-MESHFIELD_SIZE * 0.5f * MESHFIELD_W) + (MESHFIELD_SIZE * nCntW),	//x
										0.0f,																//y
										(MESHFIELD_SIZE * 0.5f * MESHFIELD_D) - (MESHFIELD_SIZE * nCntD));	//z
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[0].tex = D3DXVECTOR2((1.0f / MESHFIELD_W + 1) * nCntW, (1.0f / MESHFIELD_D + 1) * nCntD);
		}
	}

	//頂点データをアンロック
	g_pVtxBuffMeshField->Unlock();

	WORD *pIdx;		//インデックスデータへのポインタ
	
	//インデックスデータをロックし、インデックスデータへのポインタを取得
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdxD = 0; nCntIdxD < MESHFIELD_D; nCntIdxD++)
	{
		//縮退分
		if (nCntIdxD > 0)
		{
			pIdx[0] = (MESHFIELD_W + 1) + (MESHFIELD_W + 1) * nCntIdxD;
			pIdx ++;
		}

		//通常インデックス
		for (int nCntIdxW = 0; nCntIdxW < MESHFIELD_W + 1; nCntIdxW++)
		{
			pIdx[0] = (MESHFIELD_W + 1 + nCntIdxW) + ((MESHFIELD_W + 1) * nCntIdxD);	//頂点の番号(インデックス)を代入
			pIdx[1] = nCntIdxW + ((MESHFIELD_W + 1) * nCntIdxD);

			pIdx += 2;
		}
		//縮退分
		if (nCntIdxD<(MESHFIELD_D-1))
		{
			pIdx[0] = MESHFIELD_W  + (MESHFIELD_W + 1) * nCntIdxD;
			pIdx++;

		}
	}

	//頂点データをアンロック
	g_pIdxBuffMeshField->Unlock();
}