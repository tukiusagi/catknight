//=============================================================================
//
// ���@���� [magic.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "magic.h"
#include"game.h"
#include "wave.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAGIC_SIZE		(30.0f)		//���@�w�̑傫��
#define MAX_MAGIC		(10)		//�\���ő吔

#define MAGIC_TEXTURE		"data/TEXTURE/���@�w.png"	//�ǂݍ��ރe�N�X�`����

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexMagic(LPDIRECT3DDEVICE9 pDevice);


//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffMagic = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureMagic = NULL;	//�e�N�X�`���̃|�C���^

MAGIC g_aMagic;	//���@�w�̏��

//=============================================================================
// ����������
//=============================================================================
void InitMagic(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �ʒu�E��]�̏����ݒ�
		g_aMagic.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMagic.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMagic.col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		g_aMagic.fSize = MAGIC_SIZE;
		g_aMagic.bUse = false;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
							MAGIC_TEXTURE,
							&g_pTextureMagic);

	// ���_���̍쐬
	MakeVertexMagic(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitMagic(void)
{
	//�e�N�X�`���̊J��
	if (g_pTextureMagic != NULL)
	{
		g_pTextureMagic->Release();
		g_pTextureMagic = NULL;
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffMagic != NULL)
	{
		g_pVtxBuffMagic->Release();
		g_pVtxBuffMagic = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMagic(void)
{
	if (*GetGameState() == GAMESTATE_GAMEOVER)
	{
		g_aMagic.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		VERTEX_3D *pVtx;

		//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffMagic->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

		pVtx[0].col = g_aMagic.col;
		pVtx[1].col = g_aMagic.col;
		pVtx[2].col = g_aMagic.col;
		pVtx[3].col = g_aMagic.col;

		//���_�f�[�^���A�����b�N
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

		//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffMagic->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

		pVtx[0].pos = D3DXVECTOR3(-g_aMagic.fSize, 0.0f, g_aMagic.fSize);
		pVtx[1].pos = D3DXVECTOR3(g_aMagic.fSize, 0.0f, g_aMagic.fSize);
		pVtx[2].pos = D3DXVECTOR3(-g_aMagic.fSize, 0.0f, -g_aMagic.fSize);
		pVtx[3].pos = D3DXVECTOR3(g_aMagic.fSize, 0.0f, -g_aMagic.fSize);

		pVtx[0].col = g_aMagic.col;
		pVtx[1].col = g_aMagic.col;
		pVtx[2].col = g_aMagic.col;
		pVtx[3].col = g_aMagic.col;

		//���_�f�[�^���A�����b�N
		g_pVtxBuffMagic->Unlock();

		g_aMagic.rot.y += 0.01f;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMagic(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	if (*GetGameState() != GAMESTATE_GAMEOVER)
	{
		// �����_�[�X�e�[�g(���Z��������)
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	if (g_aMagic.bUse)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aMagic.mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMagic.rot.y, g_aMagic.rot.x, g_aMagic.rot.z);

		D3DXMatrixMultiply(&g_aMagic.mtxWorld, &g_aMagic.mtxWorld, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aMagic.pos.x, g_aMagic.pos.y, g_aMagic.pos.z);

		D3DXMatrixMultiply(&g_aMagic.mtxWorld, &g_aMagic.mtxWorld, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aMagic.mtxWorld);

		//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pVtxBuffMagic, 0, sizeof(VERTEX_3D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureMagic);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			0,		//�J�n����C���f�b�N�X
			2);					//�v���~�e�B�u�̐�(�|���S���̐�)
	}

	//�ʏ탂�[�h�ɖ߂�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	if (*GetGameState() != GAMESTATE_GAMEOVER)
	{
		// �����_�[�X�e�[�g(�ʏ�u�����h����)
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexMagic(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
		D3DUSAGE_WRITEONLY,		//�Œ�
		FVF_VERTEX_3D,			//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,		//�Œ�
		&g_pVtxBuffMagic,
		NULL);

	VERTEX_3D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffMagic->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

		//���_�̐ݒ�
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

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^���A�����b�N
	g_pVtxBuffMagic->Unlock();
}

//=============================================================================
// �ʒu��ݒ�
//=============================================================================
void SetMagic(D3DXVECTOR3 pos)
{
	if (!g_aMagic.bUse)
	{
		VERTEX_3D *pVtx;

		g_aMagic.pos.x = pos.x;
		g_aMagic.pos.z = pos.z;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffMagic->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_aMagic.fSize, 0.0f, g_aMagic.fSize);
		pVtx[1].pos = D3DXVECTOR3(g_aMagic.fSize, 0.0f, g_aMagic.fSize);
		pVtx[2].pos = D3DXVECTOR3(-g_aMagic.fSize, 0.0f, -g_aMagic.fSize);
		pVtx[3].pos = D3DXVECTOR3(g_aMagic.fSize, 0.0f, -g_aMagic.fSize);

		// ���_�f�[�^���A�����b�N����
		g_pVtxBuffMagic->Unlock();

		g_aMagic.bUse = true;
	}
}

//=============================================================================
// ���̎擾
//=============================================================================
MAGIC *GetMagic(void)
{
	return &g_aMagic;
}