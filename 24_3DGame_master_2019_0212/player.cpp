//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "joystick.h"
#include "camera.h"
#include "shadow.h"
#include "meshfield.h"
#include "line.h"
#include "princess.h"
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

//=============================================================================
// �}�N����`
//=============================================================================
#define PLAYER_TEXTURE	"data/TEXTURE/white.jpg"	//�ǂݍ��ރe�N�X�`����
#define PLAYER_SHADOW		(15.0f)			//�e�̑傫��
#define PLAYER_ROTSPEAD		(0.1f)			//��]���x
#define PLAYER_KEY_MAX		(4)				//�L�[�̍ő吔

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
	KEY	Key[PLAYER_MAX];	//�L�[�v�f[�p�[�c��]
} KEY_INFO;

//���[�V�������
typedef struct
{
	bool		bLoop;		//���[�v�̗L��
	int			nNumKey;	//�L�[��
	KEY_INFO	KeyInfo[PLAYER_KEY_MAX];	//�L�[���[�L�[�̐�]
} MOTION_INFO;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
#ifdef _DEBUG
void DrawPos(void);
#endif

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
PLAYER				g_Player;			//�v���C���[���
COLLISION			g_Collision;		//�����蔻����
MESH_ORBIT			g_Mesh;
LPDIRECT3DTEXTURE9	g_pTexture[6] = {};	//�e�N�X�`���̃|�C���^
int	g_nIdxShadow;		//�e��ID
int g_NumModel = 0;		//�ǂݍ��ރ��f����
int g_nNumParts = 0;	//�ǂݍ��ރp�[�c��
int g_nCntframe = 0;	//���݂̃t���[����
int g_nCntKey = 0;		//���݂̃L�[��
char acModelFileName[PLAYER_MAX][MAX_CHARA];	//���f���t�@�C�����i�[�p

#ifdef _DEBUG
LPD3DXFONT			g_pFontPL = NULL;			// �t�H���g�ւ̃|�C���^
D3DXVECTOR3			g_PLpos;
#endif

MOTION_INFO	g_aMotionInfo[PLAYERSTATE_MAX];

