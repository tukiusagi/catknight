//=============================================================================
//
// 操作説明UI[manual.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"manual.h"	//インクルードファイル

//=============================================================================
// マクロ定義
//=============================================================================
#define MANUAL_NAME	"data/TEXTURE/tutorial.png"	//読み込むテクスチャ名
#define MANUAL_POS		(1025)
#define MANUAL_SIZE_X	(255)
#define MANUAL_SIZE_Y	(720)

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexManual(LPDIRECT3DDEVICE9 pDevice);


//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureManual = NULL;			//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffManual = NULL;	//頂点バッファへのポインタ

//=============================================================================
// 初期化処理
//=============================================================================
void InitManual(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();	//デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		MANUAL_NAME,
		&g_pTextureManual);

	MakeVertexManual(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitManual(void)
{
	//テクスチャの開放
	if (g_pTextureManual != NULL)
	{					  
		g_pTextureManual->Release();
		g_pTextureManual = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxBuffManual != NULL)
	{
		g_pVtxBuffManual->Release();
		g_pVtxBuffManual = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateManual(void)
{
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void DrawManual(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffManual, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);				//頂点フォーマットの設定

	pDevice->SetTexture(0, g_pTextureManual);	//テクスチャの設定

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,					//開始するインデックス
		2);					//プリミティブの数(ポリゴンの数)
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexManual(LPDIRECT3DDEVICE9 pDevice)
{
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	//確保するバッファサイズ(サイズ*頂点数)
		D3DUSAGE_WRITEONLY,		//固定
		FVF_VERTEX_2D,			//頂点フォーマット
		D3DPOOL_MANAGED,		//固定
		&g_pVtxBuffManual,
		NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffManual->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

														//頂点の設定
	pVtx[0].pos = D3DXVECTOR3(MANUAL_POS, 0.0f, 0.0f);	//ｚは0.0ｆで固定
	pVtx[1].pos = D3DXVECTOR3(MANUAL_POS + MANUAL_SIZE_X, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(MANUAL_POS, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(MANUAL_POS + MANUAL_SIZE_X, SCREEN_HEIGHT, 0.0f);

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
	g_pVtxBuffManual->Unlock();
}