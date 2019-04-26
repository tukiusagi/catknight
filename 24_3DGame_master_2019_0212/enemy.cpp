//=============================================================================
//
// �G���� [enemy.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "enemy.h"
#include "input.h"
#include "joystick.h"
#include "camera.h"
#include "shadow.h"
#include "meshfield.h"
#include "line.h"
#include "princess.h"
#include "player.h"
#include "game.h"
#include "boss.h"
#include "lifebar.h"
#include "life.h"
#include "wave.h"
#include "obstacle.h"
#include "effect.h"
#include "score.h"
#include "sound.h"
#include "particle.h"
#include "enemywave.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define ENEMY_TEXTURE	"data/TEXTURE/Suede-bk.jpg"	//�ǂݍ��ރe�N�X�`����
#define ENEMY_TEXTURE2	"data/TEXTURE/white.png"	//�ǂݍ��ރe�N�X�`����
#define ENEMY_SHADOW	(15.0f)			//�e�̑傫��
#define ENEMY_ROTSPEAD	(0.1f)			//��]���x
#define ENEMY_KEY_MAX	(4)				//�L�[�̍ő吔
#define MAX_CHARA		(256)			//�ǂݍ��ޕ����̍ő吔
#define ENEMY_SCORE		(300)			//�ǂݍ��ޕ����̍ő吔

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
	KEY	Key[ENEMY_MAX];	//�L�[�v�f[�p�[�c��]
} KEY_INFO;

//���[�V�������
typedef struct
{
	bool		bLoop;		//���[�v�̗L��
	int			nNumKey;	//�L�[��
	KEY_INFO	KeyInfo[ENEMY_KEY_MAX];	//�L�[���[�L�[�̐�]
} MOTION_INFO;

//=============================================================================
// ���[�h�p�̍\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos[ENEMY_MAX];	//�ʒu
	D3DXVECTOR3	rot[ENEMY_MAX];	//�p�x
	char		cEnemyFileNeme[ENEMY_MAX][MAX_CHARA];	//���f���t�@�C����
	int			nParent[ENEMY_MAX];	//�e�̔ԍ�
	int			nNumEnemy;			//�ǂݍ��ރ��f����
	int			nNumEnemyParts;		//�ǂݍ��ރp�[�c��
	int			nMaxLife;			//�ő�̗�
	float		fSpeed;				//���x
	float		fRadius;			//���a
	D3DXVECTOR3	Collpos;			//�ʒu(�����蔻��)
	int			nCollParent;		//�e�̔ԍ�(�����蔻��)
	float		fCollRadius;		//���a(�����蔻��)
	int			nStart;				//�J�n����t���[��
	int			nEnd;				//�I������t���[��
}LOADENEMY;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
#ifdef _DEBUG
void DrawMaxEnemy(void);
#endif

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
ENEMY				g_aEnemy[MAX_ENEMY];					//�G���
LOADENEMY	g_LoadEnemy;
LPDIRECT3DTEXTURE9	g_pTextureEnemy[6] = {};	//�e�N�X�`���̃|�C���^
int g_nEnemyMax = 0;		//�G�̑���

MOTION_INFO	g_aEnemyMotionInfo[ENEMYSTATE_MAX];

#ifdef _DEBUG
LPD3DXFONT	g_pFontEnemy = NULL;			// �t�H���g�ւ̃|�C���^
int			g_nCountEnemy = 0;		//�G�̑���
#endif

//=============================================================================
// ����������
//=============================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_nEnemyMax = 0;

	LoadEnemy();

	// �ʒu�E�����̏����ݒ�
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NEUTRAL;
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].bHit = false;
		g_aEnemy[nCntEnemy].bDamege = false;
		g_aEnemy[nCntEnemy].bStop = false;
		g_aEnemy[nCntEnemy].nCntFrameEnemy = 0;
		g_aEnemy[nCntEnemy].nCntKeyEnemy = 0;
		g_aEnemy[nCntEnemy].nCntAttack = 0;
		g_aEnemy[nCntEnemy].nCntState = 0;
		g_aEnemy[nCntEnemy].fRadius = g_LoadEnemy.fRadius;
		g_aEnemy[nCntEnemy].fSpeed = g_LoadEnemy.fSpeed;
		g_aEnemy[nCntEnemy].nLife = g_LoadEnemy.nMaxLife;

		g_aEnemy[nCntEnemy].CollisionEnemy.nIdxModelParent = g_LoadEnemy.nCollParent;
		g_aEnemy[nCntEnemy].CollisionEnemy.pos = g_LoadEnemy.Collpos;
		g_aEnemy[nCntEnemy].CollisionEnemy.fRadius = g_LoadEnemy.fCollRadius;
		g_aEnemy[nCntEnemy].CollisionEnemy.nStartFrame = g_LoadEnemy.nStart;
		g_aEnemy[nCntEnemy].CollisionEnemy.nEndFrame = g_LoadEnemy.nEnd;
		g_aEnemy[nCntEnemy].CollisionEnemy.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].CollisionEnemy.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// X�t�@�C���̓ǂݍ���
		for (int nCntParts = 0; nCntParts < g_LoadEnemy.nNumEnemyParts; nCntParts++)
		{
			g_aEnemy[nCntEnemy].aModel[nCntParts].nIdxModelParent = g_LoadEnemy.nParent[nCntParts];
			g_aEnemy[nCntEnemy].aModel[nCntParts].pos = g_LoadEnemy.pos[nCntParts];
			g_aEnemy[nCntEnemy].aModel[nCntParts].rot = g_LoadEnemy.rot[nCntParts];
			g_aEnemy[nCntEnemy].aModel[nCntParts].pFilename = g_LoadEnemy.cEnemyFileNeme[nCntParts];

			D3DXLoadMeshFromX(g_aEnemy[nCntEnemy].aModel[nCntParts].pFilename,	//�ǂݍ��ރt�@�C����
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_aEnemy[nCntEnemy].aModel[nCntParts].pBuffMat,
				NULL,						
				&g_aEnemy[nCntEnemy].aModel[nCntParts].nNumMat,
				&g_aEnemy[nCntEnemy].aModel[nCntParts].pMesh);
		}
	}

