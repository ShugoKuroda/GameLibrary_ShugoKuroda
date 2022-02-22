#include "game.h"
#include "player.h"
#include "back.h"
#include "bullet.h"
#include "enemy.h"
#include "score.h"
#include "explosion.h"
#include "life.h"
#include "game_logo.h"
#include "sound.h"
#include "fade.h"
#include "input.h"
#include "particle.h"

//�O���[�o���ϐ�
bool g_bPause = false;		//�Q�[���̃|�[�Y(�ꎞ��~)����

//======================================================
//		�Q�[���S�̂̏���������
//======================================================
void InitGame(void)
{
	g_bPause = false;		//�|�[�Y�@�\�̏�����

	//�w�i�̏���������
	InitBack();

	//�v���C���[�̏���������
	InitPlayer();

	//�G�l�~�[�̏�����
	InitEnemy();

	//�e�̏�����
	InitBullet();

	//�c�@�̏�����
	InitLife();

	//�Q�[�������S�̏�����
	InitLogo();

	//�X�R�A�̏�����
	InitScore();

	//����������
	InitExplosion();

	//�p�[�e�B�N���̏�����
	InitParticle();

	//�T�E���h�Đ�
	PlaySound(SOUND_LABEL_BGM001);
}

//======================================================
//		�Q�[���S�̂̏I������
//======================================================
void UninitGame(void)
{
	//�T�E���h��~
	StopSound();

	//�w�i�̏I������
	UninitBack();

	//�v���C���[�̏I������
	UninitPlayer();

	//�G�l�~�[�I������
	UninitEnemy();

	//�e�̏I������
	UninitBullet();

	//�c�@�̏I������
	UninitLife();

	//�Q�[�������S�̏I������
	UninitLogo();

	//�X�R�A�̏I������
	UninitScore();

	//�����I������
	UninitExplosion();

	//�p�[�e�B�N���̏I������
	UninitParticle();
}

//======================================================
//		�Q�[���S�̂̍X�V����
//======================================================
void UpdateGame(void)
{
	Player *pPlayer;	//�v���C���[�̃|�C���^

	//�v���C���[�����擾
	pPlayer = GetPlayer();

	if (GetKeyboardTrigger(DIK_P) == true)
	{//P(�|�[�Y)�L�[�������ꂽ
		g_bPause = g_bPause ? false : true;		//�|�[�Y���j���[�̐؂�ւ�
	}

	if (pPlayer->bDie == true)
	{//�v���C���[�����S����
		g_bPause = true;		//�Q�[�����~�߂�
	}

	if (g_bPause == false)
	{//�|�[�Y���łȂ����
		//�w�i�̍X�V����
		UpdateBack();

		//�v���C���[�̍X�V����
		UpdatePlayer();

		//�G�l�~�[�X�V����
		UpdateEnemy();

		//�e�̍X�V����
		UpdateBullet();

		//�X�R�A�̍X�V����
		UpdateScore();

		//�����X�V����
		UpdateExplosion();

		//�Q�[�������S�̍X�V����
		UpdateLogo();

		//�p�[�e�B�N���̍X�V����
		UpdateParticle();
	}
}

//======================================================
//		�Q�[���S�̂̕`�揈��
//======================================================
void DrawGame(void)
{
	//�w�i�̕`�揈��
	DrawBack();

	//�v���C���[�̕`�揈��
	DrawPlayer();

	//�G�l�~�[�`�揈��
	DrawEnemy();

	//�e�̕`�揈��
	DrawBullet();

	//�c�@�̕`�揈��
	DrawLife();

	//�X�R�A�̕`�揈��
	DrawScore();

	//�����`�揈��
	DrawExplosion();

	//�Q�[�������S�`�揈��
	DrawLogo();

	//�p�[�e�B�N���̕`�揈��
	DrawParticle();
}