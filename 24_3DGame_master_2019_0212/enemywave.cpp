//=============================================================================
//
// 敵波処理 [enemywave.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include "enemywave.h"
#include"game.h"
#include "enemy.h"
#include "meshfield.h"
#include "boss.h"
#include "obstacle.h"
#include "sound.h"
#include"logo.h"
#include "princess.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	TEXTURE_E_WAVE			"data/TEXTURE/number0.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_E_WAVE_PIC		"data/TEXTURE/wave.png"	
#define	TEXTURE_E_WAVE_PIC02	"data/TEXTURE/ななめ.png"
#define	TEXTURE_E_WAVE_CLEAR	"data/TEXTURE/waveClear2.png"	
#define	TEXTURE_E_WAVE_START	"data/TEXTURE/nextwave.png"	

#define DIGIT_E_WAVE		(2)			//敵波の桁数
#define E_WAVE_SIZE_X		(35.0f)		//数字サイズX
#define E_WAVE_SIZE_Y		(70.0f)		//数字サイズY
#define E_WAVEPIC_SIZE_X	(105.0f)	//サイズX
#define E_WAVEPIC_SIZE_Y	(70.0f)		//サイズY
#define E_WAVEINFO_SIZE_X	(675.0f)	//サイズX
#define E_WAVEINFO_SIZE_Y	(150.0f)		//サイズY

