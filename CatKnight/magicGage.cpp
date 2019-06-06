//=============================================================================
//
// ���@�Q�[�W���� [magicGage.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include "magicGage.h"
#include "game.h"
#include "sound.h"
#include "wave.h"
#include "magic.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define M_GAGE_MAX			(60 * 5)	//�ő�Mp

#define M_GAGE_POS_X		(200.0f)	//�ʒuX
#define M_GAGE_POS_Y		(630.0f)	//�ʒuY

#define M_GAGE_SIZE_X		(300)	//�T�C�YX
#define M_GAGE_SIZE_Y		(30)	//�T�C�YY
#define M_GAGE_TEXTUREMAX	(2)		//�e�N�X�`����

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexMagicGage(LPDIRECT3DDEVICE9 pDevice);


//=============================================================================
// �O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureMagicGage[M_GAGE_TEXTUREMAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMagicGage = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

MAGICGAGE				g_aMagicGage;	// Mp
D3DXCOLOR				g_aMagicColor[M_GAGE_TEXTUREMAX];

//=============================================================================
// ����������
//=============================================================================
void InitMagicGage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_aMagicGage.pos = D3DXVECTOR3(M_GAGE_POS_X, M_GAGE_POS_Y, 0.0f);
	g_aMagicGage.fNowMp = 0;

	g_aMagicColor[0] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	g_aMagicColor[1] = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`���̓ǂݍ���
	for (int nCnt = 0; nCnt < M_GAGE_TEXTUREMAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice,				
									NULL,
									&g_pTextureMagicGage[nCnt]);
	}

	// ���_���̍쐬
	MakeVertexMagicGage(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitMagicGage(void)
{
	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < M_GAGE_TEXTUREMAX; nCnt++)
	{
		if (g_pTextureMagicGage[nCnt] != NULL)
		{				  
			g_pTextureMagicGage[nCnt]->Release();
			g_pTextureMagicGage[nCnt] = NULL;
		}
	}
	
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffMagicGage != NULL)
	{
		g_pVtxBuffMagicGage->Release();
		g_pVtxBuffMagicGage = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMagicGage(void)
{
	VERTEX_2D *pVtx;

	if (g_aMagicGage.fNowMp >= M_GAGE_MAX)
	{	//�ő�ŉ��F
		g_aMagicColor[1] = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	}

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffMagicGage->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	pVtx[4].col = g_aMagicColor[1];
	pVtx[5].col = g_aMagicColor[1];
	pVtx[6].col = g_aMagicColor[1];
	pVtx[7].col = g_aMagicColor[1];

	//���_�f�[�^���A�����b�N
	g_pVtxBuffMagicGage->Unlock();

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMagicGage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffMagicGage, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	for (int nCnt = 0; nCnt < M_GAGE_TEXTUREMAX; nCnt++)
	{
		pDevice->SetTexture(0, g_pTextureMagicGage[nCnt]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			nCnt * 4,			//�J�n����C���f�b�N�X
			2);					//�v���~�e�B�u�̐�(�|���S���̐�)
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void MakeVertexMagicGage(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	int nCntMagicGage;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * M_GAGE_TEXTUREMAX,	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
								D3DUSAGE_WRITEONLY,		//�Œ�
								FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,		//�Œ�
								&g_pVtxBuffMagicGage,
								NULL);

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffMagicGage->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I
	
	pVtx[0].pos = D3DXVECTOR3(g_aMagicGage.pos.x, g_aMagicGage.pos.y, 0.0f);	//����0.0���ŌŒ�
	pVtx[1].pos = D3DXVECTOR3(g_aMagicGage.pos.x + M_GAGE_SIZE_X, g_aMagicGage.pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aMagicGage.pos.x, g_aMagicGage.pos.y + M_GAGE_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aMagicGage.pos.x + M_GAGE_SIZE_X, g_aMagicGage.pos.y + M_GAGE_SIZE_Y, 0.0f);

	pVtx[4].pos = D3DXVECTOR3(g_aMagicGage.pos.x, g_aMagicGage.pos.y, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_aMagicGage.pos.x + ((g_aMagicGage.fNowMp / M_GAGE_MAX)*M_GAGE_SIZE_X), g_aMagicGage.pos.y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_aMagicGage.pos.x, g_aMagicGage.pos.y + M_GAGE_SIZE_Y, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aMagicGage.pos.x + ((g_aMagicGage.fNowMp / M_GAGE_MAX)*M_GAGE_SIZE_X), g_aMagicGage.pos.y + M_GAGE_SIZE_Y, 0.0f);

	for (nCntMagicGage = 0; nCntMagicGage < M_GAGE_TEXTUREMAX; nCntMagicGage++)
	{
		pVtx[0].rhw = 1.0f;	//1.0f�ŌŒ�
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = g_aMagicColor[nCntMagicGage];
		pVtx[1].col = g_aMagicColor[nCntMagicGage];
		pVtx[2].col = g_aMagicColor[nCntMagicGage];
		pVtx[3].col = g_aMagicColor[nCntMagicGage];

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�f�[�^���A�����b�N
	g_pVtxBuffMagicGage->Unlock();
}

//=============================================================================
// Mp�̌���
//=============================================================================
void DecreaseMagicGage()
{
	g_aMagicGage.fNowMp -= M_GAGE_MAX;
	g_aMagicColor[1] = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffMagicGage->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	// ���_���̐ݒ�
	pVtx[4].pos = D3DXVECTOR3(g_aMagicGage.pos.x, g_aMagicGage.pos.y, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_aMagicGage.pos.x + ((g_aMagicGage.fNowMp / M_GAGE_MAX)*M_GAGE_SIZE_X), g_aMagicGage.pos.y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_aMagicGage.pos.x, g_aMagicGage.pos.y + M_GAGE_SIZE_Y, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aMagicGage.pos.x + ((g_aMagicGage.fNowMp / M_GAGE_MAX)*M_GAGE_SIZE_X), g_aMagicGage.pos.y + M_GAGE_SIZE_Y, 0.0f);

	pVtx[4].col = g_aMagicColor[1];
	pVtx[5].col = g_aMagicColor[1];
	pVtx[6].col = g_aMagicColor[1];
	pVtx[7].col = g_aMagicColor[1];

	//���_�f�[�^���A�����b�N
	g_pVtxBuffMagicGage->Unlock();

}

//=============================================================================
// Mp�̑���
//=============================================================================
void IncreaseMagicGage(int nValue)
{
	g_aMagicGage.fNowMp += nValue;

	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffMagicGage->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	// ���_���̐ݒ�
	pVtx[4].pos = D3DXVECTOR3(g_aMagicGage.pos.x, g_aMagicGage.pos.y, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_aMagicGage.pos.x + ((g_aMagicGage.fNowMp / M_GAGE_MAX)*M_GAGE_SIZE_X), g_aMagicGage.pos.y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_aMagicGage.pos.x, g_aMagicGage.pos.y + M_GAGE_SIZE_Y, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aMagicGage.pos.x + ((g_aMagicGage.fNowMp / M_GAGE_MAX)*M_GAGE_SIZE_X), g_aMagicGage.pos.y + M_GAGE_SIZE_Y, 0.0f);

	//���_�f�[�^���A�����b�N
	g_pVtxBuffMagicGage->Unlock();

	if (g_aMagicGage.fNowMp >= M_GAGE_MAX)
	{	//�`���[�W����
		MAGIC *pMagic = GetMagic();
		WAVE *pWave = GetWave();
		pWave->state = WAVE_STATE_CHARGEMAX;
		pWave->col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pMagic->col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		PlaySound(SOUND_LABEL_SE_CHARGEMAX);
	}
}

//=============================================================================
// Mp�̎擾
//=============================================================================
float GetMagicGage(void)
{
	return g_aMagicGage.fNowMp;
}