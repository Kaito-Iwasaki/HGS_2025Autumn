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
// ***** �}�N����` *****
// 
//*********************************************************************
#define BULLET_TEXTURE_FILENAME	NULL		// �e�N�X�`���t�@�C����

#define INIT_BULLET_SIZE	D3DXVECTOR3 (150, 150, 0.0f)
#define INIT_BULLET_COLOR	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define INIT_BULLET_SPEED	(10.0f)

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTexBuffBullet = NULL;				// �e�N�X�`���ւ̃|�C���^
BULLET g_bullet = {};									// �e�̏��

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
	g_bullet.bulletstate = BULLETSTATE_STOP;
	g_bullet.fSpeed = INIT_BULLET_SPEED;
	g_bullet.bUse = false;

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
	if (g_bullet.bUse == true)
	{// �g�p����Ă�����

		// ��Ԕ���
		switch (g_bullet.bulletstate)
		{
		case BULLETSTATE_MOVE:
			// �ʒu�X�V
			g_bullet.obj.pos.x += sinf(g_bullet.obj.rot.z) * g_bullet.fSpeed;
			g_bullet.obj.pos.y += cosf(g_bullet.obj.rot.z) * g_bullet.fSpeed;
			break;

		case BULLETSTATE_STOP:
			break;
		}

		// �ʒu�����Ɣ���
		if (g_bullet.obj.pos.x <= 0 || g_bullet.obj.pos.x >= SCREEN_WIDTH ||
			g_bullet.obj.pos.y <= 0 || g_bullet.obj.pos.x >= SCREEN_HEIGHT)
		{
			Clampf(&g_bullet.obj.pos.x, 0, SCREEN_WIDTH);
			Clampf(&g_bullet.obj.pos.y, 0, SCREEN_HEIGHT);
			g_bullet.obj.rot.z -= D3DX_PI;

			// ������␳
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

	if (g_bullet.bUse == true)
	{
		if (g_bullet.obj.bVisible == true)
		{// �|���S���`��
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexBuffBullet);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}
}

//=====================================================================
// �e�̐ݒ菈��
//=====================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (g_bullet.bUse == false)
	{// �g�p����Ă��Ȃ����
		g_bullet.obj.pos = pos;
		g_bullet.obj.rot = rot;
		g_bullet.obj.bVisible = true;
		g_bullet.bUse = true;
	}
}

//=====================================================================
// �G�Ƃ̓����蔻�菈��
//=====================================================================
bool CollisionEnemy()
{

}