//========================================================
//
//					  �v���C���[
//					AUTHOR:�z�{���V,���c����
//
//========================================================

//======================================================
//	�C���N���[�h
//======================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "meshfield.h"
#include "bullet.h"
#include "model.h"
#include "sound.h"

//======================================================
//	�}�N����`
//======================================================
#define PLAYER_MOVE				(1.5f)		//�v���C���[�̈ړ���
#define PLAYER_MOVEUP			(5.0f)		//�v���C���[�̃W�����v��
#define PLAYER_HEIGHT			(8.0f)		//�v���C���[�̍���
#define PLAYER_MOVEDOWN			(0.3f)		//�v���C���[�ւ̏d��
#define PLAYER_ROTSPEED			(0.1f)		//�v���C���[�̉�]�̌�����
#define PLAYER_SLIDINGSPEED		(2.0f)		//�v���C���[�X���C�f�B���O���̉�����
#define PLAYER_SIDINGTIME		(10)		//�X���C�f�B���O�̎�������
#define PLAYER_SLIDING_COOLTIME (90)		//�X���C�f�B���O�̃N�[���^�C��

//-----------------------------------------------------------------------------
//	�\���̂̒�`
//-----------------------------------------------------------------------------
//���f�����̍\����
typedef struct
{
	LPD3DXMESH pMesh = NULL;		//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER pBuffMat = NULL;	//�}�e���A�����ւ̃|�C���^
	DWORD nNumMat = 0;				//�}�e���A�����̐�
	bool bUse;						//�g�p���Ă��邩�ǂ���
}PlayerPartsInfo;

//======================================================
//	�O���[�o���錾
//======================================================
PlayerPartsInfo g_aPartsInfo[MAX_PARTS];		//�p�[�c(���f��)�̏��
Player g_player[MAX_PLAYER];					//�v���C���[�̏��
int g_nJudge;
bool g_bGameSet;								//�Q�[�����I��������ǂ���

//======================================================
//	�v���C���[�̏���������
//======================================================
void InitPlayer(void)
{
	//�v���C���[�̃p�[�c(���f��)���̏�����
	ZeroMemory(&g_aPartsInfo, sizeof(g_aPartsInfo)); 
	//�v���C���[���̏�����
	ZeroMemory(&g_player, sizeof(g_player));

	for (int nCntPlayer = 0, nCntPosPlayer = 1; nCntPlayer < MAX_PLAYER; nCntPlayer++, nCntPosPlayer *= -1)
	{
		//�e�̐ݒ�
		g_player[nCntPlayer].nIdxShadow = SetShadow(D3DXVECTOR3(g_player[nCntPlayer].pos.x, 1.0f, g_player[nCntPlayer].pos.z), g_player[nCntPlayer].rot, 30.0f, 30.0f);
	}
	g_nJudge = 0;
	g_bGameSet = false;
}

//======================================================
//	�v���C���[�̏I������
//======================================================
void UninitPlayer(void)
{
	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		//���b�V���̔j��
		if (g_aPartsInfo[nCnt].pMesh != NULL)
		{
			g_aPartsInfo[nCnt].pMesh->Release();
			g_aPartsInfo[nCnt].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_aPartsInfo[nCnt].pBuffMat != NULL)
		{
			g_aPartsInfo[nCnt].pBuffMat->Release();
			g_aPartsInfo[nCnt].pBuffMat = NULL;
		}
	}
}

