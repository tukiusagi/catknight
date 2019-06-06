//=============================================================================
//
// �J�������� [camera.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "joystick.h"
#include "player.h"
#include"game.h"	

//=============================================================================
// �}�N����`
//=============================================================================
#define CAMERA_HEIGHT	(150.0f)	//����
#define CAMERA_DEPTH	(-200.0f)	//���s
#define CAMERA_MOVE		(0.1f)		//�ړ���
#define CAMERA_INERTIA	(0.4f)		//����
#define CAMERA_ROLL		(0.03f)		//��]��
#define CAMERA_DEST		(5.0f)		//�ړI�܂ł̋���
#define CAMERA_ROTMOVE	(0.01f)		//�W��

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
CAMERA g_camera;
int g_nCntMove;

//=============================================================================
// �J�����̏���������
//=============================================================================
void InitCamera(void)
{
	g_camera.posV = D3DXVECTOR3(0.0f, CAMERA_HEIGHT, CAMERA_DEPTH);	//���݂̎��_
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���݂̒����_
	g_camera.posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړI�̎��_
	g_camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړI�̒����_
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//������x�N�g��
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//��]
	g_camera.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړI�̉�]

	g_camera.fLength = sqrtf(((g_camera.posR.x - g_camera.posV.x)*(g_camera.posR.x - g_camera.posV.x)) + ((g_camera.posR.z - g_camera.posV.z)*(g_camera.posR.z - g_camera.posV.z)));

	g_nCntMove = 0;
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{
	
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	if (*GetMode() == MODE_TITLE || *GetMode() == MODE_RANKING)
	{
		g_camera.posV = D3DXVECTOR3(0.0f, 130, -300);
		g_camera.posR = D3DXVECTOR3(0.0f, 30.0f, 0.0f);	
		g_camera.fLength = sqrtf(((g_camera.posR.x - g_camera.posV.x)*(g_camera.posR.x - g_camera.posV.x)) + ((g_camera.posR.z - g_camera.posV.z)*(g_camera.posR.z - g_camera.posV.z)));

		//���_�E����
		g_camera.rot.y -= CAMERA_ROLL*0.1f;
		if (g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2;
		}
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fLength;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fLength;

	}
	else
	{
		//�Ǐ]�ړ�
		//�ڕW�����_
		if (pPlayer->move.x < 0)
		{
			g_camera.posRDest.x = pPlayer->pos.x + sinf(pPlayer->rot.y) * CAMERA_DEST * pPlayer->move.x;
		}
		else
		{
			g_camera.posRDest.x = pPlayer->pos.x - sinf(pPlayer->rot.y) * CAMERA_DEST * pPlayer->move.x;
		}
		if (pPlayer->move.z < 0)
		{
			g_camera.posRDest.z = pPlayer->pos.z + cosf(pPlayer->rot.y) * CAMERA_DEST * pPlayer->move.z;
		}
		else
		{
			g_camera.posRDest.z = pPlayer->pos.z - cosf(pPlayer->rot.y) * CAMERA_DEST * pPlayer->move.z;
		}
		g_camera.posRDest.y = pPlayer->pos.y + 5;

		//�ڕW���_
		g_camera.posVDest.x = pPlayer->pos.x - sinf(g_camera.rot.y) * g_camera.fLength;
		g_camera.posVDest.z = pPlayer->pos.z - cosf(g_camera.rot.y) * g_camera.fLength;
		g_camera.posVDest.y = pPlayer->pos.y + CAMERA_HEIGHT;


#ifdef _DEBUG
		if (GetKeyboardPress(DIK_C) || GetJoystickPress(PAD_ZR))
		{	//���_�E����
			g_camera.rot.y -= CAMERA_ROLL;
			if (g_camera.rot.y < -D3DX_PI)
			{
				g_camera.rot.y += D3DX_PI * 2;
			}
			g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fLength;
			g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fLength;
		}
		else if (GetKeyboardPress(DIK_Z) || GetJoystickPress(PAD_ZL))
		{	//������
			g_camera.rot.y += CAMERA_ROLL;
			if (g_camera.rot.y > D3DX_PI)
			{
				g_camera.rot.y -= D3DX_PI * 2;
			}
			g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fLength;
			g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fLength;
		}

#endif

		//�����_
		g_camera.posR.x += (g_camera.posRDest.x - g_camera.posR.x)*CAMERA_MOVE;
		g_camera.posR.z += (g_camera.posRDest.z - g_camera.posR.z)*CAMERA_MOVE;
		g_camera.posR.y += (g_camera.posRDest.y - g_camera.posR.y)*0.03f;


		//���_
		g_camera.posV.x += (g_camera.posVDest.x - g_camera.posV.x)*CAMERA_MOVE;
		g_camera.posV.z += (g_camera.posVDest.z - g_camera.posV.z)*CAMERA_MOVE;
		g_camera.posV.y += (g_camera.posVDest.y - g_camera.posV.y)*0.03f;
	}
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
								D3DXToRadian(45.0f),						//����p
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	//�A�X�y�N�g��
								10.0f,										//NearZ�l
								10000.0f);									//FarZ�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView,
						&g_camera.posV,		//�J�����̎��_
						&g_camera.posR,		//�J�����̒����_
						&g_camera.vecU);	//�J�����̏�����x�N�g��

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//=============================================================================
// �J�����̎擾
//=============================================================================
CAMERA *GetCamera(void)
{
	return&g_camera;
}
