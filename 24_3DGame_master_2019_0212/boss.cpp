//=============================================================================
//
// �{�X���� [boss.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "boss.h"
#include "input.h"
#include "joystick.h"
#include "camera.h"
#include "shadow.h"
#include "meshfield.h"
#include "line.h"
#include "princess.h"
#include "player.h"
#include"game.h"
#include "lifebar.h"
#include "life.h"
#include "wave.h"
#include "obstacle.h"
#include "effect.h"
#include "score.h"
#include "enemywave.h"
#include "enemy.h"
#include "sound.h"
#include "particle.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define BOSS_TEXTURE	"data/TEXTURE/Suede-bk.jpg"	//�ǂݍ��ރe�N�X�`����
#define BOSS_TEXTURE2	"data/TEXTURE/white.png"	//�ǂݍ��ރe�N�X�`����
#define BOSS_SHADOW	(15.0f)			//�e�̑傫��
#define BOSS_ROTSPEAD	(0.1f)			//��]���x
#define BOSS_KEY_MAX	(4)				//�L�[�̍ő吔
#define BOSS_WAVE_MAX	(1)				//wave�ő吔
#define MAX_CHARA		(256)			//�ǂݍ��ޕ����̍ő吔

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
	int nFrame;			//�t���[����
	KEY	Key[BOSS_MAX];	//�L�[�v�f[�p�[�c��]
} KEY_INFO;

//���[�V�������
typedef struct
{
	bool		bLoop;		//���[�v�̗L��
	int			nNumKey;	//�L�[��
	KEY_INFO	KeyInfo[BOSS_KEY_MAX];	//�L�[���[�L�[�̐�]
} MOTION_INFO;

//=============================================================================
// ���[�h�p�̍\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos[BOSS_MAX];	//�ʒu
	D3DXVECTOR3	rot[BOSS_MAX];	//�p�x
	char		cBossFileNeme[BOSS_MAX][MAX_CHARA];	//���f���t�@�C����
	int			nParent[BOSS_MAX];	//�e�̔ԍ�
	int			nNumBoss = 0;		//�ǂݍ��ރ��f����
	int			nNumBossParts = 0;	//�ǂݍ��ރp�[�c��
	float		fSpeed;		//���x
	float		fRadius;	//���a
	int			nMaxLife;	//�ő�̗�
	D3DXVECTOR3	Collpos;			//�ʒu(�����蔻��)
	int			nCollParent;		//�e�̔ԍ�(�����蔻��)
	float		fCollRadius;		//���a(�����蔻��)
	int			nStart;				//�J�n����t���[��
	int			nEnd;				//�I������t���[��
}LOADBOSS;

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9	g_pTextureBoss[6] = {};	//�e�N�X�`���̃|�C���^
BOSS				g_aBoss[MAX_BOSS];		//�{�X���
LOADBOSS			g_LoadBoss;				//�ǂݍ��ݗp
MOTION_INFO	g_aBossMotionInfo[BOSSSTATE_MAX];	//���[�V�������
int g_NumBoss = 0;	//�{�X��

//=============================================================================
// ����������
//=============================================================================
void InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �ʒu�E�����̏����ݒ�
	LoadBoss();
	g_NumBoss = 0;
	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		g_aBoss[nCntBoss].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].fRadius = g_LoadBoss.fRadius;
		g_aBoss[nCntBoss].fSpeed = g_LoadBoss.fSpeed;
		g_aBoss[nCntBoss].nLife = g_LoadBoss.nMaxLife;
		g_aBoss[nCntBoss].state = BOSSSTATE_NEUTRAL;
		g_aBoss[nCntBoss].bUse = false;
		g_aBoss[nCntBoss].bHit = false;
		g_aBoss[nCntBoss].bDamege = false;
		g_aBoss[nCntBoss].bStop = false;
		g_aBoss[nCntBoss].nCntState = 0;
		g_aBoss[nCntBoss].nCntFrameBoss = 0;
		g_aBoss[nCntBoss].nCntKeyBoss = 0;
		g_aBoss[nCntBoss].nCntAttack = 0;

		g_aBoss[nCntBoss].CollisionBoss.nIdxModelParent = g_LoadBoss.nCollParent;
		g_aBoss[nCntBoss].CollisionBoss.pos = g_LoadBoss.Collpos;
		g_aBoss[nCntBoss].CollisionBoss.fRadius = g_LoadBoss.fCollRadius;
		g_aBoss[nCntBoss].CollisionBoss.nStartFrame = g_LoadBoss.nStart;
		g_aBoss[nCntBoss].CollisionBoss.nEndFrame = g_LoadBoss.nEnd;
		g_aBoss[nCntBoss].CollisionBoss.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].CollisionBoss.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// X�t�@�C���̓ǂݍ���
		for (int nCntParts = 0; nCntParts < g_LoadBoss.nNumBossParts; nCntParts++)
		{
			g_aBoss[nCntBoss].aModel[nCntParts].nIdxModelParent = g_LoadBoss.nParent[nCntParts];
			g_aBoss[nCntBoss].aModel[nCntParts].pos = g_LoadBoss.pos[nCntParts];
			g_aBoss[nCntBoss].aModel[nCntParts].rot = g_LoadBoss.rot[nCntParts];
			g_aBoss[nCntBoss].aModel[nCntParts].pFilename = g_LoadBoss.cBossFileNeme[nCntParts];

			D3DXLoadMeshFromX(g_aBoss[nCntBoss].aModel[nCntParts].pFilename,	//�ǂݍ��ރt�@�C����
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_aBoss[nCntBoss].aModel[nCntParts].pBuffMat,
				NULL,
				&g_aBoss[nCntBoss].aModel[nCntParts].nNumMat,
				&g_aBoss[nCntBoss].aModel[nCntParts].pMesh);
		}
	}

}

