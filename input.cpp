//=====================================================================
// 
// ���͏��� [input.cpp]
// Author : Kaito Iwasaki
//
//=====================================================================

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "input.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define REPEAT_START	(20)	// ���s�[�g�J�n�܂ł̃J�E���g
#define REPEAT_INTERVAL	(8)		// ���s�[�g���̃J�E���g

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
LPDIRECTINPUT8 g_pInput = NULL;
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;

BYTE g_aKeyState[NUM_KEY_MAX];
BYTE g_aKeyTriggerState[NUM_KEY_MAX];
BYTE g_aKeyReleaseState[NUM_KEY_MAX];
int g_aKeyRepeatState[NUM_KEY_MAX];

XINPUT_STATE g_joyKeyState;
XINPUT_STATE g_joyKeyTriggerState;
XINPUT_STATE g_joyKeyReleaseState;
int g_aJoyKeyRepeatState[JOYKEY_MAX];
int g_aJoystickState[JOYSTICK_MAX];
XINPUT_VIBRATION g_vibration;
int g_nCountVibration = 0;

//=====================================================================
// 
// ***** �L�[�{�[�h *****
//
//=====================================================================
//=====================================================================
// ����������
//=====================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// DIrectInput�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&g_pInput,
		NULL
	)))
	{
		return E_FAIL;
	}
	
	// ���̓f�o�C�X�̐���
	if (FAILED(g_pInput->CreateDevice(
		GUID_SysKeyboard,
		&g_pDevKeyboard,
		NULL
	)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)
	)))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�̃A�N�Z�X�����l��
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//=====================================================================
// �I������
//=====================================================================
void UninitKeyboard(void)
{
	// ���̓f�o�C�X�i�L�[�{�[�h�j�j��
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// DirectInput�I�u�W�F�N�g�̔j��
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	// �L�[�{�[�h�̓��͏��
	int nCntKey;

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyTriggerState[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			g_aKeyReleaseState[nCntKey] = g_aKeyState[nCntKey] & (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]);
			g_aKeyState[nCntKey] = aKeyState[nCntKey];

			if (GetKeyboardPress(nCntKey))
			{
				g_aKeyRepeatState[nCntKey] += 1;
			}
			else
			{
				g_aKeyRepeatState[nCntKey] = 0;
			}
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();	// �L�[�{�[�h�̃A�N�Z�X�������Ď擾
	}
}

//=====================================================================
// �L�[�{�[�h�̃v���X�����擾
//=====================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//=====================================================================
// �L�[�{�[�h�̃g���K�[�����擾
//=====================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyTriggerState[nKey] & 0x80) ? true : false;
}

//=====================================================================
// �L�[�{�[�h�̃����[�X�����擾
//=====================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyReleaseState[nKey] & 0x80) ? true : false;
}

//=====================================================================
// �L�[�{�[�h�̃��s�[�g�����擾
//=====================================================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyRepeatState[nKey] == 1 || g_aKeyRepeatState[nKey] >= REPEAT_START && g_aKeyRepeatState[nKey] % REPEAT_INTERVAL == 0) ? true : false;
}

//=====================================================================
// �L�[�{�[�h�̏�Ԃ��擾
//=====================================================================
BYTE* GetKeyState(void)
{
	return &g_aKeyState[0];
}

//=====================================================================
// 
// ***** �W���C�p�b�h *****
//
//=====================================================================
//=====================================================================
// ����������
//=====================================================================
HRESULT InitJoypad(void)
{
	// �������̃N���A
	memset(&g_joyKeyState, 0, sizeof(XINPUT_STATE));
	memset(&g_vibration, 0, sizeof(XINPUT_VIBRATION));
	g_nCountVibration = 0;

	// XINPUT�̃X�e�[�g��L���ɂ���
	XInputEnable(true);

	return S_OK;
}

