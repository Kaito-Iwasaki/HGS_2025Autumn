//=====================================================================
// 
// bullet [bullet.cpp]
// Author : SHUMA AIZU
//
//=====================================================================
#include "bullet.h"
#include "util.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define BULLET_TEXTURE_FILENAME	NULL		// テクスチャファイル名

#define INIT_BULLET_SIZE	D3DXVECTOR3 (150, 150, 0.0f)
#define INIT_BULLET_COLOR	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define INIT_BULLET_SPEED	(10.0f)

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;		// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTexBuffBullet = NULL;				// テクスチャへのポインタ
BULLET g_bullet = {};									// 弾の情報

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
	g_bullet.bulletstate = BULLETSTATE_STOP;
	g_bullet.fSpeed = INIT_BULLET_SPEED;
	g_bullet.bUse = false;

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
	if (g_bullet.bUse == true)
	{// 使用されていたら

		// 状態判定
		switch (g_bullet.bulletstate)
		{
		case BULLETSTATE_MOVE:
			// 位置更新
			g_bullet.obj.pos.x += sinf(g_bullet.obj.rot.z) * g_bullet.fSpeed;
			g_bullet.obj.pos.y += cosf(g_bullet.obj.rot.z) * g_bullet.fSpeed;
			break;

		case BULLETSTATE_STOP:
			break;
		}

		// 位置制限と反射
		if (g_bullet.obj.pos.x <= 0 || g_bullet.obj.pos.x >= SCREEN_WIDTH ||
			g_bullet.obj.pos.y <= 0 || g_bullet.obj.pos.x >= SCREEN_HEIGHT)
		{
			Clampf(&g_bullet.obj.pos.x, 0, SCREEN_WIDTH);
			Clampf(&g_bullet.obj.pos.y, 0, SCREEN_HEIGHT);
			g_bullet.obj.rot.z -= D3DX_PI;

			// 向きを補正
			if (g_bullet.obj.rot.z < -D3DX_PI)
			{
				g_bullet.obj.rot.z += D3DX_PI * 2;
			}
			else if (g_bullet.obj.rot.z > D3DX_PI)
			{
				g_bullet.obj.rot.z -= D3DX_PI * 2;
			}
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

	if (g_bullet.bUse == true)
	{
		if (g_bullet.obj.bVisible == true)
		{// ポリゴン描画
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexBuffBullet);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}
}

//=====================================================================
// 弾の設定処理
//=====================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (g_bullet.bUse == false)
	{// 使用されていなければ
		g_bullet.obj.pos = pos;
		g_bullet.obj.rot = rot;
		g_bullet.obj.bVisible = true;
		g_bullet.bUse = true;
	}
}

//=====================================================================
// 敵との当たり判定処理
//=====================================================================
bool CollisionEnemy()
{

}