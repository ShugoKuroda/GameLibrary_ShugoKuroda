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
#include "load.h"
#include "game.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "input.h"
#include "shadow.h"
#include "billboard.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "bullet.h"
#include "effect.h"
#include "player.h"
#include "sound.h"
#include "fade.h"
#include "pause.h"
#include "time.h"
#include "score.h"
#include "ui.h"
#include "set.h"

//======================================================
//	�Q�[����ʂ̏���������
//======================================================
void InitGame(void)
{
	//�E�C���h�E�n���h���̎擾
	HWND hWnd = GetHWnd();

	//�|�[�Y��ʂ̏���������
	InitPause();

	//���_�����̏�����
	InitScore();

	//�Z�b�g�J�E���g�����̏�����
	InitSet();

	//UI�̏�����
	InitUI();

	//�e�̏���������
	InitShadow();

	//���f���̏���������
	InitModel();

	//�v���C���[�̏���������
	InitPlayer();

	//���[�h����
	LoadSetModel(hWnd);
	LoadSetPlayer(hWnd);

	//���b�V���t�B�[���h�̏���������
	InitMeshField();

	//�}�`(�~��)�̏���������
	InitMeshCylinder();

	//���C�g�̏���������
	InitLight();

	//�r���{�[�h�̏���������
	InitBillboard();

	//�e�̏���������
	InitBullet();

	//�G�t�F�N�g�̏���������
	InitEffect();

	//�J�����̏���������
	InitCamera();

	//�T�E���h�Đ�
	PlaySound(SOUND_LABEL_BGM001);

	//�Q�[���̃��Z�b�g
	ResetGame();
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

	//���_�̏I������
	UninitScore();

	//�Z�b�g�J�E���g�����̏I��
	UninitSet();

	//UI�̏I������
	UninitUI();

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

	//�v���C���[�̏I������
	UninitPlayer();

	//���f���̏I������
	UninitModel();

	//�r���{�[�h�̏I������
	UninitBillboard();

	//�e�̏I������
	UninitBullet();

	//�G�t�F�N�g�̏I������
	UninitEffect();
}

//======================================================
//	�Q�[����ʂ̍X�V����
//======================================================
void UpdateGame(void)
{
	if (!UpdatePause())
	{//�|�[�Y���łȂ����

		//UI�̍X�V����
		UpdateUI();

		//���_�̍X�V����
		UpdateScore();

		//�Z�b�g�J�E���g�����̍X�V����
		UpdateSet();

		//�e�̍X�V����
		UpdateShadow();

		//���b�V���t�B�[���h�̍X�V����
		UpdateMeshField();

		//�J�����̍X�V����
		UpdateCamera();

		//���C�g�̍X�V����
		UpdateLight();

		//���f���̍X�V����
		UpdateModel();

		//�v���C���[�̍X�V����
		UpdatePlayer();

		//�r���{�[�h�̍X�V����
		UpdateBillboard();

		//�e�̍X�V����
		UpdateBullet();

		//�G�t�F�N�g�̍X�V����
		UpdateEffect();
	}
}

//======================================================
//	�Q�[����ʂ̕`�揈��
//======================================================
void DrawGame(int nCntCamera)
{
	//�J�����̐ݒ�
	SetCamera(nCntCamera);

	//���b�V���t�B�[���h�̕`�揈��
	DrawMeshField();

	//�e�̕`�揈��
	DrawShadow();

	//�r���{�[�h�̕`�揈��
	DrawBillboard();

	//�}�`(�~��)�̕`�揈��
	DrawMeshCylinder();

	//�e�̕`�揈��
	DrawBullet();

	//�v���C���[�̕`�揈��
	DrawPlayer();

	//���f���̕`�揈��
	DrawModel(nCntCamera);

	//�G�t�F�N�g�̕`�揈��
	DrawEffect();

	//���_�̕`��
	DrawScore();

	//�Z�b�g�J�E���g�̕`��
	DrawSet();

	//�|�[�Y��ʂ̕`�揈��
	DrawPause();

	//UI�̕`��
	DrawUI();
}