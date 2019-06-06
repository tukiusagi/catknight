//=============================================================================
//
// 影処理 [shadow.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "shadow.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SHADOW_SIZE		(10.0f)		//影の大きさ
#define MAX_SHADOW		(100)		//表示最大数

#define SHADOW_TEXTURE		"data/TEXTURE/shadow000.jpg"	//読み込むテクスチャ名

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// 影の構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;		//位置
	D3DXVECTOR3	rot;		//向き
	D3DXCOLOR	col;		//色
	float		fSize;		//サイズ
	D3DXMATRIX	mtxWorld;	//ワールドマトリックス
	bool		bUse;		//使用状況
}SHADOW;

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffShadow = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureShadow = NULL;	//テクスチャのポインタ

SHADOW g_aShadow[MAX_SHADOW];	//影の情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 位置・回転の初期設定
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aShadow[nCntShadow].fSize = SHADOW_SIZE;
		g_aShadow[nCntShadow].bUse = false;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
							SHADOW_TEXTURE,
							&g_pTextureShadow);

	// 頂点情報の作成
	MakeVertexShadow(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitShadow(void)
{
	//テクスチャの開放
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateShadow(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	SHADOW *pShadow;

	//減算合成
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	pShadow = &g_aShadow[0];
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++, pShadow++)
	{
		if (pShadow->bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pShadow->mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pShadow->rot.y, pShadow->rot.x, pShadow->rot.z);

			D3DXMatrixMultiply(&pShadow->mtxWorld, &pShadow->mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pShadow->pos.x, pShadow->pos.y, pShadow->pos.z);

			D3DXMatrixMultiply(&pShadow->mtxWorld, &pShadow->mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pShadow->mtxWorld);

			//頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureShadow);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
									nCntShadow*4,		//開始するインデックス
									2);					//プリミティブの数(ポリゴンの数)
		}
	}

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
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,	//確保するバッファサイズ(サイズ*頂点数)
								D3DUSAGE_WRITEONLY,		//固定
								FVF_VERTEX_3D,			//頂点フォーマット
								D3DPOOL_MANAGED,		//固定
								&g_pVtxBuffShadow,
								NULL);

	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		//頂点の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fSize, 0.0f, g_aShadow[nCntShadow].fSize);
		pVtx[1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].fSize, 0.0f, g_aShadow[nCntShadow].fSize);
		pVtx[2].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fSize, 0.0f, -g_aShadow[nCntShadow].fSize);
		pVtx[3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].fSize, 0.0f, -g_aShadow[nCntShadow].fSize);

		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[0].col = g_aShadow[nCntShadow].col;
		pVtx[1].col = g_aShadow[nCntShadow].col;
		pVtx[2].col = g_aShadow[nCntShadow].col;
		pVtx[3].col = g_aShadow[nCntShadow].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点データをアンロック
	g_pVtxBuffShadow->Unlock();
}

//=============================================================================
// 影の作成
//=============================================================================
int SetSadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSize, D3DXCOLOR col)
{
	int nCntShadow;
	VERTEX_3D *pVtx;

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (!g_aShadow[nCntShadow].bUse)
		{
			g_aShadow[nCntShadow].pos.x = pos.x;
			g_aShadow[nCntShadow].pos.z = pos.z;

			g_aShadow[nCntShadow].rot = rot;
			g_aShadow[nCntShadow].col = col;

			g_aShadow[nCntShadow].fSize = fSize;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += nCntShadow * 4;	//頂点データのポインタを4つ分進める

			// 頂点情報の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fSize, 0.0f, g_aShadow[nCntShadow].fSize);
			pVtx[1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].fSize, 0.0f, g_aShadow[nCntShadow].fSize);
			pVtx[2].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fSize, 0.0f, -g_aShadow[nCntShadow].fSize);
			pVtx[3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].fSize, 0.0f, -g_aShadow[nCntShadow].fSize);

			pVtx[0].col = g_aShadow[nCntShadow].col;
			pVtx[1].col = g_aShadow[nCntShadow].col;
			pVtx[2].col = g_aShadow[nCntShadow].col;
			pVtx[3].col = g_aShadow[nCntShadow].col;

			// 頂点データをアンロックする
			g_pVtxBuffShadow->Unlock();

			g_aShadow[nCntShadow].bUse = true;

			break;
		}
	}
	return nCntShadow;
}

//=============================================================================
// 影の位置を設定
//=============================================================================
void SetPositionSadow(int IdxShadow, D3DXVECTOR3 pos, float fSize, D3DXCOLOR col)
{
	VERTEX_3D *pVtx;

	g_aShadow[IdxShadow].pos.x = pos.x;
	g_aShadow[IdxShadow].pos.z = pos.z;

	g_aShadow[IdxShadow].col = col;

	g_aShadow[IdxShadow].fSize = fSize;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += IdxShadow * 4;	//頂点データのポインタを4つ分進める

	// 頂点情報の設定
	pVtx[0].pos = D3DXVECTOR3(-g_aShadow[IdxShadow].fSize, 0.0f, g_aShadow[IdxShadow].fSize);
	pVtx[1].pos = D3DXVECTOR3(g_aShadow[IdxShadow].fSize, 0.0f, g_aShadow[IdxShadow].fSize);
	pVtx[2].pos = D3DXVECTOR3(-g_aShadow[IdxShadow].fSize, 0.0f, -g_aShadow[IdxShadow].fSize);
	pVtx[3].pos = D3DXVECTOR3(g_aShadow[IdxShadow].fSize, 0.0f, -g_aShadow[IdxShadow].fSize);

	pVtx[0].col = g_aShadow[IdxShadow].col;
	pVtx[1].col = g_aShadow[IdxShadow].col;
	pVtx[2].col = g_aShadow[IdxShadow].col;
	pVtx[3].col = g_aShadow[IdxShadow].col;

	// 頂点データをアンロックする
	g_pVtxBuffShadow->Unlock();
}

//=============================================================================
// 影の削除
//=============================================================================
void DeleteShadow(int IdxShadow)
{
	g_aShadow[IdxShadow].bUse = false;
}
