//=============================================================================
//
// �v���X�X�^�[�g����[pressstart.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"pressstart.h"
#include "input.h"	
#include "joystick.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureStart = NULL;	//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStart = NULL;	//���_�o�b�t�@�ւ̃|�C���^

D3DXVECTOR3 g_Startpos;		//�ʒu
D3DXCOLOR	g_StartCol;		//�F
int			g_nCntStart;	//�_�ŃJ�E���^�[
float		nInvisible;		//�_�Ō���

//=============================================================================
// �|���S���̏���������
//=============================================================================
void InitStart(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	pDevice = GetDevice();	//�f�o�C�X�̎擾

	if (*GetMode() == MODE_TITLE)
	{
		g_Startpos = D3DXVECTOR3(640.0f, 600.0f, 0.0f);
	}
	else if (*GetMode() == MODE_TUTORIAL)
	{
		g_Startpos = D3DXVECTOR3(1050.0f, SCREEN_HEIGHT-209- (START_SIZE_Y/2), 0.0f);
	}
		g_StartCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		nInvisible = 0.007f;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
							START_NAME,
							&g_pTextureStart);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
								D3DUSAGE_WRITEONLY,		//�Œ�
								FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,		//�Œ�
								&g_pVtxBuffStart,
								NULL);


	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	//���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Startpos.x - (START_SIZE_X / 2), g_Startpos.y - (START_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
	pVtx[1].pos = D3DXVECTOR3(g_Startpos.x + (START_SIZE_X / 2), g_Startpos.y - (START_SIZE_Y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Startpos.x - (START_SIZE_X / 2), g_Startpos.y + (START_SIZE_Y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Startpos.x + (START_SIZE_X / 2), g_Startpos.y + (START_SIZE_Y / 2), 0.0f);

	pVtx[0].rhw = 1.0f;	//1.0f�ŌŒ�
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = g_StartCol;
	pVtx[1].col = g_StartCol;
	pVtx[2].col = g_StartCol;
	pVtx[3].col = g_StartCol;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^���A�����b�N
	g_pVtxBuffStart->Unlock();
}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void UninitStart(void)
{
	//�e�N�X�`���̊J��
	if (g_pTextureStart != NULL)
	{
		g_pTextureStart->Release();
		g_pTextureStart = NULL;
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffStart != NULL)
	{
		g_pVtxBuffStart->Release();
		g_pVtxBuffStart = NULL;
	}

}

//=============================================================================
// �|���S���̍X�V����
//=============================================================================
void UpdateStart(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	g_nCntStart++;

	g_StartCol.a += nInvisible;

	if (g_StartCol.a <= 0.0f|| g_StartCol.a>=1.0f)
	{
		nInvisible *= -1;
	}

	if (GetKeyTrigger(DIK_RETURN) || GetJoystickTrigger(PAD_START))	//Start�ŏI��
	{
		nInvisible = 0.5;
	}

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	//���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Startpos.x - (START_SIZE_X / 2), g_Startpos.y - (START_SIZE_Y / 2), 0.0f);	//����0.0���ŌŒ�
	pVtx[1].pos = D3DXVECTOR3(g_Startpos.x + (START_SIZE_X / 2), g_Startpos.y - (START_SIZE_Y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Startpos.x - (START_SIZE_X / 2), g_Startpos.y + (START_SIZE_Y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Startpos.x + (START_SIZE_X / 2), g_Startpos.y + (START_SIZE_Y / 2), 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = g_StartCol;
	pVtx[1].col = g_StartCol;
	pVtx[2].col = g_StartCol;
	pVtx[3].col = g_StartCol;

	//���_�f�[�^���A�����b�N
	g_pVtxBuffStart->Unlock();

}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void DrawStart(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffStart, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);				//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, g_pTextureStart);		//�e�N�X�`���̐ݒ�

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
							0,					//�J�n����C���f�b�N�X
							2);					//�v���~�e�B�u�̐�(�|���S���̐�)
}