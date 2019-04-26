//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//=============================================================================
// �T�E���h�t�@�C��
//=============================================================================
typedef enum
{
	SOUND_LABEL_BGM_OP = 0,		// OP
	SOUND_LABEL_BGM_GAME,		// GAME
	SOUND_LABEL_BGM_CLEAR,		//�Q�[���N���A
	SOUND_LABEL_BGM_GAMEOVER,	//�Q�[���I�[�o�[
	SOUND_LABEL_SE_SWORD,		//��
	SOUND_LABEL_SE_NAIL,		//�Ђ�����
	SOUND_LABEL_SE_DAMAGE,		//��_��
	SOUND_LABEL_SE_CHARGE,		//�`���[�W
	SOUND_LABEL_SE_CHARGEMAX,	//�`���[�W�I��
	SOUND_LABEL_SE_MAGIC,		//���@
	SOUND_LABEL_SE_BREAK,		//�ӂ��鉹
	SOUND_LABEL_SE_CAT,			//�ɂ�[��
	SOUND_LABEL_SE_PAUSE,		// �|�[�Y��
	SOUND_LABEL_SE_ENTER,		// ���艹
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitSound(HWND hWnd);
void UninitSound(void);

HRESULT PlaySound(SOUND_LABEL label);	//�ǂ̉���炷�H
void StopSound(SOUND_LABEL label);		//�ǂ̉����~�߂�H
void StopSound(void);					//�S���̉����~�߂�

#endif
