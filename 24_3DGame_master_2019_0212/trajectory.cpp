//=============================================================================
//
// �O�Տ��� [trajectory.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "trajectory.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TRAJECTORY_MAX		(20)			//�O�Ղ̍ő吔

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexTrajectory(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �|���S���̍\����
//=============================================================================
typedef struct
{
	D3DXMATRIX	mtxWorld;		//���[���h�}�g���b�N�X
	D3DXMATRIX	aMtxPoint[2];	//�I�t�Z�b�g�̃}�g���b�N�X
	D3DXMATRIX	*pMtxParent;	//�e�̃}�g���b�N�X
	D3DXVECTOR3	offset[2];		//�I�t�Z�b�g
	D3DXVECTOR3	pos[TRAJECTORY_POLYGON * 2 + 2];			//���_�̈ʒu���i���_�����j
	D3DXVECTOR3	rot[2];			//����
	D3DXCOLOR	col;			//�F
	int			nNumVertex;		//�����_��
	int			nNumPolygon;	//���|���S����
	bool		bUse;			//�g�p���Ă��邩
}MESH_TRAJECTORY;

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffTrajectory = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureTrajectory = NULL;	//�e�N�X�`���̃|�C���^

MESH_TRAJECTORY g_aTrajectory[TRAJECTORY_MAX];

//=============================================================================
// ����������
//=============================================================================
void InitTrajectory(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �ʒu�E��]�̏����ݒ�
	for (int nCnt = 0; nCnt < TRAJECTORY_MAX; nCnt++)
	{
		for (int nCntOrbit = 0; nCntOrbit < 2; nCntOrbit++)
		{	//�I�t�Z�b�g�̏�����
			g_aTrajectory[nCnt].offset[nCntOrbit] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aTrajectory[nCnt].rot[nCntOrbit] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		}

		g_aTrajectory[nCnt].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);
		g_aTrajectory[nCnt].nNumPolygon = TRAJECTORY_POLYGON;
		g_aTrajectory[nCnt].nNumVertex = g_aTrajectory[nCnt].nNumPolygon * 2 + 2;
		g_aTrajectory[nCnt].bUse = false;

		for (int nCntOrbit = 0; nCntOrbit < g_aTrajectory[nCnt].nNumVertex; nCntOrbit++)
		{	//�ʒu���̏�����
			g_aTrajectory[nCnt].pos[nCntOrbit] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&g_pTextureTrajectory);

	// ���_���̍쐬
	MakeVertexTrajectory(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitTrajectory(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffTrajectory != NULL)
	{
		g_pVtxBuffTrajectory->Release();
		g_pVtxBuffTrajectory = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureTrajectory != NULL)
	{
		g_pTextureTrajectory->Release();
		g_pTextureTrajectory = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTrajectory(void)
{
	VERTEX_3D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffTrajectory->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	for (int nCnt = 0; nCnt < TRAJECTORY_MAX; nCnt++)
	{
		if (g_aTrajectory[nCnt].bUse)
		{
			pVtx += g_aTrajectory[nCnt].nNumVertex * nCnt;

			for (int nCntpos = 0; nCntpos < g_aTrajectory[nCnt].nNumVertex; nCntpos++,pVtx++)
			{
				pVtx[0].pos = g_aTrajectory[nCnt].pos[nCntpos];	//���_�̍X�V
				if (nCntpos % 2 == 1)
				{
					//�����x�̐ݒ�
					g_aTrajectory[nCnt].col.a = 0.0f;

					pVtx[0].col = g_aTrajectory[nCnt].col;
				}
			}
		}
	}
	//���_�f�[�^���A�����b�N
	g_pVtxBuffTrajectory->Unlock();

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTrajectory(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	//�������J�����O���Ȃ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// �����_�[�X�e�[�g(���Z��������)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	MESH_TRAJECTORY *pTrajectory;

	pTrajectory = &g_aTrajectory[0];

	for (int nCnt = 0; nCnt < TRAJECTORY_MAX; nCnt++, pTrajectory++)
	{
		if (pTrajectory->bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pTrajectory->mtxWorld);

			for (int nCntOrbit = 0; nCntOrbit < 2; nCntOrbit++)
			{	//�I�t�Z�b�g�̍���
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&pTrajectory->aMtxPoint[nCntOrbit]);

				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot, pTrajectory->rot[nCntOrbit].y, pTrajectory->rot[nCntOrbit].x, pTrajectory->rot[nCntOrbit].z);

				D3DXMatrixMultiply(&pTrajectory->aMtxPoint[nCntOrbit], &pTrajectory->aMtxPoint[nCntOrbit], &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, pTrajectory->offset[nCntOrbit].x, pTrajectory->offset[nCntOrbit].y, pTrajectory->offset[nCntOrbit].z);

				D3DXMatrixMultiply(&pTrajectory->aMtxPoint[nCntOrbit], &pTrajectory->aMtxPoint[nCntOrbit], &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

				//�e�q���f
				D3DXMatrixMultiply(&pTrajectory->aMtxPoint[nCntOrbit], &pTrajectory->aMtxPoint[nCntOrbit], pTrajectory->pMtxParent);	//���[���h�s�񁁃��[���h�s�񁖐e�̃��[���h�s��
			}

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pTrajectory->mtxWorld);

			//�ʒu�̓���ւ�
			for (int nCntpos = 0; nCntpos < pTrajectory->nNumPolygon; nCntpos++)
			{
				pTrajectory->pos[pTrajectory->nNumVertex - ((nCntpos + 1) * 2)] = pTrajectory->pos[(pTrajectory->nNumVertex - ((nCntpos + 1) * 2)) - 2];
				pTrajectory->pos[pTrajectory->nNumVertex - (nCntpos * 2 + 1)] = pTrajectory->pos[(pTrajectory->nNumVertex - (nCntpos * 2 + 1)) - 2];
			}
			pTrajectory->pos[0] = D3DXVECTOR3(pTrajectory->aMtxPoint[0]._41, pTrajectory->aMtxPoint[0]._42, pTrajectory->aMtxPoint[0]._43);
			pTrajectory->pos[1] = D3DXVECTOR3(pTrajectory->aMtxPoint[1]._41, pTrajectory->aMtxPoint[1]._42, pTrajectory->aMtxPoint[1]._43);

			//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pVtxBuffTrajectory, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureTrajectory);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCnt*pTrajectory->nNumVertex,					//�J�n����C���f�b�N�X
				pTrajectory->nNumPolygon);					//�v���~�e�B�u�̐�(�|���S���̐�)
		}
	}

	// ����(�����)���J�����O����
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// �����_�[�X�e�[�g(�ʏ�u�����h����)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexTrajectory(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aTrajectory[0].nNumVertex * TRAJECTORY_MAX,	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
		D3DUSAGE_WRITEONLY,		//�Œ�
		FVF_VERTEX_3D,			//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,		//�Œ�
		&g_pVtxBuffTrajectory,
		NULL);

	VERTEX_3D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffTrajectory->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	for (int nCnt = 0; nCnt < TRAJECTORY_MAX; nCnt++)
	{
		for (int nCntOrbit = 0; nCntOrbit < g_aTrajectory[nCnt].nNumVertex; nCntOrbit++,pVtx++)
		{
			//���_�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[0].col = g_aTrajectory[nCnt].col;
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		}
	}
	//���_�f�[�^���A�����b�N
	g_pVtxBuffTrajectory->Unlock();
}

//=============================================================================
// �O�Ղ̍쐬
//=============================================================================
void SetTrajectory(D3DXVECTOR3 offset00, D3DXVECTOR3 offset01,D3DXMATRIX *mtxParent, D3DXCOLOR	col)
{
	for (int nCnt = 0; nCnt < TRAJECTORY_MAX; nCnt++)
	{
		if (!g_aTrajectory[nCnt].bUse)
		{
			g_aTrajectory[nCnt].offset[0] = offset00;
			g_aTrajectory[nCnt].offset[1] = offset01;
			g_aTrajectory[nCnt].pMtxParent = mtxParent;
			g_aTrajectory[nCnt].col = col;

			VERTEX_3D *pVtx;

			//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_pVtxBuffTrajectory->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

			pVtx += g_aTrajectory[nCnt].nNumVertex * nCnt;

			for (int nCntOrbit = 0; nCntOrbit < g_aTrajectory[nCnt].nNumVertex; nCntOrbit++, pVtx++)
			{
				pVtx[0].col = g_aTrajectory[nCnt].col;
			}

			//���_�f�[�^���A�����b�N
			g_pVtxBuffTrajectory->Unlock();

			g_aTrajectory[nCnt].bUse = true;
			break;
		}
	}
}