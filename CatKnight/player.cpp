//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "joystick.h"
#include "xController.h"

#include "camera.h"
#include "shadow.h"
#include "meshfield.h"
#include "line.h"
#include "enemy.h"
#include"game.h"
#include "boss.h"
#include "life.h"
#include "effect.h"
#include "obstacle.h"
#include "trajectory.h"
#include "combo.h"
#include "sound.h"
#include "particle.h"
#include "wave.h"
#include "magic.h"
#include "princess.h"
#include "fileload.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define PLAYER_TEXTURE	"data/TEXTURE/white.jpg"	//�ǂݍ��ރe�N�X�`����
#define PLAYER_FILENAME "data/motion_Cat.txt"	//���f���t�@�C����
#define PLAYER_SHADOW		(15.0f)			//�e�̑傫��
#define PLAYER_ROTSPEED		(0.1f)			//��]���x
#define PLAYER_KEY_MAX		(4)				//�L�[�̍ő吔
#define PLAYER_RANGE		(20)

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
#ifdef _DEBUG
void DrawPos(void);
#endif
void SwitchMotion();		//���[�V�����؂�ւ�

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
PLAYER				g_Player;			//�v���C���[���
COLLISION			g_Collision;		//�����蔻����
LPDIRECT3DTEXTURE9	g_pTexture[6] = {};	//�e�N�X�`���̃|�C���^
int	g_nIdxShadow;		//�e��ID
int g_nCntframe = 0;	//���݂̃t���[����
int g_nCntKey = 0;		//���݂̃L�[��
char acModelFileName[PLAYER_MAX][MAX_CHARA];	//���f���t�@�C�����i�[�p

#ifdef _DEBUG
LPD3DXFONT			g_pFontPL = NULL;			// �t�H���g�ւ̃|�C���^
D3DXVECTOR3			g_PLpos;
#endif

LOAD *pLoad = GetLoad(CHARA_PLAYER);

//=============================================================================
// ����������
//=============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�t�@�C���ǂݍ���
	LoadFile(PLAYER_FILENAME, CHARA_PLAYER);

	// �ʒu�E�����̏����ݒ�
	g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, -40.0f);
	g_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.nLife = pLoad->nMaxLife;
	g_Player.fRadius = pLoad->fRadius;
	g_Player.state = PLAYERSTATE_NEUTRAL;
	g_Player.bAttack = false;
	g_Player.bDamege = false;
	g_Player.nCntState = 0;

	g_Collision.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Collision.rot= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�e�̍쐬
	g_nIdxShadow = SetSadow(g_Player.pos, g_Player.rot, PLAYER_SHADOW, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));

	// X�t�@�C���̓ǂݍ���
	for (int nCntModel = 0; nCntModel < pLoad->nNumModel; nCntModel++)
	{
		D3DXLoadMeshFromX(pLoad->cFileNeme[nCntModel],	//�ǂݍ��ރt�@�C����
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_Player.aModel[nCntModel].pBuffMat,
			NULL,
			&g_Player.aModel[nCntModel].nNumMat,
			&g_Player.aModel[nCntModel].pMesh);
	}


	//�e�N�X�`���̓ǂݍ���
	for (int nCntTexture = 0; nCntTexture < 6; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice,
			PLAYER_TEXTURE,
			&g_pTexture[nCntTexture]);
	}

	//�O�Ղ̐ݒ�
	SetTrajectory(pLoad->Meshpos[0], pLoad->Meshpos[1], &g_Player.aModel[pLoad->nMeshParent].mtxWorld, D3DXCOLOR(1.0f,0.5f,0.0f,1.0f));

#ifdef _DEBUG
	// �f�o�b�O�\���p�t�H���g��ݒ�
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "�l�r �S�V�b�N", &g_pFontPL);
#endif

}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	for (int nCntParts = 0; nCntParts < PLAYER_MAX; nCntParts++)
	{
		// ���b�V���̊J��
		if (g_Player.aModel[nCntParts].pMesh != NULL)
		{
			g_Player.aModel[nCntParts].pMesh->Release();
			g_Player.aModel[nCntParts].pMesh = NULL;
		}

		// �}�e���A���̊J��
		if (g_Player.aModel[nCntParts].pBuffMat != NULL)
		{
			g_Player.aModel[nCntParts].pBuffMat->Release();
			g_Player.aModel[nCntParts].pBuffMat = NULL;
		}
	}

	//�e�N�X�`���̊J��
	for (int nCntTex = 0; nCntTex < 6; nCntTex++)
	{
		if (g_pTexture[nCntTex] != NULL)
		{
			g_pTexture[nCntTex]->Release();
			g_pTexture[nCntTex] = NULL;
		}
	}