//======================================================
//	�v���C���[�̍X�V����
//======================================================
void UpdatePlayer(void)
{
	//�O��̈ʒu�̕ۑ�
	g_player[PLAYER_1].posOld = g_player[PLAYER_1].pos;
	g_player[PLAYER_2].posOld = g_player[PLAYER_2].pos;

	if (!g_bGameSet)
	{//�Q�[�����I������瑀��s�\�ɂȂ�

	//--------------
	//�v���C���[[1]
	//--------------

	/*�f�B�X�N���L���b�`�������*/
		if (g_player[PLAYER_1].State == PLAYERSTATE_CATCH)
		{
			g_player[PLAYER_1].rotDest.y = D3DX_PI;

			if (g_player[PLAYER_1].MotionIdx.nMotionIdx != 3)
			{//�������[�V�����̍Đ�
				g_player[PLAYER_1].MotionIdx.nMotionIdx = 3;
				g_player[PLAYER_1].MotionIdx.nKeySetIdx = 0;
				g_player[PLAYER_1].MotionIdx.nFrame = 0;
			}
			g_player[PLAYER_1].nCntThrow++;
			if (g_player[PLAYER_1].nCntThrow >= 30)
			{
				g_player[PLAYER_1].State = PLAYERSTATE_HAVEDISC;
				g_player[PLAYER_1].nCntThrow = 0;		//���Z�b�g
			}
		}

		/*�f�B�X�N�������Ă�����*/
		if (g_player[PLAYER_1].State == PLAYERSTATE_HAVEDISC)
		{
			if (GetJoypadTrigger(JOYKEY_A, PLAYER_1) && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1).z > 0.5f && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1).x < -0.5f)
			{//���O����
				g_player[PLAYER_1].Throw = PLAYERTHROW_STRAIGHT_LEFT;
				g_player[PLAYER_1].State = PLAYERSTATE_THROW;
				g_nJudge = PLAYER_1;
			}
			else if (GetJoypadTrigger(JOYKEY_A, PLAYER_1) && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1).z > 0.5f  && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1).x > 0.5f)
			{//�E�O����
				g_player[PLAYER_1].Throw = PLAYERTHROW_STRAIGHT_RIGHT;
				g_player[PLAYER_1].State = PLAYERSTATE_THROW;
				g_nJudge = PLAYER_1;
			}
			else if (GetJoypadTrigger(JOYKEY_A, PLAYER_1) && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1).x < -0.5f)
			{//������
				g_player[PLAYER_1].Throw = PLAYERTHROW_LEFT;
				g_player[PLAYER_1].State = PLAYERSTATE_THROW;
				g_nJudge = PLAYER_1;
			}
			else if (GetJoypadTrigger(JOYKEY_A, PLAYER_1) && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1).x > 0.5f)
			{//�E����
				g_player[PLAYER_1].Throw = PLAYERTHROW_RIGHT;
				g_player[PLAYER_1].State = PLAYERSTATE_THROW;
				g_nJudge = PLAYER_1;
			}
			else if (GetJoypadTrigger(JOYKEY_A, PLAYER_1) && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1).z > 0.5f)
			{//�O����
				g_player[PLAYER_1].Throw = PLAYERTHROW_STRAIGHT;
				g_player[PLAYER_1].State = PLAYERSTATE_THROW;
				g_nJudge = PLAYER_1;
			}
		}

		/*�f�B�X�N�𓊂��Ă�����*/
		if (g_player[PLAYER_1].State == PLAYERSTATE_THROW)
		{
			g_player[PLAYER_1].rotDest.y = D3DX_PI;

			if (g_player[PLAYER_1].MotionIdx.nMotionIdx != 2)
			{//�������[�V�����̍Đ�
				g_player[PLAYER_1].MotionIdx.nMotionIdx = 2;
				g_player[PLAYER_1].MotionIdx.nKeySetIdx = 0;
				g_player[PLAYER_1].MotionIdx.nFrame = 0;
			}
			g_player[PLAYER_1].nCntThrow++;
			if (g_player[PLAYER_1].nCntThrow >= 20)
			{
				g_player[PLAYER_1].State = PLAYERSTATE_NORMAL;		//�ʏ�̏�Ԃɖ߂�
				if (g_player[PLAYER_1].Throw == PLAYERTHROW_STRAIGHT)
				{//���ʓ���
					SetBullet(D3DXVECTOR3(g_player[PLAYER_1].pos.x, g_player[PLAYER_1].pos.y, g_player[PLAYER_1].pos.z), D3DXVECTOR3(0.0f, 0.0f, 7.0f), 0);
					//�T�E���h�Đ�
					PlaySound(SOUND_LABEL_SE_SHOT);
				}
				if (g_player[PLAYER_1].Throw == PLAYERTHROW_STRAIGHT_RIGHT)
				{//�E�O����
					SetBullet(D3DXVECTOR3(g_player[PLAYER_1].pos.x, g_player[PLAYER_1].pos.y, g_player[PLAYER_1].pos.z), D3DXVECTOR3(2.0f, 0.0f, 5.0f), 0);
					//�T�E���h�Đ�
					PlaySound(SOUND_LABEL_SE_SHOT);
				}
				if (g_player[PLAYER_1].Throw == PLAYERTHROW_RIGHT)
				{//�E����
					SetBullet(D3DXVECTOR3(g_player[PLAYER_1].pos.x, g_player[PLAYER_1].pos.y, g_player[PLAYER_1].pos.z), D3DXVECTOR3(5.0f, 0.0f, 2.0f), 0);
					//�T�E���h�Đ�
					PlaySound(SOUND_LABEL_SE_SHOT);
				}
				if (g_player[PLAYER_1].Throw == PLAYERTHROW_STRAIGHT_LEFT)
				{//���O����
					SetBullet(D3DXVECTOR3(g_player[PLAYER_1].pos.x, g_player[PLAYER_1].pos.y, g_player[PLAYER_1].pos.z), D3DXVECTOR3(-2.0f, 0.0f, 5.0f), 0);
					//�T�E���h�Đ�
					PlaySound(SOUND_LABEL_SE_SHOT);
				}
				if (g_player[PLAYER_1].Throw == PLAYERTHROW_LEFT)
				{//������
					SetBullet(D3DXVECTOR3(g_player[PLAYER_1].pos.x, g_player[PLAYER_1].pos.y, g_player[PLAYER_1].pos.z), D3DXVECTOR3(-5.0f, 0.0f, 2.0f), 0);
					//�T�E���h�Đ�
					PlaySound(SOUND_LABEL_SE_SHOT);
				}
				g_player[PLAYER_1].nCntThrow = 0;		//���Z�b�g
			}
		}

		/*�ʏ�*/
		if (g_player[PLAYER_1].State == PLAYERSTATE_NORMAL)
		{
			//�X���C�f�B���O�̎������Ԃ�߂�
			g_player[PLAYER_1].nSlidingTime = PLAYER_SIDINGTIME;

			//�X���C�f�B���O�̃N�[���^�C��������������
			g_player[PLAYER_1].nSlidingCoolTime--;

			//�ړ�
			if (GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1) && GetJoypadTrigger(JOYKEY_A, PLAYER_1) && g_player[PLAYER_1].nSlidingCoolTime <= 0)
			{//�ړ�+A�{�^���ŃX���C�f�B���O

				//�X�e�B�b�N��񂩂�ړ��ʂ��擾
				D3DXVECTOR3 vector = GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1);

				while (g_player[PLAYER_1].nSlidingTime >= 0)
				{
					//�X���C�f�B���O�̎������Ԃ�����������
					g_player[PLAYER_1].nSlidingTime--;
					g_player[PLAYER_1].move += vector * PLAYER_SLIDINGSPEED;
				}

				//�N�[���^�C���̍Đݒ�
				g_player[PLAYER_1].nSlidingCoolTime = PLAYER_SLIDING_COOLTIME;
			}
			else if (GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1).x > 0.0f || GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1).z > 0.0f ||
				GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1).x < 0.0f || GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1).z < 0.0f)
			{//���X�e�B�b�N���͒��̏ꍇ
			 //���X�e�B�b�N�ړ��ʂ����Z
				g_player[PLAYER_1].move += GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_1);
				//�v���C���[�̌������f
				g_player[PLAYER_1].rotDest.y = atan2f(-g_player[PLAYER_1].move.x, -g_player[PLAYER_1].move.z);
			}
		}

		//�X�e�[�W�̓����蔻��
		if (g_player[PLAYER_1].pos.z + (PLAYER_WIDTH / 2) >= 0.0f)
		{//����
			g_player[PLAYER_1].pos.z = -(PLAYER_WIDTH / 2);
		}
		else if (g_player[PLAYER_1].pos.z - (PLAYER_WIDTH / 2) <= -FIELD_SIZE_Z / 2)
		{//��O��
			g_player[PLAYER_1].pos.z = (-FIELD_SIZE_Z / 2) + (PLAYER_WIDTH / 2);
		}

		//--------------
		//�v���C���[[2]
		//--------------
		/*�f�B�X�N���L���b�`�������*/
		if (g_player[PLAYER_2].State == PLAYERSTATE_CATCH)
		{
			g_player[PLAYER_2].rotDest.y = 0.0f;

			if (g_player[PLAYER_2].MotionIdx.nMotionIdx != 3)
			{//�������[�V�����̍Đ�
				g_player[PLAYER_2].MotionIdx.nMotionIdx = 3;
				g_player[PLAYER_2].MotionIdx.nKeySetIdx = 0;
				g_player[PLAYER_2].MotionIdx.nFrame = 0;
			}
			g_player[PLAYER_2].nCntThrow++;
			if (g_player[PLAYER_2].nCntThrow >= 30)
			{
				g_player[PLAYER_2].State = PLAYERSTATE_HAVEDISC;
				g_player[PLAYER_2].nCntThrow = 0;		//���Z�b�g
			}
		}

		/*�f�B�X�N�������Ă�����*/
		if (g_player[PLAYER_2].State == PLAYERSTATE_HAVEDISC)
		{
			if (GetJoypadTrigger(JOYKEY_A, PLAYER_2) && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2).z < -0.5f && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2).x < -0.5f)
			{//���O����
				g_player[PLAYER_2].Throw = PLAYERTHROW_STRAIGHT_LEFT;
				g_player[PLAYER_2].State = PLAYERSTATE_THROW;
				g_nJudge = PLAYER_2;
			}
			else if (GetJoypadTrigger(JOYKEY_A, PLAYER_2) && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2).z < -0.5f  && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2).x > 0.5f)
			{//�E�O����
				g_player[PLAYER_2].Throw = PLAYERTHROW_STRAIGHT_RIGHT;
				g_player[PLAYER_2].State = PLAYERSTATE_THROW;
				g_nJudge = PLAYER_2;
			}
			else if (GetJoypadTrigger(JOYKEY_A, PLAYER_2) && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2).x < -0.5f)
			{//������
				g_player[PLAYER_2].Throw = PLAYERTHROW_LEFT;
				g_player[PLAYER_2].State = PLAYERSTATE_THROW;
				g_nJudge = PLAYER_2;
			}
			else if (GetJoypadTrigger(JOYKEY_A, PLAYER_2) && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2).x > 0.5f)
			{//�E����
				g_player[PLAYER_2].Throw = PLAYERTHROW_RIGHT;
				g_player[PLAYER_2].State = PLAYERSTATE_THROW;
				g_nJudge = PLAYER_2;
			}
			else if (GetJoypadTrigger(JOYKEY_A, PLAYER_2) && GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2).z < -0.5f)
			{//�O����
				g_player[PLAYER_2].Throw = PLAYERTHROW_STRAIGHT;
				g_player[PLAYER_2].State = PLAYERSTATE_THROW;
				g_nJudge = PLAYER_2;
			}
		}

		/*�f�B�X�N�𓊂��Ă�����*/
		if (g_player[PLAYER_2].State == PLAYERSTATE_THROW)
		{
			if (g_player[PLAYER_2].MotionIdx.nMotionIdx != 2)
			{//�������[�V�����̍Đ�
				g_player[PLAYER_2].MotionIdx.nMotionIdx = 2;
				g_player[PLAYER_2].MotionIdx.nKeySetIdx = 0;
				g_player[PLAYER_2].MotionIdx.nFrame = 0;
			}
			g_player[PLAYER_2].nCntThrow++;
			if (g_player[PLAYER_2].nCntThrow >= 20)
			{
				g_player[PLAYER_2].State = PLAYERSTATE_NORMAL;		//�ʏ�̏�Ԃɖ߂�
				if (g_player[PLAYER_2].Throw == PLAYERTHROW_STRAIGHT)
				{//���ʓ���
					SetBullet(D3DXVECTOR3(g_player[PLAYER_2].pos.x, g_player[PLAYER_2].pos.y, g_player[PLAYER_2].pos.z), D3DXVECTOR3(0.0f, 0.0f, -7.0f), 0);
				}
				if (g_player[PLAYER_2].Throw == PLAYERTHROW_STRAIGHT_RIGHT)
				{//�E�O����
					SetBullet(D3DXVECTOR3(g_player[PLAYER_2].pos.x, g_player[PLAYER_2].pos.y, g_player[PLAYER_2].pos.z), D3DXVECTOR3(2.0f, 0.0f, -5.0f), 0);
				}
				if (g_player[PLAYER_2].Throw == PLAYERTHROW_RIGHT)
				{//�E����
					SetBullet(D3DXVECTOR3(g_player[PLAYER_2].pos.x, g_player[PLAYER_2].pos.y, g_player[PLAYER_2].pos.z), D3DXVECTOR3(5.0f, 0.0f, -2.0f), 0);
				}
				if (g_player[PLAYER_2].Throw == PLAYERTHROW_STRAIGHT_LEFT)
				{//���O����
					SetBullet(D3DXVECTOR3(g_player[PLAYER_2].pos.x, g_player[PLAYER_2].pos.y, g_player[PLAYER_2].pos.z), D3DXVECTOR3(-2.0f, 0.0f, -5.0f), 0);
				}
				if (g_player[PLAYER_2].Throw == PLAYERTHROW_LEFT)
				{//������
					SetBullet(D3DXVECTOR3(g_player[PLAYER_2].pos.x, g_player[PLAYER_2].pos.y, g_player[PLAYER_2].pos.z), D3DXVECTOR3(-5.0f, 0.0f, -2.0f), 0);
				}
				g_player[PLAYER_2].nCntThrow = 0;		//���Z�b�g
			}
		}

		/*�ʏ�*/
		if (g_player[PLAYER_2].State == PLAYERSTATE_NORMAL)
		{
			//�X���C�f�B���O�̎������Ԃ�߂�
			g_player[PLAYER_2].nSlidingTime = PLAYER_SIDINGTIME;

			//�X���C�f�B���O�̃N�[���^�C��������������
			g_player[PLAYER_2].nSlidingCoolTime--;

			//�ړ�
			if (GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2) && GetJoypadTrigger(JOYKEY_A, PLAYER_2) && g_player[PLAYER_2].nSlidingCoolTime <= 0)
			{//�ړ�+A�{�^���ŃX���C�f�B���O

				 //�X�e�B�b�N��񂩂�ړ��ʂ��擾
				D3DXVECTOR3 vector = GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2);

				while (g_player[PLAYER_2].nSlidingTime >= 0)
				{
					//�X���C�f�B���O�̎������Ԃ�����������
					g_player[PLAYER_2].nSlidingTime--;
					g_player[PLAYER_2].move += vector * PLAYER_SLIDINGSPEED;
				}

				//�N�[���^�C���̍Đݒ�
				g_player[PLAYER_2].nSlidingCoolTime = PLAYER_SLIDING_COOLTIME;
			}
			else if (GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2).x > 0.0f || GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2).z > 0.0f ||
				GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2).x < 0.0f || GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2).z < 0.0f)
			{//���X�e�B�b�N���͒��̏ꍇ
			 //���X�e�B�b�N�ړ��ʂ����Z
				g_player[PLAYER_2].move += GetJoypadStick(JOYKEY_LEFT_STICK, PLAYER_2);
				//�v���C���[�̌������f
				g_player[PLAYER_2].rotDest.y = atan2f(-g_player[PLAYER_2].move.x, -g_player[PLAYER_2].move.z);
			}
		}

		//�X�e�[�W�̓����蔻��
		if (g_player[PLAYER_2].pos.z + (PLAYER_WIDTH / 2) >= FIELD_SIZE_Z / 2)
		{//����
			g_player[PLAYER_2].pos.z = (FIELD_SIZE_Z / 2) - (PLAYER_WIDTH / 2);
		}
		else if (g_player[PLAYER_2].pos.z - (PLAYER_WIDTH / 2) <= 0.0f)
		{//��O��
			g_player[PLAYER_2].pos.z = (PLAYER_WIDTH / 2);
		}
	}

	//--------------
	//	  ����
	//--------------
	//�����̒ǉ�
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_player[nCntPlayer].move.y--;

		//�ʒu�Ɉړ��ʂ����Z
		g_player[nCntPlayer].pos += g_player[nCntPlayer].move;

		g_player[nCntPlayer].move.x += (-g_player[nCntPlayer].move.x) * 0.5f;
		g_player[nCntPlayer].move.z += (-g_player[nCntPlayer].move.z) * 0.5f;

		////��]�̐��K��
		//�ړI�̊p�x���傫�������菬���������肵�����̌v�Z
		if (g_player[nCntPlayer].rotDest.y - g_player[nCntPlayer].rot.y > D3DX_PI)
		{
			g_player[nCntPlayer].rotDest.y -= D3DX_PI * 2.0f;
		}
		else if (g_player[nCntPlayer].rotDest.y - g_player[nCntPlayer].rot.y < -D3DX_PI)
		{
			g_player[nCntPlayer].rotDest.y += D3DX_PI * 2.0f;
		}

		//�����̌v�Z
		g_player[nCntPlayer].rot.y += (g_player[nCntPlayer].rotDest.y - g_player[nCntPlayer].rot.y) * PLAYER_ROTSPEED;

		//�v�Z���ʂ��傫�������菬���������肵�����̒���
		if (g_player[nCntPlayer].rot.y < -D3DX_PI)
		{
			g_player[nCntPlayer].rot.y += D3DX_PI * 2.0f;
		}
		else if (g_player[nCntPlayer].rot.y > D3DX_PI)
		{
			g_player[nCntPlayer].rot.y -= D3DX_PI * 2.0f;
		}

		//�ǁE���̓����蔻�菈��
		if (g_player[nCntPlayer].pos.x - (PLAYER_WIDTH / 2) <= -FIELD_SIZE_X / 2)
		{//����
			g_player[nCntPlayer].pos.x = (-FIELD_SIZE_X / 2) + (PLAYER_WIDTH / 2);
		}
		else if (g_player[nCntPlayer].pos.x + (PLAYER_WIDTH / 2) >= FIELD_SIZE_X / 2)
		{//�E��
			g_player[nCntPlayer].pos.x = (FIELD_SIZE_X / 2) - (PLAYER_WIDTH / 2);
		}
		if (g_player[nCntPlayer].pos.y - (PLAYER_HEIGHT / 2) <= 0.0f)
		{//���Ƃ̓����蔻��
			g_player[nCntPlayer].pos.y = (PLAYER_HEIGHT / 2);
		}

		//�e�̈ʒu�X�V
		SetPositionShadow(g_player[nCntPlayer].nIdxShadow, D3DXVECTOR3(g_player[nCntPlayer].pos.x, 1.0f, g_player[nCntPlayer].pos.z), g_player[nCntPlayer].rot);

		if (g_player[nCntPlayer].State != PLAYERSTATE_THROW && g_player[nCntPlayer].State != PLAYERSTATE_CATCH)
		{//�v���C���[���f�B�X�N�𓊂��Ă���A���L���b�`���łȂ��ꍇ
			if (g_player[nCntPlayer].move.x >= 0.8f
				|| g_player[nCntPlayer].move.x <= -0.8f
				|| g_player[nCntPlayer].move.z >= 0.8f
				|| g_player[nCntPlayer].move.z <= -0.8f)
			{//�v���C���[���ړ���
				if (g_player[nCntPlayer].MotionIdx.nMotionIdx != 1)
				{//�_�b�V�����[�V�����̍Đ�
					g_player[nCntPlayer].MotionIdx.nMotionIdx = 1;
					g_player[nCntPlayer].MotionIdx.nKeySetIdx = 0;
					g_player[nCntPlayer].MotionIdx.nFrame = 0;
				}
			}
			else
			{//�~�܂��Ă���ꍇ
				if (g_player[nCntPlayer].MotionIdx.nMotionIdx != 0)
				{//�j���[�g�������[�V�����̍Đ�
					g_player[nCntPlayer].MotionIdx.nMotionIdx = 0;
					g_player[nCntPlayer].MotionIdx.nKeySetIdx = 0;
					g_player[nCntPlayer].MotionIdx.nFrame = 0;
				}
			}
		}
		if(g_bGameSet)
		{
			if (g_player[nCntPlayer].State == PLAYERSTATE_WIN)
			{
				if (g_player[nCntPlayer].MotionIdx.nMotionIdx != 4)
				{//�������[�V�����̍Đ�
					g_player[nCntPlayer].MotionIdx.nMotionIdx = 4;
					g_player[nCntPlayer].MotionIdx.nKeySetIdx = 0;
					g_player[nCntPlayer].MotionIdx.nFrame = 0;
				}
			}
			else if (g_player[nCntPlayer].State == PLAYERSTATE_LOSE)
			{
				if (g_player[nCntPlayer].MotionIdx.nMotionIdx != 5)
				{//�s�k���[�V�����̍Đ�
					g_player[nCntPlayer].MotionIdx.nMotionIdx = 5;
					g_player[nCntPlayer].MotionIdx.nKeySetIdx = 0;
					g_player[nCntPlayer].MotionIdx.nFrame = 0;
				}
			}
		}

		//���[�V�����Đ�
		MotionPlayer(nCntPlayer);
	}

		//�f�B�X�N�Ƃ̓����蔻��
		if (g_nJudge == PLAYER_1)
		{//�v���C���[[1]���������f�B�X�N�̓v���C���[[2]�Ɠ����蔻����s��
			CollisionDisc(PLAYER_2);
		}
		else if (g_nJudge == PLAYER_2)
		{//�v���C���[[2]���������f�B�X�N�̓v���C���[[1]�Ɠ����蔻����s��
			CollisionDisc(PLAYER_1);
		}
}

