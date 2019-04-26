//=============================================================================
//
// プレススタート処理[pressstart.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"pressstart.h"
#include "input.h"	
#include "joystick.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureStart = NULL;	//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStart = NULL;	//頂点バッファへのポインタ

D3DXVECTOR3 g_Startpos;		//位置
D3DXCOLOR	g_StartCol;		//色
int			g_nCntStart;	//点滅カウンター
float		nInvisible;		//点滅減少

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
void InitStart(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	pDevice = GetDevice();	//デバイスの取得

	if (*GetMode() == MODE_TITLE)
	{
		g_Startpos = D3DXVECTOR3(640.0f, 600.0f, 0.0f);
	}
	else if (*GetMode() == MODE_TUTORIAL)
	{
		g_Startpos = D3DXVECTOR3(1050.0f, SCREEN_HEIGHT-209- (START_SIZE_Y/2), 0.0f);
	}
		g_StartCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		nInvisible = 0.007f;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
							START_NAME,
							&g_pTextureStart);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	//確保するバッファサイズ(サイズ*頂点数)
								D3DUSAGE_WRITEONLY,		//固定
								FVF_VERTEX_2D,			//頂点フォーマット
								D3DPOOL_MANAGED,		//固定
								&g_pVtxBuffStart,
								NULL);


	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	//頂点の設定
	pVtx[0].pos = D3DXVECTOR3(g_Startpos.x - (START_SIZE_X / 2), g_Startpos.y - (START_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
	pVtx[1].pos = D3DXVECTOR3(g_Startpos.x + (START_SIZE_X / 2), g_Startpos.y - (START_SIZE_Y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Startpos.x - (START_SIZE_X / 2), g_Startpos.y + (START_SIZE_Y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Startpos.x + (START_SIZE_X / 2), g_Startpos.y + (START_SIZE_Y / 2), 0.0f);

	pVtx[0].rhw = 1.0f;	//1.0fで固定
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = g_StartCol;
	pVtx[1].col = g_StartCol;
	pVtx[2].col = g_StartCol;
	pVtx[3].col = g_StartCol;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データをアンロック
	g_pVtxBuffStart->Unlock();
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void UninitStart(void)
{
	//テクスチャの開放
	if (g_pTextureStart != NULL)
	{
		g_pTextureStart->Release();
		g_pTextureStart = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxBuffStart != NULL)
	{
		g_pVtxBuffStart->Release();
		g_pVtxBuffStart = NULL;
	}

}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void UpdateStart(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	g_nCntStart++;

	g_StartCol.a += nInvisible;

	if (g_StartCol.a <= 0.0f|| g_StartCol.a>=1.0f)
	{
		nInvisible *= -1;
	}

	if (GetKeyTrigger(DIK_RETURN) || GetJoystickTrigger(PAD_START))	//Startで終了
	{
		nInvisible = 0.5;
	}

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	//頂点の設定
	pVtx[0].pos = D3DXVECTOR3(g_Startpos.x - (START_SIZE_X / 2), g_Startpos.y - (START_SIZE_Y / 2), 0.0f);	//ｚは0.0ｆで固定
	pVtx[1].pos = D3DXVECTOR3(g_Startpos.x + (START_SIZE_X / 2), g_Startpos.y - (START_SIZE_Y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Startpos.x - (START_SIZE_X / 2), g_Startpos.y + (START_SIZE_Y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Startpos.x + (START_SIZE_X / 2), g_Startpos.y + (START_SIZE_Y / 2), 0.0f);

	//頂点カラーの設定
	pVtx[0].col = g_StartCol;
	pVtx[1].col = g_StartCol;
	pVtx[2].col = g_StartCol;
	pVtx[3].col = g_StartCol;

	//頂点データをアンロック
	g_pVtxBuffStart->Unlock();

}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void DrawStart(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffStart, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);				//頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTextureStart);		//テクスチャの設定

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
							0,					//開始するインデックス
							2);					//プリミティブの数(ポリゴンの数)
}