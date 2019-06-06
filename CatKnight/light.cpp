//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "light.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define NUM_LIGHT	(3)		//���C�g�̐�

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
D3DLIGHT9 g_alight[NUM_LIGHT];

//=============================================================================
// ���C�g�̏���������
//=============================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXVECTOR3 avecDir[NUM_LIGHT];				//�ݒ�p�����x�N�g��

	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		// ���C�g���N���A����
		ZeroMemory(&g_alight[nCntLight], sizeof(D3DLIGHT9));
		// ���C�g�̎�ނ�ݒ�
		g_alight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;
	}

	// ���C�g�̊g�U����ݒ�
	g_alight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_alight[0].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_alight[1].Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	g_alight[1].Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	g_alight[2].Diffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	g_alight[2].Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

	// ���C�g�̕����̐ݒ�
	avecDir[0] = D3DXVECTOR3(0.20f, -0.80f, 0.40f);
	avecDir[1] = D3DXVECTOR3(-0.20f, 0.80f, -0.40f);
	avecDir[2] = D3DXVECTOR3(0.0f, -0.10f, -1.0f);

	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		D3DXVec3Normalize(&avecDir[nCntLight], &avecDir[nCntLight]);	//���K������
		g_alight[nCntLight].Direction = avecDir[nCntLight];

		// ���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &g_alight[nCntLight]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//=============================================================================
// ���C�g�̏I������
//=============================================================================
void UninitLight(void)
{
}

//=============================================================================
// ���C�g�̍X�V����
//=============================================================================
void UpdateLight(void)
{
}

