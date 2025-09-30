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
#define INIT_PLAYER_SIZE	D3DXVECTOR3 (200, 250, 0.0f)
#define INIT_PLAYER_COLOR	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define INIT_PLAYER_SPEED	(10.0f)

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
	memset(&g_player, 0, sizeof(BASEOBJECT));
	g_player.obj.pos = INIT_PLAYER_POS;
	g_player.obj.size = INIT_PLAYER_SIZE;
	g_player.obj.color = INIT_PLAYER_COLOR;
	g_player.obj.bVisible = true;
	g_player.fSpeed = INIT_PLAYER_SPEED;

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
		sizeof(VERTEX_2D) * 4,
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
	D3DXVECTOR3 direction = D3DXVECTOR3_ZERO;	// 移動量

	// キーボード入力処理
	if (GetKeyboardPress(DIK_A))
	{// 左
		direction.x -= 1;
	}
	if (GetKeyboardPress(DIK_D))
	{// 右
		direction.x += 1;
	}
	if (GetKeyboardPress(DIK_W))
	{// 上
		direction.y -= 1;
	}
	if (GetKeyboardPress(DIK_S))
	{// 下
		direction.y += 1;
	}

	// 位置更新
	g_player.obj.pos += Normalize(direction) * g_player.fSpeed;

	// 位置制限
	Clampf(&g_player.obj.pos.x, 0, SCREEN_WIDTH);
	Clampf(&g_player.obj.pos.y, 0, SCREEN_HEIGHT);
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
	SetVertexPos(pVtx, g_player.obj);
	SetVertexRHW(pVtx, 1.0f);
	SetVertexColor(pVtx, g_player.obj.color);
	SetVertexTexturePos(pVtx);

	// 頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_player.obj.bVisible == true)
	{// ポリゴン描画
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTexBuffPlayer);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}