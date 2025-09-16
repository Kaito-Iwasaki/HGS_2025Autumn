//=====================================================================
//
// main.cpp�̃w�b�_�t�@�C�� [main.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "d3dx9.h"		// �`�揈���ɕK�v
#define DIRECTINPUT_VERSION		(0x0800)	
#include "dinput.h"		// ���͏����ɕK�v
#include "xinput.h"		// �W���C�p�b�h���͏����ɕK�v
#include "xaudio2.h"	// �T�E���h�����ɕK�v

//*********************************************************************
// 
// ���C�u�����ւ̃����N
// 
//*********************************************************************
#pragma comment(lib, "d3d9.lib")		// �`�揈���ɕK�v
#pragma comment(lib, "d3dx9.lib")		// ��̊g�����C�u����
#pragma comment(lib, "dxguid.lib")		// DirextX�R���|�[�l���g�̎g�p�ɕK�v
#pragma comment(lib, "winmm.lib")		// �V�X�e�������擾�ɕK�v
#pragma comment(lib, "dinput8.lib")		// ���͏����ɕK�v
#pragma comment(lib, "xinput.lib")		// �W���C�p�b�h�����ɕK�v

//*********************************************************************
// 
// �}�N����`
// 
//*********************************************************************
#define SCREEN_WIDTH	(1280)					// ��ʕ�
#define SCREEN_HEIGHT	(720)					// ��ʍ���
#define SCREEN_CENTER	(SCREEN_WIDTH / 2)		// ��ʒ��S
#define SCREEN_VCENTER	(SCREEN_HEIGHT / 2)		// ��ʏc���S
#define WINDOWED		(true)					// �E�B���h�E�\��

#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// ���_�t�H�[�}�b�g[2D]
#define D3DXVECTOR3_ZERO	D3DXVECTOR3(0.0f, 0.0f, 0.0f)					// �x�N�g�������l

//*********************************************************************
// 
// ***** �\���� *****
// 
//*********************************************************************
//*********************************************************************
// ���_���[2D]�\����
//*********************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
}VERTEX_2D;

//*********************************************************************
// 
// �v���g�^�C�v�錾
// 
//*********************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);

#endif
