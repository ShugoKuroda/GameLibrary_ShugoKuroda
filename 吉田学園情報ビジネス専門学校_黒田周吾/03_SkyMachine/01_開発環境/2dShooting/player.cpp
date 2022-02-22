#include "main.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "enemy.h"
#include "life.h"
#include "game_logo.h"
#include "explosion.h"
#include "sound.h"
#include "fade.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Player g_Player;									//�v���C���[�̏��
bool g_bPlayerlogo;									//�v���C���[�̃��S�\���J�E���g(�J���̂�)

//======================================================
//		�v���C���[�̏���������
//======================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,"data/TEXTURE/Player001.png",&g_pTexturePlayer);

	g_Player.nCounterAnimPlayer = 0;		//�J�E���^�[������������
	g_Player.nPatternAnimPlayer = 0;		//�p�^�[��No.������������
	g_Player.nCntAttack = 0;				//�U���Ԋu�̃J�E���^�[
	g_Player.nHitCnt = 0;					//�v���C���[�̃q�b�g�J�E���g������������
	g_Player.bUse = true;					//�v���C���[�g�p��Ԃ�����������
	g_Player.bcontrol = false;				//�v���C���[�����Ԃ�����������
	g_Player.bcontrol2 = true;				//�v���C���[������2������������
	g_Player.nCntRespawn = 0;				//�v���C���[�̕����J�E���^�[������������
	g_Player.nCntRespawn2 = 0;				//�v���C���[�̕����J�E���^�[2������������
	g_Player.bInvincible = false;			//�v���C���[�̓_�Ő؂�ւ����������������
	g_Player.bDie = false;					//�v���C���[�̎��S��Ԃ����Z�b�g����
	g_Player.bEndGame = false;				//�Q�[���I�������Ԃ�����������
	g_Player.nCntEndGame = 0;				//�Q�[���I���J�ڃJ�E���^�[������������
	g_Player.bVictory = false;				//�v���C���[�̏�����������Z�b�g����
	g_Player.posPlayer = D3DXVECTOR3(-50.0f, SCREEN_HEIGHT / 2, 0.0f);		//�ʒu������������
	g_Player.movePlayer = D3DXVECTOR3(0.0f,0.0f,0.0f);						//�ړ��ʂ�����������

	g_bPlayerlogo = true;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = g_Player.posPlayer.x + sinf(-D3DX_PI*0.75f)*1.50f;
	pVtx[0].pos.y = g_Player.posPlayer.y + cosf(-D3DX_PI*0.75f)*1.50f;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Player.posPlayer.x + sinf(D3DX_PI*0.75f)*1.50f;
	pVtx[1].pos.y = g_Player.posPlayer.y + cosf(D3DX_PI*0.75f)*1.50f;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Player.posPlayer.x + sinf(-0.75f)*1.50f;
	pVtx[2].pos.y = g_Player.posPlayer.y + cosf(-0.75f)*1.50f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Player.posPlayer.x + sinf(0.75f)*1.50f;
	pVtx[3].pos.y = g_Player.posPlayer.y + cosf(0.75f)*1.50f;
	pVtx[3].pos.z = 0.0f;

	//rhw�̐ݒ�(1.0f�ŌŒ�)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//======================================================
