//=============================================================================
//
// 姫ライフ処理 [lifePrincess.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include "lifePrincess.h"
#include "game.h"
#include "princess.h"
#include "sound.h"
#include "logo.h"
#include "score.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define LIFEBAR_POS_X		(200.0f)	//位置X
#define LIFEBAR_POS_Y		(600.0f)	//位置Y

#define LIFEBAR_SIZE_X		(400)	//サイズX
#define LIFEBAR_SIZE_Y		(30)	//サイズY
#define LIFEBAR_TEXTUREMAX	(2)		//テクスチャ数

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexPLife(LPDIRECT3DDEVICE9 pDevice);


//=============================================================================
// グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTexturePLife[LIFEBAR_TEXTUREMAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPLife = NULL;	// 頂点バッファへのポインタ

LIFEBAR					g_aPLife;	// ライフ
D3DXCOLOR				g_aLifeColor[LIFEBAR_TEXTUREMAX];	//色

//=============================================================================
// 初期化処理
//=============================================================================
void InitPLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PRINCESS *pPrincess = GetPrincess();

	g_aPLife.pos = D3DXVECTOR3(LIFEBAR_POS_X, LIFEBAR_POS_Y, 0.0f);
	g_aPLife.fNowLife = pPrincess->nMaxLife;
	g_aPLife.fMaxLife = pPrincess->nMaxLife;

	g_aLifeColor[0] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	g_aLifeColor[1] = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

	// テクスチャの読み込み
	for (int nCnt = 0; nCnt < LIFEBAR_TEXTUREMAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice,				
									NULL,
									&g_pTexturePLife[nCnt]);
	}

	// 頂点情報の作成
	MakeVertexPLife(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPLife(void)
{
	// テクスチャの開放
	for (int nCnt = 0; nCnt < LIFEBAR_TEXTUREMAX; nCnt++)
	{
		if (g_pTexturePLife[nCnt] != NULL)
		{				  
			g_pTexturePLife[nCnt]->Release();
			g_pTexturePLife[nCnt] = NULL;
		}
	}
	
	// 頂点バッファの開放
	if (g_pVtxBuffPLife != NULL)
	{
		g_pVtxBuffPLife->Release();
		g_pVtxBuffPLife = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePLife(void)
{
	VERTEX_2D *pVtx;

	if (g_aPLife.fNowLife < (g_aPLife.fMaxLife / 3))
	{	//三分の一より少ないとき赤表示
		g_aLifeColor[1] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffPLife->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	pVtx[4].col = g_aLifeColor[1];
	pVtx[5].col = g_aLifeColor[1];
	pVtx[6].col = g_aLifeColor[1];
	pVtx[7].col = g_aLifeColor[1];

	//頂点データをアンロック
	g_pVtxBuffPLife->Unlock();

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffPLife, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	for (int nCnt = 0; nCnt < LIFEBAR_TEXTUREMAX; nCnt++)
	{
		pDevice->SetTexture(0, g_pTexturePLife[nCnt]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			nCnt * 4,			//開始するインデックス
			2);					//プリミティブの数(ポリゴンの数)
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
void MakeVertexPLife(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nCntPLife;

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * LIFEBAR_TEXTUREMAX,	//確保するバッファサイズ(サイズ*頂点数)
								D3DUSAGE_WRITEONLY,		//固定
								FVF_VERTEX_2D,			//頂点フォーマット
								D3DPOOL_MANAGED,		//固定
								&g_pVtxBuffPLife,
								NULL);

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffPLife->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	for (nCntPLife = 0; nCntPLife < LIFEBAR_TEXTUREMAX; nCntPLife++)
	{
		// 頂点情報の設定
		pVtx[0].pos = D3DXVECTOR3(g_aPLife.pos.x, g_aPLife.pos.y, 0.0f);	//ｚは0.0ｆで固定
		pVtx[1].pos = D3DXVECTOR3(g_aPLife.pos.x + ((g_aPLife.fNowLife / g_aPLife.fMaxLife)*LIFEBAR_SIZE_X), g_aPLife.pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPLife.pos.x, g_aPLife.pos.y + LIFEBAR_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPLife.pos.x + ((g_aPLife.fNowLife / g_aPLife.fMaxLife)*LIFEBAR_SIZE_X), g_aPLife.pos.y + LIFEBAR_SIZE_Y, 0.0f);

		pVtx[0].rhw = 1.0f;	//1.0fで固定
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = g_aLifeColor[nCntPLife];
		pVtx[1].col = g_aLifeColor[nCntPLife];
		pVtx[2].col = g_aLifeColor[nCntPLife];
		pVtx[3].col = g_aLifeColor[nCntPLife];

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点データをアンロック
	g_pVtxBuffPLife->Unlock();
}

//=============================================================================
// ライフの減少
//=============================================================================
void DecreasePLife(int nValue)
{
	PRINCESS *pPrincess = GetPrincess();

	if (*GetGameState() == GAMESTATE_NORMAL)
	{
		g_aPLife.fNowLife -= nValue;
		pPrincess->bDamege = true;
		
		VERTEX_2D *pVtx;

		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffPLife->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

		// 頂点情報の設定
		pVtx[4].pos = D3DXVECTOR3(g_aPLife.pos.x, g_aPLife.pos.y, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(g_aPLife.pos.x  + ((g_aPLife.fNowLife/g_aPLife.fMaxLife)*LIFEBAR_SIZE_X) , g_aPLife.pos.y, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(g_aPLife.pos.x, g_aPLife.pos.y + LIFEBAR_SIZE_Y, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(g_aPLife.pos.x  + ((g_aPLife.fNowLife / g_aPLife.fMaxLife)*LIFEBAR_SIZE_X), g_aPLife.pos.y + LIFEBAR_SIZE_Y, 0.0f);

		//頂点データをアンロック
		g_pVtxBuffPLife->Unlock();

		if (g_aPLife.fNowLife <= 0 && *GetGameState() != GAMESTATE_GAMEOVER)
		{
			g_aPLife.fNowLife = 0;
			SetGameState(GAMESTATE_GAMEOVER);
			pPrincess->state = PRINCESSSTATE_DETH;
			pPrincess->nCntFrame = 0;
			pPrincess->nCntKeyPrincess = 0;
			SetLogo(LOGO_GAMEOVER);
			StopSound(SOUND_LABEL_BGM_GAME);
			PlaySound(SOUND_LABEL_BGM_GAMEOVER);
		}
	}
}

//=============================================================================
// ライフの増加
//=============================================================================
void IncreasePLife(int nlife)
{
	g_aPLife.fNowLife += nlife;

	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffPLife->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	// 頂点情報の設定
	pVtx[4].pos = D3DXVECTOR3(g_aPLife.pos.x, g_aPLife.pos.y, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_aPLife.pos.x + ((g_aPLife.fNowLife / g_aPLife.fMaxLife)*LIFEBAR_SIZE_X), g_aPLife.pos.y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_aPLife.pos.x, g_aPLife.pos.y + LIFEBAR_SIZE_Y, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aPLife.pos.x + ((g_aPLife.fNowLife / g_aPLife.fMaxLife)*LIFEBAR_SIZE_X), g_aPLife.pos.y + LIFEBAR_SIZE_Y, 0.0f);

	//頂点データをアンロック
	g_pVtxBuffPLife->Unlock();
}

//=============================================================================
// ライフの取得
//=============================================================================
float GetPLife(void)
{
	return g_aPLife.fNowLife;
}