//=============================================================================
//
// ��Q������ [obstacle.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "obstacle.h"
#include "shadow.h"
#include "line.h"
#include "sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define OBSTACLE_FILE			"data/MODEL/Rubble2.x"					//�ǂݍ��ރt�@�C����
#define OBSTACLE_TEXTURENAME	"data/TEXTURE/stone_00037.jpg"			//�ǂݍ��ރe�N�X�`����

#define OBSTACLE_TEXTURE	(2)		//�e�N�X�`����
#define OBSTACLE_LIFE		(2)		//�e�N�X�`����

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPD3DXMESH				g_pMeshObstacle = NULL;		//���b�V�����̃|�C���^
LPD3DXBUFFER			g_pBuffMatObstacle = NULL;		//�}�e���A�����ւ̃|�C���^
DWORD					g_nNumMatObstacle = 0;			//�}�e���A�����̐�
LPDIRECT3DTEXTURE9		g_pTextureObstacle[OBSTACLE_TEXTURE] = {};	//�e�N�X�`���̃|�C���^

OBSTACLE g_aObstacle[OBSTACLE_MAX];

//=============================================================================
// ����������
//=============================================================================
void InitObstacle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �ʒu�E�����̏����ݒ�

	for (int nCntbasin = 0; nCntbasin < OBSTACLE_MAX; nCntbasin++)
	{
			g_aObstacle[nCntbasin].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aObstacle[nCntbasin].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aObstacle[nCntbasin].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aObstacle[nCntbasin].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aObstacle[nCntbasin].nLife = OBSTACLE_LIFE;
			g_aObstacle[nCntbasin].nCntState = 0;
			g_aObstacle[nCntbasin].bUse = false;
			g_aObstacle[nCntbasin].bHit = false;

	}

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(OBSTACLE_FILE,	//�ǂݍ��ރt�@�C����
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatObstacle,
		NULL,
		&g_nNumMatObstacle,
		&g_pMeshObstacle);

	//�e�N�X�`���̓ǂݍ���
	for (int nCnt = 0; nCnt < OBSTACLE_TEXTURE; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice,
			OBSTACLE_TEXTURENAME,
			&g_pTextureObstacle[nCnt]);
	}

	//���f���̒��_���W�̍ő�l�E�ŏ��l�̐ݒ�
	int		nNumVertices;	//���_��
	DWORD	sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE	*pVertexBuffer;	//���_�o�b�t�@�ւ̃|�C���^

	//������
	for (int nCntbasin = 0; nCntbasin < OBSTACLE_MAX; nCntbasin++)
	{
		g_aObstacle[nCntbasin].vtxMin = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
		g_aObstacle[nCntbasin].vtxMax = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);

		//���_�����擾
		nNumVertices = g_pMeshObstacle->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_pMeshObstacle->GetFVF());

		//���_�o�b�t�@�̃��b�N
		g_pMeshObstacle->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

		//���_�̍ő�E�ŏ��l�̎擾
		for (int nCntVtx = 0; nCntVtx < nNumVertices; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;
			//���ׂĂ̒��_�����r���čŏ��l�E�ő�l�𔲂��o��
			//x�̍ŏ��ƍő�
			if (g_aObstacle[nCntbasin].vtxMin.x > vtx.x)
			{
				g_aObstacle[nCntbasin].vtxMin.x = vtx.x;
			}
			if (g_aObstacle[nCntbasin].vtxMax.x < vtx.x)
			{
				g_aObstacle[nCntbasin].vtxMax.x = vtx.x;
			}
			//y�̍ŏ��ƍő�
			if (g_aObstacle[nCntbasin].vtxMin.y > vtx.y)
			{
				g_aObstacle[nCntbasin].vtxMin.y = vtx.y;
			}
			if (g_aObstacle[nCntbasin].vtxMax.y < vtx.y)
			{
				g_aObstacle[nCntbasin].vtxMax.y = vtx.y;
			}

			//z�̍ŏ��ƍő�
			if (g_aObstacle[nCntbasin].vtxMin.z > vtx.z)
			{
				g_aObstacle[nCntbasin].vtxMin.z = vtx.z;
			}
			if (g_aObstacle[nCntbasin].vtxMax.z < vtx.z)
			{
				g_aObstacle[nCntbasin].vtxMax.z = vtx.z;
			}
			pVertexBuffer += sizeFVF;	//�T�C�Y���|�C���^��i�߂�
		}

		//���_�o�b�t�@�̃A�����b�N
		g_pMeshObstacle->UnlockVertexBuffer();
	}

}