#ifdef _DEBUG
	// �f�o�b�O�\���p�t�H���g��ݒ�
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "�l�r �S�V�b�N", &g_pFontEnemy);
#endif

}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		for (int nCntParts = 0; nCntParts < g_LoadEnemy.nNumEnemyParts; nCntParts++)
		{
			// ���b�V���̊J��
			if (g_aEnemy[nCntEnemy].aModel[nCntParts].pMesh != NULL)
			{
				g_aEnemy[nCntEnemy].aModel[nCntParts].pMesh->Release();
				g_aEnemy[nCntEnemy].aModel[nCntParts].pMesh = NULL;
			}

			// �}�e���A���̊J��
			if (g_aEnemy[nCntEnemy].aModel[nCntParts].pBuffMat != NULL)
			{
				g_aEnemy[nCntEnemy].aModel[nCntParts].pBuffMat->Release();
				g_aEnemy[nCntEnemy].aModel[nCntParts].pBuffMat = NULL;
			}
		}
	}

#ifdef _DEBUG
	if (g_pFontEnemy != NULL)
	{// �f�o�b�O�\���p�t�H���g�̊J��
		g_pFontEnemy->Release();
		g_pFontEnemy = NULL;
	}
#endif

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	CAMERA *pCamera;
	pCamera = GetCamera();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse)
		{
			g_aEnemy[nCntEnemy].nCntFrameEnemy++;
			g_aEnemy[nCntEnemy].nCntAttack++;

			g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;
			g_aEnemy[nCntEnemy].CollisionEnemy.posOld = g_aEnemy[nCntEnemy].CollisionEnemy.pos;

			if (g_aEnemy[nCntEnemy].bDamege)
			{	//�_���[�W��Ԃ̉���
				g_aEnemy[nCntEnemy].nCntState++;
				if (g_aEnemy[nCntEnemy].nCntState % 10 == 0)
				{
					g_aEnemy[nCntEnemy].bDamege = false;
					g_aEnemy[nCntEnemy].nCntState = 0;
				}
			}
			else if (g_aEnemy[nCntEnemy].bStop)
			{	//�}�q��Ԃ̉���
				g_aEnemy[nCntEnemy].nCntState++;
				if (g_aEnemy[nCntEnemy].nCntState % (60 * 3) == 0)
				{
					g_aEnemy[nCntEnemy].bStop = false;
					g_aEnemy[nCntEnemy].nCntState = 0;
				}
			}

			//�t���[������
			if (g_aEnemy[nCntEnemy].bStop && g_aEnemy[nCntEnemy].nCntKeyEnemy >= 1)
			{	//�}�q��Ԃł̓X�g�b�v
			}
			else if (g_aEnemy[nCntEnemy].nCntFrameEnemy > g_aEnemyMotionInfo[g_aEnemy[nCntEnemy].state].KeyInfo[g_aEnemy[nCntEnemy].nCntKeyEnemy].nFrame)
			{
				g_aEnemy[nCntEnemy].nCntFrameEnemy = 0;
				g_aEnemy[nCntEnemy].nCntKeyEnemy++;	//�L�[����

				if (g_aEnemy[nCntEnemy].nCntKeyEnemy >= g_aEnemyMotionInfo[g_aEnemy[nCntEnemy].state].nNumKey)
				{
					g_aEnemy[nCntEnemy].nCntKeyEnemy = 0;	//�L�[���Z�b�g
				}
			}

			//���[�V�����̐؂�ւ�
			if (g_aEnemy[nCntEnemy].state != ENEMYSTATE_ATTACK && g_aEnemy[nCntEnemy].state != ENEMYSTATE_DETH && g_aEnemy[nCntEnemy].state != ENEMYSTATE_HIT)
			{
				if (g_aEnemy[nCntEnemy].move.x > 0.5f || g_aEnemy[nCntEnemy].move.z > 0.5f ||
					g_aEnemy[nCntEnemy].move.x < -0.5f || g_aEnemy[nCntEnemy].move.z < -0.5f)
				{	//���x������Ƃ��͈ړ�
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_MOVE;
				}
				else
				{	//�ʏ�̓j���[�g�������[�V����
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NEUTRAL;
				}
			}

			if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_ATTACK)
			{	//�G�t�F�N�g�\��
				SetEffect(D3DXVECTOR3(g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld._41, g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld._42, g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld._43),
					D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), g_aEnemy[nCntEnemy].CollisionEnemy.fRadius + 5, 10);
			}

			//�d��
			g_aEnemy[nCntEnemy].move.y -= 0.8f;

			//�����Ƃ̋������ׂċ߂����Ɋ��t��
			float fDistX, fDistZ, fDistans;
			float fDistXPL, fDistZPL, fDistansPL;
			float fDistXPrincess, fDistZPrincess, fDistansPrincess;
			PLAYER *pPlayer;
			pPlayer = GetPlayer();
			PRINCESS *pPrincess;
			pPrincess = GetPrincess();

			if (*GetMode() == MODE_GAME)
			{
				//�v���C���[�Ƃ̋����v�Z
				fDistXPL = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;
				fDistZPL = pPlayer->pos.z - g_aEnemy[nCntEnemy].pos.z;
				fDistansPL = (float)sqrt(fDistXPL * fDistXPL + fDistZPL * fDistZPL);
				//�P�Ƃ̋����v�Z
				fDistXPrincess = pPrincess->pos.x - g_aEnemy[nCntEnemy].pos.x;
				fDistZPrincess = pPrincess->pos.z - g_aEnemy[nCntEnemy].pos.z;
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
			}
			//�ړ�
			if (CollisionPlayer(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, g_aEnemy[nCntEnemy].fRadius) ||
				CollisionPrincess(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, g_aEnemy[nCntEnemy].fRadius, 1))
			{	//�P��R�m�ɐڐG���Ă���Ƃ��͓����Ȃ�
				if (*GetMode() == MODE_GAME)
				{
					if (g_aEnemy[nCntEnemy].state != ENEMYSTATE_ATTACK)
					{

						if ((rand() % 180 == 0 || g_aEnemy[nCntEnemy].nCntAttack % 180 == 0) && g_aEnemy[nCntEnemy].state != ENEMYSTATE_DETH)
						{	//3�b��1�񂭂炢�̊m���ōU��
							g_aEnemy[nCntEnemy].state = ENEMYSTATE_ATTACK;
							g_aEnemy[nCntEnemy].nCntFrameEnemy = 0;
							g_aEnemy[nCntEnemy].nCntKeyEnemy = 0;
							g_aEnemy[nCntEnemy].nCntAttack = 0;
							PlaySound(SOUND_LABEL_SE_NAIL);
						}
					}
					else
					{	//�U����
						if (pPrincess->state != PRINCESSSTATE_DETH &&
							CollisionPrincess(&D3DXVECTOR3(g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld._41, g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld._42, g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld._43), &g_aEnemy[nCntEnemy].CollisionEnemy.posOld, g_aEnemy[nCntEnemy].CollisionEnemy.fRadius, 1) &&
							(g_aEnemy[nCntEnemy].CollisionEnemy.nStartFrame <= g_aEnemy[nCntEnemy].nCntFrameEnemy && g_aEnemy[nCntEnemy].CollisionEnemy.nEndFrame >= g_aEnemy[nCntEnemy].nCntFrameEnemy))
						{	//�P�̗͌���
							DecreaseLifebar(3);
						}
						else if (pPlayer->state != PLAYERSTATE_DETH &&
							CollisionPlayer(&D3DXVECTOR3(g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld._41, g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld._42, g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld._43), &g_aEnemy[nCntEnemy].CollisionEnemy.posOld, g_aEnemy[nCntEnemy].CollisionEnemy.fRadius) &&
							(g_aEnemy[nCntEnemy].CollisionEnemy.nStartFrame <= g_aEnemy[nCntEnemy].nCntFrameEnemy && g_aEnemy[nCntEnemy].CollisionEnemy.nEndFrame >= g_aEnemy[nCntEnemy].nCntFrameEnemy))
						{	//�v���C���[�̗͌���
							HitPlayer(1, &g_aEnemy[nCntEnemy].pos);
						}
					}
				}
			}
			else
			{
				if (g_aEnemy[nCntEnemy].state != ENEMYSTATE_ATTACK && !g_aEnemy[nCntEnemy].bStop && *GetMode() == MODE_GAME)
				{
					//�ړ�
					MoveEnemy(fDistX, fDistZ, fDistans, nCntEnemy);
				}
			}

			OBSTACLE *pObstacle = GetObstacle();

			//��Q������
			for (int nCntObstacle = 0; nCntObstacle < OBSTACLE_MAX; nCntObstacle++, pObstacle++)
			{
				if (pObstacle->bUse)
				{
					if (CollisionObstacle(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move, g_aEnemy[nCntEnemy].fRadius, nCntObstacle))
					{
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						fDistX = pObstacle->pos.x - g_aEnemy[nCntEnemy].pos.x;
						fDistZ = pObstacle->pos.z - g_aEnemy[nCntEnemy].pos.z;

						if (g_aEnemy[nCntEnemy].state != ENEMYSTATE_ATTACK && g_aEnemy[nCntEnemy].nCntAttack % 60 == 0 && g_aEnemy[nCntEnemy].state != ENEMYSTATE_DETH)
						{	//��Q���j��
							g_aEnemy[nCntEnemy].state = ENEMYSTATE_ATTACK;
							g_aEnemy[nCntEnemy].nCntFrameEnemy = 0;
							g_aEnemy[nCntEnemy].nCntKeyEnemy = 0;
							DeleteObstacle(1, nCntObstacle);
							PlaySound(SOUND_LABEL_SE_NAIL);
						}
					}
				}
			}

			//�ړ�����������
			if (g_aEnemy[nCntEnemy].state != ENEMYSTATE_ATTACK && !g_aEnemy[nCntEnemy].bStop && *GetMode() == MODE_GAME)
			{
				g_aEnemy[nCntEnemy].rotDest.y = atan2f(fDistX, fDistZ) + D3DX_PI;
			}

			//��]
			if (g_aEnemy[nCntEnemy].rotmove.y = g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y)
			{
				if (g_aEnemy[nCntEnemy].rotmove.y < -D3DX_PI)
				{
					g_aEnemy[nCntEnemy].rotmove.y += D3DX_PI * 2;
				}
				else if (g_aEnemy[nCntEnemy].rotmove.y > D3DX_PI)
				{
					g_aEnemy[nCntEnemy].rotmove.y -= D3DX_PI * 2;
				}
			}
			if (g_aEnemy[nCntEnemy].rot.y += g_aEnemy[nCntEnemy].rotmove.y * ENEMY_ROTSPEAD)
			{
				if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
				{
					g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
				}
				else if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
				{
					g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
				}
			}

			//����
			g_aEnemy[nCntEnemy].move.x += (0.0f - g_aEnemy[nCntEnemy].move.x)*0.25f;
			g_aEnemy[nCntEnemy].move.z += (0.0f - g_aEnemy[nCntEnemy].move.z)*0.25f;


			//�ړ�
			g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
			g_aEnemy[nCntEnemy].pos.z += g_aEnemy[nCntEnemy].move.z;
			g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y;


			WAVE *pWave = GetWave();

			if (pWave->state == WAVE_STATE_ATTACK)
			{
				if (CollisionWave(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move, g_aEnemy[nCntEnemy].fRadius))
				{	//���@�����蔻��
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_HIT;
					g_aEnemy[nCntEnemy].nCntFrameEnemy = 0;
					g_aEnemy[nCntEnemy].nCntKeyEnemy = 0;
					g_aEnemy[nCntEnemy].bStop = true;
				}
			}

			//�����蔻��
			pObstacle = GetObstacle();

			for (int nCntObstacle = 0; nCntObstacle < OBSTACLE_MAX; nCntObstacle++, pObstacle++)
			{
				if (pObstacle->bUse)
				{
					CollisionObstacle(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move, g_aEnemy[nCntEnemy].fRadius, nCntObstacle);
				}
			}


			if (g_aEnemy[nCntEnemy].pos.y < 0)
			{	//�n�ʂɂ߂荞�܂Ȃ��悤��
				g_aEnemy[nCntEnemy].pos.y = 0;
				g_aEnemy[nCntEnemy].move.y = 0;
			}
			//���E�ǔ���
			if (g_aEnemy[nCntEnemy].pos.x > MESHFIELD_SIZE* MESHFIELD_W * 0.5f - g_aEnemy[nCntEnemy].fRadius)
			{
				g_aEnemy[nCntEnemy].pos.x = MESHFIELD_SIZE* MESHFIELD_W * 0.5f - g_aEnemy[nCntEnemy].fRadius;
			}
			if (g_aEnemy[nCntEnemy].pos.x < -MESHFIELD_SIZE* MESHFIELD_W * 0.5f + g_aEnemy[nCntEnemy].fRadius)
			{
				g_aEnemy[nCntEnemy].pos.x = -MESHFIELD_SIZE* MESHFIELD_W * 0.5f + g_aEnemy[nCntEnemy].fRadius;
			}

			//����O�ǔ���
			if (g_aEnemy[nCntEnemy].pos.z > MESHFIELD_SIZE* MESHFIELD_D * 0.5f - g_aEnemy[nCntEnemy].fRadius)
			{
				g_aEnemy[nCntEnemy].pos.z = MESHFIELD_SIZE* MESHFIELD_D * 0.5f - g_aEnemy[nCntEnemy].fRadius;
			}
			if (g_aEnemy[nCntEnemy].pos.z < -MESHFIELD_SIZE* MESHFIELD_D * 0.5f + g_aEnemy[nCntEnemy].fRadius)
			{
				g_aEnemy[nCntEnemy].pos.z = -MESHFIELD_SIZE * MESHFIELD_D * 0.5f + g_aEnemy[nCntEnemy].fRadius;
			}

			//���[�V�����Đ�
			if (g_aEnemy[nCntEnemy].bStop && g_aEnemy[nCntEnemy].nCntKeyEnemy >= 1)
			{
			}
			else
			{
				EnemyMotion(nCntEnemy);
			}
			//�e�̈ʒu��ݒ�
			SetPositionSadow(g_aEnemy[nCntEnemy].nIdxShadowEnemy, g_aEnemy[nCntEnemy].pos, ENEMY_SHADOW + ((0.0f - g_aEnemy[nCntEnemy].pos.y)*-0.1f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f - ((0.0f - g_aEnemy[nCntEnemy].pos.y)*-0.008f)));
		}
	}

