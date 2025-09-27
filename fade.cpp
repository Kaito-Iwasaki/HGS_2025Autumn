//=====================================================================
// 
// �t�F�[�h���� [fade.cpp]
// Author:���O��
//
//=====================================================================

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "fade.h"
#include "input.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define INIT_COLOR			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	// �t�F�[�h�F
#define FADE_SCALE			(0.025f)	// �t�F�[�h���x

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;
FADE g_fade;

//=====================================================================
// ����������
//=====================================================================
void InitFade(SCENE sceneNext)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �t�F�[�h�\���̂̏�����
	memset(&g_fade, 0, sizeof(FADE));
	g_fade.obj.pos = D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0.0f);
	g_fade.obj.size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	g_fade.obj.color = INIT_COLOR;
	g_fade.state = FADESTATE_IN;
	g_fade.sceneNext = sceneNext;
	g_fade.fFadeScale = FADE_SCALE;
	g_fade.bStopSound = false;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL
	);

	SetScene(sceneNext);
}

//=====================================================================
// �I������
//=====================================================================
void UninitFade(void)
{
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateFade(void)
{
	if (g_fade.state != FADESTATE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			if (g_fade.obj.color.a > 0 && g_fade.obj.color.a < 1)
			{// Enter�L�[�������ꂽ�瑁������
				g_fade.fFadeScale = FADE_SCALE * 2;
			}
		}

		if (g_fade.state == FADESTATE_IN)
		{// �t�F�[�h�C��
			g_fade.obj.color.a -= g_fade.fFadeScale;

			if (g_fade.obj.color.a <= 0.0f)
			{// �t�F�[�h�C���I��
				g_fade.obj.color.a = 0.0f;
				g_fade.state = FADESTATE_NONE;
				g_fade.fFadeScale = FADE_SCALE;

			}
		}
		else if (g_fade.state == FADESTATE_OUT)
		{// �t�F�[�h�A�E�g
			g_fade.obj.color.a += g_fade.fFadeScale;;

			if (g_fade.obj.color.a >= 1.0f)
			{// �t�F�[�h�A�E�g&�t�F�[�h�C���ֈڍs
				g_fade.obj.color.a = 1.0f;
				g_fade.state = FADESTATE_IN;

				SetScene(g_fade.sceneNext, g_fade.bStopSound);
			}
		}
	}
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	SetVertexPos(pVtx, g_fade.obj);
	SetVertexRHW(pVtx, 1.0f);
	SetVertexColor(pVtx, g_fade.obj.color);
	SetVertexTexturePos(pVtx);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffFade->Unlock();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=====================================================================
// �t�F�[�h�؂�ւ�����
//=====================================================================
void SetFade(SCENE sceneNext, bool bStopSound)
{
	if (g_fade.state == FADESTATE_NONE)
	{
		g_fade.state = FADESTATE_OUT;
		g_fade.sceneNext = sceneNext;
		g_fade.fFadeScale = FADE_SCALE;
		g_fade.bStopSound = bStopSound;
	}
}

//=====================================================================
// ���݂̃t�F�[�h��Ԃ��擾
//=====================================================================
FADE GetFade(void)
{
	return g_fade;
}