//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : Kaito Iwasaki
// Special Thanks : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�ꗗ
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_SE_HIT00 = 0,
	SOUND_LABEL_SE_HIT01,
	SOUND_LABEL_SE_EXPLOSION,
	SOUND_LABEL_BGM_1,
	SOUND_LABEL_MAX
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label, float fVolume = 1.0f);
void StopSound(SOUND_LABEL label);
void StopSound(void);
void PauseSound(SOUND_LABEL label);
HRESULT UnPauseSound(SOUND_LABEL label);

#endif