#ifdef _DEBUG
	g_nCountEnemy = g_nEnemyMax;
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans, mtxParent;	// �v�Z�p�}�g���b�N�X
	D3DXMATERIAL		*pMat;	//���݂̃}�e���A���ۑ��p
	D3DMATERIAL9		matDef;	//�}�e���A���f�[�^�ւ̃|�C���^
	D3DMATERIAL9		matKeep;	//�ۑ��p

//�G�̐ݒ�
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);

		D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);

		D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);

		//���f���̐ݒ�
		for (int nCntParts = 0; nCntParts < g_LoadEnemy.nNumEnemyParts; nCntParts++)
		{
			//�e�̎���
			if (g_aEnemy[nCntEnemy].aModel[nCntParts].nIdxModelParent == -1)
			{
				mtxParent = g_aEnemy[nCntEnemy].mtxWorld;
			}
			else
			{
				mtxParent = g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].aModel[nCntParts].nIdxModelParent].mtxWorld;
			}
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].aModel[nCntParts].rot.y, g_aEnemy[nCntEnemy].aModel[nCntParts].rot.x, g_aEnemy[nCntEnemy].aModel[nCntParts].rot.z);

			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld, &g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].aModel[nCntParts].pos.x, g_aEnemy[nCntEnemy].aModel[nCntParts].pos.y, g_aEnemy[nCntEnemy].aModel[nCntParts].pos.z);

			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld, &g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld, &g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld, &mtxParent);	//���[���h�s�񁁃��[���h�s�񁖐e�̃��[���h�s��
			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].aModel[nCntParts].pBuffMat->GetBufferPointer();

			// �e�N�X�`���̐ݒ������Ȃ炱��(1���̎�)

			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].aModel[nCntParts].nNumMat; nCntMat++)
			{
				if (g_aEnemy[nCntEnemy].bUse == true)
				{
					// �e�N�X�`���̐ݒ������Ȃ炱��(�������̎�)
					pDevice->SetTexture(0, g_pTextureEnemy[nCntMat]);

					matKeep = pMat[nCntMat].MatD3D;	//���̕ۑ�

					if (g_aEnemy[nCntEnemy].bDamege)
					{	//�_���[�W���ԐF�ɕς���
						pMat[nCntMat].MatD3D.Diffuse.r = 255;
					}
					else if (g_aEnemy[nCntEnemy].bStop)
					{	//�}�q��Ԃŉ��F�ɕς���
						pMat[nCntMat].MatD3D.Diffuse.r = 155;	
						pMat[nCntMat].MatD3D.Diffuse.g = 155;	
					}

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					pMat[nCntMat].MatD3D = matKeep;	//�F��߂�

					// �`��
					g_aEnemy[nCntEnemy].aModel[nCntParts].pMesh->DrawSubset(nCntMat);	//�i�q���
				}
			}

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
		//�����蔻��̐ݒ�
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].CollisionEnemy.rot.y, g_aEnemy[nCntEnemy].CollisionEnemy.rot.x, g_aEnemy[nCntEnemy].CollisionEnemy.rot.z);

		D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld, &g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].CollisionEnemy.pos.x, g_aEnemy[nCntEnemy].CollisionEnemy.pos.y, g_aEnemy[nCntEnemy].CollisionEnemy.pos.z);

		D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld, &g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

		//�e�q���f
		D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld, &g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld, &g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].CollisionEnemy.nIdxModelParent].mtxWorld);	//���[���h�s�񁁃��[���h�s�񁖐e�̃��[���h�s��

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].CollisionEnemy.mtxWorld);
	}

