//=============================================================================
//
// ���@���� [wave.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "wave.h"
#include"game.h"
#include "input.h"
#include "joystick.h"
#include "princess.h"
#include "sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define WAVE_TEXTURE	"data/TEXTURE/white.png"	//�ǂݍ��ރe�N�X�`����
#define WAVE_HEIGHT			(25.0f)
#define WAVE_RADIUS			(30.0f)
#define WAVE_RADIUS_MAX		(WAVE_RADIUS * 5)

#define WAVE_Y			(1)		//�c������
#define WAVE_XZ			(15)	//������
#define SECOND			(60)	//�b

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexWave(LPDIRECT3DDEVICE9 pDevice);


//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffWave = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureWave = NULL;	//�e�N�X�`���̃|�C���^
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffWave = NULL;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^

WAVE g_wave;
D3DXVECTOR3 g_waveMove[WAVE_XZ];

//=============================================================================
// ����������
//=============================================================================
void InitWave(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �ʒu�E��]�̏����ݒ�
		g_wave.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_wave.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_wave.col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		g_wave.state = WAVE_STATE_CHARGE;
		g_wave.nNumVertex = (WAVE_XZ + 1) * (WAVE_Y + 1);
		g_wave.nNumPolygon = WAVE_XZ * WAVE_Y * 2;
		g_wave.nNumIndex = (WAVE_XZ + 1) * (WAVE_Y + 1);
		g_wave.nCharge = 0;
		g_wave.fRadius = WAVE_RADIUS;
		g_wave.fHeight = WAVE_HEIGHT;
		g_wave.bUse = false;

		for (int nCntMove = 0; nCntMove < WAVE_XZ; nCntMove++)
		{
			g_waveMove[nCntMove] = D3DXVECTOR3(0.0f, 0.5f, 0.0f);
		}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		WAVE_TEXTURE,
		&g_pTextureWave);

	// ���_���̍쐬
	MakeVertexWave(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitWave(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffWave != NULL)
	{
		g_pVtxBuffWave->Release();
		g_pVtxBuffWave = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̊J��
	if (g_pIdxBuffWave != NULL)
	{
		g_pIdxBuffWave->Release();
		g_pIdxBuffWave = NULL;
	}

	//�e�N�X�`���̊J��
		if (g_pTextureWave != NULL)
		{					 
			g_pTextureWave->Release();
			g_pTextureWave = NULL;
		}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateWave(void)
{
	PRINCESS *pPrincess;
	pPrincess = GetPrincess();

		if (g_wave.state == WAVE_STATE_CHARGE)
		{
			g_wave.nCharge++;
			if (g_wave.nCharge % (SECOND/2) == 0)
			{
				PlaySound(SOUND_LABEL_SE_CHARGE);
			}
			if (g_wave.nCharge > SECOND*5)
			{
				g_wave.state = WAVE_STATE_CHARGEMAX;
				PlaySound(SOUND_LABEL_SE_CHARGEMAX);

			}
		}
	VERTEX_3D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffWave->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	//���_�̐ݒ�
	for (int nCntY = 0; nCntY <= WAVE_Y; nCntY++)
	{
		for (int nCntXZ = 0; nCntXZ <= WAVE_XZ; nCntXZ++, pVtx++)
		{
			pVtx[0].pos.x = sinf((2 * D3DX_PI / WAVE_XZ) * nCntXZ) * g_wave.fRadius;
			pVtx[0].pos.z = cosf((2 * D3DX_PI / WAVE_XZ) * nCntXZ) * g_wave.fRadius;

			if (nCntXZ % 2 == 1 && nCntY > 0)
			{
				pVtx[0].pos.y += g_waveMove[nCntXZ].y;

				if (pVtx[0].pos.y > WAVE_HEIGHT + 10.0f || pVtx[0].pos.y < WAVE_HEIGHT - 10.0f)
				{
					g_waveMove[nCntXZ].y *= -1;
				}
			}
				switch (g_wave.state)
				{
				case WAVE_STATE_CHARGE:
					g_wave.col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
					break;

				case WAVE_STATE_CHARGEMAX:
					g_wave.col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
						if ((GetKeyTrigger(DIK_B) || GetJoystickTrigger(PAD_Y)))
						{
							pPrincess->state = PRINCESSSTATE_ATTACK;
							pPrincess->nCntFrame = 0;
							pPrincess->nCntKeyPrincess = 0;
							g_wave.state = WAVE_STATE_ATTACK;
							PlaySound(SOUND_LABEL_SE_MAGIC);
						}
					break;

				case WAVE_STATE_ATTACK:
					g_wave.col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
					g_wave.fRadius += 0.1f;
					if (g_wave.fRadius > WAVE_RADIUS_MAX)
					{
						g_wave.state = WAVE_STATE_CHARGE;
						pPrincess->state = PRINCESSSTATE_NEUTRAL;
						g_wave.nCharge = 0;
						g_wave.fRadius = WAVE_RADIUS;
					}
					break;
				default:
					break;
			}
			if (nCntY > 0)
			{
				g_wave.col.a = 0.0f;
			}

			if (*GetGameState() == GAMESTATE_GAMEOVER)
			{
				g_wave.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			}

			pVtx[0].col = g_wave.col;
			g_wave.rot.y += 0.0005f;
		}
	}

	//���_�f�[�^���A�����b�N
	g_pVtxBuffWave->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawWave(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X


	//�������J�����O���Ȃ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// �����_�[�X�e�[�g(���Z��������)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_wave.mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_wave.rot.y, g_wave.rot.x, g_wave.rot.z);

	D3DXMatrixMultiply(&g_wave.mtxWorld, &g_wave.mtxWorld, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_wave.pos.x, g_wave.pos.y, g_wave.pos.z);

	D3DXMatrixMultiply(&g_wave.mtxWorld, &g_wave.mtxWorld, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_wave.mtxWorld);

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffWave, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffWave);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureWave);

	if (g_wave.bUse)
	{
		//�|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			0, 0,
			g_wave.nNumVertex,		//���_��
			0,
			g_wave.nNumPolygon);	//�`�悷��v���~�e�B�u��
	}
	// ����(�����)���J�����O����
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//�ʏ탂�[�h�ɖ߂�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// �����_�[�X�e�[�g(�ʏ�u�����h����)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexWave(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_wave.nNumVertex,	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
								D3DUSAGE_WRITEONLY,		//�Œ�
								FVF_VERTEX_3D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,		//�Œ�
								&g_pVtxBuffWave,
								NULL);

	//�C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_wave.nNumIndex ,	//�m�ۂ���o�b�t�@�̃T�C�Y(�T�C�Y*�K�v�ȃC���f�b�N�X��)
								D3DUSAGE_WRITEONLY,
								D3DFMT_INDEX16,		//�������16�r�b�g
								D3DPOOL_MANAGED,
								&g_pIdxBuffWave,
								NULL);

	VERTEX_3D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffWave->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I
		//���_�̐ݒ�
		for (int nCntY = 0; nCntY <= WAVE_Y; nCntY++)
		{
			for (int nCntXZ = 0; nCntXZ <= WAVE_XZ; nCntXZ++, pVtx++)
			{
				pVtx[0].pos = D3DXVECTOR3(sinf((2 * D3DX_PI / WAVE_XZ) * nCntXZ) * g_wave.fRadius,	//x
					g_wave.fHeight* nCntY,															//y
					cosf((2 * D3DX_PI / WAVE_XZ) * nCntXZ) * g_wave.fRadius);						//z

				pVtx[0].nor = pVtx[0].pos;
				D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);
				if (nCntY > 0)
				{
					g_wave.col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.0f);
				}
				else
				{
					g_wave.col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
				}
				pVtx[0].col = g_wave.col;
				pVtx[0].tex = D3DXVECTOR2((1.0f / WAVE_XZ) * nCntXZ, (1.0f / WAVE_Y) * nCntY);
			}
		}
	//���_�f�[�^���A�����b�N
	g_pVtxBuffWave->Unlock();

	WORD *pIdx;		//�C���f�b�N�X�f�[�^�ւ̃|�C���^
	
	//�C���f�b�N�X�f�[�^�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffWave->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntIdxY = 0; nCntIdxY < WAVE_Y; nCntIdxY++)
		{
			//�ʏ�C���f�b�N�X
			for (int nCntIdxXZ = 0; nCntIdxXZ <= WAVE_XZ; nCntIdxXZ++)
			{
				pIdx[0] = nCntIdxXZ + ((WAVE_XZ + 1) * nCntIdxY);	//���_�̔ԍ�(�C���f�b�N�X)����
				pIdx[1] = (WAVE_XZ + 1) + nCntIdxXZ + ((WAVE_XZ + 1) * nCntIdxY);

				pIdx += 2;
			}
		}
	//���_�f�[�^���A�����b�N
	g_pIdxBuffWave->Unlock();
}

