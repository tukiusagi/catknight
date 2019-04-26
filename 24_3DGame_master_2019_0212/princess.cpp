//=============================================================================
//
// �P�̏��� [princess.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "princess.h"
#include "input.h"
#include "joystick.h"
#include "camera.h"
#include "shadow.h"
#include "meshfield.h"
#include "line.h"
#include"game.h"
#include "enemy.h"
#include "wave.h"
#include "magic.h"
#include "player.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define PRINCESS_TEXTURE	"data/TEXTURE/mizuiro.jpg"	//�ǂݍ��ރe�N�X�`����
#define PRINCESS_TEXTURE2	"data/TEXTURE/white2.jpg"	//�ǂݍ��ރe�N�X�`����
#define PRINCESS_SHADOW		(15.0f)			//�e�̑傫��
#define PRINCESS_ROTSPEAD	(0.1f)			//��]���x
#define PRINCESS_KEY_MAX	(4)				//�L�[�̍ő吔
#define PRINCESS_TEX_MAX	(6)				//�e�N�X�`���̍ő吔
#define MAX_CHARA			(256)			//�ǂݍ��ޕ����̍ő吔

//=============================================================================
// ���[�V�����Ǘ�
//=============================================================================
//�L�[�v�f
typedef struct
{
	D3DXVECTOR3	pos;	//�ʒu
	D3DXVECTOR3	rot;	//����
} KEY;

//�L�[���
typedef struct
{
	int nFrame;				//�t���[����
	KEY	Key[PRINCESS_MAX];	//�L�[�v�f[�p�[�c��]
} KEY_INFO;

//���[�V�������
typedef struct
{
	bool		bLoop;		//���[�v�̗L��
	int			nNumKey;	//�L�[��
	KEY_INFO	KeyInfo[PRINCESS_KEY_MAX];	//�L�[���[�L�[�̐�]
} MOTION_INFO;

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
PRINCESS			g_Princess;					//�P�̏��
LPDIRECT3DTEXTURE9	g_pTexturePrincess[PRINCESS_TEX_MAX] = {};	//�e�N�X�`���̃|�C���^
int	g_nIdxShadowPrincess;		//�e��ID
int g_NumPrincess = 0;			//�ǂݍ��ރ��f����
int g_NumPrincessParts = 0;			//�ǂݍ��ރ��f����
char g_PrincessFileNeme[PRINCESS_MAX][MAX_CHARA];

MOTION_INFO	g_aPrincessMotionInfo[PRINCESSSTATE_MAX];

