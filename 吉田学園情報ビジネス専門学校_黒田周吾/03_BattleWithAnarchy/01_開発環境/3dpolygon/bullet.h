//===================================================================
//
//		�e�̏���[bullet.h]
//		Author:���c ����
//
//===================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

//======================================================
//	�C���N���[�h
//======================================================
#include "main.h"

//======================================================
//	�}�N����`
//======================================================
#define BULLET_MOVE			(10.0f)		//�e�̈ړ���

//======================================================
//	�e�̍\���̒�`
//======================================================
typedef struct
{
	D3DXVECTOR3 pos;				//�ʒu
	D3DXVECTOR3 rot;				//��]
	D3DXVECTOR3 move;				//�ړ���
	D3DXMATRIX mtxWorld;			//���[���h�}�g���b�N�X
	bool bUse;						//�g�p���Ă��邩�ǂ���
	int nType;						//���
	int nLife;						//����
	int g_nCounterAnimBullet;		//�A�j���[�V�����J�E���^�[
	int g_nPatternAnimBullet;		//�A�j���[�V�����̃p�^�[��No.
}Bullet;

//======================================================
//	�v���g�^�C�v�錾
//======================================================
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int Type);

#endif