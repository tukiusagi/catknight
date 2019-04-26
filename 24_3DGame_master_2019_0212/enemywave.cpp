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

//=============================================================================
// �}�N����`
//=============================================================================
#define	TEXTURE_E_WAVE			"data/TEXTURE/number0.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_E_WAVE_PIC		"data/TEXTURE/wave.png"	
#define	TEXTURE_E_WAVE_PIC02	"data/TEXTURE/�ȂȂ�.png"
#define	TEXTURE_E_WAVE_CLEAR	"data/TEXTURE/waveClear2.png"	
#define	TEXTURE_E_WAVE_START	"data/TEXTURE/nextwave.png"	

#define DIGIT_E_WAVE		(2)			//�G�g�̌���
#define E_WAVE_SIZE_X		(35.0f)		//�����T�C�YX
#define E_WAVE_SIZE_Y		(70.0f)		//�����T�C�YY
#define E_WAVEPIC_SIZE_X	(105.0f)	//�T�C�YX
#define E_WAVEPIC_SIZE_Y	(70.0f)		//�T�C�YY
#define E_WAVEINFO_SIZE_X	(675.0f)	//�T�C�YX
#define E_WAVEINFO_SIZE_Y	(150.0f)		//�T�C�YY

#define SECOND				(60)		//��b
#define E_WAVE_TEXTURE_MAX	(5)			//�e�N�X�`����
#define E_WAVE_TEXTURE		(4)			//�����ȊO�̃e�N�X�`����

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexEwave(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureEwave[E_WAVE_TEXTURE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEwave = NULL;			// ���_�o�b�t�@�ւ̃|�C���^

int						g_nEwave[DIGIT_E_WAVE];			// �G�g
D3DXVECTOR3				g_aEwavepos[DIGIT_E_WAVE];		// �����ʒu
D3DXVECTOR3				g_WavePicpos[E_WAVE_TEXTURE];	//�摜�ʒu
int nCounter = 0;		//���ԑ���p
int g_nSmallWave = 0;	//������wave��
int nBreakTime = 0;		//�x������
bool bUse[E_WAVE_TEXTURE];			//�g�p���
bool bResult;

//=============================================================================
// ����������
//=============================================================================
void InitEwave(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEwave;

	g_nEwave[0] = 1;
	g_nEwave[1] = E_WAVE_MAX;
	nBreakTime = 0;

	for (nCntEwave = 0; nCntEwave < DIGIT_E_WAVE; nCntEwave++)
	{
		g_aEwavepos[nCntEwave] = D3DXVECTOR3(SCREEN_WIDTH - ((DIGIT_E_WAVE+1) * E_WAVE_SIZE_X) + (nCntEwave * (E_WAVE_SIZE_X*2)), 180.0f, 0.0f);
	}

	g_WavePicpos[0] = D3DXVECTOR3(SCREEN_WIDTH - ((DIGIT_E_WAVE + 3) * E_WAVE_SIZE_X), 180.0f, 0.0f);
	g_WavePicpos[3] = D3DXVECTOR3(SCREEN_WIDTH - E_WAVE_SIZE_X * DIGIT_E_WAVE, 180.0f, 0.0f);
	g_WavePicpos[1] = D3DXVECTOR3(640.0f, 350.0f, 0.0f);
	g_WavePicpos[2] = D3DXVECTOR3(640.0f, 350.0f, 0.0f);
	bUse[0] = true;
	bUse[1] = false;
	bUse[2] = false;
	bUse[3] = true;
	bResult = true;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_E_WAVE,					// �t�@�C���̖��O
		&g_pTextureEwave[0]);			// �ǂݍ��ރ������[
	
	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_E_WAVE_PIC,				// �t�@�C���̖��O
		&g_pTextureEwave[1]);			// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_E_WAVE_CLEAR,			// �t�@�C���̖��O
		&g_pTextureEwave[2]);			// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_E_WAVE_START,			// �t�@�C���̖��O
		&g_pTextureEwave[3]);			// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_E_WAVE_PIC02,			// �t�@�C���̖��O
		&g_pTextureEwave[4]);			// �ǂݍ��ރ������[

	// ���_���̍쐬
	MakeVertexEwave(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitEwave(void)
{
	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < E_WAVE_TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureEwave[nCnt] != NULL)
		{				  
			g_pTextureEwave[nCnt]->Release();
			g_pTextureEwave[nCnt] = NULL;
		}
	}
	
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffEwave != NULL)
	{
		g_pVtxBuffEwave->Release();
		g_pVtxBuffEwave = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEwave(void)
{
	if (*GetMode() == MODE_GAME)
	{
		nCounter++;
		int pNumEnemy = *GetNumEnemy();
		BOSS *pBoss = GetBoss();
		int NumBoss = GetBossUse();

		if (g_nEwave[0] >= E_WAVE_MAX)
		{
			if (NumBoss <= 0 && pNumEnemy <= 0 && nBreakTime <= 0)
			{
				SetGameState(GAMESTATE_CLEAR);
				SetLogo(LOGO_CLEAR);
				StopSound(SOUND_LABEL_BGM_GAME);
				PlaySound(SOUND_LABEL_BGM_CLEAR);
				nBreakTime++;
			}
		}
		else
		{
			if ((nCounter % (SECOND * 10) == 0 || pNumEnemy <= 2) && g_nSmallWave < g_nEwave[0] - 1)
			{
				g_nSmallWave++;

				for (int nCntEnemy = 0; nCntEnemy < 2; nCntEnemy++)
				{
					SetEnemy(D3DXVECTOR3(rand() % MESHFIELD_W * 0.5f + MESHFIELD_SIZE * 2,
						0.0f,
						rand() % (int)MESHFIELD_SIZE * MESHFIELD_D - (int)MESHFIELD_SIZE * MESHFIELD_D * 0.5f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);

					SetEnemy(D3DXVECTOR3(-rand() % MESHFIELD_W * 0.5f - MESHFIELD_SIZE * 2,
						0.0f,
						rand() % (int)MESHFIELD_SIZE * MESHFIELD_D - (int)MESHFIELD_SIZE * MESHFIELD_D * 0.5f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
				}
			}

			if (pNumEnemy <= 0 && NumBoss <= 0 && g_nSmallWave >= g_nEwave[0] - 1)
			{
				PRINCESS *pPrincess = GetPrincess();

				if (nBreakTime < 1)
				{
					pPrincess->state = PRINCESSSTATE_WIN;
					pPrincess->nCntFrame = 0;
					pPrincess->nCntKeyPrincess = 0;
					PlaySound(SOUND_LABEL_SE_CAT);
				}
				bUse[1] = true;	//�N���A�����o��

				nBreakTime++;	//�C���^�[�o��
				if (nBreakTime > SECOND * 3)
				{
					bUse[1] = false;
					if (nBreakTime <= (SECOND * 3) + 1)
					{
						pPrincess->state = PRINCESSSTATE_NEUTRAL;
						pPrincess->nCntFrame = 0;
						pPrincess->nCntKeyPrincess = 0;
						PlaySound(SOUND_LABEL_SE_CAT);
					}
					bUse[2] = true;	//nextwave�����o��
				}
				if (nBreakTime > SECOND * 5)
				{
					bUse[2] = false;
					AddEwave(1);	//wave���̑���
					nCounter = 0;
					g_nSmallWave = 0;
					nBreakTime = 0;
				}
			}
		}
	}
	else if (*GetMode() == MODE_RESULT)
	{
		for (int nCntEwave = 0; nCntEwave < DIGIT_E_WAVE; nCntEwave++)
		{
			g_aEwavepos[nCntEwave] = D3DXVECTOR3((SCREEN_WIDTH*0.7f) - ((DIGIT_E_WAVE + 1) * E_WAVE_SIZE_X) + (nCntEwave * (E_WAVE_SIZE_X * 2)), 420.0f, 0.0f);
		}
		g_WavePicpos[0] = D3DXVECTOR3((SCREEN_WIDTH*0.7f) - ((DIGIT_E_WAVE + 3) * E_WAVE_SIZE_X), 420.0f, 0.0f);
		g_WavePicpos[3] = D3DXVECTOR3((SCREEN_WIDTH*0.7f) - E_WAVE_SIZE_X * DIGIT_E_WAVE, 420.0f, 0.0f);

		VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

		//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffEwave->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

		for (int nCntEwave = 0; nCntEwave < DIGIT_E_WAVE; nCntEwave++)
		{	//����
			// ���_���̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aEwavepos[nCntEwave].x - (E_WAVE_SIZE_X / 2), g_aEwavepos[nCntEwave].y - (E_WAVE_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
			pVtx[1].pos = D3DXVECTOR3(g_aEwavepos[nCntEwave].x + (E_WAVE_SIZE_X / 2), g_aEwavepos[nCntEwave].y - (E_WAVE_SIZE_Y / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEwavepos[nCntEwave].x - (E_WAVE_SIZE_X / 2), g_aEwavepos[nCntEwave].y + (E_WAVE_SIZE_Y / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEwavepos[nCntEwave].x + (E_WAVE_SIZE_X / 2), g_aEwavepos[nCntEwave].y + (E_WAVE_SIZE_Y / 2), 0.0f);

			pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
		}

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			// ���_���̐ݒ�
			if (nCnt >= 3)
			{	//�ȂȂ�
				pVtx[0].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x - (E_WAVE_SIZE_X / 2), g_WavePicpos[nCnt].y - (E_WAVE_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
				pVtx[1].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x + (E_WAVE_SIZE_X / 2), g_WavePicpos[nCnt].y - (E_WAVE_SIZE_Y / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x - (E_WAVE_SIZE_X / 2), g_WavePicpos[nCnt].y + (E_WAVE_SIZE_Y / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x + (E_WAVE_SIZE_X / 2), g_WavePicpos[nCnt].y + (E_WAVE_SIZE_Y / 2), 0.0f);

			}
			else if (nCnt < 1)
			{	//wave
				pVtx[0].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x - (E_WAVEPIC_SIZE_X / 2), g_WavePicpos[nCnt].y - (E_WAVEPIC_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
				pVtx[1].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x + (E_WAVEPIC_SIZE_X / 2), g_WavePicpos[nCnt].y - (E_WAVEPIC_SIZE_Y / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x - (E_WAVEPIC_SIZE_X / 2), g_WavePicpos[nCnt].y + (E_WAVEPIC_SIZE_Y / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x + (E_WAVEPIC_SIZE_X / 2), g_WavePicpos[nCnt].y + (E_WAVEPIC_SIZE_Y / 2), 0.0f);
			}
			pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
		}

		//���_�f�[�^���A�����b�N
		g_pVtxBuffEwave->Unlock();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEwave(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEwave;

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffEwave, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureEwave[0]);
	if (bResult)
	{
		// �|���S���̕`��
		for (nCntEwave = 0; nCntEwave < DIGIT_E_WAVE; nCntEwave++)
		{	//����
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntEwave * 4,		//�J�n����C���f�b�N�X
				2);					//�v���~�e�B�u�̐�(�|���S���̐�)
		}

		for (int nCnt = 0; nCnt < E_WAVE_TEXTURE; nCnt++)
		{	//����ȊO
			if (bUse[nCnt])
			{
				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureEwave[nCnt + 1]);

				// �|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
					(DIGIT_E_WAVE + nCnt) * 4,		//�J�n����C���f�b�N�X
					2);					//�v���~�e�B�u�̐�(�|���S���̐�)
			}
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void MakeVertexEwave(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	int nCntEwave;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (DIGIT_E_WAVE + E_WAVE_TEXTURE),	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
								D3DUSAGE_WRITEONLY,		//�Œ�
								FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,		//�Œ�
								&g_pVtxBuffEwave,
								NULL);

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffEwave->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	for (nCntEwave = 0; nCntEwave < DIGIT_E_WAVE; nCntEwave++)
	{	//����
		// ���_���̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aEwavepos[nCntEwave].x - (E_WAVE_SIZE_X / 2), g_aEwavepos[nCntEwave].y - (E_WAVE_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
		pVtx[1].pos = D3DXVECTOR3(g_aEwavepos[nCntEwave].x + (E_WAVE_SIZE_X / 2), g_aEwavepos[nCntEwave].y - (E_WAVE_SIZE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEwavepos[nCntEwave].x - (E_WAVE_SIZE_X / 2), g_aEwavepos[nCntEwave].y + (E_WAVE_SIZE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEwavepos[nCntEwave].x + (E_WAVE_SIZE_X / 2), g_aEwavepos[nCntEwave].y + (E_WAVE_SIZE_Y / 2), 0.0f);

		pVtx[0].rhw = 1.0f;	//1.0f�ŌŒ�
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.1f * g_nEwave[nCntEwave], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_nEwave[nCntEwave]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * g_nEwave[nCntEwave], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_nEwave[nCntEwave]), 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	for (int nCnt = 0; nCnt < E_WAVE_TEXTURE; nCnt++)
	{
		// ���_���̐ݒ�
		if (nCnt >= 3)
		{	//�ȂȂ�
			pVtx[0].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x - (E_WAVE_SIZE_X / 2), g_WavePicpos[nCnt].y - (E_WAVE_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
			pVtx[1].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x + (E_WAVE_SIZE_X / 2), g_WavePicpos[nCnt].y - (E_WAVE_SIZE_Y / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x - (E_WAVE_SIZE_X / 2), g_WavePicpos[nCnt].y + (E_WAVE_SIZE_Y / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x + (E_WAVE_SIZE_X / 2), g_WavePicpos[nCnt].y + (E_WAVE_SIZE_Y / 2), 0.0f);

		}
		else if (nCnt < 1)
		{	//wave
			pVtx[0].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x - (E_WAVEPIC_SIZE_X / 2), g_WavePicpos[nCnt].y - (E_WAVEPIC_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
			pVtx[1].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x + (E_WAVEPIC_SIZE_X / 2), g_WavePicpos[nCnt].y - (E_WAVEPIC_SIZE_Y / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x - (E_WAVEPIC_SIZE_X / 2), g_WavePicpos[nCnt].y + (E_WAVEPIC_SIZE_Y / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x + (E_WAVEPIC_SIZE_X / 2), g_WavePicpos[nCnt].y + (E_WAVEPIC_SIZE_Y / 2), 0.0f);
		}
		else
		{	//waveClear��nextwave
			pVtx[0].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x - (E_WAVEINFO_SIZE_X / 2), g_WavePicpos[nCnt].y - (E_WAVEINFO_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
			pVtx[1].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x + (E_WAVEINFO_SIZE_X / 2), g_WavePicpos[nCnt].y - (E_WAVEINFO_SIZE_Y / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x - (E_WAVEINFO_SIZE_X / 2), g_WavePicpos[nCnt].y + (E_WAVEINFO_SIZE_Y / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_WavePicpos[nCnt].x + (E_WAVEINFO_SIZE_X / 2), g_WavePicpos[nCnt].y + (E_WAVEINFO_SIZE_Y / 2), 0.0f);
		}

		pVtx[0].rhw = 1.0f;	//1.0f�ŌŒ�
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�f�[�^���A�����b�N
	g_pVtxBuffEwave->Unlock();
}

//=============================================================================
// �G�g�̕ύX
//=============================================================================
void AddEwave(int nValue)
{
	//int nDigit;
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	g_nEwave[0] += nValue;

 		//nDigit = g_nEwave % (int)powf(10.0f, DIGIT_E_WAVE) / (int)powf(10.0f, DIGIT_E_WAVE);

		//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffEwave->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I
		
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.1f * g_nEwave[0], 0.0f);
 		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_nEwave[0]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * g_nEwave[0], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_nEwave[0]), 1.0f);

		//���_�f�[�^���A�����b�N
		g_pVtxBuffEwave->Unlock();

		if (g_nEwave[0] >= E_WAVE_MAX)
		{
			DeleteALLObstacle();	//��Q���폜

			SetBoss(D3DXVECTOR3(0, 0, 500), D3DXVECTOR3(0, 0, 0));	//�{�X�o��
			SetBoss(D3DXVECTOR3(0, 0, -500), D3DXVECTOR3(0, 0, 0));	//�{�X�o��

			for (int nCntEnemy = 0; nCntEnemy < 2 ; nCntEnemy++)
			{	//�U�R�G�z�u
				SetEnemy(D3DXVECTOR3(rand() % MESHFIELD_W * 0.5f + MESHFIELD_SIZE * 3,
					0.0f,
					rand() % (int)MESHFIELD_SIZE * MESHFIELD_D - (int)MESHFIELD_SIZE * MESHFIELD_D * 0.5f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);

				SetEnemy(D3DXVECTOR3(-rand() % MESHFIELD_W * 0.5f - MESHFIELD_SIZE * 3,
					0.0f,
					rand() % (int)MESHFIELD_SIZE * MESHFIELD_D - (int)MESHFIELD_SIZE * MESHFIELD_D * 0.5f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
			}
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

		}
		else
		{
			DeleteALLObstacle();	//��Q���폜

			if (g_nEwave[0] % 3 == 0)
			{
				SetBoss(D3DXVECTOR3(0, 0, 500), D3DXVECTOR3(0, 0, 0));	//�{�X�o��
			}
			for (int nCntEnemy = 0; nCntEnemy < 2; nCntEnemy++)
			{	//�U�R�G�z�u
				SetEnemy(D3DXVECTOR3(rand() % MESHFIELD_W * 0.5f + MESHFIELD_SIZE * 3,
					0.0f,
					rand() % (int)MESHFIELD_SIZE * MESHFIELD_D - (int)MESHFIELD_SIZE * MESHFIELD_D * 0.5f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);

				SetEnemy(D3DXVECTOR3(-rand() % MESHFIELD_W * 0.5f - MESHFIELD_SIZE * 3,
					0.0f,
					rand() % (int)MESHFIELD_SIZE * MESHFIELD_D - (int)MESHFIELD_SIZE * MESHFIELD_D * 0.5f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
			}

			for (int nCnt = 0; nCnt < 5; nCnt++)
			{	//��Q���z�u
				SetObstacle(D3DXVECTOR3(rand() % 220 + 80,
					0.0f,
					rand() % (int)MESHFIELD_SIZE * MESHFIELD_D * 0.9f - (int)MESHFIELD_SIZE * MESHFIELD_D * 0.4f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				SetObstacle(D3DXVECTOR3(-rand() % 220 - 80,
					0.0f,
					rand() % (int)MESHFIELD_SIZE * MESHFIELD_D*0.9f - (int)MESHFIELD_SIZE * MESHFIELD_D * 0.4f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}

		}
}

//=============================================================================
// wave���̎擾
//=============================================================================
int  *GetEwave(void)
{
	return &g_nEwave[0];
}

//=============================================================================
// �\�����
//=============================================================================
void SetEwaveresult(bool Set)
{
	bResult = Set;
}
