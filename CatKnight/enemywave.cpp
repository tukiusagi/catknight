//=============================================================================
//
// 敵波処理 [enemywave.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include "enemywave.h"
#include"game.h"
#include "enemy.h"
#include "meshfield.h"
#include "boss.h"
#include "obstacle.h"
#include "sound.h"
#include"logo.h"
#include "princess.h"
#include "EwaveUI.h"
#include "fade.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SECOND				(60)	//一秒
#define MAP_WIDTH			(7)		//x分割数
#define MAP_HEIGHT			(7)		//ｙ分割数
#define MAP_FILENAME1		("data/wave1.txt")	//wave 1 の敵分布ファイル
#define MAP_FILENAME2		("data/wave2.txt")	//wave 2
#define MAP_FILENAME3		("data/wave3.txt")	//wave 3
#define MAP_FILENAME4		("data/wave4.txt")	//wave 4
#define MAP_FILENAME5		("data/wave5.txt")	//wave 5
#define MAP_FILENAME6		("data/wave6.txt")	//wave 5
#define MAX_CHARA			(256)				//読み込む文字の最大数
#define MAP_POS				(300.0f)			//敵の基準位置
#define MAP_INTERVAL		(100.0f)			//間隔(フィールドサイズ/分割数)

//=============================================================================
// プロトタイプ宣言
//=============================================================================

//=============================================================================
// グローバル変数宣言
//=============================================================================
int	g_nEwave;			// 敵波
int nBreakTime = 0;		//休息時間
bool bBreak;

int nData[MAP_HEIGHT][MAP_WIDTH] = {};
char aMapFilename[E_WAVE_MAX][MAX_CHARA] = { MAP_FILENAME1,MAP_FILENAME2,MAP_FILENAME3,MAP_FILENAME4,MAP_FILENAME5,MAP_FILENAME6 };

//=============================================================================
// 初期化処理
//=============================================================================
void InitEwave(void)
{
	g_nEwave = 1;
	nBreakTime = 0;
	bBreak = false;
	LoadMap(0);
	SetMap();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEwave(void)
{	
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEwave(void)
{
	if (*GetMode() == MODE_GAME)
	{
		EwaveUI *pEwaveUI = GetEwaveUI();
		int pNumEnemy = *GetNumEnemy();
		BOSS *pBoss = GetBoss();
		int NumBoss = GetBossNum();

		if (bBreak)
		{
			nBreakTime++;
		}

		if (pNumEnemy <= 0 && NumBoss <= 0 && !bBreak)
		{	//全滅させたらwaveクリア
			nBreakTime = 0;
			bBreak = true;
		}

		PRINCESS *pPrincess = GetPrincess();
		if (g_nEwave >= E_WAVE_MAX)
		{	//最終waveまで終わった
			if (nBreakTime == 1 && *GetGameState()==GAMESTATE_NORMAL)
			{	//クリア
				SetGameState(GAMESTATE_CLEAR);
				SetLogo(LOGO_CLEAR);
				StopSound(SOUND_LABEL_BGM_GAME);
				PlaySound(SOUND_LABEL_BGM_CLEAR);
			}
		}
		else
		{
			if (nBreakTime == 1)
			{	//喜ぶ
				pPrincess->state = PRINCESSSTATE_WIN;
				pPrincess->nCntFrame = 0;
				pPrincess->nCntKeyPrincess = 0;
				PlaySound(SOUND_LABEL_SE_CAT);
				pEwaveUI->bUse[1] = true;	//クリアロゴ表示
			}
			if (nBreakTime == SECOND * 3)
			{
				//元に戻る
				pEwaveUI->bUse[1] = false;	//クリアロゴ消し
				SetFade();
				pPrincess->state = PRINCESSSTATE_NEUTRAL;
				pPrincess->nCntFrame = 0;
				pPrincess->nCntKeyPrincess = 0;
				PlaySound(SOUND_LABEL_SE_CAT);
				pEwaveUI->bUse[2] = true;	//次waveロゴ表示
			}
		}
		if (nBreakTime == SECOND * 5)
		{
			pEwaveUI->bUse[2] = false;	//ロゴ消し
			//いろいろ初期化
			bBreak = false;
		}
	}
}

//=============================================================================
// 敵波の変更
//=============================================================================
void AddEwave(int nValue)
{
	g_nEwave += nValue;

	DeleteALLObstacle();	//障害物削除

	//敵削除
	DeleteEnemey();
	DeleteBoss();

	LoadMap(g_nEwave - 1);
	SetMap();

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{	//障害物設置
		SetObstacle(D3DXVECTOR3(rand() % 220 + 80,
			0.0f,
			rand() % (int)MESHFIELD_SIZE * MESHFIELD_D * 0.9f - (int)MESHFIELD_SIZE * MESHFIELD_D * 0.4f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		SetObstacle(D3DXVECTOR3(-rand() % 220 - 80,
			0.0f,
			rand() % (int)MESHFIELD_SIZE * MESHFIELD_D*0.9f - (int)MESHFIELD_SIZE * MESHFIELD_D * 0.4f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	SetPrincess();
	SetPlayer();
}

//=============================================================================
// wave毎の配置
//=============================================================================
void SetMap(void)
{
	for (int nCntZ = 0; nCntZ < MAP_HEIGHT; nCntZ++)
	{
		for (int nCntX = 0; nCntX < MAP_WIDTH; nCntX++)
		{
			if (nData[nCntZ][nCntX] == 1)
			{
				SetEnemy(D3DXVECTOR3(-MAP_POS + (nCntX * MAP_INTERVAL), 0, MAP_POS - (nCntZ * MAP_INTERVAL)));
			}
			if (nData[nCntZ][nCntX] == 2)
			{
				SetBoss(D3DXVECTOR3(-MAP_POS + (nCntX * MAP_INTERVAL), 0, MAP_POS - (nCntZ * MAP_INTERVAL)));
			}
		}
	}
}

//=============================================================================
// wave数の取得
//=============================================================================
int  *GetEwave(void)
{
	return &g_nEwave;
}


//=============================================================================
// マップのロード
//=============================================================================
void LoadMap(int nWave)
{
	FILE *pFile;

	pFile = fopen(aMapFilename[nWave], "r");

	if (pFile != NULL)
	{
		for (int nCntZ = 0; nCntZ < MAP_HEIGHT; nCntZ++)
		{
			for (int nCntX = 0; nCntX < MAP_WIDTH; nCntX++)
			{
				fscanf(pFile, "%d", &nData[nCntZ][nCntX]);
			}
		}
	}
}