//===================================================================
//
//	�f�B�X�N����[bullet.cpp]
//	Author:���c ����
//
//===================================================================

//======================================================
//	�C���N���[�h
//======================================================
#include "main.h"
#include "input.h"
#include "bullet.h"
#include "effect.h"
#include "meshfield.h"
#include "player.h"
#include "score.h"
#include "sound.h"

//======================================================
//	�}�N����`
//======================================================
#define MAX_BULLET			(1)			//�e�̍ő吔
#define BULLET_TYPE			(1)			//�e�̎��

//================================================
//	�O���[�o���ϐ�
//================================================
BulletInfo g_BulletInfo;			//�e�̃��f��(X�t�@�C��)���
Bullet g_aBullet[MAX_BULLET];		//�e�̏��

//================================================
//	�e�̏���������
//================================================
void InitBullet(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/MODEL/model/disc000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_BulletInfo.g_pBuffMatModel,
		NULL,
		&g_BulletInfo.g_nNumMatModel,
		&g_BulletInfo.g_pMeshModel);

	//�e���̏�����
	ZeroMemory(&g_aBullet, sizeof(g_aBullet));
}

//======================================================
//	�e�̏I������
//======================================================
void UninitBullet(void)
{
	//���b�V���̔j��
	if (g_BulletInfo.g_pMeshModel != NULL)
	{
		g_BulletInfo.g_pMeshModel->Release();
		g_BulletInfo.g_pMeshModel = NULL;
	}
	//�}�e���A���̔j��
	if (g_BulletInfo.g_pBuffMatModel != NULL)
	{
		g_BulletInfo.g_pBuffMatModel->Release();
		g_BulletInfo.g_pBuffMatModel = NULL;
	}
}

//======================================================
//	�e�̍X�V����
//======================================================
void UpdateBullet(void)
{
	//���ˏ���
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse)
		{//�e���g�p����Ă���ꍇ

			//�e�̈ʒu�̍X�V
			switch (g_aBullet[nCntBullet].nType)
			{
			case 0:
				//�ʒu���X�V����
				g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
				g_aBullet[nCntBullet].pos.z += g_aBullet[nCntBullet].move.z;
				break;

				//case 1:
				//	//�ʒu���X�V����
				//	g_aBullet[nCntBullet].pos.x -= g_aBullet[nCntBullet].move.x;
				//	g_aBullet[nCntBullet].pos.z -= g_aBullet[nCntBullet].move.z;
				//	break;

			default:
				break;
			}

			//�ǁE���̔��ˏ���
			if (g_aBullet[nCntBullet].pos.x <= -FIELD_SIZE_X / 2)
			{//����
				g_aBullet[nCntBullet].move = ReflectionBullet(g_aBullet[nCntBullet].move, D3DXVECTOR3(1.0f, 0.0f, 0.0f));
			}
			else if (g_aBullet[nCntBullet].pos.x >= FIELD_SIZE_X / 2)
			{//�E��
				g_aBullet[nCntBullet].move = ReflectionBullet(g_aBullet[nCntBullet].move, D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
			}
			if (g_aBullet[nCntBullet].pos.z > (FIELD_SIZE_Z / 2) + 30.0f)
			{//����
				SetPoint(PLAYER_1, g_aBullet[nCntBullet].pos);
				g_aBullet[nCntBullet].pos.z = (FIELD_SIZE_Z / 2) + 30.0f;
				g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
			else if (g_aBullet[nCntBullet].pos.z < (-FIELD_SIZE_Z / 2) - 30.0f)
			{//��O��
				SetPoint(PLAYER_2, g_aBullet[nCntBullet].pos);
				g_aBullet[nCntBullet].pos.z = (-FIELD_SIZE_Z / 2) - 30.0f;
				g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}
}

//======================================================
//	�e�̕`�揈��
//======================================================
void DrawBullet(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	//�e�̃��[���h�}�g���b�N�X�ݒ�
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse)
		{//�e���g�p����Ă���ꍇ

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBullet[nCntBullet].rot.y, g_aBullet[nCntBullet].rot.x, g_aBullet[nCntBullet].rot.z);
			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);
		}
	}

	//���݂̃}�e���A����ێ�
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_BulletInfo.g_pBuffMatModel->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)g_BulletInfo.g_nNumMatModel; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//���f���p�[�c�̕`��
		g_BulletInfo.g_pMeshModel->DrawSubset(nCntMat);
	}

	//�ێ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//======================================================
//	�e�̐ݒ菈��
//======================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int Type)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (!g_aBullet[nCntBullet].bUse)
		{//�e���g�p����Ă��Ȃ��ꍇ
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].nType = Type;
			g_aBullet[nCntBullet].bUse = true;

			break;
		}
	}
}

//======================================================
//	�e�̔��ˏ���
//======================================================
D3DXVECTOR3 ReflectionBullet(const D3DXVECTOR3 move, const D3DXVECTOR3 nor)
{
	D3DXVECTOR3 vecMove = nor * -(D3DXVec3Dot(&move, &nor));	//���ς����߂�(�e�̈ړ��x�N�g���ƁA�W���������ǂ̖@���x�N�g��)
	vecMove = move + (vecMove * 2.0f);				//���˃x�N�g�������߂�

	//�T�E���h�Đ�
	PlaySound(SOUND_LABEL_SE_REVERSE);

	return vecMove;
}
//======================================================
//	�e�̏����擾
//======================================================
Bullet *GetBullet(void)
{
	return &g_aBullet[0];
}
