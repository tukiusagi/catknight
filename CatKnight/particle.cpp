//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "particle.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define PARTICLE_TEXTURE	"data/TEXTURE/pics2269.png"	//読み込むテクスチャ名
#define PARTICLE_SIZE		(10.0f)	//サイズ
#define PARTICLE_MAX		(1000)	//表示最大数

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// パーティクルの構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;			//位置
	D3DXVECTOR3	move;			//移動量
	D3DXVECTOR3	rot;			//向き
	D3DXCOLOR	col;			//色
	float		fRadius;		// 半径
	int			nLife;			// ライフ
	D3DXMATRIX	mtxWorld;		//ワールドマトリックス
	bool		bUse;			//使用状態
}PARTICLE;

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffParticle = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureParticle = NULL;	//テクスチャのポインタ

PARTICLE g_aParticle[PARTICLE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 位置・回転の初期設定
	for (int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].fRadius = 0;
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].bUse = false;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		PARTICLE_TEXTURE,
							&g_pTextureParticle);

	// 頂点情報の作成
	MakeVertexParticle(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitParticle(void)
{
	//テクスチャの開放
	if (g_pTextureParticle != NULL)
	{			  
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxBuffParticle != NULL)
	{			  
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateParticle(void)
{
	VERTEX_3D *pVtx;

	for (int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse)	//弾が使用されている
		{
			g_aParticle[nCntParticle].nLife--;
			g_aParticle[nCntParticle].fRadius -= g_aParticle[nCntParticle].fRadius/ g_aParticle[nCntParticle].nLife;
			g_aParticle[nCntParticle].col.a -= 0.03f;
			g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;

			if (g_aParticle[nCntParticle].fRadius <= 0)
			{
				g_aParticle[nCntParticle].fRadius = 0;
			}
			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += nCntParticle * 4;	//頂点データのポインタを4つ分進める

			// 頂点情報の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius,  g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3( g_aParticle[nCntParticle].fRadius,  g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3( g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = g_aParticle[nCntParticle].col;
			pVtx[1].col = g_aParticle[nCntParticle].col;
			pVtx[2].col = g_aParticle[nCntParticle].col;
			pVtx[3].col = g_aParticle[nCntParticle].col;

			// 頂点データをアンロックする
			g_pVtxBuffParticle->Unlock();

			if (g_aParticle[nCntParticle].nLife <= 0)
			{
				g_aParticle[nCntParticle].bUse = false;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans, mtxView;	// 計算用マトリックス

	PARTICLE *pParticle;

	pParticle = &g_aParticle[0];

	//現在のビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// レンダーステート(加算合成処理)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);

	for (int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++, pParticle++)
	{
		if (pParticle->bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pParticle->mtxWorld);

			//逆行列の設定
			pParticle->mtxWorld._11 = mtxView._11;
			pParticle->mtxWorld._12 = mtxView._21;
			pParticle->mtxWorld._13 = mtxView._31;
			pParticle->mtxWorld._21 = mtxView._12;
			pParticle->mtxWorld._22 = mtxView._22;
			pParticle->mtxWorld._23 = mtxView._32;
			pParticle->mtxWorld._31 = mtxView._13;
			pParticle->mtxWorld._32 = mtxView._23;
			pParticle->mtxWorld._33 = mtxView._33;

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pParticle->rot.y, pParticle->rot.x, pParticle->rot.z);

			D3DXMatrixMultiply(&pParticle->mtxWorld, &pParticle->mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pParticle->pos.x, pParticle->pos.y, pParticle->pos.z);

			D3DXMatrixMultiply(&pParticle->mtxWorld, &pParticle->mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pParticle->mtxWorld);

			//頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureParticle);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
									nCntParticle * 4,		//開始するインデックス
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
void MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * PARTICLE_MAX,	//確保するバッファサイズ(サイズ*頂点数)
								D3DUSAGE_WRITEONLY,		//固定
								FVF_VERTEX_3D,			//頂点フォーマット
								D3DPOOL_MANAGED,		//固定
								&g_pVtxBuffParticle,
								NULL);

	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	for (int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++)
	{
		//頂点の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);

		pVtx[0].nor = D3DXVECTOR3(0.0f,1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f,1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f,1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f,1.0f, 0.0f);

		pVtx[0].col = g_aParticle[nCntParticle].col;
		pVtx[1].col = g_aParticle[nCntParticle].col;
		pVtx[2].col = g_aParticle[nCntParticle].col;
		pVtx[3].col = g_aParticle[nCntParticle].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}

	//頂点データをアンロック
	g_pVtxBuffParticle->Unlock();
}

//=============================================================================
// パーティクルの設定
//=============================================================================
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 move, float fRadius, int nLife)
{
	VERTEX_3D *pVtx;

	for (int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++)
	{
		if (!g_aParticle[nCntParticle].bUse)
		{
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].move = move;
			g_aParticle[nCntParticle].col = col;
			g_aParticle[nCntParticle].fRadius = fRadius;
			g_aParticle[nCntParticle].nLife = nLife;
			//頂点データの範囲をロックし、頂点バッファへのポインタを取得
			g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

			pVtx += nCntParticle * 4;	//頂点データのポインタを4つ分進める

			//頂点の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);

			pVtx[0].col = g_aParticle[nCntParticle].col;
			pVtx[1].col = g_aParticle[nCntParticle].col;
			pVtx[2].col = g_aParticle[nCntParticle].col;
			pVtx[3].col = g_aParticle[nCntParticle].col;

			//頂点データをアンロック
			g_pVtxBuffParticle->Unlock();

			g_aParticle[nCntParticle].bUse = true;
			break;
		}
	}
}