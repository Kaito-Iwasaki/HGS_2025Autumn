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
#define INIT_PLAYER_SIZE	D3DXVECTOR3 (200, 250, 0.0f)
#define INIT_PLAYER_COLOR	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define INIT_PLAYER_SPEED	(10.0f)

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffPlayer = NULL;
PLAYER g_player;

//=====================================================================
// ����������
//=====================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �\���̂̏�����
	memset(&g_player, 0, sizeof(BASEOBJECT));
	g_player.obj.pos = INIT_PLAYER_POS;
	g_player.obj.size = INIT_PLAYER_SIZE;
	g_player.obj.color = INIT_PLAYER_COLOR;
	g_player.obj.bVisible = true;
	g_player.fSpeed = INIT_PLAYER_SPEED;

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
		sizeof(VERTEX_2D) * 4,
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
	D3DXVECTOR3 direction = D3DXVECTOR3_ZERO;	// �ړ���

	// �L�[�{�[�h���͏���
	if (GetKeyboardPress(DIK_A))
	{// ��
		direction.x -= 1;
	}
	if (GetKeyboardPress(DIK_D))
	{// �E
		direction.x += 1;
	}
	if (GetKeyboardPress(DIK_W))
	{// ��
		direction.y -= 1;
	}
	if (GetKeyboardPress(DIK_S))
	{// ��
		direction.y += 1;
	}

	// �ʒu�X�V
	g_player.obj.pos += Normalize(direction) * g_player.fSpeed;

	// �ʒu����
	Clampf(&g_player.obj.pos.x, 0, SCREEN_WIDTH);
	Clampf(&g_player.obj.pos.y, 0, SCREEN_HEIGHT);
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
	SetVertexPos(pVtx, g_player.obj);
	SetVertexRHW(pVtx, 1.0f);
	SetVertexColor(pVtx, g_player.obj.color);
	SetVertexTexturePos(pVtx);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffPlayer->Unlock();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_player.obj.bVisible == true)
	{// �|���S���`��
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexBuffPlayer);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}