//======================================================
//	�v���C���[�̕`�揈��
//======================================================
void DrawPlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (g_player[nCntPlayer].bUse)
		{
			//�f�o�C�X�̎擾
			LPDIRECT3DDEVICE9 pDevice = GetDevice();

			D3DXMATRIX mtxRotParent;		//�e�̃��[���h�}�g���b�N�X
			D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

			//���[���h�}�g���b�N�X�̏������i�e�j
			D3DXMatrixIdentity(&mtxRotParent);

			//�v���C���[�̌����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_player[nCntPlayer].rot.y, g_player[nCntPlayer].rot.x, g_player[nCntPlayer].rot.z);

			D3DXMatrixMultiply(&mtxRotParent, &mtxRotParent, &mtxRot);

			//�v���C���[�̈ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans,
				g_player[nCntPlayer].pos.x, g_player[nCntPlayer].pos.y, g_player[nCntPlayer].pos.z);

			D3DXMatrixMultiply(&mtxRotParent, &mtxRotParent, &mtxTrans);

			for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
			{
				//�v���C���[�p�[�c���̒Z�k�p�|�C���^
				PlayerParts *pPart = &g_player[nCntPlayer].aParts[nCntParts];

				if (pPart->bUse)
				{
					D3DMATERIAL9 matDef;	//���݂̃}�e���A���ۑ��p
					D3DXMATERIAL *pMat;		//�}�e���A���f�[�^�ւ̃|�C���^

					//���[���h�}�g���b�N�X�̏������i�q�j
					D3DXMatrixIdentity(&g_player[nCntPlayer].aParts[pPart->nIndex].mtxWorld);

					//�����𔽉f
					D3DXMatrixRotationYawPitchRoll(&mtxRot,
						pPart->rot.y, pPart->rot.x, pPart->rot.z);

					D3DXMatrixMultiply(&g_player[nCntPlayer].aParts[pPart->nIndex].mtxWorld, &g_player[nCntPlayer].aParts[pPart->nIndex].mtxWorld, &mtxRot);

					//�ʒu�𔽉f
					D3DXMatrixTranslation(&mtxTrans,
						pPart->pos.x, pPart->pos.y, pPart->pos.z);

					D3DXMatrixMultiply(&g_player[nCntPlayer].aParts[pPart->nIndex].mtxWorld, &g_player[nCntPlayer].aParts[pPart->nIndex].mtxWorld, &mtxTrans);

					//�����̐e���f���̃}�g���b�N�X�Ƃ̊|���Z
					if (g_player[nCntPlayer].aParts[nCntParts].nParent < 0)
					{
						//���f���̃}�g���b�N�X �� �e�̃��[���h�}�g���b�N�X
						D3DXMatrixMultiply(&g_player[nCntPlayer].aParts[pPart->nIndex].mtxWorld, &g_player[nCntPlayer].aParts[pPart->nIndex].mtxWorld, &mtxRotParent);
					}
					else
					{
						//���f���̃}�g���b�N�X �� �e�̃��[���h�}�g���b�N�X
						D3DXMatrixMultiply(&g_player[nCntPlayer].aParts[pPart->nIndex].mtxWorld, &g_player[nCntPlayer].aParts[pPart->nIndex].mtxWorld, &g_player[nCntPlayer].aParts[pPart->nParent].mtxWorld);
					}

					if (g_aPartsInfo[pPart->nIndex].pMesh == NULL)
					{//�p�[�c�̃��f����NULL��������
						continue;		//for���̍ŏ��ɖ߂�
					}
	
					//���[���h�}�g���b�N�X�̐ݒ�
					pDevice->SetTransform(D3DTS_WORLD, &g_player[nCntPlayer].aParts[pPart->nIndex].mtxWorld);

					//���݂̃}�e���A����ێ�
					pDevice->GetMaterial(&matDef);

					//�}�e���A���f�[�^�ւ̃|�C���^���擾
					pMat = (D3DXMATERIAL*)g_aPartsInfo[pPart->nIndex].pBuffMat->GetBufferPointer();

					for (int nCntMat = 0; nCntMat < (int)g_aPartsInfo[pPart->nIndex].nNumMat; nCntMat++)
					{
						//�}�e���A���̐ݒ�
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

						//���f���p�[�c�̕`��
						g_aPartsInfo[pPart->nIndex].pMesh->DrawSubset(nCntMat);
					}

					//�ێ����Ă����}�e���A����߂�
					pDevice->SetMaterial(&matDef);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	���f���^�C�v�ǂݍ��ݏ���
//-----------------------------------------------------------------------------
HRESULT LoadPlayerType(HWND hWnd, char *sXFilePath)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntPatn = 0; nCntPatn < MAX_PARTS; nCntPatn++)
	{
		if (!g_aPartsInfo[nCntPatn].bUse)
		{
			//X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(sXFilePath,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_aPartsInfo[nCntPatn].pBuffMat,
				NULL,
				&g_aPartsInfo[nCntPatn].nNumMat,
				&g_aPartsInfo[nCntPatn].pMesh);

			g_aPartsInfo[nCntPatn].bUse = true;

			return S_OK;
		}
	}

	MessageBox(hWnd, "�ǂݍ��݉\���f�����𒴂��Ă��܂�", "�x���I", MB_ICONWARNING);
	return E_FAIL;
}

