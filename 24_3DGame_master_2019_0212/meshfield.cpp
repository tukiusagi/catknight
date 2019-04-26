//=============================================================================
//
// �t�B�[���h���� [meshfield.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "meshfield.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MESHFIELD_TEXTURE		"data/TEXTURE/field.jpg"	//�ǂݍ��ރe�N�X�`����

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �|���S���̍\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;			//�ʒu
	D3DXVECTOR3	rot;			//����
	D3DXMATRIX	mtxWorld;		//���[���h�}�g���b�N�X
	int			nNumVertex;		//�����_��
	int			nNumIndex;		//���C���f�b�N�X��
	int			nNumPolygon;	//���|���S����
}MESHFIELD;

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffMeshField = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureMeshField = NULL;	//�e�N�X�`���̃|�C���^
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffMeshField = NULL;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^

MESHFIELD g_meshfield;

//=============================================================================
// ����������
//=============================================================================
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �ʒu�E��]�̏����ݒ�
	g_meshfield.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_meshfield.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_meshfield.nNumVertex = (MESHFIELD_W + 1) * (MESHFIELD_D + 1);
	g_meshfield.nNumPolygon = (2 * MESHFIELD_D * MESHFIELD_W) + 4 * (MESHFIELD_D - 1);
	g_meshfield.nNumIndex = (2 * MESHFIELD_D) * (MESHFIELD_W + 1) + 2 * (MESHFIELD_D - 1);
		
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		MESHFIELD_TEXTURE,
							&g_pTextureMeshField);

	// ���_���̍쐬
	MakeVertexMeshField(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshField(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̊J��
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshField(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_meshfield.mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_meshfield.rot.y, g_meshfield.rot.x, g_meshfield.rot.z);

	D3DXMatrixMultiply(&g_meshfield.mtxWorld, &g_meshfield.mtxWorld, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_meshfield.pos.x, g_meshfield.pos.y, g_meshfield.pos.z);

	D3DXMatrixMultiply(&g_meshfield.mtxWorld, &g_meshfield.mtxWorld, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_meshfield.mtxWorld);

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshField);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshField);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
							0,0,
							g_meshfield.nNumVertex,		//���_��
							0,
							g_meshfield.nNumPolygon);	//�`�悷��v���~�e�B�u��				
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_meshfield.nNumVertex,	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
								D3DUSAGE_WRITEONLY,		//�Œ�
								FVF_VERTEX_3D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,		//�Œ�
								&g_pVtxBuffMeshField,
								NULL);

	//�C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_meshfield.nNumIndex,	//�m�ۂ���o�b�t�@�̃T�C�Y(�T�C�Y*�K�v�ȃC���f�b�N�X��)
								D3DUSAGE_WRITEONLY,
								D3DFMT_INDEX16,		//�������16�r�b�g
								D3DPOOL_MANAGED,
								&g_pIdxBuffMeshField,
								NULL);

	VERTEX_3D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	//���_�̐ݒ�
	for (int nCntD = 0; nCntD < MESHFIELD_D + 1; nCntD++)
	{
		for (int nCntW = 0; nCntW < MESHFIELD_W + 1; nCntW++,pVtx++)
		{
			pVtx[0].pos = D3DXVECTOR3((-MESHFIELD_SIZE * 0.5f * MESHFIELD_W) + (MESHFIELD_SIZE * nCntW),	//x
										0.0f,																//y
										(MESHFIELD_SIZE * 0.5f * MESHFIELD_D) - (MESHFIELD_SIZE * nCntD));	//z
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[0].tex = D3DXVECTOR2((1.0f / MESHFIELD_W + 1) * nCntW, (1.0f / MESHFIELD_D + 1) * nCntD);
		}
	}

	//���_�f�[�^���A�����b�N
	g_pVtxBuffMeshField->Unlock();

	WORD *pIdx;		//�C���f�b�N�X�f�[�^�ւ̃|�C���^
	
	//�C���f�b�N�X�f�[�^�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdxD = 0; nCntIdxD < MESHFIELD_D; nCntIdxD++)
	{
		//�k�ޕ�
		if (nCntIdxD > 0)
		{
			pIdx[0] = (MESHFIELD_W + 1) + (MESHFIELD_W + 1) * nCntIdxD;
			pIdx ++;
		}

		//�ʏ�C���f�b�N�X
		for (int nCntIdxW = 0; nCntIdxW < MESHFIELD_W + 1; nCntIdxW++)
		{
			pIdx[0] = (MESHFIELD_W + 1 + nCntIdxW) + ((MESHFIELD_W + 1) * nCntIdxD);	//���_�̔ԍ�(�C���f�b�N�X)����
			pIdx[1] = nCntIdxW + ((MESHFIELD_W + 1) * nCntIdxD);

			pIdx += 2;
		}
		//�k�ޕ�
		if (nCntIdxD<(MESHFIELD_D-1))
		{
			pIdx[0] = MESHFIELD_W  + (MESHFIELD_W + 1) * nCntIdxD;
			pIdx++;

		}
	}

	//���_�f�[�^���A�����b�N
	g_pIdxBuffMeshField->Unlock();
}