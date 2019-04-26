//=============================================================================
//
// �c�胉�C�t���� [life.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include "life.h"
#include "player.h"	

//=============================================================================
// �}�N����`
//=============================================================================
#define	TEXTURE_LIFE		"data/TEXTURE/life.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define DIGIT_LIFE			(10)					//�ő僉�C�t
#define LIFE_SIZE_X			(50)	//�T�C�YX
#define LIFE_SIZE_Y			(50)	//�T�C�YY

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexLife(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureLife = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

PLAYER *pPlayer;
LIFE					g_aLife[DIGIT_LIFE];	// ���C�t
int						g_nLife;				//���̃��C�t

//=============================================================================
// ����������
//=============================================================================
void InitLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntLife;
	pPlayer = GetPlayer();

	g_nLife = pPlayer->nMaxLife;

	for (nCntLife = 0; nCntLife < g_nLife; nCntLife++)
	{
		g_aLife[nCntLife].pos = D3DXVECTOR3(30.0f + nCntLife * LIFE_SIZE_X, 30.0f, 0.0f);
		g_aLife[nCntLife].bUse = true;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
								TEXTURE_LIFE,		// �t�@�C���̖��O
								&g_pTextureLife);	// �ǂݍ��ރ������[
	
	// ���_���̍쐬
	MakeVertexLife(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitLife(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}
	
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateLife(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntLife;

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffLife, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureLife);

	// �|���S���̕`��
	for (nCntLife = 0; nCntLife < DIGIT_LIFE; nCntLife++)
	{
		if (g_aLife[nCntLife].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
									nCntLife * 4,		//�J�n����C���f�b�N�X
									2);					//�v���~�e�B�u�̐�(�|���S���̐�)
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void MakeVertexLife(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	int nCntLife;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * DIGIT_LIFE,	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
								D3DUSAGE_WRITEONLY,		//�Œ�
								FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,		//�Œ�
								&g_pVtxBuffLife,
								NULL);

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	for (nCntLife = 0; nCntLife < DIGIT_LIFE; nCntLife++)
	{
		// ���_���̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aLife[nCntLife].pos.x - (LIFE_SIZE_X / 2), g_aLife[nCntLife].pos.y - (LIFE_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
		pVtx[1].pos = D3DXVECTOR3(g_aLife[nCntLife].pos.x + (LIFE_SIZE_X / 2), g_aLife[nCntLife].pos.y - (LIFE_SIZE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aLife[nCntLife].pos.x - (LIFE_SIZE_X / 2), g_aLife[nCntLife].pos.y + (LIFE_SIZE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aLife[nCntLife].pos.x + (LIFE_SIZE_X / 2), g_aLife[nCntLife].pos.y + (LIFE_SIZE_Y / 2), 0.0f);

		pVtx[0].rhw = 1.0f;	//1.0f�ŌŒ�
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�f�[�^���A�����b�N
	g_pVtxBuffLife->Unlock();
}

//=============================================================================
// ���C�t�̌���
//=============================================================================
void DecreaseLife(int nValue)
{
	pPlayer = GetPlayer();

	if (g_nLife > 0)
	{
		for (int nCntLife = g_nLife-1; nCntLife > (g_nLife-1) - nValue; nCntLife--)
		{
			g_aLife[nCntLife].bUse = false;
		}
	}

	g_nLife -= nValue;
	pPlayer->nLife = g_nLife;
}

//=============================================================================
// ���C�t�̑���
//=============================================================================
void IncreaseLife(int nlife)
{
	int nCntLife;

	for (nCntLife = 0; nCntLife < nlife; nCntLife++)
	{
		if (g_aLife[nCntLife].bUse == false)
		{
			g_aLife[nCntLife].bUse = true;
		}
	}

	g_nLife = nlife;
}
