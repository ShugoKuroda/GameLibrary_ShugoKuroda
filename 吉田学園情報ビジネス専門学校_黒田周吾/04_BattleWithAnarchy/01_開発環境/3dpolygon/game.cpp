//=============================================================================
//
//	�Q�[����ʏ��� [game.cpp]
//	Author : ���c ����
//
//=============================================================================

//======================================================
//	�C���N���[�h
//======================================================
#include "main.h"
#include <stdio.h>
#include "load.h"
#include "game.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "input.h"
#include "shadow.h"
#include "wall.h"
#include "billboard.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "bullet.h"
#include "effect.h"
#include "player.h"
#include "line.h"
#include "sound.h"
#include "fade.h"
#include "pause.h"
#include "enemy.h"
#include "particle.h"
#include "score.h"
#include "time.h"
#include "ui.h"
#include "life.h"
#include "logo.h"

//======================================================
//	�O���[�o���ϐ�
//======================================================
LogoCount g_Logo;		//���S�o�����

//======================================================
//	�Q�[����ʂ̏���������
//======================================================
void InitGame(void)
{
	//�E�C���h�E�n���h���̎擾
	HWND hWnd = GetHWnd();

	//���S���̏�����
	ZeroMemory(&g_Logo, sizeof(LogoCount));

	//�|�[�Y��ʂ̏���������
	InitPause();

	//UI��ʂ̏���������
	InitUI();

	//�X�R�A��ʂ̏���������
	InitScore();

	//�^�C�}�[�̏���������
	InitTime();

	//���C�t�̏���������
	InitLife();

	//���S�̏�����
	InitLogo();

	//���C���̏���������
	//InitLine();

	//�e�̏���������
	InitShadow();

	//���f���̏���������
	InitModel();

	//�v���C���[�̏���������
	InitPlayer();

	//�G�̏���������
	InitEnemy();

	//���[�h����
	LoadSetModel(hWnd);
	LoadSetPlayer(hWnd);
	LoadSetEnemy(hWnd);

	//���b�V���t�B�[���h�̏���������
	InitMeshField();

	//�}�`(�~��)�̏���������
	InitMeshCylinder();

	//���C�g�̏���������
	InitLight();

	//�r���{�[�h�̏���������
	//InitBillboard();

	//�e�̏���������
	InitBullet();

	//�G�t�F�N�g�̏���������
	InitEffect();

	//�p�[�e�B�N���̏���������
	InitParticle();

	//�J�����̏���������
	InitCamera();

	//�T�E���h�Đ�
	PlaySound(SOUND_LABEL_BGM001);
}

//======================================================
//	�Q�[����ʂ̏I������
//======================================================
void UninitGame(void)
{
	//�T�E���h��~
	StopSound();

	//�|�[�Y�̏I������
	UninitPause();

	//UI��ʂ̏I������
	UninitUI();

	//�X�R�A�̏I������
	UninitScore();

	//�^�C�}�[�̏I������
	UninitTime();

	//���C�t�̏I������
	UninitLife();

	//���S�̏I������
	UninitLogo();

	//�e�̏I������
	UninitShadow();

	//���b�V���t�B�[���h�̏I������
	UninitMeshField();

	//�}�`(�~��)�̏I������
	UninitMeshCylinder();

	//�J�����̏I������
	UninitCamera();

	//���C�g�̏I������
	UninitLight();

	//���C���̏I������
	//UninitLine();

	//�v���C���[�̏I������
	UninitPlayer();

	//�G�̏I������
	UninitEnemy();

	//���f���̏I������
	UninitModel();

	//�r���{�[�h�̏I������
	//UninitBillboard();

	//�e�̏I������
	UninitBullet();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//�p�[�e�B�N���̏I������
	UninitParticle();
}

//======================================================
//	�Q�[����ʂ̍X�V����
//======================================================
void UpdateGame(void)
{
	if (!g_Logo.bNot)
	{
		g_Logo.nCnt++;
		if (g_Logo.nCnt == 1)
		{
			SetLogo(0);
		}
		if (g_Logo.nCnt == 91)
		{
			SetLogo(1);
			g_Logo.bNot = true;
		}
	}

	if (!UpdatePause())
	{//�|�[�Y���łȂ����

		//�e�̍X�V����
		UpdateShadow();

		//UI�̍X�V����
		UpdateUI();

		//�X�R�A�̍X�V����
		UpdateScore();

		//�^�C�}�[�̍X�V����
		UpdateTime();

		//���C�t�̍X�V����
		UpdateLife();

		//���S�̍X�V����
		UpdateLogo();

		//���b�V���t�B�[���h�̍X�V����
		UpdateMeshField();

		//�}�`(�~��)�̍X�V����
		UpdateMeshCylinder();

		//�J�����̍X�V����
		UpdateCamera();

		//���C�g�̍X�V����
		UpdateLight();

		//���f���̍X�V����
		UpdateModel();

		//���C���̍X�V����
		//UpdateLine();

		//�v���C���[�̍X�V����
		UpdatePlayer();

		//�G�̍X�V����
		UpdateEnemy();

		//�r���{�[�h�̍X�V����
		//UpdateBillboard();

		//�e�̍X�V����
		UpdateBullet();

		//�G�t�F�N�g�̍X�V����
		UpdateEffect();

		//�p�[�e�B�N���̍X�V����
		UpdateParticle();
	}
}

//======================================================
//	�Q�[����ʂ̕`�揈��
//======================================================
void DrawGame(void)
{
	//���b�V���t�B�[���h�̕`�揈��
	DrawMeshField();

	//�e�̕`�揈��
	DrawShadow();

	//�r���{�[�h�̕`�揈��
	//5DrawBillboard();

	//�}�`(�~��)�̕`�揈��
	DrawMeshCylinder();

	//���f���̕`�揈��
	DrawModel();

	//���C���̕`�揈��
	//DrawLine();

	//�v���C���[�̕`�揈��
	DrawPlayer();

	//�G�̕`�揈��
	DrawEnemy();

	//�e�̕`�揈��
	DrawBullet();

	//�G�t�F�N�g�̕`�揈��
	DrawEffect();

	//�p�[�e�B�N���̕`�揈��
	DrawParticle();

	//�J�����̐ݒ菈��
	SetCamera();

	//UI��ʂ̕`�揈��
	DrawUI();

	//���S�̕`�揈��
	DrawLogo();

	//���C�t�̕`�揈��
	DrawLife();

	//�^�C�}�[�̕`�揈��
	DrawTime();

	//�X�R�A�̕`�揈��
	DrawScore();

	//�|�[�Y��ʂ̕`�揈��
	DrawPause();
}

//======================================================
//	���S�o�������擾
//======================================================
LogoCount *GetLogoCount(void)
{
	return &g_Logo;
}