//=============================================================================
// �I������
//=============================================================================
void UninitBoss(void)
{
	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		DeleteShadow(g_aBoss[nCntBoss].nIdxShadowBoss);
		for (int nCntParts = 0; nCntParts < g_LoadBoss.nNumBossParts; nCntParts++)
		{
			// ���b�V���̊J��
			if (g_aBoss[nCntBoss].aModel[nCntParts].pMesh != NULL)
			{
				g_aBoss[nCntBoss].aModel[nCntParts].pMesh->Release();
				g_aBoss[nCntBoss].aModel[nCntParts].pMesh = NULL;
			}

			// �}�e���A���̊J��
			if (g_aBoss[nCntBoss].aModel[nCntParts].pBuffMat != NULL)
			{
				g_aBoss[nCntBoss].aModel[nCntParts].pBuffMat->Release();
				g_aBoss[nCntBoss].aModel[nCntParts].pBuffMat = NULL;
			}
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBoss(void)
{
	CAMERA *pCamera;
	pCamera = GetCamera();
	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (g_aBoss[nCntBoss].bUse)
		{
			g_aBoss[nCntBoss].nCntAttack++;
			g_aBoss[nCntBoss].nCntFrameBoss++;

			g_aBoss[nCntBoss].posOld = g_aBoss[nCntBoss].pos;
			g_aBoss[nCntBoss].CollisionBoss.posOld = g_aBoss[nCntBoss].CollisionBoss.pos;

			if (g_aBoss[nCntBoss].bDamege)
			{	//�_���[�W���󂯂���Ԃ��ς��
				g_aBoss[nCntBoss].nCntState++;
				if (g_aBoss[nCntBoss].nCntState % 10 == 0)
				{	//�F��߂�
					g_aBoss[nCntBoss].bDamege = false;
				}
			}
			else if (g_aBoss[nCntBoss].bStop)
			{	//�}�q���
				g_aBoss[nCntBoss].nCntState++;
				if (g_aBoss[nCntBoss].nCntState % (60 * 3) == 0)
				{	//��ԉ���
					g_aBoss[nCntBoss].bStop = false;
					g_aBoss[nCntBoss].nCntState = 0;
				}
			}

			//�t���[������
			if (g_aBoss[nCntBoss].bStop && g_aBoss[nCntBoss].nCntKeyBoss >= 1)
			{	//�}�q��Ԃł̓X�g�b�v
			}
			else if (g_aBoss[nCntBoss].nCntFrameBoss > g_aBossMotionInfo[g_aBoss[nCntBoss].state].KeyInfo[g_aBoss[nCntBoss].nCntKeyBoss].nFrame)
			{
				g_aBoss[nCntBoss].nCntFrameBoss = 0;
				g_aBoss[nCntBoss].nCntKeyBoss++;	//�L�[����

				if (g_aBoss[nCntBoss].nCntKeyBoss >= g_aBossMotionInfo[g_aBoss[nCntBoss].state].nNumKey)
				{
					g_aBoss[nCntBoss].nCntKeyBoss = 0;	//�L�[���Z�b�g
				}
			}

			//���[�V�����̐؂�ւ�
			if (g_aBoss[nCntBoss].state != BOSSSTATE_ATTACK && g_aBoss[nCntBoss].state != BOSSSTATE_DETH && g_aBoss[nCntBoss].state != BOSSSTATE_HIT)
			{
				if (g_aBoss[nCntBoss].move.x > 0.5f || g_aBoss[nCntBoss].move.z > 0.5f ||
					g_aBoss[nCntBoss].move.x < -0.5f || g_aBoss[nCntBoss].move.z < -0.5f)
				{	//���x������Ƃ��͈ړ�
					g_aBoss[nCntBoss].state = BOSSSTATE_MOVE;
				}
				else
				{	//�ʏ�̓j���[�g�������[�V����
					g_aBoss[nCntBoss].state = BOSSSTATE_NEUTRAL;
				}
			}

			if (g_aBoss[nCntBoss].state == BOSSSTATE_ATTACK)
			{	//�G�t�F�N�g�\��
				SetEffect(D3DXVECTOR3(g_aBoss[nCntBoss].CollisionBoss.mtxWorld._41, g_aBoss[nCntBoss].CollisionBoss.mtxWorld._42, g_aBoss[nCntBoss].CollisionBoss.mtxWorld._43),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), g_aBoss[nCntBoss].CollisionBoss.fRadius + 5, 10);
			}

			//�d��
			g_aBoss[nCntBoss].move.y -= 0.8f;


			//�����Ƃ̋������ׂċ߂����Ɋ��t��
			float fDistX, fDistZ, fDistans;
			float fDistXPL, fDistZPL, fDistansPL;
			float fDistXPrincess, fDistZPrincess, fDistansPrincess;

			PLAYER *pPlayer;
			pPlayer = GetPlayer();
			fDistXPL = pPlayer->pos.x - g_aBoss[nCntBoss].pos.x;
			fDistZPL = pPlayer->pos.z - g_aBoss[nCntBoss].pos.z;
			fDistansPL = (float)sqrt(fDistXPL * fDistXPL + fDistZPL * fDistZPL);

			PRINCESS *pPrincess;
			pPrincess = GetPrincess();
			fDistXPrincess = pPrincess->pos.x - g_aBoss[nCntBoss].pos.x;
			fDistZPrincess = pPrincess->pos.z - g_aBoss[nCntBoss].pos.z;
			fDistansPrincess = (float)sqrt(fDistXPrincess * fDistXPrincess + fDistZPrincess * fDistZPrincess);

			if (fDistansPL > fDistansPrincess)
			{	//�P�ɋߊ��
				fDistans = fDistansPrincess;
				fDistX = fDistXPrincess;
				fDistZ = fDistZPrincess;
			}
			else
			{	//�R�m�ɋߊ��
				fDistans = fDistansPL;
				fDistX = fDistXPL;
				fDistZ = fDistZPL;
			}

			//�ړ�
			if (CollisionPlayer(&g_aBoss[nCntBoss].pos, &g_aBoss[nCntBoss].posOld, g_aBoss[nCntBoss].fRadius) ||
				CollisionPrincess(&g_aBoss[nCntBoss].pos, &g_aBoss[nCntBoss].posOld, g_aBoss[nCntBoss].fRadius,1))
			{	//�P��R�m�ɐڐG���Ă���Ƃ��͓����Ȃ�
				if (g_aBoss[nCntBoss].state != BOSSSTATE_ATTACK)
				{
					if ((rand() % 180 == 0 || g_aBoss[nCntBoss].nCntAttack % 180 == 0) && g_aBoss[nCntBoss].state != BOSSSTATE_DETH)
					{	//3�b��1�񂭂炢�̊m���ōU��
						g_aBoss[nCntBoss].state = BOSSSTATE_ATTACK;
						g_aBoss[nCntBoss].nCntFrameBoss = 0;
						g_aBoss[nCntBoss].nCntKeyBoss = 0;
						PlaySound(SOUND_LABEL_SE_NAIL);
					}
				}
				else
				{
					if (CollisionPrincess(&D3DXVECTOR3(g_aBoss[nCntBoss].CollisionBoss.mtxWorld._41, g_aBoss[nCntBoss].CollisionBoss.mtxWorld._42, g_aBoss[nCntBoss].CollisionBoss.mtxWorld._43), &g_aBoss[nCntBoss].CollisionBoss.posOld, g_aBoss[nCntBoss].CollisionBoss.fRadius,1) &&
						(g_aBoss[nCntBoss].CollisionBoss.nStartFrame <= g_aBoss[nCntBoss].nCntFrameBoss && g_aBoss[nCntBoss].CollisionBoss.nEndFrame >= g_aBoss[nCntBoss].nCntFrameBoss))
					{	//�P�̗͌���
						DecreaseLifebar(5);
					}
					else if (CollisionPlayer(&D3DXVECTOR3(g_aBoss[nCntBoss].CollisionBoss.mtxWorld._41, g_aBoss[nCntBoss].CollisionBoss.mtxWorld._42, g_aBoss[nCntBoss].CollisionBoss.mtxWorld._43), &g_aBoss[nCntBoss].CollisionBoss.posOld, g_aBoss[nCntBoss].CollisionBoss.fRadius) &&
						(g_aBoss[nCntBoss].CollisionBoss.nStartFrame <= g_aBoss[nCntBoss].nCntFrameBoss && g_aBoss[nCntBoss].CollisionBoss.nEndFrame >= g_aBoss[nCntBoss].nCntFrameBoss))
					{	//�v���C���[�̗͌���
						HitPlayer(3, &g_aBoss[nCntBoss].pos);
					}
				}
			}
			else
			{
				//�ړ�
				if (g_aBoss[nCntBoss].state != BOSSSTATE_ATTACK && !g_aBoss[nCntBoss].bStop)
				{
					MoveBoss(fDistX, fDistZ, fDistans,nCntBoss);
				}
			}

			OBSTACLE *pObstacle = GetObstacle();

			//��Q���j��
			for (int nCntObstacle = 0; nCntObstacle < OBSTACLE_MAX; nCntObstacle++, pObstacle++)
			{
				if (pObstacle->bUse)
				{
					if (CollisionObstacle(&g_aBoss[nCntBoss].pos, &g_aBoss[nCntBoss].posOld, &g_aBoss[nCntBoss].move, g_aBoss[nCntBoss].fRadius, nCntObstacle))
					{
						g_aBoss[nCntBoss].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						fDistX = pObstacle->pos.x - g_aBoss[nCntBoss].pos.x;
						fDistZ = pObstacle->pos.z - g_aBoss[nCntBoss].pos.z;

						if (g_aBoss[nCntBoss].state != BOSSSTATE_ATTACK && g_aBoss[nCntBoss].nCntAttack % 60 == 0)
						{
							g_aBoss[nCntBoss].state = BOSSSTATE_ATTACK;
							g_aBoss[nCntBoss].nCntFrameBoss = 0;
							g_aBoss[nCntBoss].nCntKeyBoss = 0;
							DeleteObstacle(5, nCntObstacle);
							PlaySound(SOUND_LABEL_SE_NAIL);
						}
					}
				}
			}

			//�ړ�����������
			if (g_aBoss[nCntBoss].state != BOSSSTATE_ATTACK && !g_aBoss[nCntBoss].bStop)
			{
				g_aBoss[nCntBoss].rotDest.y = atan2f(fDistX, fDistZ) + D3DX_PI;
			}
			//��]
			if (g_aBoss[nCntBoss].rotmove.y = g_aBoss[nCntBoss].rotDest.y - g_aBoss[nCntBoss].rot.y)
			{
				if (g_aBoss[nCntBoss].rotmove.y < -D3DX_PI)
				{
					g_aBoss[nCntBoss].rotmove.y += D3DX_PI * 2;
				}
				else if (g_aBoss[nCntBoss].rotmove.y > D3DX_PI)
				{
					g_aBoss[nCntBoss].rotmove.y -= D3DX_PI * 2;
				}
			}
			if (g_aBoss[nCntBoss].rot.y += g_aBoss[nCntBoss].rotmove.y * BOSS_ROTSPEAD)
			{
				if (g_aBoss[nCntBoss].rot.y > D3DX_PI)
				{
					g_aBoss[nCntBoss].rot.y -= D3DX_PI * 2;
				}
				else if (g_aBoss[nCntBoss].rot.y < -D3DX_PI)
				{
					g_aBoss[nCntBoss].rot.y += D3DX_PI * 2;
				}
			}

			//����
			g_aBoss[nCntBoss].move.x += (0.0f - g_aBoss[nCntBoss].move.x)*0.25f;
			g_aBoss[nCntBoss].move.z += (0.0f - g_aBoss[nCntBoss].move.z)*0.25f;

			//�ړ�
			g_aBoss[nCntBoss].pos.x += g_aBoss[nCntBoss].move.x;
			g_aBoss[nCntBoss].pos.z += g_aBoss[nCntBoss].move.z;
			g_aBoss[nCntBoss].pos.y += g_aBoss[nCntBoss].move.y;

			WAVE *pWave = GetWave();

			//���@�����蔻��
			if (pWave->state == WAVE_STATE_ATTACK)
			{
				if (CollisionWave(&g_aBoss[nCntBoss].pos, &g_aBoss[nCntBoss].posOld, &g_aBoss[nCntBoss].move, g_aBoss[nCntBoss].fRadius))
				{
					g_aBoss[nCntBoss].state = BOSSSTATE_HIT;
					g_aBoss[nCntBoss].nCntFrameBoss = 0;
					g_aBoss[nCntBoss].nCntKeyBoss = 0;
					g_aBoss[nCntBoss].bStop = true;
				}
			}

			//��Q�������蔻��
			pObstacle = GetObstacle();

			for (int nCntObstacle = 0; nCntObstacle < OBSTACLE_MAX; nCntObstacle++, pObstacle++)
			{
				if (pObstacle->bUse)
				{
					CollisionObstacle(&g_aBoss[nCntBoss].pos, &g_aBoss[nCntBoss].posOld, &g_aBoss[nCntBoss].move, g_aBoss[nCntBoss].fRadius, nCntObstacle);
				}
			}


			if (g_aBoss[nCntBoss].pos.y < 0)
			{	//�n�ʂɂ߂荞�܂Ȃ��悤��
				g_aBoss[nCntBoss].pos.y = 0;
				g_aBoss[nCntBoss].move.y = 0;
			}
			//���E�ǔ���
			if (g_aBoss[nCntBoss].pos.x > MESHFIELD_SIZE* MESHFIELD_W * 0.5f - g_aBoss[nCntBoss].fRadius)
			{
				g_aBoss[nCntBoss].pos.x = MESHFIELD_SIZE* MESHFIELD_W * 0.5f - g_aBoss[nCntBoss].fRadius;
			}
			if (g_aBoss[nCntBoss].pos.x < -MESHFIELD_SIZE* MESHFIELD_W * 0.5f + g_aBoss[nCntBoss].fRadius)
			{
				g_aBoss[nCntBoss].pos.x = -MESHFIELD_SIZE* MESHFIELD_W * 0.5f + g_aBoss[nCntBoss].fRadius;
			}

			//����O�ǔ���
			if (g_aBoss[nCntBoss].pos.z > MESHFIELD_SIZE* MESHFIELD_D * 0.5f - g_aBoss[nCntBoss].fRadius)
			{
				g_aBoss[nCntBoss].pos.z = MESHFIELD_SIZE* MESHFIELD_D * 0.5f - g_aBoss[nCntBoss].fRadius;
			}
			if (g_aBoss[nCntBoss].pos.z < -MESHFIELD_SIZE* MESHFIELD_D * 0.5f + g_aBoss[nCntBoss].fRadius)
			{
				g_aBoss[nCntBoss].pos.z = -MESHFIELD_SIZE * MESHFIELD_D * 0.5f + g_aBoss[nCntBoss].fRadius;
			}

			//���[�V�����Đ�
			if (g_aBoss[nCntBoss].bStop && g_aBoss[nCntBoss].nCntKeyBoss >= 1)
			{
			}
			else
			{
				BossMotion(nCntBoss);
			}

			//�e�̈ʒu��ݒ�
			SetPositionSadow(g_aBoss[nCntBoss].nIdxShadowBoss, g_aBoss[nCntBoss].pos, BOSS_SHADOW + ((0.0f - g_aBoss[nCntBoss].pos.y)*-0.1f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f - ((0.0f - g_aBoss[nCntBoss].pos.y)*-0.008f)));
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans, mtxParent;	// �v�Z�p�}�g���b�N�X
	D3DXMATERIAL		*pMat;	//���݂̃}�e���A���ۑ��p
	D3DMATERIAL9		matDef;	//�}�e���A���f�[�^�ւ̃|�C���^
	D3DMATERIAL9		matKeep;	//�ۑ��p
	BOSS *pBoss = GetBoss();

	//�{�X�̐ݒ�
	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++, pBoss++)
	{
		if (pBoss->bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pBoss->mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pBoss->rot.y, pBoss->rot.x, pBoss->rot.z);

			D3DXMatrixMultiply(&pBoss->mtxWorld, &pBoss->mtxWorld, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pBoss->pos.x, pBoss->pos.y, pBoss->pos.z);

			D3DXMatrixMultiply(&pBoss->mtxWorld, &pBoss->mtxWorld, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pBoss->mtxWorld);

			//���f���̐ݒ�
			for (int nCntParts = 0; nCntParts < g_LoadBoss.nNumBossParts; nCntParts++)
			{
				//�e�̎���
				if (pBoss->aModel[nCntParts].nIdxModelParent == -1)
				{
					mtxParent = pBoss->mtxWorld;
				}
				else
				{
					mtxParent = pBoss->aModel[pBoss->aModel[nCntParts].nIdxModelParent].mtxWorld;
				}
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&pBoss->aModel[nCntParts].mtxWorld);

				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot, pBoss->aModel[nCntParts].rot.y, pBoss->aModel[nCntParts].rot.x, pBoss->aModel[nCntParts].rot.z);

				D3DXMatrixMultiply(&pBoss->aModel[nCntParts].mtxWorld, &pBoss->aModel[nCntParts].mtxWorld, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, pBoss->aModel[nCntParts].pos.x, pBoss->aModel[nCntParts].pos.y, pBoss->aModel[nCntParts].pos.z);

				D3DXMatrixMultiply(&pBoss->aModel[nCntParts].mtxWorld, &pBoss->aModel[nCntParts].mtxWorld, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

				D3DXMatrixMultiply(&pBoss->aModel[nCntParts].mtxWorld, &pBoss->aModel[nCntParts].mtxWorld, &mtxParent);	//���[���h�s�񁁃��[���h�s�񁖐e�̃��[���h�s��
				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &pBoss->aModel[nCntParts].mtxWorld);

				// ���݂̃}�e���A�����擾
				pDevice->GetMaterial(&matDef);

				// �}�e���A�����ɑ΂���|�C���^���擾
				pMat = (D3DXMATERIAL*)pBoss->aModel[nCntParts].pBuffMat->GetBufferPointer();

				// �e�N�X�`���̐ݒ������Ȃ炱��(1���̎�)

				for (int nCntMat = 0; nCntMat < (int)pBoss->aModel[nCntParts].nNumMat; nCntMat++)
				{
					if (pBoss->bUse == true)
					{
						// �e�N�X�`���̐ݒ������Ȃ炱��(�������̎�)
						pDevice->SetTexture(0, g_pTextureBoss[nCntMat]);

						matKeep = pMat[nCntMat].MatD3D;	//�ۑ�

						if (pBoss->bDamege)
						{
							pMat[nCntMat].MatD3D.Diffuse.r = 255;	//�ԐF�ύX
						}
						else if (g_aBoss[nCntBoss].bStop)
						{	//���F�ɕς���
							pMat[nCntMat].MatD3D.Diffuse.r = 155;
							pMat[nCntMat].MatD3D.Diffuse.g = 155;
						}

						// �}�e���A���̐ݒ�
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

						pMat[nCntMat].MatD3D = matKeep;	//�F��߂�

						// �`��
						pBoss->aModel[nCntParts].pMesh->DrawSubset(nCntMat);	//�i�q���
					}
				}

				// �}�e���A�����f�t�H���g�ɖ߂�
				pDevice->SetMaterial(&matDef);
			}
			//�����蔻��̐ݒ�
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBoss[nCntBoss].CollisionBoss.mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBoss[nCntBoss].CollisionBoss.rot.y, g_aBoss[nCntBoss].CollisionBoss.rot.x, g_aBoss[nCntBoss].CollisionBoss.rot.z);

			D3DXMatrixMultiply(&g_aBoss[nCntBoss].CollisionBoss.mtxWorld, &g_aBoss[nCntBoss].CollisionBoss.mtxWorld, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aBoss[nCntBoss].CollisionBoss.pos.x, g_aBoss[nCntBoss].CollisionBoss.pos.y, g_aBoss[nCntBoss].CollisionBoss.pos.z);

			D3DXMatrixMultiply(&g_aBoss[nCntBoss].CollisionBoss.mtxWorld, &g_aBoss[nCntBoss].CollisionBoss.mtxWorld, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

			//�e�q���f
			D3DXMatrixMultiply(&g_aBoss[nCntBoss].CollisionBoss.mtxWorld, &g_aBoss[nCntBoss].CollisionBoss.mtxWorld, &pBoss->aModel[g_aBoss[nCntBoss].CollisionBoss.nIdxModelParent].mtxWorld);	//���[���h�s�񁁃��[���h�s�񁖐e�̃��[���h�s��

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBoss[nCntBoss].CollisionBoss.mtxWorld);
		}
	}
}

