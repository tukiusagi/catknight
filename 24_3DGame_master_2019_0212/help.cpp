//=============================================================================
//
// �w���v����[help.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"help.h"
#include "princess.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define HELP_TEXTURE		"data/TEXTURE/neko.png"	//�ǂݍ��ރe�N�X�`��

#define HELP_SIZE_X		(200)	//�|���S���T�C�YX
#define HELP_SIZE_Y		(200)	//�|���S���T�C�YY

//=============================================================================
// �w���v�̎��
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXCOLOR	col;
	bool		bUse;
	float		fFlash;
}HELP;

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureHelp = NULL;	//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHelp = NULL;	//���_�o�b�t�@�ւ̃|�C���^

HELP	g_Help;

//=============================================================================
// ����������
//=============================================================================
void InitHelp(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	pDevice = GetDevice();	//�f�o�C�X�̎擾

	g_Help.pos = D3DXVECTOR3(HELP_SIZE_X / 2, SCREEN_HEIGHT - HELP_SIZE_Y / 2, 0.0f);
	g_Help.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_Help.fFlash = 0.1f;
	g_Help.bUse = false;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		HELP_TEXTURE,
		&g_pTextureHelp);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
		D3DUSAGE_WRITEONLY,		//�Œ�
		FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,		//�Œ�
		&g_pVtxBuffHelp,
		NULL);


	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffHelp->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	//���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Help.pos.x - (HELP_SIZE_X / 2), g_Help.pos.y - (HELP_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
	pVtx[1].pos = D3DXVECTOR3(g_Help.pos.x + (HELP_SIZE_X / 2), g_Help.pos.y - (HELP_SIZE_Y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Help.pos.x - (HELP_SIZE_X / 2), g_Help.pos.y + (HELP_SIZE_Y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Help.pos.x + (HELP_SIZE_X / 2), g_Help.pos.y + (HELP_SIZE_Y / 2), 0.0f);

	pVtx[0].rhw = 1.0f;	//1.0f�ŌŒ�
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = g_Help.col;
	pVtx[1].col = g_Help.col;
	pVtx[2].col = g_Help.col;
	pVtx[3].col = g_Help.col;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^���A�����b�N
	g_pVtxBuffHelp->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitHelp(void)
{
	//�e�N�X�`���̊J��
	if (g_pTextureHelp != NULL)
	{
		g_pTextureHelp->Release();
		g_pTextureHelp = NULL;
	}
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffHelp != NULL)
	{
		g_pVtxBuffHelp->Release();
		g_pVtxBuffHelp = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateHelp(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	int nMove = 10;
	PRINCESS *pPrincess = GetPrincess();

	if (pPrincess->bPinch)
	{
		g_Help.bUse = true;
	}
	else
	{
		g_Help.bUse = false;
	}

	g_Help.col.a -= g_Help.fFlash;
	if (g_Help.col.a < 0 || g_Help.col.a > 1)
	{	//�_��
		g_Help.fFlash *= -1;
	}

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffHelp->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	pVtx[0].pos = D3DXVECTOR3(g_Help.pos.x - (HELP_SIZE_X / 2), g_Help.pos.y - (HELP_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
	pVtx[1].pos = D3DXVECTOR3(g_Help.pos.x + (HELP_SIZE_X / 2), g_Help.pos.y - (HELP_SIZE_Y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Help.pos.x - (HELP_SIZE_X / 2), g_Help.pos.y + (HELP_SIZE_Y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Help.pos.x + (HELP_SIZE_X / 2), g_Help.pos.y + (HELP_SIZE_Y / 2), 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = g_Help.col;
	pVtx[1].col = g_Help.col;
	pVtx[2].col = g_Help.col;
	pVtx[3].col = g_Help.col;

	//���_�f�[�^���A�����b�N
	g_pVtxBuffHelp->Unlock();

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawHelp(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffHelp, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);				//���_�t�H�[�}�b�g�̐ݒ�

	if (g_Help.bUse)
	{
		pDevice->SetTexture(0, g_pTextureHelp);		//�e�N�X�`���̐ݒ�

			//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			0,					//�J�n����C���f�b�N�X
			2);					//�v���~�e�B�u�̐�(�|���S���̐�)
	}
}

//=============================================================================
// �w���v�̃Z�b�g
//=============================================================================
void SetHelp(void)
{
	g_Help.bUse = true;
}

//=============================================================================
// �w���v�̏���
//=============================================================================
void DeleteHelp(void)
{
	g_Help.bUse = false;
}