#ifdef _DEBUG
	if (g_pFontPL != NULL)
	{// �f�o�b�O�\���p�t�H���g�̊J��
		g_pFontPL->Release();
		g_pFontPL = NULL;
	}
#endif

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	PRINCESS *pPrincess = GetPrincess();

	g_Player.posOld = g_Player.pos;
	g_Collision.posOld = pLoad->Collpos;

	if (g_Player.bDamege)
	{
		g_Player.nCntState++;
		if (g_Player.nCntState % 10 == 0)
		{	//�_���[�W��ԉ���
			g_Player.bDamege = false;
		}
	}

	MESH_TRAJECTORY *pTrajectory = GetTrajectory();
	if (g_Player.state == PLAYERSTATE_ATTACK01 || g_Player.state == PLAYERSTATE_ATTACK02)
	{
		pTrajectory->bUse = true;
	}
	else
	{
		pTrajectory->bUse = false;
	}

	//�t���[������
	if (g_Player.state == PLAYERSTATE_DETH && g_nCntKey >= 1)
	{
		//���S���͑������Ȃ�
	}
	else
	{
		g_nCntframe++;
		if (g_nCntframe > pLoad->MotionInfo[g_Player.state].KeyInfo[g_nCntKey].nFrame)
		{
			g_nCntframe = 0;
			g_nCntKey++;
			if (g_nCntKey >= pLoad->MotionInfo[g_Player.state].nNumKey)
			{
				g_nCntKey = 0;
			}
		}
	}

	//���[�V�����̐؂�ւ�
	SwitchMotion();

	if (g_Player.state != PLAYERSTATE_DETH)
	{
		if (g_Player.state != PLAYERSTATE_ATTACK01 && g_Player.state != PLAYERSTATE_ATTACK02)
		{
			//�C�ӂ̃L�[�������ꂽ���ǂ���
			OperationMove(&g_Player.move, pLoad->fSpeed);
		}
		if (!pPrincess->bHold && g_Player.state != PLAYERSTATE_WIN && (GetKeyTrigger(DIK_SPACE) || GetJoystickTrigger(PAD_B) || GetxControllerTrigger(XINPUT_PAD_B)))
		{	//�U��
			if (g_Player.state == PLAYERSTATE_ATTACK01)
			{	//��i�K��
				g_Player.state = PLAYERSTATE_ATTACK02;
				g_nCntframe = 0;
				g_nCntKey = 0;
			}
			if (g_Player.state == PLAYERSTATE_ATTACK02 && g_nCntKey >= 1)
			{	//��i�K�ڂɖ߂�
				g_Player.state = PLAYERSTATE_ATTACK01;
				g_nCntframe = 0;
				g_nCntKey = 0;
			}
			if ( g_Player.state != PLAYERSTATE_ATTACK01 && g_Player.state != PLAYERSTATE_ATTACK02)
			{	//��i�K
				g_Player.state = PLAYERSTATE_ATTACK01;
				g_nCntframe = 0;
				g_nCntKey = 0;
			}
			PlaySound(SOUND_LABEL_SE_SWORD);
			g_Player.bAttack = false;
		}

		float fDistX, fDistZ;
		float fDistans = 100000;
		fDistX = pPrincess->pos.x - g_Player.pos.x;
		fDistZ = pPrincess->pos.z - g_Player.pos.z;
		fDistans = (float)sqrt(fDistX * fDistX + fDistZ * fDistZ);

		if (pLoad->fRadius + PLAYER_RANGE > fDistans && (GetKeyboardPress(DIK_N) || GetJoystickPress(PAD_X) || GetxControllerPress(XINPUT_PAD_X)))
		{	//�v���C���[�̋߂��ɕP��������
			HoldPrincess(&g_Player,fDistX,fDistZ);
			g_Player.bHold = true;
		}
		else
		{
			g_Player.bHold = false;
			pPrincess->bHold = false;
		}
	}