//=============================================================================
// �{�X�̐ݒ�
//=============================================================================
void SetBoss(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (!g_aBoss[nCntBoss].bUse)
		{
			g_aBoss[nCntBoss].pos = pos;
			g_aBoss[nCntBoss].rot = rot;
			g_aBoss[nCntBoss].state = BOSSSTATE_NEUTRAL;
			g_aBoss[nCntBoss].nLife = g_LoadBoss.nMaxLife;
			g_aBoss[nCntBoss].bUse = true;
			g_NumBoss++;
			//�e�̍쐬
			g_aBoss[nCntBoss].nIdxShadowBoss = SetSadow(g_aBoss[nCntBoss].pos, g_aBoss[nCntBoss].rot, BOSS_SHADOW, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
			break;
		}
	}
}

//=============================================================================
// �{�X�̈ړ��֐�
//=============================================================================
void MoveBoss(float fDistX, float fDistZ, float fDistans,int nCntBoss)
{
		//�ǔ�
		g_aBoss[nCntBoss].move.x += (fDistX / fDistans * g_aBoss[nCntBoss].fSpeed);
		g_aBoss[nCntBoss].move.z += (fDistZ / fDistans * g_aBoss[nCntBoss].fSpeed);

		//�ړ�����������
		g_aBoss[nCntBoss].rotDest.y = atan2f(fDistX, fDistZ) + D3DX_PI;

		//��]
		if (g_aBoss[nCntBoss].rotmove.y = g_aBoss[nCntBoss].rotDest.y - g_aBoss[nCntBoss].rot.y)
		{
			if (g_aBoss[nCntBoss].rotmove.y < -D3DX_PI)
			{
				g_aBoss[nCntBoss].rotmove.y += D3DX_PI * 2;
			}
			else if (g_aBoss[nCntBoss].rotmove.y > D3DX_PI)
			{
				g_aBoss[nCntBoss].rotmove.y -= D3DX_PI * 2;
			}
		}
		if (g_aBoss[nCntBoss].rot.y += g_aBoss[nCntBoss].rotmove.y * BOSS_ROTSPEAD)
		{
			if (g_aBoss[nCntBoss].rot.y > D3DX_PI)
			{
				g_aBoss[nCntBoss].rot.y -= D3DX_PI * 2;
			}
			else if (g_aBoss[nCntBoss].rot.y < -D3DX_PI)
			{
				g_aBoss[nCntBoss].rot.y += D3DX_PI * 2;
			}
		}
}

