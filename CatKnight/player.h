//=============================================================================
//
// �v���C���[���� [player.h]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define PLAYER_MAX		(7)		//�v���C���[�̑��p�[�c��
#define MAX_CHARA		(256)	//�ǂݍ��ޕ����̍ő吔
#define LATEST_SPEED	(0.5f)	//�ړ����x�̍Œ�l
#define GRAVITY			(0.8f)	//�d��
#define INERTIA			(0.25f)	//����

//=============================================================================
// �v���C���[�̏��
//=============================================================================
typedef enum
{
	PLAYERSTATE_NEUTRAL,		//�j���[�g����
	PLAYERSTATE_HOLDNEUTRAL,	//��Ȃ��j���[�g����
	PLAYERSTATE_MOVE,			//���s
	PLAYERSTATE_HOLDMOVE,		//��Ȃ����s
	PLAYERSTATE_ATTACK01,		//�U��01
	PLAYERSTATE_ATTACK02,		//�U��02
	PLAYERSTATE_HIT,			//�_���[�W
	PLAYERSTATE_DETH,			//���S
	PLAYERSTATE_WIN,			//����
	PLAYERSTATE_MAX				//���̍ő吔
} PLAYERSTATE;

//=============================================================================
// ���f���̍\����
//=============================================================================
typedef struct
{
	LPD3DXMESH		pMesh = NULL;		//���b�V�����̃|�C���^
	LPD3DXBUFFER	pBuffMat = NULL;	//�}�e���A�����ւ̃|�C���^
	DWORD			nNumMat = 0;		//�}�e���A�����̐�
	D3DXVECTOR3		posOld;				//�O��ʒu
	D3DXVECTOR3		rotMove;			//��]�ړ���
	D3DXMATRIX		mtxWorld;			//���[���h�}�g���b�N�X
} PLAYERMODEL;

//=============================================================================
// �v���C���[�̍\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;				//�ʒu
	D3DXVECTOR3	posOld;				//�O��ʒu
	D3DXVECTOR3	rot;				//����
	D3DXVECTOR3	rotDest;			//���B���������p�x
	D3DXVECTOR3	rotmove;			//��]�ړ���
	D3DXVECTOR3	move;				//�ړ�
	D3DXMATRIX	mtxWorld;			//���[���h�}�g���b�N�X
	float		fRadius;			//���a
	int			nLife;				//�̗�
	PLAYERMODEL	aModel[PLAYER_MAX];	//���f�����
	PLAYERSTATE	state;				//�v���C���[�̏��
	bool		bUse;				//�g�p���
	bool		bAttack;			//�U�����
	bool		bHold;				//����Ȃ��ł��邩
	int			nCntState;			//��ԃJ�E���^�[
	bool		bDamege;			//�U�����󂯂�
} PLAYER;

//=============================================================================
// �����蔻��̍\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3	posOld;				//�O��ʒu
	D3DXVECTOR3	rot;				//����
	D3DXVECTOR3	move;				//�ړ�
	D3DXMATRIX	mtxWorld;			//���[���h�}�g���b�N�X
}COLLISION;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Radius);	//�v���C���[�Ƃ̓����蔻��
void CollisionWall(D3DXVECTOR3 *pPos, float fRadius);	//�ǂ߂荞�ݔ���
void RotationDir(D3DXVECTOR3 &rotmove, D3DXVECTOR3 &rotDest, D3DXVECTOR3 &rot, float fSpeed);
void OperationMove(D3DXVECTOR3 *pMove, float fSpeed);	//�ړ�
void SetPlayer(void);		//�v���C���[�̐ݒ�
PLAYER *GetPlayer(void);	//�v���C���[�̎擾
void PlayMotion(void);		//���[�V�����Đ�
void HitPlayer(int Damege, D3DXVECTOR3 *pPos);	//�_���[�W����
#endif