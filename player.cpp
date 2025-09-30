//=====================================================================
// 
// �v���C���[���� [player.cpp]
// Author : Kaito Iwasaki
//
//=====================================================================

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "player.h"
#include "input.h"
#include "util.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define TEXTURE_FILENAME	NULL		// �e�N�X�`���t�@�C����

#define INIT_PLAYER_POS		D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0.0f)
#define INIT_PLAYER_SIZE	D3DXVECTOR3 (50.0f, 50.0f, 0.0f)
#define INIT_PLAYER_COLOR	D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)
#define INIT_PLAYER_SPEED	(0.05f)

#define PLAYER_NUM			(2)
#define PLAYER_POS_RADIUS	(250.0f)

#define PLAYERSTATE_TIME_APPEAR		(60)

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffPlayer = NULL;
PLAYER g_player;
BASEOBJECT g_playerObject[PLAYER_NUM];

//=====================================================================
// ����������
//=====================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �\���̂̏�����
	memset(&g_player, 0, sizeof(PLAYER) * PLAYER_NUM);
	g_player.fSpeed = INIT_PLAYER_SPEED;
	g_player.state = PLAYERSTATE_APPEAR;

	memset(&g_playerObject[0], 0, sizeof(BASEOBJECT) * PLAYER_NUM);
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		g_playerObject[i].size = INIT_PLAYER_SIZE;
		g_playerObject[i].color = INIT_PLAYER_COLOR;
		g_playerObject[i].bVisible = true;
	}

	// �e�N�X�`���̓ǂݍ���
	if (TEXTURE_FILENAME)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffPlayer
		);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * PLAYER_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL
	);
}

//=====================================================================
// �I������
//=====================================================================
void UninitPlayer(void)
{
	if (g_pTexBuffPlayer != NULL)
	{// �e�N�X�`���̔j��
		g_pTexBuffPlayer->Release();
		g_pTexBuffPlayer = NULL;
	}

	if (g_pVtxBuffPlayer != NULL)
	{// ���_�o�b�t�@�̔j��
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdatePlayer(void)
{
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		g_player.nCounterState++;
		switch (g_player.state)
		{
		case PLAYERSTATE_APPEAR:
			g_playerObject[i].bVisible ^= 1;

			if (g_player.nCounterState > PLAYERSTATE_TIME_APPEAR)
			{
				for (int j = 0; j < PLAYER_NUM; j++)
				{
					g_playerObject[j].bVisible = true;
				}
				SetPlayerState(PLAYERSTATE_NORMAL);
			}
			break;

		case PLAYERSTATE_NORMAL:

			break;
		}

		// �ړ�����
		if (GetKeyboardPress(DIK_A) || GetJoypadPress(JOYKEY_LSHOULDER))
		{// ��
			g_playerObject[i].rot.z += g_player.fSpeed;
		}
		if (GetKeyboardPress(DIK_D) || GetJoypadPress(JOYKEY_RSHOULDER))
		{// �E
			g_playerObject[i].rot.z -= g_player.fSpeed;
		}

		// �ʒu�X�V
		g_playerObject[i].pos = 
			D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0) + Direction(g_playerObject[0].rot.z + (D3DX_PI * 2 / PLAYER_NUM * i)) * PLAYER_POS_RADIUS;
	}

	if (GetKeyboardTrigger(DIK_SPACE) || GetJoypadTrigger(JOYKEY_LSHOULDER))
	{

	}

}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	for (int i = 0; i < PLAYER_NUM; i++, pVtx += 4)
	{
		SetVertexPos(pVtx, g_playerObject[i]);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, g_playerObject[i].color);
		SetVertexTexturePos(pVtx);
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffPlayer->Unlock();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < PLAYER_NUM; i++)
	{
		if (g_playerObject[i].bVisible == true)
		{// �|���S���`��
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexBuffPlayer);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
		}
	}

}

void SetPlayerState(PLAYERSTATE state)
{
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		g_player.state = state;
		g_player.nCounterState = 0;
	}
}
