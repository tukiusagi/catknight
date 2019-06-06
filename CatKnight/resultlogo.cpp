//=============================================================================
//
// ���U���g���S����[resultlogo.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"resultlogo.h"	//�C���N���[�h�t�@�C��
#include "sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define RELOGO_BG			"data/TEXTURE/tutorialBG.jpg"	//�ǂݍ��ރe�N�X�`����
#define RELOGO_BG_BLACK		"data/TEXTURE/black.png"		//�ǂݍ��ރe�N�X�`����
#define RELOGO_BASIS		"data/TEXTURE/Basis.png"		//�ǂݍ��ރe�N�X�`����
#define RELOGO_COMBO		"data/TEXTURE/combo.png"		//�ǂݍ��ރe�N�X�`����
#define RELOGO_WAVE			"data/TEXTURE/maxwave.png"		//�ǂݍ��ރe�N�X�`����
#define RELOGO_SCORE		"data/TEXTURE/score.png"		//�ǂݍ��ރe�N�X�`����
#define RELOGO_POS_X		(0)		//���U���g���S�ʒuX
#define RELOGO_POS_Y		(0)		//���U���g���S�ʒuY
#define RELOGO_SIZE_X		(1280)	//���U���g���S�T�C�YX
#define RELOGO_SIZE_Y		(720)	//���U���g���S�T�C�YY
#define RELOGO_CHARSIZE_X	(300)	//���U���g���S�T�C�YX
#define RELOGO_CHARSIZE_Y	(105)	//���U���g���S�T�C�YY

#define RELOGOTEXTURE_MAX	(6)		//�e�N�X�`����

//=============================================================================
// �`���[�g���A���̏��
//=============================================================================

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureReLogo[RELOGOTEXTURE_MAX] = {};			//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffReLogo = NULL;	//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3	g_ReLogopos[RELOGOTEXTURE_MAX];

//=============================================================================
// ���U���g���S�̏���������
//=============================================================================
void InitReLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();	//�f�o�C�X�̎擾
	g_ReLogopos[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_ReLogopos[1] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int nCnt = 2; nCnt < RELOGOTEXTURE_MAX; nCnt++)
	{
		if (nCnt > 4)
		{
			g_ReLogopos[nCnt] = D3DXVECTOR3(300.0f, 120.0f + ((nCnt-1)*100.0f) + 100, 0.0f);
		}
		else
		{
			g_ReLogopos[nCnt] = D3DXVECTOR3(300.0f, 120.0f + ((nCnt-1)*100.0f), 0.0f);
		}
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		RELOGO_BG,
		&g_pTextureReLogo[0]);

	D3DXCreateTextureFromFile(pDevice,
		RELOGO_BG_BLACK,
		&g_pTextureReLogo[1]);

	D3DXCreateTextureFromFile(pDevice,
		RELOGO_BASIS,
		&g_pTextureReLogo[2]);

	D3DXCreateTextureFromFile(pDevice,
		RELOGO_COMBO,
		&g_pTextureReLogo[3]);

	D3DXCreateTextureFromFile(pDevice,
		RELOGO_WAVE,
		&g_pTextureReLogo[4]);

	D3DXCreateTextureFromFile(pDevice,
		RELOGO_SCORE,
		&g_pTextureReLogo[5]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * RELOGOTEXTURE_MAX,	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
								D3DUSAGE_WRITEONLY,		//�Œ�
								FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,		//�Œ�
								&g_pVtxBuffReLogo,
								NULL);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffReLogo->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	for (int nCntBG = 0; nCntBG < 2; nCntBG++)
	{
		//���_�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_ReLogopos[0].x, g_ReLogopos[0].y, 0.0f);	//����0.0���ŌŒ�
		pVtx[1].pos = D3DXVECTOR3(g_ReLogopos[0].x + RELOGO_SIZE_X, g_ReLogopos[0].y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_ReLogopos[0].x, g_ReLogopos[0].y + RELOGO_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_ReLogopos[0].x + RELOGO_SIZE_X, g_ReLogopos[0].y + RELOGO_SIZE_Y, 0.0f);

		pVtx[0].rhw = 1.0f;	//1.0f�ŌŒ�
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		if (nCntBG > 0)
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}
		else
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	for (int nCnt = 2; nCnt < RELOGOTEXTURE_MAX; nCnt++)
	{
		//���_�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_ReLogopos[nCnt].x - (RELOGO_CHARSIZE_X / 2), g_ReLogopos[nCnt].y - (RELOGO_CHARSIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
		pVtx[1].pos = D3DXVECTOR3(g_ReLogopos[nCnt].x + (RELOGO_CHARSIZE_X / 2), g_ReLogopos[nCnt].y - (RELOGO_CHARSIZE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_ReLogopos[nCnt].x - (RELOGO_CHARSIZE_X / 2), g_ReLogopos[nCnt].y + (RELOGO_CHARSIZE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_ReLogopos[nCnt].x + (RELOGO_CHARSIZE_X / 2), g_ReLogopos[nCnt].y + (RELOGO_CHARSIZE_Y / 2), 0.0f);

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

		pVtx += 4;
	}

	//���_�f�[�^���A�����b�N
	g_pVtxBuffReLogo->Unlock();

}

//=============================================================================
// ���U���g���S�̏I������
//=============================================================================
void UninitReLogo(void)
{
	//�e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < RELOGOTEXTURE_MAX; nCnt++)
	{
		if (g_pTextureReLogo[nCnt] != NULL)
		{					  
			g_pTextureReLogo[nCnt]->Release();
			g_pTextureReLogo[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffReLogo != NULL)
	{
		g_pVtxBuffReLogo->Release();
		g_pVtxBuffReLogo = NULL;
	}

}

//=============================================================================
// ���U���g���S�̍X�V����
//=============================================================================
void UpdateReLogo(void)
{
}

//=============================================================================
// ���U���g���S�̕`�揈��
//=============================================================================
void DrawReLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffReLogo, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);				//���_�t�H�[�}�b�g�̐ݒ�

	for (int nCnt = 0; nCnt < RELOGOTEXTURE_MAX; nCnt++)
	{
		pDevice->SetTexture(0, g_pTextureReLogo[nCnt]);	//�e�N�X�`���̐ݒ�

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			nCnt*4,					//�J�n����C���f�b�N�X
			2);					//�v���~�e�B�u�̐�
	}
}