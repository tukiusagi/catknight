//=============================================================================
//
// ���@�U������ [wave.h]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#ifndef _WAVE_H_
#define _WAVE_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define WAVE_RADIUS_MAX		(150.0f)	//�L����ő�l
#define WAVE_SPREAD_TIME	(1*60)	//�L���鑬��

//=============================================================================
// ���@�̏��
//=============================================================================
typedef enum
{
	WAVE_STATE_CHARGE,	//�`���[�W��
	WAVE_STATE_CHARGEMAX,	//�`���[�W�I��
	WAVE_STATE_ATTACK,	//�U��
	WAVE_STATE_MAX		//���̍ő吔
} WAVE_STATE;

//=============================================================================
//�\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;			//�ʒu
	D3DXVECTOR3	rot;			//����
	D3DXVECTOR3	move;			//�ړ�
	D3DXMATRIX	mtxWorld;		//���[���h�}�g���b�N�X
	WAVE_STATE	state;			//���
	D3DXCOLOR	col;			//�F
	int			nNumVertex;		//�����_��
	int			nNumIndex;		//���C���f�b�N�X��
	int			nNumPolygon;	//���|���S����
	int			nCharge;		//�`���[�W
	int			nInterval;		//�C���^�[�o��
	float		fRadius;		//���a
	float		fHeight;		//����
	bool		bUse;			//�g�p���
}WAVE;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitWave(void);
void UninitWave(void);
void UpdateWave(void);
void DrawWave(void);

void DeleteCharge(void);
bool CollisionWave(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float Radius);
WAVE *GetWave(void);
void SetWave(D3DXVECTOR3 pos);
void DeleteWave(void);

#endif
