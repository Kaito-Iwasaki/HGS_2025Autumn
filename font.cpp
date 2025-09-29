//=====================================================================
//
// �e�L�X�g�I�u�W�F�N�g [font.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "input.h"
#include "sound.h"
#include "util.h"

#include "font.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define INIT_POS_X				(SCREEN_WIDTH / 2)
#define INIT_POS_Y				(SCREEN_HEIGHT/ 2)
#define INIT_SIZE_X				(8.0f)
#define INIT_SIZE_Y				(32.0f)
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

//*********************************************************************
// 
// ***** �\���� *****
// 
//*********************************************************************
typedef struct
{
	char aFontFileName[MAX_PATH];	// �t�H���g�t�@�C���ւ̃p�X
	char aFontName[LF_FACESIZE];	// �t�H���g��
}FONT_INFO;

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
FONT g_aFont[MAX_FONT] = {};

FONT_INFO g_aFontInfo[FONT_LABEL_MAX] = {
	{"", ""},
	{"data\\FONT\\x10y12pxDonguriDuel.ttf", "x10y12pxDonguriDuel" }
};

//=====================================================================
// ����������
//=====================================================================
void InitFont(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X
	VERTEX_2D* pVtx;							// ���_���
	FONT* pFont = &g_aFont[0];

	// �\���̂̏�����
	memset(pFont, 0, sizeof(FONT) * MAX_FONT);

	// �t�H���g��ǉ�
	for (int nCount = 0; nCount < FONT_LABEL_MAX; nCount++)
	{
		AddFontResourceEx(&g_aFontInfo[nCount].aFontFileName[0], FR_PRIVATE, 0);
	}
}

//=====================================================================
// �I������
//=====================================================================
void UninitFont(void)
{
	for (int nCount = 0; nCount < MAX_FONT; nCount++)
	{
		if (g_aFont[nCount].font != NULL)
		{
			g_aFont[nCount].font->Release();
			g_aFont[nCount].font = NULL;
		}
	}

	for (int nCount = 0; nCount < FONT_LABEL_MAX; nCount++)
	{
		RemoveFontResourceEx(&g_aFontInfo[nCount].aFontFileName[0], FR_PRIVATE, 0);
	}
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawFont(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	FONT* pFont = &g_aFont[0];

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	pFont = &g_aFont[0];
	for (int nCount = 0; nCount < MAX_FONT; nCount++, pFont++)
	{
		if (pFont != NULL && pFont->obj.bVisible && pFont->bUsed)
		{// �e�L�X�g�`��
			RECT rect;
			float fMagnitude;

			rect = {
				(long)pFont->obj.pos.x,
				(long)pFont->obj.pos.y,
				(long)(pFont->obj.pos.x + pFont->obj.size.x),
				(long)(pFont->obj.pos.y + pFont->obj.size.y),
			};

			pFont->font->DrawText(NULL, pFont->aText, -1, &rect, pFont->format, pFont->obj.color);
		}
	}
}

//=====================================================================
// �I�u�W�F�N�g�擾����
//=====================================================================
FONT* GetFont(void)
{
	return &g_aFont[0];
}

//=====================================================================
// �摜�̐ݒ菈��
//=====================================================================
FONT* SetFont(
	FONT_LABEL type,		// �g�p����t�H���g
	D3DXVECTOR3 pos,		// �ʒu
	D3DXVECTOR3 size,		// �T�C�Y
	D3DXVECTOR3 rot,		// ��]
	D3DXCOLOR col,			// �F
	int nScale,				// �e�L�X�g�̃T�C�Y
	const char* aText,		// �\������e�L�X�g
	UINT format				// �e�L�X�g�t�H�[�}�b�g
)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	FONT* pFont = &g_aFont[0];
	for (int nCount = 0; nCount < MAX_FONT; nCount++, pFont++)
	{
		if (pFont->font == NULL)
		{
			memset(pFont, 0, sizeof(FONT));
			pFont->bUsed = true;
			pFont->obj.pos = pos;
			pFont->obj.size = size;
			pFont->obj.color = col;
			pFont->nID = nCount;
			pFont->type = type;
			pFont->format = format;
			pFont->obj.bVisible = true;
			strcpy(&pFont->aText[0], aText);

			D3DXFONT_DESC fontDesc = {
				nScale,							// Height
				0,								// Width (0 = ����)
				FW_NORMAL,						// Weight
				1,								// MipLevels
				FALSE,							// Italic
				DEFAULT_CHARSET,				// CharSet
				OUT_DEFAULT_PRECIS,				// OutputPrecision
				CLIP_DEFAULT_PRECIS,			// Quality
				DEFAULT_PITCH | FF_DONTCARE,	// PitchAndFamily
				""
			};

			strcpy(&fontDesc.FaceName[0], &g_aFontInfo[type].aFontName[0]);

			D3DXCreateFontIndirect(pDevice, &fontDesc, &pFont->font);

			return pFont;
		}
	}

	return NULL;
}

//=====================================================================
// �摜�̍폜�����i�w��j
//=====================================================================
void DeleteFont(FONT* pFont)
{
	memset(pFont, 0, sizeof(FONT));
}

//=====================================================================
// �摜�̍폜�����i�ꊇ�j
//=====================================================================
void DeleteFont(void)
{
	memset(&g_aFont[0], 0, sizeof(FONT) * MAX_FONT);
}
