//=====================================================================
//
// baseObject.cpp�̃w�b�_�t�@�C�� [baseObject.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _BASEOBJECT_H_
#define _BASEOBJECT_H_

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "main.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define OOS_TOP			(0x0001)	// ��ʊO����i��j
#define OOS_BOTTOM		(0x0002)	// ��ʊO����i���j
#define OOS_LEFT		(0x0004)	// ��ʊO����i���j
#define OOS_RIGHT		(0x0008)	// ��ʊO����i�E�j
#define OOS_ALL			OOS_TOP | OOS_BOTTOM | OOS_LEFT | OOS_RIGHT	// ��ʊO����i�S�āj

//*********************************************************************
// 
// ***** �\���� *****
// 
//*********************************************************************
//*********************************************************************
// �`��I�u�W�F�N�g
//*********************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ��]
	D3DXVECTOR3 size;		// �T�C�Y
	D3DXCOLOR	color;		// �F
	bool bVisible;			// �\�����
	bool bInversed;			// �e�N�X�`�����]
}BASEOBJECT;

//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************
void SetVertexPos(VERTEX_2D* pVtx, BASEOBJECT obj);
void SetVertexPos(VERTEX_2D* pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 size, float fRot = 0.0f);
void SetVertexPos(VERTEX_2D* pVtx, D3DXVECTOR3 leftTop, D3DXVECTOR3 rightTop, D3DXVECTOR3 leftBottom, D3DXVECTOR3 rightBottom);
void SetVertexRHW(VERTEX_2D* pVtx, float rhw);
void SetVertexColor(VERTEX_2D* pVtx, D3DXCOLOR col);
void SetVertexTexturePos(VERTEX_2D* pVtx, bool bInversed = false);
void SetVertexTexturePos(VERTEX_2D* pVtx, D3DXVECTOR2 leftTop, D3DXVECTOR2 rightTop, D3DXVECTOR2 leftBottom, D3DXVECTOR2 rightBottom, bool bInversed = false);
void SetVertexTexturePos(VERTEX_2D* pVtx, int nTexture, int nMaxTexture, bool bInversed = false);
bool IsObjectOutOfScreen(BASEOBJECT obj, DWORD flags = OOS_ALL);
bool IsObjectOutOfScreen(BASEOBJECT obj, RECT rect, DWORD flags = OOS_ALL);

#endif
