//=============================================================================
//
// ヘルプ処理[help.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"help.h"
#include "princess.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define HELP_TEXTURE		"data/TEXTURE/neko.png"	//読み込むテクスチャ

#define HELP_SIZE_X		(200)	//ポリゴンサイズX
#define HELP_SIZE_Y		(200)	//ポリゴンサイズY

//=============================================================================
// ヘルプの種類
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXCOLOR	col;
	bool		bUse;
	float		fFlash;
}HELP;

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureHelp = NULL;	//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHelp = NULL;	//頂点バッファへのポインタ

HELP	g_Help;

//=============================================================================
// 初期化処理
//=============================================================================
void InitHelp(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	pDevice = GetDevice();	//デバイスの取得

	g_Help.pos = D3DXVECTOR3(HELP_SIZE_X / 2, SCREEN_HEIGHT - HELP_SIZE_Y / 2, 0.0f);
	g_Help.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_Help.fFlash = 0.1f;
	g_Help.bUse = false;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		HELP_TEXTURE,
		&g_pTextureHelp);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	//確保するバッファサイズ(サイズ*頂点数)
		D3DUSAGE_WRITEONLY,		//固定
		FVF_VERTEX_2D,			//頂点フォーマット
		D3DPOOL_MANAGED,		//固定
		&g_pVtxBuffHelp,
		NULL);


	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffHelp->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	//頂点の設定
	pVtx[0].pos = D3DXVECTOR3(g_Help.pos.x - (HELP_SIZE_X / 2), g_Help.pos.y - (HELP_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
	pVtx[1].pos = D3DXVECTOR3(g_Help.pos.x + (HELP_SIZE_X / 2), g_Help.pos.y - (HELP_SIZE_Y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Help.pos.x - (HELP_SIZE_X / 2), g_Help.pos.y + (HELP_SIZE_Y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Help.pos.x + (HELP_SIZE_X / 2), g_Help.pos.y + (HELP_SIZE_Y / 2), 0.0f);

	pVtx[0].rhw = 1.0f;	//1.0fで固定
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = g_Help.col;
	pVtx[1].col = g_Help.col;
	pVtx[2].col = g_Help.col;
	pVtx[3].col = g_Help.col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データをアンロック
	g_pVtxBuffHelp->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitHelp(void)
{
	//テクスチャの開放
	if (g_pTextureHelp != NULL)
	{
		g_pTextureHelp->Release();
		g_pTextureHelp = NULL;
	}
	//頂点バッファの開放
	if (g_pVtxBuffHelp != NULL)
	{
		g_pVtxBuffHelp->Release();
		g_pVtxBuffHelp = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateHelp(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nMove = 10;
	PRINCESS *pPrincess = GetPrincess();

	if (pPrincess->bPinch)
	{
		g_Help.bUse = true;
	}
	else
	{
		g_Help.bUse = false;
	}

	g_Help.col.a -= g_Help.fFlash;
	if (g_Help.col.a < 0 || g_Help.col.a > 1)
	{	//点滅
		g_Help.fFlash *= -1;
	}

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffHelp->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	pVtx[0].pos = D3DXVECTOR3(g_Help.pos.x - (HELP_SIZE_X / 2), g_Help.pos.y - (HELP_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
	pVtx[1].pos = D3DXVECTOR3(g_Help.pos.x + (HELP_SIZE_X / 2), g_Help.pos.y - (HELP_SIZE_Y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Help.pos.x - (HELP_SIZE_X / 2), g_Help.pos.y + (HELP_SIZE_Y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Help.pos.x + (HELP_SIZE_X / 2), g_Help.pos.y + (HELP_SIZE_Y / 2), 0.0f);

	//頂点カラーの設定
	pVtx[0].col = g_Help.col;
	pVtx[1].col = g_Help.col;
	pVtx[2].col = g_Help.col;
	pVtx[3].col = g_Help.col;

	//頂点データをアンロック
	g_pVtxBuffHelp->Unlock();

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawHelp(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffHelp, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);				//頂点フォーマットの設定

	if (g_Help.bUse)
	{
		pDevice->SetTexture(0, g_pTextureHelp);		//テクスチャの設定

			//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			0,					//開始するインデックス
			2);					//プリミティブの数(ポリゴンの数)
	}
}

//=============================================================================
// ヘルプのセット
//=============================================================================
void SetHelp(void)
{
	g_Help.bUse = true;
}

//=============================================================================
// ヘルプの消去
//=============================================================================
void DeleteHelp(void)
{
	g_Help.bUse = false;
}