#define SECOND				(60)		//一秒
#define E_WAVE_TEXTURE_MAX	(5)			//テクスチャ数
#define E_WAVE_TEXTURE		(4)			//数字以外のテクスチャ数

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexEwave(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureEwave[E_WAVE_TEXTURE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEwave = NULL;			// 頂点バッファへのポインタ

int						g_nEwave[DIGIT_E_WAVE];			// 敵波
D3DXVECTOR3				g_aEwavepos[DIGIT_E_WAVE];		// 数字位置
D3DXVECTOR3				g_WavePicpos[E_WAVE_TEXTURE];	//画像位置
int nCounter = 0;		//時間測定用
int g_nSmallWave = 0;	//小さいwave数
int nBreakTime = 0;		//休息時間
bool bUse[E_WAVE_TEXTURE];			//使用状態
bool bResult;

//=============================================================================
// 初期化処理
//=============================================================================
void InitEwave(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEwave;

	g_nEwave[0] = 1;
	g_nEwave[1] = E_WAVE_MAX;
	nBreakTime = 0;

	for (nCntEwave = 0; nCntEwave < DIGIT_E_WAVE; nCntEwave++)
	{
		g_aEwavepos[nCntEwave] = D3DXVECTOR3(SCREEN_WIDTH - ((DIGIT_E_WAVE+1) * E_WAVE_SIZE_X) + (nCntEwave * (E_WAVE_SIZE_X*2)), 180.0f, 0.0f);
	}

	g_WavePicpos[0] = D3DXVECTOR3(SCREEN_WIDTH - ((DIGIT_E_WAVE + 3) * E_WAVE_SIZE_X), 180.0f, 0.0f);
	g_WavePicpos[3] = D3DXVECTOR3(SCREEN_WIDTH - E_WAVE_SIZE_X * DIGIT_E_WAVE, 180.0f, 0.0f);
	g_WavePicpos[1] = D3DXVECTOR3(640.0f, 350.0f, 0.0f);
	g_WavePicpos[2] = D3DXVECTOR3(640.0f, 350.0f, 0.0f);
	bUse[0] = true;
	bUse[1] = false;
	bUse[2] = false;
	bUse[3] = true;
	bResult = true;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_E_WAVE,					// ファイルの名前
		&g_pTextureEwave[0]);			// 読み込むメモリー
	
	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_E_WAVE_PIC,				// ファイルの名前
		&g_pTextureEwave[1]);			// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_E_WAVE_CLEAR,			// ファイルの名前
		&g_pTextureEwave[2]);			// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_E_WAVE_START,			// ファイルの名前
		&g_pTextureEwave[3]);			// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_E_WAVE_PIC02,			// ファイルの名前
		&g_pTextureEwave[4]);			// 読み込むメモリー

	// 頂点情報の作成
	MakeVertexEwave(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEwave(void)
{
	// テクスチャの開放
	for (int nCnt = 0; nCnt < E_WAVE_TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureEwave[nCnt] != NULL)
		{				  
			g_pTextureEwave[nCnt]->Release();
			g_pTextureEwave[nCnt] = NULL;
		}
	}
	
	// 頂点バッファの開放
	if (g_pVtxBuffEwave != NULL)
	{
		g_pVtxBuffEwave->Release();
		g_pVtxBuffEwave = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEwave(void)
{
	if (*GetMode() == MODE_GAME)
	{
		nCounter++;
		int pNumEnemy = *GetNumEnemy();
		BOSS *pBoss = GetBoss();
		int NumBoss = GetBossUse();

		if (g_nEwave[0] >= E_WAVE_MAX)
		{
			if (NumBoss <= 0 && pNumEnemy <= 0 && nBreakTime <= 0)
			{
				SetGameState(GAMESTATE_CLEAR);
				SetLogo(LOGO_CLEAR);
				StopSound(SOUND_LABEL_BGM_GAME);
				PlaySound(SOUND_LABEL_BGM_CLEAR);
				nBreakTime++;
			}
		}
		else
		{
			if ((nCounter % (SECOND * 10) == 0 || pNumEnemy <= 2) && g_nSmallWave < g_nEwave[0] - 1)
			{
				g_nSmallWave++;

				for (int nCntEnemy = 0; nCntEnemy < 2; nCntEnemy++)
				{
					SetEnemy(D3DXVECTOR3(rand() % MESHFIELD_W * 0.5f + MESHFIELD_SIZE * 2,
						0.0f,
						rand() % (int)MESHFIELD_SIZE * MESHFIELD_D - (int)MESHFIELD_SIZE * MESHFIELD_D * 0.5f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);

					SetEnemy(D3DXVECTOR3(-rand() % MESHFIELD_W * 0.5f - MESHFIELD_SIZE * 2,
						0.0f,
						rand() % (int)MESHFIELD_SIZE * MESHFIELD_D - (int)MESHFIELD_SIZE * MESHFIELD_D * 0.5f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
				}
			}

			if (pNumEnemy <= 0 && NumBoss <= 0 && g_nSmallWave >= g_nEwave[0] - 1)
			{
				PRINCESS *pPrincess = GetPrincess();

				if (nBreakTime < 1)
				{
					pPrincess->state = PRINCESSSTATE_WIN;
					pPrincess->nCntFrame = 0;
					pPrincess->nCntKeyPrincess = 0;
					PlaySound(SOUND_LABEL_SE_CAT);
				}
				bUse[1] = true;	//クリア文字出現

				nBreakTime++;	//インターバル
				if (nBreakTime > SECOND * 3)
				{
					bUse[1] = false;
					if (nBreakTime <= (SECOND * 3) + 1)
					{
						pPrincess->state = PRINCESSSTATE_NEUTRAL;
						pPrincess->nCntFrame = 0;
						pPrincess->nCntKeyPrincess = 0;
						PlaySound(SOUND_LABEL_SE_CAT);
					}
					bUse[2] = true;	//nextwave文字出現
				}
				if (nBreakTime > SECOND * 5)
				{
					bUse[2] = false;
					AddEwave(1);	//wave数の増加
					nCounter = 0;
					g_nSmallWave = 0;
					nBreakTime = 0;
				}
			}
		}
	}
	else if (*GetMode() == MODE_RESULT)
	{
		for (int nCntEwave = 0; nCntEwave < DIGIT_E_WAVE; nCntEwave++)
		{
			g_aEwavepos[nCntEwave] = D3DXVECTOR3((SCREEN_WIDTH*0.7f) - ((DIGIT_E_WAVE + 1) * E_WAVE_SIZE_X) + (nCntEwave * (E_WAVE_SIZE_X * 2)), 420.0f, 0.0f);
		}
		g_WavePicpos[0] = D3DXVECTOR3((SCREEN_WIDTH*0.7f) - ((DIGIT_E_WAVE + 3) * E_WAVE_SIZE_X), 420.0f, 0.0f);
		g_WavePicpos[3] = D3DXVECTOR3((SCREEN_WIDTH*0.7f) - E_WAVE_SIZE_X * DIGIT_E_WAVE, 420.0f, 0.0f);

		VERTEX_2D *pVtx;	//頂点情報へのポインタ

		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffEwave->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

		for (int nCntEwave = 0; nCntEwave < DIGIT_E_WAVE; nCntEwave++)
		{	//数字
			// 頂点情報の設定
			pVtx[0].pos = D3DXVECTOR3(g_aEwavepos[nCntEwave].x - (E_WAVE_SIZE_X / 2), g_aEwavepos[nCntEwave].y - (E_WAVE_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
			pVtx[1].pos = D3DXVECTOR3(g_aEwavepos[nCntEwave].x + (E_WAVE_SIZE_X / 2), g_aEwavepos[nCntEwave].y - (E_WAVE_SIZE_Y / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEwavepos[nCntEwave].x - (E_WAVE_SIZE_X / 2), g_aEwavepos[nCntEwave].y + (E_WAVE_SIZE_Y / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEwavepos[nCntEwave].x + (E_WAVE_SIZE_X / 2), g_aEwavepos[nCntEwave].y + (E_WAVE_SIZE_Y / 2), 0.0f);

			pVtx += 4;	//頂点データのポインタを4つ分進める
		}

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			// 頂点情報の設定
			if (nCnt >= 3)
			{	//ななめ
				pVtx[0].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x - (E_WAVE_SIZE_X / 2), g_WavePicpos[nCnt].y - (E_WAVE_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
				pVtx[1].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x + (E_WAVE_SIZE_X / 2), g_WavePicpos[nCnt].y - (E_WAVE_SIZE_Y / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x - (E_WAVE_SIZE_X / 2), g_WavePicpos[nCnt].y + (E_WAVE_SIZE_Y / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x + (E_WAVE_SIZE_X / 2), g_WavePicpos[nCnt].y + (E_WAVE_SIZE_Y / 2), 0.0f);

			}
			else if (nCnt < 1)
			{	//wave
				pVtx[0].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x - (E_WAVEPIC_SIZE_X / 2), g_WavePicpos[nCnt].y - (E_WAVEPIC_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
				pVtx[1].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x + (E_WAVEPIC_SIZE_X / 2), g_WavePicpos[nCnt].y - (E_WAVEPIC_SIZE_Y / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x - (E_WAVEPIC_SIZE_X / 2), g_WavePicpos[nCnt].y + (E_WAVEPIC_SIZE_Y / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x + (E_WAVEPIC_SIZE_X / 2), g_WavePicpos[nCnt].y + (E_WAVEPIC_SIZE_Y / 2), 0.0f);
			}
			pVtx += 4;	//頂点データのポインタを4つ分進める
		}

		//頂点データをアンロック
		g_pVtxBuffEwave->Unlock();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEwave(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEwave;

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffEwave, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureEwave[0]);
	if (bResult)
	{
		// ポリゴンの描画
		for (nCntEwave = 0; nCntEwave < DIGIT_E_WAVE; nCntEwave++)
		{	//数字
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntEwave * 4,		//開始するインデックス
				2);					//プリミティブの数(ポリゴンの数)
		}

		for (int nCnt = 0; nCnt < E_WAVE_TEXTURE; nCnt++)
		{	//それ以外
			if (bUse[nCnt])
			{
				// テクスチャの設定
				pDevice->SetTexture(0, g_pTextureEwave[nCnt + 1]);

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
void MakeVertexEwave(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nCntEwave;

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (DIGIT_E_WAVE + E_WAVE_TEXTURE),	//確保するバッファサイズ(サイズ*頂点数)
								D3DUSAGE_WRITEONLY,		//固定
								FVF_VERTEX_2D,			//頂点フォーマット
								D3DPOOL_MANAGED,		//固定
								&g_pVtxBuffEwave,
								NULL);

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffEwave->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	for (nCntEwave = 0; nCntEwave < DIGIT_E_WAVE; nCntEwave++)
	{	//数字
		// 頂点情報の設定
		pVtx[0].pos = D3DXVECTOR3(g_aEwavepos[nCntEwave].x - (E_WAVE_SIZE_X / 2), g_aEwavepos[nCntEwave].y - (E_WAVE_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
		pVtx[1].pos = D3DXVECTOR3(g_aEwavepos[nCntEwave].x + (E_WAVE_SIZE_X / 2), g_aEwavepos[nCntEwave].y - (E_WAVE_SIZE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEwavepos[nCntEwave].x - (E_WAVE_SIZE_X / 2), g_aEwavepos[nCntEwave].y + (E_WAVE_SIZE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEwavepos[nCntEwave].x + (E_WAVE_SIZE_X / 2), g_aEwavepos[nCntEwave].y + (E_WAVE_SIZE_Y / 2), 0.0f);

		pVtx[0].rhw = 1.0f;	//1.0fで固定
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.1f * g_nEwave[nCntEwave], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_nEwave[nCntEwave]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * g_nEwave[nCntEwave], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_nEwave[nCntEwave]), 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}

	for (int nCnt = 0; nCnt < E_WAVE_TEXTURE; nCnt++)
	{
		// 頂点情報の設定
		if (nCnt >= 3)
		{	//ななめ
			pVtx[0].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x - (E_WAVE_SIZE_X / 2), g_WavePicpos[nCnt].y - (E_WAVE_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
			pVtx[1].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x + (E_WAVE_SIZE_X / 2), g_WavePicpos[nCnt].y - (E_WAVE_SIZE_Y / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x - (E_WAVE_SIZE_X / 2), g_WavePicpos[nCnt].y + (E_WAVE_SIZE_Y / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x + (E_WAVE_SIZE_X / 2), g_WavePicpos[nCnt].y + (E_WAVE_SIZE_Y / 2), 0.0f);

		}
		else if (nCnt < 1)
		{	//wave
			pVtx[0].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x - (E_WAVEPIC_SIZE_X / 2), g_WavePicpos[nCnt].y - (E_WAVEPIC_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
			pVtx[1].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x + (E_WAVEPIC_SIZE_X / 2), g_WavePicpos[nCnt].y - (E_WAVEPIC_SIZE_Y / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x - (E_WAVEPIC_SIZE_X / 2), g_WavePicpos[nCnt].y + (E_WAVEPIC_SIZE_Y / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x + (E_WAVEPIC_SIZE_X / 2), g_WavePicpos[nCnt].y + (E_WAVEPIC_SIZE_Y / 2), 0.0f);
		}
		else
		{	//waveClearとnextwave
			pVtx[0].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x - (E_WAVEINFO_SIZE_X / 2), g_WavePicpos[nCnt].y - (E_WAVEINFO_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
			pVtx[1].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x + (E_WAVEINFO_SIZE_X / 2), g_WavePicpos[nCnt].y - (E_WAVEINFO_SIZE_Y / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x - (E_WAVEINFO_SIZE_X / 2), g_WavePicpos[nCnt].y + (E_WAVEINFO_SIZE_Y / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x + (E_WAVEINFO_SIZE_X / 2), g_WavePicpos[nCnt].y + (E_WAVEINFO_SIZE_Y / 2), 0.0f);
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
	g_pVtxBuffEwave->Unlock();
}

//=============================================================================
// 敵波の変更
//=============================================================================
void AddEwave(int nValue)
{
	//int nDigit;
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	g_nEwave[0] += nValue;

 		//nDigit = g_nEwave % (int)powf(10.0f, DIGIT_E_WAVE) / (int)powf(10.0f, DIGIT_E_WAVE);

		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffEwave->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！
		
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.1f * g_nEwave[0], 0.0f);
 		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_nEwave[0]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * g_nEwave[0], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_nEwave[0]), 1.0f);

		//頂点データをアンロック
		g_pVtxBuffEwave->Unlock();

		if (g_nEwave[0] >= E_WAVE_MAX)
		{
			DeleteALLObstacle();	//障害物削除

			SetBoss(D3DXVECTOR3(0, 0, 500), D3DXVECTOR3(0, 0, 0));	//ボス出現
			SetBoss(D3DXVECTOR3(0, 0, -500), D3DXVECTOR3(0, 0, 0));	//ボス出現

			for (int nCntEnemy = 0; nCntEnemy < 2 ; nCntEnemy++)
			{	//ザコ敵配置
				SetEnemy(D3DXVECTOR3(rand() % MESHFIELD_W * 0.5f + MESHFIELD_SIZE * 3,
					0.0f,
					rand() % (int)MESHFIELD_SIZE * MESHFIELD_D - (int)MESHFIELD_SIZE * MESHFIELD_D * 0.5f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);

				SetEnemy(D3DXVECTOR3(-rand() % MESHFIELD_W * 0.5f - MESHFIELD_SIZE * 3,
					0.0f,
					rand() % (int)MESHFIELD_SIZE * MESHFIELD_D - (int)MESHFIELD_SIZE * MESHFIELD_D * 0.5f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
			}
			for (int nCnt = 0; nCnt < 5; nCnt++)
			{	//障害物設置
				SetObstacle(D3DXVECTOR3(rand() % 220 + 80,
					0.0f,
					rand() % (int)MESHFIELD_SIZE * MESHFIELD_D * 0.9f - (int)MESHFIELD_SIZE * MESHFIELD_D * 0.4f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				SetObstacle(D3DXVECTOR3(-rand() % 220 - 80,
					0.0f,
					rand() % (int)MESHFIELD_SIZE * MESHFIELD_D*0.9f - (int)MESHFIELD_SIZE * MESHFIELD_D * 0.4f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}

		}
		else
		{
			DeleteALLObstacle();	//障害物削除

			if (g_nEwave[0] % 3 == 0)
			{
				SetBoss(D3DXVECTOR3(0, 0, 500), D3DXVECTOR3(0, 0, 0));	//ボス出現
			}
			for (int nCntEnemy = 0; nCntEnemy < 2; nCntEnemy++)
			{	//ザコ敵配置
				SetEnemy(D3DXVECTOR3(rand() % MESHFIELD_W * 0.5f + MESHFIELD_SIZE * 3,
					0.0f,
					rand() % (int)MESHFIELD_SIZE * MESHFIELD_D - (int)MESHFIELD_SIZE * MESHFIELD_D * 0.5f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);

				SetEnemy(D3DXVECTOR3(-rand() % MESHFIELD_W * 0.5f - MESHFIELD_SIZE * 3,
					0.0f,
					rand() % (int)MESHFIELD_SIZE * MESHFIELD_D - (int)MESHFIELD_SIZE * MESHFIELD_D * 0.5f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
			}

			for (int nCnt = 0; nCnt < 5; nCnt++)
			{	//障害物配置
				SetObstacle(D3DXVECTOR3(rand() % 220 + 80,
					0.0f,
					rand() % (int)MESHFIELD_SIZE * MESHFIELD_D * 0.9f - (int)MESHFIELD_SIZE * MESHFIELD_D * 0.4f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				SetObstacle(D3DXVECTOR3(-rand() % 220 - 80,
					0.0f,
					rand() % (int)MESHFIELD_SIZE * MESHFIELD_D*0.9f - (int)MESHFIELD_SIZE * MESHFIELD_D * 0.4f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}

		}
}

//=============================================================================
// wave数の取得
//=============================================================================
int  *GetEwave(void)
{
	return &g_nEwave[0];
}

//=============================================================================
// 表示状態
//=============================================================================
void SetEwaveresult(bool Set)
{
	bResult = Set;
}
