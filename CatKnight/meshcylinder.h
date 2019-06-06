//=============================================================================
//
// ���b�V�������� [meshcylinder.h]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitMeshCylinder(void);
void UninitMeshCylinder(void);
void UpdateMeshCylinder(void);
void DrawMeshCylinder(void);

void SetMeshCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float radius, float height, int type);
#endif