//=============================================================================
// �I������
//=============================================================================
void UninitObstacle(void)
{
	// ���b�V���̊J��
	if (g_pMeshObstacle != NULL)
	{
		g_pMeshObstacle->Release();
		g_pMeshObstacle = NULL;
	}

	// �}�e���A���̊J��
	if (g_pBuffMatObstacle != NULL)
	{
		g_pBuffMatObstacle->Release();
		g_pBuffMatObstacle = NULL;
	}

	//�e�N�X�`���̊J��
	for (int nCntTex = 0; nCntTex < OBSTACLE_TEXTURE; nCntTex++)
	{
		if (g_pTextureObstacle[nCntTex] != NULL)
		{
			g_pTextureObstacle[nCntTex]->Release();
			g_pTextureObstacle[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateObstacle(void)
{
	for (int nCntObstacle = 0; nCntObstacle < OBSTACLE_MAX; nCntObstacle++)
	{
		if (g_aObstacle[nCntObstacle].bHit)
		{
			g_aObstacle[nCntObstacle].nCntState++;
			if (g_aObstacle[nCntObstacle].nCntState % 10 == 0)
			{
				g_aObstacle[nCntObstacle].bHit = false;
			}
		}
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawObstacle(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	D3DXMATRIX		mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATERIAL	*pMat;				//���݂̃}�e���A���ۑ��p
	D3DMATERIAL9	matDef;				//�}�e���A���f�[�^�ւ̃|�C���^
	D3DMATERIAL9	matKeep;	//�ۑ��p

	for (int nCntbasin = 0; nCntbasin < OBSTACLE_MAX; nCntbasin++)
	{
		if (g_aObstacle[nCntbasin].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aObstacle[nCntbasin].mtxWorldObstacle);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aObstacle[nCntbasin].rot.y, g_aObstacle[nCntbasin].rot.x, g_aObstacle[nCntbasin].rot.z);

			D3DXMatrixMultiply(&g_aObstacle[nCntbasin].mtxWorldObstacle, &g_aObstacle[nCntbasin].mtxWorldObstacle, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aObstacle[nCntbasin].pos.x, g_aObstacle[nCntbasin].pos.y, g_aObstacle[nCntbasin].pos.z);

			D3DXMatrixMultiply(&g_aObstacle[nCntbasin].mtxWorldObstacle, &g_aObstacle[nCntbasin].mtxWorldObstacle, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aObstacle[nCntbasin].mtxWorldObstacle);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_pBuffMatObstacle->GetBufferPointer();

			// �e�N�X�`���̐ݒ������Ȃ炱��(1���̎�)

			for (int nCntMat = 0; nCntMat < (int)g_nNumMatObstacle; nCntMat++)
			{
				// �e�N�X�`���̐ݒ������Ȃ炱��(�������̎�)
				pDevice->SetTexture(0, g_pTextureObstacle[nCntMat]);

				matKeep = pMat[nCntMat].MatD3D;	//���̕ۑ�

				if (g_aObstacle[nCntbasin].bHit)
				{
					pMat[nCntMat].MatD3D.Diffuse.r = 255;	//�ԐF�ɕς���
				}
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				pMat[nCntMat].MatD3D = matKeep;	//�F��߂�

				// �`��
				g_pMeshObstacle->DrawSubset(nCntMat);	//�i�q���
			}
			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// ��Q���̐ݒ�
//=============================================================================
void SetObstacle(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntbasin = 0; nCntbasin < OBSTACLE_MAX; nCntbasin++)
	{
		if (!g_aObstacle[nCntbasin].bUse)
		{
			g_aObstacle[nCntbasin].pos = pos;
			g_aObstacle[nCntbasin].rot = rot;
			g_aObstacle[nCntbasin].bUse = true;

#ifdef _DEBUG
			//���C���̕`��
			SetLine(g_aObstacle[nCntbasin].pos,	D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMax.x, g_aObstacle[nCntbasin].vtxMin.y, g_aObstacle[nCntbasin].vtxMax.z), D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMax.x, g_aObstacle[nCntbasin].vtxMax.y, g_aObstacle[nCntbasin].vtxMax.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			SetLine(g_aObstacle[nCntbasin].pos, D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMin.x, g_aObstacle[nCntbasin].vtxMin.y, g_aObstacle[nCntbasin].vtxMin.z), D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMin.x, g_aObstacle[nCntbasin].vtxMax.y, g_aObstacle[nCntbasin].vtxMin.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			SetLine(g_aObstacle[nCntbasin].pos, D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMin.x, g_aObstacle[nCntbasin].vtxMin.y, g_aObstacle[nCntbasin].vtxMax.z), D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMin.x, g_aObstacle[nCntbasin].vtxMax.y, g_aObstacle[nCntbasin].vtxMax.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			SetLine(g_aObstacle[nCntbasin].pos, D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMax.x, g_aObstacle[nCntbasin].vtxMin.y, g_aObstacle[nCntbasin].vtxMin.z), D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMax.x, g_aObstacle[nCntbasin].vtxMax.y, g_aObstacle[nCntbasin].vtxMin.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

			SetLine(g_aObstacle[nCntbasin].pos, D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMax.x, g_aObstacle[nCntbasin].vtxMax.y, g_aObstacle[nCntbasin].vtxMax.z), D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMax.x, g_aObstacle[nCntbasin].vtxMax.y, g_aObstacle[nCntbasin].vtxMin.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			SetLine(g_aObstacle[nCntbasin].pos, D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMin.x, g_aObstacle[nCntbasin].vtxMax.y, g_aObstacle[nCntbasin].vtxMin.z), D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMin.x, g_aObstacle[nCntbasin].vtxMax.y, g_aObstacle[nCntbasin].vtxMax.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			SetLine(g_aObstacle[nCntbasin].pos, D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMin.x, g_aObstacle[nCntbasin].vtxMax.y, g_aObstacle[nCntbasin].vtxMax.z), D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMax.x, g_aObstacle[nCntbasin].vtxMax.y, g_aObstacle[nCntbasin].vtxMax.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			SetLine(g_aObstacle[nCntbasin].pos, D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMax.x, g_aObstacle[nCntbasin].vtxMax.y, g_aObstacle[nCntbasin].vtxMin.z), D3DXVECTOR3(g_aObstacle[nCntbasin].vtxMin.x, g_aObstacle[nCntbasin].vtxMax.y, g_aObstacle[nCntbasin].vtxMin.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif
			break;
		}
	}
}

//=============================================================================
// ���̎擾
//=============================================================================
OBSTACLE *GetObstacle(void)
{
	return &g_aObstacle[0];
}

//=============================================================================
// �ϋv�͂̌���
//=============================================================================
void DeleteObstacle(int Damage,int nCntObstacle)
{
	g_aObstacle[nCntObstacle].nLife -= Damage;
	g_aObstacle[nCntObstacle].bHit = true;

	if (g_aObstacle[nCntObstacle].nLife <= 0)
	{
		PlaySound(SOUND_LABEL_SE_BREAK);
		g_aObstacle[nCntObstacle].bUse = false;
		g_aObstacle[nCntObstacle].bHit = false;
	}
}

//=============================================================================
// ��Q���̑S����
//=============================================================================
void DeleteALLObstacle(void)
{
	for (int nCntbasin = 0; nCntbasin < OBSTACLE_MAX; nCntbasin++)
	{
		if (g_aObstacle[nCntbasin].bUse)
		{
			g_aObstacle[nCntbasin].bUse = false;
			g_aObstacle[nCntbasin].bHit = false;
			g_aObstacle[nCntbasin].nLife = OBSTACLE_LIFE;

		}
	}
	DeleteLine();
}

//=============================================================================
// �����蔻��
//=============================================================================
bool CollisionObstacle(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3*pMove, float Radius,int nCntbasin)
{
	bool bLand = false;
	//for (int nCntbasin = 0; nCntbasin < OBSTACLE_MAX; nCntbasin++)
	//{
		if (g_aObstacle[nCntbasin].bUse)
		{
			if (pPos->x + Radius >= g_aObstacle[nCntbasin].pos.x + g_aObstacle[nCntbasin].vtxMin.x &&
				pPos->x - Radius <= g_aObstacle[nCntbasin].pos.x + g_aObstacle[nCntbasin].vtxMax.x)
			{
				if (pPosOld->z - Radius >= g_aObstacle[nCntbasin].pos.z + g_aObstacle[nCntbasin].vtxMax.z &&
					pPos->z - Radius <= g_aObstacle[nCntbasin].pos.z + g_aObstacle[nCntbasin].vtxMax.z)
				{	//�����痈����
					bLand = true;
					pPos->z = g_aObstacle[nCntbasin].pos.z + g_aObstacle[nCntbasin].vtxMax.z + Radius;
					//pMove->z = 0;
				}
				else if (pPosOld->z + Radius <= g_aObstacle[nCntbasin].pos.z + g_aObstacle[nCntbasin].vtxMin.z &&
					pPos->z + Radius >= g_aObstacle[nCntbasin].pos.z + g_aObstacle[nCntbasin].vtxMin.z)
				{	//��O���痈����
					bLand = true;
					pPos->z = g_aObstacle[nCntbasin].pos.z + g_aObstacle[nCntbasin].vtxMin.z - Radius;
					//pMove->z = 0;
				}
			}
			if (pPos->z + Radius >= g_aObstacle[nCntbasin].pos.z + g_aObstacle[nCntbasin].vtxMin.z &&
				pPos->z - Radius <= g_aObstacle[nCntbasin].pos.z + g_aObstacle[nCntbasin].vtxMax.z)
			{
				if (pPosOld->x - Radius >= g_aObstacle[nCntbasin].pos.x + g_aObstacle[nCntbasin].vtxMax.x &&
					pPos->x - Radius <= g_aObstacle[nCntbasin].pos.x + g_aObstacle[nCntbasin].vtxMax.x)
				{	//�E���痈����
					bLand = true;
					pPos->x = g_aObstacle[nCntbasin].pos.x + g_aObstacle[nCntbasin].vtxMax.x + Radius;
					//pMove->x = 0;
				}
				else if (pPosOld->x + Radius <= g_aObstacle[nCntbasin].pos.x + g_aObstacle[nCntbasin].vtxMin.x &&
					pPos->x + Radius >= g_aObstacle[nCntbasin].pos.x + g_aObstacle[nCntbasin].vtxMin.x)
				{	//�����痈����
					bLand = true;
					pPos->x = g_aObstacle[nCntbasin].pos.x + g_aObstacle[nCntbasin].vtxMin.x - Radius;
					//pMove->x = 0;
				}
			}
		}
	//}
	return bLand;
}