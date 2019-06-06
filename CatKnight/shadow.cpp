//=============================================================================
//
// �e���� [shadow.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "shadow.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define SHADOW_SIZE		(10.0f)		//�e�̑傫��
#define MAX_SHADOW		(100)		//�\���ő吔

#define SHADOW_TEXTURE		"data/TEXTURE/shadow000.jpg"	//�ǂݍ��ރe�N�X�`����

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �e�̍\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;		//�ʒu
	D3DXVECTOR3	rot;		//����
	D3DXCOLOR	col;		//�F
	float		fSize;		//�T�C�Y
	D3DXMATRIX	mtxWorld;	//���[���h�}�g���b�N�X
	bool		bUse;		//�g�p��
}SHADOW;

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffShadow = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureShadow = NULL;	//�e�N�X�`���̃|�C���^

SHADOW g_aShadow[MAX_SHADOW];	//�e�̏��

//=============================================================================
// ����������
//=============================================================================
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �ʒu�E��]�̏����ݒ�
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aShadow[nCntShadow].fSize = SHADOW_SIZE;
		g_aShadow[nCntShadow].bUse = false;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
							SHADOW_TEXTURE,
							&g_pTextureShadow);

	// ���_���̍쐬
	MakeVertexShadow(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitShadow(void)
{
	//�e�N�X�`���̊J��
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateShadow(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	SHADOW *pShadow;

	//���Z����
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	pShadow = &g_aShadow[0];
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++, pShadow++)
	{
		if (pShadow->bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pShadow->mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pShadow->rot.y, pShadow->rot.x, pShadow->rot.z);

			D3DXMatrixMultiply(&pShadow->mtxWorld, &pShadow->mtxWorld, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pShadow->pos.x, pShadow->pos.y, pShadow->pos.z);

			D3DXMatrixMultiply(&pShadow->mtxWorld, &pShadow->mtxWorld, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pShadow->mtxWorld);

			//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureShadow);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
									nCntShadow*4,		//�J�n����C���f�b�N�X
									2);					//�v���~�e�B�u�̐�(�|���S���̐�)
		}
	}

	//�ʏ탂�[�h�ɖ߂�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// �����_�[�X�e�[�g(�ʏ�u�����h����)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
								D3DUSAGE_WRITEONLY,		//�Œ�
								FVF_VERTEX_3D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,		//�Œ�
								&g_pVtxBuffShadow,
								NULL);

	VERTEX_3D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		//���_�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fSize, 0.0f, g_aShadow[nCntShadow].fSize);
		pVtx[1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].fSize, 0.0f, g_aShadow[nCntShadow].fSize);
		pVtx[2].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fSize, 0.0f, -g_aShadow[nCntShadow].fSize);
		pVtx[3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].fSize, 0.0f, -g_aShadow[nCntShadow].fSize);

		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[0].col = g_aShadow[nCntShadow].col;
		pVtx[1].col = g_aShadow[nCntShadow].col;
		pVtx[2].col = g_aShadow[nCntShadow].col;
		pVtx[3].col = g_aShadow[nCntShadow].col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�f�[�^���A�����b�N
	g_pVtxBuffShadow->Unlock();
}

//=============================================================================
// �e�̍쐬
//=============================================================================
int SetSadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSize, D3DXCOLOR col)
{
	int nCntShadow;
	VERTEX_3D *pVtx;

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (!g_aShadow[nCntShadow].bUse)
		{
			g_aShadow[nCntShadow].pos.x = pos.x;
			g_aShadow[nCntShadow].pos.z = pos.z;

			g_aShadow[nCntShadow].rot = rot;
			g_aShadow[nCntShadow].col = col;

			g_aShadow[nCntShadow].fSize = fSize;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += nCntShadow * 4;	//���_�f�[�^�̃|�C���^��4���i�߂�

			// ���_���̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fSize, 0.0f, g_aShadow[nCntShadow].fSize);
			pVtx[1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].fSize, 0.0f, g_aShadow[nCntShadow].fSize);
			pVtx[2].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fSize, 0.0f, -g_aShadow[nCntShadow].fSize);
			pVtx[3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].fSize, 0.0f, -g_aShadow[nCntShadow].fSize);

			pVtx[0].col = g_aShadow[nCntShadow].col;
			pVtx[1].col = g_aShadow[nCntShadow].col;
			pVtx[2].col = g_aShadow[nCntShadow].col;
			pVtx[3].col = g_aShadow[nCntShadow].col;

			// ���_�f�[�^���A�����b�N����
			g_pVtxBuffShadow->Unlock();

			g_aShadow[nCntShadow].bUse = true;

			break;
		}
	}
	return nCntShadow;
}

//=============================================================================
// �e�̈ʒu��ݒ�
//=============================================================================
void SetPositionSadow(int IdxShadow, D3DXVECTOR3 pos, float fSize, D3DXCOLOR col)
{
	VERTEX_3D *pVtx;

	g_aShadow[IdxShadow].pos.x = pos.x;
	g_aShadow[IdxShadow].pos.z = pos.z;

	g_aShadow[IdxShadow].col = col;

	g_aShadow[IdxShadow].fSize = fSize;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += IdxShadow * 4;	//���_�f�[�^�̃|�C���^��4���i�߂�

	// ���_���̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-g_aShadow[IdxShadow].fSize, 0.0f, g_aShadow[IdxShadow].fSize);
	pVtx[1].pos = D3DXVECTOR3(g_aShadow[IdxShadow].fSize, 0.0f, g_aShadow[IdxShadow].fSize);
	pVtx[2].pos = D3DXVECTOR3(-g_aShadow[IdxShadow].fSize, 0.0f, -g_aShadow[IdxShadow].fSize);
	pVtx[3].pos = D3DXVECTOR3(g_aShadow[IdxShadow].fSize, 0.0f, -g_aShadow[IdxShadow].fSize);

	pVtx[0].col = g_aShadow[IdxShadow].col;
	pVtx[1].col = g_aShadow[IdxShadow].col;
	pVtx[2].col = g_aShadow[IdxShadow].col;
	pVtx[3].col = g_aShadow[IdxShadow].col;

	// ���_�f�[�^���A�����b�N����
	g_pVtxBuffShadow->Unlock();
}

//=============================================================================
// �e�̍폜
//=============================================================================
void DeleteShadow(int IdxShadow)
{
	g_aShadow[IdxShadow].bUse = false;
}