//=====================================================================
// �I������
//=====================================================================
void UninitJoypad(void)
{
	// XInput�̃X�e�[�g�𖳌�
	XInputEnable(false);
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateJoypad(void)
{
	XINPUT_STATE joyKeyState;
	XINPUT_GAMEPAD gamepad;

	// �W���C�p�b�h�̏��
	if (XInputGetState(0, &joyKeyState) == ERROR_SUCCESS)
	{
		// �W���C�L�[
		gamepad = joyKeyState.Gamepad;

		g_joyKeyTriggerState.Gamepad.wButtons = (g_joyKeyState.Gamepad.wButtons ^ joyKeyState.Gamepad.wButtons)
			& joyKeyState.Gamepad.wButtons;
		g_joyKeyReleaseState.Gamepad.wButtons = g_joyKeyState.Gamepad.wButtons & (g_joyKeyState.Gamepad.wButtons
			^ joyKeyState.Gamepad.wButtons);
		g_joyKeyState = joyKeyState;

		for (int nCntKey = 0; nCntKey < JOYKEY_MAX; nCntKey++)
		{
			if (GetJoypadPress((JOYKEY)nCntKey))
			{
				g_aJoyKeyRepeatState[nCntKey] += 1;
			}
			else
			{
				g_aJoyKeyRepeatState[nCntKey] = 0;
			}
		}

		// �X�e�B�b�N
		if (gamepad.sThumbLX < -INPUT_DEADZONE)
		{// L��
			g_aJoystickState[JOYSTICK_L_LEFT]++;
		}
		else
		{
			g_aJoystickState[JOYSTICK_L_LEFT] = 0;
		}

		if (gamepad.sThumbLX > INPUT_DEADZONE)
		{// L�E
			g_aJoystickState[JOYSTICK_L_RIGHT]++;
		}
		else
		{
			g_aJoystickState[JOYSTICK_L_RIGHT] = 0;
		}

		if (gamepad.sThumbLY < -INPUT_DEADZONE)
		{// L��
			g_aJoystickState[JOYSTICK_L_DOWN]++;
		}
		else
		{
			g_aJoystickState[JOYSTICK_L_DOWN] = 0;
		}

		if (gamepad.sThumbLY > INPUT_DEADZONE)
		{// L��
			g_aJoystickState[JOYSTICK_L_UP]++;
		}
		else
		{
			g_aJoystickState[JOYSTICK_L_UP] = 0;
		}

		if (gamepad.sThumbLY < -INPUT_DEADZONE)
		{// R��
			g_aJoystickState[JOYSTICK_R_LEFT]++;
		}
		else
		{
			g_aJoystickState[JOYSTICK_R_LEFT] = 0;
		}

		if (gamepad.sThumbLY > INPUT_DEADZONE)
		{// R�E
			g_aJoystickState[JOYSTICK_R_RIGHT]++;
		}
		else
		{
			g_aJoystickState[JOYSTICK_R_RIGHT] = 0;
		}

		if (gamepad.sThumbLY < -INPUT_DEADZONE)
		{// R��
			g_aJoystickState[JOYSTICK_R_DOWN]++;
		}
		else
		{
			g_aJoystickState[JOYSTICK_R_DOWN] = 0;
		}

		if (gamepad.sThumbLY > INPUT_DEADZONE)
		{// R��
			g_aJoystickState[JOYSTICK_R_UP]++;
		}
		else
		{
			g_aJoystickState[JOYSTICK_R_UP] = 0;
		}
	}

	// �U�����̍X�V
	if (g_nCountVibration > -1) g_nCountVibration--;
	if (g_nCountVibration == 0)
	{
		g_vibration.wLeftMotorSpeed = 0;
		g_vibration.wRightMotorSpeed = 0;
		XInputSetState(0, &g_vibration);
	}
}

//=====================================================================
// �W���C�p�b�h�̎擾
//=====================================================================
XINPUT_STATE* GetJoypad(void)
{
	return &g_joyKeyState;
}

//=====================================================================
// �W���C�p�b�h�̃v���X�����擾
//=====================================================================
bool GetJoypadPress(JOYKEY key)
{
	return (g_joyKeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=====================================================================
// �W���C�p�b�h�̃g���K�[�����擾
//=====================================================================
bool GetJoypadTrigger(JOYKEY key)
{
	return (g_joyKeyTriggerState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=====================================================================
// �W���C�p�b�h�̃����[�X�����擾
//=====================================================================
bool GetJoypadRelease(JOYKEY key)
{
	return (g_joyKeyReleaseState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=====================================================================
// �W���C�p�b�h�̃��s�[�g�����擾
//=====================================================================
bool GetJoypadRepeat(JOYKEY key)
{
	return (
		g_aJoyKeyRepeatState[key] == 1 ||
		g_aJoyKeyRepeatState[key] >= REPEAT_START &&
		g_aJoyKeyRepeatState[key] % REPEAT_INTERVAL == 0
		) ? true : false;
}

bool GetJoystickPress(JOYSTICK stick)
{
	return g_aJoystickState[stick] > 0;
}

bool GetJoystickRepeat(JOYSTICK stick)
{
	return (
		g_aJoystickState[stick] == 1 ||
		g_aJoystickState[stick] >= REPEAT_START &&
		g_aJoystickState[stick] % REPEAT_INTERVAL == 0
		) ? true : false;
}

//=====================================================================
// �U�����̐ݒ菈��
//=====================================================================
void SetVibration(WORD wLeftMotorSpeed, WORD wRightMotorSpeed, int nCountVibration)
{
	g_nCountVibration = nCountVibration;
	g_vibration.wLeftMotorSpeed = wLeftMotorSpeed; // use any value between 0-65535 here
	g_vibration.wRightMotorSpeed = wRightMotorSpeed; // use any value between 0-65535 here
	XInputSetState(0, &g_vibration);
}