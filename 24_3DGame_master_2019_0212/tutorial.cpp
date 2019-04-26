//=============================================================================
//
// �^�C�g������[tutorial.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"tutorial.h"	//�C���N���[�h�t�@�C��
#include "input.h"	
#include "joystick.h"
#include "fade.h"
#include"logo.h"
#include "sound.h"
#include"pressstart.h"
#include "camera.h"
#include "player.h"
#include "shadow.h"
#include "effect.h"
#include "princess.h"
#include "enemy.h"
#include "magic.h"
#include "wave.h"
#include "trajectory.h"
#include "meshfield.h"
#include "meshwall.h"
#include "meshcylinder.h"
#include "ground.h"
#include "light.h"
#include "particle.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TUTORIAL_NAME	"data/TEXTURE/�`���[�g���A��.png"	//�ǂݍ��ރe�N�X�`����
#define TUTORIAL_POS	(0.0f)
#define TUTORIAL_SIZE_X	(1280)
#define TUTORIAL_SIZE_Y	(209)

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexTutorial(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �`���[�g���A���̏��
//=============================================================================
typedef enum
{
	TUTORIAL_NORMAL,		//�ʏ�
	TUTORIAL_GETBOTTON,	//�{�^���������ꂽ
	TUTORIAL_MAX
} TUTORIAL;

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureTutorial = NULL;			//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;	//���_�o�b�t�@�ւ̃|�C���^
TUTORIAL g_Tutorialstate;

//=============================================================================
// �|���S���̏���������
//=============================================================================
void InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();	//�f�o�C�X�̎擾
	g_Tutorialstate = TUTORIAL_NORMAL;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		TUTORIAL_NAME,
		&g_pTextureTutorial);

	MakeVertexTutorial(pDevice);

	// �J�����̏���������
	InitCamera();

	//�e�̏�����
	InitShadow();

	//���@�w�̏�����
	InitMagic();

	//���@�̏�����
	InitWave();
	SetWave(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//�G�t�F�N�g�̏�����
	InitEffect();
	InitTrajectory();

	//�p�[�e�B�N���̏�����
	InitParticle();

	//�P�̏�����
	InitPrincess();

	// �v���C���[�̏�����
	InitPlayer();

	//�G�̏�����
	InitEnemy();
	SetEnemy(D3DXVECTOR3(0, 0.0f, 100), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100);

	//�v���X�X�^�[�g�̏�����
	InitStart();
}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void UninitTutorial(void)
{
	//�v���X�X�^�[�g�̏I��
	UninitStart();

	//�G�l�~�[�̏���
	DeleteEnemey();

	//�p�[�e�B�N���̏I��
	UninitParticle();

	//�e�N�X�`���̊J��
	if (g_pTextureTutorial != NULL)
	{					  
		g_pTextureTutorial->Release();
		g_pTextureTutorial = NULL;
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}

}

//=============================================================================
// �|���S���̍X�V����
//=============================================================================
void UpdateTutorial(void)
{
	// �J�����̍X�V����
	UpdateCamera();

	// ���C�g�̍X�V����
	UpdateLight();

	//�V�����_�[�̍X�V
	UpdateMeshCylinder();

	//�e�̍X�V
	UpdateShadow();

	//���@�w�̍X�V
	UpdateMagic();

	//�n�ʂ̍X�V
	UpdateGround();

	//�t�B�[���h�̍X�V
	UpdateMeshField();

	//���b�V���ǂ̍X�V
	UpdateMeshWall();

	//�P�̍X�V
	UpdatePrincess();

	//�v���C���[�̍X�V
	UpdatePlayer();

	//�G�̍X�V
	UpdateEnemy();

	//�G�t�F�N�g�̍X�V
	UpdateEffect();
	UpdateTrajectory();

	//�p�[�e�B�N���̍X�V
	UpdateParticle();

	//���@�̍X�V
	UpdateWave();

	//�v���X�X�^�[�g�̍X�V
	UpdateStart();

	if (g_Tutorialstate == TUTORIAL_NORMAL && (GetKeyTrigger(DIK_RETURN) || GetJoystickTrigger(PAD_START)))	//Enter�ŏI��
	{
		PlaySound(SOUND_LABEL_SE_CAT);
		g_Tutorialstate = TUTORIAL_GETBOTTON;
		SetFade(MODE_GAME);
	}
}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void DrawTutorial(void)
{
	// �J�����̐ݒ�
	SetCamera();

	//�n�ʂ̕`��
	DrawGround();

	//�t�B�[���h�̕`��
	DrawMeshField();

	//���b�V���ǂ̕`��
	DrawMeshWall();

	//�e�̕`��
	DrawShadow();

	//���@�w�̕`��
	DrawMagic();

	//�P�̕`��
	DrawPrincess();

	//�G�̕`��
	DrawEnemy();

	//�v���C���[�̕`��
	DrawPlayer();

	//�V�����_�[�̕`��
	DrawMeshCylinder();

	//���@�̕`��
	DrawWave();

	//�G�t�F�N�g�̕`��
	DrawEffect();
	DrawTrajectory();

	//�p�[�e�B�N���̕`��
	DrawParticle();

	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);				//���_�t�H�[�}�b�g�̐ݒ�

	pDevice->SetTexture(0, g_pTextureTutorial);	//�e�N�X�`���̐ݒ�

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
		0,					//�J�n����C���f�b�N�X
		2);					//�v���~�e�B�u�̐�(�|���S���̐�)

//�v���X�X�^�[�g�̕`��
	DrawStart();
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexTutorial(LPDIRECT3DDEVICE9 pDevice)
{
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
		D3DUSAGE_WRITEONLY,		//�Œ�
		FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,		//�Œ�
		&g_pVtxBuffTutorial,
		NULL);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

														//���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(TUTORIAL_POS, SCREEN_HEIGHT- TUTORIAL_SIZE_Y, 0.0f);	//����0.0���ŌŒ�
	pVtx[1].pos = D3DXVECTOR3(TUTORIAL_POS + TUTORIAL_SIZE_X, SCREEN_HEIGHT - TUTORIAL_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TUTORIAL_POS, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TUTORIAL_POS + TUTORIAL_SIZE_X, SCREEN_HEIGHT, 0.0f);

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
	g_pVtxBuffTutorial->Unlock();
}