//=============================================================================
//
// �Q�[������[game.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"game.h"	//�C���N���[�h�t�@�C��

#include "input.h"
#include "joystick.h"
#include "xController.h"

#include "camera.h"
#include "light.h"

#include "fade.h"
#include "line.h"
#include "logo.h"
#include "sound.h"

#include "player.h"
#include "princess.h"
#include "enemy.h"
#include "boss.h"
#include "obstacle.h"

#include "shadow.h"
#include "effect.h"
#include "magic.h"
#include "wave.h"

#include "meshfield.h"
#include "meshwall.h"
#include "meshcylinder.h"
#include "ground.h"
#include "trajectory.h"
#include "particle.h"

#include "life.h"
#include "pause.h"
#include "score.h"
#include "combo.h"
#include"help.h"
#include "EwaveUI.h"
#include "lifePrincess.h"
#include "magicGage.h"

#include "enemywave.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
GAMESTATE g_gameState = GAMESTATE_NONE;	//�Q�[�����
int g_nCounterGameState = 0;			//�Q�[����ԃJ�E���^�[
int g_nLastScore;							//�ŏI�X�R�A

//=============================================================================
// ����������
//=============================================================================
void InitGame(void)
{
	srand((unsigned int)time(NULL));

	g_gameState = GAMESTATE_NORMAL;	//�Q�[�����
	g_nCounterGameState = 0;
	g_nLastScore = 0;

	//�|�[�Y�̏�����
	InitPause();

	//���C���̏�����
	InitLine();

	//�I�u�W�F�N�g�̏�����
	InitObstacle();
	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		SetObstacle(D3DXVECTOR3(rand() % 220 + 80,
			0.0f,
			rand() % int(MESHFIELD_SIZE * MESHFIELD_D * 0.9f) - int(MESHFIELD_SIZE * MESHFIELD_D * 0.4f)),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		SetObstacle(D3DXVECTOR3(-rand() % 220 - 80,
			0.0f,
			rand() % int(MESHFIELD_SIZE * MESHFIELD_D*0.9f) - int(MESHFIELD_SIZE * MESHFIELD_D * 0.4f)),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	//�v���C���[�̏����ݒ�
	SetPlayer();

	//�P�̏�����
	SetPrincess();
	DeleteCharge();

	//�{�X�̏�����
	InitBoss();

	//�p�[�e�B�N���̏�����
	InitParticle();

	//PL���C�t�̏�����
	InitLife();

	//�P���C�t�̏�����
	InitPLife();

	//���@�Q�[�W�̏�����
	InitMagicGage();

	//�X�R�A�̏�����
	InitScore();
	InitCombo();
	InitEwave();
	InitEwaveUI();

	//�w���v�̏�����
	InitHelp();
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{
	//�G�l�~�[�̏���
	DeleteEnemey();

	//�{�X�̏I��
	UninitBoss();

	//�I�u�W�F�N�g�̏I��
	UninitObstacle();
	DeleteALLObstacle();

	//PL���C�t�̏I��
	UninitLife();

	//���@�Q�[�W�̏I��
	UninitMagicGage();

	//�w���v�̏I��
	UninitHelp();

	//�|�[�Y�̏I��
	UninitPause();

	//���C���̏I��
	UninitLine();

	//���S�̏I��
	DeleteLogo();

	//�p�[�e�B�N���̏I��
	UninitParticle();

	SetScore(false);
	SetCombo(false);
	SetEwaveUIresult(false);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{

	if (g_gameState != GAMESTATE_PAUSE && g_gameState != GAMESTATE_NONE)
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

		//��Q���̍X�V
		UpdateObstacle();

		//�P�̍X�V
		UpdatePrincess();

		//�v���C���[�̍X�V
		UpdatePlayer();

		//�G�̍X�V
		UpdateEnemy();

		//�{�X�̍X�V
		UpdateBoss();

		//�G�t�F�N�g�̍X�V
		UpdateEffect();
		UpdateTrajectory();

		//���@�̍X�V
		UpdateWave();

		//���C���̍X�V
		UpdateLine();

		//�p�[�e�B�N���̍X�V
		UpdateParticle();

		//PL���C�t�̍X�V
		UpdateLife();

		//�P���C�t�̍X�V
		UpdatePLife();

		//���@�Q�[�W�̍X�V
		UpdateMagicGage();

		//�w���v�̍X�V
		UpdateHelp();

		//�X�R�A�̍X�V
		UpdateScore();
		UpdateCombo();
		UpdateEwave();
		UpdateEwaveUI();

		switch (g_gameState)
		{
		case GAMESTATE_NORMAL:
			if (GetKeyTrigger(DIK_P) || GetJoystickTrigger(PAD_START) || GetxControllerTrigger(XINPUT_PAD_START))
			{	//�|�[�Y�̌Ăяo��
				g_gameState = GAMESTATE_PAUSE;
				PlaySound(SOUND_LABEL_SE_PAUSE);
			}
#ifdef _DEBUG
			if (GetKeyTrigger(DIK_I))
			{
				AddEwave(1);
			}
#endif
			break;

		case GAMESTATE_CLEAR:
			g_nCounterGameState++;
			if ((GetKeyTrigger(DIK_RETURN) || GetJoystickTrigger(PAD_START) || GetxControllerTrigger(XINPUT_PAD_START)))	//Enter�ŏI��
			{
				SetFade(MODE_RESULT);
				g_gameState = GAMESTATE_NONE;
			}
			if (g_nCounterGameState >= COUNTER_MAX * 5)
			{
				SetFade(MODE_RESULT);
				g_gameState = GAMESTATE_NONE;
			}
			break;

		case GAMESTATE_GAMEOVER:
			g_nCounterGameState++;
			if ((GetKeyTrigger(DIK_RETURN) || GetJoystickTrigger(PAD_START) || GetxControllerTrigger(XINPUT_PAD_START)))
			{
				SetFade(MODE_RESULT);
				g_gameState = GAMESTATE_NONE;
			}
			if (g_nCounterGameState >= COUNTER_MAX * 5)
			{
				SetFade(MODE_RESULT);
				g_gameState = GAMESTATE_NONE;
			}
			break;
		default:
			break;
		}
	}
	else if(g_gameState == GAMESTATE_PAUSE)
	{
		UpdatePause();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame(void)
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

	//�{�X�̕`��
	DrawBoss();

	//�v���C���[�̕`��
	DrawPlayer();

	//���f���̕`��
	DrawObstacle();

	//�V�����_�[�̕`��
	DrawMeshCylinder();

	//���C���̕`��
	DrawLine();

	//���@�̕`��
	DrawWave();

	//�G�t�F�N�g�̕`��
	DrawEffect();
	DrawTrajectory();

	//�p�[�e�B�N���̕`��
	DrawParticle();

	//PL���C�t�̕`��
	DrawLife();

	//�P���C�t�̕`��
	DrawPLife();

	//���@�Q�[�W�̕`��
	DrawMagicGage();

	//�w���v�̕`��
	DrawHelp();

	//�X�R�A�̕`��
	DrawScore();
	DrawCombo();
	DrawEwaveUI();

	if (g_gameState == GAMESTATE_PAUSE)
	{	//�|�[�Y�̕`��
		DrawPause();
	}
}

//=============================================================================
// �Q�[����Ԃ̐ݒ�
//=============================================================================
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterGameState = 0;
}

//=============================================================================
// �Q�[����Ԃ̎擾
//=============================================================================
GAMESTATE *GetGameState(void)
{
	return &g_gameState;
}
