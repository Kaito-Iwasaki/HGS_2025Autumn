//=====================================================================
// 
// input.cpp�̃w�b�_�t�@�C�� [input.h]
// Author : Kaito Iwasaki
//
//=====================================================================
#ifndef _INPUT_H_
#define _INPUT_H_		// ��d�C���N���[�h�h�~�̃}�N��

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "main.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define NUM_KEY_MAX		(256)	// �ő�L�[��
#define INPUT_MAX_MAGNITUDE		(32767)		// �X�e�B�b�N�̍ő�ړ���
#define INPUT_DEADZONE			(10000)		// �X�e�B�b�N�̃f�b�h�]�[���i0�`32767�j

//*********************************************************************
// 
// ***** �񋓌^ *****
// 
//*********************************************************************
typedef enum
{
	JOYKEY_UP = 0,		// �\���L�[��
	JOYKEY_DOWN,		// �\���L�[��
	JOYKEY_LEFT,		// �\���L�[��
	JOYKEY_RIGHT,		// �\���L�[�E
	JOYKEY_START,		// �X�^�[�g
	JOYKEY_BACK,		// �o�b�N�i�Z���N�g�j
	JOYKEY_LTHUMB,		// ���X�e�B�b�N��������
	JOYKEY_RTHUMB,		// �E�X�e�B�b�N��������
	JOYKEY_LSHOULDER,	// LB
	JOYKEY_RSHOULDER,	// RB
	JOYKEY_A = 12,		// A
	JOYKEY_B,			// B
	JOYKEY_X,			// X
	JOYKEY_Y,			// Y
	JOYKEY_MAX
}JOYKEY;

typedef enum
{
	JOYSTICK_L_UP = 0,	// ���W���C�X�e�B�b�N��
	JOYSTICK_L_DOWN,		// ���W���C�X�e�B�b�N��
	JOYSTICK_L_LEFT,		// ���W���C�X�e�B�b�N��
	JOYSTICK_L_RIGHT,	// ���W���C�X�e�B�b�N�E
	JOYSTICK_R_UP,		// �E�W���C�X�e�B�b�N��
	JOYSTICK_R_DOWN,	// �E�W���C�X�e�B�b�N��
	JOYSTICK_R_LEFT,	// �E�W���C�X�e�B�b�N��
	JOYSTICK_R_RIGHT,	// �E�W���C�X�e�B�b�N�E
	JOYSTICK_MAX
}JOYSTICK;

//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************
//*********************************************************************
// �L�[�{�[�h
//*********************************************************************
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);
bool GetKeyboardRepeat(int nKey);
BYTE* GetKeyState(void);
//*********************************************************************
// �W���C�p�b�h
//*********************************************************************
HRESULT InitJoypad(void);
void UninitJoypad(void);
void UpdateJoypad(void);
XINPUT_STATE* GetJoypad(void);
bool GetJoypadPress(JOYKEY key);
bool GetJoypadTrigger(JOYKEY key);
bool GetJoypadRelease(JOYKEY key);
bool GetJoypadRepeat(JOYKEY key);
bool GetJoystickPress(JOYSTICK stick);
bool GetJoystickRepeat(JOYSTICK stick);
void SetVibration(WORD wLeftMotorSpeed, WORD wRightMotorSpeed, int nCountVibration = -1);

#endif