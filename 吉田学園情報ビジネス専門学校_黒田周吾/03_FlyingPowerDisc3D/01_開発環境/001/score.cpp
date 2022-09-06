//========================================================
//
//					���_���̏���
//					AUTHOR:�z�{���V
//
//========================================================
#include "player.h"
#include "score.h"
#include "bullet.h"
#include "set.h"
#include "ui.h"
#include "fade.h"
#include "sound.h"
//#include "game.h"
//#include "result.h"
//#include "fade.h"

//�}�N����`
#define NUM_SCORE (2)											//���_�̌�
#define ONE_SENT  (60)											//1�t���[����1/60�b�Ƃ����Ƃ���1�b

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;			//���_�̃e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		//���_�̒��_���ւ̃|�C���^
Score g_aScore[MAX_PLAYER];							//���_�̏����i�[
bool g_bUse;				//���_�̕\���E��\�����Ǘ�
int g_nCntScoreTime;		//�X�R�A�̕\�����ԃJ�E���^�[
int g_nCntChange;			//�ŏ��ɓ�����l����シ��p
int g_nSetPlayer1;			//Set���L���p(1P)
int g_nSetPlayer2;			//Set���L���p(2P)
int g_nCntGameEnd;			//�Q�[�����I���A��ʑJ�ڂ���܂ł̃J�E���^�[
	
//========================================================
//					���_�̏���������
//========================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureScore);

	//�O���[�o���ϐ��̏�����
	g_bUse = false;						//�\�����Ă��Ȃ���Ԃɂ���
	g_nCntScoreTime = 0;
	g_nCntChange = PLAYER_1;
	g_nSetPlayer1 = 0;
	g_nSetPlayer2 = 0;
	g_nCntGameEnd = 0;
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_aScore[nCntPlayer].nScore = 0;							//�X�R�A�̏�����
		g_aScore[nCntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu�̏�����
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (NUM_SCORE * MAX_PLAYER),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	//�v���C���[[1]�̓��_
	for (int nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(320.0f + (80.0f * nCntScore), 250.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(400.0f + (80.0f * nCntScore), 250.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(320.0f + (80.0f * nCntScore), 430.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(400.0f + (80.0f * nCntScore), 430.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`���̍��W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	//�v���C���[[2]�̓��_
	for (int nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(800.0f + (80.0f * nCntScore), 250.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(880.0f + (80.0f * nCntScore), 250.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(800.0f + (80.0f * nCntScore), 430.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(880.0f + (80.0f * nCntScore), 430.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`���̍��W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	//���_���W���A�����b�N
	g_pVtxBuffScore->Unlock();
}

//========================================================
//					���_�̏I������
//========================================================
void UninitScore(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//========================================================
//					���_�̍X�V����
//========================================================
void UpdateScore(void)
{
	if (g_bUse)
	{//���_���\������Ă�����
		g_nCntScoreTime++;		//�\���J�E���^�[���Z
		if (g_nCntScoreTime >= 150)
		{//�J�E���^�[�����ȏ�
			if (g_aScore[PLAYER_1].nScore >= 15)
			{//1P�̓_��15�_�ȏ�
				if (g_nCntScoreTime == 151)
				{//�Z�b�g�J�E���g��\��
					SetCounter(PLAYER_1);
					SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.4f, 0.0f), D3DXVECTOR2(200.0f, 170.0f), 150, 7);		//SetCount�\�L
					g_nSetPlayer1++;
					if (g_nSetPlayer1 >= 2)
					{//Set����2�ȏ�
						GameSet(PLAYER_1, PLAYER_2);
						g_nCntScoreTime = 301;
						SetUI(D3DXVECTOR3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 1.4f, 0.0f), D3DXVECTOR2(200.0f, 170.0f), 150, 5);		//win�\�L
						SetUI(D3DXVECTOR3(SCREEN_WIDTH / 1.25f, SCREEN_HEIGHT / 1.4f, 0.0f), D3DXVECTOR2(200.0f, 170.0f), 150, 6);		//lose�\�L
					}
				}
				if (g_nCntScoreTime == 300)
				{//�X�R�A�A�Q�[����Ԃ����Z�b�g
					ResetGame();
					g_aScore[PLAYER_1].nScore = 0;
					g_aScore[PLAYER_2].nScore = 0;
					g_nCntScoreTime = 0;
				}
				if (g_nCntScoreTime == 450)
				{//�^�C�g���ɖ߂�
					SetFade(MODE_TITLE);
					g_nCntScoreTime = 0;
				}
			}
			else if (g_aScore[PLAYER_2].nScore >= 15)
			{//2P�̓_��15�_�ȏ�
				if (g_nCntScoreTime == 151)
				{//�Z�b�g�J�E���g��\��
					SetCounter(PLAYER_2);
					SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.4f, 0.0f), D3DXVECTOR2(200.0f, 170.0f), 150, 7);		//SetCount�\�L
					g_nSetPlayer2++;
					if (g_nSetPlayer2 >= 2)
					{//Set����2�ȏ�
						GameSet(PLAYER_2, PLAYER_1);
						g_nCntScoreTime = 301;
						SetUI(D3DXVECTOR3(SCREEN_WIDTH / 1.25f, SCREEN_HEIGHT / 1.4f, 0.0f), D3DXVECTOR2(200.0f, 170.0f), 150, 5);		//win�\�L
						SetUI(D3DXVECTOR3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 1.4f, 0.0f), D3DXVECTOR2(200.0f, 170.0f), 150, 6);		//lose�\�L
					}
				}
				if (g_nCntScoreTime >= 300)
				{//�X�R�A�A�Q�[����Ԃ����Z�b�g
					ResetGame();
					g_aScore[PLAYER_1].nScore = 0;
					g_aScore[PLAYER_2].nScore = 0;
					g_nCntScoreTime = 0;
				}
				if (g_nCntScoreTime == 450)
				{//�^�C�g���ɖ߂�
					SetFade(MODE_TITLE);
					g_nCntScoreTime = 0;
				}
			}
			else
			{//�Q�[����Ԃ����Z�b�g
				ResetGame();
				g_nCntScoreTime = 0;
			}
		}
	}
}

