//=====================================================================
// 
// enemy [Enemy.cpp]
// Author:
//
//=====================================================================
#include "enemy.h"
#include "baseObject.h"
#include "util.h"
#include "score.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define FILENAME_ENEMY			"data\\ENEMY\\ENEMY_DATA.txt"		// ファイル名(敵関連)
#define ENEMY_SPD				(0.1f)		// 移動速度
#define MAX_TEXTURE				(20)		// テクスチャの最大数
#define OUT_RECT				(5)			// 画面外から出現する敵の消える範囲
#define COUNTERSTATE_APPEAR		(60)		// 出現状態カウンター
#define COUNTERSTATE_DAMAGE		(5)			// ダメージ状態カウンター
#define FILE_TOO_LONG			(0x0001)	// ファイル名が長すぎる場合の返り値
#define FILE_NOT_FOUND			(0x0002)	// ファイルが見つからなかった場合の返り値
#define FILE_CLEAR				(0x0004)	// 成功時の返り値

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;
LPDIRECT3DTEXTURE9 g_apTexBuffEnemy[MAX_TEXTURE] = {};
ENEMY g_aEnemy[MAX_ENEMY] = {};							// 敵の構造体
ENEMY g_aEnemyNormal[MAX_TEXTURE] = {};					// 基本的な敵の種類の構造体
char g_aFileName[MAX_TEXTURE][MAX_PATH] = {};			// ファイル名
int g_nCounterEnemyTexNum;								// テクスチャの数
int g_nCounterEnemy;									// 時間
int g_nEndingTime;										// 敵の出現が終了する時間
int g_nRandSpawn;										// 出現する確率

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void SpownOutOfScreen(ENEMY *pEnemy);
bool IsEnemyInOfRect(ENEMY *pEnemy, RECT rect);
int OpenFileEnemy(const char *pFileName);

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
		pEnemy->nScore = 0;
		pEnemy->spd = 0.0f;
		pEnemy->nCounterState = 0;
		pEnemy->nSpawnTime = -1;
		pEnemy->nTextype = 0;
		pEnemy->bUse = false;
	}

	memset(g_aFileName, NULL, sizeof(g_aFileName));
	g_nCounterEnemyTexNum = 0;
	g_nCounterEnemy = 0;
	g_nEndingTime = 10000;

	int Error = OpenFileEnemy(FILENAME_ENEMY);
	if (Error != FILE_CLEAR)
	{
		HWND hWnd = GetActiveWindow();
		MessageBox(hWnd, "error", "", MB_ICONERROR);
	}

	if (g_aFileName != NULL)
	{
		for (int n = 0; n < g_nCounterEnemyTexNum; n++)
		{
			D3DXCreateTextureFromFile(
				pDevice,
				&g_aFileName[n][0],
				&g_apTexBuffEnemy[n]
			);
		}
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
	for (int n = 0; n < MAX_TEXTURE; n++)
	{
		if (g_apTexBuffEnemy[n] != NULL)
		{// テクスチャの破棄
			g_apTexBuffEnemy[n]->Release();
			g_apTexBuffEnemy[n] = NULL;
		}
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

				if (IsEnemyInOfRect(pEnemy, rect) == true)
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

			switch (pEnemy->state)
			{
			case ENEMYSTATE_NORMAL:

				break;

			case ENEMYSTATE_DAMAGE:

				pEnemy->nCounterState--;
				if (pEnemy->nCounterState <= 0)
				{
					pEnemy->nCounterState = 0;
					pEnemy->state = ENEMYSTATE_NORMAL;
					pEnemy->obj.color = D3DXCOLOR_WHITE;
					if (pEnemy->nHealth == 1)
					{
						pEnemy->obj.color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					}
				}
				else
				{
					if (pEnemy->nCounterState % 4 < 2)
					{
						pEnemy->obj.color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					}
					else
					{
						pEnemy->obj.color = D3DXCOLOR_WHITE;
					}
				}

				break;

			case ENEMYSTATE_APPEAR:

				break;
			}

			pEnemy->obj.pos += pEnemy->move;
			pEnemy->obj.rot.z += pEnemy->move.z * 0.04f;
		}
	}

	g_nCounterEnemy++;

	float aColor[3] = { (float)((rand() % 100) * 0.01f), (float)((rand() % 100) * 0.01f), (float)((rand() % 100) * 0.01f) };

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		if (aColor[nCnt] <= 0.5f)
		{
			aColor[nCnt] *= 1.75f;
			if (aColor[nCnt] >= 1.0f)
			{
				aColor[nCnt] = 1.0f;
			}
		}
	}

	if (rand() % g_nRandSpawn == 0)
	{
		SetEnemy(D3DXVECTOR3(50.0f, 50.0f, 0.0f), ENEMY_SPOWN_IN, D3DXCOLOR(aColor[0], aColor[1], aColor[2], 1.0f));
	}
	else if(rand() % g_nRandSpawn == 1)
	{
		SetEnemy(D3DXVECTOR3(50.0f, 50.0f, 0.0f), ENEMY_SPOWN_OUT, D3DXCOLOR(aColor[0], aColor[1], aColor[2], 1.0f));
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

	// 加算合成を適用
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	pEnemy = &g_aEnemy[0];			// 敵の先頭アドレス

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true && pEnemy->obj.bVisible == true)
		{// ポリゴン描画
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTexBuffEnemy[pEnemy->nTextype]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntEnemy, 2);
		}
	}

	// 加算合成を解除
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=====================================================================
// 
// ***** 敵の配置処理 *****
// 
//=====================================================================
void SetEnemy(D3DXVECTOR3 size, ENEMY_SPOWN spown, D3DXCOLOR col, bool bInversed, D3DXVECTOR3 rot)
{
	ENEMY *pEnemy = &g_aEnemy[0];			// 敵の先頭アドレス
	VERTEX_2D *pVtx;
	int nCnt = 0;

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
			
			nCnt = rand() % g_nCounterEnemyTexNum;

			if (nCnt == 4) break;

			pEnemy->nHealth = g_aEnemyNormal[nCnt].nHealth;
			pEnemy->spd = g_aEnemyNormal[nCnt].spd;
			pEnemy->nScore = g_aEnemyNormal[nCnt].nScore;
			pEnemy->nTextype = g_aEnemyNormal[nCnt].nTextype;

			switch (pEnemy->spown)
			{
			case ENEMY_SPOWN_IN:
				pEnemy->obj.pos = D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0.0f);

				pEnemy->move.z = (float)(RandRange(-(int)(D3DX_PI * 100.0f), (int)(D3DX_PI * 100.0f)) * 0.01f);

				pEnemy->move.x = sinf(pEnemy->move.z) * pEnemy->spd;
				pEnemy->move.y = cosf(pEnemy->move.z) * pEnemy->spd;

				break;

			case ENEMY_SPOWN_OUT:

				SpownOutOfScreen(pEnemy);

				pEnemy->move.z = Angle(pEnemy->obj.pos, D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0.0f));

				pEnemy->move.x = sinf(pEnemy->move.z) * pEnemy->spd;
				pEnemy->move.y = cosf(pEnemy->move.z) * pEnemy->spd;

				break;

			default:

				break;
			}

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
		AddScore(1000 * (pEnemy->nTextype + 1));
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

