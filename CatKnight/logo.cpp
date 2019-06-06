//=============================================================================
//
// ���S����[logo.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"logo.h"
#include "input.h"	
#include "joystick.h"
#include "fade.h"
#include "main.h"
#include"game.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TITLE_LOGO		"data/TEXTURE/titlelogo.png"	//�ǂݍ��ރ^�C�g�����S
#define CREAR_LOGO		"data/TEXTURE/Clear.png"		//�ǂݍ��ރN���A���S
#define GAMEOVER_LOGO	"data/TEXTURE/gameover.png"		//�ǂݍ��ރQ�[���I�[�o�[���S
#define RESULT_LOGO		"data/TEXTURE/result.png"		//�ǂݍ��ރ��U���g���S
#define RANKING_LOGO	"data/TEXTURE/ranking.png"		//�ǂݍ��ރ����L���O���S

#define LOGO_POS_X		(0)		//�|���S���ʒuX
#define LOGO_POS_Y		(0)		//�|���S���ʒuY
#define LOGO_SIZE_X		(600)	//�|���S���T�C�YX
#define LOGO_SIZE_Y		(200)	//�|���S���T�C�YY


//=============================================================================
// ���S�̍\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXCOLOR	col;
	LOGOTYPE	type;
	bool		bUse;
}LOGO;
//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureLogo[LOGO_MAX] = {};	//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLogo = NULL;	//���_�o�b�t�@�ւ̃|�C���^

LOGO	g_Logo;

//=============================================================================
// ����������
//=============================================================================
void InitLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	pDevice = GetDevice();	//�f�o�C�X�̎擾

	g_Logo.pos = D3DXVECTOR3(640.0f, 150.0f, 0.0f);
	g_Logo.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	g_Logo.type = LOGO_TITLE;
	g_Logo.bUse = false;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		TITLE_LOGO,
		&g_pTextureLogo[LOGO_TITLE]);

	D3DXCreateTextureFromFile(pDevice,
		CREAR_LOGO,
		&g_pTextureLogo[LOGO_CLEAR]);

	D3DXCreateTextureFromFile(pDevice,
		GAMEOVER_LOGO,
		&g_pTextureLogo[LOGO_GAMEOVER]);

	D3DXCreateTextureFromFile(pDevice,
		RESULT_LOGO,
		&g_pTextureLogo[LOGO_RESULT]);

	D3DXCreateTextureFromFile(pDevice,
		RANKING_LOGO,
		&g_pTextureLogo[LOGO_RANKING]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
		D3DUSAGE_WRITEONLY,		//�Œ�
		FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,		//�Œ�
		&g_pVtxBuffLogo,
		NULL);


	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffLogo->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	//���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Logo.pos.x - (LOGO_SIZE_X / 2), g_Logo.pos.y - (LOGO_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
	pVtx[1].pos = D3DXVECTOR3(g_Logo.pos.x + (LOGO_SIZE_X / 2), g_Logo.pos.y - (LOGO_SIZE_Y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Logo.pos.x - (LOGO_SIZE_X / 2), g_Logo.pos.y + (LOGO_SIZE_Y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Logo.pos.x + (LOGO_SIZE_X / 2), g_Logo.pos.y + (LOGO_SIZE_Y / 2), 0.0f);

	pVtx[0].rhw = 1.0f;	//1.0f�ŌŒ�
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = g_Logo.col;
	pVtx[1].col = g_Logo.col;
	pVtx[2].col = g_Logo.col;
	pVtx[3].col = g_Logo.col;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^���A�����b�N
	g_pVtxBuffLogo->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitLogo(void)
{
	for (int nCntlogo = 0; nCntlogo < LOGO_MAX; nCntlogo++)
	{
		//�e�N�X�`���̊J��
		if (g_pTextureLogo[nCntlogo] != NULL)
		{				  
			g_pTextureLogo[nCntlogo]->Release();
			g_pTextureLogo[nCntlogo] = NULL;
		}
	}
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffLogo != NULL)
	{
		g_pVtxBuffLogo->Release();
		g_pVtxBuffLogo = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateLogo(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	int nMove = 10;
	
	g_Logo.col.a += 0.01f;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffLogo->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	pVtx[0].pos = D3DXVECTOR3(g_Logo.pos.x - (LOGO_SIZE_X / 2), g_Logo.pos.y - (LOGO_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
	pVtx[1].pos = D3DXVECTOR3(g_Logo.pos.x + (LOGO_SIZE_X / 2), g_Logo.pos.y - (LOGO_SIZE_Y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Logo.pos.x - (LOGO_SIZE_X / 2), g_Logo.pos.y + (LOGO_SIZE_Y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Logo.pos.x + (LOGO_SIZE_X / 2), g_Logo.pos.y + (LOGO_SIZE_Y / 2), 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = g_Logo.col;
	pVtx[1].col = g_Logo.col;
	pVtx[2].col = g_Logo.col;
	pVtx[3].col = g_Logo.col;

	//���_�f�[�^���A�����b�N
	g_pVtxBuffLogo->Unlock();

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffLogo, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);				//���_�t�H�[�}�b�g�̐ݒ�
	
	if (g_Logo.bUse)
	{
		pDevice->SetTexture(0, g_pTextureLogo[g_Logo.type]);		//�e�N�X�`���̐ݒ�

			//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			0,					//�J�n����C���f�b�N�X
			2);					//�v���~�e�B�u�̐�(�|���S���̐�)
	}
}

//=============================================================================
// ���S�̃Z�b�g
//=============================================================================
void SetLogo(LOGOTYPE nType)
{
	g_Logo.type = nType;
	if (nType == LOGO_RANKING || nType == LOGO_RESULT)
	{
		g_Logo.pos.y = 100;
	}
	g_Logo.bUse = true;
}

//=============================================================================
// ���S�̏���
//=============================================================================
void DeleteLogo(void)
{
	g_Logo.bUse = false;
	g_Logo.pos = D3DXVECTOR3(640.0f, 150.0f, 0.0f);
	g_Logo.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
}
