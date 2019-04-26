//=============================================================================
//
// �R���{���� [combo.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include "combo.h"
#include"game.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	TEXTURE_COMBO		"data/TEXTURE/number0.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_HIT			"data/TEXTURE/hit2.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define DIGIT_COMBO			(2)		//�R���{�̌���
#define COMBO_SIZE_X		(35.0f)	//�����T�C�YX
#define COMBO_SIZE_Y		(70.0f)	//�����T�C�YY
#define HIT_SIZE_X			(75.0f)	//�T�C�YX
#define HIT_SIZE_Y			(50.0f)	//�T�C�YY
#define TEXTURE_COMBO_MAX	(2)		//�e�N�X�`����

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexCombo(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureCombo[TEXTURE_COMBO_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCombo = NULL;		// ���_�o�b�t�@�ւ̃|�C���^

int						g_nCombo;					// �R���{
int						g_nMaxCombo;				//�ő�R���{
D3DXVECTOR3				g_aCombopos[DIGIT_COMBO+1];	// �ʒu
bool					g_bResultCombo;

//=============================================================================
// ����������
//=============================================================================
void InitCombo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntCombo;

	g_nCombo = 0;
	g_nMaxCombo = 0;
	for (nCntCombo = 0; nCntCombo < DIGIT_COMBO; nCntCombo++)
	{
		g_aCombopos[nCntCombo] = D3DXVECTOR3((SCREEN_WIDTH - COMBO_SIZE_X * DIGIT_COMBO * 2) + (nCntCombo * COMBO_SIZE_X), 110.0f, 0.0f);
	}
	g_aCombopos[2] = D3DXVECTOR3(SCREEN_WIDTH - COMBO_SIZE_X * 1.3f, 110.0f, 0.0f);
	g_bResultCombo = true;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
								TEXTURE_COMBO,		// �t�@�C���̖��O
								&g_pTextureCombo[0]);	// �ǂݍ��ރ������[
	
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_HIT,		// �t�@�C���̖��O
		&g_pTextureCombo[1]);	// �ǂݍ��ރ������[

	// ���_���̍쐬
	MakeVertexCombo(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitCombo(void)
{
	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < TEXTURE_COMBO_MAX; nCnt++)
	{
		if (g_pTextureCombo[nCnt] != NULL)
		{				   
			g_pTextureCombo[nCnt]->Release();
			g_pTextureCombo[nCnt] = NULL;
		}
	}
	
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffCombo != NULL)
	{
		g_pVtxBuffCombo->Release();
		g_pVtxBuffCombo = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCombo(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	int nCntCombo;

	if (*GetMode() == MODE_RESULT)
	{
		for (int nCntCombo = 0; nCntCombo < DIGIT_COMBO; nCntCombo++)
		{
			g_aCombopos[nCntCombo] = D3DXVECTOR3(((SCREEN_WIDTH*0.76f) - COMBO_SIZE_X * DIGIT_COMBO * 2) + (nCntCombo * COMBO_SIZE_X), 320.0f, 0.0f);
		}
		g_aCombopos[2] = D3DXVECTOR3((SCREEN_WIDTH*0.76f) - COMBO_SIZE_X * 1.3f, 320.0f, 0.0f);

		//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

		for (nCntCombo = 0; nCntCombo < DIGIT_COMBO; nCntCombo++)
		{
			// ���_���̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x - (COMBO_SIZE_X / 2), g_aCombopos[nCntCombo].y - (COMBO_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
			pVtx[1].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x + (COMBO_SIZE_X / 2), g_aCombopos[nCntCombo].y - (COMBO_SIZE_Y / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x - (COMBO_SIZE_X / 2), g_aCombopos[nCntCombo].y + (COMBO_SIZE_Y / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x + (COMBO_SIZE_X / 2), g_aCombopos[nCntCombo].y + (COMBO_SIZE_Y / 2), 0.0f);
			pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�

		}
		pVtx[0].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x - (HIT_SIZE_X / 2), g_aCombopos[nCntCombo].y - (HIT_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
		pVtx[1].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x + (HIT_SIZE_X / 2), g_aCombopos[nCntCombo].y - (HIT_SIZE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x - (HIT_SIZE_X / 2), g_aCombopos[nCntCombo].y + (HIT_SIZE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x + (HIT_SIZE_X / 2), g_aCombopos[nCntCombo].y + (HIT_SIZE_Y / 2), 0.0f);
		
		//���_�f�[�^���A�����b�N
		g_pVtxBuffCombo->Unlock();

	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawCombo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntCombo;

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffCombo, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureCombo[0]);

	if (g_bResultCombo)
	{
		// �|���S���̕`��
		for (nCntCombo = 0; nCntCombo < DIGIT_COMBO; nCntCombo++)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntCombo * 4,		//�J�n����C���f�b�N�X
				2);					//�v���~�e�B�u�̐�(�|���S���̐�)
		}

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureCombo[1]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			DIGIT_COMBO * 4,		//�J�n����C���f�b�N�X
			2);					//�v���~�e�B�u�̐�(�|���S���̐�)
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void MakeVertexCombo(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	int nCntCombo;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (DIGIT_COMBO + 1),	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
								D3DUSAGE_WRITEONLY,		//�Œ�
								FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,		//�Œ�
								&g_pVtxBuffCombo,
								NULL);

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	for (nCntCombo = 0; nCntCombo < DIGIT_COMBO; nCntCombo++)
	{
		// ���_���̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x - (COMBO_SIZE_X / 2), g_aCombopos[nCntCombo].y - (COMBO_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
		pVtx[1].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x + (COMBO_SIZE_X / 2), g_aCombopos[nCntCombo].y - (COMBO_SIZE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x - (COMBO_SIZE_X / 2), g_aCombopos[nCntCombo].y + (COMBO_SIZE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x + (COMBO_SIZE_X / 2), g_aCombopos[nCntCombo].y + (COMBO_SIZE_Y / 2), 0.0f);

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

	// ���_���̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x - (HIT_SIZE_X / 2), g_aCombopos[nCntCombo].y - (HIT_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
	pVtx[1].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x + (HIT_SIZE_X / 2), g_aCombopos[nCntCombo].y - (HIT_SIZE_Y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x - (HIT_SIZE_X / 2), g_aCombopos[nCntCombo].y + (HIT_SIZE_Y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aCombopos[nCntCombo].x + (HIT_SIZE_X / 2), g_aCombopos[nCntCombo].y + (HIT_SIZE_Y / 2), 0.0f);

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

	//���_�f�[�^���A�����b�N
	g_pVtxBuffCombo->Unlock();
}

//=============================================================================
// �R���{�̕ύX
//=============================================================================
void AddCombo(int nValue)
{
	int nDigit;
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	g_nCombo += nValue;

	if (g_nCombo > g_nMaxCombo)
	{
		g_nMaxCombo = g_nCombo;	//�ő�R���{�ۑ�
	}

	for (int nCntCombo = 0; nCntCombo < DIGIT_COMBO; nCntCombo++)
	{
 		nDigit = g_nCombo % (int)powf(10.0f, DIGIT_COMBO - nCntCombo) / (int)powf(10.0f, DIGIT_COMBO - (nCntCombo + 1));

		//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I
		
		pVtx += nCntCombo * 4;	//���_�f�[�^�̃|�C���^��4���i�߂�

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.1f*nDigit, 0.0f);
 		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f*nDigit), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f*nDigit, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f*nDigit), 1.0f);

		//���_�f�[�^���A�����b�N
		g_pVtxBuffCombo->Unlock();
	}
}

//=============================================================================
// �R���{�̌���
//=============================================================================
void DeleteCombo(void)
{
	int nDigit;
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	if (g_nCombo > g_nMaxCombo)
	{
		g_nMaxCombo = g_nCombo;	//�ő�R���{�ۑ�
	}

	g_nCombo = 0;

	for (int nCntCombo = 0; nCntCombo < DIGIT_COMBO; nCntCombo++)
	{
		nDigit = g_nCombo % (int)powf(10.0f, DIGIT_COMBO - nCntCombo) / (int)powf(10.0f, DIGIT_COMBO - (nCntCombo + 1));

		//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

		pVtx += nCntCombo * 4;	//���_�f�[�^�̃|�C���^��4���i�߂�

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.1f*nDigit, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f*nDigit), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f*nDigit, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f*nDigit), 1.0f);

		//���_�f�[�^���A�����b�N
		g_pVtxBuffCombo->Unlock();
	}

}

//=============================================================================
// �R���{�̌���
//=============================================================================
int	GetCombo(void)
{
	return g_nMaxCombo;
}

//=============================================================================
// �\�����
//=============================================================================
void SetCombo(bool Set)
{
	g_bResultCombo = Set;
}