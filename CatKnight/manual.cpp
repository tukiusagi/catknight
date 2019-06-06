//=============================================================================
//
// �������UI[manual.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"manual.h"	//�C���N���[�h�t�@�C��

//=============================================================================
// �}�N����`
//=============================================================================
#define MANUAL_NAME	"data/TEXTURE/tutorial.png"	//�ǂݍ��ރe�N�X�`����
#define MANUAL_POS		(1025)
#define MANUAL_SIZE_X	(255)
#define MANUAL_SIZE_Y	(720)

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexManual(LPDIRECT3DDEVICE9 pDevice);


//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureManual = NULL;			//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffManual = NULL;	//���_�o�b�t�@�ւ̃|�C���^

//=============================================================================
// ����������
//=============================================================================
void InitManual(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();	//�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		MANUAL_NAME,
		&g_pTextureManual);

	MakeVertexManual(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitManual(void)
{
	//�e�N�X�`���̊J��
	if (g_pTextureManual != NULL)
	{					  
		g_pTextureManual->Release();
		g_pTextureManual = NULL;
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffManual != NULL)
	{
		g_pVtxBuffManual->Release();
		g_pVtxBuffManual = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateManual(void)
{
}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void DrawManual(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffManual, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);				//���_�t�H�[�}�b�g�̐ݒ�

	pDevice->SetTexture(0, g_pTextureManual);	//�e�N�X�`���̐ݒ�

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
		0,					//�J�n����C���f�b�N�X
		2);					//�v���~�e�B�u�̐�(�|���S���̐�)
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexManual(LPDIRECT3DDEVICE9 pDevice)
{
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
		D3DUSAGE_WRITEONLY,		//�Œ�
		FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,		//�Œ�
		&g_pVtxBuffManual,
		NULL);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffManual->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

														//���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(MANUAL_POS, 0.0f, 0.0f);	//����0.0���ŌŒ�
	pVtx[1].pos = D3DXVECTOR3(MANUAL_POS + MANUAL_SIZE_X, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(MANUAL_POS, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(MANUAL_POS + MANUAL_SIZE_X, SCREEN_HEIGHT, 0.0f);

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
	g_pVtxBuffManual->Unlock();
}