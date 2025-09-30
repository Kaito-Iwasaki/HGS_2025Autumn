//=====================================================================
// 
// enemy [Enemy.cpp]
// Author:
//
//=====================================================================
#include "enemy.h"
#include "baseObject.h"
#include "util.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************

#define ENEMY_SPD				(3.0f)		// �ړ����x
#define OUT_RECT				(10)		// ��ʊO����o������G�̏�����͈�
#define COUNTERSTATE_APPEAR		(60)		// �o����ԃJ�E���^�[
#define COUNTERSTATE_DAMAGE		(5)			// �_���[�W��ԃJ�E���^�[

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffEnemy = NULL;
ENEMY g_aEnemy[MAX_ENEMY] = {};				// �G�̍\����

//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************

void SpownOutOfScreen(ENEMY *pEnemy);

//=====================================================================
// 
// ***** ���������� *****
// 
//=====================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *pEnemy = &g_aEnemy[0];			// �G�̐擪�A�h���X

	// �G�̍\���̂̏�����
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		pEnemy->obj.pos = D3DXVECTOR3_ZERO;
		pEnemy->obj.rot = D3DXVECTOR3_ZERO;
		pEnemy->obj.size = D3DXVECTOR3_ZERO;
		pEnemy->obj.color = D3DXCOLOR_WHITE;
		pEnemy->obj.bVisible = false;
		pEnemy->obj.bInversed = false;
		pEnemy->move = D3DXVECTOR3_ZERO;
		pEnemy->state = ENEMYSTATE_APPEAR;
		pEnemy->spown = ENEMY_SPOWN_IN;
		pEnemy->nHealth = 0;
		pEnemy->nCounterState = 0;
		pEnemy->bUse = false;
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL
	);
}

//=====================================================================
// 
// ***** �I������ *****
// 
//=====================================================================
void UninitEnemy(void)
{
	if (g_pTexBuffEnemy != NULL)
	{// �e�N�X�`���̔j��
		g_pTexBuffEnemy->Release();
		g_pTexBuffEnemy = NULL;
	}

	if (g_pVtxBuffEnemy != NULL)
	{// ���_�o�b�t�@�̔j��
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//=====================================================================
// 
// ***** �X�V���� *****
// 
//=====================================================================
void UpdateEnemy(void)
{
	ENEMY* pEnemy = &g_aEnemy[0];			// �G�̐擪�A�h���X
	RECT rect = { SCREEN_CENTER - OUT_RECT , SCREEN_VCENTER - OUT_RECT, SCREEN_WIDTH + OUT_RECT, SCREEN_VCENTER + OUT_RECT };

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			switch (pEnemy->spown)
			{
			case ENEMY_SPOWN_IN:
				
				if (IsObjectOutOfScreen(pEnemy->obj))
				{
					pEnemy->bUse = false;
					pEnemy->obj.bVisible = false;
				}

				break;

			case ENEMY_SPOWN_OUT:

				if (IsObjectOutOfScreen(pEnemy->obj, rect) == false)
				{
					pEnemy->bUse = false;
					pEnemy->obj.bVisible = false;
				}

				break;

			default:

				if (IsObjectOutOfScreen(pEnemy->obj))
				{
					pEnemy->bUse = false;
					pEnemy->obj.bVisible = false;
				}

				break;
			}

			pEnemy->obj.pos += pEnemy->move;
		}
	}
}

//=====================================================================
// 
// ***** �`�揈�� *****
// 
//=====================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;
	ENEMY *pEnemy = &g_aEnemy[0];			// �G�̐擪�A�h���X

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�o�b�t�@�̐ݒ�
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{ // �G���g�p����Ă���ΐݒ�
			// ���_����ݒ�
			SetVertexPos(pVtx, pEnemy->obj);			// �ʒu�ݒ�
			SetVertexRHW(pVtx, 1.0f);					// RHW�ݒ�
			SetVertexColor(pVtx, pEnemy->obj.color);	// �F�ݒ�
			SetVertexTexturePos(pVtx);					// �e�N�X�`�����W�ݒ�
		}

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffEnemy->Unlock();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pEnemy = &g_aEnemy[0];			// �G�̐擪�A�h���X

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true && pEnemy->obj.bVisible == true)
		{// �|���S���`��
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntEnemy, 2);
		}
	}
}

