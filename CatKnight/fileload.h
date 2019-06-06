//=============================================================================
//
// �t�@�C�����[�h���� [fileload.h]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#ifndef _FILELOAD_H_
#define _FILELOAD_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_CHARA	(256)
#define MAX_PARTS	(10)
#define MAX_KEY		(4)
#define MAX_MOTION	(10)

//=============================================================================
// �L�����̎��
//=============================================================================
typedef enum
{
	CHARA_NONE = -1,
	CHARA_PLAYER,
	CHARA_PRINCESS,
	CHARA_ENEMY,
	CHARA_BOSS,
	CHARA_MAX
}CHARA_TYPE;

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
	KEY	Key[MAX_PARTS];		//�L�[�v�f[�p�[�c��]
} KEY_INFO;

//���[�V�������
typedef struct
{
	bool		bLoop;		//���[�v�̗L��
	int			nNumKey;	//�L�[��
	KEY_INFO	KeyInfo[MAX_KEY];	//�L�[���[�L�[�̐�]
} MOTION_INFO;

//=============================================================================
// �\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos[MAX_PARTS];						//�ʒu[�p�[�c��]
	D3DXVECTOR3	rot[MAX_PARTS];						//�p�x[�p�[�c��]
	char		cFileNeme[MAX_PARTS][MAX_CHARA];	//���f���t�@�C����[�p�[�c��]
	int			nParent[MAX_PARTS];					//�e�̔ԍ�[�p�[�c��]
	int			nNumModel = 0;						//�ǂݍ��ރ��f����
	int			nNumParts = 0;						//�ǂݍ��ރp�[�c��
	float		fSpeed;								//���x
	float		fRadius;							//���a
	int			nAttack;							//�U����
	int			nMaxLife;							//�ő�̗�
	D3DXVECTOR3	Collpos;							//�ʒu(�����蔻��)
	int			nCollParent;						//�e�̔ԍ�(�����蔻��)
	float		fCollRadius;						//���a(�����蔻��)
	int			nStart;								//�J�n����t���[��
	int			nEnd;								//�I������t���[��
	D3DXVECTOR3	Meshpos[2];							//�ʒu(�O��)
	int			nMeshParent;						//�e�̔ԍ�(�O��)
	MOTION_INFO MotionInfo[MAX_MOTION];				//���[�V�������[���[�V������]
}LOAD;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void LoadFile(char *FileName, int nCharaType);	//�t�@�C���̓ǂݍ���
void LoadLine(FILE *pFile,  char *cLine, char *cStr);	//��s�ǂݍ���
void CutTub(char *cStr);	//tab����

void LoadChara(FILE *pFile, char *cLine, char *cStr, int nCharaType);	//�L�������
void LoadParts(FILE *pFile, char *cLine, char *cStr, int nCharaType);	//�p�[�c���
void LoadMotion(FILE *pFile, char *cLine, char *cStr, int nCntMotion, int nCharaType);	//���[�V�������
void LoadKey(FILE *pFile, char *cLine, char *cStr, int nCntMotion, int nCntKey, int nCntParts, int nCharaType);	//�L�[���
void LoadXYZ(D3DXVECTOR3 *xyz, char *cStr, char *cPunctuation);	//XYZ���

LOAD *GetLoad(int nCntType);

#endif