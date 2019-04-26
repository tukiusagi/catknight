//=============================================================================
//
// ランキング処理 [rank.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include "rank.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	TEXTURE_RANK		"data/TEXTURE/number0.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_FIRST		"data/TEXTURE/1位.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_SECOND		"data/TEXTURE/2位.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_THIRD		"data/TEXTURE/3位.png"	// 読み込むテクスチャファイル名

#define DIGIT_RANK			(8)		//スコアの桁数
#define RANKING				(6)		//順位数
#define ALL_RANK			(DIGIT_RANK * RANKING)	//表示する全部の桁数
#define RANK_SIZE_X			(35)	//数字サイズX
#define RANK_SIZE_Y			(70)	//数字サイズY
#define RANK_PIC_SIZE_X		(105)	//画像サイズX
#define RANK_PIC_SIZE_Y		(126)	//画像サイズY

#define RANK_TEXTUREMAX		(4)		//テクスチャ数
#define	RANK_TOP			(3)

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexRank(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureRank[RANK_TEXTUREMAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank = NULL;		// 頂点バッファへのポインタ

int						g_aRank[RANKING] = {30000,25000,20000,15000,10000,0};			// ランク
D3DXVECTOR3				g_aRankpos[ALL_RANK];		// 数字位置
D3DXVECTOR3				g_Picpos[RANK_TOP];	//画像位置

//=============================================================================
// 初期化処理
//=============================================================================
void InitRank(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntAll=0;
	int nCntRank;

	for (int nCntRanking = 0; nCntRanking < RANKING - 1; nCntRanking++)
	{
		for (nCntRank = 0; nCntRank < DIGIT_RANK; nCntRank++, nCntAll++)
		{
			g_aRankpos[nCntAll] = D3DXVECTOR3(530.0f + nCntRank*40.0f, 200 + (nCntRanking  * 100.0f), 0.0f);

		}
	}
	for (nCntAll = (RANKING - 1) * DIGIT_RANK; nCntAll < ALL_RANK; nCntAll++)
	{
		g_aRankpos[nCntAll] = D3DXVECTOR3(530.0f + nCntRank*35.0f, 1000.0f, 0.0f);
	}

	for (int nCnt = 0; nCnt < RANK_TOP; nCnt++)
	{
		g_Picpos[nCnt] = D3DXVECTOR3(450.0f, 190 + (nCnt  * 105.0f), 0.0f);
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_RANK,					// ファイルの名前
		&g_pTextureRank[0]);			// 読み込むメモリー
	
	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_FIRST,					// ファイルの名前
		&g_pTextureRank[1]);			// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_SECOND,					// ファイルの名前
		&g_pTextureRank[2]);			// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_THIRD,					// ファイルの名前
		&g_pTextureRank[3]);			// 読み込むメモリー


	// 頂点情報の作成
	MakeVertexRank(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitRank(void)
{
	// テクスチャの開放
	for (int nCnt = 0; nCnt < RANK_TEXTUREMAX; nCnt++)
	{
		if (g_pTextureRank[nCnt] != NULL)
		{				  
			g_pTextureRank[nCnt]->Release();
			g_pTextureRank[nCnt] = NULL;
		}
	}
	
	// 頂点バッファの開放
	if (g_pVtxBuffRank != NULL)
	{
		g_pVtxBuffRank->Release();
		g_pVtxBuffRank = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateRank(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawRank(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffRank, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRank[0]);

	// ポリゴンの描画
	for (int nCntRank = 0; nCntRank < ALL_RANK-1; nCntRank++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
								nCntRank * 4,		//開始するインデックス
								2);					//プリミティブの数(ポリゴンの数)
	}

	for (int nCnt = 0; nCnt < RANK_TOP; nCnt++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureRank[nCnt+1]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			(ALL_RANK + nCnt) * 4,		//開始するインデックス
			2);					//プリミティブの数(ポリゴンの数)
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
void MakeVertexRank(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (ALL_RANK+ RANK_TOP),	//確保するバッファサイズ(サイズ*頂点数)
								D3DUSAGE_WRITEONLY,		//固定
								FVF_VERTEX_2D,			//頂点フォーマット
								D3DPOOL_MANAGED,		//固定
								&g_pVtxBuffRank,
								NULL);

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	for (int nCntRank = 0; nCntRank < ALL_RANK-1; nCntRank++)
	{
		// 頂点情報の設定
		pVtx[0].pos = D3DXVECTOR3(g_aRankpos[nCntRank].x - (RANK_SIZE_X / 2), g_aRankpos[nCntRank].y - (RANK_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
		pVtx[1].pos = D3DXVECTOR3(g_aRankpos[nCntRank].x + (RANK_SIZE_X / 2), g_aRankpos[nCntRank].y - (RANK_SIZE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aRankpos[nCntRank].x - (RANK_SIZE_X / 2), g_aRankpos[nCntRank].y + (RANK_SIZE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aRankpos[nCntRank].x + (RANK_SIZE_X / 2), g_aRankpos[nCntRank].y + (RANK_SIZE_Y / 2), 0.0f);

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

	pVtx += 4;	//頂点データのポインタを4つ分進める

	for (int nCnt = 0; nCnt < RANK_TOP; nCnt++)
	{
		pVtx[0].pos = D3DXVECTOR3(g_Picpos[nCnt].x - (RANK_PIC_SIZE_X / 2), g_Picpos[nCnt].y - (RANK_PIC_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
		pVtx[1].pos = D3DXVECTOR3(g_Picpos[nCnt].x + (RANK_PIC_SIZE_X / 2), g_Picpos[nCnt].y - (RANK_PIC_SIZE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Picpos[nCnt].x - (RANK_PIC_SIZE_X / 2), g_Picpos[nCnt].y + (RANK_PIC_SIZE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Picpos[nCnt].x + (RANK_PIC_SIZE_X / 2), g_Picpos[nCnt].y + (RANK_PIC_SIZE_Y / 2), 0.0f);

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

		pVtx += 4;	//頂点データのポインタを4つ分進める

	}
	//頂点データをアンロック
	g_pVtxBuffRank->Unlock();
}

//=============================================================================
// スコアの変更
//=============================================================================
void AddRank(int nScore)
{
	int nWork = 0, nDigit = 0, nCntAll = 0;
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	g_aRank[5] = nScore;

	for (int nNum01 = 0; nNum01 < RANKING; nNum01++)
	{
		for (int nNum02 = 0; nNum02 < (RANKING - (nNum01+1)); nNum02++)
		{
			if (g_aRank[nNum02] < g_aRank[nNum02 + 1])
			{
				nWork = g_aRank[nNum02];
				g_aRank[nNum02] = g_aRank[nNum02 + 1];
				g_aRank[nNum02 + 1] = nWork;
			}
		}
	}


	for (int nCntRanking = 0; nCntRanking < RANKING; nCntRanking++)
	{
		for (int nCntRank = 0; nCntRank < DIGIT_RANK; nCntRank++, nCntAll++)
		{
			nDigit = g_aRank[nCntRanking] % (int)powf(10.0f, DIGIT_RANK - nCntRank) / (int)powf(10.0f, DIGIT_RANK - (nCntRank + 1));

			//頂点データの範囲をロックし、頂点バッファへのポインタを取得
			g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

			pVtx += nCntAll * 4;	//頂点データのポインタを4つ分進める

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.1f*nDigit, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f*nDigit), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f*nDigit, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f*nDigit), 1.0f);

			//頂点データをアンロック
			g_pVtxBuffRank->Unlock();
		}
	}
}