//=====================================================================
// 
// effect.cpp�̃w�b�_�t�@�C�� [effect.h]
// Author:
//
//=====================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_		// ��d�C���N���[�h�h�~�̃}�N��

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
#define MAX_EFFECT			(1024)

//*********************************************************************
// 
// ***** �񋓌^ *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** �\���� *****
// 
//*********************************************************************
typedef struct
{
	float fSpeed;
	float fRotSpeed;
	float fMaxScale;
	int nMaxLife;
	float fMaxAlpha;
	D3DXCOLOR col;
}EFFECTINFO;

typedef struct
{
	BASEOBJECT obj;
	bool bUsed;
	EFFECTINFO info;
	float fAngle;
	float fScale;
	int nLife;
}EFFECT;

//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, float fSpeed, float fRotSpeed, float fAngle, float fScale, int nLife, D3DXCOLOR col);

#endif