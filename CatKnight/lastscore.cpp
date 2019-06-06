//=============================================================================
//
// �X�R�A���� [lastscore.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include "lastscore.h"
#include"game.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	TEXTURE_LASTSCORE		"data/TEXTURE/number0.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define DIGIT_LASTSCORE				(8)		//�X�R�A�̌���
#define LASTSCORE_SIZE_X			(35.0f)	//�T�C�YX
#define LASTSCORE_SIZE_Y			(70.0f)	//�T�C�YY

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexLastScore(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureLastScore = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLastScore = NULL;		// ���_�o�b�t�@�ւ̃|�C���^

int						g_ScoreLast;					// �X�R�A
D3DXVECTOR3				g_aLastScorepos[DIGIT_LASTSCORE];	// �ʒu
bool					g_bScore;

//=============================================================================
// ����������
//=============================================================================
void InitLastScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntLastScore;

	g_ScoreLast = 0;
	for (nCntLastScore = 0; nCntLastScore < DIGIT_LASTSCORE; nCntLastScore++)
	{
		g_aLastScorepos[nCntLastScore] = D3DXVECTOR3(((SCREEN_WIDTH*0.7f) - (DIGIT_LASTSCORE * LASTSCORE_SIZE_X)) + (nCntLastScore * LASTSCORE_SIZE_X), 620.0f, 0.0f);
	}
	g_bScore = false;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
								TEXTURE_LASTSCORE,		// �t�@�C���̖��O
								&g_pTextureLastScore);	// �ǂݍ��ރ������[
	
	// ���_���̍쐬
	MakeVertexLastScore(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitLastScore(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureLastScore != NULL)
	{
		g_pTextureLastScore->Release();
		g_pTextureLastScore = NULL;
	}
	
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffLastScore != NULL)
	{
		g_pVtxBuffLastScore->Release();
		g_pVtxBuffLastScore = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateLastScore(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawLastScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntLastScore;

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffLastScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureLastScore);

	if (g_bScore)
	{
		// �|���S���̕`��
		for (nCntLastScore = 0; nCntLastScore < DIGIT_LASTSCORE; nCntLastScore++)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntLastScore * 4,		//�J�n����C���f�b�N�X
				2);					//�v���~�e�B�u�̐�(�|���S���̐�)
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void MakeVertexLastScore(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	int nCntLastScore;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * DIGIT_LASTSCORE,	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
								D3DUSAGE_WRITEONLY,		//�Œ�
								FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,		//�Œ�
								&g_pVtxBuffLastScore,
								NULL);

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffLastScore->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	for (nCntLastScore = 0; nCntLastScore < DIGIT_LASTSCORE; nCntLastScore++)
	{
		// ���_���̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aLastScorepos[nCntLastScore].x - (LASTSCORE_SIZE_X / 2), g_aLastScorepos[nCntLastScore].y - (LASTSCORE_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
		pVtx[1].pos = D3DXVECTOR3(g_aLastScorepos[nCntLastScore].x + (LASTSCORE_SIZE_X / 2), g_aLastScorepos[nCntLastScore].y - (LASTSCORE_SIZE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aLastScorepos[nCntLastScore].x - (LASTSCORE_SIZE_X / 2), g_aLastScorepos[nCntLastScore].y + (LASTSCORE_SIZE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aLastScorepos[nCntLastScore].x + (LASTSCORE_SIZE_X / 2), g_aLastScorepos[nCntLastScore].y + (LASTSCORE_SIZE_Y / 2), 0.0f);

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
	//���_�f�[�^���A�����b�N
	g_pVtxBuffLastScore->Unlock();
}

//=============================================================================
// �X�R�A�̕ύX
//=============================================================================
void AddLastScore(int nValue)
{
	int nCntLastScore;
	int nDigit;
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	g_ScoreLast += nValue;

	for (nCntLastScore = 0; nCntLastScore < DIGIT_LASTSCORE; nCntLastScore++)
	{
 		nDigit = g_ScoreLast % (int)powf(10.0f, DIGIT_LASTSCORE - nCntLastScore) / (int)powf(10.0f, DIGIT_LASTSCORE - (nCntLastScore + 1));

		//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffLastScore->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I
		
		pVtx += nCntLastScore * 4;	//���_�f�[�^�̃|�C���^��4���i�߂�

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.1f*nDigit, 0.0f);
 		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f*nDigit), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f*nDigit, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f*nDigit), 1.0f);

		//���_�f�[�^���A�����b�N
		g_pVtxBuffLastScore->Unlock();
	}
}

//=============================================================================
// �X�R�A�̕ύX
//=============================================================================
int	GetLastScore(void)
{
	return g_ScoreLast;
}

//=============================================================================
// �\�����
//=============================================================================
void SetLastScore(bool Set)
{
	g_bScore = Set;
}