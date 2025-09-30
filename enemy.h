//=====================================================================
// 
// ENEMY.cpp�̃w�b�_�t�@�C�� [Enemy.h]
// Author:
//
//=====================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_		// ��d�C���N���[�h�h�~�̃}�N��

#include "main.h"
#include "baseObject.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************

#define MAX_ENEMY			(256)			// �G�̑���

//*********************************************************************
// 
// ***** �񋓌^ *****
// 
//*********************************************************************

// �G�̔����ʒu
typedef enum
{
	ENEMY_SPOWN_IN = 0,						// ���S���甭�U����悤�ɏo��
	ENEMY_SPOWN_OUT,						// �g�O���璆�S�Ɏ�������悤�ɏo��
	ENEMY_SPOWN_MAX
}ENEMY_SPOWN;

// �G�̏��
typedef enum
{
	ENEMYSTATE_NORMAL = 0,					// �ʏ�
	ENEMYSTATE_DAMAGE,						// �_���[�W���
	ENEMYSTATE_APPEAR,						// �o��
	ENEMYSTATE_DEATH,						// ���S
	ENEMYSTATE_MAX
}ENEMYSTATE;

//*********************************************************************
// 
// ***** �\���� *****
// 
//*********************************************************************
typedef struct
{
	BASEOBJECT obj;							// �I�u�W�F�N�g���
	D3DXVECTOR3 move;						// �ړ���
	ENEMYSTATE state;						// ���
	ENEMY_SPOWN spown;						// �G�̔����ʒu
	int nHealth;							// �G�̗̑�
	int nCounterState;						// ��ԃJ�E���^�[
	bool bUse;								// �G�̎g�p��
}ENEMY;

//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

/*
	�G�̔z�u�֐�
	pos �z�u�ʒu
	move �ړ���
	size �T�C�Y
	nHealth �̗�
	spown �����ʒu
	col �F(��{�͔�)
	bInversed ���](��{�͖��g�p)
	rot ��]�p�x(��{��0)
*/
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, int nHealth, ENEMY_SPOWN spown, D3DXCOLOR col = D3DXCOLOR_WHITE, bool bInversed = false, D3DXVECTOR3 rot = D3DXVECTOR3_ZERO);

/*
	�G�̏��̎擾
	return �G�̍\���̂̐擪�A�h���X
*/
ENEMY *GetEnemy(void);

/*
	�G�̃_���[�W����
	pEnemy �G�̃A�h���X
	nDamage �_���[�W (��{��1�_���[�W)
*/
void HitEnemy(ENEMY *pEnemy, int nDamage = 1);

#endif