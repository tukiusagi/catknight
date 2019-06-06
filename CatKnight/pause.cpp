//=============================================================================
//
// �|�[�Y����[pause.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"pause.h"	//�C���N���[�h�t�@�C��
#include "input.h"	
#include "joystick.h"
#include "xController.h"

#include "fade.h"
#include"game.h"
#include "sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define PAUSE_CONTINUE	"data/TEXTURE/continue.png"	//�ǂݍ��ރe�N�X�`����
#define PAUSE_RETRY	"data/TEXTURE/retry.png"	
#define PAUSE_QUIT	"data/TEXTURE/backtitle.png"	

#define PAUSE_POS_X	(0)			//�|�[�Y�ʒuX
#define PAUSE_POS_Y	(0)			//�|�[�Y�ʒuY
#define PAUSE_SIZE_X	(600)	//�|�[�Y�T�C�YX
#define PAUSE_SIZE_Y	(200)	//�|�[�Y�T�C�YY
#define PAUSE_TEXTURE	(3)			//�|�[�Y�ʒuX

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexPause(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTexturePause[PAUSE_TEXTURE] = {};			//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;	//���_�o�b�t�@�ւ̃|�C���^
PAUSESTATE			g_PauseState;
PAUSE_COLORSTATE	g_PauseColor;
D3DXVECTOR3			g_posPause;	//�w�i�ʒu
int g_nSelectPause;

//=============================================================================
// �|�[�Y�̏���������
//=============================================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();	//�f�o�C�X�̎擾

	g_posPause = D3DXVECTOR3(640, 200, 0);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
							PAUSE_CONTINUE,
							&g_pTexturePause[0]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
								PAUSE_RETRY,
								&g_pTexturePause[1]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
								PAUSE_QUIT,
								&g_pTexturePause[2]);

	MakeVertexPause(pDevice);

	g_posPause = D3DXVECTOR3(0, 0, 0);
	g_PauseColor = PAUSE_COLORSTATE_NONE;
	g_PauseState = PAUSESTATE_NONE;
	g_nSelectPause = 0;
}

//=============================================================================
// �|�[�Y�̏I������
//=============================================================================
void UninitPause(void)
{
	for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE; nCntPause++)
	{
		//�e�N�X�`���̊J��
		if (g_pTexturePause[nCntPause] != NULL)
		{				  
			g_pTexturePause[nCntPause]->Release();
			g_pTexturePause[nCntPause] = NULL;
		}
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//=============================================================================
// �|�[�Y�̍X�V����
//=============================================================================
void UpdatePause(void)
{
	if ((GetKeyTrigger(DIK_UP) || GetJoystickTrigger(PAD_UP_L) || GetxControllerTrigger(XINPUT_PAD_UP_L)))
	{
		g_nSelectPause = (g_nSelectPause + 2) % 3;
		PlaySound(SOUND_LABEL_SE_PAUSE);

	}
	if ((GetKeyTrigger(DIK_DOWN) || GetJoystickTrigger(PAD_DOWN_L) || GetxControllerTrigger(XINPUT_PAD_DOWN_L)))
	{
		g_nSelectPause = (g_nSelectPause + 1) % 3;
		PlaySound(SOUND_LABEL_SE_PAUSE);
	}

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	switch (g_nSelectPause)
	{
	case 0:
		g_PauseColor = PAUSE_COLORSTATE_CONTINUE;
		if ((GetKeyTrigger(DIK_RETURN) || GetJoystickTrigger(PAD_B) || GetxControllerTrigger(XINPUT_PAD_B)))
		{
			g_PauseState = PAUSESTATE_CONTINUE;
			PlaySound(SOUND_LABEL_SE_ENTER);
		}

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

		pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

		break;
	case 1:
		g_PauseColor = PAUSE_COLORSTATE_RETRY;
		if ((GetKeyTrigger(DIK_RETURN) || GetJoystickTrigger(PAD_B) || GetxControllerTrigger(XINPUT_PAD_B)))
		{
			g_PauseState = PAUSESTATE_RETRY;
			PlaySound(SOUND_LABEL_SE_ENTER);
		}

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

		pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

		break;
	case 2:
		g_PauseColor = PAUSE_COLORSTATE_QUIT;
		if ((GetKeyTrigger(DIK_RETURN) || GetJoystickTrigger(PAD_B) || GetxControllerTrigger(XINPUT_PAD_B)))
		{
			g_PauseState = PAUSESTATE_QUIT;
			PlaySound(SOUND_LABEL_SE_ENTER);
		}

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

		pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

		pVtx[8].col	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		break;
	default:
		break;
	}
	//���_�f�[�^���A�����b�N
	g_pVtxBuffPause->Unlock();

	//��ʑJ��
	switch (g_PauseState)
	{
	case PAUSESTATE_CONTINUE:
		if (*GetFade() == FADE_NONE)
		{
			SetGameState(GAMESTATE_NORMAL);
			g_PauseState = PAUSESTATE_NONE;
		}
		break;
	case PAUSESTATE_RETRY:
		if (*GetFade() == FADE_NONE)
		{
			SetFade(MODE_GAME);
			g_PauseState = PAUSESTATE_NONE;
		}
		break;
	case PAUSESTATE_QUIT:
		if (*GetFade() == FADE_NONE)
		{
			SetFade(MODE_TITLE);
			StopSound(SOUND_LABEL_BGM_GAME);
			g_PauseState = PAUSESTATE_NONE;
		}
		break;
	default:
		break;
	}
}

//=============================================================================
// �|�[�Y�̕`�揈��
//=============================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);			//���_�t�H�[�}�b�g�̐ݒ�

	for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE; nCntPause++)
	{
		pDevice->SetTexture(0, g_pTexturePause[nCntPause]);	//�e�N�X�`���̐ݒ�

		//�|�[�Y�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			nCntPause * 4,				//�J�n����C���f�b�N�X
			2);				//�v���~�e�B�u�̐�(�|�[�Y�̐�)
	}
}

//=============================================================================
// ���_�o�b�t�@�̐���
//=============================================================================
void MakeVertexPause(LPDIRECT3DDEVICE9 pDevice)
{
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSE_TEXTURE,	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
		D3DUSAGE_WRITEONLY,		//�Œ�
		FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,		//�Œ�
		&g_pVtxBuffPause,
		NULL);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE; nCntPause++)
	{
		//���_�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posPause.x - (PAUSE_SIZE_X / 2), g_posPause.y - (PAUSE_SIZE_Y / 2) + (150.0f*nCntPause), 0.0f);	//����0.0���ŌŒ�
		pVtx[1].pos = D3DXVECTOR3(g_posPause.x + (PAUSE_SIZE_X / 2), g_posPause.y - (PAUSE_SIZE_Y / 2) + (150.0f*nCntPause), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posPause.x - (PAUSE_SIZE_X / 2), g_posPause.y + (PAUSE_SIZE_Y / 2) + (150.0f*nCntPause), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posPause.x + (PAUSE_SIZE_X / 2), g_posPause.y + (PAUSE_SIZE_Y / 2) + (150.0f*nCntPause), 0.0f);

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

		pVtx += 4;
	}
	//���_�f�[�^���A�����b�N
	g_pVtxBuffPause->Unlock();
}