//=============================================================================
//
// �{�X���� [boss.h]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define BOSS_MAX	(6)	//�G�̑��p�[�c��
#define MAX_BOSS	(5)				//�G�̍ő吔

//=============================================================================
// �G�̏��
//=============================================================================
typedef enum
{
	BOSSSTATE_NEUTRAL,	//�j���[�g����
	BOSSSTATE_MOVE,		//���s
	BOSSSTATE_ATTACK,	//�U��
	BOSSSTATE_HIT,		//�_���[�W
	BOSSSTATE_DETH,		//���S
	BOSSSTATE_MAX		//���̍ő吔
} BOSSSTATE;

//=============================================================================
// ���f���̍\����
//=============================================================================
typedef struct
{
	LPD3DXMESH		pMesh = NULL;		//���b�V�����̃|�C���^
	LPD3DXBUFFER	pBuffMat = NULL;	//�}�e���A�����ւ̃|�C���^
	DWORD			nNumMat = 0;		//�}�e���A�����̐�
	D3DXVECTOR3		pos;				//�ʒu
	D3DXVECTOR3		posMove;			//�ʒu�ړ���
	D3DXVECTOR3		rot;				//����
	D3DXVECTOR3		rotMove;			//��]�ړ���
	D3DXMATRIX		mtxWorld;			//���[���h�}�g���b�N�X
	int				nIdxModelParent;	//�e���f���̃C���f�b�N�X
	char			*pFilename = NULL;	// �t�@�C����
} BOSSMODEL;

//=============================================================================
// �����蔻��̍\����
//=============================================================================
typedef struct
{
	int			nIdxModelParent;	//�e���f���̃C���f�b�N�X
	D3DXVECTOR3	pos;				//�I�t�Z�b�g�l
	D3DXVECTOR3	posOld;				//�O��ʒu
	D3DXVECTOR3	rot;				//����
	D3DXVECTOR3	move;				//�ړ�
	float		fRadius;			//���a
	D3DXMATRIX	mtxWorld;			//���[���h�}�g���b�N�X
	int			nStartFrame;		//�J�n����t���[��
	int			nEndFrame;			//�I������t���[��
}COLLISIONBOSS;

//=============================================================================
// �{�X�̍\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;				//�ʒu
	D3DXVECTOR3	posOld;				//�O��ʒu
	D3DXVECTOR3	rot;				//����
	D3DXVECTOR3	rotDest;			//���B���������p�x
	D3DXVECTOR3	rotmove;			//��]�ړ���
	D3DXVECTOR3	move;				//�ړ�
	float		fSpeed;				//�ړ����x
	D3DXMATRIX	mtxWorld;			//���[���h�}�g���b�N�X
	float		fRadius;			//���a
	int			nLife;				//�̗�
	int			nAttack;			//�U����
	BOSSMODEL	aModel[BOSS_MAX];	//���f�����
	BOSSSTATE	state;				//�G�̏��
	bool		bUse;				//�g�p���
	bool		bHit;				//PL�Ɠ�������
	bool		bDamege;			//�U�����󂯂�
	bool		bStop;				//�X�^�����
	int			nCntState;			//��ԃJ�E���^�[
	int			nIdxShadowBoss;		//�e��ID
	int			nCntFrameBoss;		//���݂̃t���[����
	int			nCntKeyBoss;		//���݂̃L�[��
	int			nCntAttack;			//�U���^�C�~���O
	COLLISIONBOSS	CollisionBoss;		//�����蔻����
} BOSS;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);

void MoveBoss(float fDistX, float fDistZ, float fDistans,int nCntBoss);
void AttackBoss(int nCntBoss);
void HitBoss(int Damege, D3DXVECTOR3 *pPos,int nCntBoss);
void SetBoss(D3DXVECTOR3 pos);
void DeleteBoss();
BOSS *GetBoss(void);
int GetBossNum(void);
void BossMotion(int nCntBoss);
bool CollisionBoss(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Radius, int nCntBoss);
#endif