#ifdef _DEBUG
	if (GetKeyTrigger(DIK_J))
	{	// �W�����v
		g_Player.move.y += 10.0f;
	}
#endif

	//��]
	RotationDir(g_Player.rotmove, g_Player.rotDest, g_Player.rot, PLAYER_ROTSPEED);

	//�d��
	g_Player.move.y -= GRAVITY;

	//����
	g_Player.move.x += (0.0f - g_Player.move.x) * INERTIA;
	g_Player.move.z += (0.0f - g_Player.move.z) * INERTIA;

	//�ړ�
	g_Player.pos.x += g_Player.move.x;
	g_Player.pos.z += g_Player.move.z;
	g_Player.pos.y += g_Player.move.y;

	//�����蔻��
	CollisionPrincess(&g_Player.pos, &g_Player.posOld, pLoad->fRadius, 0);

	//��Q��
	OBSTACLE *pObstacle = GetObstacle();
	for (int nCntObstacle = 0; nCntObstacle < OBSTACLE_MAX; nCntObstacle++, pObstacle++)
	{
		CollisionObstacle(&g_Player.pos, &g_Player.posOld, &g_Player.move, pLoad->fRadius, nCntObstacle);
	}

	BOSS *pBoss;
	pBoss = GetBoss();
	//�{�X
	for (int nCntBoss = 0; nCntBoss < MAX_ENEMY; nCntBoss++, pBoss++)
	{
		if (pBoss->bUse)
		{
			if ((g_Player.state == PLAYERSTATE_ATTACK01 || g_Player.state == PLAYERSTATE_ATTACK02) &&
				(!pBoss->bDamege && pBoss->state != BOSSSTATE_DETH) &&
				(pLoad->nStart <= g_nCntframe) && (g_nCntframe <= pLoad->nEnd))
			{
				if (CollisionBoss(&D3DXVECTOR3(g_Collision.mtxWorld._41, g_Collision.mtxWorld._42, g_Collision.mtxWorld._43), &g_Collision.posOld, pLoad->fCollRadius,nCntBoss))
				{
					//�{�X�̗͌���
					HitBoss(1, &g_Player.pos,nCntBoss);
					AddCombo(1);	//�R���{����
					g_Player.bAttack = true;
				}
			}
		}
	}
	//�G
	ENEMY *pEnemy;
	pEnemy = GetEnemy();
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse)
		{
			if ((g_Player.state == PLAYERSTATE_ATTACK01 || g_Player.state == PLAYERSTATE_ATTACK02) &&
				(pEnemy->state != ENEMYSTATE_DETH) &&
				(pLoad->nStart <= g_nCntframe) && (g_nCntframe <= pLoad->nEnd))
			{
				if (CollisionEnemy(&D3DXVECTOR3(g_Collision.mtxWorld._41, g_Collision.mtxWorld._42, g_Collision.mtxWorld._43), &g_Collision.posOld, pLoad->fCollRadius, nCntEnemy))
				{	//�G�̗͌���
					HitEnemy(nCntEnemy, 1, &g_Player.pos);
					if (*GetGameState() == GAMESTATE_NORMAL)
					{
						AddCombo(1);	//�R���{����
					}
					g_Player.bAttack = true;
				}
			}
		}
	}

	if (g_Player.pos.y < 0)
	{	//�n�ʂɂ߂荞�܂Ȃ��悤��
		g_Player.pos.y = 0;
		g_Player.move.y = 0;
	}

	//�ǂ߂荞�ݔ���
	CollisionWall(&g_Player.pos, pLoad->fRadius);

	//���[�V�����Đ�
	if (g_Player.state == PLAYERSTATE_DETH && g_nCntKey >= 1)
	{

	}
	else
	{
		PlayMotion();
	}

	//�e�̈ʒu��ݒ�
	SetPositionSadow(g_nIdxShadow, g_Player.pos, PLAYER_SHADOW + ((0.0f - g_Player.pos.y)*-0.1f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f - ((0.0f - g_Player.pos.y)*-0.008f)));

#ifdef _DEBUG
	g_PLpos = g_Player.pos;
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans, mtxParent;	// �v�Z�p�}�g���b�N�X
	D3DXMATERIAL		*pMat;	//���݂̃}�e���A���ۑ��p
	D3DMATERIAL9		matDef;	//�}�e���A���f�[�^�ւ̃|�C���^
	D3DMATERIAL9		matKeep;	//�ۑ��p

