//=============================================================================
//
// �P�̏��� [princess.h]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#ifndef _PRINCESS_H_
#define _PRINCESS_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "player.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define PRINCESS_MAX	(6)	//�P�̑��p�[�c��

//=============================================================================
// �P�̏��
//=============================================================================
typedef enum
{
	PRINCESSSTATE_NEUTRAL,		//�j���[�g����
	PRINCESSSTATE_HOLDNEUTRAL,	//��Ȃ��j���[�g����
	PRINCESSSTATE_MOVE,			//�ړ�
	PRINCESSSTATE_ATTACK,		//�U��
	PRINCESSSTATE_WIN,			//����
	PRINCESSSTATE_DETH,			//���S
	PRINCESSSTATE_MAX			//���̍ő吔
} PRINCESSSTATE;

//=============================================================================
// ���f���̍\����
//=============================================================================
typedef struct
{
	LPD3DXMESH		pMesh = NULL;		//���b�V�����̃|�C���^
	LPD3DXBUFFER	pBuffMat = NULL;	//�}�e���A�����ւ̃|�C���^
	DWORD			nNumMat = 0;		//�}�e���A�����̐�
	D3DXVECTOR3		rotMove;			//��]�ړ���
	D3DXMATRIX		mtxWorld;			//���[���h�}�g���b�N�X
} PRINCESSMODEL;

//=============================================================================
// �P�̍\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;						//�ʒu
	D3DXVECTOR3	posOld;						//�O��ʒu
	D3DXVECTOR3	rot;						//����
	D3DXVECTOR3	rotDest;					//���B���������p�x
	D3DXVECTOR3	rotmove;					//��]�ړ���
	D3DXVECTOR3	move;						//�ړ�
	D3DXMATRIX	mtxWorld;					//���[���h�}�g���b�N�X
	int			nCntFrame;					//���݂̃t���[����
	int			nCntKeyPrincess;			//���݂̃L�[��
	int			nCntState;					//��ԃJ�E���^�[
	int			nMaxLife;
	bool		bDamege;					//�U�����󂯂�
	bool		bPinch;						//�댯���ۂ�
	bool		bHold;						//����Ȃ��ł��邩�ǂ���
	PRINCESSMODEL	aModel[PRINCESS_MAX];	//���f�����
	PRINCESSSTATE	state;					//�P�̏��
	PRINCESSSTATE	stateOld;				//�O��̕P�̏��

} PRINCESS;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitPrincess(void);
void UninitPrincess(void);
void UpdatePrincess(void);
void DrawPrincess(void);

void SetPrincess(void);			//�P�̐ݒu
PRINCESS *GetPrincess(void);	//�P���擾
void PrincessMotion(void);		//���[�V�����Đ�
void HoldPrincess(PLAYER *pPlayer, float fDistX, float fDistZ);	//�A�����
bool CollisionPrincess(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Radius,int nType);	//�����蔻��
#endif