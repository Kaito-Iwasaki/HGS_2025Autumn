//=====================================================================
// 
// player.cppのヘッダファイル [player.h]
// Author : Kaito Iwasaki
//
//=====================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "main.h"
#include "baseObject.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define PLAYER_NUM			(2)

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{
	PLAYERSTATE_INIT = 0,
	PLAYERSTATE_APPEAR,
	PLAYERSTATE_NORMAL,
	PLAYERSTATE_HOLD,
	PLAYERSTATE_DAMAGED,
	PLAYERSTATE_DIED,
	PLAYERSTATE_END,
	PLAYERSTATE_MAX
}PLAYERSTATE;

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
//*********************************************************************
// プレイヤー構造体
//*********************************************************************
typedef struct
{
	BASEOBJECT obj[PLAYER_NUM];
	float fSpeed;
	float fAngle;
	PLAYERSTATE state;
	int nCounterState;
	int nCountHit;
	int nPlayerLeft;
	bool bIsHold;
}PLAYER;

//*********************************************************************
// 
// ***** プロトタイプ宣言*****
// 
//*********************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER* GetPlayer(void);
void SetPlayerState(PLAYERSTATE state);
void HitPlayer(void);

#endif