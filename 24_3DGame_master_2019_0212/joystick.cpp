//=============================================================================
//
// �Q�[���p�b�h���͏��� [joystick.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include "joystick.h"
#include"input.h"	

//=============================================================================
// �}�N����`
//=============================================================================
#define	NUM_BUTTON_MAX			(128)	// �{�^���̍ő吔

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECTINPUT8 g_pJoystick = NULL;				//DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevJoystick = NULL;		//���̓f�o�C�X�i�L�[�{�[�h�j�ւ̃|�C���^
DIDEVCAPS g_diDevCaps;
BYTE g_aButtonState[NUM_BUTTON_MAX];			//�L�[�{�[�h���͏�񃏁[�N
BYTE g_aButtonStateTrigger[NUM_BUTTON_MAX];		//�L�[�{�[�h���̓g���K�[
BYTE g_aPad[PAD_MAX];

//=============================================================================
// �W���C�X�e�B�b�N�̏�����
//=============================================================================
HRESULT InitJoystick(HINSTANCE hInstance, HWND hWnd)
{

	HRESULT hr;
	int nCntPad;

	for (nCntPad = 0; nCntPad < PAD_MAX; nCntPad++)
	{
		g_aPad[nCntPad] = false;
	}

	// DirectInput�I�u�W�F�N�g�̍쐬
	if (FAILED(hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pJoystick, NULL)))
	{
		return E_FAIL;
	}
	
	// ���̓f�o�C�X�̍쐬
	if (FAILED(hr = g_pJoystick->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}
	
	if (g_pDevJoystick == NULL)
	{
		return false;
	}
	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(hr = g_pDevJoystick->SetDataFormat(&c_dfDIJoystick2)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	if (FAILED(hr = g_pDevJoystick->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̔\�͂̎擾
	hr = g_pDevJoystick->GetCapabilities(&g_diDevCaps);

	//�\���L�[�͈͎̔w��
	hr = g_pDevJoystick->EnumObjects(EnumAxesCallback, (VOID*)hWnd, DIDFT_AXIS);

	hr = g_pDevJoystick->Poll();
	if (FAILED(hr))
	{
		hr = g_pDevJoystick->Acquire();
		while (hr==DIERR_INPUTLOST)
		{
			hr = g_pDevJoystick->Acquire();
		}
	}
	return S_OK;
}

//=============================================================================
// �W���C�X�e�B�b�N�̏I������
//=============================================================================
void UninitJoystick(void)
{
	// ���̓f�o�C�X(�W���C�X�e�B�b�N)�̊J��
	if (g_pDevJoystick != NULL)
	{
		g_pDevJoystick->Unacquire();
		g_pDevJoystick->Release();
		g_pDevJoystick = NULL;
	}
	
	// DirectInput�I�u�W�F�N�g�̊J��
	if (g_pJoystick != NULL)
	{
		g_pJoystick->Release();
		g_pJoystick = NULL;
	}
}

//=============================================================================
// �W���C�X�e�B�b�N�̍X�V����
//=============================================================================
BOOL UpdateJoystick(void)
{
	HRESULT	hr;
	DIJOYSTATE2	js;
	int nCntButton;

	if (NULL == g_pDevJoystick)
	{
		return FALSE;
	}

	hr = g_pDevJoystick->Poll();
	if (FAILED(hr))
	{
		hr = g_pDevJoystick->Acquire();
	}

	hr = g_pDevJoystick->GetDeviceState(sizeof(DIJOYSTATE2), &js);
	if (SUCCEEDED(hr))
	{
		//�{�^��
		if (js.rgbButtons[0] & 0x80)
		{
			g_aPad[PAD_X] = true;
		}
		else
		{
			g_aPad[PAD_X] = false;
		}
		if (js.rgbButtons[1] & 0x80)
		{
			g_aPad[PAD_Y] = true;
		}
		else
		{
			g_aPad[PAD_Y] = false;
		}
		 if (js.rgbButtons[2] & 0x80)
		{
			g_aPad[PAD_A] = true;
		}
		 else
		 {
			 g_aPad[PAD_A] = false;
		 }
		 if (js.rgbButtons[3] & 0x80)
		{
			g_aPad[PAD_B] = true;
		}
		 else
		 {
			 g_aPad[PAD_B] = false;
		 }
		 if (js.rgbButtons[4] & 0x80)
		{
			g_aPad[PAD_L] = true;
		}
		 else
		 {
			 g_aPad[PAD_L] = false;
		 }
		 if (js.rgbButtons[5] & 0x80)
		{
			g_aPad[PAD_R] = true;
		}
		 else
		 {
			 g_aPad[PAD_R] = false;
		 }
		 if (js.rgbButtons[6] & 0x80)
		{
			g_aPad[PAD_ZL] = true;
		}
		 else
		{
			 g_aPad[PAD_ZL] = false;
		}
		if (js.rgbButtons[7] & 0x80)
		{
			g_aPad[PAD_ZR] = true;
		}
		else
		{
			g_aPad[PAD_ZR] = false;
		}

		if (js.rgbButtons[8] & 0x80)
		{
			g_aPad[PAD_LSTICKBUTTON] = true;
		}
		if (js.rgbButtons[9] & 0x80)
		{
			g_aPad[PAD_RSTICKBUTTON] = true;
		}
		if (js.rgbButtons[10] & 0x80)
		{
			g_aPad[PAD_BACK] = true;
		}
		if (js.rgbButtons[11] & 0x80)
		{
			g_aPad[PAD_START] = true;
		}
		else
		{
			g_aPad[PAD_START] = false;
		}
		if (js.rgbButtons[12] & 0x80)
		{
			g_aPad[PAD_GUIDE] = true;
		}
		else
		{
			g_aPad[PAD_GUIDE] = false;
		}

		//���X�e�B�b�N
		//���E
		if (js.lX >= 800)
		{
			g_aPad[PAD_RIGHT_L] = true;
		}
		else
		{
			g_aPad[PAD_RIGHT_L] = false;
		}
		if (js.lX <= -800)
		{
			g_aPad[PAD_LEFT_L] = true;
		}
		else
		{
			g_aPad[PAD_LEFT_L] = false;
		}
		//�㉺
		if (js.lY <= -800)
		{
			g_aPad[PAD_UP_L] = true;
		}
		else
		{
			g_aPad[PAD_UP_L] = false;
		}

		if (js.lY >= 800)
		{
			g_aPad[PAD_DOWN_L] = true;
		}
		else
		{
			g_aPad[PAD_DOWN_L] = false;
		}
		
		//�E�X�e�B�b�N
		//���E
		if (js.lZ >= 800)
		{
			g_aPad[PAD_RIGHT_R] = true;
		}
		else
		{
			g_aPad[PAD_RIGHT_R] = false;
		}
		if (js.lZ <= -800)
		{
			g_aPad[PAD_LEFT_R] = true;
		}
		else
		{
			g_aPad[PAD_LEFT_R] = false;
		}
		//�㉺
		if (js.lRz <= -800)
		{
			g_aPad[PAD_UP_R] = true;
		}
		else
		{
			g_aPad[PAD_UP_R] = false;
		}

		if (js.lRz >= 800)
		{
			g_aPad[PAD_DOWN_R] = true;
		}
		else
		{
			g_aPad[PAD_DOWN_R] = false;
		}


		for (nCntButton = 0; nCntButton < PAD_MAX; nCntButton++)
		{
			g_aButtonStateTrigger[nCntButton] = (g_aButtonState[nCntButton] ^ g_aPad[nCntButton]) & g_aPad[nCntButton];	//�L�[�g���K�[���ۑ�
			g_aButtonState[nCntButton] = g_aPad[nCntButton];
		}
	}
	return hr;
}

//=============================================================================
// �{�^���̃v���X��Ԃ��擾
//=============================================================================
bool GetJoystickPress(int nKey)
{
	return (g_aPad[nKey]) ? true : false;	//�O�����Z�q
}

//=============================================================================
// �g���K�[����
//=============================================================================
bool GetJoystickTrigger(int nKey)
{
	return (g_aButtonStateTrigger[nKey]) ? true : false;	//�O�����Z�q
}

//=============================================================================
// �R�[���o�b�N�֐�
//=============================================================================
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE*pdidInstance, VOID*pContext)
{
	HRESULT hr;

	hr = g_pJoystick->CreateDevice(pdidInstance->guidInstance, &g_pDevJoystick, NULL);
	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}

	return DIENUM_STOP;
}

//=============================================================================
// �L�[�ݒ�R�[���o�b�N�֐�
//=============================================================================
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE*pdidoi, VOID*pContext)
{
	HRESULT hr;
	DIPROPRANGE diprg;

	diprg.diph.dwSize		= sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow		= DIPH_BYID;
	diprg.diph.dwObj		= pdidoi->dwType;
	diprg.lMin				= 0 - 1000;
	diprg.lMax				= 0 + 1000;

	hr = g_pDevJoystick->SetProperty(DIPROP_RANGE,&diprg.diph);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}
