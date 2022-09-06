//===================================================================
//
//	�f�B�X�N����[bullet.h]
//	Author:���c ����
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
#define BULLET_MOVE			(6.0f)		//�e�̈ړ���

//======================================================
//	�e�̍\���̒�`
//======================================================
//�e�̃��f��(X�t�@�C��)���
typedef struct
{
	LPD3DXMESH g_pMeshModel = NULL;			//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER g_pBuffMatModel = NULL;	//�}�e���A�����ւ̃|�C���^
	DWORD g_nNumMatModel = 0;				//�}�e���A�����̐�
}BulletInfo;

//�e�̏��
typedef struct
{
	D3DXVECTOR3 pos;				//�ʒu
	D3DXVECTOR3 rot;				//��]
	D3DXVECTOR3 move;				//�ړ���
	D3DXMATRIX mtxWorld;			//���[���h�}�g���b�N�X
	bool bUse;						//�g�p���Ă��邩�ǂ���
	float fRotCurve;				//�e�̃J�[�u�p�x
	int nType;						//���
}Bullet;

//======================================================
//	�v���g�^�C�v�錾
//======================================================
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int Type);
D3DXVECTOR3 ReflectionBullet(const D3DXVECTOR3 move, const D3DXVECTOR3 nor);
Bullet *GetBullet(void);

#endif