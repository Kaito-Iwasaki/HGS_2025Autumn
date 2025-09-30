//=====================================================================
// 
// bullet [bullet.cpp]
// Author : SHUMA AIZU
//
//=====================================================================
#include "bullet.h"
#include "player.h"
#include "enemy.h"
#include "input.h"
#include "collision.h"
#include "util.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define BULLET_TEXTURE_FILENAME	"data\\TEXTURE\\BULLET.png"		// テクスチャファイル名

#define INIT_BULLET_SIZE	D3DXVECTOR3 (25, 25, 0.0f)
#define INIT_BULLET_COLOR	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define INIT_BULLET_SPEED	(7.5f)
#define REFLECTION_TIMER	(6)
#define MAX_SPEED			(30.0f)
#define HORUD_DIFF			(25.0f)

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;		// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTexBuffBullet = NULL;				// テクスチャへのポインタ
BULLET g_bullet = {};									// 弾の情報
bool g_bReflection = false;								// 反射したかどうか
bool g_bUseReflection = false;							// 反射できるかどうか
bool g_bUseHorld = false;
int g_nReflectionCount = 0;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************


//=====================================================================
// 
// ***** 初期化処理 *****
// 
//=====================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得

	memset(&g_bullet, 0, sizeof(BASEOBJECT));
	g_bullet.obj.size = INIT_BULLET_SIZE;
	g_bullet.obj.color = INIT_BULLET_COLOR;
	g_bullet.bulletstate = BULLETSTATE_HORLD;
	g_bullet.obj.bVisible = true;
	g_bullet.move = {};
	g_bullet.fSpeed = INIT_BULLET_SPEED;
	g_bullet.nHorldNumber = 0;
	g_bullet.bUse = true;

	g_bUseHorld = true;
	g_bReflection = false;
	g_bUseReflection = true;
	g_nReflectionCount = 0;

	// テクスチャの読み込み
	if (BULLET_TEXTURE_FILENAME)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			BULLET_TEXTURE_FILENAME,
			&g_pTexBuffBullet
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL
	);
}

//=====================================================================
// 
// ***** 終了処理 *****
// 
//=====================================================================
void UninitBullet(void)
{
	if (g_pTexBuffBullet != NULL)
	{// テクスチャの破棄
		g_pTexBuffBullet->Release();
		g_pTexBuffBullet = NULL;
	}

	if (g_pVtxBuffBullet != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=====================================================================
// 
// ***** 更新処理 *****
// 
//=====================================================================
void UpdateBullet(void)
{

	PLAYER* pPlayer = GetPlayer();

	static int nReflectionCounter = REFLECTION_TIMER;	// 反射のインターバル

	if (g_bReflection == false)
	{// もし反射不可なら
		nReflectionCounter--;							// カウンターを回す
		if (nReflectionCounter <= 0)
		{// 回り切ったら
			nReflectionCounter = REFLECTION_TIMER;		// カウンターリセット
			g_bUseReflection = true;					// 反射可能に
		}
	}

	if (g_bullet.bUse == true)
	{// 使用されていたら

		float fAngle = atan2f(g_bullet.move.x, g_bullet.move.y);

		if (g_bReflection == true)
		{

			g_bullet.fSpeed += 0.35f;

			Clampf(&g_bullet.fSpeed, 0, MAX_SPEED);

			g_bReflection = false;
		}

		// 状態判定
		switch (g_bullet.bulletstate)
		{
		case BULLETSTATE_MOVE:
			// 位置更新
			g_bullet.obj.pos += g_bullet.move;
			CollisionPlayer();
			CollisionEnemy();
			break;

		case BULLETSTATE_HORLD:
			if (pPlayer->bIsHold == true)
			{
				g_bullet.obj.pos.x = pPlayer->obj[g_bullet.nHorldNumber].pos.x + sinf(pPlayer->obj[g_bullet.nHorldNumber].rot.z) * HORUD_DIFF;
				g_bullet.obj.pos.y = pPlayer->obj[g_bullet.nHorldNumber].pos.y + cosf(pPlayer->obj[g_bullet.nHorldNumber].rot.z) * HORUD_DIFF;
				
			}
			else
			{
				g_bUseHorld = false;
				g_bullet.move = Direction(pPlayer->obj[g_bullet.nHorldNumber].rot.z);
				g_bullet.fSpeed = INIT_BULLET_SPEED;
				g_bullet.bulletstate = BULLETSTATE_MOVE;
			}
			break;
		}

		// 位置制限と反射
		if (g_bullet.obj.pos.x <= 0 || g_bullet.obj.pos.x >= SCREEN_WIDTH)
		{
			Clampf(&g_bullet.obj.pos.x, 0, SCREEN_WIDTH);
			g_bullet.move.x *= -1;
			g_bUseReflection = true;
			g_bReflection = true;
		}

		if (g_bullet.obj.pos.y <= 0 || g_bullet.obj.pos.y >= SCREEN_HEIGHT)
		{
			Clampf(&g_bullet.obj.pos.y, 0, SCREEN_HEIGHT);
			g_bullet.move.y *= -1;
			g_bUseReflection = true;
			g_bReflection = true;
		}
	}
}

//=====================================================================
// 
// ***** 描画処理 *****
// 
//=====================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	SetVertexPos(pVtx, g_bullet.obj);
	SetVertexRHW(pVtx, 1.0f);
	SetVertexColor(pVtx, g_bullet.obj.color);
	SetVertexTexturePos(pVtx);

	// 頂点バッファをアンロック
	g_pVtxBuffBullet->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_bullet.obj.bVisible == true)
	{// ポリゴン描画
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTexBuffBullet);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	}
}

