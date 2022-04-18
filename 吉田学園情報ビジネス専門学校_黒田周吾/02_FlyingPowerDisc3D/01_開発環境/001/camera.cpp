//===================================================================
//
//	�J��������[camera.cpp]
//	Author:���c ����
//
//===================================================================

//======================================================
//	�C���N���[�h
//======================================================
#include "main.h"
#include "camera.h"
#include "input.h"
#include "player.h"

//======================================================
//	�O���[�o���ϐ�
//======================================================
Camera g_camera[MAX_CAMERA];	//�J�������

//======================================================
//	�J�����̏���������
//======================================================
void InitCamera(void)
{
	float fAngle = 0.0f;

	for (int nCntCamera = 0; nCntCamera < MAX_CAMERA; nCntCamera++, fAngle += D3DX_PI)
	{
		//���_�E�����_�E�������ݒ肷��
		g_camera[nCntCamera].posV = D3DXVECTOR3(0.0f, 130.0f, -230.0f);
		g_camera[nCntCamera].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera[nCntCamera].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//�Œ�
		g_camera[nCntCamera].posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera[nCntCamera].posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�����_�܂ł̋���
		D3DXVECTOR3 Distance = D3DXVECTOR3((g_camera[nCntCamera].posV.x + g_camera[nCntCamera].posR.x),
			(g_camera[nCntCamera].posV.y + g_camera[nCntCamera].posR.y),
			(g_camera[nCntCamera].posV.z + g_camera[nCntCamera].posR.z));

		//�����E���_���璍���_�܂ł̋���������������
		g_camera[nCntCamera].rot = D3DXVECTOR3(atan2f(Distance.z, Distance.y), fAngle, 0.0f);
		g_camera[nCntCamera].fDistance = sqrtf(Distance.x * Distance.x + Distance.z * Distance.z);
		g_camera[nCntCamera].fDistance = sqrtf(Distance.y * Distance.y + g_camera[nCntCamera].fDistance * g_camera[nCntCamera].fDistance);
	}
}

//======================================================
//	�J�����̏I������
//======================================================
void UninitCamera(void)
{

}

//======================================================
//	�J�����̍X�V����
//======================================================
void UpdateCamera(void)
{
	//�v���C���[���̎擾
	Player *pPlayer = GetPlayer();

	for (int nCntCamera = 0; nCntCamera < MAX_CAMERA; nCntCamera++, pPlayer++)
	{
		//�v���C���[�Ǐ]
		g_camera[nCntCamera].posRDest.x = (pPlayer->pos.x) + sinf(pPlayer->rot.x) * sinf(pPlayer->rot.y);
		g_camera[nCntCamera].posRDest.z = (pPlayer->pos.z) + sinf(pPlayer->rot.x) * cosf(pPlayer->rot.y);
		g_camera[nCntCamera].posRDest.y = (pPlayer->pos.y + 50.0f) + cosf(pPlayer->rot.x);

		g_camera[nCntCamera].posVDest.x = pPlayer->pos.x + sinf(g_camera[nCntCamera].rot.x) * sinf(g_camera[nCntCamera].rot.y) * g_camera[nCntCamera].fDistance;
		g_camera[nCntCamera].posVDest.z = pPlayer->pos.z + sinf(g_camera[nCntCamera].rot.x) * cosf(g_camera[nCntCamera].rot.y) * g_camera[nCntCamera].fDistance;
		g_camera[nCntCamera].posVDest.y = (pPlayer->pos.y + 50.0f) + cosf(g_camera[nCntCamera].rot.x) * g_camera[nCntCamera].fDistance;

		//�J�����ʒu�̍X�V(�ړI�̈ʒu�ւ̍X�V)
		g_camera[nCntCamera].posR += (g_camera[nCntCamera].posRDest - g_camera[nCntCamera].posR) * 0.1f;
		g_camera[nCntCamera].posV += (g_camera[nCntCamera].posVDest - g_camera[nCntCamera].posV) * 0.1f;

		//�p�x�̐��K��
		if (g_camera[nCntCamera].rot.y > D3DX_PI)
		{
			g_camera[nCntCamera].rot.y -= D3DX_PI * 2;
		}
		if (g_camera[nCntCamera].rot.y < -D3DX_PI)
		{
			g_camera[nCntCamera].rot.y += D3DX_PI * 2;
		}
	}
}

//======================================================
//	�J�����̐ݒ菈��
//======================================================
void SetCamera(int nCntCamera)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera[nCntCamera].mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera[nCntCamera].mtxView, &g_camera[nCntCamera].posV, &g_camera[nCntCamera].posR, &g_camera[nCntCamera].vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera[nCntCamera].mtxView);

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_camera[nCntCamera].mtxProjection,
		D3DXToRadian(45.0f),
		(float)(SCREEN_WIDTH / 2) / (float)SCREEN_HEIGHT,
		10.0f,
		6400.0f);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera[nCntCamera].mtxProjection);
}

//======================================================
//	�J�����̏����擾
//======================================================
Camera *GetCamera(void)
{
	return &g_camera[0];
}