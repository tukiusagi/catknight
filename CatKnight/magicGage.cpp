//=============================================================================
//
// 魔法ゲージ処理 [magicGage.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include "magicGage.h"
#include "game.h"
#include "sound.h"
#include "wave.h"
#include "magic.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define M_GAGE_MAX			(60 * 5)	//最大Mp

#define M_GAGE_POS_X		(200.0f)	//位置X
#define M_GAGE_POS_Y		(630.0f)	//位置Y

#define M_GAGE_SIZE_X		(300)	//サイズX
#define M_GAGE_SIZE_Y		(30)	//サイズY
#define M_GAGE_TEXTUREMAX	(2)		//テクスチャ数

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexMagicGage(LPDIRECT3DDEVICE9 pDevice);


//=============================================================================
// グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureMagicGage[M_GAGE_TEXTUREMAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMagicGage = NULL;	// 頂点バッファへのポインタ

MAGICGAGE				g_aMagicGage;	// Mp
D3DXCOLOR				g_aMagicColor[M_GAGE_TEXTUREMAX];

//=============================================================================
// 初期化処理
//=============================================================================
void InitMagicGage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_aMagicGage.pos = D3DXVECTOR3(M_GAGE_POS_X, M_GAGE_POS_Y, 0.0f);
	g_aMagicGage.fNowMp = 0;

	g_aMagicColor[0] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	g_aMagicColor[1] = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャの読み込み
	for (int nCnt = 0; nCnt < M_GAGE_TEXTUREMAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice,				
									NULL,
									&g_pTextureMagicGage[nCnt]);
	}

	// 頂点情報の作成
	MakeVertexMagicGage(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMagicGage(void)
{
	// テクスチャの開放
	for (int nCnt = 0; nCnt < M_GAGE_TEXTUREMAX; nCnt++)
	{
		if (g_pTextureMagicGage[nCnt] != NULL)
		{				  
			g_pTextureMagicGage[nCnt]->Release();
			g_pTextureMagicGage[nCnt] = NULL;
		}
	}
	
	// 頂点バッファの開放
	if (g_pVtxBuffMagicGage != NULL)
	{
		g_pVtxBuffMagicGage->Release();
		g_pVtxBuffMagicGage = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMagicGage(void)
{
	VERTEX_2D *pVtx;

	if (g_aMagicGage.fNowMp >= M_GAGE_MAX)
	{	//最大で黄色
		g_aMagicColor[1] = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	}

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffMagicGage->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	pVtx[4].col = g_aMagicColor[1];
	pVtx[5].col = g_aMagicColor[1];
	pVtx[6].col = g_aMagicColor[1];
	pVtx[7].col = g_aMagicColor[1];

	//頂点データをアンロック
	g_pVtxBuffMagicGage->Unlock();

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMagicGage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffMagicGage, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	for (int nCnt = 0; nCnt < M_GAGE_TEXTUREMAX; nCnt++)
	{
		pDevice->SetTexture(0, g_pTextureMagicGage[nCnt]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			nCnt * 4,			//開始するインデックス
			2);					//プリミティブの数(ポリゴンの数)
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
void MakeVertexMagicGage(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nCntMagicGage;

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * M_GAGE_TEXTUREMAX,	//確保するバッファサイズ(サイズ*頂点数)
								D3DUSAGE_WRITEONLY,		//固定
								FVF_VERTEX_2D,			//頂点フォーマット
								D3DPOOL_MANAGED,		//固定
								&g_pVtxBuffMagicGage,
								NULL);

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffMagicGage->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！
	
	pVtx[0].pos = D3DXVECTOR3(g_aMagicGage.pos.x, g_aMagicGage.pos.y, 0.0f);	//ｚは0.0ｆで固定
	pVtx[1].pos = D3DXVECTOR3(g_aMagicGage.pos.x + M_GAGE_SIZE_X, g_aMagicGage.pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aMagicGage.pos.x, g_aMagicGage.pos.y + M_GAGE_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aMagicGage.pos.x + M_GAGE_SIZE_X, g_aMagicGage.pos.y + M_GAGE_SIZE_Y, 0.0f);

	pVtx[4].pos = D3DXVECTOR3(g_aMagicGage.pos.x, g_aMagicGage.pos.y, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_aMagicGage.pos.x + ((g_aMagicGage.fNowMp / M_GAGE_MAX)*M_GAGE_SIZE_X), g_aMagicGage.pos.y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_aMagicGage.pos.x, g_aMagicGage.pos.y + M_GAGE_SIZE_Y, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aMagicGage.pos.x + ((g_aMagicGage.fNowMp / M_GAGE_MAX)*M_GAGE_SIZE_X), g_aMagicGage.pos.y + M_GAGE_SIZE_Y, 0.0f);

	for (nCntMagicGage = 0; nCntMagicGage < M_GAGE_TEXTUREMAX; nCntMagicGage++)
	{
		pVtx[0].rhw = 1.0f;	//1.0fで固定
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = g_aMagicColor[nCntMagicGage];
		pVtx[1].col = g_aMagicColor[nCntMagicGage];
		pVtx[2].col = g_aMagicColor[nCntMagicGage];
		pVtx[3].col = g_aMagicColor[nCntMagicGage];

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点データをアンロック
	g_pVtxBuffMagicGage->Unlock();
}

//=============================================================================
// Mpの減少
//=============================================================================
void DecreaseMagicGage()
{
	g_aMagicGage.fNowMp -= M_GAGE_MAX;
	g_aMagicColor[1] = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffMagicGage->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	// 頂点情報の設定
	pVtx[4].pos = D3DXVECTOR3(g_aMagicGage.pos.x, g_aMagicGage.pos.y, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_aMagicGage.pos.x + ((g_aMagicGage.fNowMp / M_GAGE_MAX)*M_GAGE_SIZE_X), g_aMagicGage.pos.y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_aMagicGage.pos.x, g_aMagicGage.pos.y + M_GAGE_SIZE_Y, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aMagicGage.pos.x + ((g_aMagicGage.fNowMp / M_GAGE_MAX)*M_GAGE_SIZE_X), g_aMagicGage.pos.y + M_GAGE_SIZE_Y, 0.0f);

	pVtx[4].col = g_aMagicColor[1];
	pVtx[5].col = g_aMagicColor[1];
	pVtx[6].col = g_aMagicColor[1];
	pVtx[7].col = g_aMagicColor[1];

	//頂点データをアンロック
	g_pVtxBuffMagicGage->Unlock();

}

//=============================================================================
// Mpの増加
//=============================================================================
void IncreaseMagicGage(int nValue)
{
	g_aMagicGage.fNowMp += nValue;

	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffMagicGage->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	// 頂点情報の設定
	pVtx[4].pos = D3DXVECTOR3(g_aMagicGage.pos.x, g_aMagicGage.pos.y, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_aMagicGage.pos.x + ((g_aMagicGage.fNowMp / M_GAGE_MAX)*M_GAGE_SIZE_X), g_aMagicGage.pos.y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_aMagicGage.pos.x, g_aMagicGage.pos.y + M_GAGE_SIZE_Y, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aMagicGage.pos.x + ((g_aMagicGage.fNowMp / M_GAGE_MAX)*M_GAGE_SIZE_X), g_aMagicGage.pos.y + M_GAGE_SIZE_Y, 0.0f);

	//頂点データをアンロック
	g_pVtxBuffMagicGage->Unlock();

	if (g_aMagicGage.fNowMp >= M_GAGE_MAX)
	{	//チャージ完了
		MAGIC *pMagic = GetMagic();
		WAVE *pWave = GetWave();
		pWave->state = WAVE_STATE_CHARGEMAX;
		pWave->col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pMagic->col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		PlaySound(SOUND_LABEL_SE_CHARGEMAX);
	}
}

//=============================================================================
// Mpの取得
//=============================================================================
float GetMagicGage(void)
{
	return g_aMagicGage.fNowMp;
}