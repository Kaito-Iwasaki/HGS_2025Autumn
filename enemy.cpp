//=====================================================================
// 
// enemy [Enemy.cpp]
// Author:
//
//=====================================================================
#include "enemy.h"
#include "baseObject.h"
#include "util.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************

#define ENEMY_SPD				(3.0f)		// 移動速度
#define OUT_RECT				(10)		// 画面外から出現する敵の消える範囲
#define COUNTERSTATE_APPEAR		(60)		// 出現状態カウンター
#define COUNTERSTATE_DAMAGE		(5)			// ダメージ状態カウンター

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffEnemy = NULL;
ENEMY g_aEnemy[MAX_ENEMY] = {};				// 敵の構造体

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************

void SpownOutOfScreen(ENEMY *pEnemy);

//=====================================================================
// 
// ***** 初期化処理 *****
// 
//=====================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *pEnemy = &g_aEnemy[0];			// 敵の先頭アドレス

	// 敵の構造体の初期化
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		pEnemy->obj.pos = D3DXVECTOR3_ZERO;
		pEnemy->obj.rot = D3DXVECTOR3_ZERO;
		pEnemy->obj.size = D3DXVECTOR3_ZERO;
		pEnemy->obj.color = D3DXCOLOR_WHITE;
		pEnemy->obj.bVisible = false;
		pEnemy->obj.bInversed = false;
		pEnemy->move = D3DXVECTOR3_ZERO;
		pEnemy->state = ENEMYSTATE_APPEAR;
		pEnemy->spown = ENEMY_SPOWN_IN;
		pEnemy->nHealth = 0;
		pEnemy->nCounterState = 0;
		pEnemy->bUse = false;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL
	);
}

//=====================================================================
// 
// ***** 終了処理 *****
// 
//=====================================================================
void UninitEnemy(void)
{
	if (g_pTexBuffEnemy != NULL)
	{// テクスチャの破棄
		g_pTexBuffEnemy->Release();
		g_pTexBuffEnemy = NULL;
	}

	if (g_pVtxBuffEnemy != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//=====================================================================
// 
// ***** 更新処理 *****
// 
//=====================================================================
void UpdateEnemy(void)
{
	ENEMY* pEnemy = &g_aEnemy[0];			// 敵の先頭アドレス
	RECT rect = { SCREEN_CENTER - OUT_RECT , SCREEN_VCENTER - OUT_RECT, SCREEN_WIDTH + OUT_RECT, SCREEN_VCENTER + OUT_RECT };

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			switch (pEnemy->spown)
			{
			case ENEMY_SPOWN_IN:
				
				if (IsObjectOutOfScreen(pEnemy->obj))
				{
					pEnemy->bUse = false;
					pEnemy->obj.bVisible = false;
				}

				break;

			case ENEMY_SPOWN_OUT:

				if (IsObjectOutOfScreen(pEnemy->obj, rect) == false)
				{
					pEnemy->bUse = false;
					pEnemy->obj.bVisible = false;
				}

				break;

			default:

				if (IsObjectOutOfScreen(pEnemy->obj))
				{
					pEnemy->bUse = false;
					pEnemy->obj.bVisible = false;
				}

				break;
			}

			pEnemy->obj.pos += pEnemy->move;
		}
	}
}

//=====================================================================
// 
// ***** 描画処理 *****
// 
//=====================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;
	ENEMY *pEnemy = &g_aEnemy[0];			// 敵の先頭アドレス

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点バッファの設定
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{ // 敵が使用されていれば設定
			// 頂点情報を設定
			SetVertexPos(pVtx, pEnemy->obj);			// 位置設定
			SetVertexRHW(pVtx, 1.0f);					// RHW設定
			SetVertexColor(pVtx, pEnemy->obj.color);	// 色設定
			SetVertexTexturePos(pVtx);					// テクスチャ座標設定
		}

		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pEnemy = &g_aEnemy[0];			// 敵の先頭アドレス

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true && pEnemy->obj.bVisible == true)
		{// ポリゴン描画
			// テクスチャの設定
			pDevice->SetTexture(0, NULL);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntEnemy, 2);
		}
	}
}

