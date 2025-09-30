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
#define FILENAME_ENEMY			"data\\ENEMY\\ENEMY_DATA.txt"		// ファイル名(敵関連)
#define ENEMY_SPD				(3.0f)		// 移動速度
#define OUT_RECT				(10)		// 画面外から出現する敵の消える範囲
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
LPDIRECT3DTEXTURE9 g_pTexBuffEnemy = NULL;
ENEMY g_aEnemy[MAX_ENEMY] = {};				// 敵の構造体
char g_aFileName[MAX_PATH] = {};			// ファイル名
int g_nCounterEnemyTexNum;					// テクスチャの数
int g_nCounterEnemy;						// 時間

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void SpownOutOfScreen(ENEMY *pEnemy);
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
		pEnemy->nCounterState = 0;
		pEnemy->nSpawnTime = -1;
		pEnemy->bUse = false;
	}

	memset(g_aFileName, NULL, sizeof(g_aFileName));
	g_nCounterEnemyTexNum = 0;
	g_nCounterEnemy = 0;

	int Error = OpenFileEnemy(FILENAME_ENEMY);
	if (Error != FILE_CLEAR)
	{
		HWND hWnd = GetActiveWindow();
		MessageBox(hWnd, "error", "", MB_ICONERROR);
	}

	if (g_aFileName != NULL)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			"data\\TEXTURE\\nihaha000.png",
			&g_pTexBuffEnemy
		);
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
		}

		if (pEnemy->nSpawnTime == g_nCounterEnemy)
		{
			SetEnemy(pEnemy->obj.pos, pEnemy->move, pEnemy->obj.size, pEnemy->nHealth, pEnemy->spown);
		}
	}

	g_nCounterEnemy++;
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
			pDevice->SetTexture(0, g_pTexBuffEnemy);

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

			if (strcmp(aStr, "TEXTURE_PATH") == 0)
			{
				fread(&aTrash[0], 1, sizeof(aTrash), pFile);
				(void)fscanf(pFile, "%s", &g_aFileName[0]);
			}

			if (strcmp(aStr, "TEXTURE_NUM") == 0)
			{
				fread(&aTrash[0], 1, sizeof(aTrash), pFile);
				(void)fscanf(pFile, "%d", &g_nCounterEnemyTexNum);
			}

			if (strcmp(aStr, "TIME") == 0)
			{
				fread(&aTrash[0], 1, sizeof(aTrash), pFile);
				(void)fscanf(pFile, "%d", &nTime);
				while (1)
				{
					memset(aStr, NULL, sizeof(aStr));
					(void)fscanf(pFile, "%s", &aStr[0]);
					if (strcmp(aStr, "START_SETENEMY") == 0)
					{
						while (1)
						{
							memset(aStr, NULL, sizeof(aStr));
							(void)fscanf(pFile, "%s", &aStr[0]);
							if (strcmp(aStr, "ENEMY_SPAWN") == 0)
							{
								fread(&aTrash[0], 1, sizeof(aTrash), pFile);
								memset(aStr, NULL, sizeof(aStr));
								(void)fscanf(pFile, "%s", &aStr[0]);
								if (strcmp(aStr, "TRUE") == 0)
								{
									memset(aStr, NULL, sizeof(aStr));
									(void)fscanf(pFile, "%s", &aStr[0]);
									if (strcmp(aStr, "SPAWN") == 0)
									{
										fread(&aTrash[0], 1, sizeof(aTrash), pFile);
										memset(aStr, NULL, sizeof(aStr));
										(void)fscanf(pFile, "%s", &aStr[0]);
										if (strcmp(aStr, "IN") == 0)
										{
											g_aEnemy[n].spown = ENEMY_SPOWN_IN;
										}
										else if (strcmp(aStr, "OUT") == 0)
										{
											g_aEnemy[n].spown = ENEMY_SPOWN_OUT;
										}
									}

									memset(aStr, NULL, sizeof(aStr));
									(void)fscanf(pFile, "%s", &aStr[0]);
									if (strcmp(aStr, "HEALTH") == 0)
									{
										fread(&aTrash[0], 1, sizeof(aTrash), pFile);
										(void)fscanf(pFile, "%d", &g_aEnemy[n].nHealth);
									}

									g_aEnemy[n].obj.pos = D3DXVECTOR3_ZERO;
									g_aEnemy[n].move = D3DXVECTOR3_ZERO;
								}
								else if (strcmp(aStr, "FALSE") == 0)
								{
									memset(aStr, NULL, sizeof(aStr));
									(void)fscanf(pFile, "%s", &aStr[0]);
									if (strcmp(aStr, "POS") == 0)
									{
										fread(&aTrash[0], 1, sizeof(aTrash), pFile);
										(void)fscanf(pFile, "%f %f %f", &g_aEnemy[n].obj.pos.x, &g_aEnemy[n].obj.pos.y, &g_aEnemy[n].obj.pos.z);
									}

									memset(aStr, NULL, sizeof(aStr));
									(void)fscanf(pFile, "%s", &aStr[0]);
									if (strcmp(aStr, "MOVE") == 0)
									{
										fread(&aTrash[0], 1, sizeof(aTrash), pFile);
										(void)fscanf(pFile, "%f %f %f", &g_aEnemy[n].move.x, &g_aEnemy[n].move.y, &g_aEnemy[n].move.z);
									}

									memset(aStr, NULL, sizeof(aStr));
									(void)fscanf(pFile, "%s", &aStr[0]);
									if (strcmp(aStr, "HEALTH") == 0)
									{
										fread(&aTrash[0], 1, sizeof(aTrash), pFile);
										(void)fscanf(pFile, "%d", &g_aEnemy[n].nHealth);
									}
								}
							}

							g_aEnemy[n].obj.size = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
							g_aEnemy[n].nSpawnTime = nTime;

							if (strcmp(aStr, "END_SETENEMY") == 0)
							{
								n++;
								break;
							}
						}
					}

					if (strcmp(aStr, "END_TIME") == 0)
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