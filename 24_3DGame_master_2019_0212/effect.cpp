//=============================================================================
//
// �G�t�F�N�g���� [effect.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "effect.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define EFFECT_TEXTURE	"data/TEXTURE/effect000.jpg"	//�ǂݍ��ރe�N�X�`����
#define EFFECT_SIZE		(10.0f)	//�T�C�Y
#define EFFECT_MAX		(1000)	//�\���ő吔

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �|���S���̍\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;			//�ʒu
	D3DXVECTOR3	rot;			//����
	D3DXCOLOR	col;			//�F
	float		fRadius;		// ���a
	int			nLife;			// ���C�t
	D3DXMATRIX	mtxWorld;		//���[���h�}�g���b�N�X
	bool		bUse;			//�g�p���
}EFFECT;

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffEffect = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureEffect = NULL;	//�e�N�X�`���̃|�C���^

EFFECT g_aEffect[EFFECT_MAX];

//=============================================================================
// ����������
//=============================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �ʒu�E��]�̏����ݒ�
	for (int nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].fRadius = 0;
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].bUse = false;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		EFFECT_TEXTURE,
							&g_pTextureEffect);

	// ���_���̍쐬
	MakeVertexEffect(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitEffect(void)
{
	//�e�N�X�`���̊J��
	if (g_pTextureEffect != NULL)
	{			  
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffEffect != NULL)
	{			  
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEffect(void)
{
	VERTEX_3D *pVtx;

	for (int nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse)	//�e���g�p����Ă���
		{
			g_aEffect[nCntEffect].nLife--;
			g_aEffect[nCntEffect].fRadius -= g_aEffect[nCntEffect].fRadius/ g_aEffect[nCntEffect].nLife;
			g_aEffect[nCntEffect].col.a -= 0.03f;

			if (g_aEffect[nCntEffect].fRadius <= 0)
			{
				g_aEffect[nCntEffect].fRadius = 0;
			}
			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += nCntEffect * 4;	//���_�f�[�^�̃|�C���^��4���i�߂�

			// ���_���̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius,  g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3( g_aEffect[nCntEffect].fRadius,  g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3( g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			// ���_�f�[�^���A�����b�N����
			g_pVtxBuffEffect->Unlock();

			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].bUse = false;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans, mtxView;	// �v�Z�p�}�g���b�N�X

	EFFECT *pEffect;

	pEffect = &g_aEffect[0];

	//���݂̃r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// �����_�[�X�e�[�g(���Z��������)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);

	for (int nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++, pEffect++)
	{
		if (pEffect->bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pEffect->mtxWorld);

			//�t�s��̐ݒ�
			pEffect->mtxWorld._11 = mtxView._11;
			pEffect->mtxWorld._12 = mtxView._21;
			pEffect->mtxWorld._13 = mtxView._31;
			pEffect->mtxWorld._21 = mtxView._12;
			pEffect->mtxWorld._22 = mtxView._22;
			pEffect->mtxWorld._23 = mtxView._32;
			pEffect->mtxWorld._31 = mtxView._13;
			pEffect->mtxWorld._32 = mtxView._23;
			pEffect->mtxWorld._33 = mtxView._33;

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pEffect->rot.y, pEffect->rot.x, pEffect->rot.z);

			D3DXMatrixMultiply(&pEffect->mtxWorld, &pEffect->mtxWorld, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pEffect->pos.x, pEffect->pos.y, pEffect->pos.z);

			D3DXMatrixMultiply(&pEffect->mtxWorld, &pEffect->mtxWorld, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pEffect->mtxWorld);

			//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEffect);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
									nCntEffect * 4,		//�J�n����C���f�b�N�X
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
void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * EFFECT_MAX,	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
								D3DUSAGE_WRITEONLY,		//�Œ�
								FVF_VERTEX_3D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,		//�Œ�
								&g_pVtxBuffEffect,
								NULL);

	VERTEX_3D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	for (int nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++)
	{
		//���_�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

		pVtx[0].nor = D3DXVECTOR3(0.0f,1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f,1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f,1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f,1.0f, 0.0f);

		pVtx[0].col = g_aEffect[nCntEffect].col;
		pVtx[1].col = g_aEffect[nCntEffect].col;
		pVtx[2].col = g_aEffect[nCntEffect].col;
		pVtx[3].col = g_aEffect[nCntEffect].col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�f�[�^���A�����b�N
	g_pVtxBuffEffect->Unlock();
}

//=============================================================================
// �����̐ݒ�
//=============================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife)
{
	VERTEX_3D *pVtx;

	for (int nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++)
	{
		if (!g_aEffect[nCntEffect].bUse)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

			pVtx += nCntEffect * 4;	//���_�f�[�^�̃|�C���^��4���i�߂�

			//���_�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			//���_�f�[�^���A�����b�N
			g_pVtxBuffEffect->Unlock();

			g_aEffect[nCntEffect].bUse = true;
			break;
		}
	}
}