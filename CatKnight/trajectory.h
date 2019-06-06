#pragma once
//=============================================================================
//
// �O�Տ��� [trajectory.h]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#ifndef _TRAJECTORY_H_
#define _TRAJECTORY_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TRAJECTORY_POLYGON			(10)				//�ő�|���S����

//=============================================================================
// �O�Ղ̍\����
//=============================================================================
typedef struct
{
	D3DXMATRIX	mtxWorld;		//���[���h�}�g���b�N�X
	D3DXMATRIX	aMtxPoint[2];	//�I�t�Z�b�g�̃}�g���b�N�X
	D3DXMATRIX	*pMtxParent;	//�e�̃}�g���b�N�X
	D3DXVECTOR3	offset[2];		//�I�t�Z�b�g
	D3DXVECTOR3	pos[TRAJECTORY_POLYGON * 2 + 2];			//���_�̈ʒu���i���_�����j
	D3DXVECTOR3	rot[2];			//����
	D3DXCOLOR	col;			//�F
	int			nNumVertex;		//�����_��
	int			nNumPolygon;	//���|���S����
	bool		bUse;			//�g�p���Ă��邩
}MESH_TRAJECTORY;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitTrajectory(void);
void UninitTrajectory(void);
void UpdateTrajectory(void);
void DrawTrajectory(void);
void SetTrajectory(D3DXVECTOR3 offset00, D3DXVECTOR3 offset01, D3DXMATRIX *mtxParent, D3DXCOLOR	col);

MESH_TRAJECTORY *GetTrajectory();
#endif