//=============================================================================
// �����蔻��
//=============================================================================
bool CollisionBoss(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Radius,int nCntBoss)
{
	bool bLand = false;
	float fDistance = (pPos->x - g_aBoss[nCntBoss].pos.x)*(pPos->x - g_aBoss[nCntBoss].pos.x) + (pPos->z - g_aBoss[nCntBoss].pos.z)*(pPos->z - g_aBoss[nCntBoss].pos.z);
	float fLength = (Radius + g_aBoss[nCntBoss].fRadius)*(Radius + g_aBoss[nCntBoss].fRadius);
	float fDiff = (Radius + g_aBoss[nCntBoss].fRadius) - (float)sqrt(fDistance);	//��
	D3DXVECTOR3 vec = D3DXVECTOR3((pPos->x - g_aBoss[nCntBoss].pos.x), 0.0f, (pPos->z - g_aBoss[nCntBoss].pos.z));	//�x�N�g��
	D3DXVec3Normalize(&vec, &vec);

	if (g_aBoss[nCntBoss].bUse)
	{
		if (fDistance < fLength)
		{
			bLand = true;
			pPos->x = pPosOld->x + vec.x * fDiff;
			pPos->z = pPosOld->z + vec.z * fDiff;
		}
	}
	return bLand;
}

//=============================================================================
//	���[�V�����̍Đ�
//=============================================================================
void BossMotion(int nCntBoss)
{
	//�p�[�c����
	for (int nCntMotion = 0; nCntMotion < g_LoadBoss.nNumBossParts; nCntMotion++)
	{
		KEY *pKey, *pNextKey;
		int nNextKey = g_aBoss[nCntBoss].nCntKeyBoss + 1;

		if (nNextKey >= g_aBossMotionInfo[g_aBoss[nCntBoss].state].nNumKey)
		{
			if (g_aBossMotionInfo[g_aBoss[nCntBoss].state].bLoop)
			{
				nNextKey = 0;
			}
			else
			{
				if (g_aBoss[nCntBoss].state == BOSSSTATE_DETH /*&& *GetGameState() == GAMESTATE_NORMAL*/)
				{
					g_aBoss[nCntBoss].bUse = false;
					DeleteShadow(g_aBoss[nCntBoss].nIdxShadowBoss);
				}
				else
				{
					g_aBoss[nCntBoss].state = BOSSSTATE_NEUTRAL;
					g_aBoss[nCntBoss].nCntFrameBoss = 0;
					g_aBoss[nCntBoss].nCntKeyBoss = 0;
				}
			}
		}
		pKey = &g_aBossMotionInfo[g_aBoss[nCntBoss].state].KeyInfo[g_aBoss[nCntBoss].nCntKeyBoss].Key[nCntMotion];	//���݂̃L�[	
		pNextKey = &g_aBossMotionInfo[g_aBoss[nCntBoss].state].KeyInfo[nNextKey].Key[nCntMotion];	//���̃L�[		

		//��]
		g_aBoss[nCntBoss].aModel[nCntMotion].rotMove = (pNextKey->rot - pKey->rot)*((float)g_aBoss[nCntBoss].nCntFrameBoss / (float)g_aBossMotionInfo[g_aBoss[nCntBoss].state].KeyInfo[g_aBoss[nCntBoss].nCntKeyBoss].nFrame);
		g_aBoss[nCntBoss].aModel[nCntMotion].rot = pKey->rot + g_aBoss[nCntBoss].aModel[nCntMotion].rotMove;
	}
}

