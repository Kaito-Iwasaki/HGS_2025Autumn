//=====================================================================
//
// font.cpp�̃w�b�_�t�@�C�� [font.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _FONT_H_
#define _FONT_H_

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "baseObject.h"
#include "main.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define MAX_FONT		(256)
#define MAX_TEXTCHAR	(256)

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
typedef enum
{
	FONT_LABEL_DEFAULT = 0,
	FONT_LABEL_DONGURI,
	FONT_LABEL_MAX
}FONT_LABEL;

//*********************************************************************
// 
// ***** �\���� *****
// 
//*********************************************************************
typedef struct
{
	LPD3DXFONT font;

	BASEOBJECT obj;
	FONT_LABEL type;
	bool bUsed;
	int nID;
	char aText[MAX_TEXTCHAR];
	UINT format;
}FONT;

//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************
void InitFont(void);
void UninitFont(void);
void DrawFont(void);
FONT* GetFont(void);
FONT* SetFont(
	FONT_LABEL type,		// �g�p����t�H���g
	D3DXVECTOR3 pos,		// �ʒu
	D3DXVECTOR3 size,		// �T�C�Y
	D3DXVECTOR3 rot,		// ��]
	D3DXCOLOR col,			// �F
	int nScale,				// �e�L�X�g�̃T�C�Y
	const char* aText,		// �\������e�L�X�g
	UINT format				// �e�L�X�g�t�H�[�}�b�g
);
void DeleteFont(FONT* pFont);
void DeleteFont(void);

#endif