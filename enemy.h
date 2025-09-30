//=====================================================================
// 
// ENEMY.cppのヘッダファイル [Enemy.h]
// Author:
//
//=====================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_		// 二重インクルード防止のマクロ

#include "main.h"
#include "baseObject.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************

#define MAX_ENEMY			(256)			// 敵の総数

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************

// 敵の発生位置
typedef enum
{
	ENEMY_SPOWN_IN = 0,						// 中心から発散するように出現
	ENEMY_SPOWN_OUT,						// 枠外から中心に収束するように出現
	ENEMY_SPOWN_MAX
}ENEMY_SPOWN;

// 敵の状態
typedef enum
{
	ENEMYSTATE_NORMAL = 0,					// 通常
	ENEMYSTATE_DAMAGE,						// ダメージ状態
	ENEMYSTATE_APPEAR,						// 出現
	ENEMYSTATE_DEATH,						// 死亡
	ENEMYSTATE_MAX
}ENEMYSTATE;

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct
{
	BASEOBJECT obj;							// オブジェクト情報
	D3DXVECTOR3 move;						// 移動量
	ENEMYSTATE state;						// 状態
	ENEMY_SPOWN spown;						// 敵の発生位置
	int nHealth;							// 敵の体力
	int nCounterState;						// 状態カウンター
	bool bUse;								// 敵の使用状況
}ENEMY;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

/*
	敵の配置関数
	pos 配置位置
	move 移動量
	size サイズ
	nHealth 体力
	spown 発生位置
	col 色(基本は白)
	bInversed 反転(基本は未使用)
	rot 回転角度(基本は0)
*/
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, int nHealth, ENEMY_SPOWN spown, D3DXCOLOR col = D3DXCOLOR_WHITE, bool bInversed = false, D3DXVECTOR3 rot = D3DXVECTOR3_ZERO);

/*
	敵の情報の取得
	return 敵の構造体の先頭アドレス
*/
ENEMY *GetEnemy(void);

/*
	敵のダメージ判定
	pEnemy 敵のアドレス
	nDamage ダメージ (基本は1ダメージ)
*/
void HitEnemy(ENEMY *pEnemy, int nDamage = 1);

#endif