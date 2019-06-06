//=============================================================================
//
// �P���C�t���� [lifePrincess.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include "lifePrincess.h"
#include "game.h"
#include "princess.h"
#include "sound.h"
#include "logo.h"
#include "score.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define LIFEBAR_POS_X		(200.0f)	//�ʒuX
#define LIFEBAR_POS_Y		(600.0f)	//�ʒuY

#define LIFEBAR_SIZE_X		(400)	//�T�C�YX
#define LIFEBAR_SIZE_Y		(30)	//�T�C�YY
#define LIFEBAR_TEXTUREMAX	(2)		//�e�N�X�`����

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexPLife(LPDIRECT3DDEVICE9 pDevice);


//=============================================================================
// �O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTexturePLife[LIFEBAR_TEXTUREMAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPLife = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

LIFEBAR					g_aPLife;	// ���C�t
D3DXCOLOR				g_aLifeColor[LIFEBAR_TEXTUREMAX];	//�F

//=============================================================================
// ����������
//=============================================================================
void InitPLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PRINCESS *pPrincess = GetPrincess();

	g_aPLife.pos = D3DXVECTOR3(LIFEBAR_POS_X, LIFEBAR_POS_Y, 0.0f);
	g_aPLife.fNowLife = pPrincess->nMaxLife;
	g_aPLife.fMaxLife = pPrincess->nMaxLife;

	g_aLifeColor[0] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	g_aLifeColor[1] = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

	// �e�N�X�`���̓ǂݍ���
	for (int nCnt = 0; nCnt < LIFEBAR_TEXTUREMAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice,				
									NULL,
									&g_pTexturePLife[nCnt]);
	}

	// ���_���̍쐬
	MakeVertexPLife(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitPLife(void)
{
	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < LIFEBAR_TEXTUREMAX; nCnt++)
	{
		if (g_pTexturePLife[nCnt] != NULL)
		{				  
			g_pTexturePLife[nCnt]->Release();
			g_pTexturePLife[nCnt] = NULL;
		}
	}
	
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffPLife != NULL)
	{
		g_pVtxBuffPLife->Release();
		g_pVtxBuffPLife = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePLife(void)
{
	VERTEX_2D *pVtx;

	if (g_aPLife.fNowLife < (g_aPLife.fMaxLife / 3))
	{	//�O���̈��菭�Ȃ��Ƃ��ԕ\��
		g_aLifeColor[1] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffPLife->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	pVtx[4].col = g_aLifeColor[1];
	pVtx[5].col = g_aLifeColor[1];
	pVtx[6].col = g_aLifeColor[1];
	pVtx[7].col = g_aLifeColor[1];

	//���_�f�[�^���A�����b�N
	g_pVtxBuffPLife->Unlock();

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffPLife, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	for (int nCnt = 0; nCnt < LIFEBAR_TEXTUREMAX; nCnt++)
	{
		pDevice->SetTexture(0, g_pTexturePLife[nCnt]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			nCnt * 4,			//�J�n����C���f�b�N�X
			2);					//�v���~�e�B�u�̐�(�|���S���̐�)
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void MakeVertexPLife(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	int nCntPLife;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * LIFEBAR_TEXTUREMAX,	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
								D3DUSAGE_WRITEONLY,		//�Œ�
								FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,		//�Œ�
								&g_pVtxBuffPLife,
								NULL);

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffPLife->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	for (nCntPLife = 0; nCntPLife < LIFEBAR_TEXTUREMAX; nCntPLife++)
	{
		// ���_���̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aPLife.pos.x, g_aPLife.pos.y, 0.0f);	//����0.0���ŌŒ�
		pVtx[1].pos = D3DXVECTOR3(g_aPLife.pos.x + ((g_aPLife.fNowLife / g_aPLife.fMaxLife)*LIFEBAR_SIZE_X), g_aPLife.pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPLife.pos.x, g_aPLife.pos.y + LIFEBAR_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPLife.pos.x + ((g_aPLife.fNowLife / g_aPLife.fMaxLife)*LIFEBAR_SIZE_X), g_aPLife.pos.y + LIFEBAR_SIZE_Y, 0.0f);

		pVtx[0].rhw = 1.0f;	//1.0f�ŌŒ�
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = g_aLifeColor[nCntPLife];
		pVtx[1].col = g_aLifeColor[nCntPLife];
		pVtx[2].col = g_aLifeColor[nCntPLife];
		pVtx[3].col = g_aLifeColor[nCntPLife];

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�f�[�^���A�����b�N
	g_pVtxBuffPLife->Unlock();
}

//=============================================================================
// ���C�t�̌���
//=============================================================================
void DecreasePLife(int nValue)
{
	PRINCESS *pPrincess = GetPrincess();

	if (*GetGameState() == GAMESTATE_NORMAL)
	{
		g_aPLife.fNowLife -= nValue;
		pPrincess->bDamege = true;
		
		VERTEX_2D *pVtx;

		//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffPLife->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

		// ���_���̐ݒ�
		pVtx[4].pos = D3DXVECTOR3(g_aPLife.pos.x, g_aPLife.pos.y, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(g_aPLife.pos.x  + ((g_aPLife.fNowLife/g_aPLife.fMaxLife)*LIFEBAR_SIZE_X) , g_aPLife.pos.y, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(g_aPLife.pos.x, g_aPLife.pos.y + LIFEBAR_SIZE_Y, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(g_aPLife.pos.x  + ((g_aPLife.fNowLife / g_aPLife.fMaxLife)*LIFEBAR_SIZE_X), g_aPLife.pos.y + LIFEBAR_SIZE_Y, 0.0f);

		//���_�f�[�^���A�����b�N
		g_pVtxBuffPLife->Unlock();

		if (g_aPLife.fNowLife <= 0 && *GetGameState() != GAMESTATE_GAMEOVER)
		{
			g_aPLife.fNowLife = 0;
			SetGameState(GAMESTATE_GAMEOVER);
			pPrincess->state = PRINCESSSTATE_DETH;
			pPrincess->nCntFrame = 0;
			pPrincess->nCntKeyPrincess = 0;
			SetLogo(LOGO_GAMEOVER);
			StopSound(SOUND_LABEL_BGM_GAME);
			PlaySound(SOUND_LABEL_BGM_GAMEOVER);
		}
	}
}

//=============================================================================
// ���C�t�̑���
//=============================================================================
void IncreasePLife(int nlife)
{
	g_aPLife.fNowLife += nlife;

	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffPLife->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	// ���_���̐ݒ�
	pVtx[4].pos = D3DXVECTOR3(g_aPLife.pos.x, g_aPLife.pos.y, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_aPLife.pos.x + ((g_aPLife.fNowLife / g_aPLife.fMaxLife)*LIFEBAR_SIZE_X), g_aPLife.pos.y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_aPLife.pos.x, g_aPLife.pos.y + LIFEBAR_SIZE_Y, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aPLife.pos.x + ((g_aPLife.fNowLife / g_aPLife.fMaxLife)*LIFEBAR_SIZE_X), g_aPLife.pos.y + LIFEBAR_SIZE_Y, 0.0f);

	//���_�f�[�^���A�����b�N
	g_pVtxBuffPLife->Unlock();
}

//=============================================================================
// ���C�t�̎擾
//=============================================================================
float GetPLife(void)
{
	return g_aPLife.fNowLife;
}