//=============================================================================
// ����������
//=============================================================================
void InitPrincess(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�f�[�^�̃��[�h
	LoadPrincess();

	// �ʒu�E�����̏����ݒ�
	g_Princess.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.state = PRINCESSSTATE_NEUTRAL;
	g_Princess.bDamege = false;
	g_Princess.bPinch = false;
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
	for (int nCntModel = 0; nCntModel < g_NumPrincess; nCntModel++)
	{
		D3DXLoadMeshFromX(g_Princess.aModel[nCntModel].pFilename,	//�ǂݍ��ރt�@�C����
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
	for (int nCntPrincess = 0; nCntPrincess < g_NumPrincess; nCntPrincess++)
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
	CAMERA *pCamera;
	pCamera = GetCamera();
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

	//�t���[������
	if (g_Princess.state == PRINCESSSTATE_DETH && g_Princess.nCntKeyPrincess >= 1)
	{	//���S���ɂ͑������Ȃ�
	}
	else if (pWave->state == WAVE_STATE_ATTACK && g_Princess.nCntKeyPrincess >= 1 && *GetGameState() == GAMESTATE_NORMAL)
	{
		//���@�g�p�����~�߂�
	}
	else
	{
		g_Princess.nCntFrame++;
		if (g_Princess.nCntFrame > g_aPrincessMotionInfo[g_Princess.state].KeyInfo[g_Princess.nCntKeyPrincess].nFrame)
		{
			g_Princess.nCntFrame = 0;
			g_Princess.nCntKeyPrincess++;
			if (g_Princess.nCntKeyPrincess >= g_aPrincessMotionInfo[g_Princess.state].nNumKey)
			{
				g_Princess.nCntKeyPrincess = 0;
			}
		}
	}
	//���[�V�����̐؂�ւ�
	if (*GetMode() == MODE_GAME)
	{
		switch (*GetGameState())
		{
		case GAMESTATE_CLEAR:
			g_Princess.state = PRINCESSSTATE_WIN;
			break;
		case GAMESTATE_GAMEOVER:
			if (g_Princess.state != PRINCESSSTATE_DETH)
			{
				g_Princess.state = PRINCESSSTATE_DETH;
				g_Princess.nCntFrame = 0;
				g_Princess.nCntKeyPrincess = 0;
			}
			break;
		default:
			if (g_Princess.state != PRINCESSSTATE_ATTACK && g_Princess.state != PRINCESSSTATE_WIN)
			{
				g_Princess.state = PRINCESSSTATE_NEUTRAL;
			}
			break;
		}

		//��]
		if (g_Princess.rotmove.y = g_Princess.rotDest.y - g_Princess.rot.y)
		{
			if (g_Princess.rotmove.y < -D3DX_PI)
			{
				g_Princess.rotmove.y += D3DX_PI * 2;
			}
			else if (g_Princess.rotmove.y > D3DX_PI)
			{
				g_Princess.rotmove.y -= D3DX_PI * 2;
			}
		}
		if (g_Princess.rot.y += g_Princess.rotmove.y * PRINCESS_ROTSPEAD)
		{
			if (g_Princess.rot.y > D3DX_PI)
			{
				g_Princess.rot.y -= D3DX_PI * 2;
			}
			else if (g_Princess.rot.y < -D3DX_PI)
			{
				g_Princess.rot.y += D3DX_PI * 2;
			}
		}

		//�d��
		g_Princess.move.y -= 0.8f;

		//����
		g_Princess.move.x += (0.0f - g_Princess.move.x)*0.25f;
		g_Princess.move.z += (0.0f - g_Princess.move.z)*0.25f;

		//�ړ�
		g_Princess.pos.x += g_Princess.move.x;
		g_Princess.pos.z += g_Princess.move.z;
		g_Princess.pos.y += g_Princess.move.y;

	}

	if (g_Princess.pos.y < 0)
	{	//�n�ʂɂ߂荞�܂Ȃ��悤��
		g_Princess.pos.y = 0;
		g_Princess.move.y = 0;
	}
	//���E�ǔ���
	if (g_Princess.pos.x > MESHFIELD_SIZE * MESHFIELD_W * 0.5f - g_Princess.fRadius)
	{
		g_Princess.pos.x = MESHFIELD_SIZE * MESHFIELD_W * 0.5f - g_Princess.fRadius;
	}
	if (g_Princess.pos.x < -MESHFIELD_SIZE * MESHFIELD_W * 0.5f + g_Princess.fRadius)
	{
		g_Princess.pos.x = -MESHFIELD_SIZE * MESHFIELD_W * 0.5f + g_Princess.fRadius;
	}
	//����O�ǔ���
	if (g_Princess.pos.z > MESHFIELD_SIZE * MESHFIELD_D * 0.5f - g_Princess.fRadius)
	{
		g_Princess.pos.z = MESHFIELD_SIZE * MESHFIELD_D * 0.5f - g_Princess.fRadius;
	}
	if (g_Princess.pos.z < -MESHFIELD_SIZE * MESHFIELD_D * 0.5f + g_Princess.fRadius)
	{
		g_Princess.pos.z = -MESHFIELD_SIZE * MESHFIELD_D * 0.5f + g_Princess.fRadius;
	}

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
	for (int nCntPrincess = 0; nCntPrincess < g_NumPrincessParts; nCntPrincess++)
	{
		//�e�̎���
		if (g_Princess.aModel[nCntPrincess].nIdxModelParent == -1)
		{
			mtxParent = g_Princess.mtxWorld;
		}
		else
		{
			mtxParent = g_Princess.aModel[g_Princess.aModel[nCntPrincess].nIdxModelParent].mtxWorld;
		}
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Princess.aModel[nCntPrincess].mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Princess.aModel[nCntPrincess].rot.y, g_Princess.aModel[nCntPrincess].rot.x, g_Princess.aModel[nCntPrincess].rot.z);

		D3DXMatrixMultiply(&g_Princess.aModel[nCntPrincess].mtxWorld, &g_Princess.aModel[nCntPrincess].mtxWorld, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Princess.aModel[nCntPrincess].pos.x, g_Princess.aModel[nCntPrincess].pos.y, g_Princess.aModel[nCntPrincess].pos.z);

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
// �P���̎擾
//=============================================================================
PRINCESS *GetPrincess(void)
{
	return &g_Princess;
}

//=============================================================================
//	���[�V�����̍Đ�
//=============================================================================
void PrincessMotion(void)
{
	//�p�[�c����
	for (int nCntMotion = 0; nCntMotion < g_NumPrincessParts; nCntMotion++)
	{
		KEY *pKey, *pNextKey;
		int nNextKey = g_Princess.nCntKeyPrincess + 1;


		if (nNextKey >= g_aPrincessMotionInfo[g_Princess.state].nNumKey)
		{
			if (g_aPrincessMotionInfo[g_Princess.state].bLoop)
			{
				nNextKey = 0;
			}
			else
			{
				g_Princess.state = PRINCESSSTATE_NEUTRAL;
				g_Princess.nCntFrame = 0;
				g_Princess.nCntKeyPrincess = 0;
			}
		}
		pKey = &g_aPrincessMotionInfo[g_Princess.state].KeyInfo[g_Princess.nCntKeyPrincess].Key[nCntMotion];		//���݂̃L�[	
		pNextKey = &g_aPrincessMotionInfo[g_Princess.state].KeyInfo[nNextKey].Key[nCntMotion];	//���̃L�[		

		//��]
		g_Princess.aModel[nCntMotion].rotMove = (pNextKey->rot - pKey->rot)*((float)g_Princess.nCntFrame / (float)g_aPrincessMotionInfo[g_Princess.state].KeyInfo[g_Princess.nCntKeyPrincess].nFrame);

		g_Princess.aModel[nCntMotion].rot = pKey->rot + g_Princess.aModel[nCntMotion].rotMove;
	}
}

//=============================================================================
// �A�����
//=============================================================================
void MovePrincess(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove)
{
	float fDistX, fDistZ, fDistans;
	WAVE *pWave = GetWave();
	MAGIC *pMagic = GetMagic();
	pMagic->bUse = false;
	pWave->bUse = false;

	fDistX = pPos->x - g_Princess.pos.x;
	fDistZ = pPos->z - g_Princess.pos.z;
	fDistans = (float)sqrt(fDistX * fDistX + fDistZ * fDistZ);

	//�ړ�����������
	g_Princess.rotDest.y = atan2f(fDistX, fDistZ) + D3DX_PI;

	//��]
	if (g_Princess.rotmove.y = g_Princess.rotDest.y - g_Princess.rot.y)
	{
		if (g_Princess.rotmove.y < -D3DX_PI)
		{
			g_Princess.rotmove.y += D3DX_PI * 2;
		}
		else if (g_Princess.rotmove.y > D3DX_PI)
		{
			g_Princess.rotmove.y -= D3DX_PI * 2;
		}
	}
	if (g_Princess.rot.y += g_Princess.rotmove.y * PRINCESS_ROTSPEAD)
	{
		if (g_Princess.rot.y > D3DX_PI)
		{
			g_Princess.rot.y -= D3DX_PI * 2;
		}
		else if (g_Princess.rot.y < -D3DX_PI)
		{
			g_Princess.rot.y += D3DX_PI * 2;
		}
	}

	g_Princess.move.x = pMove->x;
	g_Princess.move.z = pMove->z;

	g_Princess.pos.x += g_Princess.move.x;
	g_Princess.pos.z += g_Princess.move.z;
}

//=============================================================================
// �����蔻��
//=============================================================================
bool CollisionPrincess(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Radius, int nType)
{
	bool bLand = false;
	float fDistance = (pPos->x - g_Princess.pos.x)*(pPos->x - g_Princess.pos.x) + (pPos->z - g_Princess.pos.z)*(pPos->z - g_Princess.pos.z);	//2�_�Ԃ̋���
	float fLength = (Radius + g_Princess.fRadius)*(Radius + g_Princess.fRadius);	//���a
	float fDiff = (Radius + g_Princess.fRadius) - (float)sqrt(fDistance);	//��
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

//=============================================================================
//	�t�@�C���̓ǂݍ���
//=============================================================================
void LoadPrincess(void)
{
	//�t�@�C���|�C���g
	FILE *pFile;

	//�ϐ��錾
	char *pStrCur = NULL;	//������̐擪�ւ̃|�C���^
	char cLine[MAX_CHARA];	//������ǂݍ��ݗp
	char cStr[MAX_CHARA];	//�����񔲂��o���p
	char *cPos;				//������؂�p
	int nCnt = 0;			//�󔒃J�E���^�[
	int nCntModel = 0;		//���f���ԍ�
	int nCntMotion = 0;		//���[�V�����ԍ�
	int nCntKey = 0;		//���݃L�[
	int nCntParts;			//�p�[�c�ԍ�

	pFile = fopen("data/motion_Princess.txt", "r");

	//�J����
	if (pFile != NULL)
	{
		while (memcmp(cStr, "SCRIPT", strlen("SCRIPT")) != 0)	//�w��̕������������܂œǂݍ���
		{
			//�e�L�X�g1�s�ǂݍ��ނ̂Ɂ�3�s1�Z�b�g���K�v
			fgets(cLine, MAX_CHARA, pFile);	//1�s256�����ǂݍ���
			pStrCur = &cLine[0];			//����������
			strcpy(cStr, pStrCur);			//pStrCur�œǂݎ����������cStr�Ɋi�[
		}
		while (memcmp(cStr, "END_SCRIPT", strlen("END_SCRIPT")) != 0)
		{
			fgets(cLine, MAX_CHARA, pFile);
			pStrCur = &cLine[0];
			strcpy(cStr, pStrCur);
			//�ǂݍ��ރ��f����
			if (memcmp(cStr, "NUM_MODEL = ", strlen("NUM_MODEL = ")) == 0)
			{
				pStrCur += strlen("NUM_MODEL = ");	//������̎����v�Z
				strcpy(cStr, pStrCur);
				g_NumPrincess = atoi(cStr);	//atoi�@char�^��int�^�ɕϊ�����
			}

			//���f���t�@�C�����̓ǂݍ���
			if (memcmp(cStr, "MODEL_FILENAME = ", strlen("MODEL_FILENAME = ")) == 0)
			{
				for (int nCntStr = 0; nCntStr < MAX_CHARA; nCntStr++)
				{
					if (cStr[nCntStr] == '\t')	//TAB���������炻���ŕ������؂�
					{
						cStr[nCntStr] = '\0';
						strcpy(pStrCur, cStr);
					}
				}
				pStrCur += strlen("MODEL_FILENAME = ");
				strcpy(g_PrincessFileNeme[nCntModel], pStrCur);
				g_Princess.aModel[nCntModel].pFilename = g_PrincessFileNeme[nCntModel];
				nCntModel++;
			}
			//�L�����N�^�[���
			if (memcmp(cStr, "CHARACTERSET", strlen("CHARACTERSET")) == 0)
			{
				while (memcmp(cStr, "END_CHARACTERSET", strlen("END_CHARACTERSET")) != 0)
				{
					fgets(cLine, MAX_CHARA, pFile);
					nCnt = 0;
					while (1)
					{	//�����̑O�ɑ��݂���󔒂�^�u���J�b�g����
						if (cLine[nCnt] == ' ' || cLine[nCnt] == '\t')
						{
							nCnt++;
						}
						else
						{
							break;
						}
					}
					pStrCur = &cLine[nCnt];
					strcpy(cStr, pStrCur);
					if (memcmp(&cLine[nCnt], "RADIUS = ", strlen("RADIUS = ")) == 0)
					{	//���a
						pStrCur += strlen("RADIUS = ");
						strcpy(cStr, pStrCur);
						g_Princess.fRadius = (float)atof(cStr);
					}
					if (memcmp(&cLine[nCnt], "LIFE = ", strlen("LIFE = ")) == 0)
					{	//�̗�
						pStrCur += strlen("LIFE = ");
						strcpy(cStr, pStrCur);
						g_Princess.nLife = atoi(cStr);
					}
					if (memcmp(&cLine[nCnt], "NUM_PARTS = ", strlen("NUM_PARTS = ")) == 0)
					{	//�ǂݍ��ރp�[�c��
						pStrCur += strlen("NUM_PARTS = ");
						strcpy(cStr, pStrCur);
						g_NumPrincessParts = atoi(cStr);
					}
					//�L�����p�[�c���
					if (memcmp(&cLine[nCnt], "PARTSSET", strlen("PARTSSET")) == 0)
					{
						while (memcmp(&cLine[nCnt], "END_PARTSSET", strlen("END_PARTSSET")) != 0)
						{
							fgets(cLine, MAX_CHARA, pFile);
							nCnt = 0;
							while (1)
							{
								if (cLine[nCnt] == ' ' || cLine[nCnt] == '\t')
								{
									nCnt++;
								}
								else
								{
									break;
								}
							}
							pStrCur = &cLine[nCnt];
							strcpy(cStr, pStrCur);

							if (memcmp(&cLine[nCnt], "INDEX = ", strlen("INDEX = ")) == 0)
							{	//����͉��Ԃ̃p�[�c��
								pStrCur += strlen("INDEX = ");
								strcpy(cStr, pStrCur);
								nCntParts = atoi(cStr);
							}
							if (memcmp(&cLine[nCnt], "PARENT = ", strlen("PARENT = ")) == 0)
							{	//�e�̔ԍ�
								pStrCur += strlen("PARENT = ");
								strcpy(cStr, pStrCur);
								g_Princess.aModel[nCntParts].nIdxModelParent = atoi(cStr);
							}
							if (memcmp(&cLine[nCnt], "POS = ", strlen("POS = ")) == 0)
							{	//�ʒu
								pStrCur += strlen("POS = ");
								strcpy(cStr, pStrCur);

								cPos = strtok(cStr, " ");	//�������󔒂��Ƃɋ�؂��ĕ���
								g_Princess.aModel[nCntParts].pos.x = (float)atof(cPos);

								cPos = strtok(NULL, " ");
								g_Princess.aModel[nCntParts].pos.y = (float)atof(cPos);

								cPos = strtok(NULL, " ");
								g_Princess.aModel[nCntParts].pos.z = (float)atof(cPos);
							}
							if (memcmp(&cLine[nCnt], "ROT = ", strlen("ROT = ")) == 0)
							{	//�p�x
								pStrCur += strlen("ROT = ");
								strcpy(cStr, pStrCur);
								cPos = strtok(cStr, " ");
								g_Princess.aModel[nCntParts].rot.x = (float)atof(cPos);
								cPos = strtok(NULL, " ");
								g_Princess.aModel[nCntParts].rot.y = (float)atof(cPos);
								cPos = strtok(NULL, " ");
								g_Princess.aModel[nCntParts].rot.z = (float)atof(cPos);
							}
						}
					}
				}
			}
			//���[�V�������
			if (memcmp(cStr, "MOTIONSET", strlen("MOTIONSET")) == 0)
			{
				nCntKey = 0;	//�L�[�J�E���^�[���Z�b�g
				while (memcmp(&cLine[nCnt], "END_MOTIONSET", strlen("END_MOTIONSET")) != 0)
				{
					fgets(cLine, MAX_CHARA, pFile);
					nCnt = 0;
					while (1)
					{
						if (cLine[nCnt] == ' ' || cLine[nCnt] == '\t')
						{
							nCnt++;
						}
						else
						{
							break;
						}
					}
					pStrCur = &cLine[nCnt];
					strcpy(cStr, pStrCur);

					if (memcmp(&cLine[nCnt], "LOOP = ", strlen("LOOP = ")) == 0)
					{	//���[�v�̗L��
						pStrCur += strlen("LOOP = ");
						strcpy(cStr, pStrCur);
						g_aPrincessMotionInfo[nCntMotion].bLoop = atoi(cStr);
					}
					if (memcmp(&cLine[nCnt], "NUM_KEY = ", strlen("NUM_KEY = ")) == 0)
					{	//�L�[��
						pStrCur += strlen("NUM_KEY = ");
						strcpy(cStr, pStrCur);
						g_aPrincessMotionInfo[nCntMotion].nNumKey = atoi(cStr);
					}
					//�L�[���
					if (memcmp(&cLine[nCnt], "KEYSET", strlen("KEYSET")) == 0)
					{
						nCntParts = 0;	//�p�[�c�J�E���^�[���Z�b�g
						while (memcmp(&cLine[nCnt], "END_KEYSET", strlen("END_KEYSET")) != 0)
						{
							fgets(cLine, MAX_CHARA, pFile);
							nCnt = 0;
							while (1)
							{
								if (cLine[nCnt] == ' ' || cLine[nCnt] == '\t')
								{
									nCnt++;
								}
								else
								{
									break;
								}
							}
							pStrCur = &cLine[nCnt];
							strcpy(cStr, pStrCur);

							if (memcmp(&cLine[nCnt], "FRAME = ", strlen("FRAME = ")) == 0)
							{	//�t���[����
								pStrCur += strlen("FRAME = ");
								strcpy(cStr, pStrCur);
								g_aPrincessMotionInfo[nCntMotion].KeyInfo[nCntKey].nFrame = atoi(cStr);
							}
							//�L�[�v�f
							if (memcmp(&cLine[nCnt], "KEY", strlen("KEY")) == 0)
							{
								while (memcmp(&cLine[nCnt], "END_KEY", strlen("END_KEY")) != 0)
								{
									fgets(cLine, MAX_CHARA, pFile);
									nCnt = 0;
									while (1)
									{
										if (cLine[nCnt] == ' ' || cLine[nCnt] == '\t')
										{
											nCnt++;
										}
										else
										{
											break;
										}
									}
									pStrCur = &cLine[nCnt];
									strcpy(cStr, pStrCur);

									if (memcmp(&cLine[nCnt], "POS = ", strlen("POS = ")) == 0)
									{	//�ʒu
										pStrCur += strlen("POS = ");
										strcpy(cStr, pStrCur);
										cPos = strtok(cStr, " ");
										g_aPrincessMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos.x = (float)atof(cPos);
										cPos = strtok(NULL, " ");
										g_aPrincessMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos.y = (float)atof(cPos);
										cPos = strtok(NULL, " ");
										g_aPrincessMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos.z = (float)atof(cPos);
									}
									if (memcmp(&cLine[nCnt], "ROT = ", strlen("ROT = ")) == 0)
									{	//�p�x
										pStrCur += strlen("ROT = ");
										strcpy(cStr, pStrCur);
										cPos = strtok(cStr, " ");
										g_aPrincessMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot.x = (float)atof(cPos);
										cPos = strtok(NULL, " ");
										g_aPrincessMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot.y = (float)atof(cPos);
										cPos = strtok(NULL, " ");
										g_aPrincessMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot.z = (float)atof(cPos);
									}
								}
								nCntParts++;	//���̃p�[�c��
							}
						}
						nCntKey++;	//���̃L�[��
					}
				}
				nCntMotion++;	//���̃��[�V������
			}
		}
	}
}
