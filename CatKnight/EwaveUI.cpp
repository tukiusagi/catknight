//=============================================================================
//
// 敵波UI処理 [EwaveUI.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include "EwaveUI.h"
#include "enemywave.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	TEXTURE_EWAVE_NUM	"data/TEXTURE/number0.png"		//数字
#define	TEXTURE_EWAVE_WAVE	"data/TEXTURE/wave.png"			//wave
#define	TEXTURE_EWAVE_SLASH	"data/TEXTURE/slash.png"		//毎
#define	TEXTURE_EWAVE_CLEAR	"data/TEXTURE/waveClear2.png"	//クリア―
#define	TEXTURE_EWAVE_START	"data/TEXTURE/nextwave.png"		//ネクスト

#define E_WAVE_SIZE_X		(35.0f)		//数字サイズX
#define E_WAVE_SIZE_Y		(70.0f)		//数字サイズY
#define E_WAVEPIC_SIZE_X	(105.0f)	//サイズX
#define E_WAVEPIC_SIZE_Y	(70.0f)		//サイズY
#define E_WAVEINFO_SIZE_X	(675.0f)	//サイズX
#define E_WAVEINFO_SIZE_Y	(150.0f)	//サイズY

#define E_WAVE_TEXTURE_MAX	(5)			//テクスチャ数

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexEwaveUI(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureEwaveUI[E_WAVE_TEXTURE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEwaveUI = NULL;			// 頂点バッファへのポインタ

int		g_nEwaveUI[DIGIT_E_WAVE];			// 敵波
EwaveUI	g_EwavePic;	//絵
bool	bResult;	//リザルト状態

//=============================================================================
// 初期化処理
//=============================================================================
void InitEwaveUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEwaveUI;

	g_nEwaveUI[0] = 1;
	g_nEwaveUI[1] = E_WAVE_MAX;

	for (nCntEwaveUI = 0; nCntEwaveUI < DIGIT_E_WAVE; nCntEwaveUI++)
	{
		g_EwavePic.Numpos[nCntEwaveUI] = D3DXVECTOR3(SCREEN_WIDTH - ((DIGIT_E_WAVE+1) * E_WAVE_SIZE_X) + (nCntEwaveUI * (E_WAVE_SIZE_X*2)), 180.0f, 0.0f);
	}

	g_EwavePic.Picpos[0] = D3DXVECTOR3(SCREEN_WIDTH - ((DIGIT_E_WAVE + 3) * E_WAVE_SIZE_X), 180.0f, 0.0f);
	g_EwavePic.Picpos[3] = D3DXVECTOR3(SCREEN_WIDTH - E_WAVE_SIZE_X * DIGIT_E_WAVE, 180.0f, 0.0f);
	g_EwavePic.Picpos[1] = D3DXVECTOR3(640.0f, 350.0f, 0.0f);
	g_EwavePic.Picpos[2] = D3DXVECTOR3(640.0f, 350.0f, 0.0f);
	g_EwavePic.bUse[0] = true;
	g_EwavePic.bUse[1] = false;
	g_EwavePic.bUse[2] = false;
	g_EwavePic.bUse[3] = true;
	bResult = true;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_EWAVE_NUM,					// ファイルの名前
		&g_pTextureEwaveUI[0]);			// 読み込むメモリー
	
	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_EWAVE_WAVE,				// ファイルの名前
		&g_pTextureEwaveUI[1]);			// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_EWAVE_CLEAR,			// ファイルの名前
		&g_pTextureEwaveUI[2]);			// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_EWAVE_START,			// ファイルの名前
		&g_pTextureEwaveUI[3]);			// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_EWAVE_SLASH,			// ファイルの名前
		&g_pTextureEwaveUI[4]);			// 読み込むメモリー

	// 頂点情報の作成
	MakeVertexEwaveUI(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEwaveUI(void)
{
	// テクスチャの開放
	for (int nCnt = 0; nCnt < E_WAVE_TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureEwaveUI[nCnt] != NULL)
		{				  
			g_pTextureEwaveUI[nCnt]->Release();
			g_pTextureEwaveUI[nCnt] = NULL;
		}
	}
	
	// 頂点バッファの開放
	if (g_pVtxBuffEwaveUI != NULL)
	{
		g_pVtxBuffEwaveUI->Release();
		g_pVtxBuffEwaveUI = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEwaveUI(void)
{
	int g_nEwaveNum = *GetEwave();

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffEwaveUI->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.1f * g_nEwaveNum, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_nEwaveNum), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f * g_nEwaveNum, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_nEwaveNum), 1.0f);

	//頂点データをアンロック
	g_pVtxBuffEwaveUI->Unlock();

	if (*GetMode() == MODE_RESULT)
	{
		for (int nCntEwaveUI = 0; nCntEwaveUI < DIGIT_E_WAVE; nCntEwaveUI++)
		{
			g_EwavePic.Numpos[nCntEwaveUI] = D3DXVECTOR3((SCREEN_WIDTH*0.7f) - ((DIGIT_E_WAVE + 1) * E_WAVE_SIZE_X) + (nCntEwaveUI * (E_WAVE_SIZE_X * 2)), 420.0f, 0.0f);
		}
		g_EwavePic.Picpos[0] = D3DXVECTOR3((SCREEN_WIDTH*0.7f) - ((DIGIT_E_WAVE + 3) * E_WAVE_SIZE_X), 420.0f, 0.0f);
		g_EwavePic.Picpos[3] = D3DXVECTOR3((SCREEN_WIDTH*0.7f) - E_WAVE_SIZE_X * DIGIT_E_WAVE, 420.0f, 0.0f);


		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffEwaveUI->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

		for (int nCntEwaveUI = 0; nCntEwaveUI < DIGIT_E_WAVE; nCntEwaveUI++)
		{	//数字
			// 頂点情報の設定
			pVtx[0].pos = D3DXVECTOR3(g_EwavePic.Numpos[nCntEwaveUI].x - (E_WAVE_SIZE_X / 2), g_EwavePic.Numpos[nCntEwaveUI].y - (E_WAVE_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
			pVtx[1].pos = D3DXVECTOR3(g_EwavePic.Numpos[nCntEwaveUI].x + (E_WAVE_SIZE_X / 2), g_EwavePic.Numpos[nCntEwaveUI].y - (E_WAVE_SIZE_Y / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_EwavePic.Numpos[nCntEwaveUI].x - (E_WAVE_SIZE_X / 2), g_EwavePic.Numpos[nCntEwaveUI].y + (E_WAVE_SIZE_Y / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_EwavePic.Numpos[nCntEwaveUI].x + (E_WAVE_SIZE_X / 2), g_EwavePic.Numpos[nCntEwaveUI].y + (E_WAVE_SIZE_Y / 2), 0.0f);

			pVtx += 4;	//頂点データのポインタを4つ分進める
		}

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			// 頂点情報の設定
			if (nCnt >= 3)
			{	//ななめ
				pVtx[0].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x - (E_WAVE_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y - (E_WAVE_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
				pVtx[1].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x + (E_WAVE_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y - (E_WAVE_SIZE_Y / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x - (E_WAVE_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y + (E_WAVE_SIZE_Y / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x + (E_WAVE_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y + (E_WAVE_SIZE_Y / 2), 0.0f);

			}
			else if (nCnt < 1)
			{	//wave
				pVtx[0].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x - (E_WAVEPIC_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y - (E_WAVEPIC_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
				pVtx[1].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x + (E_WAVEPIC_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y - (E_WAVEPIC_SIZE_Y / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x - (E_WAVEPIC_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y + (E_WAVEPIC_SIZE_Y / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x + (E_WAVEPIC_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y + (E_WAVEPIC_SIZE_Y / 2), 0.0f);
			}
			pVtx += 4;	//頂点データのポインタを4つ分進める
		}

		//頂点データをアンロック
		g_pVtxBuffEwaveUI->Unlock();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEwaveUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEwaveUI;

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffEwaveUI, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureEwaveUI[0]);
	if (bResult)
	{
		// ポリゴンの描画
		for (nCntEwaveUI = 0; nCntEwaveUI < DIGIT_E_WAVE; nCntEwaveUI++)
		{	//数字
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntEwaveUI * 4,		//開始するインデックス
				2);					//プリミティブの数(ポリゴンの数)
		}

		for (int nCnt = 0; nCnt < E_WAVE_TEXTURE; nCnt++)
		{	//それ以外
			if (g_EwavePic.bUse[nCnt])
			{
				// テクスチャの設定
				pDevice->SetTexture(0, g_pTextureEwaveUI[nCnt + 1]);

				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
					(DIGIT_E_WAVE + nCnt) * 4,		//開始するインデックス
					2);					//プリミティブの数(ポリゴンの数)
			}
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
void MakeVertexEwaveUI(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nCntEwaveUI;

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (DIGIT_E_WAVE + E_WAVE_TEXTURE),	//確保するバッファサイズ(サイズ*頂点数)
								D3DUSAGE_WRITEONLY,		//固定
								FVF_VERTEX_2D,			//頂点フォーマット
								D3DPOOL_MANAGED,		//固定
								&g_pVtxBuffEwaveUI,
								NULL);

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffEwaveUI->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	for (nCntEwaveUI = 0; nCntEwaveUI < DIGIT_E_WAVE; nCntEwaveUI++)
	{	//数字
		// 頂点情報の設定
		pVtx[0].pos = D3DXVECTOR3(g_EwavePic.Numpos[nCntEwaveUI].x - (E_WAVE_SIZE_X / 2), g_EwavePic.Numpos[nCntEwaveUI].y - (E_WAVE_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
		pVtx[1].pos = D3DXVECTOR3(g_EwavePic.Numpos[nCntEwaveUI].x + (E_WAVE_SIZE_X / 2), g_EwavePic.Numpos[nCntEwaveUI].y - (E_WAVE_SIZE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_EwavePic.Numpos[nCntEwaveUI].x - (E_WAVE_SIZE_X / 2), g_EwavePic.Numpos[nCntEwaveUI].y + (E_WAVE_SIZE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_EwavePic.Numpos[nCntEwaveUI].x + (E_WAVE_SIZE_X / 2), g_EwavePic.Numpos[nCntEwaveUI].y + (E_WAVE_SIZE_Y / 2), 0.0f);

		pVtx[0].rhw = 1.0f;	//1.0fで固定
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.1f * g_nEwaveUI[nCntEwaveUI], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_nEwaveUI[nCntEwaveUI]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * g_nEwaveUI[nCntEwaveUI], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_nEwaveUI[nCntEwaveUI]), 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}

	for (int nCnt = 0; nCnt < E_WAVE_TEXTURE; nCnt++)
	{
		// 頂点情報の設定
		if (nCnt >= 3)
		{	//ななめ
			pVtx[0].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x - (E_WAVE_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y - (E_WAVE_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
			pVtx[1].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x + (E_WAVE_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y - (E_WAVE_SIZE_Y / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x - (E_WAVE_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y + (E_WAVE_SIZE_Y / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x + (E_WAVE_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y + (E_WAVE_SIZE_Y / 2), 0.0f);

		}
		else if (nCnt < 1)
		{	//wave
			pVtx[0].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x - (E_WAVEPIC_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y - (E_WAVEPIC_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
			pVtx[1].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x + (E_WAVEPIC_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y - (E_WAVEPIC_SIZE_Y / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x - (E_WAVEPIC_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y + (E_WAVEPIC_SIZE_Y / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x + (E_WAVEPIC_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y + (E_WAVEPIC_SIZE_Y / 2), 0.0f);
		}
		else
		{	//waveClearとnextwave
			pVtx[0].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x - (E_WAVEINFO_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y - (E_WAVEINFO_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
			pVtx[1].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x + (E_WAVEINFO_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y - (E_WAVEINFO_SIZE_Y / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x - (E_WAVEINFO_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y + (E_WAVEINFO_SIZE_Y / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x + (E_WAVEINFO_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y + (E_WAVEINFO_SIZE_Y / 2), 0.0f);
		}

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
	g_pVtxBuffEwaveUI->Unlock();
}

//=============================================================================
// 敵波の変更
//=============================================================================
EwaveUI  *GetEwaveUI(void)
{
	return &g_EwavePic;
}

//=============================================================================
// 表示状態
//=============================================================================
void SetEwaveUIresult(bool Set)
{
	bResult = Set;
}
