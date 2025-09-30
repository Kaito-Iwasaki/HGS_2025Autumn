//=====================================================================
// 
// particle [particle.cpp]
// Author : Kaito Iwasaki
//
//=====================================================================
#include "particle.h"
#include "util.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
PARTICLE g_aParticle[MAX_PARTICLE];

//=====================================================================
// 
// ***** ���������� *****
// 
//=====================================================================
void InitParticle(void)
{
	memset(&g_aParticle[0], 0, sizeof(PARTICLE) * MAX_PARTICLE);
}

//=====================================================================
// 
// ***** �I������ *****
// 
//=====================================================================
void UninitParticle(void)
{
	memset(&g_aParticle[0], 0, sizeof(PARTICLE) * MAX_PARTICLE);
}

//=====================================================================
// 
// ***** �X�V���� *****
// 
//=====================================================================
void UpdateParticle(void)
{
	PARTICLE* pParticle = &g_aParticle[0];

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pParticle++)
	{
		if (pParticle->bUsed == false) continue;

		if (pParticle->nLife < 1)
		{
			pParticle->bUsed = false;
			continue;
		}

		pParticle->nLife--;
		for (int nCntEffect = 0; nCntEffect < pParticle->nNumEffect; nCntEffect++)
		{
			SetEffect(
				pParticle->obj.pos,
				pParticle->info.fSpeed,
				pParticle->info.fRotSpeed,
				RandRange(
					(pParticle->fAngle - pParticle->fRange) * 100,
					(pParticle->fAngle + pParticle->fRange) * 100
				) * 0.01f,
				pParticle->info.fMaxScale,
				pParticle->info.nMaxLife,
				pParticle->info.col
			);
		}
	}
}

//=====================================================================
// 
// ***** �p�[�e�B�N���ݒ菈�� *****
// 
//=====================================================================
void SetParticle(EFFECTINFO info, D3DXVECTOR3 pos, float fAngle, float fRange, int nLife, int nNumEffect)
{
	PARTICLE* pParticle = &g_aParticle[0];

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pParticle++)
	{
		if (pParticle->bUsed == true) continue;

		pParticle->bUsed = true;

		pParticle->obj.pos = pos;
		pParticle->info = info;
		pParticle->fAngle = fAngle;
		pParticle->fRange = fRange;
		pParticle->nLife = nLife;
		pParticle->nNumEffect = nNumEffect;

		break;
	}
}