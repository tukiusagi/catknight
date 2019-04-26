//=============================================================================
//
// �����L���O��ʏ���[ranking.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include"ranking.h"
#include "input.h"	
#include "joystick.h"
#include "fade.h"
#include "camera.h"
#include "light.h"
#include "meshfield.h"
#include "meshwall.h"
#include"logo.h"
#include "meshcylinder.h"
#include "ground.h"
#include "rank.h"
#include "princess.h"
#include "shadow.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define SECOND (60)

//=============================================================================
// �����L���O�̏��
//=============================================================================
typedef enum
{
	RANK_NORMAL,
	RANK_GETBOTTON,
	RANK_MAX
} RANKING;

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
RANKING RankState;
int g_nCntRanking;

//=============================================================================
// �|���S���̏���������
//=============================================================================
void InitRanking(void)
{
	RankState = RANK_NORMAL;
	g_nCntRanking = 0;

	// �J�����̏���������
	InitCamera();

	//�����L���O�̏�����
	InitRank();

	SetLogo(LOGO_RANKING);
}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void UninitRanking(void)
{
	// �J�����̏I������
	UninitCamera();

	// ���C�g�̏I������
	UninitLight();

	//�e�̏I��
	UninitShadow();

	//�P�̏I��
	UninitPrincess();

	//�V�����_�[�̏I��
	UninitMeshCylinder();

	//�n�ʂ̏I��
	UninitGround();

	//�t�B�[���h�̏I��
	UninitMeshField();

	//���b�V���ǂ̏I��
	UninitMeshWall();

	//���S�̏I��
	DeleteLogo();

	//�����L���O�̏I��
	UninitRank();
}

//=============================================================================
// �|���S���̍X�V����
//=============================================================================
void UpdateRanking(void)
{
	// �J�����̍X�V����
	UpdateCamera();

	// ���C�g�̍X�V����
	UpdateLight();

	//�V�����_�[�̍X�V
	UpdateMeshCylinder();

	//�n�ʂ̍X�V
	UpdateGround();

	//�t�B�[���h�̍X�V
	UpdateMeshField();

	//���b�V���ǂ̍X�V
	UpdateMeshWall();

	//�����L���O�̍X�V
	UpdateRank();

	g_nCntRanking++;
	if (RankState == RANK_NORMAL && (GetKeyTrigger(DIK_SPACE)|| GetJoystickTrigger(PAD_B)))	//Enter�ŏI��
	{
		SetFade(MODE_TITLE);
		RankState = RANK_GETBOTTON;
	}
	else if (g_nCntRanking > SECOND * 5)
	{
		SetFade(MODE_TITLE);
		g_nCntRanking = 0;
	}
}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void DrawRanking(void)
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

	//�����L���O�̕`��
	DrawRank();
}