//=====================================================================
// 弾の設定処理
//=====================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (g_bullet.bUse == false)
	{// 使用されていなければ
		g_bullet.obj.pos = pos + (rot * 100);
		g_bullet.obj.rot = rot;

		g_bullet.move.x = rot.x * INIT_BULLET_SPEED;
		g_bullet.move.y = rot.y * INIT_BULLET_SPEED;
		g_bullet.move.z = rot.z;

		g_bullet.bulletstate = BULLETSTATE_HORLD;
		g_bullet.obj.bVisible = true;
		g_bullet.bUse = true;
	}
}

//=====================================================================
// プレイヤーとの当たり判定処理
//=====================================================================
void CollisionPlayer(void)
{
	PLAYER* pPlayer = GetPlayer();

	for (int nCntPlayer = 0; nCntPlayer < PLAYER_NUM; nCntPlayer++)
	{
		if (g_bUseReflection == true)
		{
			if (CircleCollision(g_bullet.obj.pos, 2.5f, pPlayer->obj[nCntPlayer].pos, pPlayer->obj[nCntPlayer].size.x) == true)
			{
				char aStr[256];

				float fDot = Dot(Vector3To2(Direction(pPlayer->obj[nCntPlayer].rot.z)), Vector3To2(Direction(pPlayer->obj[nCntPlayer].pos, g_bullet.obj.pos)));

				sprintf(&aStr[0], "%f\n", fDot);

				OutputDebugString(&aStr[0]);

				if (fDot < 0.0f)
				{
					HitPlayer();
					g_bullet.bUse = false;
					g_bullet.fSpeed = INIT_BULLET_SPEED;
				}
				else
				{
					if (pPlayer->bIsHold == true)
					{
						g_bullet.bulletstate = BULLETSTATE_HORLD;
						g_bullet.nHorldNumber = nCntPlayer;
					}
					else
					{
						float fAngle = atan2f(g_bullet.obj.pos.x - pPlayer->obj[nCntPlayer].pos.x, g_bullet.obj.pos.y - pPlayer->obj[nCntPlayer].pos.y);

						if (g_bUseReflection == true)
						{
							g_bullet.move = Direction(pPlayer->obj[nCntPlayer].pos, g_bullet.obj.pos) * g_bullet.fSpeed;

							g_bReflection = true;

							g_bUseReflection = false;
						}
					}
				}
			}
		}

	}
}

//=====================================================================
// 敵との当たり判定処理
//=====================================================================
void CollisionEnemy(void)
{
	ENEMY* pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			float fAngle = atan2f(g_bullet.obj.pos.x - pEnemy->obj.pos.x, g_bullet.obj.pos.y - pEnemy->obj.pos.y);

			if (CircleCollision(g_bullet.obj.pos, g_bullet.obj.size.x / 2, pEnemy->obj.pos, pEnemy->obj.size.x / 2) == true)
			{
				HitEnemy(pEnemy);

				if (pEnemy->bUse == true)
				{
					g_bullet.move = Direction(pEnemy->obj.pos, g_bullet.obj.pos) * g_bullet.fSpeed;

					g_bReflection = true;

					g_bUseReflection = false;
				}

			}
		}
	}
}