//========================================================
//					���_�̕`�揈��
//========================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntScore = 0; nCntScore < NUM_SCORE * MAX_PLAYER; nCntScore++)
	{
		if (g_bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureScore);

			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,								//�v���~�e�B�u�̎��
				4 * nCntScore,
				2);													//�v���~�e�B�u�̐�	
		}
	}
}

//========================================================
//					���_�̐ݒ菈��
//========================================================
void SetPoint(int nIdxPlayer, D3DXVECTOR3 Bulletpos)
{
	VERTEX_2D *pVtx;
	float aPosTexU[MAX_PLAYER][NUM_SCORE];							//�e���̐��l���i�[

	g_bUse = true;		//�X�R�A���g�p��Ԃɂ���

	if (nIdxPlayer == 0)
	{
		if (Bulletpos.x > FIELD_SIZE_X / 5 || Bulletpos.x < -FIELD_SIZE_X / 5)
		{//�e�̈ʒu�����[
			g_aScore[PLAYER_1].nScore += 3;

			//�T�E���h�Đ�
			PlaySound(SOUND_LABEL_SE_KANKYAKU);
		}
		else
		{//����ȊO
			g_aScore[PLAYER_1].nScore += 5;

			//�T�E���h�Đ�
			PlaySound(SOUND_LABEL_SE_KANKYAKU);
		}
	}
	else if (nIdxPlayer == 1)
	{
		if (Bulletpos.x > FIELD_SIZE_X / 5 || Bulletpos.x < -FIELD_SIZE_X / 5)
		{//�e�̈ʒu�����[
			g_aScore[PLAYER_2].nScore += 3;

			//�T�E���h�Đ�
			PlaySound(SOUND_LABEL_SE_KANKYAKU);
		}
		else
		{//����ȊO
			g_aScore[PLAYER_2].nScore += 5;

			//�T�E���h�Đ�
			PlaySound(SOUND_LABEL_SE_KANKYAKU);
		}
	}

	//�v���C���[[1]
	aPosTexU[0][0] = ((g_aScore[0].nScore % 100) / 10) * 0.1f;
	aPosTexU[0][1] = (g_aScore[0].nScore % 10) * 0.1f;

	//�v���C���[[2]
	aPosTexU[1][0] = ((g_aScore[1].nScore % 100) / 10) * 0.1f;
	aPosTexU[1][1] = (g_aScore[1].nScore % 10) * 0.1f;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	//�v���C���[[1]�̓��_
	for (int nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + aPosTexU[0][nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aPosTexU[0][nCntScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aPosTexU[0][nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aPosTexU[0][nCntScore], 1.0f);

		pVtx += 4;
	}

	//�v���C���[[2]�̓��_
	for (int nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + aPosTexU[1][nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aPosTexU[1][nCntScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aPosTexU[1][nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aPosTexU[1][nCntScore], 1.0f);

		pVtx += 4;
	}

	//���_���W���A�����b�N
	g_pVtxBuffScore->Unlock();
}

//========================================================
//					�Q�[���̃��Z�b�g����
//========================================================
void ResetGame(void)
{
	Bullet *pBullet = GetBullet();		//�f�B�X�N���̎擾
	Player *pPlayer = GetPlayer();		//�v���C���[���̎擾

	g_bUse = false;				//���_�\��������
	g_nCntScoreTime = 0;		//�J�E���^�[���Z�b�g

	//�e�����Z�b�g(�Ďˏo)����
	pBullet->bUse = false;
	if (g_nCntChange == PLAYER_1)
	{//1P�ɍĎˏo
		SetBullet(D3DXVECTOR3(FIELD_SIZE_X / 2, 30.0f, 0.0f), D3DXVECTOR3(-2.0f, 0.0f, -3.0f), 0);
		g_nCntChange++;
		DiscChange(PLAYER_2);
	}

	else if (g_nCntChange == PLAYER_2)
	{//2P�ɍĎˏo
		SetBullet(D3DXVECTOR3(FIELD_SIZE_X / 2, 30.0f, 0.0f), D3DXVECTOR3(-2.0f, 0.0f, 3.0f), 0);
		g_nCntChange--;
		DiscChange(PLAYER_1);
	}

	//1P�̏�Ԃƈʒu��������
	pPlayer->pos = D3DXVECTOR3(0.0f, 0.0f, -(FIELD_SIZE_Z - PLAYER_WIDTH));
	pPlayer->State = PLAYERSTATE_NORMAL;
	pPlayer++;

	//2P�̏�Ԃƈʒu��������
	pPlayer->pos = D3DXVECTOR3(0.0f, 0.0f, FIELD_SIZE_Z - PLAYER_WIDTH);
	pPlayer->State = PLAYERSTATE_NORMAL;
}

//========================================================
//					���U���g�^�C���̕\������
//========================================================
//void DrawResultTime(int nResultTime)
//{
//	VERTEX_2D *pVtx;
//	float aPosTexU[NUM_SCORE];									//�e���̐��l�����[
//
//	//�e���̐��l���v�Z
//	aPosTexU[0] = ((nResultTime % 1000) / 100) * 0.1f;
//	aPosTexU[1] = ((nResultTime % 100) / 10) * 0.1f;
//	aPosTexU[2] = (nResultTime % 10) * 0.1f;
//
//	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
//	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (int nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
//	{
//		//���_���W�̐ݒ�
//		pVtx[0].pos = D3DXVECTOR3(570.0f + (60.0f * nCntScore), 220.0f, 0.0f);
//		pVtx[1].pos = D3DXVECTOR3(630.0f + (60.0f * nCntScore), 220.0f, 0.0f);
//		pVtx[2].pos = D3DXVECTOR3(570.0f + (60.0f * nCntScore), 300.0f, 0.0f);
//		pVtx[3].pos = D3DXVECTOR3(630.0f + (60.0f * nCntScore), 300.0f, 0.0f);
//
//		//���_�J���[�̐ݒ�
//		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//		//�e�N�X�`���̐ݒ�
//		pVtx[0].tex = D3DXVECTOR2(0.0f + aPosTexU[nCntScore], 0.0f);
//		pVtx[1].tex = D3DXVECTOR2(0.1f + aPosTexU[nCntScore], 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(0.0f + aPosTexU[nCntScore], 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(0.1f + aPosTexU[nCntScore], 1.0f);
//
//		pVtx += 4;
//	}
//
//	//���_���W���A�����b�N
//	g_pVtxBuffScore->Unlock();
//
//	DrawScore();
//}