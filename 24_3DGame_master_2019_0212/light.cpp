//=============================================================================
//
// ライト処理 [light.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "light.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define NUM_LIGHT	(3)		//ライトの数

//=============================================================================
// グローバル変数
//=============================================================================
D3DLIGHT9 g_alight[NUM_LIGHT];

//=============================================================================
// ライトの初期化処理
//=============================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXVECTOR3 avecDir[NUM_LIGHT];				//設定用方向ベクトル

	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		// ライトをクリアする
		ZeroMemory(&g_alight[nCntLight], sizeof(D3DLIGHT9));
		// ライトの種類を設定
		g_alight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;
	}

	// ライトの拡散光を設定
	g_alight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_alight[0].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_alight[1].Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	g_alight[1].Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	g_alight[2].Diffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	g_alight[2].Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

	// ライトの方向の設定
	avecDir[0] = D3DXVECTOR3(0.20f, -0.80f, 0.40f);
	avecDir[1] = D3DXVECTOR3(-0.20f, 0.80f, -0.40f);
	avecDir[2] = D3DXVECTOR3(0.0f, -0.10f, -1.0f);

	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		D3DXVec3Normalize(&avecDir[nCntLight], &avecDir[nCntLight]);	//正規化する
		g_alight[nCntLight].Direction = avecDir[nCntLight];

		// ライトを設定する
		pDevice->SetLight(nCntLight, &g_alight[nCntLight]);

		// ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//=============================================================================
// ライトの終了処理
//=============================================================================
void UninitLight(void)
{
}

//=============================================================================
// ライトの更新処理
//=============================================================================
void UpdateLight(void)
{
}

