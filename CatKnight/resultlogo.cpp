//=============================================================================
//
// リザルトロゴ処理[resultlogo.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"resultlogo.h"	//インクルードファイル
#include "sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define RELOGO_BG			"data/TEXTURE/tutorialBG.jpg"	//読み込むテクスチャ名
#define RELOGO_BG_BLACK		"data/TEXTURE/black.png"		//読み込むテクスチャ名
#define RELOGO_BASIS		"data/TEXTURE/Basis.png"		//読み込むテクスチャ名
#define RELOGO_COMBO		"data/TEXTURE/combo.png"		//読み込むテクスチャ名
#define RELOGO_WAVE			"data/TEXTURE/maxwave.png"		//読み込むテクスチャ名
#define RELOGO_SCORE		"data/TEXTURE/score.png"		//読み込むテクスチャ名
#define RELOGO_POS_X		(0)		//リザルトロゴ位置X
#define RELOGO_POS_Y		(0)		//リザルトロゴ位置Y
#define RELOGO_SIZE_X		(1280)	//リザルトロゴサイズX
#define RELOGO_SIZE_Y		(720)	//リザルトロゴサイズY
#define RELOGO_CHARSIZE_X	(300)	//リザルトロゴサイズX
#define RELOGO_CHARSIZE_Y	(105)	//リザルトロゴサイズY

#define RELOGOTEXTURE_MAX	(6)		//テクスチャ数

//=============================================================================
// チュートリアルの状態
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureReLogo[RELOGOTEXTURE_MAX] = {};			//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffReLogo = NULL;	//頂点バッファへのポインタ
D3DXVECTOR3	g_ReLogopos[RELOGOTEXTURE_MAX];

//=============================================================================
// リザルトロゴの初期化処理
//=============================================================================
void InitReLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();	//デバイスの取得
	g_ReLogopos[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_ReLogopos[1] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int nCnt = 2; nCnt < RELOGOTEXTURE_MAX; nCnt++)
	{
		if (nCnt > 4)
		{
			g_ReLogopos[nCnt] = D3DXVECTOR3(300.0f, 120.0f + ((nCnt-1)*100.0f) + 100, 0.0f);
		}
		else
		{
			g_ReLogopos[nCnt] = D3DXVECTOR3(300.0f, 120.0f + ((nCnt-1)*100.0f), 0.0f);
		}
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		RELOGO_BG,
		&g_pTextureReLogo[0]);

	D3DXCreateTextureFromFile(pDevice,
		RELOGO_BG_BLACK,
		&g_pTextureReLogo[1]);

	D3DXCreateTextureFromFile(pDevice,
		RELOGO_BASIS,
		&g_pTextureReLogo[2]);

	D3DXCreateTextureFromFile(pDevice,
		RELOGO_COMBO,
		&g_pTextureReLogo[3]);

	D3DXCreateTextureFromFile(pDevice,
		RELOGO_WAVE,
		&g_pTextureReLogo[4]);

	D3DXCreateTextureFromFile(pDevice,
		RELOGO_SCORE,
		&g_pTextureReLogo[5]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * RELOGOTEXTURE_MAX,	//確保するバッファサイズ(サイズ*頂点数)
								D3DUSAGE_WRITEONLY,		//固定
								FVF_VERTEX_2D,			//頂点フォーマット
								D3DPOOL_MANAGED,		//固定
								&g_pVtxBuffReLogo,
								NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffReLogo->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	for (int nCntBG = 0; nCntBG < 2; nCntBG++)
	{
		//頂点の設定
		pVtx[0].pos = D3DXVECTOR3(g_ReLogopos[0].x, g_ReLogopos[0].y, 0.0f);	//ｚは0.0ｆで固定
		pVtx[1].pos = D3DXVECTOR3(g_ReLogopos[0].x + RELOGO_SIZE_X, g_ReLogopos[0].y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_ReLogopos[0].x, g_ReLogopos[0].y + RELOGO_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_ReLogopos[0].x + RELOGO_SIZE_X, g_ReLogopos[0].y + RELOGO_SIZE_Y, 0.0f);

		pVtx[0].rhw = 1.0f;	//1.0fで固定
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		if (nCntBG > 0)
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}
		else
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	for (int nCnt = 2; nCnt < RELOGOTEXTURE_MAX; nCnt++)
	{
		//頂点の設定
		pVtx[0].pos = D3DXVECTOR3(g_ReLogopos[nCnt].x - (RELOGO_CHARSIZE_X / 2), g_ReLogopos[nCnt].y - (RELOGO_CHARSIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
		pVtx[1].pos = D3DXVECTOR3(g_ReLogopos[nCnt].x + (RELOGO_CHARSIZE_X / 2), g_ReLogopos[nCnt].y - (RELOGO_CHARSIZE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_ReLogopos[nCnt].x - (RELOGO_CHARSIZE_X / 2), g_ReLogopos[nCnt].y + (RELOGO_CHARSIZE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_ReLogopos[nCnt].x + (RELOGO_CHARSIZE_X / 2), g_ReLogopos[nCnt].y + (RELOGO_CHARSIZE_Y / 2), 0.0f);

		pVtx[0].rhw = 1.0f;	//1.0fで固定
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点データをアンロック
	g_pVtxBuffReLogo->Unlock();

}

//=============================================================================
// リザルトロゴの終了処理
//=============================================================================
void UninitReLogo(void)
{
	//テクスチャの開放
	for (int nCnt = 0; nCnt < RELOGOTEXTURE_MAX; nCnt++)
	{
		if (g_pTextureReLogo[nCnt] != NULL)
		{					  
			g_pTextureReLogo[nCnt]->Release();
			g_pTextureReLogo[nCnt] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffReLogo != NULL)
	{
		g_pVtxBuffReLogo->Release();
		g_pVtxBuffReLogo = NULL;
	}

}

//=============================================================================
// リザルトロゴの更新処理
//=============================================================================
void UpdateReLogo(void)
{
}

//=============================================================================
// リザルトロゴの描画処理
//=============================================================================
void DrawReLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffReLogo, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);				//頂点フォーマットの設定

	for (int nCnt = 0; nCnt < RELOGOTEXTURE_MAX; nCnt++)
	{
		pDevice->SetTexture(0, g_pTextureReLogo[nCnt]);	//テクスチャの設定

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			nCnt*4,					//開始するインデックス
			2);					//プリミティブの数
	}
}