//-----------------------------------------------------------------------------
//	�v���C���[�̃Z�b�g����
//-----------------------------------------------------------------------------
void SetPlayer(Player player, D3DXVECTOR3 InitPos, int nCnt)
{
	g_player[nCnt] = player;		//�v���C���[�̏��
	g_player[nCnt].pos = InitPos;	//�v���C���[�̏����ʒu
}

//-----------------------------------------------------------------------------
//	���[�V�����Đ�����
//-----------------------------------------------------------------------------
void MotionPlayer(int nCntPlayer)
{
	int nMotion = g_player[nCntPlayer].MotionIdx.nMotionIdx;
	int nKey = g_player[nCntPlayer].MotionIdx.nKeySetIdx;
	int nFrame = g_player[nCntPlayer].aMotion[nMotion].nFrame[nKey];

	if (nFrame <= 0)
	{//�e�L�X�g�Őݒ肳�ꂽ�t���[������0�ȉ��������ꍇ
		nFrame = 1;
	}

	//���[�V�����Đ�����
	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		if (g_player[nCntPlayer].aParts[nCnt].bUse)
		{
			//�ʒu�X�V�i���[�J�����W�j
			g_player[nCntPlayer].aParts[nCnt].pos += g_player[nCntPlayer].aMotion[nMotion].aKey[nKey].pos[nCnt] / (float)g_player[nCntPlayer].aMotion[nMotion].nFrame[nKey];

			//�p�x�X�V
			g_player[nCntPlayer].aParts[nCnt].rot += (g_player[nCntPlayer].aParts[nCnt].baseRot + g_player[nCntPlayer].aMotion[nMotion].aKey[nKey].rot[nCnt] - g_player[nCntPlayer].aParts[nCnt].rot) / (float)g_player[nCntPlayer].aMotion[nMotion].nFrame[nKey];
		}
	}

	//�t���[���̉��Z
	g_player[nCntPlayer].MotionIdx.nFrame++;

	if (g_player[nCntPlayer].aMotion[nMotion].nFrame[nKey] <= g_player[nCntPlayer].MotionIdx.nFrame)
	{//�t���[�������ݒ�̒l�𒴂�����

	 //�Đ����̃L�[���̉��Z
		g_player[nCntPlayer].MotionIdx.nKeySetIdx++;

		//�t���[���̏�����
		g_player[nCntPlayer].MotionIdx.nFrame = 0;

		if (g_player[nCntPlayer].aMotion[nMotion].nNumKey <= g_player[nCntPlayer].MotionIdx.nKeySetIdx)
		{//�Đ����̃L�[�����ݒ�̒l�𒴂�����
			if (g_player[nCntPlayer].aMotion[nMotion].nLoop == 1)
			{//���[�V���������[�v�ݒ肾������
				g_player[nCntPlayer].MotionIdx.nKeySetIdx = 0;
			}
			else
			{//���ݍĐ����̃��[�V��������j���[�g�������[�V�����ɕύX
				g_player[nCntPlayer].MotionIdx.nKeySetIdx = 0;
				g_player[nCntPlayer].MotionIdx.nMotionIdx = 0;
			}
		}
	}
}

