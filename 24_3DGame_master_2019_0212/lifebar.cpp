//=============================================================================
//
// ���C�t���[�^�[���� [lifebar.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include "lifebar.h"
#include"game.h"
#include "princess.h"
#include "sound.h"
#include"logo.h"
#include "score.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define LIFEBAR_MAX			(50.0f)	//�ő僉�C�t
#define LIFEBAR_SIZE_Y		(2.0f)	//�T�C�YY
#define LIFEBAR_TEXTUREMAX	(2)		//�ő僉�C�t

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexLifebar(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// ���C�t�o�[�\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;		//�ʒu
	D3DXVECTOR3	rot;		//����
	D3DXCOLOR	col;		//�F
	D3DXMATRIX	mtxWorld;	//���[���h�}�g���b�N�X
	float		fMaxLife;	//���C�t�̐��l
}LIFEBAR;

//=============================================================================
// �O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureLifebar[LIFEBAR_TEXTUREMAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLifebar = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

LIFEBAR					g_aLifebar;	// ���C�t

//=============================================================================
// ����������
//=============================================================================
void InitLifebar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

		g_aLifebar.pos = D3DXVECTOR3(-LIFEBAR_MAX * 0.5f, 60.0f, 0.0f);
		g_aLifebar.fMaxLife = LIFEBAR_MAX;
		g_aLifebar.col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								NULL,					// �t�@�C���̖��O
								&g_pTextureLifebar[0]);	// �ǂݍ��ރ������[
	
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								NULL,					// �t�@�C���̖��O
								&g_pTextureLifebar[1]);	// �ǂݍ��ރ������[

	// ���_���̍쐬
	MakeVertexLifebar(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitLifebar(void)
{
	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < LIFEBAR_TEXTUREMAX; nCnt++)
	{
		if (g_pTextureLifebar[nCnt] != NULL)
		{
			g_pTextureLifebar[nCnt]->Release();
			g_pTextureLifebar[nCnt] = NULL;
		}

	}
	
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffLifebar != NULL)
	{
		g_pVtxBuffLifebar->Release();
		g_pVtxBuffLifebar = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateLifebar(void)
{
	VERTEX_3D *pVtx;

	if (g_aLifebar.fMaxLife < (LIFEBAR_MAX / 3))
	{	//�O���̈��菭�Ȃ��Ƃ��ԕ\��
		g_aLifebar.col= D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffLifebar->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	pVtx[4].col = g_aLifebar.col;
	pVtx[5].col = g_aLifebar.col;
	pVtx[6].col = g_aLifebar.col;
	pVtx[7].col = g_aLifebar.col;

	//���_�f�[�^���A�����b�N
	g_pVtxBuffLifebar->Unlock();

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawLifebar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// �v�Z�p�}�g���b�N�X
	LIFEBAR *pLifebar;

	pLifebar = &g_aLifebar;

	//���݂̃r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 111);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&pLifebar->mtxWorld);

	//�t�s��̐ݒ�
	pLifebar->mtxWorld._11 = mtxView._11;
	pLifebar->mtxWorld._12 = mtxView._21;
	pLifebar->mtxWorld._13 = mtxView._31;
	pLifebar->mtxWorld._21 = mtxView._12;
	pLifebar->mtxWorld._22 = mtxView._22;
	pLifebar->mtxWorld._23 = mtxView._32;
	pLifebar->mtxWorld._31 = mtxView._13;
	pLifebar->mtxWorld._32 = mtxView._23;
	pLifebar->mtxWorld._33 = mtxView._33;

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, pLifebar->rot.y, pLifebar->rot.x, pLifebar->rot.z);

	D3DXMatrixMultiply(&pLifebar->mtxWorld, &pLifebar->mtxWorld, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pLifebar->pos.x, pLifebar->pos.y, pLifebar->pos.z);

	D3DXMatrixMultiply(&pLifebar->mtxWorld, &pLifebar->mtxWorld, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &pLifebar->mtxWorld);

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffLifebar, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCntBar = 0; nCntBar < LIFEBAR_TEXTUREMAX; nCntBar++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureLifebar[nCntBar]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			nCntBar * 4,		//�J�n����C���f�b�N�X
			2);					//�v���~�e�B�u�̐�(�|���S���̐�)
	}
	//�ʏ탂�[�h�ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void MakeVertexLifebar(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * LIFEBAR_TEXTUREMAX,	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
		D3DUSAGE_WRITEONLY,		//�Œ�
		FVF_VERTEX_3D,			//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,		//�Œ�
		&g_pVtxBuffLifebar,
		NULL);

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffLifebar->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	// ���_���̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0, LIFEBAR_SIZE_Y, 0.0f);	//����0.0���ŌŒ�
	pVtx[1].pos = D3DXVECTOR3(LIFEBAR_MAX, LIFEBAR_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, -LIFEBAR_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(LIFEBAR_MAX, -LIFEBAR_SIZE_Y, 0.0f);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_���̐ݒ�
	pVtx[4].pos = D3DXVECTOR3(0, LIFEBAR_SIZE_Y, 0.0f);	//����0.0���ŌŒ�
	pVtx[5].pos = D3DXVECTOR3(LIFEBAR_MAX, LIFEBAR_SIZE_Y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(0, -LIFEBAR_SIZE_Y, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(LIFEBAR_MAX, -LIFEBAR_SIZE_Y, 0.0f);

	pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[4].col = g_aLifebar.col;
	pVtx[5].col = g_aLifebar.col;
	pVtx[6].col = g_aLifebar.col;
	pVtx[7].col = g_aLifebar.col;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^���A�����b�N
	g_pVtxBuffLifebar->Unlock();
}

//=============================================================================
// ���C�t�̌���
//=============================================================================
void DecreaseLifebar(int nValue)
{
	PRINCESS *pPrincess = GetPrincess();

	if (*GetGameState() == GAMESTATE_NORMAL)
	{
		g_aLifebar.fMaxLife -= nValue;
		pPrincess->bDamege = true;

		VERTEX_3D *pVtx;

		//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffLifebar->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

		// ���_���̐ݒ�
		pVtx[4].pos = D3DXVECTOR3(-0, LIFEBAR_SIZE_Y, 0.0f);	//����0.0���ŌŒ�
		pVtx[5].pos = D3DXVECTOR3(g_aLifebar.fMaxLife, LIFEBAR_SIZE_Y, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(-0, -LIFEBAR_SIZE_Y, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(g_aLifebar.fMaxLife, -LIFEBAR_SIZE_Y, 0.0f);

		//���_�f�[�^���A�����b�N
		g_pVtxBuffLifebar->Unlock();

		if (g_aLifebar.fMaxLife <= 0 && *GetGameState() != GAMESTATE_GAMEOVER)
		{
			g_aLifebar.fMaxLife = 0;
			SetGameState(GAMESTATE_GAMEOVER);
			SetLogo(LOGO_GAMEOVER);
			StopSound(SOUND_LABEL_BGM_GAME);
			PlaySound(SOUND_LABEL_BGM_GAMEOVER);
		}
	}
}
//=============================================================================
// ���C�t�̑���
//=============================================================================
void IncreaseLifebar(int nlife)
{
	g_aLifebar.fMaxLife += nlife;

	VERTEX_3D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffLifebar->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	// ���_���̐ݒ�
	pVtx[4].pos = D3DXVECTOR3(0, LIFEBAR_SIZE_Y, 0.0f);	//����0.0���ŌŒ�
	pVtx[5].pos = D3DXVECTOR3(g_aLifebar.fMaxLife, LIFEBAR_SIZE_Y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(0, -LIFEBAR_SIZE_Y, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aLifebar.fMaxLife, -LIFEBAR_SIZE_Y, 0.0f);

	//���_�f�[�^���A�����b�N
	g_pVtxBuffLifebar->Unlock();
}

//=============================================================================
// ���C�t�̑���
//=============================================================================
float GetLifeber(void)
{
	return g_aLifebar.fMaxLife;
}