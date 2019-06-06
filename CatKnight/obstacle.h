//=============================================================================
//
// ��Q������ [obstacle.h]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define OBSTACLE_MAX		(30)		//���f����

//=============================================================================
// ���f���̍\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;			//�ʒu
	D3DXVECTOR3	rot;			//����
	D3DXVECTOR3 move;			//�ړ�
	D3DXVECTOR3 rotDest;		//���B���������p�x
	D3DXMATRIX	mtxWorldObstacle;	//���[���h�}�g���b�N�X
	int			nIdxShadow;		//�e��ID
	int			nLife;			//�ϋv��
	int			nCntState;		//��ԃJ�E���^�[
	D3DXVECTOR3	vtxMin;			//�ŏ��l
	D3DXVECTOR3	vtxMax;			//�ő�l
	bool		bUse;			//�g�p���
	bool		bHit;			//�U�����󂯂�
}OBSTACLE;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitObstacle(void);
void UninitObstacle(void);
void UpdateObstacle(void);
void DrawObstacle(void);

OBSTACLE *GetObstacle(void);
void SetObstacle(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void DeleteObstacle(int Damege,int nCntObstacle);
void DeleteALLObstacle(void);

bool CollisionObstacle(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3*pMove,float Radius, int nCntbasin);
#endif
