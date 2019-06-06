//=============================================================================
//
// �P�̏��� [princess.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "princess.h"
#include "input.h"
#include "joystick.h"
#include "xController.h"

#include "camera.h"
#include "shadow.h"
#include "meshfield.h"
#include "line.h"
#include"game.h"
#include "enemy.h"
#include "wave.h"
#include "sound.h"
#include "magic.h"
#include "fileload.h"
#include "magicGage.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define PRINCESS_TEXTURE	"data/TEXTURE/mizuiro.jpg"	//�ǂݍ��ރe�N�X�`����
#define PRINCESS_TEXTURE2	"data/TEXTURE/white2.jpg"	//�ǂݍ��ރe�N�X�`����
#define PRINCESS_FILENAME	"data/motion_Princess.txt"	//�ǂݍ��ރt�@�C����

#define PRINCESS_SHADOW		(15.0f)			//�e�̑傫��
#define PRINCESS_ROTSPEAD	(0.1f)			//��]���x
#define PRINCESS_KEY_MAX	(4)				//�L�[�̍ő吔
#define PRINCESS_TEX_MAX	(6)				//�e�N�X�`���̍ő吔
#define MAX_CHARA			(256)			//�ǂݍ��ޕ����̍ő吔
#define INTERBAL_FRAME		(10)			//�⊮�t���[��

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void SwitchMotionPrincess();			//���[�V�����؂�ւ�

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
PRINCESS			g_Princess;					//�P�̏��
LPDIRECT3DTEXTURE9	g_pTexturePrincess[PRINCESS_TEX_MAX] = {};	//�e�N�X�`���̃|�C���^
int	g_nIdxShadowPrincess;		//�e��ID
char g_PrincessFileNeme[PRINCESS_MAX][MAX_CHARA];

LOAD *pLoadPrincess = GetLoad(CHARA_PRINCESS);
int g_nFrame = 40;
bool g_bMotion;

