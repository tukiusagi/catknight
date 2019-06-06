//=============================================================================
//
// �`���[�g���A������[tutorial.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"tutorial.h"	//�C���N���[�h�t�@�C��
#include"manual.h"

#include "input.h"	
#include "joystick.h"
#include "xController.h"

#include"pressstart.h"
#include"logo.h"
#include "fade.h"
#include "sound.h"

#include "camera.h"
#include "light.h"

#include "meshfield.h"
#include "meshwall.h"
#include "meshcylinder.h"
#include "ground.h"

#include "player.h"
#include "princess.h"
#include "enemy.h"

#include "shadow.h"
#include "effect.h"
#include "magic.h"
#include "wave.h"
#include "trajectory.h"
#include "particle.h"
#include "magicGage.h"
#include "lifePrincess.h"

//=============================================================================
// �`���[�g���A���̏��
//=============================================================================
typedef enum
{
	TUTORIAL_NORMAL,	//�ʏ�
	TUTORIAL_GETBOTTON,	//�{�^���������ꂽ
	TUTORIAL_MAX
} TUTORIAL;

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
TUTORIAL g_Tutorialstate;

//=============================================================================
// ����������
//=============================================================================
void InitTutorial(void)
{
	g_Tutorialstate = TUTORIAL_NORMAL;

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

	// �v���C���[�̏�����
	InitPlayer();

	//�P�̏�����
	InitPrincess();

	//�G�̏�����
	InitEnemy();
	SetEnemy(D3DXVECTOR3(0, 0.0f, 100));

	//�P���C�t�̏�����
	InitPLife();

	//���@�Q�[�W�̏�����
	InitMagicGage();

	//�v���X�X�^�[�g�̏�����
	InitStart();

	//�������
	InitManual();
}

//=============================================================================
// �I������
//=============================================================================
void UninitTutorial(void)
{
	//�v���X�X�^�[�g�̏I��
	UninitStart();

	//�G�l�~�[�̏���
	DeleteEnemey();

	////�p�[�e�B�N���̏I��
	//UninitParticle();

	//�������
	UninitManual();
}

//=============================================================================
// �X�V����
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

	//�P���C�t�̍X�V
	UpdatePLife();

	//���@�Q�[�W�̍X�V
	UpdateMagicGage();

	//�v���X�X�^�[�g�̍X�V
	UpdateStart();

	//�������
	UpdateManual();

	if (g_Tutorialstate == TUTORIAL_NORMAL && (GetKeyTrigger(DIK_RETURN) || GetJoystickTrigger(PAD_START) || GetxControllerTrigger(XINPUT_PAD_START)))	//Enter�ŏI��
	{
		PlaySound(SOUND_LABEL_SE_CAT);
		g_Tutorialstate = TUTORIAL_GETBOTTON;
		SetFade(MODE_GAME);
	}
}

//=============================================================================
// �`�揈��
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

	//�P���C�t�̕`��
	DrawPLife();

	//���@�Q�[�W�̕`��
	DrawMagicGage();

	//�p�[�e�B�N���̕`��
	DrawParticle();

	//�������
	DrawManual();

	//�v���X�X�^�[�g�̕`��
	DrawStart();
}