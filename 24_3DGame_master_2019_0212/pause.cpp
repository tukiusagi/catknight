//=============================================================================
//
// ポーズ処理[pause.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"pause.h"	//インクルードファイル
#include "input.h"	
#include "joystick.h"
#include "fade.h"
#include"game.h"
#include "sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define PAUSE_CONTINUE	"data/TEXTURE/戻る.png"	//読み込むテクスチャ名
#define PAUSE_RETRY	"data/TEXTURE/はじめから.png"	
#define PAUSE_QUIT	"data/TEXTURE/タイトル.png"	

#define PAUSE_POS_X	(0)			//ポリゴン位置X
#define PAUSE_POS_Y	(0)			//ポリゴン位置Y
#define PAUSE_SIZE_X	(600)	//ポリゴンサイズX
#define PAUSE_SIZE_Y	(200)	//ポリゴンサイズY
#define PAUSE_TEXTURE	(3)			//ポリゴン位置X

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTexturePause[PAUSE_TEXTURE] = {};			//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;	//頂点バッファへのポインタ
PAUSESTATE			g_PauseState;
PAUSE_COLORSTATE	g_PauseColor;
D3DXVECTOR3			g_posPause;	//背景位置
int g_nSelectPause;

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();	//デバイスの取得

	g_posPause = D3DXVECTOR3(640, 200, 0);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		PAUSE_CONTINUE,
							&g_pTexturePause[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		PAUSE_RETRY,
		&g_pTexturePause[1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		PAUSE_QUIT,
		&g_pTexturePause[2]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSE_TEXTURE,	//確保するバッファサイズ(サイズ*頂点数)
									D3DUSAGE_WRITEONLY,		//固定
									FVF_VERTEX_2D,			//頂点フォーマット
									D3DPOOL_MANAGED,		//固定
									&g_pVtxBuffPause,
									NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE; nCntPause++)
	{
		//頂点の設定
		pVtx[0].pos = D3DXVECTOR3(g_posPause.x - (PAUSE_SIZE_X / 2), g_posPause.y - (PAUSE_SIZE_Y / 2) + (150.0f*nCntPause), 0.0f);	//ｚは0.0ｆで固定
		pVtx[1].pos = D3DXVECTOR3(g_posPause.x + (PAUSE_SIZE_X / 2), g_posPause.y - (PAUSE_SIZE_Y / 2) + (150.0f*nCntPause), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posPause.x - (PAUSE_SIZE_X / 2), g_posPause.y + (PAUSE_SIZE_Y / 2) + (150.0f*nCntPause), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posPause.x + (PAUSE_SIZE_X / 2), g_posPause.y + (PAUSE_SIZE_Y / 2) + (150.0f*nCntPause), 0.0f);

		pVtx[0].rhw = 1.0f;	//1.0fで固定
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点データをアンロック
	g_pVtxBuffPause->Unlock();

	g_posPause = D3DXVECTOR3(0, 0, 0);
	g_PauseColor = PAUSE_COLORSTATE_NONE;
	g_PauseState = PAUSESTATE_NONE;
	g_nSelectPause = 0;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void UninitPause(void)
{
	for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE; nCntPause++)
	{
		//テクスチャの開放
		if (g_pTexturePause[nCntPause] != NULL)
		{				  
			g_pTexturePause[nCntPause]->Release();
			g_pTexturePause[nCntPause] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void UpdatePause(void)
{
	if ((GetKeyTrigger(DIK_UP) || GetJoystickTrigger(PAD_UP_L)))
	{
		g_nSelectPause = (g_nSelectPause + 2) % 3;
		PlaySound(SOUND_LABEL_SE_PAUSE);

	}
	if ((GetKeyTrigger(DIK_DOWN) || GetJoystickTrigger(PAD_DOWN_L)))
	{
		g_nSelectPause = (g_nSelectPause + 1) % 3;
		PlaySound(SOUND_LABEL_SE_PAUSE);
	}

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);	//いじるときはロックをかけてから！

	switch (g_nSelectPause)
	{
	case 0:
		g_PauseColor = PAUSE_COLORSTATE_CONTINUE;
		if ((GetKeyTrigger(DIK_RETURN) || GetJoystickTrigger(PAD_B)))
		{
			g_PauseState = PAUSESTATE_CONTINUE;
			PlaySound(SOUND_LABEL_SE_ENTER);
		}

		//コンティニュー以外は半透明に
		//for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE; nCntPause++)
		//{
		//	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		//	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		//	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		//	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 128);

		//	pVtx += 4;
		//}
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 128);

		pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 128);

		break;
	case 1:
		g_PauseColor = PAUSE_COLORSTATE_RETRY;
		if ((GetKeyTrigger(DIK_RETURN) || GetJoystickTrigger(PAD_B)))
		{
			g_PauseState = PAUSESTATE_RETRY;
			PlaySound(SOUND_LABEL_SE_ENTER);
		}

		//for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE; nCntPause++)
		//{
		//	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		//	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		//	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		//	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		//	pVtx += 4;

		//}

		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 128);

		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 128);

		break;
	case 2:
		g_PauseColor = PAUSE_COLORSTATE_QUIT;
		if ((GetKeyTrigger(DIK_RETURN) || GetJoystickTrigger(PAD_B)))
		{
			g_PauseState = PAUSESTATE_QUIT;
			PlaySound(SOUND_LABEL_SE_ENTER);
		}
		//for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE; nCntPause++)
		//{
		//	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		//	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		//	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		//	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		//	pVtx += 4;
		//}

		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 128);

		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 128);

		pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 255);


		break;
	default:
		break;
	}
	//頂点データをアンロック
	g_pVtxBuffPause->Unlock();

	switch (g_PauseState)
	{
	case PAUSESTATE_CONTINUE:
		if (*GetFade() == FADE_NONE)
		{
			SetGameState(GAMESTATE_NORMAL);
			g_PauseState = PAUSESTATE_NONE;
		}
		break;
	case PAUSESTATE_RETRY:
		if (*GetFade() == FADE_NONE)
		{
			SetFade(MODE_GAME);
			g_PauseState = PAUSESTATE_NONE;
		}
		break;
	case PAUSESTATE_QUIT:
		if (*GetFade() == FADE_NONE)
		{
			SetFade(MODE_TITLE);
			StopSound(SOUND_LABEL_BGM_GAME);
			g_PauseState = PAUSESTATE_NONE;
		}
		break;

	default:
		break;
	}
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);			//頂点フォーマットの設定

	for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE; nCntPause++)
	{
		pDevice->SetTexture(0, g_pTexturePause[nCntPause]);	//テクスチャの設定

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			nCntPause * 4,				//開始するインデックス
			2);				//プリミティブの数(ポリゴンの数)
	}
}