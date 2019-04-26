//=============================================================================
//
// �����L���O���� [rank.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include "rank.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	TEXTURE_RANK		"data/TEXTURE/number0.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_FIRST		"data/TEXTURE/1��.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_SECOND		"data/TEXTURE/2��.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_THIRD		"data/TEXTURE/3��.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

#define DIGIT_RANK			(8)		//�X�R�A�̌���
#define RANKING				(6)		//���ʐ�
#define ALL_RANK			(DIGIT_RANK * RANKING)	//�\������S���̌���
#define RANK_SIZE_X			(35)	//�����T�C�YX
#define RANK_SIZE_Y			(70)	//�����T�C�YY
#define RANK_PIC_SIZE_X		(105)	//�摜�T�C�YX
#define RANK_PIC_SIZE_Y		(126)	//�摜�T�C�YY

#define RANK_TEXTUREMAX		(4)		//�e�N�X�`����
#define	RANK_TOP			(3)

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexRank(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureRank[RANK_TEXTUREMAX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank = NULL;		// ���_�o�b�t�@�ւ̃|�C���^

int						g_aRank[RANKING] = {30000,25000,20000,15000,10000,0};			// �����N
D3DXVECTOR3				g_aRankpos[ALL_RANK];		// �����ʒu
D3DXVECTOR3				g_Picpos[RANK_TOP];	//�摜�ʒu

//=============================================================================
// ����������
//=============================================================================
void InitRank(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntAll=0;
	int nCntRank;

	for (int nCntRanking = 0; nCntRanking < RANKING - 1; nCntRanking++)
	{
		for (nCntRank = 0; nCntRank < DIGIT_RANK; nCntRank++, nCntAll++)
		{
			g_aRankpos[nCntAll] = D3DXVECTOR3(530.0f + nCntRank*40.0f, 200 + (nCntRanking  * 100.0f), 0.0f);

		}
	}
	for (nCntAll = (RANKING - 1) * DIGIT_RANK; nCntAll < ALL_RANK; nCntAll++)
	{
		g_aRankpos[nCntAll] = D3DXVECTOR3(530.0f + nCntRank*35.0f, 1000.0f, 0.0f);
	}

	for (int nCnt = 0; nCnt < RANK_TOP; nCnt++)
	{
		g_Picpos[nCnt] = D3DXVECTOR3(450.0f, 190 + (nCnt  * 105.0f), 0.0f);
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_RANK,					// �t�@�C���̖��O
		&g_pTextureRank[0]);			// �ǂݍ��ރ������[
	
	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FIRST,					// �t�@�C���̖��O
		&g_pTextureRank[1]);			// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_SECOND,					// �t�@�C���̖��O
		&g_pTextureRank[2]);			// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_THIRD,					// �t�@�C���̖��O
		&g_pTextureRank[3]);			// �ǂݍ��ރ������[


	// ���_���̍쐬
	MakeVertexRank(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitRank(void)
{
	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < RANK_TEXTUREMAX; nCnt++)
	{
		if (g_pTextureRank[nCnt] != NULL)
		{				  
			g_pTextureRank[nCnt]->Release();
			g_pTextureRank[nCnt] = NULL;
		}
	}
	
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffRank != NULL)
	{
		g_pVtxBuffRank->Release();
		g_pVtxBuffRank = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateRank(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawRank(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffRank, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureRank[0]);

	// �|���S���̕`��
	for (int nCntRank = 0; nCntRank < ALL_RANK-1; nCntRank++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
								nCntRank * 4,		//�J�n����C���f�b�N�X
								2);					//�v���~�e�B�u�̐�(�|���S���̐�)
	}

	for (int nCnt = 0; nCnt < RANK_TOP; nCnt++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureRank[nCnt+1]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			(ALL_RANK + nCnt) * 4,		//�J�n����C���f�b�N�X
			2);					//�v���~�e�B�u�̐�(�|���S���̐�)
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void MakeVertexRank(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (ALL_RANK+ RANK_TOP),	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
								D3DUSAGE_WRITEONLY,		//�Œ�
								FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,		//�Œ�
								&g_pVtxBuffRank,
								NULL);

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	for (int nCntRank = 0; nCntRank < ALL_RANK-1; nCntRank++)
	{
		// ���_���̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aRankpos[nCntRank].x - (RANK_SIZE_X / 2), g_aRankpos[nCntRank].y - (RANK_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
		pVtx[1].pos = D3DXVECTOR3(g_aRankpos[nCntRank].x + (RANK_SIZE_X / 2), g_aRankpos[nCntRank].y - (RANK_SIZE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aRankpos[nCntRank].x - (RANK_SIZE_X / 2), g_aRankpos[nCntRank].y + (RANK_SIZE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aRankpos[nCntRank].x + (RANK_SIZE_X / 2), g_aRankpos[nCntRank].y + (RANK_SIZE_Y / 2), 0.0f);

		pVtx[0].rhw = 1.0f;	//1.0f�ŌŒ�
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�

	for (int nCnt = 0; nCnt < RANK_TOP; nCnt++)
	{
		pVtx[0].pos = D3DXVECTOR3(g_Picpos[nCnt].x - (RANK_PIC_SIZE_X / 2), g_Picpos[nCnt].y - (RANK_PIC_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
		pVtx[1].pos = D3DXVECTOR3(g_Picpos[nCnt].x + (RANK_PIC_SIZE_X / 2), g_Picpos[nCnt].y - (RANK_PIC_SIZE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Picpos[nCnt].x - (RANK_PIC_SIZE_X / 2), g_Picpos[nCnt].y + (RANK_PIC_SIZE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Picpos[nCnt].x + (RANK_PIC_SIZE_X / 2), g_Picpos[nCnt].y + (RANK_PIC_SIZE_Y / 2), 0.0f);

		pVtx[0].rhw = 1.0f;	//1.0f�ŌŒ�
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�

	}
	//���_�f�[�^���A�����b�N
	g_pVtxBuffRank->Unlock();
}

//=============================================================================
// �X�R�A�̕ύX
//=============================================================================
void AddRank(int nScore)
{
	int nWork = 0, nDigit = 0, nCntAll = 0;
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	g_aRank[5] = nScore;

	for (int nNum01 = 0; nNum01 < RANKING; nNum01++)
	{
		for (int nNum02 = 0; nNum02 < (RANKING - (nNum01+1)); nNum02++)
		{
			if (g_aRank[nNum02] < g_aRank[nNum02 + 1])
			{
				nWork = g_aRank[nNum02];
				g_aRank[nNum02] = g_aRank[nNum02 + 1];
				g_aRank[nNum02 + 1] = nWork;
			}
		}
	}


	for (int nCntRanking = 0; nCntRanking < RANKING; nCntRanking++)
	{
		for (int nCntRank = 0; nCntRank < DIGIT_RANK; nCntRank++, nCntAll++)
		{
			nDigit = g_aRank[nCntRanking] % (int)powf(10.0f, DIGIT_RANK - nCntRank) / (int)powf(10.0f, DIGIT_RANK - (nCntRank + 1));

			//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

			pVtx += nCntAll * 4;	//���_�f�[�^�̃|�C���^��4���i�߂�

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.1f*nDigit, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f*nDigit), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f*nDigit, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f*nDigit), 1.0f);

			//���_�f�[�^���A�����b�N
			g_pVtxBuffRank->Unlock();
		}
	}
}