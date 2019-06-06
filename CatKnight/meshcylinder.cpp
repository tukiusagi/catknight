//=============================================================================
//
// ���b�V�������� [meshcylinder.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "meshcylinder.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MEDHCYLINDER_TEXTURE00	"data/TEXTURE/sky001.jpg"	//�ǂݍ��ރe�N�X�`����
#define MEDHCYLINDER_HEIGHT		(1000.0f)
#define MEDHCYLINDER_RADIUS		(1000.0f)

#define MESHCYLINDER_Y			(1)		//�c������
#define MESHCYLINDER_XZ			(10)	//������

#define MESHCYLINDER_MAX			(1)	//���̍ő吔
#define MEDHCYLINDER_TEXTURE_MAX	(2)		//�e�N�X�`���̍ő吔

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexMeshCylinder(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// ���b�V�����̍\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;			//�ʒu
	D3DXVECTOR3	rot;			//����
	D3DXMATRIX	mtxWorld;		//���[���h�}�g���b�N�X
	int			nNumVertex;		//�����_��
	int			nNumIndex;		//���C���f�b�N�X��
	int			nNumPolygon;	//���|���S����
	int			nType;			//���
	float		fRadius;		//���a
	float		fHeight;		//����
	bool		bUse;			//�g�p���
}MESHCYLINDER;

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffMeshCylinder = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureMeshCylinder[MEDHCYLINDER_TEXTURE_MAX] = {};	//�e�N�X�`���̃|�C���^
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffMeshCylinder = NULL;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^

MESHCYLINDER g_meshcylinder[MESHCYLINDER_MAX];

//=============================================================================
// ����������
//=============================================================================
void InitMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �ʒu�E��]�̏����ݒ�
	for (int nCntCylinder = 0; nCntCylinder < MESHCYLINDER_MAX; nCntCylinder++)
	{
		g_meshcylinder[nCntCylinder].pos = D3DXVECTOR3(0.0f, 800.0f, 0.0f);
		g_meshcylinder[nCntCylinder].rot = D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f);
		g_meshcylinder[nCntCylinder].nNumVertex = (MESHCYLINDER_XZ + 1) * (MESHCYLINDER_Y + 1);
		g_meshcylinder[nCntCylinder].nNumPolygon = MESHCYLINDER_XZ * MESHCYLINDER_Y * 2;
		g_meshcylinder[nCntCylinder].nNumIndex = (MESHCYLINDER_XZ + 1) * (MESHCYLINDER_Y + 1);
		g_meshcylinder[nCntCylinder].fRadius = MEDHCYLINDER_RADIUS;
		g_meshcylinder[nCntCylinder].fHeight = MEDHCYLINDER_HEIGHT;
		g_meshcylinder[nCntCylinder].bUse = false;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		MEDHCYLINDER_TEXTURE00,
		&g_pTextureMeshCylinder[0]);

	// ���_���̍쐬
	MakeVertexMeshCylinder(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshCylinder(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffMeshCylinder != NULL)
	{
		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̊J��
	if (g_pIdxBuffMeshCylinder != NULL)
	{
		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}

	//�e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < MEDHCYLINDER_TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureMeshCylinder[nCnt] != NULL)
		{						 
			g_pTextureMeshCylinder[nCnt]->Release();
			g_pTextureMeshCylinder[nCnt] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshCylinder(void)
{
	VERTEX_3D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	for (int nCntCylinder = 0; nCntCylinder < MESHCYLINDER_MAX; nCntCylinder++)
	{
		if (g_meshcylinder[nCntCylinder].bUse)
		{
			pVtx += nCntCylinder * 22;

			//���_�̐ݒ�
			for (int nCntY = 0; nCntY <= MESHCYLINDER_Y; nCntY++)
			{
				for (int nCntXZ = 0; nCntXZ <= MESHCYLINDER_XZ; nCntXZ++, pVtx++)
				{
					pVtx[0].tex.x += 0.0001f;
				}
			}
		}
	}
	//���_�f�[�^���A�����b�N
	g_pVtxBuffMeshCylinder->Unlock();


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	MESHCYLINDER *pMeshCylinder;
	pMeshCylinder = &g_meshcylinder[0];

	for (int nCntCylinder = 0; nCntCylinder < MESHCYLINDER_MAX; nCntCylinder++, pMeshCylinder++)
	{
		if (pMeshCylinder->bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pMeshCylinder->mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMeshCylinder->rot.y, pMeshCylinder->rot.x, pMeshCylinder->rot.z);

			D3DXMatrixMultiply(&pMeshCylinder->mtxWorld, &pMeshCylinder->mtxWorld, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pMeshCylinder->pos.x, pMeshCylinder->pos.y, pMeshCylinder->pos.z);

			D3DXMatrixMultiply(&pMeshCylinder->mtxWorld, &pMeshCylinder->mtxWorld, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pMeshCylinder->mtxWorld);

			//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));

			//�C���f�b�N�X�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffMeshCylinder);

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureMeshCylinder[pMeshCylinder->nType]);

			//�|���S���̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				0, 0,
				pMeshCylinder->nNumVertex,		//���_��
				0,
				pMeshCylinder->nNumPolygon);	//�`�悷��v���~�e�B�u��
		}
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexMeshCylinder(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_meshcylinder[0].nNumVertex * MESHCYLINDER_MAX,	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
								D3DUSAGE_WRITEONLY,		//�Œ�
								FVF_VERTEX_3D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,		//�Œ�
								&g_pVtxBuffMeshCylinder,
								NULL);

	//�C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_meshcylinder[0].nNumIndex * MESHCYLINDER_MAX,	//�m�ۂ���o�b�t�@�̃T�C�Y(�T�C�Y*�K�v�ȃC���f�b�N�X��)
								D3DUSAGE_WRITEONLY,
								D3DFMT_INDEX16,		//�������16�r�b�g
								D3DPOOL_MANAGED,
								&g_pIdxBuffMeshCylinder,
								NULL);

	VERTEX_3D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I
	for (int nCntCylinder = 0; nCntCylinder < MESHCYLINDER_MAX; nCntCylinder++)
	{
		//���_�̐ݒ�
		for (int nCntY = 0; nCntY <= MESHCYLINDER_Y; nCntY++)
		{
			for (int nCntXZ = 0; nCntXZ <= MESHCYLINDER_XZ; nCntXZ++, pVtx++)
			{
				pVtx[0].pos = D3DXVECTOR3(sinf((2 * D3DX_PI / MESHCYLINDER_XZ) * nCntXZ) * g_meshcylinder[nCntCylinder].fRadius,	//x
					g_meshcylinder[nCntCylinder].fHeight* nCntY,															//y
					cosf((2 * D3DX_PI / MESHCYLINDER_XZ) * nCntXZ) * g_meshcylinder[nCntCylinder].fRadius);						//z

				pVtx[0].nor = pVtx[0].pos;
				D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[0].tex = D3DXVECTOR2((1.0f / MESHCYLINDER_XZ) * nCntXZ, (1.0f / MESHCYLINDER_Y) * nCntY);
			}
		}
	}
	//���_�f�[�^���A�����b�N
	g_pVtxBuffMeshCylinder->Unlock();

	WORD *pIdx;		//�C���f�b�N�X�f�[�^�ւ̃|�C���^
	
	//�C���f�b�N�X�f�[�^�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntCylinder = 0; nCntCylinder < MESHCYLINDER_MAX; nCntCylinder++)
	{
		for (int nCntIdxY = 0; nCntIdxY < MESHCYLINDER_Y; nCntIdxY++)
		{
			//�ʏ�C���f�b�N�X
			for (int nCntIdxXZ = 0; nCntIdxXZ <= MESHCYLINDER_XZ; nCntIdxXZ++)
			{
				pIdx[0] = nCntIdxXZ + ((MESHCYLINDER_XZ + 1) * nCntIdxY);	//���_�̔ԍ�(�C���f�b�N�X)����
				pIdx[1] = (MESHCYLINDER_XZ + 1) + nCntIdxXZ + ((MESHCYLINDER_XZ + 1) * nCntIdxY);

				pIdx += 2;
			}
		}
	}
	//���_�f�[�^���A�����b�N
	g_pIdxBuffMeshCylinder->Unlock();
}