//		�v���C���[�̏I������
//======================================================
void UninitPlayer(void)
{
	//�T�E���h��~
	StopSound();

	//�e�N�X�`���̔j��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//======================================================
//		�v���C���[�̍X�V����
//======================================================
void UpdatePlayer(void)
{
	if (g_Player.bcontrol2 == true)
	{
		if (g_Player.bcontrol == true)
		{
			if (GetKeyboardPress(DIK_A) == true)
			{//A�L�[�������ꂽ
				if (GetKeyboardPress(DIK_W) == true)
				{
					g_Player.posPlayer.x += sinf(-D3DX_PI * 0.75f)*PLAYER_MOVE;
					g_Player.posPlayer.y += cosf(-D3DX_PI * 0.75f)*PLAYER_MOVE;
				}
				else if (GetKeyboardPress(DIK_S) == true)
				{
					g_Player.posPlayer.x += sinf(-D3DX_PI * 0.25f)*PLAYER_MOVE;
					g_Player.posPlayer.y += cosf(-D3DX_PI * 0.25f)*PLAYER_MOVE;
				}
				else
				{
					g_Player.posPlayer.x += sinf(-D3DX_PI * 0.5f)*PLAYER_MOVE;
					g_Player.posPlayer.y += cosf(-D3DX_PI * 0.5f)*PLAYER_MOVE;
				}
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{//D�L�[�������ꂽ
				if (GetKeyboardPress(DIK_W) == true)
				{
					g_Player.posPlayer.x += sinf(D3DX_PI *0.75f)*PLAYER_MOVE;
					g_Player.posPlayer.y += cosf(D3DX_PI *0.75f)*PLAYER_MOVE;
				}
				else if (GetKeyboardPress(DIK_S) == true)
				{
					g_Player.posPlayer.x += sinf(D3DX_PI * 0.25f)*PLAYER_MOVE;
					g_Player.posPlayer.y += cosf(D3DX_PI * 0.25f)*PLAYER_MOVE;
				}
				else
				{
					g_Player.posPlayer.x += sinf(D3DX_PI * 0.5f)*PLAYER_MOVE;
					g_Player.posPlayer.y += cosf(D3DX_PI * 0.5f)*PLAYER_MOVE;
				}
			}
			else if (GetKeyboardPress(DIK_W) == true)
			{//W�L�[�������ꂽ
				g_Player.posPlayer.x += sinf(D3DX_PI * 1.0f) * PLAYER_MOVE;
				g_Player.posPlayer.y += cosf(D3DX_PI * 1.0f) * PLAYER_MOVE;

			}
			else if (GetKeyboardPress(DIK_S) == true)
			{//S�L�[�������ꂽ
				g_Player.posPlayer.x += sinf(D3DX_PI * 0.0f) * PLAYER_MOVE;
				g_Player.posPlayer.y += cosf(D3DX_PI * 0.0f) * PLAYER_MOVE;
			}

			if (GetKeyboardPress(DIK_SPACE) == true)
			{//SPACE�L�[�������ꂽ
				g_Player.nCntAttack++;		//�U���Ԋu�̃J�E���^�[
				if (g_Player.nCntAttack > 10)
				{
					//�e�̐ݒ�
					SetBullet(D3DXVECTOR3(g_Player.posPlayer.x + 30, g_Player.posPlayer.y + 15, g_Player.posPlayer.z), 0);
					g_Player.nCntAttack = 0;

					//�T�E���h�Đ�
					PlaySound(SOUND_LABEL_SE_SHOT);
				}
			}
			else
			{//SPACE�L�[��������Ă��Ȃ�
				g_Player.nCntAttack = 10;
			}

			//��ʊO�ւ̂͂ݏo���h�~����
			if (g_Player.posPlayer.x - PLAYER_SIZE_X <= 0)
			{
				g_Player.posPlayer.x = PLAYER_SIZE_X;
			}
			else if (g_Player.posPlayer.x + PLAYER_SIZE_X >= SCREEN_WIDTH)
			{
				g_Player.posPlayer.x = SCREEN_WIDTH - PLAYER_SIZE_X;
			}
			if (g_Player.posPlayer.y - PLAYER_SIZE_Y <= 0)
			{
				g_Player.posPlayer.y = PLAYER_SIZE_Y;
			}
			else if (g_Player.posPlayer.y + PLAYER_SIZE_Y >= SCREEN_HEIGHT)
			{
				g_Player.posPlayer.y = SCREEN_HEIGHT - PLAYER_SIZE_Y;
			}
		}
	}

	if (g_Player.bcontrol == false)
	{//�v���C���[������ł��Ȃ��ꍇ
		g_Player.nCntRespawn++;			//���X�|�[���J�E���^�[�����Z

		if (g_Player.nCntRespawn >= 60)
		{//�J�E���^�[�����l�𒴂�����
			if (g_Player.posPlayer.x < SCREEN_WIDTH / 4.0f)
			{
				g_Player.posPlayer.x += 7.0f;		//�v���C���[��������o��������

				if (g_Player.posPlayer.x >= SCREEN_WIDTH / 5.0f)
				{//�v���C���[�����X�|�[�����I�������
					g_Player.bcontrol = true;		//�v���C���[�𑀍�\�ɂ���
					g_Player.bcontrol2 = true;
					g_Player.nCntRespawn = 0;
				}
			}
		}
	}

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Player.posPlayer.x - PLAYER_SIZE_X, g_Player.posPlayer.y - PLAYER_SIZE_Y, g_Player.posPlayer.z);
	pVtx[1].pos = D3DXVECTOR3(g_Player.posPlayer.x + PLAYER_SIZE_X, g_Player.posPlayer.y - PLAYER_SIZE_Y, g_Player.posPlayer.z);
	pVtx[2].pos = D3DXVECTOR3(g_Player.posPlayer.x - PLAYER_SIZE_X, g_Player.posPlayer.y + PLAYER_SIZE_Y, g_Player.posPlayer.z);
	pVtx[3].pos = D3DXVECTOR3(g_Player.posPlayer.x + PLAYER_SIZE_X, g_Player.posPlayer.y + PLAYER_SIZE_Y, g_Player.posPlayer.z);

	g_Player.nCounterAnimPlayer++;		//�J�E���^�[�����Z
	if (g_Player.nCounterAnimPlayer > 3)
	{
		g_Player.nCounterAnimPlayer = 0;
		g_Player.nPatternAnimPlayer++;

		if (g_Player.nPatternAnimPlayer >= 2)
		{//�p�^�[��No.�������l�ɖ߂�
			g_Player.nPatternAnimPlayer = 0;
		}

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2((0.5f*g_Player.nPatternAnimPlayer), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.5f*(g_Player.nPatternAnimPlayer + 1)), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.5f*g_Player.nPatternAnimPlayer), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.5f*(g_Player.nPatternAnimPlayer + 1)), 1.0f);
	}

	//��Ԕ���
	switch (g_Player.State)
	{
	case PLAYERSTATE_NORMAL:
		break;
	case PLAYERSTATE_RESPAWN:
		if (g_Player.bInvincible == false)
		{
			g_Player.nCntRespawn2++;
			g_Player.nCounterState++;
			if (g_Player.nCounterState == 3)
			{
				g_Player.bInvincible = true;
			}
			if (g_Player.nCntRespawn2 > 180)
			{
				g_Player.bUse = true;
				g_Player.nCntRespawn2 = 0;
				g_Player.State = PLAYERSTATE_NORMAL;
			}
		}
		break;
	}

	if (g_Player.bEndGame == true)
	{
		g_Player.nCntEndGame++;
		if (g_Player.nCntEndGame == 180)
		{
			DiePlayer();
		}
	}

	if (g_bPlayerlogo == true)
	{
		Setlogo(0);
		g_bPlayerlogo = false;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//======================================================
//		�v���C���[�̕`�揈��
//======================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePlayer);

	switch (g_Player.State)
	{
	case PLAYERSTATE_NORMAL:
		//�|���S���`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			0,											//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);											//�v���~�e�B�u(�|���S��)��
		break;
	case PLAYERSTATE_RESPAWN:
		if (g_Player.bInvincible == true)
		{
			//�|���S���`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
				0,											//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);											//�v���~�e�B�u(�|���S��)��

			g_Player.nCounterState--;

			if (g_Player.nCounterState == 0)
			{
				g_Player.bInvincible = false;
			}
		}
		break;
	}
}

