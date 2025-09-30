//=====================================================================
// 
// bullet [bullet.cpp]
// Author : SHUMA AIZU
//
//=====================================================================
#include "bullet.h"
#include "player.h"
#include "enemy.h"
#include "input.h"
#include "collision.h"
#include "util.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define BULLET_TEXTURE_FILENAME	"data\\TEXTURE\\BULLET.png"		// �e�N�X�`���t�@�C����

#define INIT_BULLET_SIZE	D3DXVECTOR3 (25, 25, 0.0f)
#define INIT_BULLET_COLOR	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define INIT_BULLET_SPEED	(7.5f)
#define REFLECTION_TIMER	(6)
#define MAX_SPEED			(30.0f)
#define HORUD_DIFF			(25.0f)

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTexBuffBullet = NULL;				// �e�N�X�`���ւ̃|�C���^
BULLET g_bullet = {};									// �e�̏��
bool g_bReflection = false;								// ���˂������ǂ���
bool g_bUseReflection = false;							// ���˂ł��邩�ǂ���
bool g_bUseHorld = false;
int g_nReflectionCount = 0;

//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************


//=====================================================================
// 
// ***** ���������� *****
// 
//=====================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�̎擾

	memset(&g_bullet, 0, sizeof(BASEOBJECT));
	g_bullet.obj.size = INIT_BULLET_SIZE;
	g_bullet.obj.color = INIT_BULLET_COLOR;
	g_bullet.bulletstate = BULLETSTATE_HORLD;
	g_bullet.obj.bVisible = true;
	g_bullet.move = {};
	g_bullet.fSpeed = INIT_BULLET_SPEED;
	g_bullet.nHorldNumber = 0;
	g_bullet.bUse = true;

	g_bUseHorld = true;
	g_bReflection = false;
	g_bUseReflection = true;
	g_nReflectionCount = 0;

	// �e�N�X�`���̓ǂݍ���
	if (BULLET_TEXTURE_FILENAME)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			BULLET_TEXTURE_FILENAME,
			&g_pTexBuffBullet
		);
	}

	// ���_�o�b�t�@�̐���
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
// ***** �I������ *****
// 
//=====================================================================
void UninitBullet(void)
{
	if (g_pTexBuffBullet != NULL)
	{// �e�N�X�`���̔j��
		g_pTexBuffBullet->Release();
		g_pTexBuffBullet = NULL;
	}

	if (g_pVtxBuffBullet != NULL)
	{// ���_�o�b�t�@�̔j��
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=====================================================================
// 
// ***** �X�V���� *****
// 
//=====================================================================
void UpdateBullet(void)
{

	PLAYER* pPlayer = GetPlayer();

	static int nReflectionCounter = REFLECTION_TIMER;	// ���˂̃C���^�[�o��

	if (g_bReflection == false)
	{// �������˕s�Ȃ�
		nReflectionCounter--;							// �J�E���^�[����
		if (nReflectionCounter <= 0)
		{// ���؂�����
			nReflectionCounter = REFLECTION_TIMER;		// �J�E���^�[���Z�b�g
			g_bUseReflection = true;					// ���ˉ\��
		}
	}

	if (g_bullet.bUse == true)
	{// �g�p����Ă�����

		float fAngle = atan2f(g_bullet.move.x, g_bullet.move.y);

		if (g_bReflection == true)
		{

			g_bullet.fSpeed += 0.35f;

			Clampf(&g_bullet.fSpeed, 0, MAX_SPEED);

			g_bReflection = false;
		}

		// ��Ԕ���
		switch (g_bullet.bulletstate)
		{
		case BULLETSTATE_MOVE:
			// �ʒu�X�V
			g_bullet.obj.pos += g_bullet.move;
			CollisionPlayer();
			CollisionEnemy();
			break;

		case BULLETSTATE_HORLD:
			if (pPlayer->bIsHold == true)
			{
				g_bullet.obj.pos.x = pPlayer->obj[g_bullet.nHorldNumber].pos.x + sinf(pPlayer->obj[g_bullet.nHorldNumber].rot.z) * HORUD_DIFF;
				g_bullet.obj.pos.y = pPlayer->obj[g_bullet.nHorldNumber].pos.y + cosf(pPlayer->obj[g_bullet.nHorldNumber].rot.z) * HORUD_DIFF;
				
			}
			else
			{
				g_bUseHorld = false;
				g_bullet.move = Direction(pPlayer->obj[g_bullet.nHorldNumber].rot.z);
				g_bullet.fSpeed = INIT_BULLET_SPEED;
				g_bullet.bulletstate = BULLETSTATE_MOVE;
			}
			break;
		}

		// �ʒu�����Ɣ���
		if (g_bullet.obj.pos.x <= 0 || g_bullet.obj.pos.x >= SCREEN_WIDTH)
		{
			Clampf(&g_bullet.obj.pos.x, 0, SCREEN_WIDTH);
			g_bullet.move.x *= -1;
			g_bUseReflection = true;
			g_bReflection = true;
		}

		if (g_bullet.obj.pos.y <= 0 || g_bullet.obj.pos.y >= SCREEN_HEIGHT)
		{
			Clampf(&g_bullet.obj.pos.y, 0, SCREEN_HEIGHT);
			g_bullet.move.y *= -1;
			g_bUseReflection = true;
			g_bReflection = true;
		}
	}
}

//=====================================================================
// 
// ***** �`�揈�� *****
// 
//=====================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	SetVertexPos(pVtx, g_bullet.obj);
	SetVertexRHW(pVtx, 1.0f);
	SetVertexColor(pVtx, g_bullet.obj.color);
	SetVertexTexturePos(pVtx);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBullet->Unlock();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_bullet.obj.bVisible == true)
	{// �|���S���`��
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexBuffBullet);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	}
}

