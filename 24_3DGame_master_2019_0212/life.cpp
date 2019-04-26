//=============================================================================
//
// 残りライフ処理 [life.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include "life.h"
#include "player.h"	

//=============================================================================
// マクロ定義
//=============================================================================
#define	TEXTURE_LIFE		"data/TEXTURE/life.png"	// 読み込むテクスチャファイル名
#define DIGIT_LIFE			(10)					//最大ライフ
#define LIFE_SIZE_X			(50)	//サイズX
#define LIFE_SIZE_Y			(50)	//サイズY

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexLife(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureLife = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;	// 頂点バッファへのポインタ

PLAYER *pPlayer;
LIFE					g_aLife[DIGIT_LIFE];	// ライフ
int						g_nLife;				//今のライフ

//=============================================================================
// 初期化処理
//=============================================================================
void InitLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntLife;
	pPlayer = GetPlayer();

	g_nLife = pPlayer->nMaxLife;

	for (nCntLife = 0; nCntLife < g_nLife; nCntLife++)
	{
		g_aLife[nCntLife].pos = D3DXVECTOR3(30.0f + nCntLife * LIFE_SIZE_X, 30.0f, 0.0f);
		g_aLife[nCntLife].bUse = true;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
								TEXTURE_LIFE,		// ファイルの名前
								&g_pTextureLife);	// 読み込むメモリー
	
	// 頂点情報の作成
	MakeVertexLife(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLife(void)
{
	// テクスチャの開放
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}
	
	// 頂点バッファの開放
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLife(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntLife;

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffLife, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureLife);

	// ポリゴンの描画
	for (nCntLife = 0; nCntLife < DIGIT_LIFE; nCntLife++)
	{
		if (g_aLife[nCntLife].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
									nCntLife * 4,		//開始するインデックス
									2);					//プリミティブの数(ポリゴンの数)
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
void MakeVertexLife(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nCntLife;

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * DIGIT_LIFE,	//確保するバッファサイズ(サイズ*頂点数)
								D3DUSAGE_WRITEONLY,		//固定
								FVF_VERTEX_2D,			//頂点フォーマット
								D3DPOOL_MANAGED,		//固定
								&g_pVtxBuffLife,
								NULL);

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	for (nCntLife = 0; nCntLife < DIGIT_LIFE; nCntLife++)
	{
		// 頂点情報の設定
		pVtx[0].pos = D3DXVECTOR3(g_aLife[nCntLife].pos.x - (LIFE_SIZE_X / 2), g_aLife[nCntLife].pos.y - (LIFE_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
		pVtx[1].pos = D3DXVECTOR3(g_aLife[nCntLife].pos.x + (LIFE_SIZE_X / 2), g_aLife[nCntLife].pos.y - (LIFE_SIZE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aLife[nCntLife].pos.x - (LIFE_SIZE_X / 2), g_aLife[nCntLife].pos.y + (LIFE_SIZE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aLife[nCntLife].pos.x + (LIFE_SIZE_X / 2), g_aLife[nCntLife].pos.y + (LIFE_SIZE_Y / 2), 0.0f);

		pVtx[0].rhw = 1.0f;	//1.0fで固定
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点データをアンロック
	g_pVtxBuffLife->Unlock();
}

//=============================================================================
// ライフの減少
//=============================================================================
void DecreaseLife(int nValue)
{
	pPlayer = GetPlayer();

	if (g_nLife > 0)
	{
		for (int nCntLife = g_nLife-1; nCntLife > (g_nLife-1) - nValue; nCntLife--)
		{
			g_aLife[nCntLife].bUse = false;
		}
	}

	g_nLife -= nValue;
	pPlayer->nLife = g_nLife;
}

//=============================================================================
// ライフの増加
//=============================================================================
void IncreaseLife(int nlife)
{
	int nCntLife;

	for (nCntLife = 0; nCntLife < nlife; nCntLife++)
	{
		if (g_aLife[nCntLife].bUse == false)
		{
			g_aLife[nCntLife].bUse = true;
		}
	}

	g_nLife = nlife;
}
