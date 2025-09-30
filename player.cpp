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

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME	NULL		// テクスチャファイル名

#define INIT_PLAYER_POS		D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0.0f)
#define INIT_PLAYER_SIZE	D3DXVECTOR3 (50.0f, 50.0f, 0.0f)
#define INIT_PLAYER_COLOR	D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)
#define INIT_PLAYER_SPEED	(0.05f)

#define PLAYER_NUM			(2)
#define PLAYER_POS_RADIUS	(250.0f)

#define PLAYERSTATE_TIME_APPEAR		(60)

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffPlayer = NULL;
PLAYER g_player;
BASEOBJECT g_playerObject[PLAYER_NUM];

//=====================================================================
// 初期化処理
//=====================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 構造体の初期化
	memset(&g_player, 0, sizeof(PLAYER) * PLAYER_NUM);
	g_player.fSpeed = INIT_PLAYER_SPEED;
	g_player.state = PLAYERSTATE_APPEAR;

	memset(&g_playerObject[0], 0, sizeof(BASEOBJECT) * PLAYER_NUM);
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		g_playerObject[i].size = INIT_PLAYER_SIZE;
		g_playerObject[i].color = INIT_PLAYER_COLOR;
		g_playerObject[i].bVisible = true;
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
		case PLAYERSTATE_APPEAR:
			g_playerObject[i].bVisible ^= 1;

			if (g_player.nCounterState > PLAYERSTATE_TIME_APPEAR)
			{
				for (int j = 0; j < PLAYER_NUM; j++)
				{
					g_playerObject[j].bVisible = true;
				}
				SetPlayerState(PLAYERSTATE_NORMAL);
			}
			break;

		case PLAYERSTATE_NORMAL:

			break;
		}

		// 移動処理
		if (GetKeyboardPress(DIK_A) || GetJoypadPress(JOYKEY_LSHOULDER))
		{// 左
			g_playerObject[i].rot.z += g_player.fSpeed;
		}
		if (GetKeyboardPress(DIK_D) || GetJoypadPress(JOYKEY_RSHOULDER))
		{// 右
			g_playerObject[i].rot.z -= g_player.fSpeed;
		}

		// 位置更新
		g_playerObject[i].pos = 
			D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0) + Direction(g_playerObject[0].rot.z + (D3DX_PI * 2 / PLAYER_NUM * i)) * PLAYER_POS_RADIUS;
	}

	if (GetKeyboardTrigger(DIK_SPACE) || GetJoypadTrigger(JOYKEY_LSHOULDER))
	{

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
		SetVertexPos(pVtx, g_playerObject[i]);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, g_playerObject[i].color);
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
		if (g_playerObject[i].bVisible == true)
		{// ポリゴン描画
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexBuffPlayer);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
		}
	}

}

void SetPlayerState(PLAYERSTATE state)
{
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		g_player.state = state;
		g_player.nCounterState = 0;
	}
}