//�v���C���[�̐ݒ�
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Player.mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);

	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);

	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_Player.mtxWorld);

	//���f���̐ݒ�
	for (int nCntParts = 0; nCntParts < pLoad->nNumParts; nCntParts++)
	{
		//�e�̎���
		if (pLoad->nParent[nCntParts] == -1)
		{
			mtxParent = g_Player.mtxWorld;
		}
		else
		{
			mtxParent = g_Player.aModel[pLoad->nParent[nCntParts]].mtxWorld;
		}
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Player.aModel[nCntParts].mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pLoad->rot[nCntParts].y, pLoad->rot[nCntParts].x, pLoad->rot[nCntParts].z);

		D3DXMatrixMultiply(&g_Player.aModel[nCntParts].mtxWorld, &g_Player.aModel[nCntParts].mtxWorld, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pLoad->pos[nCntParts].x, pLoad->pos[nCntParts].y, pLoad->pos[nCntParts].z);

		D3DXMatrixMultiply(&g_Player.aModel[nCntParts].mtxWorld, &g_Player.aModel[nCntParts].mtxWorld, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

		//�e�q���f
		D3DXMatrixMultiply(&g_Player.aModel[nCntParts].mtxWorld, &g_Player.aModel[nCntParts].mtxWorld, &mtxParent);	//���[���h�s�񁁃��[���h�s�񁖐e�̃��[���h�s��

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Player.aModel[nCntParts].mtxWorld);

		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		// �}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_Player.aModel[nCntParts].pBuffMat->GetBufferPointer();

		// �e�N�X�`���̐ݒ������Ȃ炱��(1���̎�)

		for (int nCntMat = 0; nCntMat < (int)g_Player.aModel[nCntParts].nNumMat; nCntMat++)
		{
			// �e�N�X�`���̐ݒ������Ȃ炱��(�������̎�)
			pDevice->SetTexture(0, g_pTexture[nCntMat]);

			pMat[nCntMat].MatD3D.Ambient.r = pMat[nCntMat].MatD3D.Diffuse.r / 10;
			pMat[nCntMat].MatD3D.Ambient.g = pMat[nCntMat].MatD3D.Diffuse.g / 10;
			pMat[nCntMat].MatD3D.Ambient.b = pMat[nCntMat].MatD3D.Diffuse.b / 10;

			matKeep = pMat[nCntMat].MatD3D;	//�ۑ�

			if (g_Player.bDamege)
			{	//�_���[�W��ԂŐԐF�ɕύX
				pMat[nCntMat].MatD3D.Diffuse.r = 255;
			}

			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			pMat[nCntMat].MatD3D = matKeep;	//�F��߂�

			// �`��
			g_Player.aModel[nCntParts].pMesh->DrawSubset(nCntMat);	//�i�q���
		}

		// �}�e���A�����f�t�H���g�ɖ߂�
		pDevice->SetMaterial(&matDef);
	}
//�����蔻��̐ݒ�
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Collision.mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Collision.rot.y, g_Collision.rot.x, g_Collision.rot.z);

	D3DXMatrixMultiply(&g_Collision.mtxWorld, &g_Collision.mtxWorld, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pLoad->Collpos.x, pLoad->Collpos.y, pLoad->Collpos.z);

	D3DXMatrixMultiply(&g_Collision.mtxWorld, &g_Collision.mtxWorld, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

	//�e�q���f
	D3DXMatrixMultiply(&g_Collision.mtxWorld, &g_Collision.mtxWorld, &g_Player.aModel[pLoad->nCollParent].mtxWorld);	//���[���h�s�񁁃��[���h�s�񁖐e�̃��[���h�s��

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_Collision.mtxWorld);

#ifdef _DEBUG
	// PLpos�\��
	DrawPos();
#endif

}

//=============================================================================
// �v���C���[�̃Z�b�g
//=============================================================================
void SetPlayer(void)
{
	g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, -40.0f);
	g_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.nLife = pLoad->nMaxLife;
	g_Player.state = PLAYERSTATE_NEUTRAL;

}

