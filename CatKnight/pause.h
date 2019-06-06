//=============================================================================
//
// �|�[�Y����[pause.h]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include"main.h"

//=============================================================================
// �|�[�Y�̏�� 
//=============================================================================
typedef enum
{
	PAUSESTATE_NONE = 0,
	PAUSESTATE_CONTINUE,	//��������
	PAUSESTATE_RETRY,		//�ŏ�����
	PAUSESTATE_QUIT,		//�^�C�g���ɖ߂�
	PAUSESTATE_MAX
}PAUSESTATE;

typedef enum
{
	PAUSE_COLORSTATE_NONE = 0,
	PAUSE_COLORSTATE_CONTINUE,	//��������
	PAUSE_COLORSTATE_RETRY,		//�ŏ�����
	PAUSE_COLORSTATE_QUIT,		//�^�C�g���ɖ߂�
	PAUSE_COLORSTATE_MAX
}PAUSE_COLORSTATE;

//=============================================================================
// �v���g�^�C�v�錾(�֐��̌^�����m�F)
//=============================================================================
void InitPause(void);		//������
void UninitPause(void);		//�I������
void UpdatePause(void);		//�X�V����
void DrawPause(void);		//�`�揈��

#endif // !_Pause_H_