//======================================================
//		�v���C���[�̏����擾
//======================================================
Player *GetPlayer(void)
{
	return &g_Player;
}

//======================================================
//		�v���C���[�̃q�b�g����
//======================================================
void HitPlayer(void)
{
	//�q�b�g�J�E���g
	g_Player.nHitCnt++;

	//�����l�U������
	SetExplosion(g_Player.posPlayer, 100.0f);

	//���C�t�\���̌���
	DownLifePlayer();

	if (g_Player.nHitCnt <= 2)
	{
		//�T�E���h�Đ�
		PlaySound(SOUND_LABEL_SE_PLAYERHIT);

		//�v���C���[����ʊO�ɏo��
		g_Player.bUse = false;
		g_Player.bcontrol = false;
		g_Player.bcontrol2 = false;
		g_Player.posPlayer = D3DXVECTOR3(-50.0f, SCREEN_HEIGHT / 2, 0.0f);

		g_Player.State = PLAYERSTATE_RESPAWN;
		g_Player.nCounterState = 0;
	}

	//�v���C���[�̎��S����
	if (g_Player.nHitCnt == 3)
	{
		//�T�E���h��~
		StopSound();

		//�T�E���h�Đ�
		PlaySound(SOUND_LABEL_SE_PLAYERHIT);

		g_Player.bUse = false;
		g_Player.bcontrol2 = false;
		g_Player.posPlayer = D3DXVECTOR3(-50.0f, SCREEN_HEIGHT / 2, 0.0f);

		g_Player.bEndGame = true;
	}
}

//======================================================
//		�v���C���[�̎��S���菈��
//======================================================
void DiePlayer(void)
{
	g_Player.bDie = true;
	SetFade(MODE_RESULT);
}