//=============================================================================
// �`���[�W���Z�b�g
//=============================================================================
void DeleteCharge(void)
{
	g_wave.nCharge = 0;
	g_wave.state = WAVE_STATE_CHARGE;
}

//=============================================================================
// �����蔻��
//=============================================================================
bool CollisionWave(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove,float Radius)
{
	bool bLand = false;
	float fDistance = (pPos->x - g_wave.pos.x)*(pPos->x - g_wave.pos.x) + (pPos->z - g_wave.pos.z)*(pPos->z - g_wave.pos.z);
	float fLength = (Radius + g_wave.fRadius)*(Radius + g_wave.fRadius);
	float fDiff = (Radius + g_wave.fRadius) - (float)sqrt(fDistance);	//��
	D3DXVECTOR3 vec = D3DXVECTOR3((pPos->x - g_wave.pos.x), 0.0f, (pPos->z - g_wave.pos.z));	//�x�N�g��
	D3DXVec3Normalize(&vec, &vec);

	if (fDistance < fLength)
	{
		bLand = true;

		pPos->x = pPos->x + vec.x * fDiff;
		pPos->z = pPos->z + vec.z * fDiff;
	}

	return bLand;
}

//=============================================================================
// ���̎擾
//=============================================================================
WAVE *GetWave(void)
{
	return &g_wave;
}

