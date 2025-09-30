//=====================================================================
// 
// プレイヤー処理 [player.cpp]
// Author : Kaito Iwasaki
//
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "player.h"
#include "input.h"
#include "util.h"
#include "Bullet.h"
#include "decal.h"
#include "fade.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME	"data\\TEXTURE\\player000.png"		// テクスチャファイル名

#define INIT_PLAYER_POS		D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0.0f)
#define INIT_PLAYER_SIZE	D3DXVECTOR3 (50.0f, 50.0f, 0.0f)
#define INIT_PLAYER_COLOR	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define INIT_PLAYER_SPEED	(0.05f)
#define INIT_PLAYER_LEFT	(3)

#define PLAYER_POS_RADIUS	(250.0f)

#define PLAYERSTATE_TIME_APPEAR		(120)

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffPlayer = NULL;
PLAYER g_player;

//=====================================================================
// 初期化処理
//=====================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 構造体の初期化
	memset(&g_player, 0, sizeof(PLAYER));
	g_player.fSpeed = INIT_PLAYER_SPEED;
	g_player.state = PLAYERSTATE_INIT;
	g_player.bIsHold = true;
	g_player.nPlayerLeft = INIT_PLAYER_LEFT;

	for (int i = 0; i < PLAYER_NUM; i++)
	{
		g_player.obj[i].size = INIT_PLAYER_SIZE;
		g_player.obj[i].color = INIT_PLAYER_COLOR;
		g_player.obj[i].bVisible = true;
	}

	// テクスチャの読み込み
	if (TEXTURE_FILENAME)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffPlayer
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * PLAYER_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL
	);

	SetDecal(
		DECAL_LABEL_CIRCLE,
		D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0),
		D3DXVECTOR3(PLAYER_POS_RADIUS * 2, PLAYER_POS_RADIUS * 2, 0),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR_WHITE
	);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitPlayer(void)
{
	if (g_pTexBuffPlayer != NULL)
	{// テクスチャの破棄
		g_pTexBuffPlayer->Release();
		g_pTexBuffPlayer = NULL;
	}

	if (g_pVtxBuffPlayer != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdatePlayer(void)
{
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		g_player.nCounterState++;
		switch (g_player.state)
		{
		case PLAYERSTATE_INIT:
			g_player.bIsHold = true;
			GetBullet()->bUse = true;
			g_player.state = PLAYERSTATE_APPEAR;

		case PLAYERSTATE_APPEAR:
			g_player.obj[i].bVisible ^= 1;

			if (g_player.nCounterState > PLAYERSTATE_TIME_APPEAR)
			{
				for (int j = 0; j < PLAYER_NUM; j++)
				{
					g_player.obj[j].bVisible = true;
				}
				SetPlayerState(PLAYERSTATE_NORMAL);
			}
			break;

		case PLAYERSTATE_NORMAL:
			break;

		case PLAYERSTATE_DIED:
			for (int j = 0; j < PLAYER_NUM; j++)
			{
				g_player.obj[j].bVisible = false;
			}

			if (g_player.nCounterState > 60)
			{
				if (g_player.nPlayerLeft < 1)
				{
					SetFade(SCENE_RESULT);
				}
				else
				{
					SetPlayerState(PLAYERSTATE_INIT);
				}
			}

			return;
		}

		g_player.obj[i].rot.z = g_player.fAngle + (D3DX_PI * 2 / PLAYER_NUM * i);

		// 位置更新
		g_player.obj[i].pos =
			D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0) + Direction(g_player.obj[i].rot.z) * PLAYER_POS_RADIUS;
	}

	// 移動処理
	if (GetKeyboardPress(DIK_A) || GetJoypadPress(JOYKEY_LSHOULDER))
	{// 左
		g_player.fAngle += g_player.fSpeed;
	}
	if (GetKeyboardPress(DIK_D) || GetJoypadPress(JOYKEY_RSHOULDER))
	{// 右
		g_player.fAngle -= g_player.fSpeed;
	}

	if (GetKeyboardTrigger(DIK_SPACE) || GetJoypadTrigger(JOYKEY_LSHOULDER))
	{
		SetBullet(
			g_player.obj[0].pos,
			Direction(g_player.obj[0].rot.z)
		);
	}

	
	if (GetKeyboardTrigger(DIK_SPACE))
	{
		g_player.bIsHold = true;
	}
	else if (GetKeyboardRelease(DIK_SPACE))
	{
		g_player.bIsHold = false;
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	for (int i = 0; i < PLAYER_NUM; i++, pVtx += 4)
	{
		SetVertexPos(pVtx, g_player.obj[i]);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, g_player.obj[i].color);
		SetVertexTexturePos(pVtx);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < PLAYER_NUM; i++)
	{
		if (g_player.obj[i].bVisible == true)
		{// ポリゴン描画
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexBuffPlayer);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
		}
	}

}

PLAYER* GetPlayer(void)
{
	return &g_player;
}

void SetPlayerState(PLAYERSTATE state)
{
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		g_player.state = state;
		g_player.nCounterState = 0;
	}
}

void HitPlayer(void)
{
	g_player.nPlayerLeft--;

	SetPlayerState(PLAYERSTATE_DIED);
}