//=============================================================================
// �������
//=============================================================================
void OperationMove(D3DXVECTOR3 *pMove,float fSpeed)
{
	CAMERA *pCamera;
	pCamera = GetCamera();

	if (GetKeyboardPress(DIK_A) || GetJoystickPress(PAD_LEFT_L) || GetxControllerPress(XINPUT_PAD_LEFT_L))
	{
		if (GetKeyboardPress(DIK_W) || GetJoystickPress(PAD_UP_L) || GetxControllerPress(XINPUT_PAD_UP_L))
		{	//����
			pMove->x += sinf(-D3DX_PI * 0.75f - pCamera->rot.y) * fSpeed;	//�����l
			pMove->z -= cosf(-D3DX_PI * 0.75f - pCamera->rot.y) * fSpeed;
			g_Player.rotDest.y = D3DX_PI * 0.75f + pCamera->rot.y;
		}
		else if (GetKeyboardPress(DIK_S) || GetJoystickPress(PAD_DOWN_L) || GetxControllerPress(XINPUT_PAD_DOWN_L))
		{	//����
			pMove->x += sinf(-D3DX_PI * 0.25f - pCamera->rot.y) * fSpeed;
			pMove->z -= cosf(-D3DX_PI * 0.25f - pCamera->rot.y) * fSpeed;
			g_Player.rotDest.y = D3DX_PI * 0.25f + pCamera->rot.y;
		}
		else
		{	//��
			pMove->x += sinf(-D3DX_PI * 0.5f + pCamera->rot.y) * fSpeed;
			pMove->z += cosf(-D3DX_PI * 0.5f + pCamera->rot.y) * fSpeed;
			g_Player.rotDest.y = D3DX_PI * 0.5f + pCamera->rot.y;
		}
	}
	else if (GetKeyboardPress(DIK_D) || GetJoystickPress(PAD_RIGHT_L) || GetxControllerPress(XINPUT_PAD_RIGHT_L))
	{
		if (GetKeyboardPress(DIK_W) || GetJoystickPress(PAD_UP_L))
		{	//�E��
			pMove->x += sinf(D3DX_PI * 0.75f - pCamera->rot.y) * fSpeed;
			pMove->z -= cosf(D3DX_PI * 0.75f - pCamera->rot.y) * fSpeed;
			g_Player.rotDest.y = -D3DX_PI * 0.75f + pCamera->rot.y;
		}
		else if (GetKeyboardPress(DIK_S) || GetJoystickPress(PAD_DOWN_L) || GetxControllerPress(XINPUT_PAD_DOWN_L))
		{	//�E��
			pMove->x += sinf(D3DX_PI * 0.25f - pCamera->rot.y) * fSpeed;
			pMove->z -= cosf(D3DX_PI * 0.25f - pCamera->rot.y) * fSpeed;
			g_Player.rotDest.y = -D3DX_PI * 0.25f + pCamera->rot.y;
		}
		else
		{	//�E
			pMove->x += sinf(D3DX_PI * 0.5f + pCamera->rot.y) * fSpeed;
			pMove->z += cosf(D3DX_PI * 0.5f + pCamera->rot.y) * fSpeed;
			g_Player.rotDest.y = -D3DX_PI * 0.5f + pCamera->rot.y;
		}
	}
	else if (GetKeyboardPress(DIK_W) || GetJoystickPress(PAD_UP_L) || GetxControllerPress(XINPUT_PAD_UP_L))
	{	//��
		pMove->x -= sinf(D3DX_PI * 1.0f + pCamera->rot.y) * fSpeed;
		pMove->z -= cosf(D3DX_PI * 1.0f + pCamera->rot.y) * fSpeed;
		g_Player.rotDest.y = D3DX_PI * 1.0f + pCamera->rot.y;
	}
	else if (GetKeyboardPress(DIK_S) || GetJoystickPress(PAD_DOWN_L) || GetxControllerPress(XINPUT_PAD_DOWN_L))
	{	//��O
		pMove->x -= sinf(D3DX_PI * 0.0f + pCamera->rot.y) * fSpeed;
		pMove->z -= cosf(D3DX_PI * 0.0f + pCamera->rot.y) * fSpeed;
		g_Player.rotDest.y = D3DX_PI * 0.0f + pCamera->rot.y;
	}
}

//=============================================================================
// �v���C���[���̎擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

