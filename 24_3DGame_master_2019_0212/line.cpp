//=============================================================================
//
// ライン処理 [line.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "line.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define LINE_TEXTURE	"data/TEXTURE/tree000.png"	//読み込むテクスチャ名
#define LINE_SIZE		(30.0f)	//サイズ
#define LINE_MAX		(100)	//表示最大数

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexLine(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// ポリゴンの構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;		//位置
	D3DXVECTOR3	rot;		//向き
	D3DXVECTOR3	start;		//始点
	D3DXVECTOR3	end;		//終点
	D3DXMATRIX	mtxWorld;	//ワールドマトリックス
	D3DXCOLOR	col;		//色
	bool		bUse;		//使用状態
}LINE;

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffLine = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureLine = NULL;	//テクスチャのポインタ

LINE g_aline[LINE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
void InitLine(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 位置・回転の初期設定
	for (int nCntLine = 0; nCntLine < LINE_MAX; nCntLine++)
	{
		g_aline[nCntLine].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aline[nCntLine].start = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aline[nCntLine].end = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aline[nCntLine].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aline[nCntLine].col= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aline[nCntLine].bUse = false;
	}

	//テクスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice,
	//						LINE_TEXTURE,
	//						&g_pTextureLine);

	// 頂点情報の作成
	MakeVertexLine(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLine(void)
{
	//テクスチャの開放
	//if (g_pTextureLine != NULL)
	//{			  
	//	g_pTextureLine->Release();
	//	g_pTextureLine = NULL;
	//}

	//頂点バッファの開放
	if (g_pVtxBuffLine != NULL)
	{			  
		g_pVtxBuffLine->Release();
		g_pVtxBuffLine = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLine(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLine(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス
	LINE *pLine;

	pLine = &g_aline[0];

	for (int nCntLine = 0; nCntLine < LINE_MAX; nCntLine++, pLine++)
	{
		if (pLine->bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pLine->mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pLine->rot.y, pLine->rot.x, pLine->rot.z);

			D3DXMatrixMultiply(&pLine->mtxWorld, &pLine->mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pLine->pos.x, pLine->pos.y, pLine->pos.z);

			D3DXMatrixMultiply(&pLine->mtxWorld, &pLine->mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pLine->mtxWorld);

			//頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pVtxBuffLine, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			//pDevice->SetTexture(0, g_pTextureLine);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_LINELIST,	//プリミティブの種類
									nCntLine * 2,	//開始するインデックス
									2);				//プリミティブの数(ポリゴンの数)
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexLine(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 2 * LINE_MAX,	//確保するバッファサイズ(サイズ*頂点数)
								D3DUSAGE_WRITEONLY,		//固定
								FVF_VERTEX_3D,			//頂点フォーマット
								D3DPOOL_MANAGED,		//固定
								&g_pVtxBuffLine,
								NULL);

	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffLine->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	for (int nCntLine = 0; nCntLine < LINE_MAX; nCntLine++)
	{
		//頂点の設定
		pVtx[0].pos = D3DXVECTOR3(g_aline[nCntLine].start.x, g_aline[nCntLine].start.y, g_aline[nCntLine].start.z);
		pVtx[1].pos = D3DXVECTOR3(g_aline[nCntLine].end.x, g_aline[nCntLine].end.y, g_aline[nCntLine].end.z);

		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[0].col = g_aline[nCntLine].col;
		pVtx[1].col = g_aline[nCntLine].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 2;	//頂点データのポインタを2つ分進める

	}
	//頂点データをアンロック
	g_pVtxBuffLine->Unlock();
}

//=============================================================================
// ラインの作成
//=============================================================================
void SetLine(D3DXVECTOR3 pos,D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR col)
{
	VERTEX_3D *pVtx;

	for (int nCntLine = 0; nCntLine < LINE_MAX; nCntLine++)
	{
		if (!g_aline[nCntLine].bUse)
		{
			g_aline[nCntLine].pos = pos;
			g_aline[nCntLine].start = start;
			g_aline[nCntLine].end = end;
			g_aline[nCntLine].col = col;
			//頂点データの範囲をロックし、頂点バッファへのポインタを取得
			g_pVtxBuffLine->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

			pVtx += nCntLine * 2;	//頂点データのポインタを2つ分進める

			pVtx[0].pos = D3DXVECTOR3(g_aline[nCntLine].start.x, g_aline[nCntLine].start.y, g_aline[nCntLine].start.z);
			pVtx[1].pos = D3DXVECTOR3(g_aline[nCntLine].end.x, g_aline[nCntLine].end.y, g_aline[nCntLine].end.z);

			pVtx[0].col = g_aline[nCntLine].col;
			pVtx[1].col = g_aline[nCntLine].col;

			//頂点データをアンロック
			g_pVtxBuffLine->Unlock();

			g_aline[nCntLine].bUse = true;
			break;
		}
	}
}
//=============================================================================
// ラインの削除
//=============================================================================
void DeleteLine(void)
{
	for (int nCntLine = 0; nCntLine < LINE_MAX; nCntLine++)
	{
		if (g_aline[nCntLine].bUse)
		{
			g_aline[nCntLine].bUse = false;
		}
	}
}
