//=============================================================================
//
// �G�g���� [enemywave.cpp]
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
// �}�N����`
//=============================================================================
#define SECOND				(60)	//��b
#define MAP_WIDTH			(7)		//x������
#define MAP_HEIGHT			(7)		//��������
#define MAP_FILENAME1		("data/wave1.txt")	//wave 1 �̓G���z�t�@�C��
#define MAP_FILENAME2		("data/wave2.txt")	//wave 2
#define MAP_FILENAME3		("data/wave3.txt")	//wave 3
#define MAP_FILENAME4		("data/wave4.txt")	//wave 4
#define MAP_FILENAME5		("data/wave5.txt")	//wave 5
#define MAP_FILENAME6		("data/wave6.txt")	//wave 5
#define MAX_CHARA			(256)				//�ǂݍ��ޕ����̍ő吔
#define MAP_POS				(300.0f)			//�G�̊�ʒu
#define MAP_INTERVAL		(100.0f)			//�Ԋu(�t�B�[���h�T�C�Y/������)

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================

//=============================================================================
// �O���[�o���ϐ��錾
//=============================================================================
int	g_nEwave;			// �G�g
int nBreakTime = 0;		//�x������
bool bBreak;

int nData[MAP_HEIGHT][MAP_WIDTH] = {};
char aMapFilename[E_WAVE_MAX][MAX_CHARA] = { MAP_FILENAME1,MAP_FILENAME2,MAP_FILENAME3,MAP_FILENAME4,MAP_FILENAME5,MAP_FILENAME6 };

//=============================================================================
// ����������
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
// �I������
//=============================================================================
void UninitEwave(void)
{	
}

//=============================================================================
// �X�V����
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
		{	//�S�ł�������wave�N���A
			nBreakTime = 0;
			bBreak = true;
		}

		PRINCESS *pPrincess = GetPrincess();
		if (g_nEwave >= E_WAVE_MAX)
		{	//�ŏIwave�܂ŏI�����
			if (nBreakTime == 1 && *GetGameState()==GAMESTATE_NORMAL)
			{	//�N���A
				SetGameState(GAMESTATE_CLEAR);
				SetLogo(LOGO_CLEAR);
				StopSound(SOUND_LABEL_BGM_GAME);
				PlaySound(SOUND_LABEL_BGM_CLEAR);
			}
		}
		else
		{
			if (nBreakTime == 1)
			{	//���
				pPrincess->state = PRINCESSSTATE_WIN;
				pPrincess->nCntFrame = 0;
				pPrincess->nCntKeyPrincess = 0;
				PlaySound(SOUND_LABEL_SE_CAT);
				pEwaveUI->bUse[1] = true;	//�N���A���S�\��
			}
			if (nBreakTime == SECOND * 3)
			{
				//���ɖ߂�
				pEwaveUI->bUse[1] = false;	//�N���A���S����
				SetFade();
				pPrincess->state = PRINCESSSTATE_NEUTRAL;
				pPrincess->nCntFrame = 0;
				pPrincess->nCntKeyPrincess = 0;
				PlaySound(SOUND_LABEL_SE_CAT);
				pEwaveUI->bUse[2] = true;	//��wave���S�\��
			}
		}
		if (nBreakTime == SECOND * 5)
		{
			pEwaveUI->bUse[2] = false;	//���S����
			//���낢�돉����
			bBreak = false;
		}
	}
}

//=============================================================================
// �G�g�̕ύX
//=============================================================================
void AddEwave(int nValue)
{
	g_nEwave += nValue;

	DeleteALLObstacle();	//��Q���폜

	//�G�폜
	DeleteEnemey();
	DeleteBoss();

	LoadMap(g_nEwave - 1);
	SetMap();

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{	//��Q���ݒu
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
// wave���̔z�u
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
// wave���̎擾
//=============================================================================
int  *GetEwave(void)
{
	return &g_nEwave;
}


//=============================================================================
// �}�b�v�̃��[�h
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