//=============================================================================
// �����̉�]
//=============================================================================
void RotationDir(D3DXVECTOR3 &rotmove, D3DXVECTOR3 &rotDest, D3DXVECTOR3 &rot, float fSpeed)
{
	if (rotmove.y = rotDest.y - rot.y)
	{
		if (rotmove.y < -D3DX_PI)
		{
			rotmove.y += D3DX_PI * 2;
		}
		else if (rotmove.y > D3DX_PI)
		{
			rotmove.y -= D3DX_PI * 2;
		}
	}
	if (rot.y += rotmove.y * fSpeed)
	{
		if (rot.y > D3DX_PI)
		{
			rot.y -= D3DX_PI * 2;
		}
		else if (rot.y < -D3DX_PI)
		{
			rot.y += D3DX_PI * 2;
		}
	}

}

//=============================================================================
// �����蔻��
//=============================================================================
bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Radius)
{
	bool bLand = false;
	float fDistance = (pPos->x - g_Player.pos.x)*(pPos->x - g_Player.pos.x) + (pPos->z - g_Player.pos.z)*(pPos->z - g_Player.pos.z);	//2�_�Ԃ̋���
	float fLength = (Radius + pLoad->fRadius)*(Radius + pLoad->fRadius);	//���a
	float fDiff = (Radius + pLoad->fRadius) - (float)sqrt(fDistance);	//��
	D3DXVECTOR3 vec = D3DXVECTOR3((pPos->x - g_Player.pos.x), 0.0f, (pPos->z - g_Player.pos.z));	//�x�N�g��
	D3DXVec3Normalize(&vec, &vec);

	if (fDistance <= fLength)
	{
		bLand = true;

		pPos->x = pPos->x + vec.x * fDiff;
		pPos->z = pPos->z + vec.z * fDiff;
	}

	return bLand;
}

//=============================================================================
// �ǂ߂荞�ݔ���
//=============================================================================
void CollisionWall(D3DXVECTOR3 *pPos,float fRadius)
{
	//���E�ǔ���
	if (pPos->x > MESHFIELD_SIZE * MESHFIELD_W * 0.5f - fRadius)
	{
		pPos->x = MESHFIELD_SIZE * MESHFIELD_W * 0.5f - fRadius;
	}
	if (pPos->x < -MESHFIELD_SIZE * MESHFIELD_W * 0.5f + fRadius)
	{
		pPos->x = -MESHFIELD_SIZE * MESHFIELD_W * 0.5f + fRadius;
	}
	//����O�ǔ���
	if (pPos->z > MESHFIELD_SIZE * MESHFIELD_D * 0.5f - fRadius)
	{
		pPos->z = MESHFIELD_SIZE * MESHFIELD_D * 0.5f - fRadius;
	}
	if (pPos->z < -MESHFIELD_SIZE * MESHFIELD_D * 0.5f + fRadius)
	{
		pPos->z = -MESHFIELD_SIZE * MESHFIELD_D * 0.5f + fRadius;
	}
}
//=============================================================================
// �_���[�W����
//=============================================================================
void HitPlayer(int Damege, D3DXVECTOR3 *pPos)
{
	D3DXVECTOR3 vec = D3DXVECTOR3((g_Player.pos.x - pPos->x), 0.0f, (g_Player.pos.z - pPos->z));	//�x�N�g��
	D3DXVec3Normalize(&vec, &vec);

	DecreaseLife(Damege);
	//�q�b�g�G�t�F�N�g
	float fAngle = 0;
	float fSpeed = 5;
	for (int nCnt = 0; nCnt < 20; nCnt++)
	{
		fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;	//-3.14�`3.14�͈̔�
		D3DXVECTOR3 move = D3DXVECTOR3(sinf(fAngle)*fSpeed, cosf(fAngle)*fSpeed, 0.0f);
		SetParticle(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y + 20, g_Player.pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, 10, 10);
	}
	PlaySound(SOUND_LABEL_SE_DAMAGE);

	if (g_Player.state != PLAYERSTATE_DETH)
	{	//�ʏ�q�b�g
		g_Player.state = PLAYERSTATE_HIT;
		g_nCntframe = 0;
		g_nCntKey = 0;
		g_Player.bDamege = true;
		g_Player.move.x = g_Player.move.x + vec.x * 10;
		g_Player.move.z = g_Player.move.z + vec.z * 10;
	}

	if (g_Player.nLife <= 0)
	{	//���S��
		g_Player.state = PLAYERSTATE_DETH;
		g_nCntframe = 0;
		g_nCntKey = 0;

		g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, -30.0f);
		g_Player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player.nLife = pLoad->nMaxLife;
		IncreaseLife(pLoad->nMaxLife);
	}

}