//=============================================================================
// �_���[�W����
//=============================================================================
void HitBoss(int Damege, D3DXVECTOR3 *pPos, int nCntBoss)
{
	D3DXVECTOR3 vec = D3DXVECTOR3((g_aBoss[nCntBoss].pos.x - pPos->x), 0.0f, (g_aBoss[nCntBoss].pos.z - pPos->z));	//�x�N�g��
	D3DXVec3Normalize(&vec, &vec);

	if (g_aBoss[nCntBoss].bUse)
	{
		g_aBoss[nCntBoss].nLife -= Damege;
		g_aBoss[nCntBoss].state = BOSSSTATE_HIT;
		g_aBoss[nCntBoss].nCntFrameBoss = 0;
		g_aBoss[nCntBoss].nCntKeyBoss = 0;
		g_aBoss[nCntBoss].bDamege = true;
		g_aBoss[nCntBoss].bStop = false;
		PlaySound(SOUND_LABEL_SE_DAMAGE);

		//�p�[�e�B�N������
		float fAngle = 0;
		float fSpeed = 5;
		for (int nCnt = 0; nCnt < 20; nCnt++)
		{
			fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;	//0�`3.14���o���@*-1�Ő��������߂�
			D3DXVECTOR3 move = D3DXVECTOR3(sinf(fAngle)*fSpeed, cosf(fAngle)*fSpeed, 0.0f);
			SetParticle(D3DXVECTOR3(g_aBoss[nCntBoss].pos.x, g_aBoss[nCntBoss].aModel[0].pos.y + 20, g_aBoss[nCntBoss].pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, 10, 10);
		}
		//�͂������
		g_aBoss[nCntBoss].move.x = g_aBoss[nCntBoss].move.x + vec.x * 10;
		g_aBoss[nCntBoss].move.z = g_aBoss[nCntBoss].move.z + vec.z * 10;

		if (g_aBoss[nCntBoss].nLife <= 0)
		{	//���S
			AddScore(1500);
			g_aBoss[nCntBoss].state = BOSSSTATE_DETH;
			g_aBoss[nCntBoss].nCntFrameBoss = 0;
			g_aBoss[nCntBoss].nCntKeyBoss = 0;
			g_NumBoss--;

		}
	}
}

//=============================================================================
// �{�X���̎擾
//=============================================================================
BOSS *GetBoss(void)
{
	return &g_aBoss[0];
}

int GetBossUse(void)
{
	return g_NumBoss;
}
//=============================================================================
//	�t�@�C���ǂݍ���
//=============================================================================
void LoadBoss(void)
{
	//�t�@�C���|�C���g
	FILE *pFile;

	//�ϐ��錾
	char *pStrCur = NULL;	//������̐擪�ւ̃|�C���^
	char cLine[MAX_CHARA];	//������ǂݍ��ݗp
	char cStr[MAX_CHARA];	//�����񔲂��o���p
	char *cPunctuation;				//������؂�p
	int nCnt = 0;			//�󔒃J�E���^�[
	int nCntModel = 0;		//���f���ԍ�
	int nCntMotion = 0;		//���[�V�����ԍ�
	int nCntKey = 0;		//���݃L�[
	int nCntParts;			//�p�[�c�ԍ�

	pFile = fopen("data/motion_boss.txt", "r");

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
				g_LoadBoss.nNumBoss = atoi(cStr);	//atoi�@char�^��int�^�ɕϊ�����
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
					strcpy(g_LoadBoss.cBossFileNeme[nCntModel], pStrCur);
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

						if (memcmp(&cLine[nCnt], "MOVE = ", strlen("MOVE = ")) == 0)
						{	//�ړ����x
							pStrCur += strlen("MOVE = ");
							strcpy(cStr, pStrCur);
							g_LoadBoss.fSpeed = (float)atof(cStr);	//atof�@char�^��float�^�ɕϊ�����
						}
						if (memcmp(&cLine[nCnt], "RADIUS = ", strlen("RADIUS = ")) == 0)
						{	//���a
							pStrCur += strlen("RADIUS = ");
							strcpy(cStr, pStrCur);
							g_LoadBoss.fRadius = (float)atof(cStr);
						}
						if (memcmp(&cLine[nCnt], "LIFE = ", strlen("LIFE = ")) == 0)
						{	//�̗�
							pStrCur += strlen("LIFE = ");
							strcpy(cStr, pStrCur);
							g_LoadBoss.nMaxLife = atoi(cStr);
						}
						if (memcmp(&cLine[nCnt], "NUM_PARTS = ", strlen("NUM_PARTS = ")) == 0)
						{	//�ǂݍ��ރp�[�c��
							pStrCur += strlen("NUM_PARTS = ");
							strcpy(cStr, pStrCur);
							g_LoadBoss.nNumBossParts = atoi(cStr);
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
									g_LoadBoss.nParent[nCntParts] = atoi(cStr);
								}
								if (memcmp(&cLine[nCnt], "POS = ", strlen("POS = ")) == 0)
								{	//�ʒu
									pStrCur += strlen("POS = ");
									strcpy(cStr, pStrCur);

									cPunctuation = strtok(cStr, " ");	//�������󔒂��Ƃɋ�؂��ĕ���
									g_LoadBoss.pos[nCntParts].x = (float)atof(cPunctuation);

									cPunctuation = strtok(NULL, " ");
									g_LoadBoss.pos[nCntParts].y = (float)atof(cPunctuation);

									cPunctuation = strtok(NULL, " ");
									g_LoadBoss.pos[nCntParts].z = (float)atof(cPunctuation);
								}
								if (memcmp(&cLine[nCnt], "ROT = ", strlen("ROT = ")) == 0)
								{	//�p�x
									pStrCur += strlen("ROT = ");
									strcpy(cStr, pStrCur);
									cPunctuation = strtok(cStr, " ");
									g_LoadBoss.rot[nCntParts].x = (float)atof(cPunctuation);
									cPunctuation = strtok(NULL, " ");
									g_LoadBoss.rot[nCntParts].y = (float)atof(cPunctuation);
									cPunctuation = strtok(NULL, " ");
									g_LoadBoss.rot[nCntParts].z = (float)atof(cPunctuation);
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
						g_aBossMotionInfo[nCntMotion].bLoop = atoi(cStr);
					}
					if (memcmp(&cLine[nCnt], "NUM_KEY = ", strlen("NUM_KEY = ")) == 0)
					{	//�L�[��
						pStrCur += strlen("NUM_KEY = ");
						strcpy(cStr, pStrCur);
						g_aBossMotionInfo[nCntMotion].nNumKey = atoi(cStr);
					}
					if (memcmp(&cLine[nCnt], "COLLISION = ", strlen("COLLISION = ")) == 0)
					{	//�����蔻��
						pStrCur += strlen("COLLISION = ");
						strcpy(cStr, pStrCur);

						//�e�̔ԍ�
						cPunctuation = strtok(cStr, " ");
						g_LoadBoss.nCollParent = atoi(cPunctuation);
						//�I�t�Z�b�g�l
						//X
						cPunctuation = strtok(NULL, " ");
						g_LoadBoss.Collpos.x = (float)atof(cPunctuation);
						//Y
						cPunctuation = strtok(NULL, " ");
						g_LoadBoss.Collpos.y = (float)atof(cPunctuation);
						//Z
						cPunctuation = strtok(NULL, " ");
						g_LoadBoss.Collpos.z = (float)atof(cPunctuation);
						//���a
						cPunctuation = strtok(NULL, " ");
						g_LoadBoss.fCollRadius = (float)atof(cPunctuation);
						//�����蔻����J�n����t���[����
						cPunctuation = strtok(NULL, " ");
						g_LoadBoss.nStart = atoi(cPunctuation);
						//�I������t���[����
						cPunctuation = strtok(NULL, " ");
						g_LoadBoss.nEnd = atoi(cPunctuation);
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
								g_aBossMotionInfo[nCntMotion].KeyInfo[nCntKey].nFrame = atoi(cStr);
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
										cPunctuation = strtok(cStr, " ");
										g_aBossMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos.x = (float)atof(cPunctuation);
										cPunctuation = strtok(NULL, " ");
										g_aBossMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos.y = (float)atof(cPunctuation);
										cPunctuation = strtok(NULL, " ");
										g_aBossMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos.z = (float)atof(cPunctuation);
									}
									if (memcmp(&cLine[nCnt], "ROT = ", strlen("ROT = ")) == 0)
									{	//�p�x
										pStrCur += strlen("ROT = ");
										strcpy(cStr, pStrCur);
										cPunctuation = strtok(cStr, " ");
										g_aBossMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot.x = (float)atof(cPunctuation);
										cPunctuation = strtok(NULL, " ");
										g_aBossMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot.y = (float)atof(cPunctuation);
										cPunctuation = strtok(NULL, " ");
										g_aBossMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot.z = (float)atof(cPunctuation);
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