//=============================================================================
// ���̍쐬
//=============================================================================
void SetMeshCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float radius, float height, int type)
{
	for (int nCntCylinder = 0; nCntCylinder < MESHCYLINDER_MAX; nCntCylinder++)
	{
		if (!g_meshcylinder[nCntCylinder].bUse)
		{
			g_meshcylinder[nCntCylinder].pos = pos;
			g_meshcylinder[nCntCylinder].rot = rot;
			g_meshcylinder[nCntCylinder].fRadius = radius;
			g_meshcylinder[nCntCylinder].fHeight = height;
			g_meshcylinder[nCntCylinder].nType = type;

			VERTEX_3D *pVtx;

			//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

			pVtx += nCntCylinder * 22;

			//���_�̐ݒ�
			for (int nCntY = 0; nCntY <= MESHCYLINDER_Y; nCntY++)
			{
				for (int nCntXZ = 0; nCntXZ <= MESHCYLINDER_XZ; nCntXZ++, pVtx++)
				{
					pVtx[0].pos = D3DXVECTOR3(sinf((2 * D3DX_PI / MESHCYLINDER_XZ) * nCntXZ) * g_meshcylinder[nCntCylinder].fRadius,	//x
						g_meshcylinder[nCntCylinder].fHeight* nCntY,															//y
						cosf((2 * D3DX_PI / MESHCYLINDER_XZ) * nCntXZ) * g_meshcylinder[nCntCylinder].fRadius);						//z
				}
			}

			//���_�f�[�^���A�����b�N
			g_pVtxBuffMeshCylinder->Unlock();

			g_meshcylinder[nCntCylinder].bUse = true;
			break;
		}
	}
}