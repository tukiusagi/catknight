//=============================================================================
//
// �G���� [enemy.h]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define ENEMY_MAX			(6)		//�G�̑��p�[�c��
#define MAX_ENEMY			(60)	//�G�̍ő吔
#define DAMEGE_WAITTIME		(10)	//�_���[�W��Ԏ���
#define PARALYZE_WAITTIME	(180)	//�}�q��Ԏ���
#define ATTACK_TIMING		(180)	//�U���^�C�~���O

//=============================================================================
// �G�̏��
//=============================================================================
typedef enum
{
	ENEMYSTATE_NEUTRAL,	//�j���[�g����
	ENEMYSTATE_MOVE,	//���s
	ENEMYSTATE_ATTACK,	//�U��
	ENEMYSTATE_HIT,		//�_���[�W
	ENEMYSTATE_DETH,	//���S
	ENEMYSTATE_MAX		//���̍ő吔
} ENEMYSTATE;

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
} ENEMYMODEL;

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
}COLLISIONENEMY;

//=============================================================================
// �G�̍\����
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
	ENEMYMODEL	aModel[ENEMY_MAX];	//���f�����
	ENEMYSTATE	state;				//�G�̏��
	bool		bUse;				//�g�p���
	bool		bHit;				//PL�Ɠ�������
	bool		bDamege;			//�U�����󂯂�
	bool		bStop;				//�X�^�����
	int			nIdxShadowEnemy;	//�e��ID
	int			nCntFrameEnemy;		//���݂̃t���[����
	int			nCntKeyEnemy;		//���݂̃L�[��
	int			nCntAttack;			//�U���^�C�~���O
	int			nCntState;			//��ԃJ�E���^�[
	COLLISIONENEMY	CollisionEnemy;		//�����蔻����
} ENEMY;


//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

void CalculateDist(float &fDistX, float &fDistZ, float &fDistans, D3DXVECTOR3 pos);
void MoveEnemy(float fDistX, float fDistZ, float fDistans, int nCntEnemy);
void AttackEnemy(int nCntEnemy);
void HitEnemy(int nCntEnemy, int Damege, D3DXVECTOR3 *pPos);
void SetEnemy(D3DXVECTOR3 pos);
void DeleteEnemey(void);
ENEMY *GetEnemy(void);
int *GetNumEnemy(void);
void EnemyMotion(int nCntEnemy);
bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Radius,int nCntEnemy);

#endif