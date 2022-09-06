//===================================================================
//
//		���C�g����[light.cpp]
//		Author:���c ����
//
//===================================================================

//======================================================
//	�C���N���[�h
//======================================================
#include "main.h"
#include "light.h"

//======================================================
//	�}�N����`
//======================================================
#define MAX_LIGHT	(2)		//���C�g�̍ő吔

//======================================================
//	�O���[�o���ϐ�
//======================================================
D3DLIGHT9 g_aLight[MAX_LIGHT];		//���C�g���

//======================================================
//	���C�g�̏���������
//======================================================
void InitLight(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;		//���C�g�����x�N�g��

	//���C�g���N���A����
	ZeroMemory(&g_aLight[0], sizeof(g_aLight));

	////���C�g�P�̐ݒ�
	//���C�g�̎�ނ�ݒ�
	g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

	//���C�g�̊g�U����ݒ�
	g_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���C�g�̕�����ݒ�
	vecDir = D3DXVECTOR3(-0.2f, -0.8f, 0.4f);

	//���K������(�傫���P�̃x�N�g���ɂ���)
	D3DXVec3Normalize(&vecDir, &vecDir);
	g_aLight[0].Direction = vecDir;

	//���C�g��ݒ� 
	pDevice->SetLight(0, &g_aLight[0]);

	//���C�g��L���ɂ���
	pDevice->LightEnable(0, TRUE);


	////���C�g�Q�̐ݒ�
	//���C�g�̎�ނ�ݒ�
	g_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

	//���C�g�̊g�U����ݒ�
	g_aLight[1].Diffuse = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);

	//���C�g�̕�����ݒ�
	vecDir = D3DXVECTOR3(0.2f, -0.1f, -0.8f);

	//���K������(�傫���P�̃x�N�g���ɂ���)
	D3DXVec3Normalize(&vecDir, &vecDir);
	g_aLight[1].Direction = vecDir;

	//���C�g��ݒ�
	pDevice->SetLight(1, &g_aLight[1]);

	//���C�g��L���ɂ���
	pDevice->LightEnable(1, TRUE);
}

//======================================================
//	���C�g�̏I������
//======================================================
void UninitLight(void)
{

}

//======================================================
//	���C�g�̍X�V����
//======================================================
void UpdateLight(void)
{

}
