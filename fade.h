//=====================================================================
// 
// fade.cpp�̃w�b�_�t�@�C�� [fade.h]
// Author:���O��
//
//=====================================================================
#ifndef _FADE_H_
#define _FADE_H_		// ��d�C���N���[�h�h�~�̃}�N��

#include "main.h"
#include "baseScene.h"

typedef enum
{
	FADE_NONE = 0,
	FADE_IN,
	FADE_OUT,
	FADE_MAX
}FADE;

// �v���g�^�C�v�錾
void InitFade(SCENE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);
void SetFade(SCENE modeNext, bool bStopSound = true);
FADE GetFade(void);
float GetFadeAlpha(void);

#endif