//=============================================================================
// ����������
//=============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�t�@�C���ǂݍ���
	LoadPlayer();

	// �ʒu�E�����̏����ݒ�
	g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, -40.0f);
	g_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.nLife = g_Player.nMaxLife;
	g_Player.state = PLAYERSTATE_NEUTRAL;
	g_Player.bAttack = false;
	g_Player.bDamege = false;
	g_Player.nCntState = 0;

	g_Collision.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Collision.rot= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�e�̍쐬
	g_nIdxShadow = SetSadow(g_Player.pos, g_Player.rot, PLAYER_SHADOW, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));

	// X�t�@�C���̓ǂݍ���
	for (int nCntModel = 0; nCntModel < g_NumModel; nCntModel++)
	{
		D3DXLoadMeshFromX(g_Player.aModel[nCntModel].pFilename,	//�ǂݍ��ރt�@�C����
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
	SetTrajectory(g_Mesh.pos[0], g_Mesh.pos[1], &g_Player.aModel[g_Mesh.nParent].mtxWorld, D3DXCOLOR(1.0f,0.5f,0.0f,1.0f));

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
	for (int nCntPlayer = 0; nCntPlayer < PLAYER_MAX; nCntPlayer++)
	{
		// ���b�V���̊J��
		if (g_Player.aModel[nCntPlayer].pMesh != NULL)
		{
			g_Player.aModel[nCntPlayer].pMesh->Release();
			g_Player.aModel[nCntPlayer].pMesh = NULL;
		}

		// �}�e���A���̊J��
		if (g_Player.aModel[nCntPlayer].pBuffMat != NULL)
		{
			g_Player.aModel[nCntPlayer].pBuffMat->Release();
			g_Player.aModel[nCntPlayer].pBuffMat = NULL;
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
	CAMERA *pCamera;
	pCamera = GetCamera();

	g_Player.posOld = g_Player.pos;
	g_Collision.posOld = g_Collision.pos;

	if (g_Player.bDamege)
	{
		g_Player.nCntState++;
		if (g_Player.nCntState % 10 == 0)
		{	//�_���[�W��ԉ���
			g_Player.bDamege = false;
		}
	}

	//�t���[������
	if (g_Player.state == PLAYERSTATE_DETH && g_nCntKey >= 1)
	{
		//���S���͑������Ȃ�
	}
	else
	{
		g_nCntframe++;
		if (g_nCntframe > g_aMotionInfo[g_Player.state].KeyInfo[g_nCntKey].nFrame)
		{
			g_nCntframe = 0;
			g_nCntKey++;
			if (g_nCntKey >= g_aMotionInfo[g_Player.state].nNumKey)
			{
				g_nCntKey = 0;
			}
		}
	}

	//���[�V�����̐؂�ւ�
	switch (*GetGameState())
	{
	case GAMESTATE_CLEAR:
		if (g_Player.state != PLAYERSTATE_ATTACK01 && g_Player.state != PLAYERSTATE_ATTACK02)
		{
			g_Player.state = PLAYERSTATE_WIN;
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
			if (g_Player.move.x > 0.5f || g_Player.move.z > 0.5f ||
				g_Player.move.x < -0.5f || g_Player.move.z < -0.5f)
			{
				g_Player.state = PLAYERSTATE_MOVE;
			}
			else
			{
				g_Player.state = PLAYERSTATE_NEUTRAL;
			}
		}
		break;
	}

	if (g_Player.state != PLAYERSTATE_DETH)
	{
		if (g_Player.state != PLAYERSTATE_ATTACK01 && g_Player.state != PLAYERSTATE_ATTACK02)
		{
			//�C�ӂ̃L�[�������ꂽ���ǂ���
			if (GetKeyboardPress(DIK_A) || GetJoystickPress(PAD_LEFT_L))
			{
				g_Player.state = PLAYERSTATE_MOVE;
				if (GetKeyboardPress(DIK_W) || GetJoystickPress(PAD_UP_L))
				{	//����
					g_Player.move.x += sinf(-D3DX_PI * 0.75f - pCamera->rot.y) * g_Player.fSpeed;	//�����l
					g_Player.move.z -= cosf(-D3DX_PI * 0.75f - pCamera->rot.y) * g_Player.fSpeed;
					g_Player.rotDest.y = D3DX_PI * 0.75f + pCamera->rot.y;
				}
				else if (GetKeyboardPress(DIK_S) || GetJoystickPress(PAD_DOWN_L))
				{	//����
					g_Player.move.x += sinf(-D3DX_PI * 0.25f - pCamera->rot.y) * g_Player.fSpeed;
					g_Player.move.z -= cosf(-D3DX_PI * 0.25f - pCamera->rot.y) * g_Player.fSpeed;
					g_Player.rotDest.y = D3DX_PI * 0.25f + pCamera->rot.y;
				}
				else
				{	//��
					g_Player.move.x += sinf(-D3DX_PI * 0.5f + pCamera->rot.y) * g_Player.fSpeed;
					g_Player.move.z += cosf(-D3DX_PI * 0.5f + pCamera->rot.y) * g_Player.fSpeed;
					g_Player.rotDest.y = D3DX_PI * 0.5f + pCamera->rot.y;
				}
			}
			else if (GetKeyboardPress(DIK_D) || GetJoystickPress(PAD_RIGHT_L))
			{
				g_Player.state = PLAYERSTATE_MOVE;

				if (GetKeyboardPress(DIK_W) || GetJoystickPress(PAD_UP_L))
				{	//�E��
					g_Player.move.x += sinf(D3DX_PI * 0.75f - pCamera->rot.y) * g_Player.fSpeed;
					g_Player.move.z -= cosf(D3DX_PI * 0.75f - pCamera->rot.y) * g_Player.fSpeed;
					g_Player.rotDest.y = -D3DX_PI * 0.75f + pCamera->rot.y;
				}
				else if (GetKeyboardPress(DIK_S) || GetJoystickPress(PAD_DOWN_L))
				{	//�E��
					g_Player.move.x += sinf(D3DX_PI * 0.25f - pCamera->rot.y) * g_Player.fSpeed;
					g_Player.move.z -= cosf(D3DX_PI * 0.25f - pCamera->rot.y) * g_Player.fSpeed;
					g_Player.rotDest.y = -D3DX_PI * 0.25f + pCamera->rot.y;
				}
				else
				{	//�E
					g_Player.move.x += sinf(D3DX_PI * 0.5f + pCamera->rot.y) * g_Player.fSpeed;
					g_Player.move.z += cosf(D3DX_PI * 0.5f + pCamera->rot.y) * g_Player.fSpeed;
					g_Player.rotDest.y = -D3DX_PI * 0.5f + pCamera->rot.y;
				}
			}
			else if (GetKeyboardPress(DIK_W) || GetJoystickPress(PAD_UP_L))
			{	//��
				g_Player.state = PLAYERSTATE_MOVE;

				g_Player.move.x -= sinf(D3DX_PI * 1.0f + pCamera->rot.y) * g_Player.fSpeed;
				g_Player.move.z -= cosf(D3DX_PI * 1.0f + pCamera->rot.y) * g_Player.fSpeed;
				g_Player.rotDest.y = D3DX_PI * 1.0f + pCamera->rot.y;
			}
			else if (GetKeyboardPress(DIK_S) || GetJoystickPress(PAD_DOWN_L))
			{	//��O
				g_Player.state = PLAYERSTATE_MOVE;

				g_Player.move.x -= sinf(D3DX_PI * 0.0f + pCamera->rot.y) * g_Player.fSpeed;
				g_Player.move.z -= cosf(D3DX_PI * 0.0f + pCamera->rot.y) * g_Player.fSpeed;
				g_Player.rotDest.y = D3DX_PI * 0.0f + pCamera->rot.y;
			}
		}
		if (g_Player.state != PLAYERSTATE_WIN && (GetKeyTrigger(DIK_SPACE)|| GetJoystickTrigger(PAD_B)))
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
	}
#ifdef _DEBUG
	if (GetKeyTrigger(DIK_J))
	{	// �W�����v
		g_Player.move.y += 10.0f;
	}
#endif

	//��]
	if (g_Player.rotmove.y = g_Player.rotDest.y - g_Player.rot.y)
	{
		if (g_Player.rotmove.y < -D3DX_PI)
		{
			g_Player.rotmove.y += D3DX_PI * 2;
		}
		else if (g_Player.rotmove.y > D3DX_PI)
		{
			g_Player.rotmove.y -= D3DX_PI * 2;
		}
	}
	if (g_Player.rot.y += g_Player.rotmove.y * PLAYER_ROTSPEAD)
	{
		if (g_Player.rot.y > D3DX_PI)
		{
			g_Player.rot.y -= D3DX_PI * 2;
		}
		else if (g_Player.rot.y < -D3DX_PI)
		{
			g_Player.rot.y += D3DX_PI * 2;
		}
	}

	//�d��
	g_Player.move.y -= 0.8f;

	//����
	g_Player.move.x += (0.0f - g_Player.move.x)*0.25f;
	g_Player.move.z += (0.0f - g_Player.move.z)*0.25f;

	//�ړ�
	g_Player.pos.x += g_Player.move.x;
	g_Player.pos.z += g_Player.move.z;
	g_Player.pos.y += g_Player.move.y;

	//�����蔻��
	ENEMY *pEnemy;
	pEnemy = GetEnemy();
	float fCollision = 5.0f;
	CollisionPrincess(&g_Player.pos, &g_Player.posOld, g_Player.fRadius, 0);

	OBSTACLE *pObstacle = GetObstacle();
	//��Q��
	for (int nCntObstacle = 0; nCntObstacle < OBSTACLE_MAX; nCntObstacle++, pObstacle++)
	{
		if (pObstacle->bUse)
		{
			CollisionObstacle(&g_Player.pos, &g_Player.posOld, &g_Player.move, g_Player.fRadius, nCntObstacle);
		}
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
				(g_Collision.nStartFrame <= g_nCntframe) && (g_nCntframe <= g_Collision.nEndFrame))
			{
				if (CollisionBoss(&D3DXVECTOR3(g_Collision.mtxWorld._41, g_Collision.mtxWorld._42, g_Collision.mtxWorld._43), &g_Collision.posOld, g_Collision.fRadius,nCntBoss))
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
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse)
		{
			if ((g_Player.state == PLAYERSTATE_ATTACK01 || g_Player.state == PLAYERSTATE_ATTACK02) &&
				(pEnemy->state != ENEMYSTATE_DETH) &&
				(g_Collision.nStartFrame <= g_nCntframe) && (g_nCntframe <= g_Collision.nEndFrame))
			{
				if (CollisionEnemy(&D3DXVECTOR3(g_Collision.mtxWorld._41, g_Collision.mtxWorld._42, g_Collision.mtxWorld._43), &g_Collision.posOld, g_Collision.fRadius, nCntEnemy))
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
	//���E�ǔ���
	if (g_Player.pos.x > MESHFIELD_SIZE * MESHFIELD_W * 0.5f - g_Player.fRadius)
	{
		g_Player.pos.x = MESHFIELD_SIZE * MESHFIELD_W * 0.5f - g_Player.fRadius;
	}
	if (g_Player.pos.x < -MESHFIELD_SIZE * MESHFIELD_W * 0.5f + g_Player.fRadius)
	{
		g_Player.pos.x = -MESHFIELD_SIZE * MESHFIELD_W * 0.5f + g_Player.fRadius;
	}
	//����O�ǔ���
	if (g_Player.pos.z > MESHFIELD_SIZE * MESHFIELD_D * 0.5f - g_Player.fRadius)
	{
		g_Player.pos.z = MESHFIELD_SIZE * MESHFIELD_D * 0.5f - g_Player.fRadius;
	}
	if (g_Player.pos.z < -MESHFIELD_SIZE * MESHFIELD_D * 0.5f + g_Player.fRadius)
	{
		g_Player.pos.z = -MESHFIELD_SIZE * MESHFIELD_D * 0.5f + g_Player.fRadius;
	}

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
	for (int nCntPlayer = 0; nCntPlayer < g_nNumParts; nCntPlayer++)
	{
		//�e�̎���
		if (g_Player.aModel[nCntPlayer].nIdxModelParent == -1)
		{
			mtxParent = g_Player.mtxWorld;
		}
		else
		{
			mtxParent = g_Player.aModel[g_Player.aModel[nCntPlayer].nIdxModelParent].mtxWorld;
		}
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Player.aModel[nCntPlayer].mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.aModel[nCntPlayer].rot.y, g_Player.aModel[nCntPlayer].rot.x, g_Player.aModel[nCntPlayer].rot.z);

		D3DXMatrixMultiply(&g_Player.aModel[nCntPlayer].mtxWorld, &g_Player.aModel[nCntPlayer].mtxWorld, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Player.aModel[nCntPlayer].pos.x, g_Player.aModel[nCntPlayer].pos.y, g_Player.aModel[nCntPlayer].pos.z);

		D3DXMatrixMultiply(&g_Player.aModel[nCntPlayer].mtxWorld, &g_Player.aModel[nCntPlayer].mtxWorld, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

		//�e�q���f
		D3DXMatrixMultiply(&g_Player.aModel[nCntPlayer].mtxWorld, &g_Player.aModel[nCntPlayer].mtxWorld, &mtxParent);	//���[���h�s�񁁃��[���h�s�񁖐e�̃��[���h�s��

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Player.aModel[nCntPlayer].mtxWorld);

		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		// �}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_Player.aModel[nCntPlayer].pBuffMat->GetBufferPointer();

		// �e�N�X�`���̐ݒ������Ȃ炱��(1���̎�)

		for (int nCntMat = 0; nCntMat < (int)g_Player.aModel[nCntPlayer].nNumMat; nCntMat++)
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
			g_Player.aModel[nCntPlayer].pMesh->DrawSubset(nCntMat);	//�i�q���
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
	D3DXMatrixTranslation(&mtxTrans, g_Collision.pos.x, g_Collision.pos.y, g_Collision.pos.z);

	D3DXMatrixMultiply(&g_Collision.mtxWorld, &g_Collision.mtxWorld, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

	//�e�q���f
	D3DXMatrixMultiply(&g_Collision.mtxWorld, &g_Collision.mtxWorld, &g_Player.aModel[g_Collision.nIdxModelParent].mtxWorld);	//���[���h�s�񁁃��[���h�s�񁖐e�̃��[���h�s��

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
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.nLife = g_Player.nMaxLife;
	g_Player.state = PLAYERSTATE_NEUTRAL;

}

//=============================================================================
// �v���C���[���̎擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

//=============================================================================
// �����蔻��
//=============================================================================
bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Radius)
{
	bool bLand = false;
	float fDistance = (pPos->x - g_Player.pos.x)*(pPos->x - g_Player.pos.x) + (pPos->z - g_Player.pos.z)*(pPos->z - g_Player.pos.z);	//2�_�Ԃ̋���
	float fLength = (Radius + g_Player.fRadius)*(Radius + g_Player.fRadius);	//���a
	float fDiff = (Radius + g_Player.fRadius) - (float)sqrt(fDistance);	//��
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
// �_���[�W����
//=============================================================================
void HitPlayer(int Damege, D3DXVECTOR3 *pPos)
{
	D3DXVECTOR3 vec = D3DXVECTOR3((g_Player.pos.x - pPos->x), 0.0f, (g_Player.pos.z - pPos->z));	//�x�N�g��
	D3DXVec3Normalize(&vec, &vec);

	DecreaseLife(1);
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
		g_Player.nLife = g_Player.nMaxLife;
		IncreaseLife(g_Player.nMaxLife);
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
		if (nNextKey >= g_aMotionInfo[g_Player.state].nNumKey)
		{
			if (g_aMotionInfo[g_Player.state].bLoop)
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

		pKey = &g_aMotionInfo[g_Player.state].KeyInfo[g_nCntKey].Key[nCntMotion];		//���݂̃L�[
		pNextKey = &g_aMotionInfo[g_Player.state].KeyInfo[nNextKey].Key[nCntMotion];	//���̃L�[

		//��]
		g_Player.aModel[nCntMotion].rotMove = (pNextKey->rot - pKey->rot)*((float)g_nCntframe / (float)g_aMotionInfo[g_Player.state].KeyInfo[g_nCntKey].nFrame);

		g_Player.aModel[nCntMotion].rot = pKey->rot + g_Player.aModel[nCntMotion].rotMove;
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

//=============================================================================
//	�t�@�C���ǂݍ���
//=============================================================================
void LoadPlayer(void)
{
	//�t�@�C���|�C���g
	FILE *pFile;

	//�ϐ��錾
	char *pStrCur = NULL;	//������̐擪�ւ̃|�C���^
	char cLine[MAX_CHARA];	//������ǂݍ��ݗp
	char cStr[MAX_CHARA];	//�����񔲂��o���p
	char *cPunctuation;		//������؂�p
	int nCnt = 0;			//�󔒃J�E���^�[
	int nCntModel = 0;		//���f���ԍ�
	int nCntMotion = 0;		//���[�V�����ԍ�
	int nCntKey = 0;		//���݃L�[
	int nCntParts;			//�p�[�c�ԍ�

	pFile = fopen("data/motion_Cat.txt", "r");

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
				g_NumModel = atoi(cStr);	//atoi�@char�^��int�^�ɕϊ�����
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
				strcpy(acModelFileName[nCntModel], pStrCur);
				g_Player.aModel[nCntModel].pFilename = acModelFileName[nCntModel];
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
						g_Player.fSpeed = (float)atof(cStr);	//atof�@char�^��float�^�ɕϊ�����
					}
					if (memcmp(&cLine[nCnt], "RADIUS = ", strlen("RADIUS = ")) == 0)
					{	//���a
						pStrCur += strlen("RADIUS = ");
						strcpy(cStr, pStrCur);
						g_Player.fRadius = (float)atof(cStr);
					}
					if (memcmp(&cLine[nCnt], "LIFE = ", strlen("LIFE = ")) == 0)
					{	//�̗�
						pStrCur += strlen("LIFE = ");
						strcpy(cStr, pStrCur);
						g_Player.nMaxLife = atoi(cStr);
					}
					if (memcmp(&cLine[nCnt], "NUM_PARTS = ", strlen("NUM_PARTS = ")) == 0)
					{	//�ǂݍ��ރp�[�c��
						pStrCur += strlen("NUM_PARTS = ");
						strcpy(cStr, pStrCur);
						g_nNumParts = atoi(cStr);
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
								g_Player.aModel[nCntParts].nIdxModelParent = atoi(cStr);
							}
							if (memcmp(&cLine[nCnt], "POS = ", strlen("POS = ")) == 0)
							{	//�ʒu
								pStrCur += strlen("POS = ");
								strcpy(cStr, pStrCur);

								cPunctuation = strtok(cStr, " ");	//�������󔒂��Ƃɋ�؂��ĕ���
								g_Player.aModel[nCntParts].pos.x = (float)atof(cPunctuation);

								cPunctuation = strtok(NULL, " ");
								g_Player.aModel[nCntParts].pos.y = (float)atof(cPunctuation);

								cPunctuation = strtok(NULL, " ");
								g_Player.aModel[nCntParts].pos.z = (float)atof(cPunctuation);
							}
							if (memcmp(&cLine[nCnt], "ROT = ", strlen("ROT = ")) == 0)
							{	//�p�x
								pStrCur += strlen("ROT = ");
								strcpy(cStr, pStrCur);
								cPunctuation = strtok(cStr, " ");
								g_Player.aModel[nCntParts].rot.x = (float)atof(cPunctuation);
								cPunctuation = strtok(NULL, " ");
								g_Player.aModel[nCntParts].rot.y = (float)atof(cPunctuation);
								cPunctuation = strtok(NULL, " ");
								g_Player.aModel[nCntParts].rot.z = (float)atof(cPunctuation);
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
						g_aMotionInfo[nCntMotion].bLoop = atoi(cStr);
					}
					if (memcmp(&cLine[nCnt], "NUM_KEY = ", strlen("NUM_KEY = ")) == 0)
					{	//�L�[��
						pStrCur += strlen("NUM_KEY = ");
						strcpy(cStr, pStrCur);
						g_aMotionInfo[nCntMotion].nNumKey = atoi(cStr);
					}
					if (memcmp(&cLine[nCnt], "COLLISION = ", strlen("COLLISION = ")) == 0)
					{	//�����蔻��
						pStrCur += strlen("COLLISION = ");
						strcpy(cStr, pStrCur);

						//�e�̔ԍ�
						cPunctuation = strtok(cStr, " ");
						g_Collision.nIdxModelParent = atoi(cPunctuation);
						//�I�t�Z�b�g�l
						//X
						cPunctuation = strtok(NULL, " ");
						g_Collision.pos.x = (float)atof(cPunctuation);	
						//Y
						cPunctuation = strtok(NULL, " ");
						g_Collision.pos.y = (float)atof(cPunctuation);	
						//Z
						cPunctuation = strtok(NULL, " ");
						g_Collision.pos.z = (float)atof(cPunctuation);	
						//���a
						cPunctuation = strtok(NULL, " ");
						g_Collision.fRadius = (float)atof(cPunctuation);
						//�����蔻����J�n����t���[����
						cPunctuation = strtok(NULL, " ");
						g_Collision.nStartFrame = atoi(cPunctuation);
						//�I������t���[����
						cPunctuation = strtok(NULL, " ");
						g_Collision.nEndFrame = atoi(cPunctuation);
					}
					if (memcmp(&cLine[nCnt], "ORBIT = ", strlen("ORBIT = ")) == 0)
					{	//�O��
						pStrCur += strlen("ORBIT = ");
						strcpy(cStr, pStrCur);

						//�e�̔ԍ�
						cPunctuation = strtok(cStr, " ");
						g_Mesh.nParent = atoi(cPunctuation);
						//�I�t�Z�b�g�l
						//X
						cPunctuation = strtok(NULL, " ");
						g_Mesh.pos[0].x = (float)atof(cPunctuation);
						//Y
						cPunctuation = strtok(NULL, " ");
						g_Mesh.pos[0].y = (float)atof(cPunctuation);
						//Z
						cPunctuation = strtok(NULL, " ");
						g_Mesh.pos[0].z = (float)atof(cPunctuation);
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
								g_aMotionInfo[nCntMotion].KeyInfo[nCntKey].nFrame = atoi(cStr);
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
										g_aMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos.x = (float)atof(cPunctuation);
										cPunctuation = strtok(NULL, " ");
										g_aMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos.y = (float)atof(cPunctuation);
										cPunctuation = strtok(NULL, " ");
										g_aMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos.z = (float)atof(cPunctuation);
									}
									if (memcmp(&cLine[nCnt], "ROT = ", strlen("ROT = ")) == 0)
									{	//�p�x
										pStrCur += strlen("ROT = ");
										strcpy(cStr, pStrCur);
										cPunctuation = strtok(cStr, " ");
										g_aMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot.x = (float)atof(cPunctuation);
										cPunctuation = strtok(NULL, " ");
										g_aMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot.y = (float)atof(cPunctuation);
										cPunctuation = strtok(NULL, " ");
										g_aMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot.z = (float)atof(cPunctuation);
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
