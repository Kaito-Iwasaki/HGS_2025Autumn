//=====================================================================
//
// ��ʏ��� [baseScene.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "baseScene.h"
#include "sound.h"
#include "input.h"
#include "fade.h"

// �V�[��
#include "title.h"
#include "game.h"
#include "result.h"
#include "endless.h"

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
SCENE g_currentScene = SCENE_TITLE;		// ���݂̃V�[��

//*********************************************************************
// �e�V�[���̏����֐�
//*********************************************************************
SCENEDATA g_scenes[SCENE_MAX] = {
	InitTitle, UninitTitle, UpdateTitle, DrawTitle,			// �^�C�g�����
	InitGame, UninitGame, UpdateGame, DrawGame,				// �Q�[�����
	InitResult, UninitResult, UpdateResult, DrawResult,			// �Q�[�����
	InitEndless, UninitEndless, UpdateEndless, DrawEndless,		// �Q�[�����
};

//=====================================================================
// ����������
//=====================================================================
void InitScene(void)
{
	// �t�F�[�h���V�[���̏���������
	InitFade(g_currentScene);
}

//=====================================================================
// �I������
//=====================================================================
void UninitScene(void)
{
	// �e�V�[���̏I������
	for (int nCount = 0; nCount < SCENE_MAX; nCount++)
	{
		g_scenes[nCount].Uninit();
	}

	// �t�F�[�h�̂̏I������
	UninitFade();
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateScene(void)
{
	// ���݂̃V�[���̍X�V����
	g_scenes[g_currentScene].Update();

	// �t�F�[�h�̍X�V����
	UpdateFade();
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawScene(void)
{
	// ���݂̃V�[���̕`�揈��
	g_scenes[g_currentScene].Draw();

	// �t�F�[�h�̕`�揈��
	DrawFade();
}

//=====================================================================
// �V�[���ݒ菈��
//=====================================================================
SCENE SetScene(SCENE nextScene, bool bStopSound)
{
	// ���݂̃V�[�����I��
	g_scenes[g_currentScene].Uninit();

	// �V�K�V�[����������
	g_scenes[nextScene].Init();

	// �V�K�V�[�������݂̃V�[���ɐݒ�
	g_currentScene = nextScene;

	return g_currentScene;
}

SCENE GetCurrentScene(void)
{
	return g_currentScene;
}
