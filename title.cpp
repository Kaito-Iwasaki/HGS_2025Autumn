//=====================================================================
// 
// タイトル画面 [title.h]
// Author : Kaito Iwasaki
//
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "title.h"
#include "player.h"
#include "input.h"
#include "fade.h"
#include "decal.h"
#include "font.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** 列挙型定義 *****
// 
//*********************************************************************
typedef enum
{
	TITLESTATE_INTRO = 0,
	TITLESTATE_NORMAL,
	TITLESTATE_START
}TITLESTATE;

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
FONT* g_pFontStart = NULL;
int g_nCounterStateTitle;
TITLESTATE g_stateTitle = TITLESTATE_NORMAL;

//=====================================================================
// 初期化処理
//=====================================================================
void InitTitle(void)
{
	InitDecal();
	InitFont();

	g_nCounterStateTitle = 0;
	g_stateTitle = TITLESTATE_NORMAL;

	// タイトル画像を配置
	SetDecal(
		DECAL_LABEL_TITLE,
		D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
	);

	// テキストを配置
	g_pFontStart = SetFont(
		FONT_LABEL_DONGURI,
		D3DXVECTOR3(0, SCREEN_VCENTER + 200.0f, 0),
		D3DXVECTOR3(SCREEN_WIDTH, 200, 0),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		100,
		"PRESS ENTER",
		DT_CENTER
	);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitTitle(void)
{
	UninitDecal();
	UninitFont();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateTitle(void)
{
	g_nCounterStateTitle++;
	switch (g_stateTitle)
	{
	case TITLESTATE_NORMAL:
		if (g_nCounterStateTitle % 30 == 0)
		{
			g_pFontStart->obj.bVisible ^= 1;
		}

		if (GetKeyboardTrigger(DIK_RETURN))
		{
			g_stateTitle = TITLESTATE_START;
			g_nCounterStateTitle = 0;
		}
		break;

	case TITLESTATE_START:
		if (g_nCounterStateTitle % 3 == 0)
		{
			g_pFontStart->obj.bVisible ^= 1;
		}

		if (g_nCounterStateTitle > 60)
		{
			SetFade(SCENE_GAME);
		}
		break;
	}


}

//=====================================================================
// 終了処理
//=====================================================================
void DrawTitle(void)
{
	DrawDecal();
	DrawFont();
}
