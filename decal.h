//=====================================================================
//
// decal.cpp�̃w�b�_�t�@�C�� [decal.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _DECAL_H_
#define _DECAL_H_

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "baseObject.h"
#include "main.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define MAX_DECAL		(8)

//*********************************************************************
// 
// ***** �񋓌^ *****
// 
//*********************************************************************
typedef enum
{
	DECAL_LABEL_NULL = 0,
	DECAL_LABEL_TITLE,
	DECAL_LABEL_CIRCLE,
	DECAL_LABEL_RESULT,
	DECAL_LABEL_MAX
}DECAL_LABEL;

//*********************************************************************
// 
// ***** �\���� *****
// 
//*********************************************************************
typedef struct
{
	BASEOBJECT obj;
	bool bUsed;
	DECAL_LABEL label;
	int nID;
}DECAL;

//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************
void InitDecal(bool bPreload = false);
void UninitDecal(void);
void DrawDecal(void);
DECAL* GetDecal(void);
DECAL* SetDecal(DECAL_LABEL label, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXCOLOR col);
void DeleteDecal(DECAL* pDecal);
void DeleteDecal(void);
#endif