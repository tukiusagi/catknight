//=============================================================================
//
// ���U���g����[result.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"result.h"
#include "input.h"	
#include "joystick.h"
#include "fade.h"
#include"logo.h"
#include"resultlogo.h"
#include "combo.h"
#include "score.h"
#include "enemywave.h"
#include "lastscore.h"
#include "sound.h"
#include "camera.h"
#include "player.h"
#include "effect.h"
#include "enemy.h"
#include "lifebar.h"
#include "magic.h"
#include "wave.h"
#include "trajectory.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define SECOND (60)	//�b��

//=============================================================================
// ���U���g�̏��
//=============================================================================
typedef enum
{
	RESULTSTATE_NONE,		//�ʏ�
	RESULTSTATE_SCORE,		//�X�R�A
	RESULTSTATE_COMBO,		//�R���{
	RESULTSTATE_WAVE,		//�E�F�[�u
	RESULTSTATE_LAST,		//�ŏI�X�R�A
	RESULTSTATE_NEXT,		//�����L���O��
	RESULTSTATE_MAX
} RESULTSTATE;

//=============================================================================
// ���U���g�̏��
//=============================================================================
typedef enum
{
	RESULT_NORMAL,		//�ʏ�
	RESULT_GETBOTTON,	//�{�^���������ꂽ
	RESULT_MAX
} RESULT;

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
RESULT ResultState;
RESULTSTATE g_NowState;
int g_nCntResult;
int g_Combo;
int g_nNumScore;
int g_Wave;

//=============================================================================
// ����������
//=============================================================================
void InitResult(void)
{
	ResultState = RESULT_NORMAL;
	g_NowState = RESULTSTATE_NONE;

	g_nCntResult = 0;

	//�ő�R���{�̎擾
	DeleteCombo();
	g_Combo=GetCombo();
	AddCombo(g_Combo);

	AddScore(GetLifeber() * 100);
	g_nNumScore = GetScore();
	g_Wave = *GetEwave();

	//�ŏI�X�R�A�̏�����
	InitLastScore();
	AddLastScore(g_nNumScore + g_Combo * 100 + g_Wave * 500);

	//���ʔ��\�̏�����
	InitReLogo();
	SetLogo(LOGO_RESULT);
}

//=============================================================================
// �I������
//=============================================================================
int UninitResult(void)
{
	// �J�����̏I������
	UninitCamera();

	//�G�̏I��
	UninitEnemy();

	// �v���C���[�̏I��
	UninitPlayer();

	//�G�t�F�N�g�̏I��
	UninitEffect();
	UninitTrajectory();

	//���@�w�̏I��
	UninitMagic();

	//���@�̏I��
	UninitWave();

	//�P���C�t�̏I��
	UninitLifebar();

	//���ʔ��\�̏I��
	UninitReLogo();

	//�X�R�A�̏I��
	UninitScore();
	UninitCombo();
	UninitEwave();
	UninitLastScore();

	//���S�̏I��
	DeleteLogo();

	return (g_nNumScore + g_Combo * 100 + g_Wave * 500);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResult(void)
{
	//���S�̍X�V
	UpdateReLogo();

	//�X�R�A�̍X�V
	UpdateScore();
	UpdateCombo();
	UpdateEwave();
	UpdateLastScore();

	g_nCntResult++;
	//���U���g�\��
	if (g_NowState == RESULTSTATE_NONE && ResultState != RESULT_GETBOTTON && (g_nCntResult == SECOND * 2 || (GetKeyTrigger(DIK_SPACE) || GetJoystickTrigger(PAD_B))))
	{
		SetScore(true);
		PlaySound(SOUND_LABEL_SE_PAUSE);
		ResultState = RESULT_GETBOTTON;
		g_NowState = RESULTSTATE_SCORE;
	}
	if (g_NowState == RESULTSTATE_SCORE && ResultState != RESULT_GETBOTTON && (g_nCntResult == SECOND * 3 || (GetKeyTrigger(DIK_SPACE) || GetJoystickTrigger(PAD_B))))
	{
		SetCombo(true);
		PlaySound(SOUND_LABEL_SE_PAUSE);
		ResultState = RESULT_GETBOTTON;
		g_NowState = RESULTSTATE_COMBO;
	}
	if (g_NowState == RESULTSTATE_COMBO && ResultState != RESULT_GETBOTTON && (g_nCntResult == SECOND * 4 || (GetKeyTrigger(DIK_SPACE) || GetJoystickTrigger(PAD_B))))
	{
		SetEwaveresult(true);
		PlaySound(SOUND_LABEL_SE_PAUSE);
		ResultState = RESULT_GETBOTTON;
		g_NowState = RESULTSTATE_WAVE;
	}
	if (g_NowState == RESULTSTATE_WAVE && ResultState != RESULT_GETBOTTON && (g_nCntResult == SECOND * 5 || (GetKeyTrigger(DIK_SPACE) || GetJoystickTrigger(PAD_B))))
	{
		SetLastScore(true);
		PlaySound(SOUND_LABEL_SE_CAT);
		ResultState = RESULT_GETBOTTON;
		g_NowState = RESULTSTATE_LAST;
	}
	if (g_NowState == RESULTSTATE_LAST && ResultState != RESULT_GETBOTTON && (GetKeyTrigger(DIK_SPACE) || GetJoystickTrigger(PAD_B)))	//Enter�ŏI��
	{
		SetFade(MODE_RANKING);
		ResultState = RESULT_GETBOTTON;
	}
	else if (g_nCntResult > SECOND * 10)
	{
		SetFade(MODE_RANKING);
		ResultState = RESULT_GETBOTTON;
		g_nCntResult = 0;
	}

	ResultState = RESULT_NORMAL;
}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void DrawResult(void)
{
	//���S�̕`��
	DrawReLogo();

	//�X�R�A�̕`��
	DrawCombo();
	DrawScore();
	DrawEwave();
	DrawLastScore();

}