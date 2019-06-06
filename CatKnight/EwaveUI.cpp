//=============================================================================
//
// �G�gUI���� [EwaveUI.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include "EwaveUI.h"
#include "enemywave.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	TEXTURE_EWAVE_NUM	"data/TEXTURE/number0.png"		//����
#define	TEXTURE_EWAVE_WAVE	"data/TEXTURE/wave.png"			//wave
#define	TEXTURE_EWAVE_SLASH	"data/TEXTURE/slash.png"		//��
#define	TEXTURE_EWAVE_CLEAR	"data/TEXTURE/waveClear2.png"	//�N���A�\
#define	TEXTURE_EWAVE_START	"data/TEXTURE/nextwave.png"		//�l�N�X�g

#define E_WAVE_SIZE_X		(35.0f)		//�����T�C�YX
#define E_WAVE_SIZE_Y		(70.0f)		//�����T�C�YY
#define E_WAVEPIC_SIZE_X	(105.0f)	//�T�C�YX
#define E_WAVEPIC_SIZE_Y	(70.0f)		//�T�C�YY
#define E_WAVEINFO_SIZE_X	(675.0f)	//�T�C�YX
#define E_WAVEINFO_SIZE_Y	(150.0f)	//�T�C�YY

#define E_WAVE_TEXTURE_MAX	(5)			//�e�N�X�`����

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexEwaveUI(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureEwaveUI[E_WAVE_TEXTURE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEwaveUI = NULL;			// ���_�o�b�t�@�ւ̃|�C���^

int		g_nEwaveUI[DIGIT_E_WAVE];			// �G�g
EwaveUI	g_EwavePic;	//�G
bool	bResult;	//���U���g���

//=============================================================================
// ����������
//=============================================================================
void InitEwaveUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEwaveUI;

	g_nEwaveUI[0] = 1;
	g_nEwaveUI[1] = E_WAVE_MAX;

	for (nCntEwaveUI = 0; nCntEwaveUI < DIGIT_E_WAVE; nCntEwaveUI++)
	{
		g_EwavePic.Numpos[nCntEwaveUI] = D3DXVECTOR3(SCREEN_WIDTH - ((DIGIT_E_WAVE+1) * E_WAVE_SIZE_X) + (nCntEwaveUI * (E_WAVE_SIZE_X*2)), 180.0f, 0.0f);
	}

	g_EwavePic.Picpos[0] = D3DXVECTOR3(SCREEN_WIDTH - ((DIGIT_E_WAVE + 3) * E_WAVE_SIZE_X), 180.0f, 0.0f);
	g_EwavePic.Picpos[3] = D3DXVECTOR3(SCREEN_WIDTH - E_WAVE_SIZE_X * DIGIT_E_WAVE, 180.0f, 0.0f);
	g_EwavePic.Picpos[1] = D3DXVECTOR3(640.0f, 350.0f, 0.0f);
	g_EwavePic.Picpos[2] = D3DXVECTOR3(640.0f, 350.0f, 0.0f);
	g_EwavePic.bUse[0] = true;
	g_EwavePic.bUse[1] = false;
	g_EwavePic.bUse[2] = false;
	g_EwavePic.bUse[3] = true;
	bResult = true;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_EWAVE_NUM,					// �t�@�C���̖��O
		&g_pTextureEwaveUI[0]);			// �ǂݍ��ރ������[
	
	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_EWAVE_WAVE,				// �t�@�C���̖��O
		&g_pTextureEwaveUI[1]);			// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_EWAVE_CLEAR,			// �t�@�C���̖��O
		&g_pTextureEwaveUI[2]);			// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_EWAVE_START,			// �t�@�C���̖��O
		&g_pTextureEwaveUI[3]);			// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_EWAVE_SLASH,			// �t�@�C���̖��O
		&g_pTextureEwaveUI[4]);			// �ǂݍ��ރ������[

	// ���_���̍쐬
	MakeVertexEwaveUI(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitEwaveUI(void)
{
	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < E_WAVE_TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureEwaveUI[nCnt] != NULL)
		{				  
			g_pTextureEwaveUI[nCnt]->Release();
			g_pTextureEwaveUI[nCnt] = NULL;
		}
	}
	
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffEwaveUI != NULL)
	{
		g_pVtxBuffEwaveUI->Release();
		g_pVtxBuffEwaveUI = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEwaveUI(void)
{
	int g_nEwaveNum = *GetEwave();

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffEwaveUI->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.1f * g_nEwaveNum, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_nEwaveNum), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f * g_nEwaveNum, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_nEwaveNum), 1.0f);

	//���_�f�[�^���A�����b�N
	g_pVtxBuffEwaveUI->Unlock();

	if (*GetMode() == MODE_RESULT)
	{
		for (int nCntEwaveUI = 0; nCntEwaveUI < DIGIT_E_WAVE; nCntEwaveUI++)
		{
			g_EwavePic.Numpos[nCntEwaveUI] = D3DXVECTOR3((SCREEN_WIDTH*0.7f) - ((DIGIT_E_WAVE + 1) * E_WAVE_SIZE_X) + (nCntEwaveUI * (E_WAVE_SIZE_X * 2)), 420.0f, 0.0f);
		}
		g_EwavePic.Picpos[0] = D3DXVECTOR3((SCREEN_WIDTH*0.7f) - ((DIGIT_E_WAVE + 3) * E_WAVE_SIZE_X), 420.0f, 0.0f);
		g_EwavePic.Picpos[3] = D3DXVECTOR3((SCREEN_WIDTH*0.7f) - E_WAVE_SIZE_X * DIGIT_E_WAVE, 420.0f, 0.0f);


		//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffEwaveUI->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

		for (int nCntEwaveUI = 0; nCntEwaveUI < DIGIT_E_WAVE; nCntEwaveUI++)
		{	//����
			// ���_���̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_EwavePic.Numpos[nCntEwaveUI].x - (E_WAVE_SIZE_X / 2), g_EwavePic.Numpos[nCntEwaveUI].y - (E_WAVE_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
			pVtx[1].pos = D3DXVECTOR3(g_EwavePic.Numpos[nCntEwaveUI].x + (E_WAVE_SIZE_X / 2), g_EwavePic.Numpos[nCntEwaveUI].y - (E_WAVE_SIZE_Y / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_EwavePic.Numpos[nCntEwaveUI].x - (E_WAVE_SIZE_X / 2), g_EwavePic.Numpos[nCntEwaveUI].y + (E_WAVE_SIZE_Y / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_EwavePic.Numpos[nCntEwaveUI].x + (E_WAVE_SIZE_X / 2), g_EwavePic.Numpos[nCntEwaveUI].y + (E_WAVE_SIZE_Y / 2), 0.0f);

			pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
		}

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			// ���_���̐ݒ�
			if (nCnt >= 3)
			{	//�ȂȂ�
				pVtx[0].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x - (E_WAVE_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y - (E_WAVE_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
				pVtx[1].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x + (E_WAVE_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y - (E_WAVE_SIZE_Y / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x - (E_WAVE_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y + (E_WAVE_SIZE_Y / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x + (E_WAVE_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y + (E_WAVE_SIZE_Y / 2), 0.0f);

			}
			else if (nCnt < 1)
			{	//wave
				pVtx[0].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x - (E_WAVEPIC_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y - (E_WAVEPIC_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
				pVtx[1].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x + (E_WAVEPIC_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y - (E_WAVEPIC_SIZE_Y / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x - (E_WAVEPIC_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y + (E_WAVEPIC_SIZE_Y / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x + (E_WAVEPIC_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y + (E_WAVEPIC_SIZE_Y / 2), 0.0f);
			}
			pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
		}

		//���_�f�[�^���A�����b�N
		g_pVtxBuffEwaveUI->Unlock();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEwaveUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEwaveUI;

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffEwaveUI, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureEwaveUI[0]);
	if (bResult)
	{
		// �|���S���̕`��
		for (nCntEwaveUI = 0; nCntEwaveUI < DIGIT_E_WAVE; nCntEwaveUI++)
		{	//����
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntEwaveUI * 4,		//�J�n����C���f�b�N�X
				2);					//�v���~�e�B�u�̐�(�|���S���̐�)
		}

		for (int nCnt = 0; nCnt < E_WAVE_TEXTURE; nCnt++)
		{	//����ȊO
			if (g_EwavePic.bUse[nCnt])
			{
				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureEwaveUI[nCnt + 1]);

				// �|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
					(DIGIT_E_WAVE + nCnt) * 4,		//�J�n����C���f�b�N�X
					2);					//�v���~�e�B�u�̐�(�|���S���̐�)
			}
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void MakeVertexEwaveUI(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	int nCntEwaveUI;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (DIGIT_E_WAVE + E_WAVE_TEXTURE),	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
								D3DUSAGE_WRITEONLY,		//�Œ�
								FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,		//�Œ�
								&g_pVtxBuffEwaveUI,
								NULL);

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffEwaveUI->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	for (nCntEwaveUI = 0; nCntEwaveUI < DIGIT_E_WAVE; nCntEwaveUI++)
	{	//����
		// ���_���̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_EwavePic.Numpos[nCntEwaveUI].x - (E_WAVE_SIZE_X / 2), g_EwavePic.Numpos[nCntEwaveUI].y - (E_WAVE_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
		pVtx[1].pos = D3DXVECTOR3(g_EwavePic.Numpos[nCntEwaveUI].x + (E_WAVE_SIZE_X / 2), g_EwavePic.Numpos[nCntEwaveUI].y - (E_WAVE_SIZE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_EwavePic.Numpos[nCntEwaveUI].x - (E_WAVE_SIZE_X / 2), g_EwavePic.Numpos[nCntEwaveUI].y + (E_WAVE_SIZE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_EwavePic.Numpos[nCntEwaveUI].x + (E_WAVE_SIZE_X / 2), g_EwavePic.Numpos[nCntEwaveUI].y + (E_WAVE_SIZE_Y / 2), 0.0f);

		pVtx[0].rhw = 1.0f;	//1.0f�ŌŒ�
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.1f * g_nEwaveUI[nCntEwaveUI], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_nEwaveUI[nCntEwaveUI]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * g_nEwaveUI[nCntEwaveUI], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_nEwaveUI[nCntEwaveUI]), 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	for (int nCnt = 0; nCnt < E_WAVE_TEXTURE; nCnt++)
	{
		// ���_���̐ݒ�
		if (nCnt >= 3)
		{	//�ȂȂ�
			pVtx[0].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x - (E_WAVE_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y - (E_WAVE_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
			pVtx[1].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x + (E_WAVE_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y - (E_WAVE_SIZE_Y / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x - (E_WAVE_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y + (E_WAVE_SIZE_Y / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x + (E_WAVE_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y + (E_WAVE_SIZE_Y / 2), 0.0f);

		}
		else if (nCnt < 1)
		{	//wave
			pVtx[0].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x - (E_WAVEPIC_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y - (E_WAVEPIC_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
			pVtx[1].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x + (E_WAVEPIC_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y - (E_WAVEPIC_SIZE_Y / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x - (E_WAVEPIC_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y + (E_WAVEPIC_SIZE_Y / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x + (E_WAVEPIC_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y + (E_WAVEPIC_SIZE_Y / 2), 0.0f);
		}
		else
		{	//waveClear��nextwave
			pVtx[0].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x - (E_WAVEINFO_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y - (E_WAVEINFO_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
			pVtx[1].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x + (E_WAVEINFO_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y - (E_WAVEINFO_SIZE_Y / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x - (E_WAVEINFO_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y + (E_WAVEINFO_SIZE_Y / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_EwavePic.Picpos[nCnt].x + (E_WAVEINFO_SIZE_X / 2), g_EwavePic.Picpos[nCnt].y + (E_WAVEINFO_SIZE_Y / 2), 0.0f);
		}

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
	g_pVtxBuffEwaveUI->Unlock();
}

//=============================================================================
// �G�g�̕ύX
//=============================================================================
EwaveUI  *GetEwaveUI(void)
{
	return &g_EwavePic;
}

//=============================================================================
// �\�����
//=============================================================================
void SetEwaveUIresult(bool Set)
{
	bResult = Set;
}
