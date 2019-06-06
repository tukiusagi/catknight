//=============================================================================
//
// �{�X���� [boss.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "boss.h"
#include "enemy.h"
#include "shadow.h"
#include "princess.h"
#include "player.h"
#include "wave.h"
#include "obstacle.h"
#include "effect.h"
#include "score.h"
#include "sound.h"
#include "particle.h"
#include "enemywave.h"
#include "fileload.h"
#include "lifePrincess.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define BOSS_FILENAME	"data/motion_boss.txt"		//�ǂݍ��ރe�N�X�`����

#define BOSS_SHADOW			(15.0f)	//�e�̑傫��
#define BOSS_ROTSPEAD		(0.1f)	//��]���x
#define BOSS_KEY_MAX		(4)		//�L�[�̍ő吔
#define BOSS_WAVE_MAX		(1)		//wave�ő吔
#define MAX_CHARA			(256)	//�ǂݍ��ޕ����̍ő吔
#define BOSS_SCORE			(1500)	//����X�R�A
#define ATTACK_PRINCESS		(5);

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9	g_pTextureBoss[6] = {};	//�e�N�X�`���̃|�C���^
BOSS				g_aBoss[MAX_BOSS];		//�{�X���
int					g_NumBoss = 0;			//�{�X��
LOAD *pLoadBoss = GetLoad(CHARA_BOSS);		//�t�@�C�����

//=============================================================================
// ����������
//=============================================================================
void InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �ʒu�E�����̏����ݒ�
	LoadFile(BOSS_FILENAME, CHARA_BOSS);
	g_NumBoss = 0;

	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		g_aBoss[nCntBoss].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].fRadius = pLoadBoss->fRadius;
		g_aBoss[nCntBoss].fSpeed = pLoadBoss->fSpeed;
		g_aBoss[nCntBoss].nLife = pLoadBoss->nMaxLife;
		g_aBoss[nCntBoss].nAttack = pLoadBoss->nAttack;
		g_aBoss[nCntBoss].state = BOSSSTATE_NEUTRAL;
		g_aBoss[nCntBoss].bUse = false;
		g_aBoss[nCntBoss].bHit = false;
		g_aBoss[nCntBoss].bDamege = false;
		g_aBoss[nCntBoss].bStop = false;
		g_aBoss[nCntBoss].nCntState = 0;
		g_aBoss[nCntBoss].nCntFrameBoss = 0;
		g_aBoss[nCntBoss].nCntKeyBoss = 0;
		g_aBoss[nCntBoss].nCntAttack = 0;

		g_aBoss[nCntBoss].CollisionBoss.nIdxModelParent = pLoadBoss->nCollParent;
		g_aBoss[nCntBoss].CollisionBoss.pos = pLoadBoss->Collpos;
		g_aBoss[nCntBoss].CollisionBoss.fRadius = pLoadBoss->fCollRadius;
		g_aBoss[nCntBoss].CollisionBoss.nStartFrame = pLoadBoss->nStart;
		g_aBoss[nCntBoss].CollisionBoss.nEndFrame = pLoadBoss->nEnd;
		g_aBoss[nCntBoss].CollisionBoss.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].CollisionBoss.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// X�t�@�C���̓ǂݍ���
		for (int nCntParts = 0; nCntParts < pLoadBoss->nNumParts; nCntParts++)
		{
			g_aBoss[nCntBoss].aModel[nCntParts].nIdxModelParent = pLoadBoss->nParent[nCntParts];
			g_aBoss[nCntBoss].aModel[nCntParts].pos = pLoadBoss->pos[nCntParts];
			g_aBoss[nCntBoss].aModel[nCntParts].rot = pLoadBoss->rot[nCntParts];
			g_aBoss[nCntBoss].aModel[nCntParts].pFilename = pLoadBoss->cFileNeme[nCntParts];

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
		for (int nCntParts = 0; nCntParts < pLoadBoss->nNumParts; nCntParts++)
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
	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (g_aBoss[nCntBoss].bUse)
		{
			g_aBoss[nCntBoss].nCntAttack++;	//�U���^�C�~���O�J�E���^�[

			g_aBoss[nCntBoss].posOld = g_aBoss[nCntBoss].pos;
			g_aBoss[nCntBoss].CollisionBoss.posOld = g_aBoss[nCntBoss].CollisionBoss.pos;

			if (g_aBoss[nCntBoss].bDamege)
			{	//�_���[�W���󂯂���Ԃ��ς��
				g_aBoss[nCntBoss].nCntState++;
				if (g_aBoss[nCntBoss].nCntState % DAMEGE_WAITTIME == 0)
				{	//�F��߂�
					g_aBoss[nCntBoss].bDamege = false;
				}
			}
			else if (g_aBoss[nCntBoss].bStop)
			{	//�}�q���
				g_aBoss[nCntBoss].nCntState++;
				if (g_aBoss[nCntBoss].nCntState % PARALYZE_WAITTIME == 0)
				{	//��ԉ���
					g_aBoss[nCntBoss].bStop = false;
					g_aBoss[nCntBoss].nCntState = 0;
				}
			}

			//�t���[������]
			g_aBoss[nCntBoss].nCntFrameBoss++;

			if (g_aBoss[nCntBoss].bStop && g_aBoss[nCntBoss].nCntKeyBoss >= 1)
			{	//�}�q��Ԃł̓X�g�b�v
			}
			else if (g_aBoss[nCntBoss].nCntFrameBoss > pLoadBoss->MotionInfo[g_aBoss[nCntBoss].state].KeyInfo[g_aBoss[nCntBoss].nCntKeyBoss].nFrame)
			{
				g_aBoss[nCntBoss].nCntFrameBoss = 0;
				g_aBoss[nCntBoss].nCntKeyBoss++;	//�L�[����

				if (g_aBoss[nCntBoss].nCntKeyBoss >= pLoadBoss->MotionInfo[g_aBoss[nCntBoss].state].nNumKey)
				{
					g_aBoss[nCntBoss].nCntKeyBoss = 0;	//�L�[���Z�b�g
				}
			}

			//���[�V�����̐؂�ւ�
			if (g_aBoss[nCntBoss].state != BOSSSTATE_ATTACK && g_aBoss[nCntBoss].state != BOSSSTATE_DETH && g_aBoss[nCntBoss].state != BOSSSTATE_HIT)
			{
				if (g_aBoss[nCntBoss].move.x > LATEST_SPEED || g_aBoss[nCntBoss].move.z > LATEST_SPEED ||
					g_aBoss[nCntBoss].move.x < -LATEST_SPEED || g_aBoss[nCntBoss].move.z < -LATEST_SPEED)
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
			g_aBoss[nCntBoss].move.y -= GRAVITY;

			//�����Ƃ̋������ׂċ߂����Ɋ��t��
			float fDistX, fDistZ, fDistans;
			CalculateDist(fDistX, fDistZ, fDistans, g_aBoss[nCntBoss].pos);

			//�ړ�
			if (CollisionPlayer(&g_aBoss[nCntBoss].pos, &g_aBoss[nCntBoss].posOld, g_aBoss[nCntBoss].fRadius) ||
				CollisionPrincess(&g_aBoss[nCntBoss].pos, &g_aBoss[nCntBoss].posOld, g_aBoss[nCntBoss].fRadius,1))
			{	//�P��R�m�ɐڐG���Ă���Ƃ�
				AttackBoss(nCntBoss);
			}
			else
			{
				//�ړ�
				if (g_aBoss[nCntBoss].state != BOSSSTATE_ATTACK && !g_aBoss[nCntBoss].bStop)
				{
					MoveBoss(fDistX, fDistZ, fDistans,nCntBoss);
				}
			}

			//��Q���j��
			OBSTACLE *pObstacle = GetObstacle();
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
			RotationDir(g_aBoss[nCntBoss].rotmove, g_aBoss[nCntBoss].rotDest, g_aBoss[nCntBoss].rot, BOSS_ROTSPEAD);

			//����
			g_aBoss[nCntBoss].move.x += (0.0f - g_aBoss[nCntBoss].move.x)*INERTIA;
			g_aBoss[nCntBoss].move.z += (0.0f - g_aBoss[nCntBoss].move.z)*INERTIA;

			//�ړ�
			g_aBoss[nCntBoss].pos.x += g_aBoss[nCntBoss].move.x;
			g_aBoss[nCntBoss].pos.z += g_aBoss[nCntBoss].move.z;
			g_aBoss[nCntBoss].pos.y += g_aBoss[nCntBoss].move.y;

			//���@�����蔻��
			WAVE *pWave = GetWave();
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
			ENEMY *pEnemy = GetEnemy();
			for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++)
			{	//�G�Ƃ̓����蔻��
				if (pEnemy->bUse)
				{
					CollisionEnemy(&g_aBoss[nCntBoss].pos, &g_aBoss[nCntBoss].posOld, g_aBoss[nCntBoss].fRadius, nCnt);
				}
			}
			BOSS *pBoss = GetBoss();
			for (int nCnt = 0; nCnt < MAX_BOSS; nCnt++,pBoss++)
			{
				if (pBoss->bUse && nCnt != nCntBoss)
				{
					CollisionBoss(&g_aBoss[nCntBoss].pos, &g_aBoss[nCntBoss].posOld, g_aBoss[nCntBoss].fRadius, nCnt);
				}
			}

			if (g_aBoss[nCntBoss].pos.y < 0)
			{	//�n�ʂɂ߂荞�܂Ȃ��悤��
				g_aBoss[nCntBoss].pos.y = 0;
				g_aBoss[nCntBoss].move.y = 0;
			}

			//�Ǔ����蔻��
			CollisionWall(&g_aBoss[nCntBoss].pos, g_aBoss[nCntBoss].fRadius);

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
			for (int nCntParts = 0; nCntParts < pLoadBoss->nNumParts; nCntParts++)
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
void SetBoss(D3DXVECTOR3 pos)
{
	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (!g_aBoss[nCntBoss].bUse)
		{
			g_aBoss[nCntBoss].pos = pos;
			g_aBoss[nCntBoss].state = BOSSSTATE_NEUTRAL;
			g_aBoss[nCntBoss].nLife = pLoadBoss->nMaxLife;
			g_aBoss[nCntBoss].bUse = true;
			g_NumBoss++;
			//�e�̍쐬
			g_aBoss[nCntBoss].nIdxShadowBoss = SetSadow(g_aBoss[nCntBoss].pos, g_aBoss[nCntBoss].rot, BOSS_SHADOW, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
			break;
		}
	}
}

//=============================================================================
// �{�X�̍폜
//=============================================================================
void DeleteBoss()
{
	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (g_aBoss[nCntBoss].bUse)
		{
			g_aBoss[nCntBoss].bUse = false;
			g_aBoss[nCntBoss].bHit = false;
			g_aBoss[nCntBoss].bDamege = false;
			g_aBoss[nCntBoss].bStop = false;
			g_aBoss[nCntBoss].state = BOSSSTATE_NEUTRAL;
			DeleteShadow(g_aBoss[nCntBoss].nIdxShadowBoss);
		}
	}
	g_NumBoss = 0;
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
// �U��
//=============================================================================
void AttackBoss(int nCntBoss)
{
	if (g_aBoss[nCntBoss].state != BOSSSTATE_ATTACK)
	{
		if ((rand() % ATTACK_TIMING == 0 || g_aBoss[nCntBoss].nCntAttack % ATTACK_TIMING == 0) && g_aBoss[nCntBoss].state != BOSSSTATE_DETH)
		{	//3�b��1�񂭂炢�̊m���ōU��
			g_aBoss[nCntBoss].state = BOSSSTATE_ATTACK;
			g_aBoss[nCntBoss].nCntFrameBoss = 0;
			g_aBoss[nCntBoss].nCntKeyBoss = 0;
			PlaySound(SOUND_LABEL_SE_NAIL);
		}
	}
	else
	{
		if (CollisionPrincess(&D3DXVECTOR3(g_aBoss[nCntBoss].CollisionBoss.mtxWorld._41, g_aBoss[nCntBoss].CollisionBoss.mtxWorld._42, g_aBoss[nCntBoss].CollisionBoss.mtxWorld._43), &g_aBoss[nCntBoss].CollisionBoss.posOld, g_aBoss[nCntBoss].CollisionBoss.fRadius, 1) &&
			(g_aBoss[nCntBoss].CollisionBoss.nStartFrame <= g_aBoss[nCntBoss].nCntFrameBoss && g_aBoss[nCntBoss].CollisionBoss.nEndFrame >= g_aBoss[nCntBoss].nCntFrameBoss))
		{	//�P�̗͌���
			DecreasePLife(g_aBoss[nCntBoss].nAttack);
		}
		else if (CollisionPlayer(&D3DXVECTOR3(g_aBoss[nCntBoss].CollisionBoss.mtxWorld._41, g_aBoss[nCntBoss].CollisionBoss.mtxWorld._42, g_aBoss[nCntBoss].CollisionBoss.mtxWorld._43), &g_aBoss[nCntBoss].CollisionBoss.posOld, g_aBoss[nCntBoss].CollisionBoss.fRadius) &&
			(g_aBoss[nCntBoss].CollisionBoss.nStartFrame <= g_aBoss[nCntBoss].nCntFrameBoss && g_aBoss[nCntBoss].CollisionBoss.nEndFrame >= g_aBoss[nCntBoss].nCntFrameBoss))
		{	//�v���C���[�̗͌���
			HitPlayer(g_aBoss[nCntBoss].nAttack / 2, &g_aBoss[nCntBoss].pos);
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
	for (int nCntMotion = 0; nCntMotion < pLoadBoss->nNumParts; nCntMotion++)
	{
		KEY *pKey, *pNextKey;
		int nNextKey = g_aBoss[nCntBoss].nCntKeyBoss + 1;

		if (nNextKey >= pLoadBoss->MotionInfo[g_aBoss[nCntBoss].state].nNumKey)
		{
			if (pLoadBoss->MotionInfo[g_aBoss[nCntBoss].state].bLoop)
			{
				nNextKey = 0;
			}
			else
			{
				if (g_aBoss[nCntBoss].state == BOSSSTATE_DETH)
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
		pKey = &pLoadBoss->MotionInfo[g_aBoss[nCntBoss].state].KeyInfo[g_aBoss[nCntBoss].nCntKeyBoss].Key[nCntMotion];	//���݂̃L�[	
		pNextKey = &pLoadBoss->MotionInfo[g_aBoss[nCntBoss].state].KeyInfo[nNextKey].Key[nCntMotion];	//���̃L�[		

		//��]
		g_aBoss[nCntBoss].aModel[nCntMotion].rotMove = (pNextKey->rot - pKey->rot)*((float)g_aBoss[nCntBoss].nCntFrameBoss / (float)pLoadBoss->MotionInfo[g_aBoss[nCntBoss].state].KeyInfo[g_aBoss[nCntBoss].nCntKeyBoss].nFrame);
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
			AddScore(BOSS_SCORE);
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

//=============================================================================
// �{�X�����̎擾
//=============================================================================
int GetBossNum(void)
{
	return g_NumBoss;
}
