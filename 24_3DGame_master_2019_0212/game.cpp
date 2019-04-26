//=============================================================================
//
// �Q�[������[game.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"game.h"	//�C���N���[�h�t�@�C��
#include "camera.h"
#include "light.h"
#include "player.h"
#include "input.h"
#include "joystick.h"
#include "shadow.h"
#include "meshfield.h"
#include "meshwall.h"
#include "effect.h"
#include "fade.h"
#include "line.h"
#include "princess.h"
#include "enemy.h"
#include "life.h"
#include "logo.h"
#include "meshcylinder.h"
#include "ground.h"
#include "lifebar.h"
#include "pause.h"
#include "boss.h"
#include "magic.h"
#include "wave.h"
#include "obstacle.h"
#include "trajectory.h"
#include "score.h"
#include "combo.h"
#include "enemywave.h"
#include "sound.h"
#include"help.h"
#include "particle.h"

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


	//�G�̏�����
	//InitEnemy();
	SetEnemy(D3DXVECTOR3(300, 0.0f, 300), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
	SetEnemy(D3DXVECTOR3(-300, 0.0f, 300), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
	SetEnemy(D3DXVECTOR3(300, 0.0f, -300), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
	SetEnemy(D3DXVECTOR3(-300, 0.0f, -300), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);

	//�`���[�W���Z�b�g
	DeleteCharge();

	//�v���C���[�̏����ݒ�
	SetPlayer();

	//�{�X�̏�����
	InitBoss();

	//�p�[�e�B�N���̏�����
	InitParticle();

	//PL���C�t�̏�����
	InitLife();

	//�P���C�t�̏�����
	InitLifebar();

	//�X�R�A�̏�����
	InitScore();
	InitCombo();
	InitEwave();

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
	SetEwaveresult(false);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:
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

		//���f���̍X�V
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
		UpdateLifebar();

		//�w���v�̍X�V
		UpdateHelp();

		//�X�R�A�̍X�V
		UpdateScore();
		UpdateCombo();
		UpdateEwave();

		if (GetKeyTrigger(DIK_P) || GetJoystickTrigger(PAD_START))
		{
			g_gameState = GAMESTATE_PAUSE;
			PlaySound(SOUND_LABEL_SE_PAUSE);
		}
		break;

	case GAMESTATE_CLEAR:
		// �J�����̍X�V����
		UpdateCamera();

		//���@�w�̍X�V
		UpdateMagic();

		//���@�̍X�V
		UpdateWave();

		//�G�t�F�N�g�̍X�V
		UpdateEffect();
		UpdateTrajectory();

		//�P�̍X�V
		UpdatePrincess();

		//�G�̍X�V
		UpdateEnemy();

		//�{�X�̍X�V
		UpdateBoss();

		//�v���C���[�̍X�V
		UpdatePlayer();

		//�p�[�e�B�N���̍X�V
		UpdateParticle();

		g_nCounterGameState++;
		if ((GetKeyTrigger(DIK_RETURN) || GetJoystickTrigger(PAD_START)))	//Enter�ŏI��
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
		// �J�����̍X�V����
		UpdateCamera();

		//���@�w�̍X�V
		UpdateMagic();

		//�G�t�F�N�g�̍X�V
		UpdateEffect();
		UpdateTrajectory();

		//�P�̍X�V
		UpdatePrincess();

		//�G�̍X�V
		UpdateEnemy();

		//�{�X�̍X�V
		UpdateBoss();

		//�v���C���[�̍X�V
		UpdatePlayer();

		//�p�[�e�B�N���̍X�V
		UpdateParticle();

		//�w���v�̍X�V
		UpdateHelp();

		g_nCounterGameState++;
		if ((GetKeyTrigger(DIK_RETURN) || GetJoystickTrigger(PAD_START)))	//Enter�ŏI��
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

	case GAMESTATE_PAUSE:
		//�|�[�Y�̍X�V
		UpdatePause();
		break;

	default:
		break;
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
	DrawLifebar();

	//�w���v�̕`��
	DrawHelp();

	//�X�R�A�̕`��
	DrawScore();
	DrawCombo();
	DrawEwave();

	switch (g_gameState)
	{
	case GAMESTATE_PAUSE:
		//�|�[�Y�̕`��
		DrawPause();
		break;
	default:
		break;
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
