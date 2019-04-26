//=============================================================================
//
// 魔法処理 [magic.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "magic.h"
#include"game.h"
#include "wave.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAGIC_SIZE		(30.0f)		//魔法陣の大きさ
#define MAX_MAGIC		(10)		//表示最大数

#define MAGIC_TEXTURE		"data/TEXTURE/魔法陣.png"	//読み込むテクスチャ名

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexMagic(LPDIRECT3DDEVICE9 pDevice);


//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffMagic = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureMagic = NULL;	//テクスチャのポインタ

MAGIC g_aMagic;	//魔法陣の情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitMagic(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 位置・回転の初期設定
		g_aMagic.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMagic.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMagic.col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		g_aMagic.fSize = MAGIC_SIZE;
		g_aMagic.bUse = false;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
							MAGIC_TEXTURE,
							&g_pTextureMagic);

	// 頂点情報の作成
	MakeVertexMagic(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMagic(void)
{
	//テクスチャの開放
	if (g_pTextureMagic != NULL)
	{
		g_pTextureMagic->Release();
		g_pTextureMagic = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxBuffMagic != NULL)
	{
		g_pVtxBuffMagic->Release();
		g_pVtxBuffMagic = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMagic(void)
{
	if (*GetGameState() == GAMESTATE_GAMEOVER)
	{
		g_aMagic.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		VERTEX_3D *pVtx;

		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffMagic->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

		pVtx[0].col = g_aMagic.col;
		pVtx[1].col = g_aMagic.col;
		pVtx[2].col = g_aMagic.col;
		pVtx[3].col = g_aMagic.col;

		//頂点データをアンロック
		g_pVtxBuffMagic->Unlock();
	}
	else
	{
		WAVE *pWave;
		pWave = GetWave();

		switch (pWave->state)
		{
		case WAVE_STATE_CHARGE:
			g_aMagic.fSize = MAGIC_SIZE;
			g_aMagic.col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
			break;
		case WAVE_STATE_CHARGEMAX:
			g_aMagic.col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			break;
		case WAVE_STATE_ATTACK:
			g_aMagic.col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
			g_aMagic.fSize += 3.3f;
			break;
		default:
			break;
		}

		VERTEX_3D *pVtx;

		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffMagic->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

		pVtx[0].pos = D3DXVECTOR3(-g_aMagic.fSize, 0.0f, g_aMagic.fSize);
		pVtx[1].pos = D3DXVECTOR3(g_aMagic.fSize, 0.0f, g_aMagic.fSize);
		pVtx[2].pos = D3DXVECTOR3(-g_aMagic.fSize, 0.0f, -g_aMagic.fSize);
		pVtx[3].pos = D3DXVECTOR3(g_aMagic.fSize, 0.0f, -g_aMagic.fSize);

		pVtx[0].col = g_aMagic.col;
		pVtx[1].col = g_aMagic.col;
		pVtx[2].col = g_aMagic.col;
		pVtx[3].col = g_aMagic.col;

		//頂点データをアンロック
		g_pVtxBuffMagic->Unlock();

		g_aMagic.rot.y += 0.01f;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMagic(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	if (*GetGameState() != GAMESTATE_GAMEOVER)
	{
		// レンダーステート(加算合成処理)
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	if (g_aMagic.bUse)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aMagic.mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMagic.rot.y, g_aMagic.rot.x, g_aMagic.rot.z);

		D3DXMatrixMultiply(&g_aMagic.mtxWorld, &g_aMagic.mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aMagic.pos.x, g_aMagic.pos.y, g_aMagic.pos.z);

		D3DXMatrixMultiply(&g_aMagic.mtxWorld, &g_aMagic.mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aMagic.mtxWorld);

		//頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pVtxBuffMagic, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureMagic);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			0,		//開始するインデックス
			2);					//プリミティブの数(ポリゴンの数)
	}

	//通常モードに戻す
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	if (*GetGameState() != GAMESTATE_GAMEOVER)
	{
		// レンダーステート(通常ブレンド処理)
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexMagic(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,	//確保するバッファサイズ(サイズ*頂点数)
		D3DUSAGE_WRITEONLY,		//固定
		FVF_VERTEX_3D,			//頂点フォーマット
		D3DPOOL_MANAGED,		//固定
		&g_pVtxBuffMagic,
		NULL);

	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffMagic->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

		//頂点の設定
	pVtx[0].pos = D3DXVECTOR3(-g_aMagic.fSize, 0.0f, g_aMagic.fSize);
	pVtx[1].pos = D3DXVECTOR3(g_aMagic.fSize, 0.0f, g_aMagic.fSize);
	pVtx[2].pos = D3DXVECTOR3(-g_aMagic.fSize, 0.0f, -g_aMagic.fSize);
	pVtx[3].pos = D3DXVECTOR3(g_aMagic.fSize, 0.0f, -g_aMagic.fSize);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0].col = g_aMagic.col;
	pVtx[1].col = g_aMagic.col;
	pVtx[2].col = g_aMagic.col;
	pVtx[3].col = g_aMagic.col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データをアンロック
	g_pVtxBuffMagic->Unlock();
}

//=============================================================================
// 位置を設定
//=============================================================================
void SetMagic(D3DXVECTOR3 pos)
{
	if (!g_aMagic.bUse)
	{
		VERTEX_3D *pVtx;

		g_aMagic.pos.x = pos.x;
		g_aMagic.pos.z = pos.z;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffMagic->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点情報の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aMagic.fSize, 0.0f, g_aMagic.fSize);
		pVtx[1].pos = D3DXVECTOR3(g_aMagic.fSize, 0.0f, g_aMagic.fSize);
		pVtx[2].pos = D3DXVECTOR3(-g_aMagic.fSize, 0.0f, -g_aMagic.fSize);
		pVtx[3].pos = D3DXVECTOR3(g_aMagic.fSize, 0.0f, -g_aMagic.fSize);

		// 頂点データをアンロックする
		g_pVtxBuffMagic->Unlock();

		g_aMagic.bUse = true;
	}
}

//=============================================================================
// 情報の取得
//=============================================================================
MAGIC *GetMagic(void)
{
	return &g_aMagic;
}