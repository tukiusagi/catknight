//=============================================================================
//
// コンボ処理 [combo.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include "combo.h"
#include"game.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	TEXTURE_COMBO		"data/TEXTURE/number0.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_HIT			"data/TEXTURE/hit2.png"	// 読み込むテクスチャファイル名
#define DIGIT_COMBO			(2)		//コンボの桁数
#define COMBO_SIZE_X		(35.0f)	//数字サイズX
#define COMBO_SIZE_Y		(70.0f)	//数字サイズY
#define HIT_SIZE_X			(75.0f)	//サイズX
#define HIT_SIZE_Y			(50.0f)	//サイズY
#define TEXTURE_COMBO_MAX	(2)		//テクスチャ数

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexCombo(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureCombo[TEXTURE_COMBO_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCombo = NULL;		// 頂点バッファへのポインタ

int						g_nCombo;					// コンボ
int						g_nMaxCombo;				//最大コンボ
D3DXVECTOR3				g_aCombopos[DIGIT_COMBO+1];	// 位置
bool					g_bResultCombo;

//=============================================================================
// 初期化処理
//=============================================================================
void InitCombo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntCombo;

	g_nCombo = 0;
	g_nMaxCombo = 0;
	for (nCntCombo = 0; nCntCombo < DIGIT_COMBO; nCntCombo++)
	{
		g_aCombopos[nCntCombo] = D3DXVECTOR3((SCREEN_WIDTH - COMBO_SIZE_X * DIGIT_COMBO * 2) + (nCntCombo * COMBO_SIZE_X), 110.0f, 0.0f);
	}
	g_aCombopos[2] = D3DXVECTOR3(SCREEN_WIDTH - COMBO_SIZE_X * 1.3f, 110.0f, 0.0f);
	g_bResultCombo = true;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
								TEXTURE_COMBO,		// ファイルの名前
								&g_pTextureCombo[0]);	// 読み込むメモリー
	
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_HIT,		// ファイルの名前
		&g_pTextureCombo[1]);	// 読み込むメモリー

	// 頂点情報の作成
	MakeVertexCombo(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCombo(void)
{
	// テクスチャの開放
	for (int nCnt = 0; nCnt < TEXTURE_COMBO_MAX; nCnt++)
	{
		if (g_pTextureCombo[nCnt] != NULL)
		{				   
			g_pTextureCombo[nCnt]->Release();
			g_pTextureCombo[nCnt] = NULL;
		}
	}
	
	// 頂点バッファの開放
	if (g_pVtxBuffCombo != NULL)
	{
		g_pVtxBuffCombo->Release();
		g_pVtxBuffCombo = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCombo(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nCntCombo;

	if (*GetMode() == MODE_RESULT)
	{
		for (int nCntCombo = 0; nCntCombo < DIGIT_COMBO; nCntCombo++)
		{
			g_aCombopos[nCntCombo] = D3DXVECTOR3(((SCREEN_WIDTH*0.76f) - COMBO_SIZE_X * DIGIT_COMBO * 2) + (nCntCombo * COMBO_SIZE_X), 320.0f, 0.0f);
		}
		g_aCombopos[2] = D3DXVECTOR3((SCREEN_WIDTH*0.76f) - COMBO_SIZE_X * 1.3f, 320.0f, 0.0f);

		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

		for (nCntCombo = 0; nCntCombo < DIGIT_COMBO; nCntCombo++)
		{
			// 頂点情報の設定
			pVtx[0].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x - (COMBO_SIZE_X / 2), g_aCombopos[nCntCombo].y - (COMBO_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
			pVtx[1].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x + (COMBO_SIZE_X / 2), g_aCombopos[nCntCombo].y - (COMBO_SIZE_Y / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x - (COMBO_SIZE_X / 2), g_aCombopos[nCntCombo].y + (COMBO_SIZE_Y / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x + (COMBO_SIZE_X / 2), g_aCombopos[nCntCombo].y + (COMBO_SIZE_Y / 2), 0.0f);
			pVtx += 4;	//頂点データのポインタを4つ分進める

		}
		pVtx[0].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x - (HIT_SIZE_X / 2), g_aCombopos[nCntCombo].y - (HIT_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
		pVtx[1].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x + (HIT_SIZE_X / 2), g_aCombopos[nCntCombo].y - (HIT_SIZE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x - (HIT_SIZE_X / 2), g_aCombopos[nCntCombo].y + (HIT_SIZE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x + (HIT_SIZE_X / 2), g_aCombopos[nCntCombo].y + (HIT_SIZE_Y / 2), 0.0f);
		
		//頂点データをアンロック
		g_pVtxBuffCombo->Unlock();

	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCombo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntCombo;

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffCombo, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureCombo[0]);

	if (g_bResultCombo)
	{
		// ポリゴンの描画
		for (nCntCombo = 0; nCntCombo < DIGIT_COMBO; nCntCombo++)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntCombo * 4,		//開始するインデックス
				2);					//プリミティブの数(ポリゴンの数)
		}

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureCombo[1]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			DIGIT_COMBO * 4,		//開始するインデックス
			2);					//プリミティブの数(ポリゴンの数)
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
void MakeVertexCombo(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nCntCombo;

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (DIGIT_COMBO + 1),	//確保するバッファサイズ(サイズ*頂点数)
								D3DUSAGE_WRITEONLY,		//固定
								FVF_VERTEX_2D,			//頂点フォーマット
								D3DPOOL_MANAGED,		//固定
								&g_pVtxBuffCombo,
								NULL);

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	for (nCntCombo = 0; nCntCombo < DIGIT_COMBO; nCntCombo++)
	{
		// 頂点情報の設定
		pVtx[0].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x - (COMBO_SIZE_X / 2), g_aCombopos[nCntCombo].y - (COMBO_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
		pVtx[1].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x + (COMBO_SIZE_X / 2), g_aCombopos[nCntCombo].y - (COMBO_SIZE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x - (COMBO_SIZE_X / 2), g_aCombopos[nCntCombo].y + (COMBO_SIZE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x + (COMBO_SIZE_X / 2), g_aCombopos[nCntCombo].y + (COMBO_SIZE_Y / 2), 0.0f);

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

	// 頂点情報の設定
	pVtx[0].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x - (HIT_SIZE_X / 2), g_aCombopos[nCntCombo].y - (HIT_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
	pVtx[1].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x + (HIT_SIZE_X / 2), g_aCombopos[nCntCombo].y - (HIT_SIZE_Y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x - (HIT_SIZE_X / 2), g_aCombopos[nCntCombo].y + (HIT_SIZE_Y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x + (HIT_SIZE_X / 2), g_aCombopos[nCntCombo].y + (HIT_SIZE_Y / 2), 0.0f);

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

	//頂点データをアンロック
	g_pVtxBuffCombo->Unlock();
}

//=============================================================================
// コンボの変更
//=============================================================================
void AddCombo(int nValue)
{
	int nDigit;
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	g_nCombo += nValue;

	if (g_nCombo > g_nMaxCombo)
	{
		g_nMaxCombo = g_nCombo;	//最大コンボ保存
	}

	for (int nCntCombo = 0; nCntCombo < DIGIT_COMBO; nCntCombo++)
	{
 		nDigit = g_nCombo % (int)powf(10.0f, DIGIT_COMBO - nCntCombo) / (int)powf(10.0f, DIGIT_COMBO - (nCntCombo + 1));

		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！
		
		pVtx += nCntCombo * 4;	//頂点データのポインタを4つ分進める

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.1f*nDigit, 0.0f);
 		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f*nDigit), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f*nDigit, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f*nDigit), 1.0f);

		//頂点データをアンロック
		g_pVtxBuffCombo->Unlock();
	}
}

//=============================================================================
// コンボの減少
//=============================================================================
void DeleteCombo(void)
{
	int nDigit;
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	if (g_nCombo > g_nMaxCombo)
	{
		g_nMaxCombo = g_nCombo;	//最大コンボ保存
	}

	g_nCombo = 0;

	for (int nCntCombo = 0; nCntCombo < DIGIT_COMBO; nCntCombo++)
	{
		nDigit = g_nCombo % (int)powf(10.0f, DIGIT_COMBO - nCntCombo) / (int)powf(10.0f, DIGIT_COMBO - (nCntCombo + 1));

		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

		pVtx += nCntCombo * 4;	//頂点データのポインタを4つ分進める

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.1f*nDigit, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f*nDigit), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f*nDigit, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f*nDigit), 1.0f);

		//頂点データをアンロック
		g_pVtxBuffCombo->Unlock();
	}

}

//=============================================================================
// コンボの減少
//=============================================================================
int	GetCombo(void)
{
	return g_nMaxCombo;
}

//=============================================================================
// 表示状態
//=============================================================================
void SetCombo(bool Set)
{
	g_bResultCombo = Set;
}