//=====================================================================
// �e�̐ݒ菈��
//=====================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (g_bullet.bUse == false)
	{// �g�p����Ă��Ȃ����
		g_bullet.obj.pos = pos + (rot * 100);
		g_bullet.obj.rot = rot;

		g_bullet.move.x = rot.x * INIT_BULLET_SPEED;
		g_bullet.move.y = rot.y * INIT_BULLET_SPEED;
		g_bullet.move.z = rot.z;

		g_bullet.bulletstate = BULLETSTATE_HORLD;
		g_bullet.obj.bVisible = true;
		g_bullet.bUse = true;
	}
}

//=====================================================================
// �v���C���[�Ƃ̓����蔻�菈��
//=====================================================================
void CollisionPlayer(void)
{
	PLAYER* pPlayer = GetPlayer();

	for (int nCntPlayer = 0; nCntPlayer < PLAYER_NUM; nCntPlayer++)
	{
		if (g_bUseReflection == true)
		{
			if (CircleCollision(g_bullet.obj.pos, 2.5f, pPlayer->obj[nCntPlayer].pos, pPlayer->obj[nCntPlayer].size.x) == true)
			{
				char aStr[256];

				float fDot = Dot(Vector3To2(Direction(pPlayer->obj[nCntPlayer].rot.z)), Vector3To2(Direction(pPlayer->obj[nCntPlayer].pos, g_bullet.obj.pos)));

				sprintf(&aStr[0], "%f\n", fDot);

				OutputDebugString(&aStr[0]);

				if (fDot < 0.0f)
				{
					HitPlayer();
					g_bullet.bUse = false;
					g_bullet.fSpeed = INIT_BULLET_SPEED;
				}
				else
				{
					if (pPlayer->bIsHold == true)
					{
						g_bullet.bulletstate = BULLETSTATE_HORLD;
						g_bullet.nHorldNumber = nCntPlayer;
					}
					else
					{
						float fAngle = atan2f(g_bullet.obj.pos.x - pPlayer->obj[nCntPlayer].pos.x, g_bullet.obj.pos.y - pPlayer->obj[nCntPlayer].pos.y);

						if (g_bUseReflection == true)
						{
							g_bullet.move = Direction(pPlayer->obj[nCntPlayer].pos, g_bullet.obj.pos) * g_bullet.fSpeed;

							g_bReflection = true;

							g_bUseReflection = false;
						}
					}
				}
			}
		}

	}
}

//=====================================================================
// �G�Ƃ̓����蔻�菈��
//=====================================================================
void CollisionEnemy(void)
{
	ENEMY* pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			float fAngle = atan2f(g_bullet.obj.pos.x - pEnemy->obj.pos.x, g_bullet.obj.pos.y - pEnemy->obj.pos.y);

			if (CircleCollision(g_bullet.obj.pos, g_bullet.obj.size.x / 2, pEnemy->obj.pos, pEnemy->obj.size.x / 2) == true)
			{
				HitEnemy(pEnemy);

				if (pEnemy->bUse == true)
				{
					g_bullet.move = Direction(pEnemy->obj.pos, g_bullet.obj.pos) * g_bullet.fSpeed;

					g_bReflection = true;

					g_bUseReflection = false;
				}

			}
		}
	}
}