#ifdef _DEBUG
	// �G�����\��
	DrawMaxEnemy();
#endif

}

//=============================================================================
// �G�̐ݒ�
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int Life)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (!g_aEnemy[nCntEnemy].bUse)	//�G���g�p����Ă��Ȃ��ꍇ
		{
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].rot = rot;
			g_aEnemy[nCntEnemy].nLife = Life;
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_NEUTRAL;
			g_aEnemy[nCntEnemy].bUse = true;

			//�e�̍쐬
			g_aEnemy[nCntEnemy].nIdxShadowEnemy = SetSadow(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].rot, ENEMY_SHADOW, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));

			g_nEnemyMax++;	//�G�̑������J�E���g
			break;
		}
	}
}

//=============================================================================
// �G�̏���
//=============================================================================
void DeleteEnemey(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse)	//�G���g�p����Ă���ꍇ
		{
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_NEUTRAL;
			g_aEnemy[nCntEnemy].bUse = false;
			g_aEnemy[nCntEnemy].bHit = false;
			g_aEnemy[nCntEnemy].bDamege = false;
			g_aEnemy[nCntEnemy].bStop = false;
			g_nEnemyMax--;
			DeleteShadow(g_aEnemy[nCntEnemy].nIdxShadowEnemy);

		}
	}

}
//=============================================================================
// �G�̈ړ��֐�
//=============================================================================
void MoveEnemy(float fDistX, float fDistZ, float fDistans, int nCntEnemy)
{
	float fSpeed = g_aEnemy[nCntEnemy].fSpeed;
	//�ǔ�
	if (*GetEwave() <= 3)
	{	//���Ղ͈ړ����x����
		fSpeed = g_aEnemy[nCntEnemy].fSpeed / 2.0f;
	}
	g_aEnemy[nCntEnemy].move.x += (fDistX / fDistans * fSpeed);
	g_aEnemy[nCntEnemy].move.z += (fDistZ / fDistans * fSpeed);

	//�ړ�����������
	g_aEnemy[nCntEnemy].rotDest.y = atan2f(fDistX, fDistZ)+ D3DX_PI;

	//��]
	if (g_aEnemy[nCntEnemy].rotmove.y = g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y)
	{
		if (g_aEnemy[nCntEnemy].rotmove.y < -D3DX_PI)
		{
			g_aEnemy[nCntEnemy].rotmove.y += D3DX_PI * 2;
		}
		else if (g_aEnemy[nCntEnemy].rotmove.y > D3DX_PI)
		{
			g_aEnemy[nCntEnemy].rotmove.y -= D3DX_PI * 2;
		}
	}
	if (g_aEnemy[nCntEnemy].rot.y += g_aEnemy[nCntEnemy].rotmove.y * ENEMY_ROTSPEAD)
	{
		if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
		{
			g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
		}
		else if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
		{
			g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
		}
	}
	
}

