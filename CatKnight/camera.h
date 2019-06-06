//=============================================================================
//
// �J�������� [camera.h]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �J�����̍\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3 posV;			//���݂̎��_
	D3DXVECTOR3 posR;			//���݂̒����_(�Q�Ɠ_)
	D3DXVECTOR3 posVDest;		//�ړI�̎��_
	D3DXVECTOR3 posRDest;		//�ړI�̒����_(�Q�Ɠ_)
	D3DXVECTOR3 vecU;			//������x�N�g��
	D3DXVECTOR3 moveV;			//���_�ړ�
	D3DXVECTOR3 moveR;			//�����_�ړ�
	D3DXVECTOR3 rot;			//��]
	D3DXVECTOR3 rotDest;		//�ړI�̉�]
	D3DXMATRIX  mtxProjection;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX  mtxView;		//�r���[�}�g���b�N�X
	float fLength;				//����
}CAMERA;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);

CAMERA *GetCamera(void);
#endif