//=====================================================================
// 
// ***** �G�̔z�u���� *****
// 
//=====================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, int nHealth, ENEMY_SPOWN spown, D3DXCOLOR col, bool bInversed, D3DXVECTOR3 rot)
{
	ENEMY *pEnemy = &g_aEnemy[0];			// �G�̐擪�A�h���X
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{ // �G���g���Ă��Ȃ���ΐݒ� 
			pEnemy->obj.size = size;
			pEnemy->obj.rot = rot;
			pEnemy->obj.color = col;
			pEnemy->obj.bInversed = bInversed;
			pEnemy->obj.bVisible = true;
			pEnemy->state = ENEMYSTATE_APPEAR;
			pEnemy->spown = spown;
			switch (pEnemy->spown)
			{
			case ENEMY_SPOWN_IN:
				pEnemy->obj.pos = D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0.0f);

				pEnemy->move.z = (float)(RandRange(-(int)(D3DX_PI * 100.0f), (int)(D3DX_PI * 100.0f)) * 0.01f);

				pEnemy->move.x = sinf(pEnemy->move.z) * ENEMY_SPD;
				pEnemy->move.y = cosf(pEnemy->move.z) * ENEMY_SPD;

				break;

			case ENEMY_SPOWN_OUT:

				SpownOutOfScreen(pEnemy);

				pEnemy->move.z = Angle(pEnemy->obj.pos, D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0.0f));

				pEnemy->move.x = sinf(pEnemy->move.z) * ENEMY_SPD;
				pEnemy->move.y = cosf(pEnemy->move.z) * ENEMY_SPD;

				break;

			default:

				pEnemy->obj.pos = pos;
				pEnemy->move = move;

				break;
			}

			pEnemy->nHealth = nHealth;
			pEnemy->nCounterState = COUNTERSTATE_APPEAR;
			pEnemy->bUse = true;

			// ���_����ݒ�
			SetVertexPos(pVtx, pEnemy->obj);			// �ʒu�ݒ�
			SetVertexRHW(pVtx, 1.0f);					// RHW�ݒ�
			SetVertexColor(pVtx, pEnemy->obj.color);	// �F�ݒ�
			SetVertexTexturePos(pVtx);					// �e�N�X�`�����W�ݒ�

			break;
		}

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffEnemy->Unlock();
}

//=====================================================================
// 
// ***** �G�̏��̎擾 *****
// 
//=====================================================================
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//=====================================================================
// 
// ***** �G�̃_���[�W���� *****
// 
//=====================================================================
void HitEnemy(ENEMY* pEnemy, int nDamage)
{
	pEnemy->nHealth -= nDamage;				// �G�̗̑͂��_���[�W������
	if (pEnemy->nHealth <= 0)
	{ // �̗͂�0�ȉ��ɂȂ�����
		pEnemy->obj.bVisible = false;		// �s����Ԃ�
		pEnemy->bUse = false;				// ���g�p��
	}
	else
	{ // �̗͂�0�����傫�����
		pEnemy->state = ENEMYSTATE_DAMAGE;				// �_���[�W��Ԃ�
		pEnemy->nCounterState = COUNTERSTATE_DAMAGE;	// �J�E���^�[�Z�b�g
	}
}

//=====================================================================
// 
// ***** �G�̉�ʊO�o������ *****
// 
//=====================================================================
void SpownOutOfScreen(ENEMY* pEnemy)
{
	D3DXVECTOR3 pos;

	// �o���ʒu��ݒ�
	int n = rand() % SCREEN_WIDTH;

	pos.x = (float)n;

	if (n == 0 || n == SCREEN_WIDTH)
	{
		pos.y = (float)(rand() % SCREEN_HEIGHT);
	}
	else
	{
		if (rand() % 2 == 0)
		{
			pos.y = 0.0f;
		}
		else
		{
			pos.y = (float)SCREEN_HEIGHT;
		}
	}
	
	pEnemy->obj.pos = pos;
}