//=============================================================================
// �����蔻��
//=============================================================================
bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Radius, int nCntEnemy)
{
	bool bLand = false;
	float fDistance = (pPos->x - g_aEnemy[nCntEnemy].pos.x)*(pPos->x - g_aEnemy[nCntEnemy].pos.x) + (pPos->z - g_aEnemy[nCntEnemy].pos.z)*(pPos->z - g_aEnemy[nCntEnemy].pos.z);	//����
	float fLength = (Radius + g_aEnemy[nCntEnemy].fRadius)*(Radius + g_aEnemy[nCntEnemy].fRadius);	//����
	float fDiff = (Radius + g_aEnemy[nCntEnemy].fRadius) - (float)sqrt(fDistance);	//��
	D3DXVECTOR3 vec = D3DXVECTOR3((pPos->x - g_aEnemy[nCntEnemy].pos.x), 0.0f, (pPos->z - g_aEnemy[nCntEnemy].pos.z));	//�x�N�g��
	D3DXVec3Normalize(&vec, &vec);

	if (fDistance < fLength)
	{
		bLand = true;

		pPos->x = pPosOld->x + vec.x * fDiff;
		pPos->z = pPosOld->z + vec.z * fDiff;
	}

	return bLand;
}

//=============================================================================
//	���[�V�����̍Đ�
//=============================================================================
void EnemyMotion(int nCntEnemy)
{
	//�p�[�c����
	for (int nCntMotion = 0; nCntMotion < g_LoadEnemy.nNumEnemyParts; nCntMotion++)
	{
		KEY *pKey, *pNextKey;
		int nNextKey = g_aEnemy[nCntEnemy].nCntKeyEnemy + 1;

		if (nNextKey >= g_aEnemyMotionInfo[g_aEnemy[nCntEnemy].state].nNumKey)
		{
			if (g_aEnemyMotionInfo[g_aEnemy[nCntEnemy].state].bLoop)
			{
				nNextKey = 0;
			}
			else
			{
				if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_DETH)
				{	//���S��
					if (g_aEnemy[nCntEnemy].bUse)
					{
						g_aEnemy[nCntEnemy].bUse = false;
						if (*GetMode() == MODE_GAME)
						{
							AddScore(ENEMY_SCORE);	//�X�R�A���Z
						}
						g_nEnemyMax--;
						DeleteShadow(g_aEnemy[nCntEnemy].nIdxShadowEnemy);
					}
				}
				else
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NEUTRAL;
					g_aEnemy[nCntEnemy].nCntFrameEnemy = 0;
					g_aEnemy[nCntEnemy].nCntKeyEnemy = 0;
				}
			}
		}
		pKey = &g_aEnemyMotionInfo[g_aEnemy[nCntEnemy].state].KeyInfo[g_aEnemy[nCntEnemy].nCntKeyEnemy].Key[nCntMotion];	//���݂̃L�[	
		pNextKey = &g_aEnemyMotionInfo[g_aEnemy[nCntEnemy].state].KeyInfo[nNextKey].Key[nCntMotion];	//���̃L�[		

		//��]
		g_aEnemy[nCntEnemy].aModel[nCntMotion].rotMove = (pNextKey->rot - pKey->rot)*((float)g_aEnemy[nCntEnemy].nCntFrameEnemy / (float)g_aEnemyMotionInfo[g_aEnemy[nCntEnemy].state].KeyInfo[g_aEnemy[nCntEnemy].nCntKeyEnemy].nFrame);
		g_aEnemy[nCntEnemy].aModel[nCntMotion].rot = pKey->rot + g_aEnemy[nCntEnemy].aModel[nCntMotion].rotMove;
	}
}

