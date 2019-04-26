//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include "score.h"
#include"game.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	TEXTURE_SCORE		"data/TEXTURE/number0.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define DIGIT_SCORE				(8)		//�X�R�A�̌���
#define SCORE_SIZE_X			(35.0f)	//�T�C�YX
#define SCORE_SIZE_Y			(70.0f)	//�T�C�YY

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureScore = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		// ���_�o�b�t�@�ւ̃|�C���^

int						g_nScore;					// �X�R�A
D3DXVECTOR3				g_aScorepos[DIGIT_SCORE];	// �ʒu
bool					g_bResult;

//=============================================================================
// ����������
//=============================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntScore;

	g_nScore = 0;
	for (nCntScore = 0; nCntScore < DIGIT_SCORE; nCntScore++)
	{
		g_aScorepos[nCntScore] = D3DXVECTOR3((SCREEN_WIDTH - (DIGIT_SCORE * SCORE_SIZE_X)) + (nCntScore * SCORE_SIZE_X), 40.0f, 0.0f);
	}
	g_bResult = true;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
								TEXTURE_SCORE,		// �t�@�C���̖��O
								&g_pTextureScore);	// �ǂݍ��ރ������[
	
	// ���_���̍쐬
	MakeVertexScore(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitScore(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
	
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateScore(void)
{
	if (*GetMode() == MODE_RESULT)
	{
		for (int nCntScore = 0; nCntScore < DIGIT_SCORE; nCntScore++)
		{
			g_aScorepos[nCntScore] = D3DXVECTOR3(((SCREEN_WIDTH*0.7f) - (DIGIT_SCORE * SCORE_SIZE_X)) + (nCntScore * SCORE_SIZE_X), 220.0f, 0.0f);
		}

		VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

		//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

		for (int nCntScore = 0; nCntScore < DIGIT_SCORE; nCntScore++)
		{
			// ���_���̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aScorepos[nCntScore].x - (SCORE_SIZE_X / 2), g_aScorepos[nCntScore].y - (SCORE_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
			pVtx[1].pos = D3DXVECTOR3(g_aScorepos[nCntScore].x + (SCORE_SIZE_X / 2), g_aScorepos[nCntScore].y - (SCORE_SIZE_Y / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aScorepos[nCntScore].x - (SCORE_SIZE_X / 2), g_aScorepos[nCntScore].y + (SCORE_SIZE_Y / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aScorepos[nCntScore].x + (SCORE_SIZE_X / 2), g_aScorepos[nCntScore].y + (SCORE_SIZE_Y / 2), 0.0f);

			pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
		}

		//���_�f�[�^���A�����b�N
		g_pVtxBuffScore->Unlock();

	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntScore;

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureScore);

	if (g_bResult)
	{
		// �|���S���̕`��
		for (nCntScore = 0; nCntScore < DIGIT_SCORE; nCntScore++)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntScore * 4,		//�J�n����C���f�b�N�X
				2);					//�v���~�e�B�u�̐�(�|���S���̐�)
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	int nCntScore;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * DIGIT_SCORE,	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
								D3DUSAGE_WRITEONLY,		//�Œ�
								FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,		//�Œ�
								&g_pVtxBuffScore,
								NULL);

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	for (nCntScore = 0; nCntScore < DIGIT_SCORE; nCntScore++)
	{
		// ���_���̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aScorepos[nCntScore].x - (SCORE_SIZE_X / 2), g_aScorepos[nCntScore].y - (SCORE_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
		pVtx[1].pos = D3DXVECTOR3(g_aScorepos[nCntScore].x + (SCORE_SIZE_X / 2), g_aScorepos[nCntScore].y - (SCORE_SIZE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aScorepos[nCntScore].x - (SCORE_SIZE_X / 2), g_aScorepos[nCntScore].y + (SCORE_SIZE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aScorepos[nCntScore].x + (SCORE_SIZE_X / 2), g_aScorepos[nCntScore].y + (SCORE_SIZE_Y / 2), 0.0f);

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
	g_pVtxBuffScore->Unlock();
}

//=============================================================================
// �X�R�A�̕ύX
//=============================================================================
void AddScore(int nValue)
{
	int nCntScore;
	int nDigit;
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	g_nScore += nValue;

	for (nCntScore = 0; nCntScore < DIGIT_SCORE; nCntScore++)
	{
 		nDigit = g_nScore % (int)powf(10.0f, DIGIT_SCORE - nCntScore) / (int)powf(10.0f, DIGIT_SCORE - (nCntScore + 1));

		//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I
		
		pVtx += nCntScore * 4;	//���_�f�[�^�̃|�C���^��4���i�߂�

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.1f*nDigit, 0.0f);
 		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f*nDigit), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f*nDigit, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f*nDigit), 1.0f);

		//���_�f�[�^���A�����b�N
		g_pVtxBuffScore->Unlock();
	}
}

//=============================================================================
// �X�R�A�̎擾
//=============================================================================
int	GetScore(void)
{
	return g_nScore;
}

//=============================================================================
// �\�����
//=============================================================================
void SetScore(bool Set)
{
	g_bResult = Set;
}
