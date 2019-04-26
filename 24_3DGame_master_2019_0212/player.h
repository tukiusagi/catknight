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
#define PLAYER_MAX	(7)		//�v���C���[�̑��p�[�c��
#define MAX_CHARA	(256)	//�ǂݍ��ޕ����̍ő吔

//=============================================================================
// �v���C���[�̏��
//=============================================================================
typedef enum
{
	PLAYERSTATE_NEUTRAL,	//�j���[�g����
	PLAYERSTATE_MOVE,		//���s
	PLAYERSTATE_ATTACK01,	//�U��01
	PLAYERSTATE_ATTACK02,	//�U��02
	PLAYERSTATE_HIT,		//�_���[�W
	PLAYERSTATE_DETH,		//���S
	PLAYERSTATE_WIN,		//����
	PLAYERSTATE_MAX			//���̍ő吔
} PLAYERSTATE;

//=============================================================================
// ���f���̍\����
//=============================================================================
typedef struct
{
	LPD3DXMESH		pMesh = NULL;		//���b�V�����̃|�C���^
	LPD3DXBUFFER	pBuffMat = NULL;	//�}�e���A�����ւ̃|�C���^
	DWORD			nNumMat = 0;		//�}�e���A�����̐�
	D3DXVECTOR3		pos;				//�ʒu
	D3DXVECTOR3		posOld;				//�O��ʒu
	D3DXVECTOR3		rot;				//����
	D3DXVECTOR3		rotMove;			//��]�ړ���
	D3DXMATRIX		mtxWorld;			//���[���h�}�g���b�N�X
	int				nIdxModelParent;	//�e���f���̃C���f�b�N�X
	char			*pFilename = NULL;	// �t�@�C����
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
	float		fSpeed;				//�ړ����x
	D3DXMATRIX	mtxWorld;			//���[���h�}�g���b�N�X
	float		fRadius;			//���a
	int			nLife;				//�̗�
	int			nMaxLife;			//�ő�̗�
	PLAYERMODEL	aModel[PLAYER_MAX];	//���f�����
	PLAYERSTATE	state;				//�v���C���[�̏��
	bool		bUse;				//�g�p���
	bool		bAttack;			//�U�����
	int			nCntState;			//��ԃJ�E���^�[
	bool		bDamege;			//�U�����󂯂�
} PLAYER;

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
}COLLISION;

//=============================================================================
// �O�Ղ̍\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos[2];			//2�_�̃I�t�Z�b�g�l
	int			nParent;		//�e�̔ԍ�
}MESH_ORBIT;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Radius);

PLAYER *GetPlayer(void);
void PlayMotion(void);
void HitPlayer(int Damege, D3DXVECTOR3 *pPos);
void LoadPlayer(void);
void SetPlayer(void);
#endif