//===================================================================
//
//		�J��������[camera.cpp]
//		Author:���c ����
//
//===================================================================

//======================================================
//	�C���N���[�h
//======================================================
#include "main.h"
#include "input.h"
#include "inputMouse.h"
#include "camera.h"
#include "player.h"
#include "model.h"

//======================================================
//	�O���[�o���ϐ�
//======================================================
Camera g_camera;	//�J�������
float g_AngleX;

//======================================================
//	�J�����̏���������
//======================================================
void InitCamera(void)
{
	//���_�E�����_�E�������ݒ肷��
	g_camera.posV = D3DXVECTOR3(0.0f, 30.0f, -150.0f);
	g_camera.posR = D3DXVECTOR3(0.0f, 90.0f, 0.0f);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//�Œ�
	g_camera.posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�����_�܂ł̋���
	D3DXVECTOR3 Distance = D3DXVECTOR3((g_camera.posV.x + g_camera.posR.x), (g_camera.posV.y + g_camera.posR.y), (g_camera.posV.z + g_camera.posR.z));

	//�����E���_���璍���_�܂ł̋���������������
	g_AngleX = atan2f(Distance.z, Distance.y);
	g_camera.rot = D3DXVECTOR3(g_AngleX, 0.0f, 0.0f);
	g_camera.fDistance = sqrtf(Distance.x * Distance.x + Distance.z * Distance.z);
	g_camera.fDistance = sqrtf(Distance.y * Distance.y + g_camera.fDistance * g_camera.fDistance);
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

	//�v���C���[���̎擾
	Model *pModel = GetModel();

	//�}�E�X�̈ړ��ʏ��̎擾
	DIMOUSESTATE *pMouse = GetMouseMove();

	//if (GetKeyboardPress(DIK_V) == true)
	//{//���b�N�I��
	//	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++, pModel++)
	//	{
	//		if (pModel->bUse)
	//		{
	//			g_camera.posRDest.x = pModel->pos.x + sinf(pModel->rot.x) * sinf(pModel->rot.y);
	//			g_camera.posRDest.z = pModel->pos.z + sinf(pModel->rot.x) * cosf(pModel->rot.y);
	//			g_camera.posRDest.y = pModel->pos.y + cosf(pModel->rot.x);

	//			D3DXVECTOR3 vecCamera = (pPlayer->pos - g_camera.posR) * 1.5f;
	//			D3DXMATRIX mtxRot;

	//			//�J�����̂̌����𔽉f
	//			D3DXMatrixRotationYawPitchRoll(&mtxRot, -D3DX_PI / 12.0f, 0.0f, 0.0f);

	//			D3DXVec3TransformCoord(&vecCamera, &vecCamera, &mtxRot);

	//			g_camera.posVDest = g_camera.posR + vecCamera;
	//		}
	//	}
	//}
	if (!pPlayer->bIsDie)
	{//�v���C���[�����S������J��������A�Ǐ]��OFF�ɂ���
		//�����_���}�E�X����ŕς���
		g_camera.rot.y += (D3DX_PI / 180.0f) * (pMouse->lX * 0.25f);
		g_camera.rot.x += (D3DX_PI / 180.0f) * (pMouse->lY * 0.25f);
		if (g_camera.rot.x >= (D3DX_PI / 8.0f) - 0.4f)
		{
			g_camera.rot.x = (D3DX_PI / 8.0f) - 0.4f;
		}
		else if (g_camera.rot.x <= -1.8f)
		{
			g_camera.rot.x = -1.8f;
		}
		g_camera.posV.x = g_camera.posR.x + sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z + sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.y = g_camera.posR.y + cosf(g_camera.rot.x) * g_camera.fDistance;

		//�v���C���[�Ǐ]
		g_camera.posRDest.x = pPlayer->pos.x + sinf(pPlayer->rot.x) * sinf(pPlayer->rot.y);
		g_camera.posRDest.z = pPlayer->pos.z + sinf(pPlayer->rot.x) * cosf(pPlayer->rot.y);
		g_camera.posRDest.y = (pPlayer->pos.y + 50.0f) + cosf(pPlayer->rot.x);
		g_camera.posVDest.x = pPlayer->pos.x + sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posVDest.z = pPlayer->pos.z + sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posVDest.y = (pPlayer->pos.y + 50.0f) + cosf(g_camera.rot.x) * g_camera.fDistance;
	}

	//�J�����ʒu�̍X�V
	g_camera.posR.x += (g_camera.posRDest.x - g_camera.posR.x) * 0.3f;
	g_camera.posR.z += (g_camera.posRDest.z - g_camera.posR.z) * 0.3f;
	g_camera.posR.y += (g_camera.posRDest.y - g_camera.posR.y) * 0.3f;
	g_camera.posV.x += (g_camera.posVDest.x - g_camera.posV.x) * 0.3f;
	g_camera.posV.z += (g_camera.posVDest.z - g_camera.posV.z) * 0.3f;
	g_camera.posV.y += (g_camera.posVDest.y - g_camera.posV.y) * 0.3f;

	//�p�x�̐��K��
	if (g_camera.rot.y > D3DX_PI)
	{
		g_camera.rot.y -= D3DX_PI * 2;
	}
	if (g_camera.rot.y < -D3DX_PI)
	{
		g_camera.rot.y += D3DX_PI * 2;
	}
}

//======================================================
//	�J�����̐ݒ菈��
//======================================================
void SetCamera(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView, &g_camera.posV, &g_camera.posR, &g_camera.vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		6400.0f);			//���E�̐ݒ�

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);
}

//======================================================
//	�J�����̏����擾
//======================================================
Camera *GetCamera(void)
{
	return &g_camera;
}