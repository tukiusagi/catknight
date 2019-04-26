//=============================================================================
//
// ライフメーター処理 [lifebar.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include "lifebar.h"
#include"game.h"
#include "princess.h"
#include "sound.h"
#include"logo.h"
#include "score.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define LIFEBAR_MAX			(50.0f)	//最大ライフ
#define LIFEBAR_SIZE_Y		(2.0f)	//サイズY
#define LIFEBAR_TEXTUREMAX	(2)		//最大ライフ

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexLifebar(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// ライフバー構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;		//位置
	D3DXVECTOR3	rot;		//向き
	D3DXCOLOR	col;		//色
	D3DXMATRIX	mtxWorld;	//ワールドマトリックス
	float		fMaxLife;	//ライフの数値
}LIFEBAR;

//=============================================================================
// グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureLifebar[LIFEBAR_TEXTUREMAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLifebar = NULL;	// 頂点バッファへのポインタ

LIFEBAR					g_aLifebar;	// ライフ

//=============================================================================
// 初期化処理
//=============================================================================
void InitLifebar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

		g_aLifebar.pos = D3DXVECTOR3(-LIFEBAR_MAX * 0.5f, 60.0f, 0.0f);
		g_aLifebar.fMaxLife = LIFEBAR_MAX;
		g_aLifebar.col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								NULL,					// ファイルの名前
								&g_pTextureLifebar[0]);	// 読み込むメモリー
	
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								NULL,					// ファイルの名前
								&g_pTextureLifebar[1]);	// 読み込むメモリー

	// 頂点情報の作成
	MakeVertexLifebar(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLifebar(void)
{
	// テクスチャの開放
	for (int nCnt = 0; nCnt < LIFEBAR_TEXTUREMAX; nCnt++)
	{
		if (g_pTextureLifebar[nCnt] != NULL)
		{
			g_pTextureLifebar[nCnt]->Release();
			g_pTextureLifebar[nCnt] = NULL;
		}

	}
	
	// 頂点バッファの開放
	if (g_pVtxBuffLifebar != NULL)
	{
		g_pVtxBuffLifebar->Release();
		g_pVtxBuffLifebar = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLifebar(void)
{
	VERTEX_3D *pVtx;

	if (g_aLifebar.fMaxLife < (LIFEBAR_MAX / 3))
	{	//三分の一より少ないとき赤表示
		g_aLifebar.col= D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffLifebar->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	pVtx[4].col = g_aLifebar.col;
	pVtx[5].col = g_aLifebar.col;
	pVtx[6].col = g_aLifebar.col;
	pVtx[7].col = g_aLifebar.col;

	//頂点データをアンロック
	g_pVtxBuffLifebar->Unlock();

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLifebar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// 計算用マトリックス
	LIFEBAR *pLifebar;

	pLifebar = &g_aLifebar;

	//現在のビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 111);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&pLifebar->mtxWorld);

	//逆行列の設定
	pLifebar->mtxWorld._11 = mtxView._11;
	pLifebar->mtxWorld._12 = mtxView._21;
	pLifebar->mtxWorld._13 = mtxView._31;
	pLifebar->mtxWorld._21 = mtxView._12;
	pLifebar->mtxWorld._22 = mtxView._22;
	pLifebar->mtxWorld._23 = mtxView._32;
	pLifebar->mtxWorld._31 = mtxView._13;
	pLifebar->mtxWorld._32 = mtxView._23;
	pLifebar->mtxWorld._33 = mtxView._33;

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, pLifebar->rot.y, pLifebar->rot.x, pLifebar->rot.z);

	D3DXMatrixMultiply(&pLifebar->mtxWorld, &pLifebar->mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, pLifebar->pos.x, pLifebar->pos.y, pLifebar->pos.z);

	D3DXMatrixMultiply(&pLifebar->mtxWorld, &pLifebar->mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &pLifebar->mtxWorld);

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffLifebar, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCntBar = 0; nCntBar < LIFEBAR_TEXTUREMAX; nCntBar++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureLifebar[nCntBar]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			nCntBar * 4,		//開始するインデックス
			2);					//プリミティブの数(ポリゴンの数)
	}
	//通常モードに戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// 頂点の作成
//=============================================================================
void MakeVertexLifebar(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * LIFEBAR_TEXTUREMAX,	//確保するバッファサイズ(サイズ*頂点数)
		D3DUSAGE_WRITEONLY,		//固定
		FVF_VERTEX_3D,			//頂点フォーマット
		D3DPOOL_MANAGED,		//固定
		&g_pVtxBuffLifebar,
		NULL);

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffLifebar->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	// 頂点情報の設定
	pVtx[0].pos = D3DXVECTOR3(0, LIFEBAR_SIZE_Y, 0.0f);	//ｚは0.0ｆで固定
	pVtx[1].pos = D3DXVECTOR3(LIFEBAR_MAX, LIFEBAR_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, -LIFEBAR_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(LIFEBAR_MAX, -LIFEBAR_SIZE_Y, 0.0f);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点情報の設定
	pVtx[4].pos = D3DXVECTOR3(0, LIFEBAR_SIZE_Y, 0.0f);	//ｚは0.0ｆで固定
	pVtx[5].pos = D3DXVECTOR3(LIFEBAR_MAX, LIFEBAR_SIZE_Y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(0, -LIFEBAR_SIZE_Y, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(LIFEBAR_MAX, -LIFEBAR_SIZE_Y, 0.0f);

	pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[4].col = g_aLifebar.col;
	pVtx[5].col = g_aLifebar.col;
	pVtx[6].col = g_aLifebar.col;
	pVtx[7].col = g_aLifebar.col;

	//テクスチャ座標の設定
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データをアンロック
	g_pVtxBuffLifebar->Unlock();
}

//=============================================================================
// ライフの減少
//=============================================================================
void DecreaseLifebar(int nValue)
{
	PRINCESS *pPrincess = GetPrincess();

	if (*GetGameState() == GAMESTATE_NORMAL)
	{
		g_aLifebar.fMaxLife -= nValue;
		pPrincess->bDamege = true;

		VERTEX_3D *pVtx;

		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffLifebar->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

		// 頂点情報の設定
		pVtx[4].pos = D3DXVECTOR3(-0, LIFEBAR_SIZE_Y, 0.0f);	//ｚは0.0ｆで固定
		pVtx[5].pos = D3DXVECTOR3(g_aLifebar.fMaxLife, LIFEBAR_SIZE_Y, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(-0, -LIFEBAR_SIZE_Y, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(g_aLifebar.fMaxLife, -LIFEBAR_SIZE_Y, 0.0f);

		//頂点データをアンロック
		g_pVtxBuffLifebar->Unlock();

		if (g_aLifebar.fMaxLife <= 0 && *GetGameState() != GAMESTATE_GAMEOVER)
		{
			g_aLifebar.fMaxLife = 0;
			SetGameState(GAMESTATE_GAMEOVER);
			SetLogo(LOGO_GAMEOVER);
			StopSound(SOUND_LABEL_BGM_GAME);
			PlaySound(SOUND_LABEL_BGM_GAMEOVER);
		}
	}
}
//=============================================================================
// ライフの増加
//=============================================================================
void IncreaseLifebar(int nlife)
{
	g_aLifebar.fMaxLife += nlife;

	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffLifebar->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	// 頂点情報の設定
	pVtx[4].pos = D3DXVECTOR3(0, LIFEBAR_SIZE_Y, 0.0f);	//ｚは0.0ｆで固定
	pVtx[5].pos = D3DXVECTOR3(g_aLifebar.fMaxLife, LIFEBAR_SIZE_Y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(0, -LIFEBAR_SIZE_Y, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aLifebar.fMaxLife, -LIFEBAR_SIZE_Y, 0.0f);

	//頂点データをアンロック
	g_pVtxBuffLifebar->Unlock();
}

//=============================================================================
// ライフの増加
//=============================================================================
float GetLifeber(void)
{
	return g_aLifebar.fMaxLife;
}