//=====================================================================
// 
// score [score.cpp]
// Author : Kaito Iwasaki
//
//=====================================================================
#include "score.h"
#include "util.h"
#include "baseScene.h"
#include "sound.h"
#include "font.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME		"data\\TEXTURE\\number000.png"
#define TEXTURE_MAX_X			(10)

#define INIT_POS				{0.0f, 0.0f, 0.0f}
#define INIT_SIZE				{200.0f, 50.0f, 0.0f}
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		// 頂点バッファへのポインタ
SCORE g_score;

//=====================================================================
// 
// ***** 初期化処理 *****
// 
//=====================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_FILENAME, &g_pTextureScore);

	// スコアの情報の初期化
	memset(&g_score, 0, sizeof(SCORE));
	g_score.obj.pos = INIT_POS;
	g_score.obj.size = INIT_SIZE;
	g_score.obj.color = INIT_COLOR;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL
	);
}

//=====================================================================
// 
// ***** 終了処理 *****
// 
//=====================================================================
void UninitScore(void)
{
	// テクスチャの破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//=====================================================================
// 
// ***** 更新処理 *****
// 
//=====================================================================
void UpdateScore(void)
{
	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		g_score.aTexPattern[nCntScore] = g_score.nScore % (int)pow(10, (double)NUM_PLACE - nCntScore) / (int)pow(10, (double)NUM_PLACE - 1 - nCntScore);
	}
}

//=====================================================================
// 
// ***** 描画処理 *****
// 
//=====================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < NUM_PLACE; nCount++, pVtx += 4)
	{
		// 頂点情報を設定
		SetVertexPos(pVtx,
			D3DXVECTOR3(g_score.obj.pos.x + (nCount * g_score.obj.size.x / TEXTURE_MAX_X), g_score.obj.pos.y, 0.0f),
			D3DXVECTOR3(g_score.obj.pos.x + ((nCount + 1) * g_score.obj.size.x / TEXTURE_MAX_X), g_score.obj.pos.y, 0.0f),
			D3DXVECTOR3(g_score.obj.pos.x + (nCount * g_score.obj.size.x / TEXTURE_MAX_X), g_score.obj.pos.y + g_score.obj.size.y, 0.0f),
			D3DXVECTOR3(g_score.obj.pos.x + ((nCount + 1) * g_score.obj.size.x / TEXTURE_MAX_X), g_score.obj.pos.y + g_score.obj.size.y, 0.0f)
			);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, g_score.obj.color);
		SetVertexTexturePos(pVtx,
			D3DXVECTOR2(1.0f / TEXTURE_MAX_X * (g_score.aTexPattern[nCount]), 0.0f),
			D3DXVECTOR2(1.0f / TEXTURE_MAX_X * (g_score.aTexPattern[nCount] + 1), 0.0f),
			D3DXVECTOR2(1.0f / TEXTURE_MAX_X * (g_score.aTexPattern[nCount]), 1.0f),
			D3DXVECTOR2(1.0f / TEXTURE_MAX_X * (g_score.aTexPattern[nCount] + 1), 1.0f)
			);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffScore->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureScore);

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}

void SetScore(int nScore)
{
	g_score.nScore = nScore;
}

void AddScore(int nValue)
{
	g_score.nScore += nValue;
}

int GetScore(void)
{
	return g_score.nScore;
}
