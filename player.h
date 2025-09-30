//=====================================================================
// 
// player.cpp�̃w�b�_�t�@�C�� [player.h]
// Author : Kaito Iwasaki
//
//=====================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "main.h"
#include "baseObject.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define PLAYER_NUM			(2)

//*********************************************************************
// 
// ***** �񋓌^ *****
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
// ***** �\���� *****
// 
//*********************************************************************
//*********************************************************************
// �v���C���[�\����
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
// ***** �v���g�^�C�v�錾*****
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