//=============================================================================
// �_���[�W����
//=============================================================================
void HitEnemy(int nCntEnemy, int Damege, D3DXVECTOR3 *pPos)
{
	D3DXVECTOR3 vec = D3DXVECTOR3((g_aEnemy[nCntEnemy].pos.x - pPos->x), 0.0f, (g_aEnemy[nCntEnemy].pos.z - pPos->z));	//�x�N�g��
	D3DXVec3Normalize(&vec, &vec);

	g_aEnemy[nCntEnemy].nLife -= Damege;
	g_aEnemy[nCntEnemy].bStop = false;
	g_aEnemy[nCntEnemy].bDamege = true;

	//�q�b�g�G�t�F�N�g
	float fAngle = 0;
	float fSpeed = 5;
	for (int nCnt = 0; nCnt < 20; nCnt++)
	{
		fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;	//0�`3.14���o��
		D3DXVECTOR3 move = D3DXVECTOR3(sinf(fAngle)*fSpeed, cosf(fAngle)*fSpeed, 0.0f);
		SetParticle(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].aModel[0].pos.y+20, g_aEnemy[nCntEnemy].pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, 10, 10);
	}

	PlaySound(SOUND_LABEL_SE_DAMAGE);

	//�͂������
	g_aEnemy[nCntEnemy].move.x = g_aEnemy[nCntEnemy].move.x + vec.x * 10;
	g_aEnemy[nCntEnemy].move.z = g_aEnemy[nCntEnemy].move.z + vec.z * 10;

	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DETH;
		g_aEnemy[nCntEnemy].nCntFrameEnemy = 0;
		g_aEnemy[nCntEnemy].nCntKeyEnemy = 0;

	}
}