//=============================================================================
// ����������
//=============================================================================
void InitPrincess(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�f�[�^�̃��[�h
	LoadFile(PRINCESS_FILENAME, CHARA_PRINCESS);
	g_bMotion = false;

	// �ʒu�E�����̏����ݒ�
	g_Princess.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Princess.state = PRINCESSSTATE_NEUTRAL;
	g_Princess.stateOld = PRINCESSSTATE_NEUTRAL;

	g_Princess.bDamege = false;
	g_Princess.bPinch = false;
	g_Princess.bHold = false;

	g_Princess.nMaxLife = pLoadPrincess->nMaxLife;
	g_Princess.nCntState = 0;
	g_Princess.nCntFrame = 0;
	g_Princess.nCntKeyPrincess = 0;

	//�e�̍쐬
	g_nIdxShadowPrincess = SetSadow(g_Princess.pos, g_Princess.rot, PRINCESS_SHADOW, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
	if (*GetMode() != MODE_TITLE)
	{
		SetMagic(g_Princess.pos);
	}

	// X�t�@�C���̓ǂݍ���
	for (int nCntModel = 0; nCntModel < pLoadPrincess->nNumModel; nCntModel++)
	{
		D3DXLoadMeshFromX(pLoadPrincess->cFileNeme[nCntModel],	//�ǂݍ��ރt�@�C����
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_Princess.aModel[nCntModel].pBuffMat,
			NULL,
			&g_Princess.aModel[nCntModel].nNumMat,
			&g_Princess.aModel[nCntModel].pMesh);

	}

	//�e�N�X�`���̓ǂݍ���
	for (int nCntTex = 0; nCntTex < PRINCESS_TEX_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			PRINCESS_TEXTURE2,
			&g_pTexturePrincess[nCntTex]);
	}

	D3DXCreateTextureFromFile(pDevice,
		PRINCESS_TEXTURE,
		&g_pTexturePrincess[2]);

}

//=============================================================================
// �I������
//=============================================================================
void UninitPrincess(void)
{
	for (int nCntPrincess = 0; nCntPrincess < pLoadPrincess->nNumModel; nCntPrincess++)
	{
		// ���b�V���̊J��
		if (g_Princess.aModel[nCntPrincess].pMesh != NULL)
		{
			g_Princess.aModel[nCntPrincess].pMesh->Release();
			g_Princess.aModel[nCntPrincess].pMesh = NULL;
		}

		// �}�e���A���̊J��
		if (g_Princess.aModel[nCntPrincess].pBuffMat != NULL)
		{
			g_Princess.aModel[nCntPrincess].pBuffMat->Release();
			g_Princess.aModel[nCntPrincess].pBuffMat = NULL;
		}
	}

	//�e�N�X�`���̊J��
	for (int nCntTex = 0; nCntTex < PRINCESS_TEX_MAX; nCntTex++)
	{
		if (g_pTexturePrincess[nCntTex] != NULL)
		{
			g_pTexturePrincess[nCntTex]->Release();
			g_pTexturePrincess[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePrincess(void)
{
	WAVE *pWave = GetWave();
	g_Princess.bPinch = false;
	g_Princess.posOld = g_Princess.pos;

	if (g_Princess.bDamege)
	{	//�_���[�W���
		g_Princess.nCntState++;
		if (g_Princess.nCntState % 10 == 0)
		{
			g_Princess.bDamege = false;	//��ԉ���
		}
	}

	//�t���[��
	if (g_Princess.state == PRINCESSSTATE_DETH && g_Princess.nCntKeyPrincess >= 1)
	{	//���S���ɂ͑������Ȃ�
	}
	else
	{	//�t���[������
		g_Princess.nCntFrame++;
		if (!g_bMotion)
		{
			if (g_Princess.nCntFrame > pLoadPrincess->MotionInfo[g_Princess.state].KeyInfo[g_Princess.nCntKeyPrincess].nFrame)
			{
				g_Princess.nCntFrame = 0;
				g_Princess.nCntKeyPrincess++;	//�L�[����
				if (g_Princess.nCntKeyPrincess >= pLoadPrincess->MotionInfo[g_Princess.state].nNumKey)
				{
					g_Princess.nCntKeyPrincess = 0;
				}
			}
		}
		else
		{
			if (g_Princess.nCntFrame > g_nFrame)
			{
				g_Princess.nCntFrame = 0;
				g_Princess.stateOld = g_Princess.state;
			}
		}
	}

	if (*GetMode() == MODE_GAME || *GetMode() == MODE_TUTORIAL)
	{
		if (g_Princess.bHold)
		{	//�ړ����͖��@������
			DeleteMagic();
			DeleteWave();
		}
		else
		{	//���@�Z�b�g
			SetMagic(g_Princess.pos);
			SetWave(g_Princess.pos);
		}
	}

	//���[�V�����̐؂�ւ�
	if (*GetMode() == MODE_GAME)
	{
		SwitchMotionPrincess();
	}
	else
	{
		if (g_Princess.move.x > LATEST_SPEED || g_Princess.move.z > LATEST_SPEED ||
			g_Princess.move.x < -LATEST_SPEED || g_Princess.move.z < -LATEST_SPEED)
		{	//���x������
			g_Princess.state = PRINCESSSTATE_MOVE;
		}
		else if (g_Princess.state != PRINCESSSTATE_ATTACK && g_Princess.state != PRINCESSSTATE_WIN)
		{	//�ʏ펞
			if (g_Princess.bHold)
			{	//����Ȃ��ł�
				g_Princess.state = PRINCESSSTATE_HOLDNEUTRAL;
			}
			else
			{	//1�l
				g_Princess.state = PRINCESSSTATE_NEUTRAL;
			}
		}
	}

	if (pWave->state == WAVE_STATE_CHARGEMAX)
	{

		if ((GetKeyTrigger(DIK_B) || GetJoystickTrigger(PAD_Y) || GetxControllerTrigger(XINPUT_PAD_Y)))
		{	//�{�^�����������疂�@
			MAGIC *pMagic = GetMagic();
			g_Princess.state = PRINCESSSTATE_ATTACK;
			g_Princess.nCntFrame = 0;
			g_Princess.nCntKeyPrincess = 0;
			pWave->state = WAVE_STATE_ATTACK;
			pWave->col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
			pMagic->col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
			DecreaseMagicGage();
			PlaySound(SOUND_LABEL_SE_MAGIC);
		}
	}

	//��]
	RotationDir(g_Princess.rotmove, g_Princess.rotDest, g_Princess.rot, PRINCESS_ROTSPEAD);

	//�d��
	g_Princess.move.y -= GRAVITY;

	//����
	g_Princess.move.x += (0.0f - g_Princess.move.x)*INERTIA;
	g_Princess.move.z += (0.0f - g_Princess.move.z)*INERTIA;

	//�ړ�
	g_Princess.pos.x += g_Princess.move.x;
	g_Princess.pos.z += g_Princess.move.z;
	g_Princess.pos.y += g_Princess.move.y;


	if (g_Princess.pos.y < 0)
	{	//�n�ʂɂ߂荞�܂Ȃ��悤��
		g_Princess.pos.y = 0;
		g_Princess.move.y = 0;
	}
	//���E�ǔ���
	CollisionWall(&g_Princess.pos, pLoadPrincess->fRadius);

	if (g_Princess.state == PRINCESSSTATE_DETH && g_Princess.nCntKeyPrincess >= 1)
	{
	}
	else
	{
		//���[�V�����Đ�
		PrincessMotion();
	}

	//�e�̈ʒu��ݒ�
	SetPositionSadow(g_nIdxShadowPrincess, g_Princess.pos, PRINCESS_SHADOW + ((0.0f - g_Princess.pos.y)*-0.1f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f - ((0.0f - g_Princess.pos.y)*-0.008f)));
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPrincess(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	D3DXMATRIX		mtxRot, mtxTrans, mtxParent;	// �v�Z�p�}�g���b�N�X
	D3DXMATERIAL	*pMat;	//���݂̃}�e���A���ۑ��p
	D3DMATERIAL9	matDef;	//�}�e���A���f�[�^�ւ̃|�C���^
	D3DMATERIAL9		matKeep;	//�ۑ��p

//�P�̐ݒ�
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Princess.mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Princess.rot.y, g_Princess.rot.x, g_Princess.rot.z);

	D3DXMatrixMultiply(&g_Princess.mtxWorld, &g_Princess.mtxWorld, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_Princess.pos.x, g_Princess.pos.y, g_Princess.pos.z);

	D3DXMatrixMultiply(&g_Princess.mtxWorld, &g_Princess.mtxWorld, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_Princess.mtxWorld);

//���f���̐ݒ�
	for (int nCntPrincess = 0; nCntPrincess < pLoadPrincess->nNumParts; nCntPrincess++)
	{
		//�e�̎���
		if (pLoadPrincess->nParent[nCntPrincess] == -1)
		{
			mtxParent = g_Princess.mtxWorld;
		}
		else
		{
			mtxParent = g_Princess.aModel[pLoadPrincess->nParent[nCntPrincess]].mtxWorld;
		}
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Princess.aModel[nCntPrincess].mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pLoadPrincess->rot[nCntPrincess].y, pLoadPrincess->rot[nCntPrincess].x, pLoadPrincess->rot[nCntPrincess].z);

		D3DXMatrixMultiply(&g_Princess.aModel[nCntPrincess].mtxWorld, &g_Princess.aModel[nCntPrincess].mtxWorld, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pLoadPrincess->pos[nCntPrincess].x, pLoadPrincess->pos[nCntPrincess].y, pLoadPrincess->pos[nCntPrincess].z);

		D3DXMatrixMultiply(&g_Princess.aModel[nCntPrincess].mtxWorld, &g_Princess.aModel[nCntPrincess].mtxWorld, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

		D3DXMatrixMultiply(&g_Princess.aModel[nCntPrincess].mtxWorld, &g_Princess.aModel[nCntPrincess].mtxWorld, &mtxParent);	//���[���h�s�񁁃��[���h�s�񁖐e�̃��[���h�s��
		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Princess.aModel[nCntPrincess].mtxWorld);

		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		// �}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_Princess.aModel[nCntPrincess].pBuffMat->GetBufferPointer();
		// �e�N�X�`���̐ݒ������Ȃ炱��(1���̎�)

		for (int nCntMat = 0; nCntMat < (int)g_Princess.aModel[nCntPrincess].nNumMat; nCntMat++)
		{
			// �e�N�X�`���̐ݒ������Ȃ炱��(�������̎�)
			pDevice->SetTexture(0, g_pTexturePrincess[nCntMat]);

			pMat[nCntMat].MatD3D.Ambient.r = pMat[nCntMat].MatD3D.Diffuse.r/12;
			pMat[nCntMat].MatD3D.Ambient.g = pMat[nCntMat].MatD3D.Diffuse.g/12;
			pMat[nCntMat].MatD3D.Ambient.b = pMat[nCntMat].MatD3D.Diffuse.b/12;

			matKeep = pMat[nCntMat].MatD3D;	//�ۑ�

			if (g_Princess.bDamege)
			{
				pMat[nCntMat].MatD3D.Diffuse.r = 255;	//�ԐF�ύX
			}

			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			pMat[nCntMat].MatD3D = matKeep;	//�F��߂�

			// �`��
			g_Princess.aModel[nCntPrincess].pMesh->DrawSubset(nCntMat);	//�i�q���
		}

		// �}�e���A�����f�t�H���g�ɖ߂�
		pDevice->SetMaterial(&matDef);
	}
}

//=============================================================================
// �P�̐ݒ�
//=============================================================================
void SetPrincess(void)
{
	g_Princess.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.state = PRINCESSSTATE_NEUTRAL;
	g_Princess.stateOld = PRINCESSSTATE_NEUTRAL;

	g_Princess.bDamege = false;
	g_Princess.bPinch = false;
	g_Princess.bHold = false;

	DeleteMagic();
	DeleteWave();
	SetMagic(g_Princess.pos);
	SetWave(g_Princess.pos);

}

//=============================================================================
// �P���̎擾
//=============================================================================
PRINCESS *GetPrincess(void)
{
	return &g_Princess;
}

//=============================================================================
//	���[�V�����̐؂�ւ�
//=============================================================================
void SwitchMotionPrincess()
{
	switch (*GetGameState())
	{
	case GAMESTATE_CLEAR:
		//���
		g_Princess.state = PRINCESSSTATE_WIN;
		break;
	case GAMESTATE_GAMEOVER:
		//��
		g_Princess.state = PRINCESSSTATE_DETH;
		break;
	default:
		if (g_Princess.move.x > LATEST_SPEED || g_Princess.move.z > LATEST_SPEED ||
			g_Princess.move.x < -LATEST_SPEED || g_Princess.move.z < -LATEST_SPEED)
		{	//���x������
			g_Princess.state = PRINCESSSTATE_MOVE;
		}
		else if (g_Princess.state != PRINCESSSTATE_ATTACK && g_Princess.state != PRINCESSSTATE_WIN)
		{	//�ʏ펞
			if (g_Princess.bHold)
			{	//����Ȃ��ł�
				g_Princess.state = PRINCESSSTATE_HOLDNEUTRAL;
			}
			else
			{	//1�l
				g_Princess.state = PRINCESSSTATE_NEUTRAL;
			}
		}
		break;
	}
}
//=============================================================================
//	���[�V�����̍Đ�
//=============================================================================
void PrincessMotion(void)
{
	//�O��̃��[�V�����ƍ���̃��[�V������������牽�t���[�����q���ĕω�����

	//�p�[�c����
	for (int nCntMotion = 0; nCntMotion < pLoadPrincess->nNumParts; nCntMotion++)
	{
		KEY *pKey, *pNextKey;
		int nNextKey = g_Princess.nCntKeyPrincess + 1;
		if (g_Princess.state != g_Princess.stateOld)
		{
			nNextKey = 0;
		}

		if (nNextKey >= pLoadPrincess->MotionInfo[g_Princess.state].nNumKey)
		{
			if (pLoadPrincess->MotionInfo[g_Princess.state].bLoop)
			{
				nNextKey = 0;
			}
			else
			{
				g_Princess.stateOld = g_Princess.state;
				g_Princess.state = PRINCESSSTATE_NEUTRAL;
				nNextKey = 0;
				g_Princess.nCntFrame = 0;
				g_Princess.nCntKeyPrincess = 0;
			}
		}
		if (g_Princess.state != g_Princess.stateOld)
		{
			g_bMotion = true;
			g_nFrame = INTERBAL_FRAME;
			pKey = &pLoadPrincess->MotionInfo[g_Princess.stateOld].KeyInfo[pLoadPrincess->MotionInfo[g_Princess.stateOld].nNumKey-1].Key[nCntMotion];		//�Ō�̃L�[	
			pNextKey = &pLoadPrincess->MotionInfo[g_Princess.state].KeyInfo[nNextKey].Key[nCntMotion];	//���̃L�[		
		}
		else
		{
			g_bMotion = false;
			g_nFrame = pLoadPrincess->MotionInfo[g_Princess.state].KeyInfo[g_Princess.nCntKeyPrincess].nFrame;
			pKey = &pLoadPrincess->MotionInfo[g_Princess.state].KeyInfo[g_Princess.nCntKeyPrincess].Key[nCntMotion];		//���݂̃L�[	
			pNextKey = &pLoadPrincess->MotionInfo[g_Princess.state].KeyInfo[nNextKey].Key[nCntMotion];	//���̃L�[		
		}

		//��]
		g_Princess.aModel[nCntMotion].rotMove = (pNextKey->rot - pKey->rot)*((float)g_Princess.nCntFrame / (float)g_nFrame);
		pLoadPrincess->rot[nCntMotion] = pKey->rot + g_Princess.aModel[nCntMotion].rotMove;
	}
}

//=============================================================================
// �A���������
//=============================================================================
void HoldPrincess(PLAYER *pPlayer, float fDistX, float fDistZ)
{
	float fLength = (pPlayer->fRadius + pLoadPrincess->fRadius);
	D3DXVECTOR3 vec = -pPlayer->move;	//�v���C���[�̈ړ������Ɣ��Ε����̃x�N�g��
	D3DXVec3Normalize(&vec, &vec);
	g_Princess.bHold = true;

	//�P���v���C���[�̌��ɃZ�b�g
	if (vec.x >= 0.1f || vec.z >= 0.1f || vec.x <= -0.1f || vec.z <= -0.1f)
	{
		g_Princess.pos.x = pPlayer->pos.x + vec.x * fLength;
		g_Princess.pos.z = pPlayer->pos.z + vec.z * fLength;
	}

	//�ꏏ�ɓ���
	g_Princess.move = pPlayer->move;
	g_Princess.rotDest.y = atan2f(fDistX, fDistZ);
}

//=============================================================================
// �����蔻��
//=============================================================================
bool CollisionPrincess(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Radius, int nType)
{
	bool bLand = false;
	float fDistance = (pPos->x - g_Princess.pos.x)*(pPos->x - g_Princess.pos.x) + (pPos->z - g_Princess.pos.z)*(pPos->z - g_Princess.pos.z);	//2�_�Ԃ̋���
	float fLength = (Radius + pLoadPrincess->fRadius)*(Radius + pLoadPrincess->fRadius);	//���a
	float fDiff = (Radius + pLoadPrincess->fRadius) - (float)sqrt(fDistance);	//��
	D3DXVECTOR3 vec = D3DXVECTOR3((pPos->x - g_Princess.pos.x), 0.0f, (pPos->z - g_Princess.pos.z));	//�x�N�g��
	D3DXVec3Normalize(&vec, &vec);

	if (fDistance <= fLength)
	{
		if (nType == 1)
		{	//�G���߂��ɂ�����s���`
			g_Princess.bPinch = true;
		}
		bLand = true;
		pPos->x = pPos->x + vec.x * fDiff;
		pPos->z = pPos->z + vec.z * fDiff;
	}

	return bLand;
}
