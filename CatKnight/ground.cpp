//=============================================================================
//
// 地面	処理 [ground.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "ground.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define GROUND_TEXTURE		"data/TEXTURE/field004.jpg"	//読み込むテクスチャ名

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexGround(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// 地面の構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;			//位置
	D3DXVECTOR3	rot;			//向き
	D3DXMATRIX	mtxWorld;		//ワールドマトリックス
	int			nNumVertex;		//総頂点数
	int			nNumIndex;		//総インデックス数
	int			nNumPolygon;	//総ポリゴン数
}GROUND;

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffGround = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureGround = NULL;	//テクスチャのポインタ
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffGround = NULL;	//インデックスバッファへのポインタ

GROUND g_ground;

//=============================================================================
// 初期化処理
//=============================================================================
void InitGround(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 位置・回転の初期設定
	g_ground.pos = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	g_ground.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_ground.nNumVertex = (GROUND_W + 1) * (GROUND_D + 1);
	g_ground.nNumPolygon = (2 * GROUND_D * GROUND_W) + 4 * (GROUND_D - 1);
	g_ground.nNumIndex = (2 * GROUND_D) * (GROUND_W + 1) + 2 * (GROUND_D - 1);
		
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		GROUND_TEXTURE,
							&g_pTextureGround);

	// 頂点情報の作成
	MakeVertexGround(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGround(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffGround != NULL)
	{
		g_pVtxBuffGround->Release();
		g_pVtxBuffGround = NULL;
	}

	//インデックスバッファの開放
	if (g_pIdxBuffGround != NULL)
	{
		g_pIdxBuffGround->Release();
		g_pIdxBuffGround = NULL;
	}

	//テクスチャの開放
	if (g_pTextureGround != NULL)
	{
		g_pTextureGround->Release();
		g_pTextureGround = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGround(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGround(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_ground.mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ground.rot.y, g_ground.rot.x, g_ground.rot.z);

	D3DXMatrixMultiply(&g_ground.mtxWorld, &g_ground.mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_ground.pos.x, g_ground.pos.y, g_ground.pos.z);

	D3DXMatrixMultiply(&g_ground.mtxWorld, &g_ground.mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_ground.mtxWorld);

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffGround, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデバイスのデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffGround);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureGround);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
							0,0,
							g_ground.nNumVertex,		//頂点数
							0,
							g_ground.nNumPolygon);	//描画するプリミティブ数				
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexGround(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_ground.nNumVertex,	//確保するバッファサイズ(サイズ*頂点数)
								D3DUSAGE_WRITEONLY,		//固定
								FVF_VERTEX_3D,			//頂点フォーマット
								D3DPOOL_MANAGED,		//固定
								&g_pVtxBuffGround,
								NULL);

	//インデックスバッファを生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_ground.nNumIndex,	//確保するバッファのサイズ(サイズ*必要なインデックス数)
								D3DUSAGE_WRITEONLY,
								D3DFMT_INDEX16,		//一つ当たり16ビット
								D3DPOOL_MANAGED,
								&g_pIdxBuffGround,
								NULL);

	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffGround->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	//頂点の設定
	for (int nCntD = 0; nCntD < GROUND_D + 1; nCntD++)
	{
		for (int nCntW = 0; nCntW < GROUND_W + 1; nCntW++,pVtx++)
		{
			pVtx[0].pos = D3DXVECTOR3((-GROUND_SIZE * 0.5f * GROUND_W) + (GROUND_SIZE * nCntW),	//x
										0.0f,																//y
										(GROUND_SIZE * 0.5f * GROUND_D) - (GROUND_SIZE * nCntD));	//z
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[0].tex = D3DXVECTOR2((1.0f / GROUND_W + 1) * nCntW, (1.0f / GROUND_D + 1) * nCntD);
		}
	}

	//頂点データをアンロック
	g_pVtxBuffGround->Unlock();

	WORD *pIdx;		//インデックスデータへのポインタ
	
	//インデックスデータをロックし、インデックスデータへのポインタを取得
	g_pIdxBuffGround->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdxD = 0; nCntIdxD < GROUND_D; nCntIdxD++)
	{
		//縮退分
		if (nCntIdxD > 0)
		{
			pIdx[0] = (GROUND_W + 1) + (GROUND_W + 1) * nCntIdxD;
			pIdx ++;
		}

		//通常インデックス
		for (int nCntIdxW = 0; nCntIdxW < GROUND_W + 1; nCntIdxW++)
		{
			pIdx[0] = (GROUND_W + 1 + nCntIdxW) + ((GROUND_W + 1) * nCntIdxD);	//頂点の番号(インデックス)を代入
			pIdx[1] = nCntIdxW + ((GROUND_W + 1) * nCntIdxD);

			pIdx += 2;
		}
		//縮退分
		if (nCntIdxD<(GROUND_D-1))
		{
			pIdx[0] = GROUND_W  + (GROUND_W + 1) * nCntIdxD;
			pIdx++;

		}
	}

	//頂点データをアンロック
	g_pIdxBuffGround->Unlock();
}