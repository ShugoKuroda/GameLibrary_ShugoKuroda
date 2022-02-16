#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//�v���g�^�C�v�錾
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, int Type);

//�}�N����`
#define MAX_BULLET			(128)		//�e�̍ő吔
#define BULLET_SIZE			(15.0f)		//�e�̃T�C�Y
#define BULLET_TYPE			(2)			//�e�̎��
#define BULLET_MOVE			(6.0f)		//�e�̈ړ���

//�e�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;				//�ʒu
	D3DXVECTOR3 move;				//�ړ���
	bool bUse;						//�g�p���Ă��邩�ǂ���
	int nType;						//���
	int g_nCounterAnimBullet;		//�A�j���[�V�����J�E���^�[
	int g_nPatternAnimBullet;		//�A�j���[�V�����̃p�^�[��No.
}Bullet;

#endif
