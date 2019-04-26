//=============================================================================
//
// 軌跡処理 [trajectory.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "trajectory.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TRAJECTORY_MAX		(20)			//軌跡の最大数

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexTrajectory(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// ポリゴンの構造体
//=============================================================================
typedef struct
{
	D3DXMATRIX	mtxWorld;		//ワールドマトリックス
	D3DXMATRIX	aMtxPoint[2];	//オフセットのマトリックス
	D3DXMATRIX	*pMtxParent;	//親のマトリックス
	D3DXVECTOR3	offset[2];		//オフセット
	D3DXVECTOR3	pos[TRAJECTORY_POLYGON * 2 + 2];			//頂点の位置情報（頂点数分）
	D3DXVECTOR3	rot[2];			//向き
	D3DXCOLOR	col;			//色
	int			nNumVertex;		//総頂点数
	int			nNumPolygon;	//総ポリゴン数
	bool		bUse;			//使用しているか
}MESH_TRAJECTORY;

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffTrajectory = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureTrajectory = NULL;	//テクスチャのポインタ

MESH_TRAJECTORY g_aTrajectory[TRAJECTORY_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
void InitTrajectory(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 位置・回転の初期設定
	for (int nCnt = 0; nCnt < TRAJECTORY_MAX; nCnt++)
	{
		for (int nCntOrbit = 0; nCntOrbit < 2; nCntOrbit++)
		{	//オフセットの初期化
			g_aTrajectory[nCnt].offset[nCntOrbit] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aTrajectory[nCnt].rot[nCntOrbit] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		}

		g_aTrajectory[nCnt].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);
		g_aTrajectory[nCnt].nNumPolygon = TRAJECTORY_POLYGON;
		g_aTrajectory[nCnt].nNumVertex = g_aTrajectory[nCnt].nNumPolygon * 2 + 2;
		g_aTrajectory[nCnt].bUse = false;

		for (int nCntOrbit = 0; nCntOrbit < g_aTrajectory[nCnt].nNumVertex; nCntOrbit++)
		{	//位置情報の初期化
			g_aTrajectory[nCnt].pos[nCntOrbit] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&g_pTextureTrajectory);

	// 頂点情報の作成
	MakeVertexTrajectory(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTrajectory(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffTrajectory != NULL)
	{
		g_pVtxBuffTrajectory->Release();
		g_pVtxBuffTrajectory = NULL;
	}

	//テクスチャの開放
	if (g_pTextureTrajectory != NULL)
	{
		g_pTextureTrajectory->Release();
		g_pTextureTrajectory = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTrajectory(void)
{
	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffTrajectory->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	for (int nCnt = 0; nCnt < TRAJECTORY_MAX; nCnt++)
	{
		if (g_aTrajectory[nCnt].bUse)
		{
			pVtx += g_aTrajectory[nCnt].nNumVertex * nCnt;

			for (int nCntpos = 0; nCntpos < g_aTrajectory[nCnt].nNumVertex; nCntpos++,pVtx++)
			{
				pVtx[0].pos = g_aTrajectory[nCnt].pos[nCntpos];	//頂点の更新
				if (nCntpos % 2 == 1)
				{
					//透明度の設定
					g_aTrajectory[nCnt].col.a = 0.0f;

					pVtx[0].col = g_aTrajectory[nCnt].col;
				}
			}
		}
	}
	//頂点データをアンロック
	g_pVtxBuffTrajectory->Unlock();

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTrajectory(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	//裏側をカリングしない
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// レンダーステート(加算合成処理)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	MESH_TRAJECTORY *pTrajectory;

	pTrajectory = &g_aTrajectory[0];

	for (int nCnt = 0; nCnt < TRAJECTORY_MAX; nCnt++, pTrajectory++)
	{
		if (pTrajectory->bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pTrajectory->mtxWorld);

			for (int nCntOrbit = 0; nCntOrbit < 2; nCntOrbit++)
			{	//オフセットの合成
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&pTrajectory->aMtxPoint[nCntOrbit]);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, pTrajectory->rot[nCntOrbit].y, pTrajectory->rot[nCntOrbit].x, pTrajectory->rot[nCntOrbit].z);

				D3DXMatrixMultiply(&pTrajectory->aMtxPoint[nCntOrbit], &pTrajectory->aMtxPoint[nCntOrbit], &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

				// 位置を反映
				D3DXMatrixTranslation(&mtxTrans, pTrajectory->offset[nCntOrbit].x, pTrajectory->offset[nCntOrbit].y, pTrajectory->offset[nCntOrbit].z);

				D3DXMatrixMultiply(&pTrajectory->aMtxPoint[nCntOrbit], &pTrajectory->aMtxPoint[nCntOrbit], &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

				//親子反映
				D3DXMatrixMultiply(&pTrajectory->aMtxPoint[nCntOrbit], &pTrajectory->aMtxPoint[nCntOrbit], pTrajectory->pMtxParent);	//ワールド行列＝ワールド行列＊親のワールド行列
			}

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pTrajectory->mtxWorld);

			//位置の入れ替え
			for (int nCntpos = 0; nCntpos < pTrajectory->nNumPolygon; nCntpos++)
			{
				pTrajectory->pos[pTrajectory->nNumVertex - ((nCntpos + 1) * 2)] = pTrajectory->pos[(pTrajectory->nNumVertex - ((nCntpos + 1) * 2)) - 2];
				pTrajectory->pos[pTrajectory->nNumVertex - (nCntpos * 2 + 1)] = pTrajectory->pos[(pTrajectory->nNumVertex - (nCntpos * 2 + 1)) - 2];
			}
			pTrajectory->pos[0] = D3DXVECTOR3(pTrajectory->aMtxPoint[0]._41, pTrajectory->aMtxPoint[0]._42, pTrajectory->aMtxPoint[0]._43);
			pTrajectory->pos[1] = D3DXVECTOR3(pTrajectory->aMtxPoint[1]._41, pTrajectory->aMtxPoint[1]._42, pTrajectory->aMtxPoint[1]._43);

			//頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pVtxBuffTrajectory, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureTrajectory);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCnt*pTrajectory->nNumVertex,					//開始するインデックス
				pTrajectory->nNumPolygon);					//プリミティブの数(ポリゴンの数)
		}
	}

	// 裏面(左回り)をカリングする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// レンダーステート(通常ブレンド処理)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexTrajectory(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aTrajectory[0].nNumVertex * TRAJECTORY_MAX,	//確保するバッファサイズ(サイズ*頂点数)
		D3DUSAGE_WRITEONLY,		//固定
		FVF_VERTEX_3D,			//頂点フォーマット
		D3DPOOL_MANAGED,		//固定
		&g_pVtxBuffTrajectory,
		NULL);

	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffTrajectory->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	for (int nCnt = 0; nCnt < TRAJECTORY_MAX; nCnt++)
	{
		for (int nCntOrbit = 0; nCntOrbit < g_aTrajectory[nCnt].nNumVertex; nCntOrbit++,pVtx++)
		{
			//頂点の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[0].col = g_aTrajectory[nCnt].col;
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		}
	}
	//頂点データをアンロック
	g_pVtxBuffTrajectory->Unlock();
}

//=============================================================================
// 軌跡の作成
//=============================================================================
void SetTrajectory(D3DXVECTOR3 offset00, D3DXVECTOR3 offset01,D3DXMATRIX *mtxParent, D3DXCOLOR	col)
{
	for (int nCnt = 0; nCnt < TRAJECTORY_MAX; nCnt++)
	{
		if (!g_aTrajectory[nCnt].bUse)
		{
			g_aTrajectory[nCnt].offset[0] = offset00;
			g_aTrajectory[nCnt].offset[1] = offset01;
			g_aTrajectory[nCnt].pMtxParent = mtxParent;
			g_aTrajectory[nCnt].col = col;

			VERTEX_3D *pVtx;

			//頂点データの範囲をロックし、頂点バッファへのポインタを取得
			g_pVtxBuffTrajectory->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

			pVtx += g_aTrajectory[nCnt].nNumVertex * nCnt;

			for (int nCntOrbit = 0; nCntOrbit < g_aTrajectory[nCnt].nNumVertex; nCntOrbit++, pVtx++)
			{
				pVtx[0].col = g_aTrajectory[nCnt].col;
			}

			//頂点データをアンロック
			g_pVtxBuffTrajectory->Unlock();

			g_aTrajectory[nCnt].bUse = true;
			break;
		}
	}
}