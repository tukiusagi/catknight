//=============================================================================
//
// �^�C�g������[title.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"title.h"	//�C���N���[�h�t�@�C��
#include "input.h"	
#include "joystick.h"
#include "xController.h"

#include "fade.h"
#include "camera.h"
#include "light.h"
#include "meshfield.h"
#include "meshwall.h"
#include "shadow.h"
#include"logo.h"
#include "princess.h"
#include"pressstart.h"
#include "meshcylinder.h"
#include "ground.h"
#include "sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define SECOND (60)	//�b��

//=============================================================================
// �^�C�g���̏��
//=============================================================================
typedef enum
{
	TITLE_NORMAL,		//�ʏ�
	TITLE_GETBOTTON,	//�{�^���������ꂽ
	TITLE_MAX
} TITLE;

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
TITLE TitleState;
int g_nCntTitle;

//=============================================================================
// ����������
//=============================================================================
void InitTitle(void)
{
	TitleState = TITLE_NORMAL;
	g_nCntTitle = 0;

	// �J�����̏���������
	InitCamera();

	// ���C�g�̏���������
	InitLight();

	//�V�����_�[�̏�����
	InitMeshCylinder();
	SetMeshCylinder(D3DXVECTOR3(0.0f, 800.0f, 0.0f), D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f), 1000.0f, 1000.0f, 0);

	//�n�ʂ̏�����
	InitGround();

	//�t�B�[���h�̏�����
	InitMeshField();

	//���b�V���ǂ̏�����
	InitMeshWall();
	SetMeshWall(D3DXVECTOR3(0.0f, 0.0f, MESHFIELD_SIZE* 0.5f * MESHFIELD_D), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetMeshWall(D3DXVECTOR3(MESHFIELD_SIZE* 0.5f * MESHFIELD_W, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f));
	SetMeshWall(D3DXVECTOR3(-MESHFIELD_SIZE* 0.5f * MESHFIELD_W, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI / 2, 0.0f));
	SetMeshWall(D3DXVECTOR3(0.0f, 0.0f, -MESHFIELD_SIZE* 0.5f * MESHFIELD_D), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

	//�e�̏�����
	InitShadow();

	//�P�̏�����
	InitPrincess();

	SetLogo(LOGO_TITLE);
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitle(void)
{
	// �J�����̏I������
	UninitCamera();

	//���S�̏I��
	DeleteLogo();

	//�v���X�X�^�[�g�̏I��
	UninitStart();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTitle(void)
{
	// �J�����̍X�V����
	UpdateCamera();

	// ���C�g�̍X�V����
	UpdateLight();

	//�e�̍X�V
	UpdateShadow();

	//�V�����_�[�̍X�V
	UpdateMeshCylinder();

	//�n�ʂ̍X�V
	UpdateGround();

	//�t�B�[���h�̍X�V
	UpdateMeshField();

	//���b�V���ǂ̍X�V
	UpdateMeshWall();

	//�P�̍X�V
	UpdatePrincess();

	//�v���X�X�^�[�g�̍X�V
	UpdateStart();

	g_nCntTitle++;
	if (TitleState == TITLE_NORMAL && (GetKeyTrigger(DIK_RETURN)||GetJoystickTrigger(PAD_START) || GetxControllerTrigger(XINPUT_PAD_START)))	//Enter�ŏI��
	{
		PlaySound(SOUND_LABEL_SE_CAT);
		SetFade(MODE_TUTORIAL);
		TitleState = TITLE_GETBOTTON;
	}
	else if (g_nCntTitle > SECOND * 5)
	{
		SetFade(MODE_RANKING);
		g_nCntTitle = 0;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
{
	// �J�����̐ݒ�
	SetCamera();

	//�V�����_�[�̕`��
	DrawMeshCylinder();

	//�n�ʂ̕`��
	DrawGround();

	//�t�B�[���h�̕`��
	DrawMeshField();

	//���b�V���ǂ̕`��
	DrawMeshWall();

	//�e�̕`��
	DrawShadow();

	//�P�̕`��
	DrawPrincess();

	//�v���X�X�^�[�g�̕`��
	DrawStart();
}