//=============================================================================
// �G���̎擾
//=============================================================================
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//=============================================================================
// �G���̎擾
//=============================================================================
int *GetNumEnemy(void)
{
	return &g_nEnemyMax;
}

#ifdef _DEBUG
//=============================================================================
// pos�\������
//=============================================================================
void DrawMaxEnemy(void)
{
	RECT rect = { 0, 100, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "�G���F%d\n", g_nCountEnemy);

	// �e�L�X�g�`��
	g_pFontEnemy->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}
#endif

//=============================================================================
//	�t�@�C���ǂݍ���
//=============================================================================
void LoadEnemy(void)
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

	pFile = fopen("data/motion_mouse.txt", "r");

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
				g_LoadEnemy.nNumEnemy = atoi(cStr);	//atoi�@char�^��int�^�ɕϊ�����
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
					strcpy(g_LoadEnemy.cEnemyFileNeme[nCntModel], pStrCur);
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
							g_LoadEnemy.fSpeed = (float)atof(cStr);	//atof�@char�^��float�^�ɕϊ�����
						}
						if (memcmp(&cLine[nCnt], "RADIUS = ", strlen("RADIUS = ")) == 0)
						{	//���a
							pStrCur += strlen("RADIUS = ");
							strcpy(cStr, pStrCur);
							g_LoadEnemy.fRadius = (float)atof(cStr);
						}
						if (memcmp(&cLine[nCnt], "LIFE = ", strlen("LIFE = ")) == 0)
						{	//�̗�
							pStrCur += strlen("LIFE = ");
							strcpy(cStr, pStrCur);
							g_LoadEnemy.nMaxLife = atoi(cStr);
						}
						if (memcmp(&cLine[nCnt], "NUM_PARTS = ", strlen("NUM_PARTS = ")) == 0)
						{	//�ǂݍ��ރp�[�c��
							pStrCur += strlen("NUM_PARTS = ");
							strcpy(cStr, pStrCur);
							g_LoadEnemy.nNumEnemyParts = atoi(cStr);
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
									g_LoadEnemy.nParent[nCntParts] = atoi(cStr);
								}
								if (memcmp(&cLine[nCnt], "POS = ", strlen("POS = ")) == 0)
								{	//�ʒu
									pStrCur += strlen("POS = ");
									strcpy(cStr, pStrCur);

									cPunctuation = strtok(cStr, " ");	//�������󔒂��Ƃɋ�؂��ĕ���
									g_LoadEnemy.pos[nCntParts].x = (float)atof(cPunctuation);

									cPunctuation = strtok(NULL, " ");
									g_LoadEnemy.pos[nCntParts].y = (float)atof(cPunctuation);

									cPunctuation = strtok(NULL, " ");
									g_LoadEnemy.pos[nCntParts].z = (float)atof(cPunctuation);
								}
								if (memcmp(&cLine[nCnt], "ROT = ", strlen("ROT = ")) == 0)
								{	//�p�x
									pStrCur += strlen("ROT = ");
									strcpy(cStr, pStrCur);
									cPunctuation = strtok(cStr, " ");
									g_LoadEnemy.rot[nCntParts].x = (float)atof(cPunctuation);
									cPunctuation = strtok(NULL, " ");
									g_LoadEnemy.rot[nCntParts].y = (float)atof(cPunctuation);
									cPunctuation = strtok(NULL, " ");
									g_LoadEnemy.rot[nCntParts].z = (float)atof(cPunctuation);
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
						g_aEnemyMotionInfo[nCntMotion].bLoop = atoi(cStr);
					}
					if (memcmp(&cLine[nCnt], "NUM_KEY = ", strlen("NUM_KEY = ")) == 0)
					{	//�L�[��
						pStrCur += strlen("NUM_KEY = ");
						strcpy(cStr, pStrCur);
						g_aEnemyMotionInfo[nCntMotion].nNumKey = atoi(cStr);
					}
					if (memcmp(&cLine[nCnt], "COLLISION = ", strlen("COLLISION = ")) == 0)
					{	//�����蔻��
						pStrCur += strlen("COLLISION = ");
						strcpy(cStr, pStrCur);

						//�e�̔ԍ�
						cPunctuation = strtok(cStr, " ");
						g_LoadEnemy.nCollParent = atoi(cPunctuation);
						//�I�t�Z�b�g�l
						//X
						cPunctuation = strtok(NULL, " ");
						g_LoadEnemy.Collpos.x = (float)atof(cPunctuation);
						//Y
						cPunctuation = strtok(NULL, " ");
						g_LoadEnemy.Collpos.y = (float)atof(cPunctuation);
						//Z
						cPunctuation = strtok(NULL, " ");
						g_LoadEnemy.Collpos.z = (float)atof(cPunctuation);
						//���a
						cPunctuation = strtok(NULL, " ");
						g_LoadEnemy.fCollRadius = (float)atof(cPunctuation);
						//�����蔻����J�n����t���[����
						cPunctuation = strtok(NULL, " ");
						g_LoadEnemy.nStart = atoi(cPunctuation);
						//�I������t���[����
						cPunctuation = strtok(NULL, " ");
						g_LoadEnemy.nEnd = atoi(cPunctuation);
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
								g_aEnemyMotionInfo[nCntMotion].KeyInfo[nCntKey].nFrame = atoi(cStr);
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
										g_aEnemyMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos.x = (float)atof(cPunctuation);
										cPunctuation = strtok(NULL, " ");
										g_aEnemyMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos.y = (float)atof(cPunctuation);
										cPunctuation = strtok(NULL, " ");
										g_aEnemyMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos.z = (float)atof(cPunctuation);
									}
									if (memcmp(&cLine[nCnt], "ROT = ", strlen("ROT = ")) == 0)
									{	//�p�x
										pStrCur += strlen("ROT = ");
										strcpy(cStr, pStrCur);
										cPunctuation = strtok(cStr, " ");
										g_aEnemyMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot.x = (float)atof(cPunctuation);
										cPunctuation = strtok(NULL, " ");
										g_aEnemyMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot.y = (float)atof(cPunctuation);
										cPunctuation = strtok(NULL, " ");
										g_aEnemyMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot.z = (float)atof(cPunctuation);
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