//=============================================================================
//	���[�V�����̐؂�ւ�
//=============================================================================
void SwitchMotion()
{
	switch (*GetGameState())
	{
	case GAMESTATE_CLEAR:
		if (g_Player.state != PLAYERSTATE_ATTACK01 && g_Player.state != PLAYERSTATE_ATTACK02)
		{
			if (g_Player.move.x > LATEST_SPEED || g_Player.move.z > LATEST_SPEED ||
				g_Player.move.x < -LATEST_SPEED || g_Player.move.z < -LATEST_SPEED)
			{
				if (g_Player.bHold)
				{
					g_Player.state = PLAYERSTATE_HOLDMOVE;
				}
				else
				{
					g_Player.state = PLAYERSTATE_MOVE;
				}
			}
			else
			{
				g_Player.state = PLAYERSTATE_WIN;
			}
		}
		break;
	case GAMESTATE_GAMEOVER:
		if (g_Player.state != PLAYERSTATE_DETH)
		{
			g_Player.state = PLAYERSTATE_DETH;
			g_nCntframe = 0;
			g_nCntKey = 0;
		}
		break;
	default:
		if (g_Player.state != PLAYERSTATE_ATTACK01 && g_Player.state != PLAYERSTATE_ATTACK02 && g_Player.state != PLAYERSTATE_HIT)
		{
			if (g_Player.move.x > LATEST_SPEED || g_Player.move.z > LATEST_SPEED ||
				g_Player.move.x < -LATEST_SPEED || g_Player.move.z < -LATEST_SPEED)
			{
				if (g_Player.bHold)
				{
					g_Player.state = PLAYERSTATE_HOLDMOVE;
				}
				else
				{
					g_Player.state = PLAYERSTATE_MOVE;
				}
			}
			else
			{
				if (g_Player.bHold)
				{
					g_Player.state = PLAYERSTATE_HOLDNEUTRAL;
				}
				else
				{
					g_Player.state = PLAYERSTATE_NEUTRAL;
				}
			}
		}
		break;
	}
}

//=============================================================================
//	���[�V�����̍Đ�
//=============================================================================
void PlayMotion(void)
{
	//�p�[�c����
	for (int nCntMotion = 0; nCntMotion < PLAYER_MAX; nCntMotion++)
	{
		KEY *pKey, *pNextKey;
		int nNextKey = g_nCntKey + 1;

		//���[�v���邩�ǂ���
		if (nNextKey >= pLoad->MotionInfo[g_Player.state].nNumKey)
		{
			if (pLoad->MotionInfo[g_Player.state].bLoop)
			{
				nNextKey = 0;
			}
			else
			{
				if (!g_Player.bAttack && *GetGameState() == GAMESTATE_NORMAL)
				{
					DeleteCombo();	//�R���{�̓r�؂�
				}
				g_Player.state = PLAYERSTATE_NEUTRAL;
				g_nCntframe = 0;
				g_nCntKey = 0;
			}
		}

		pKey = &pLoad->MotionInfo[g_Player.state].KeyInfo[g_nCntKey].Key[nCntMotion];		//���݂̃L�[
		pNextKey = &pLoad->MotionInfo[g_Player.state].KeyInfo[nNextKey].Key[nCntMotion];	//���̃L�[

		//��]
		g_Player.aModel[nCntMotion].rotMove = (pNextKey->rot - pKey->rot)*((float)g_nCntframe / (float)pLoad->MotionInfo[g_Player.state].KeyInfo[g_nCntKey].nFrame);

		pLoad->rot[nCntMotion] = pKey->rot + g_Player.aModel[nCntMotion].rotMove;
	}
}

#ifdef _DEBUG
//=============================================================================
// pos�\������
//=============================================================================
void DrawPos(void)
{
	RECT rect = { 0, 50, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "pos�F%3.2f %3.2f %3.2f\n", g_PLpos.x, g_PLpos.y, g_PLpos.z);

	// �e�L�X�g�`��
	g_pFontPL->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}
#endif