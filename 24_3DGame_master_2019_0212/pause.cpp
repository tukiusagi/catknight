//=============================================================================
//
// �|�[�Y����[pause.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"pause.h"	//�C���N���[�h�t�@�C��
#include "input.h"	
#include "joystick.h"
#include "fade.h"
#include"game.h"
#include "sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define PAUSE_CONTINUE	"data/TEXTURE/�߂�.png"	//�ǂݍ��ރe�N�X�`����
#define PAUSE_RETRY	"data/TEXTURE/�͂��߂���.png"	
#define PAUSE_QUIT	"data/TEXTURE/�^�C�g��.png"	

#define PAUSE_POS_X	(0)			//�|���S���ʒuX
#define PAUSE_POS_Y	(0)			//�|���S���ʒuY
#define PAUSE_SIZE_X	(600)	//�|���S���T�C�YX
#define PAUSE_SIZE_Y	(200)	//�|���S���T�C�YY
#define PAUSE_TEXTURE	(3)			//�|���S���ʒuX

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
// �|���S���̏���������
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

		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//���_�f�[�^���A�����b�N
	g_pVtxBuffPause->Unlock();

	g_posPause = D3DXVECTOR3(0, 0, 0);
	g_PauseColor = PAUSE_COLORSTATE_NONE;
	g_PauseState = PAUSESTATE_NONE;
	g_nSelectPause = 0;
}

//=============================================================================
// �|���S���̏I������
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
// �|���S���̍X�V����
//=============================================================================
void UpdatePause(void)
{
	if ((GetKeyTrigger(DIK_UP) || GetJoystickTrigger(PAD_UP_L)))
	{
		g_nSelectPause = (g_nSelectPause + 2) % 3;
		PlaySound(SOUND_LABEL_SE_PAUSE);

	}
	if ((GetKeyTrigger(DIK_DOWN) || GetJoystickTrigger(PAD_DOWN_L)))
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
		if ((GetKeyTrigger(DIK_RETURN) || GetJoystickTrigger(PAD_B)))
		{
			g_PauseState = PAUSESTATE_CONTINUE;
			PlaySound(SOUND_LABEL_SE_ENTER);
		}

		//�R���e�B�j���[�ȊO�͔�������
		//for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE; nCntPause++)
		//{
		//	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		//	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		//	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		//	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 128);

		//	pVtx += 4;
		//}
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 128);

		pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 128);

		break;
	case 1:
		g_PauseColor = PAUSE_COLORSTATE_RETRY;
		if ((GetKeyTrigger(DIK_RETURN) || GetJoystickTrigger(PAD_B)))
		{
			g_PauseState = PAUSESTATE_RETRY;
			PlaySound(SOUND_LABEL_SE_ENTER);
		}

		//for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE; nCntPause++)
		//{
		//	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		//	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		//	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		//	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		//	pVtx += 4;

		//}

		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 128);

		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 128);

		break;
	case 2:
		g_PauseColor = PAUSE_COLORSTATE_QUIT;
		if ((GetKeyTrigger(DIK_RETURN) || GetJoystickTrigger(PAD_B)))
		{
			g_PauseState = PAUSESTATE_QUIT;
			PlaySound(SOUND_LABEL_SE_ENTER);
		}
		//for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE; nCntPause++)
		//{
		//	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		//	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		//	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		//	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		//	pVtx += 4;
		//}

		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 128);

		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 128);

		pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 255);


		break;
	default:
		break;
	}
	//���_�f�[�^���A�����b�N
	g_pVtxBuffPause->Unlock();

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
// �|���S���̕`�揈��
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

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			nCntPause * 4,				//�J�n����C���f�b�N�X
			2);				//�v���~�e�B�u�̐�(�|���S���̐�)
	}
}