//=====================================================================
// 
// ***** 敵の配置処理 *****
// 
//=====================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, int nHealth, ENEMY_SPOWN spown, D3DXCOLOR col, bool bInversed, D3DXVECTOR3 rot)
{
	ENEMY *pEnemy = &g_aEnemy[0];			// 敵の先頭アドレス
	VERTEX_2D *pVtx;

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{ // 敵が使われていなければ設定 
			pEnemy->obj.size = size;
			pEnemy->obj.rot = rot;
			pEnemy->obj.color = col;
			pEnemy->obj.bInversed = bInversed;
			pEnemy->obj.bVisible = true;
			pEnemy->state = ENEMYSTATE_APPEAR;
			pEnemy->spown = spown;
			switch (pEnemy->spown)
			{
			case ENEMY_SPOWN_IN:
				pEnemy->obj.pos = D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0.0f);

				pEnemy->move.z = (float)(RandRange(-(int)(D3DX_PI * 100.0f), (int)(D3DX_PI * 100.0f)) * 0.01f);

				pEnemy->move.x = sinf(pEnemy->move.z) * ENEMY_SPD;
				pEnemy->move.y = cosf(pEnemy->move.z) * ENEMY_SPD;

				break;

			case ENEMY_SPOWN_OUT:

				SpownOutOfScreen(pEnemy);

				pEnemy->move.z = Angle(pEnemy->obj.pos, D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0.0f));

				pEnemy->move.x = sinf(pEnemy->move.z) * ENEMY_SPD;
				pEnemy->move.y = cosf(pEnemy->move.z) * ENEMY_SPD;

				break;

			default:

				pEnemy->obj.pos = pos;
				pEnemy->move = move;

				break;
			}

			pEnemy->nHealth = nHealth;
			pEnemy->nCounterState = COUNTERSTATE_APPEAR;
			pEnemy->bUse = true;

			// 頂点情報を設定
			SetVertexPos(pVtx, pEnemy->obj);			// 位置設定
			SetVertexRHW(pVtx, 1.0f);					// RHW設定
			SetVertexColor(pVtx, pEnemy->obj.color);	// 色設定
			SetVertexTexturePos(pVtx);					// テクスチャ座標設定

			break;
		}

		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();
}

//=====================================================================
// 
// ***** 敵の情報の取得 *****
// 
//=====================================================================
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//=====================================================================
// 
// ***** 敵のダメージ判定 *****
// 
//=====================================================================
void HitEnemy(ENEMY* pEnemy, int nDamage)
{
	pEnemy->nHealth -= nDamage;				// 敵の体力をダメージ分減少
	if (pEnemy->nHealth <= 0)
	{ // 体力が0以下になった時
		pEnemy->obj.bVisible = false;		// 不可視状態に
		pEnemy->bUse = false;				// 未使用に
	}
	else
	{ // 体力が0よりも大きければ
		pEnemy->state = ENEMYSTATE_DAMAGE;				// ダメージ状態に
		pEnemy->nCounterState = COUNTERSTATE_DAMAGE;	// カウンターセット
	}
}

//=====================================================================
// 
// ***** 敵の画面外出現処理 *****
// 
//=====================================================================
void SpownOutOfScreen(ENEMY* pEnemy)
{
	D3DXVECTOR3 pos;

	// 出現位置を設定
	int n = rand() % SCREEN_WIDTH;

	pos.x = (float)n;

	if (n == 0 || n == SCREEN_WIDTH)
	{
		pos.y = (float)(rand() % SCREEN_HEIGHT);
	}
	else
	{
		if (rand() % 2 == 0)
		{
			pos.y = 0.0f;
		}
		else
		{
			pos.y = (float)SCREEN_HEIGHT;
		}
	}
	
	pEnemy->obj.pos = pos;
}