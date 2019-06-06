//=============================================================================
//
// ���b�V���Ǐ��� [meshwall.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "meshwall.h"
#include "meshfield.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MESHWALL_TEXTURE	"data/TEXTURE/Free Texture Seamless Brick 09-28-10_07.jpg"	//�ǂݍ��ރe�N�X�`����
#define MESHWALL_MAX		(20)			//�ǂ̍ő吔

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexMeshWall(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// ���b�V���ǂ̍\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;			//�ʒu
	D3DXVECTOR3	rot;			//����
	D3DXMATRIX	mtxWorld;		//���[���h�}�g���b�N�X
	int			nNumVertex;		//�����_��
	int			nNumIndex;		//���C���f�b�N�X��
	int			nNumPolygon;	//���|���S����
	bool		bUse;			//�g�p���Ă��邩
}MESHWALL;

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffMeshWall = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureMeshWall = NULL;	//�e�N�X�`���̃|�C���^
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffMeshWall = NULL;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^

MESHWALL g_ameshwall[MESHWALL_MAX];

//=============================================================================
// ����������
//=============================================================================
void InitMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �ʒu�E��]�̏����ݒ�
	for (int nCntWall = 0; nCntWall < MESHWALL_MAX; nCntWall++)
	{
		g_ameshwall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ameshwall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ameshwall[nCntWall].nNumVertex = (MESHWALL_W + 1) * (MESHWALL_H + 1);
		g_ameshwall[nCntWall].nNumPolygon = (2 * MESHWALL_H * MESHWALL_W) + 4 * (MESHWALL_H - 1);
		g_ameshwall[nCntWall].nNumIndex = (2 * MESHWALL_H) * (MESHWALL_W + 1) + 2 * (MESHWALL_H - 1);
		g_ameshwall[nCntWall].bUse = false;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		MESHWALL_TEXTURE,
							&g_pTextureMeshWall);

	// ���_���̍쐬
	MakeVertexMeshWall(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshWall(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffMeshWall != NULL)
	{
		g_pVtxBuffMeshWall->Release();
		g_pVtxBuffMeshWall = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̊J��
	if (g_pIdxBuffMeshWall != NULL)
	{
		g_pIdxBuffMeshWall->Release();
		g_pIdxBuffMeshWall = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureMeshWall != NULL)
	{
		g_pTextureMeshWall->Release();
		g_pTextureMeshWall = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshWall(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	MESHWALL *pMeshWall;

	pMeshWall = &g_ameshwall[0];

	for (int nCntWall = 0; nCntWall < MESHWALL_MAX; nCntWall++, pMeshWall++)
	{
		if (pMeshWall->bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pMeshWall->mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMeshWall->rot.y, pMeshWall->rot.x, pMeshWall->rot.z);

			D3DXMatrixMultiply(&pMeshWall->mtxWorld, &pMeshWall->mtxWorld, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pMeshWall->pos.x, pMeshWall->pos.y, pMeshWall->pos.z);

			D3DXMatrixMultiply(&pMeshWall->mtxWorld, &pMeshWall->mtxWorld, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pMeshWall->mtxWorld);

			//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pVtxBuffMeshWall, 0, sizeof(VERTEX_3D));

			//�C���f�b�N�X�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffMeshWall);

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureMeshWall);

			//�|���S���̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				0, 0,
				pMeshWall->nNumVertex,		//���_��
				0,
				pMeshWall->nNumPolygon);	//�`�悷��v���~�e�B�u��
		}
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexMeshWall(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_ameshwall[0].nNumVertex * MESHWALL_MAX,	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
								D3DUSAGE_WRITEONLY,		//�Œ�
								FVF_VERTEX_3D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,		//�Œ�
								&g_pVtxBuffMeshWall,
								NULL);

	//�C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_ameshwall[0].nNumIndex * MESHWALL_MAX,	//�m�ۂ���o�b�t�@�̃T�C�Y(�T�C�Y*�K�v�ȃC���f�b�N�X��)
								D3DUSAGE_WRITEONLY,
								D3DFMT_INDEX16,		//�������16�r�b�g
								D3DPOOL_MANAGED,
								&g_pIdxBuffMeshWall,
								NULL);

	VERTEX_3D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffMeshWall->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	for (int nCntWall = 0; nCntWall < MESHWALL_MAX; nCntWall++)
	{
		//���_�̐ݒ�
		for (int nCntY = 0; nCntY < MESHWALL_H + 1; nCntY++)
		{
			for (int nCntW = 0; nCntW < MESHWALL_W + 1; nCntW++,pVtx++)
			{
				pVtx[0].pos = D3DXVECTOR3((-MESHWALL_WIDTH * 0.5f * MESHWALL_W) + (MESHWALL_WIDTH * nCntW),				//x
											(MESHWALL_HEIGHT * 0.5f * (MESHWALL_H * 2)) - (MESHWALL_HEIGHT * nCntY),	//y
											0.0f);																		//z
				pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[0].tex = D3DXVECTOR2((1.0f / MESHWALL_W + 1) * nCntW, (1.0f / MESHWALL_H + 1) * nCntY);
			}
		}
	}

	//���_�f�[�^���A�����b�N
	g_pVtxBuffMeshWall->Unlock();

	WORD *pIdx;		//�C���f�b�N�X�f�[�^�ւ̃|�C���^
	
	//�C���f�b�N�X�f�[�^�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshWall->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntWall = 0; nCntWall < MESHWALL_MAX; nCntWall++)
	{
		for (int nCntIdxH = 0; nCntIdxH < MESHWALL_H; nCntIdxH++)
		{
			//�k�ޕ�
			if (nCntIdxH > 0)
			{
				pIdx[0] = (MESHWALL_W + 1) + (MESHWALL_W + 1) * nCntIdxH;
				pIdx++;
			}

			//�ʏ�C���f�b�N�X
			for (int nCntIdxW = 0; nCntIdxW < MESHWALL_W + 1; nCntIdxW++)
			{
				pIdx[0] = (MESHWALL_W + 1 + nCntIdxW) + ((MESHWALL_W + 1) * nCntIdxH);	//���_�̔ԍ�(�C���f�b�N�X)����
				pIdx[1] = nCntIdxW + ((MESHWALL_W + 1) * nCntIdxH);

				pIdx += 2;
			}
			//�k�ޕ�
			if (nCntIdxH < (MESHWALL_H - 1))
			{
				pIdx[0] = MESHWALL_W + (MESHWALL_W + 1) * nCntIdxH;
				pIdx++;
			}

		}
	}
	//���_�f�[�^���A�����b�N
	g_pIdxBuffMeshWall->Unlock();
}

//=============================================================================
// �ǂ̍쐬
//=============================================================================
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntWall = 0; nCntWall < MESHWALL_MAX; nCntWall++)
	{
		if (!g_ameshwall[nCntWall].bUse)
		{
			g_ameshwall[nCntWall].pos = pos;
			g_ameshwall[nCntWall].rot = rot;
			g_ameshwall[nCntWall].bUse = true;

			break;
		}
	}
}