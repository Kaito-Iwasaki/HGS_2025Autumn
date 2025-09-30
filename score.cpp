//=====================================================================
// 
// score [score.cpp]
// Author : Kaito Iwasaki
//
//=====================================================================
#include "score.h"
#include "util.h"
#include "baseScene.h"
#include "sound.h"
#include "font.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define TEXTURE_FILENAME		"data\\TEXTURE\\number000.png"
#define TEXTURE_MAX_X			(10)

#define INIT_POS				{0.0f, 0.0f, 0.0f}
#define INIT_SIZE				{200.0f, 50.0f, 0.0f}
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

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
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
SCORE g_score;

//=====================================================================
// 
// ***** ���������� *****
// 
//=====================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_FILENAME, &g_pTextureScore);

	// �X�R�A�̏��̏�����
	memset(&g_score, 0, sizeof(SCORE));
	g_score.obj.pos = INIT_POS;
	g_score.obj.size = INIT_SIZE;
	g_score.obj.color = INIT_COLOR;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL
	);
}

//=====================================================================
// 
// ***** �I������ *****
// 
//=====================================================================
void UninitScore(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//=====================================================================
// 
// ***** �X�V���� *****
// 
//=====================================================================
void UpdateScore(void)
{
	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		g_score.aTexPattern[nCntScore] = g_score.nScore % (int)pow(10, (double)NUM_PLACE - nCntScore) / (int)pow(10, (double)NUM_PLACE - 1 - nCntScore);
	}
}

//=====================================================================
// 
// ***** �`�揈�� *****
// 
//=====================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < NUM_PLACE; nCount++, pVtx += 4)
	{
		// ���_����ݒ�
		SetVertexPos(pVtx,
			D3DXVECTOR3(g_score.obj.pos.x + (nCount * g_score.obj.size.x / TEXTURE_MAX_X), g_score.obj.pos.y, 0.0f),
			D3DXVECTOR3(g_score.obj.pos.x + ((nCount + 1) * g_score.obj.size.x / TEXTURE_MAX_X), g_score.obj.pos.y, 0.0f),
			D3DXVECTOR3(g_score.obj.pos.x + (nCount * g_score.obj.size.x / TEXTURE_MAX_X), g_score.obj.pos.y + g_score.obj.size.y, 0.0f),
			D3DXVECTOR3(g_score.obj.pos.x + ((nCount + 1) * g_score.obj.size.x / TEXTURE_MAX_X), g_score.obj.pos.y + g_score.obj.size.y, 0.0f)
			);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, g_score.obj.color);
		SetVertexTexturePos(pVtx,
			D3DXVECTOR2(1.0f / TEXTURE_MAX_X * (g_score.aTexPattern[nCount]), 0.0f),
			D3DXVECTOR2(1.0f / TEXTURE_MAX_X * (g_score.aTexPattern[nCount] + 1), 0.0f),
			D3DXVECTOR2(1.0f / TEXTURE_MAX_X * (g_score.aTexPattern[nCount]), 1.0f),
			D3DXVECTOR2(1.0f / TEXTURE_MAX_X * (g_score.aTexPattern[nCount] + 1), 1.0f)
			);
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffScore->Unlock();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureScore);

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}

void SetScore(int nScore)
{
	g_score.nScore = nScore;
}

void AddScore(int nValue)
{
	g_score.nScore += nValue;
}

int GetScore(void)
{
	return g_score.nScore;
}
