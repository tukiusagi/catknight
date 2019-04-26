//=============================================================================
//
// ���C������ [line.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "line.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define LINE_TEXTURE	"data/TEXTURE/tree000.png"	//�ǂݍ��ރe�N�X�`����
#define LINE_SIZE		(30.0f)	//�T�C�Y
#define LINE_MAX		(100)	//�\���ő吔

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexLine(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �|���S���̍\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;		//�ʒu
	D3DXVECTOR3	rot;		//����
	D3DXVECTOR3	start;		//�n�_
	D3DXVECTOR3	end;		//�I�_
	D3DXMATRIX	mtxWorld;	//���[���h�}�g���b�N�X
	D3DXCOLOR	col;		//�F
	bool		bUse;		//�g�p���
}LINE;

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffLine = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureLine = NULL;	//�e�N�X�`���̃|�C���^

LINE g_aline[LINE_MAX];

//=============================================================================
// ����������
//=============================================================================
void InitLine(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �ʒu�E��]�̏����ݒ�
	for (int nCntLine = 0; nCntLine < LINE_MAX; nCntLine++)
	{
		g_aline[nCntLine].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aline[nCntLine].start = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aline[nCntLine].end = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aline[nCntLine].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aline[nCntLine].col= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aline[nCntLine].bUse = false;
	}

	//�e�N�X�`���̓ǂݍ���
	//D3DXCreateTextureFromFile(pDevice,
	//						LINE_TEXTURE,
	//						&g_pTextureLine);

	// ���_���̍쐬
	MakeVertexLine(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitLine(void)
{
	//�e�N�X�`���̊J��
	//if (g_pTextureLine != NULL)
	//{			  
	//	g_pTextureLine->Release();
	//	g_pTextureLine = NULL;
	//}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffLine != NULL)
	{			  
		g_pVtxBuffLine->Release();
		g_pVtxBuffLine = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateLine(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawLine(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X
	LINE *pLine;

	pLine = &g_aline[0];

	for (int nCntLine = 0; nCntLine < LINE_MAX; nCntLine++, pLine++)
	{
		if (pLine->bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pLine->mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pLine->rot.y, pLine->rot.x, pLine->rot.z);

			D3DXMatrixMultiply(&pLine->mtxWorld, &pLine->mtxWorld, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pLine->pos.x, pLine->pos.y, pLine->pos.z);

			D3DXMatrixMultiply(&pLine->mtxWorld, &pLine->mtxWorld, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pLine->mtxWorld);

			//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pVtxBuffLine, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			//pDevice->SetTexture(0, g_pTextureLine);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_LINELIST,	//�v���~�e�B�u�̎��
									nCntLine * 2,	//�J�n����C���f�b�N�X
									2);				//�v���~�e�B�u�̐�(�|���S���̐�)
		}
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexLine(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 2 * LINE_MAX,	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
								D3DUSAGE_WRITEONLY,		//�Œ�
								FVF_VERTEX_3D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,		//�Œ�
								&g_pVtxBuffLine,
								NULL);

	VERTEX_3D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffLine->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	for (int nCntLine = 0; nCntLine < LINE_MAX; nCntLine++)
	{
		//���_�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aline[nCntLine].start.x, g_aline[nCntLine].start.y, g_aline[nCntLine].start.z);
		pVtx[1].pos = D3DXVECTOR3(g_aline[nCntLine].end.x, g_aline[nCntLine].end.y, g_aline[nCntLine].end.z);

		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[0].col = g_aline[nCntLine].col;
		pVtx[1].col = g_aline[nCntLine].col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 2;	//���_�f�[�^�̃|�C���^��2���i�߂�

	}
	//���_�f�[�^���A�����b�N
	g_pVtxBuffLine->Unlock();
}

//=============================================================================
// ���C���̍쐬
//=============================================================================
void SetLine(D3DXVECTOR3 pos,D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR col)
{
	VERTEX_3D *pVtx;

	for (int nCntLine = 0; nCntLine < LINE_MAX; nCntLine++)
	{
		if (!g_aline[nCntLine].bUse)
		{
			g_aline[nCntLine].pos = pos;
			g_aline[nCntLine].start = start;
			g_aline[nCntLine].end = end;
			g_aline[nCntLine].col = col;
			//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_pVtxBuffLine->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

			pVtx += nCntLine * 2;	//���_�f�[�^�̃|�C���^��2���i�߂�

			pVtx[0].pos = D3DXVECTOR3(g_aline[nCntLine].start.x, g_aline[nCntLine].start.y, g_aline[nCntLine].start.z);
			pVtx[1].pos = D3DXVECTOR3(g_aline[nCntLine].end.x, g_aline[nCntLine].end.y, g_aline[nCntLine].end.z);

			pVtx[0].col = g_aline[nCntLine].col;
			pVtx[1].col = g_aline[nCntLine].col;

			//���_�f�[�^���A�����b�N
			g_pVtxBuffLine->Unlock();

			g_aline[nCntLine].bUse = true;
			break;
		}
	}
}
//=============================================================================
// ���C���̍폜
//=============================================================================
void DeleteLine(void)
{
	for (int nCntLine = 0; nCntLine < LINE_MAX; nCntLine++)
	{
		if (g_aline[nCntLine].bUse)
		{
			g_aline[nCntLine].bUse = false;
		}
	}
}
