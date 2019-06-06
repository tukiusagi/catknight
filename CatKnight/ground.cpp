//=============================================================================
//
// �n��	���� [ground.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "ground.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define GROUND_TEXTURE		"data/TEXTURE/field004.jpg"	//�ǂݍ��ރe�N�X�`����

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexGround(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �n�ʂ̍\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;			//�ʒu
	D3DXVECTOR3	rot;			//����
	D3DXMATRIX	mtxWorld;		//���[���h�}�g���b�N�X
	int			nNumVertex;		//�����_��
	int			nNumIndex;		//���C���f�b�N�X��
	int			nNumPolygon;	//���|���S����
}GROUND;

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffGround = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureGround = NULL;	//�e�N�X�`���̃|�C���^
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffGround = NULL;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^

GROUND g_ground;

//=============================================================================
// ����������
//=============================================================================
void InitGround(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �ʒu�E��]�̏����ݒ�
	g_ground.pos = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	g_ground.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_ground.nNumVertex = (GROUND_W + 1) * (GROUND_D + 1);
	g_ground.nNumPolygon = (2 * GROUND_D * GROUND_W) + 4 * (GROUND_D - 1);
	g_ground.nNumIndex = (2 * GROUND_D) * (GROUND_W + 1) + 2 * (GROUND_D - 1);
		
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		GROUND_TEXTURE,
							&g_pTextureGround);

	// ���_���̍쐬
	MakeVertexGround(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitGround(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffGround != NULL)
	{
		g_pVtxBuffGround->Release();
		g_pVtxBuffGround = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̊J��
	if (g_pIdxBuffGround != NULL)
	{
		g_pIdxBuffGround->Release();
		g_pIdxBuffGround = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureGround != NULL)
	{
		g_pTextureGround->Release();
		g_pTextureGround = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGround(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGround(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_ground.mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ground.rot.y, g_ground.rot.x, g_ground.rot.z);

	D3DXMatrixMultiply(&g_ground.mtxWorld, &g_ground.mtxWorld, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_ground.pos.x, g_ground.pos.y, g_ground.pos.z);

	D3DXMatrixMultiply(&g_ground.mtxWorld, &g_ground.mtxWorld, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_ground.mtxWorld);

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffGround, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffGround);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureGround);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
							0,0,
							g_ground.nNumVertex,		//���_��
							0,
							g_ground.nNumPolygon);	//�`�悷��v���~�e�B�u��				
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexGround(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_ground.nNumVertex,	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
								D3DUSAGE_WRITEONLY,		//�Œ�
								FVF_VERTEX_3D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,		//�Œ�
								&g_pVtxBuffGround,
								NULL);

	//�C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_ground.nNumIndex,	//�m�ۂ���o�b�t�@�̃T�C�Y(�T�C�Y*�K�v�ȃC���f�b�N�X��)
								D3DUSAGE_WRITEONLY,
								D3DFMT_INDEX16,		//�������16�r�b�g
								D3DPOOL_MANAGED,
								&g_pIdxBuffGround,
								NULL);

	VERTEX_3D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffGround->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	//���_�̐ݒ�
	for (int nCntD = 0; nCntD < GROUND_D + 1; nCntD++)
	{
		for (int nCntW = 0; nCntW < GROUND_W + 1; nCntW++,pVtx++)
		{
			pVtx[0].pos = D3DXVECTOR3((-GROUND_SIZE * 0.5f * GROUND_W) + (GROUND_SIZE * nCntW),	//x
										0.0f,																//y
										(GROUND_SIZE * 0.5f * GROUND_D) - (GROUND_SIZE * nCntD));	//z
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[0].tex = D3DXVECTOR2((1.0f / GROUND_W + 1) * nCntW, (1.0f / GROUND_D + 1) * nCntD);
		}
	}

	//���_�f�[�^���A�����b�N
	g_pVtxBuffGround->Unlock();

	WORD *pIdx;		//�C���f�b�N�X�f�[�^�ւ̃|�C���^
	
	//�C���f�b�N�X�f�[�^�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffGround->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdxD = 0; nCntIdxD < GROUND_D; nCntIdxD++)
	{
		//�k�ޕ�
		if (nCntIdxD > 0)
		{
			pIdx[0] = (GROUND_W + 1) + (GROUND_W + 1) * nCntIdxD;
			pIdx ++;
		}

		//�ʏ�C���f�b�N�X
		for (int nCntIdxW = 0; nCntIdxW < GROUND_W + 1; nCntIdxW++)
		{
			pIdx[0] = (GROUND_W + 1 + nCntIdxW) + ((GROUND_W + 1) * nCntIdxD);	//���_�̔ԍ�(�C���f�b�N�X)����
			pIdx[1] = nCntIdxW + ((GROUND_W + 1) * nCntIdxD);

			pIdx += 2;
		}
		//�k�ޕ�
		if (nCntIdxD<(GROUND_D-1))
		{
			pIdx[0] = GROUND_W  + (GROUND_W + 1) * nCntIdxD;
			pIdx++;

		}
	}

	//���_�f�[�^���A�����b�N
	g_pIdxBuffGround->Unlock();
}