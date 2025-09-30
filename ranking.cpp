//=====================================================================
//
// ランキング画面 [ranking.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "baseScene.h"
#include "ranking.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "font.h"

#include "player.h"
#include "bullet.h"
#include "decal.h"

#include "util.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define FILEPATH_RANKING		"data\\ranking.bin"
#define MAX_PLACE				(10)

#define INIT_RANKING			{ 50000, 40000, 30000, 20000, 10000 }

#define FADE_START				(600)

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
int compare(const void* arg1, const void* arg2);

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
int g_aRanking[MAX_PLACE] = INIT_RANKING;
int g_nCountStateRanking = 0;
int g_nHighlight = -1;
FONT* g_apFontNum[MAX_PLACE] = {};

//=====================================================================
// 初期化処理
//=====================================================================
void InitRanking(void)
{
	char aString[MAX_PATH] = {};
	g_nCountStateRanking = 0;
	memset(g_apFontNum, 0, sizeof(g_apFontNum));

	InitFont();

	LoadBin(FILEPATH_RANKING, &g_aRanking[0], sizeof(int), MAX_PLACE);

	qsort(&g_aRanking[0], MAX_PLACE, sizeof(int), compare);

	SetFont(
		FONT_LABEL_DEFAULT,
		D3DXVECTOR3(0, 75.0f, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH, 100.0f, 0.0f),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		40,
		"RANKING",
		DT_CENTER
	);

	for (int nCount = 0; nCount < MAX_PLACE; nCount++)
	{
		sprintf(&aString[0], "%2d, %06d", nCount + 1, g_aRanking[nCount]);

		g_apFontNum[nCount] = SetFont(
			FONT_LABEL_DEFAULT,
			D3DXVECTOR3(0, 175.0f + (nCount * 50.0f), 0.0f),
			D3DXVECTOR3(SCREEN_CENTER / 2, 100.0f, 0.0f),
			D3DXVECTOR3_ZERO,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			40,
			&aString[0],
			DT_CENTER
		);
	}
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitRanking(void)
{
	g_nHighlight = -1;
	memset(g_apFontNum, 0, sizeof(g_apFontNum));

	UninitFont();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateRanking(void)
{
	if (g_nHighlight != -1 && g_nCountStateRanking % 5 == 0)
	{
		if (g_nCountStateRanking % 10 == 0)
		{
			g_apFontNum[g_nHighlight]->obj.color = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		}
		else
		{
			g_apFontNum[g_nHighlight]->obj.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START))
	{
		SetFade(SCENE_TITLE);
	}

	g_nCountStateRanking++;

	if (g_nCountStateRanking > FADE_START)
	{
		SetFade(SCENE_TITLE);
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawRanking(void)
{
	DrawFont();
}

void SaveScore(int nScore)
{
	int* pRanking;
	char pFilename[MAX_PATH];

	pRanking = &g_aRanking[0];
	strcpy(pFilename, FILEPATH_RANKING);

	// ファイルからランキングを読み込み
	LoadBin(pFilename, pRanking, sizeof(int), MAX_PLACE);

	// 並び変える
	qsort(pRanking, MAX_PLACE, sizeof(int), compare);

	if (pRanking[MAX_PLACE - 1] < nScore)
	{// 一番低いスコアより現在のスコアの方が高かったら置き換える
		pRanking[MAX_PLACE - 1] = nScore;
	}
	else
	{// そうでなければ保存しない
		return;
	}

	// 並び変える
	qsort(pRanking, MAX_PLACE, sizeof(int), compare);

	// 点滅情報の設定
	for (int nCount = MAX_PLACE - 1; nCount >= 0; nCount--)
	{// ランキングを下から確認していく
		if (pRanking[nCount] == nScore)
		{
			// 同じだったら点滅情報を設定
			g_nHighlight = nCount;
			break;
		}
	}

	// ファイルにランキングを書き出し
	SaveBin(pFilename, pRanking, sizeof(int), MAX_PLACE);
}

//=====================================================================
// qsort関数の判定処理
//=====================================================================
int compare(const void* arg1, const void* arg2)
{
	int va = *(const int*)arg1;
	int vb = *(const int*)arg2;

	if (va > vb) return -1;
	if (va < vb) return 1;
	return 0;
}