//========================================================
//				�f�B�X�N�Ƃ̓����蔻��
//========================================================
void CollisionDisc(int nIdxPlayer)
{
	Bullet *pBullet = GetBullet();

	if (pBullet->bUse == true)
	{//�f�B�X�N���g�p����Ă���Ƃ�
		if (pBullet->pos.x + 10.0f > g_player[nIdxPlayer].pos.x - PLAYER_COLLISION &&
			pBullet->pos.x - 10.0f < g_player[nIdxPlayer].pos.x + PLAYER_COLLISION &&
			pBullet->pos.z + 10.0f > g_player[nIdxPlayer].pos.z - PLAYER_COLLISION &&
			pBullet->pos.z - 10.0f < g_player[nIdxPlayer].pos.z + PLAYER_COLLISION)
		{
			g_player[nIdxPlayer].State = PLAYERSTATE_CATCH;		//�v���C���[���f�B�X�N���L���b�`������Ԃɂ���
			pBullet->bUse = false;								//�e���g�p���Ă��Ȃ���Ԃɂ���
			pBullet->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ��ʂ����Z�b�g
		}
	}
}

//========================================================
//		�ŏ��Ƀf�B�X�N�𓊂���v���C���[�����
//========================================================
void DiscChange(int nIdxPlayer)
{
	g_nJudge = nIdxPlayer;
}

//========================================================
//		�Q�[���Z�b�g����
//========================================================
void GameSet(int nWinPlayer, int nLosePlayer)
{
	g_bGameSet = true;
	g_player[nWinPlayer].State = PLAYERSTATE_WIN;
	g_player[nLosePlayer].State = PLAYERSTATE_LOSE;

	//�T�E���h�Đ�
	PlaySound(SOUND_LABEL_SE_VICTORY);
}

//======================================================
//	�v���C���[�̏����擾
//======================================================
Player *GetPlayer(void)
{
	return &g_player[0];
}