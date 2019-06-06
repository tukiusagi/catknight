//=============================================================================
//
// X�C���v�b�g���� [xController.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include "xController.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	MAX_CONTROLLER	(4)		// �R���g���[���[�̍ő吔
#define	NUM_BUTTON_MAX	(128)	// �{�^���̍ő吔
#define THRESHOLD		(65535/4) //�������l

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
BYTE g_aXinputState[NUM_BUTTON_MAX];			//�L�[�{�[�h���͏�񃏁[�N
BYTE g_aXinputStateTrigger[NUM_BUTTON_MAX];		//�L�[�{�[�h���̓g���K�[
BYTE g_aXPad[XINPUT_PAD_MAX];
typedef struct  
{
	XINPUT_STATE state;
	bool bConnected;
}CONTROLER_STATE;

CONTROLER_STATE g_xController[MAX_CONTROLLER];

//=============================================================================
// �W���C�X�e�B�b�N�̏�����
//=============================================================================
void InitxController()
{
	
}

//=============================================================================
// �W���C�X�e�B�b�N�̏I������
//=============================================================================
void UninitxController(void)
{
}

//=============================================================================
// �W���C�X�e�B�b�N�̍X�V����
//=============================================================================
void UpdatexController(void)
{
	GetxController();

	for (DWORD nCnt = 0; nCnt < MAX_CONTROLLER; nCnt++)
	{
		if (g_xController[nCnt].bConnected)
		{
			WORD Buttons = g_xController[nCnt].state.Gamepad.wButtons;

			//�f�b�h�]�[���ݒ�
			//�A�i���O�����L�[
			if ((g_xController[nCnt].state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				g_xController[nCnt].state.Gamepad.sThumbLX	> -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
				(g_xController[nCnt].state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				g_xController[nCnt].state.Gamepad.sThumbLY	> -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
			{
				g_xController[nCnt].state.Gamepad.sThumbLX = 0;
				g_xController[nCnt].state.Gamepad.sThumbLY = 0;
			}

			if ((g_xController[nCnt].state.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				g_xController[nCnt].state.Gamepad.sThumbRX	> -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
				(g_xController[nCnt].state.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
					g_xController[nCnt].state.Gamepad.sThumbRY	> -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
			{
				g_xController[nCnt].state.Gamepad.sThumbRX = 0;
				g_xController[nCnt].state.Gamepad.sThumbRY = 0;
			}
			
			//���͏��
			//�{�^��
			if (Buttons & XINPUT_GAMEPAD_X)
			{
				g_aXPad[XINPUT_PAD_X] = true;
			}
			else
			{
				g_aXPad[XINPUT_PAD_X] = false;
			}
			if (Buttons & XINPUT_GAMEPAD_Y)
			{
				g_aXPad[XINPUT_PAD_Y] = true;
			}
			else
			{
				g_aXPad[XINPUT_PAD_Y] = false;
			}
			if (Buttons & XINPUT_GAMEPAD_A)
			{
				g_aXPad[XINPUT_PAD_A] = true;
			}
			else
			{
				g_aXPad[XINPUT_PAD_A] = false;
			}
			if (Buttons & XINPUT_GAMEPAD_B)
			{
				g_aXPad[XINPUT_PAD_B] = true;
			}
			else
			{
				g_aXPad[XINPUT_PAD_B] = false;
			}
			if (Buttons & XINPUT_GAMEPAD_LEFT_SHOULDER)
			{
				g_aXPad[XINPUT_PAD_L] = true;
			}
			else
			{
				g_aXPad[XINPUT_PAD_L] = false;
			}
			if (Buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
			{
				g_aXPad[XINPUT_PAD_R] = true;
			}
			else
			{
				g_aXPad[XINPUT_PAD_R] = false;
			}
			if (g_xController[nCnt].state.Gamepad.bLeftTrigger)
			{
				g_aXPad[XINPUT_PAD_ZL] = true;
			}
			else
			{
				g_aXPad[XINPUT_PAD_ZL] = false;
			}
			if (g_xController[nCnt].state.Gamepad.bRightTrigger)
			{
				g_aXPad[XINPUT_PAD_ZR] = true;
			}
			else
			{
				g_aXPad[XINPUT_PAD_ZR] = false;
			}

			if (Buttons & XINPUT_GAMEPAD_LEFT_THUMB)
			{
				g_aXPad[XINPUT_PAD_LSTICKBUTTON] = true;
			}
			if (Buttons & XINPUT_GAMEPAD_RIGHT_THUMB)
			{
				g_aXPad[XINPUT_PAD_RSTICKBUTTON] = true;
			}
			if (Buttons & XINPUT_GAMEPAD_BACK)
			{
				g_aXPad[XINPUT_PAD_BACK] = true;
			}
			if (Buttons & XINPUT_GAMEPAD_START)
			{
				g_aXPad[XINPUT_PAD_START] = true;
			}
			else
			{
				g_aXPad[XINPUT_PAD_START] = false;
			}

			//���X�e�B�b�N
			//���E
			if (g_xController[nCnt].state.Gamepad.sThumbLX > THRESHOLD)
			{
				g_aXPad[XINPUT_PAD_RIGHT_L] = true;
			}
			else
			{
				g_aXPad[XINPUT_PAD_RIGHT_L] = false;
			}
			if (g_xController[nCnt].state.Gamepad.sThumbLX < -THRESHOLD)
			{
				g_aXPad[XINPUT_PAD_LEFT_L] = true;
			}
			else
			{
				g_aXPad[XINPUT_PAD_LEFT_L] = false;
			}
			//�㉺
			if (g_xController[nCnt].state.Gamepad.sThumbLY > THRESHOLD)
			{
				g_aXPad[XINPUT_PAD_UP_L] = true;
			}
			else
			{
				g_aXPad[XINPUT_PAD_UP_L] = false;
			}

			if (g_xController[nCnt].state.Gamepad.sThumbLY < -THRESHOLD)
			{
				g_aXPad[XINPUT_PAD_DOWN_L] = true;
			}
			else
			{
				g_aXPad[XINPUT_PAD_DOWN_L] = false;
			}

			//�E�X�e�B�b�N
			//���E
			if (g_xController[nCnt].state.Gamepad.sThumbRX > THRESHOLD)
			{
				g_aXPad[XINPUT_PAD_RIGHT_R] = true;
			}
			else
			{
				g_aXPad[XINPUT_PAD_RIGHT_R] = false;
			}
			if (g_xController[nCnt].state.Gamepad.sThumbRX < -THRESHOLD)
			{
				g_aXPad[XINPUT_PAD_LEFT_R] = true;
			}
			else
			{
				g_aXPad[XINPUT_PAD_LEFT_R] = false;
			}
			//�㉺
			if (g_xController[nCnt].state.Gamepad.sThumbRY > THRESHOLD)
			{
				g_aXPad[XINPUT_PAD_UP_R] = true;
			}
			else
			{
				g_aXPad[XINPUT_PAD_UP_R] = false;
			}

			if (g_xController[nCnt].state.Gamepad.sThumbRY < -THRESHOLD)
			{
				g_aXPad[XINPUT_PAD_DOWN_R] = true;
			}
			else
			{
				g_aXPad[XINPUT_PAD_DOWN_R] = false;
			}

		}
	}

	for (int nCntButton = 0; nCntButton < XINPUT_PAD_MAX; nCntButton++)
	{
		g_aXinputStateTrigger[nCntButton] = (g_aXinputState[nCntButton] ^ g_aXPad[nCntButton]) & g_aXPad[nCntButton];	//�L�[�g���K�[���ۑ�
		g_aXinputState[nCntButton] = g_aXPad[nCntButton];
	}
}

//=============================================================================
// �{�^���̃v���X��Ԃ��擾
//=============================================================================
bool GetxControllerPress(int nKey)
{
	return (g_aXPad[nKey]) ? true : false;	//�O�����Z�q
}

//=============================================================================
// �g���K�[����
//=============================================================================
bool GetxControllerTrigger(int nKey)
{
	return (g_aXinputStateTrigger[nKey]) ? true : false;	//�O�����Z�q
}

//=============================================================================
// �R���g���[���[�̎擾
//=============================================================================
HRESULT GetxController()
{
	DWORD dwResult;

	//�ڑ�����Ă邩���ׂ�
	for (DWORD nCnt = 0; nCnt < MAX_CONTROLLER; nCnt++)
	{
		dwResult = XInputGetState(nCnt, &g_xController[nCnt].state);

		if (dwResult == ERROR_SUCCESS)
		{
			g_xController[nCnt].bConnected = true;
		}
		else
		{
			g_xController[nCnt].bConnected = false;
		}
	}
	return S_OK;

}