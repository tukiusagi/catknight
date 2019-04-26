//=============================================================================
//
// エフェクト処理 [effect.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "effect.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define EFFECT_TEXTURE	"data/TEXTURE/effect000.jpg"	//読み込むテクスチャ名
#define EFFECT_SIZE		(10.0f)	//サイズ
#define EFFECT_MAX		(1000)	//表示最大数

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// ポリゴンの構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;			//位置
	D3DXVECTOR3	rot;			//向き
	D3DXCOLOR	col;			//色
	float		fRadius;		// 半径
	int			nLife;			// ライフ
	D3DXMATRIX	mtxWorld;		//ワールドマトリックス
	bool		bUse;			//使用状態
}EFFECT;

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffEffect = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureEffect = NULL;	//テクスチャのポインタ

EFFECT g_aEffect[EFFECT_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 位置・回転の初期設定
	for (int nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].fRadius = 0;
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].bUse = false;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		EFFECT_TEXTURE,
							&g_pTextureEffect);

	// 頂点情報の作成
	MakeVertexEffect(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect(void)
{
	//テクスチャの開放
	if (g_pTextureEffect != NULL)
	{			  
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxBuffEffect != NULL)
	{			  
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEffect(void)
{
	VERTEX_3D *pVtx;

	for (int nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse)	//弾が使用されている
		{
			g_aEffect[nCntEffect].nLife--;
			g_aEffect[nCntEffect].fRadius -= g_aEffect[nCntEffect].fRadius/ g_aEffect[nCntEffect].nLife;
			g_aEffect[nCntEffect].col.a -= 0.03f;

			if (g_aEffect[nCntEffect].fRadius <= 0)
			{
				g_aEffect[nCntEffect].fRadius = 0;
			}
			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += nCntEffect * 4;	//頂点データのポインタを4つ分進める

			// 頂点情報の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius,  g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3( g_aEffect[nCntEffect].fRadius,  g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3( g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			// 頂点データをアンロックする
			g_pVtxBuffEffect->Unlock();

			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].bUse = false;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans, mtxView;	// 計算用マトリックス

	EFFECT *pEffect;

	pEffect = &g_aEffect[0];

	//現在のビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// レンダーステート(加算合成処理)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);

	for (int nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++, pEffect++)
	{
		if (pEffect->bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pEffect->mtxWorld);

			//逆行列の設定
			pEffect->mtxWorld._11 = mtxView._11;
			pEffect->mtxWorld._12 = mtxView._21;
			pEffect->mtxWorld._13 = mtxView._31;
			pEffect->mtxWorld._21 = mtxView._12;
			pEffect->mtxWorld._22 = mtxView._22;
			pEffect->mtxWorld._23 = mtxView._32;
			pEffect->mtxWorld._31 = mtxView._13;
			pEffect->mtxWorld._32 = mtxView._23;
			pEffect->mtxWorld._33 = mtxView._33;

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pEffect->rot.y, pEffect->rot.x, pEffect->rot.z);

			D3DXMatrixMultiply(&pEffect->mtxWorld, &pEffect->mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pEffect->pos.x, pEffect->pos.y, pEffect->pos.z);

			D3DXMatrixMultiply(&pEffect->mtxWorld, &pEffect->mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pEffect->mtxWorld);

			//頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
									nCntEffect * 4,		//開始するインデックス
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
void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * EFFECT_MAX,	//確保するバッファサイズ(サイズ*頂点数)
								D3DUSAGE_WRITEONLY,		//固定
								FVF_VERTEX_3D,			//頂点フォーマット
								D3DPOOL_MANAGED,		//固定
								&g_pVtxBuffEffect,
								NULL);

	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	for (int nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++)
	{
		//頂点の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

		pVtx[0].nor = D3DXVECTOR3(0.0f,1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f,1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f,1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f,1.0f, 0.0f);

		pVtx[0].col = g_aEffect[nCntEffect].col;
		pVtx[1].col = g_aEffect[nCntEffect].col;
		pVtx[2].col = g_aEffect[nCntEffect].col;
		pVtx[3].col = g_aEffect[nCntEffect].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}

	//頂点データをアンロック
	g_pVtxBuffEffect->Unlock();
}

//=============================================================================
// 爆発の設定
//=============================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife)
{
	VERTEX_3D *pVtx;

	for (int nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++)
	{
		if (!g_aEffect[nCntEffect].bUse)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			//頂点データの範囲をロックし、頂点バッファへのポインタを取得
			g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

			pVtx += nCntEffect * 4;	//頂点データのポインタを4つ分進める

			//頂点の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			//頂点データをアンロック
			g_pVtxBuffEffect->Unlock();

			g_aEffect[nCntEffect].bUse = true;
			break;
		}
	}
}