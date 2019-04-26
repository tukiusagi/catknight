//=============================================================================
//
// スコア処理 [score.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include "score.h"
#include"game.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	TEXTURE_SCORE		"data/TEXTURE/number0.png"	// 読み込むテクスチャファイル名
#define DIGIT_SCORE				(8)		//スコアの桁数
#define SCORE_SIZE_X			(35.0f)	//サイズX
#define SCORE_SIZE_Y			(70.0f)	//サイズY

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureScore = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		// 頂点バッファへのポインタ

int						g_nScore;					// スコア
D3DXVECTOR3				g_aScorepos[DIGIT_SCORE];	// 位置
bool					g_bResult;

//=============================================================================
// 初期化処理
//=============================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntScore;

	g_nScore = 0;
	for (nCntScore = 0; nCntScore < DIGIT_SCORE; nCntScore++)
	{
		g_aScorepos[nCntScore] = D3DXVECTOR3((SCREEN_WIDTH - (DIGIT_SCORE * SCORE_SIZE_X)) + (nCntScore * SCORE_SIZE_X), 40.0f, 0.0f);
	}
	g_bResult = true;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
								TEXTURE_SCORE,		// ファイルの名前
								&g_pTextureScore);	// 読み込むメモリー
	
	// 頂点情報の作成
	MakeVertexScore(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitScore(void)
{
	// テクスチャの開放
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
	
	// 頂点バッファの開放
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateScore(void)
{
	if (*GetMode() == MODE_RESULT)
	{
		for (int nCntScore = 0; nCntScore < DIGIT_SCORE; nCntScore++)
		{
			g_aScorepos[nCntScore] = D3DXVECTOR3(((SCREEN_WIDTH*0.7f) - (DIGIT_SCORE * SCORE_SIZE_X)) + (nCntScore * SCORE_SIZE_X), 220.0f, 0.0f);
		}

		VERTEX_2D *pVtx;	//頂点情報へのポインタ

		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

		for (int nCntScore = 0; nCntScore < DIGIT_SCORE; nCntScore++)
		{
			// 頂点情報の設定
			pVtx[0].pos = D3DXVECTOR3(g_aScorepos[nCntScore].x - (SCORE_SIZE_X / 2), g_aScorepos[nCntScore].y - (SCORE_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
			pVtx[1].pos = D3DXVECTOR3(g_aScorepos[nCntScore].x + (SCORE_SIZE_X / 2), g_aScorepos[nCntScore].y - (SCORE_SIZE_Y / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aScorepos[nCntScore].x - (SCORE_SIZE_X / 2), g_aScorepos[nCntScore].y + (SCORE_SIZE_Y / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aScorepos[nCntScore].x + (SCORE_SIZE_X / 2), g_aScorepos[nCntScore].y + (SCORE_SIZE_Y / 2), 0.0f);

			pVtx += 4;	//頂点データのポインタを4つ分進める
		}

		//頂点データをアンロック
		g_pVtxBuffScore->Unlock();

	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntScore;

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureScore);

	if (g_bResult)
	{
		// ポリゴンの描画
		for (nCntScore = 0; nCntScore < DIGIT_SCORE; nCntScore++)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntScore * 4,		//開始するインデックス
				2);					//プリミティブの数(ポリゴンの数)
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nCntScore;

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * DIGIT_SCORE,	//確保するバッファサイズ(サイズ*頂点数)
								D3DUSAGE_WRITEONLY,		//固定
								FVF_VERTEX_2D,			//頂点フォーマット
								D3DPOOL_MANAGED,		//固定
								&g_pVtxBuffScore,
								NULL);

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	for (nCntScore = 0; nCntScore < DIGIT_SCORE; nCntScore++)
	{
		// 頂点情報の設定
		pVtx[0].pos = D3DXVECTOR3(g_aScorepos[nCntScore].x - (SCORE_SIZE_X / 2), g_aScorepos[nCntScore].y - (SCORE_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
		pVtx[1].pos = D3DXVECTOR3(g_aScorepos[nCntScore].x + (SCORE_SIZE_X / 2), g_aScorepos[nCntScore].y - (SCORE_SIZE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aScorepos[nCntScore].x - (SCORE_SIZE_X / 2), g_aScorepos[nCntScore].y + (SCORE_SIZE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aScorepos[nCntScore].x + (SCORE_SIZE_X / 2), g_aScorepos[nCntScore].y + (SCORE_SIZE_Y / 2), 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点データをアンロック
	g_pVtxBuffScore->Unlock();
}

//=============================================================================
// スコアの変更
//=============================================================================
void AddScore(int nValue)
{
	int nCntScore;
	int nDigit;
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	g_nScore += nValue;

	for (nCntScore = 0; nCntScore < DIGIT_SCORE; nCntScore++)
	{
 		nDigit = g_nScore % (int)powf(10.0f, DIGIT_SCORE - nCntScore) / (int)powf(10.0f, DIGIT_SCORE - (nCntScore + 1));

		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！
		
		pVtx += nCntScore * 4;	//頂点データのポインタを4つ分進める

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.1f*nDigit, 0.0f);
 		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f*nDigit), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f*nDigit, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f*nDigit), 1.0f);

		//頂点データをアンロック
		g_pVtxBuffScore->Unlock();
	}
}

//=============================================================================
// スコアの取得
//=============================================================================
int	GetScore(void)
{
	return g_nScore;
}

//=============================================================================
// 表示状態
//=============================================================================
void SetScore(bool Set)
{
	g_bResult = Set;
}
