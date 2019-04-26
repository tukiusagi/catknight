//=============================================================================
//
// スコア処理 [lastscore.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include "lastscore.h"
#include"game.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	TEXTURE_LASTSCORE		"data/TEXTURE/number0.png"	// 読み込むテクスチャファイル名
#define DIGIT_LASTSCORE				(8)		//スコアの桁数
#define LASTSCORE_SIZE_X			(35.0f)	//サイズX
#define LASTSCORE_SIZE_Y			(70.0f)	//サイズY

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexLastScore(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureLastScore = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLastScore = NULL;		// 頂点バッファへのポインタ

int						g_ScoreLast;					// スコア
D3DXVECTOR3				g_aLastScorepos[DIGIT_LASTSCORE];	// 位置
bool					g_bScore;

//=============================================================================
// 初期化処理
//=============================================================================
void InitLastScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntLastScore;

	g_ScoreLast = 0;
	for (nCntLastScore = 0; nCntLastScore < DIGIT_LASTSCORE; nCntLastScore++)
	{
		g_aLastScorepos[nCntLastScore] = D3DXVECTOR3(((SCREEN_WIDTH*0.7f) - (DIGIT_LASTSCORE * LASTSCORE_SIZE_X)) + (nCntLastScore * LASTSCORE_SIZE_X), 620.0f, 0.0f);
	}
	g_bScore = false;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
								TEXTURE_LASTSCORE,		// ファイルの名前
								&g_pTextureLastScore);	// 読み込むメモリー
	
	// 頂点情報の作成
	MakeVertexLastScore(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLastScore(void)
{
	// テクスチャの開放
	if (g_pTextureLastScore != NULL)
	{
		g_pTextureLastScore->Release();
		g_pTextureLastScore = NULL;
	}
	
	// 頂点バッファの開放
	if (g_pVtxBuffLastScore != NULL)
	{
		g_pVtxBuffLastScore->Release();
		g_pVtxBuffLastScore = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLastScore(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLastScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntLastScore;

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffLastScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureLastScore);

	if (g_bScore)
	{
		// ポリゴンの描画
		for (nCntLastScore = 0; nCntLastScore < DIGIT_LASTSCORE; nCntLastScore++)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntLastScore * 4,		//開始するインデックス
				2);					//プリミティブの数(ポリゴンの数)
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
void MakeVertexLastScore(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nCntLastScore;

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * DIGIT_LASTSCORE,	//確保するバッファサイズ(サイズ*頂点数)
								D3DUSAGE_WRITEONLY,		//固定
								FVF_VERTEX_2D,			//頂点フォーマット
								D3DPOOL_MANAGED,		//固定
								&g_pVtxBuffLastScore,
								NULL);

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffLastScore->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	for (nCntLastScore = 0; nCntLastScore < DIGIT_LASTSCORE; nCntLastScore++)
	{
		// 頂点情報の設定
		pVtx[0].pos = D3DXVECTOR3(g_aLastScorepos[nCntLastScore].x - (LASTSCORE_SIZE_X / 2), g_aLastScorepos[nCntLastScore].y - (LASTSCORE_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
		pVtx[1].pos = D3DXVECTOR3(g_aLastScorepos[nCntLastScore].x + (LASTSCORE_SIZE_X / 2), g_aLastScorepos[nCntLastScore].y - (LASTSCORE_SIZE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aLastScorepos[nCntLastScore].x - (LASTSCORE_SIZE_X / 2), g_aLastScorepos[nCntLastScore].y + (LASTSCORE_SIZE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aLastScorepos[nCntLastScore].x + (LASTSCORE_SIZE_X / 2), g_aLastScorepos[nCntLastScore].y + (LASTSCORE_SIZE_Y / 2), 0.0f);

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
	g_pVtxBuffLastScore->Unlock();
}

//=============================================================================
// スコアの変更
//=============================================================================
void AddLastScore(int nValue)
{
	int nCntLastScore;
	int nDigit;
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	g_ScoreLast += nValue;

	for (nCntLastScore = 0; nCntLastScore < DIGIT_LASTSCORE; nCntLastScore++)
	{
 		nDigit = g_ScoreLast % (int)powf(10.0f, DIGIT_LASTSCORE - nCntLastScore) / (int)powf(10.0f, DIGIT_LASTSCORE - (nCntLastScore + 1));

		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffLastScore->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！
		
		pVtx += nCntLastScore * 4;	//頂点データのポインタを4つ分進める

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.1f*nDigit, 0.0f);
 		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f*nDigit), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f*nDigit, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f*nDigit), 1.0f);

		//頂点データをアンロック
		g_pVtxBuffLastScore->Unlock();
	}
}

//=============================================================================
// スコアの変更
//=============================================================================
int	GetLastScore(void)
{
	return g_ScoreLast;
}

//=============================================================================
// 表示状態
//=============================================================================
void SetLastScore(bool Set)
{
	g_bScore = Set;
}