//=============================================================================
// ���@�̐ݒ�
//=============================================================================
void SetWave(D3DXVECTOR3 pos)
{
	g_wave.pos = pos;
	VERTEX_3D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffWave->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I
													//���_�̐ݒ�
	for (int nCntY = 0; nCntY <= WAVE_Y; nCntY++)
	{
		for (int nCntXZ = 0; nCntXZ <= WAVE_XZ; nCntXZ++, pVtx++)
		{
			pVtx[0].pos = D3DXVECTOR3(sinf((2 * D3DX_PI / WAVE_XZ) * nCntXZ) * g_wave.fRadius,	//x
				g_wave.fHeight* nCntY,															//y
				cosf((2 * D3DX_PI / WAVE_XZ) * nCntXZ) * g_wave.fRadius);						//z

			pVtx[0].nor = pVtx[0].pos;
			D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);
			if (nCntY > 0)
			{
				g_wave.col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.0f);
			}
			else
			{
				g_wave.col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
			}
			pVtx[0].col = g_wave.col;
			pVtx[0].tex = D3DXVECTOR2((1.0f / WAVE_XZ) * nCntXZ, (1.0f / WAVE_Y) * nCntY);
		}
	}
	//���_�f�[�^���A�����b�N
	g_pVtxBuffWave->Unlock();

	g_wave.bUse = true;
}