//=====================================================================
// 
// ***** 敵のファイル読み込み処理 *****
// 
//=====================================================================
int OpenFileEnemy(const char* pFileName)
{
	FILE *pFile = NULL;
	ENEMY *pEnemy = &g_aEnemy[0];
	char aStr[1024] = {};
	char aTrash[3] = {};
	char aIn[10] = { "IN" };
	char aOut[10] = { "OUT" };
	int nTime = 0;
	int n = 0;
	int nCnt = 0;
	int nCntTexNum = 0;

	if ((int)strlen(pFileName) > MAX_PATH) return FILE_TOO_LONG;

	pFile = fopen(pFileName, "r");
	if (pFile == NULL) return FILE_NOT_FOUND;

	(void)fscanf(pFile, "%s", &aStr[0]);
	if (strcmp(aStr, "START_SCRIPT") == 0)
	{
		while (1)
		{
			memset(aStr, NULL, sizeof(aStr));
			(void)fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(aStr, "TEXTURE_NUM") == 0)
			{
				fread(&aTrash[0], 1, sizeof(aTrash), pFile);
				(void)fscanf(pFile, "%d", &g_nCounterEnemyTexNum);

				for (int n = 0; n < g_nCounterEnemyTexNum; n++)
				{
					do
					{
						memset(aStr, NULL, sizeof(aStr));
						(void)fscanf(pFile, "%s", &aStr[0]);
						if (strcmp(aStr, "TEXTURE_PATH") == 0)
						{
							fread(&aTrash[0], 1, sizeof(aTrash), pFile);
							(void)fscanf(pFile, "%s", &g_aFileName[n][0]);
						}
					} while (strcmp(aStr, "TEXTURE_PATH") != 0);
				}
			}

			if (strcmp(aStr, "ENEMYSPAWN_RAND") == 0)
			{
				fread(&aTrash[0], 1, sizeof(aTrash), pFile);
				(void)fscanf(pFile, "%d", &g_nRandSpawn);
			}

			if (strcmp(aStr, "ENEMY_SETTING") == 0)
			{
				while (1)
				{
					memset(aStr, NULL, sizeof(aStr));
					(void)fscanf(pFile, "%s", &aStr[0]);

					if (strcmp(aStr, "START_SETTING") == 0)
					{
						do
						{
							memset(aStr, NULL, sizeof(aStr));
							(void)fscanf(pFile, "%s", &aStr[0]);
							if (strcmp(aStr, "TEXTURE_NUM") == 0)
							{
								fread(&aTrash[0], 1, sizeof(aTrash), pFile);
								(void)fscanf(pFile, "%d", &nCntTexNum);
								g_aEnemyNormal[nCntTexNum].nTextype = nCntTexNum;

								do
								{
									memset(aStr, NULL, sizeof(aStr));
									(void)fscanf(pFile, "%s", &aStr[0]);

									if (strcmp(aStr, "HEALTH") == 0)
									{
										fread(&aTrash[0], 1, sizeof(aTrash), pFile);
										(void)fscanf(pFile, "%d", &g_aEnemyNormal[nCntTexNum].nHealth);
									}

									if (strcmp(aStr, "SPD") == 0)
									{
										fread(&aTrash[0], 1, sizeof(aTrash), pFile);
										(void)fscanf(pFile, "%f", &g_aEnemyNormal[nCntTexNum].spd);
									}

									if (strcmp(aStr, "SCORE") == 0)
									{
										fread(&aTrash[0], 1, sizeof(aTrash), pFile);
										(void)fscanf(pFile, "%d", &g_aEnemyNormal[nCntTexNum].nScore);
									}
								} while (strcmp(aStr, "END_SETTING") != 0);
							}

						} while (strcmp(aStr, "END_SETTING") != 0);
					}

					if (strcmp(aStr, "END_ENEMY_SETTING") == 0)
					{
						break;
					}
				}
			}

			if (strcmp(aStr, "END_SCRIPT") == 0)
			{
				break;
			}
		}
	}
	

	return FILE_CLEAR;
}

//=====================================================================
// 
// ***** タイマー取得 *****
// 
//=====================================================================
int GetEndingTimer(void)
{
	return g_nEndingTime;
}

//=====================================================================
// 
// ***** 敵の座標より指定した範囲に入ったかを確認 *****
// 
//=====================================================================
bool IsEnemyInOfRect(ENEMY* pEnemy, RECT rect)
{
	if (pEnemy->obj.pos.x > rect.left
		&& pEnemy->obj.pos.x < rect.right
		&& pEnemy->obj.pos.y > rect.top
		&& pEnemy->obj.pos.y < rect.bottom)
	{
		return true;
	}
	else
	{
		return false;
	}
}