//===================================================================
//
//	�����L���O����[rank.cpp]
//	Author:���c ����
//
//===================================================================

//======================================================
//	�C���N���[�h
//======================================================
#include "main.h"
#include "rank.h"
#include "score.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include <stdio.h>

//======================================================
//	�O���[�o���ϐ�
//======================================================
LPDIRECT3DTEXTURE9 g_pTextureRank[MAX_RANK_IMAGE] = { NULL };			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureRank2 = NULL;								//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank[MAX_RANK_IMAGE] = { NULL };		//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank2 = NULL;							//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3	g_posRankNum;			//�����L���O���ʂ̕\���ʒu
D3DXVECTOR3	g_posRankRogo;			//�����L���O���S�̕\���ʒu
RankScore g_aRankScore[MAX_RANK];	//�����L���O�X�R�A�̏��
int g_nPlayerRank;					//�v���C���[�̏���
bool g_bGameEndPush;				//�Q�[���I�����o�̌J��Ԃ��h�~

//======================================================
//	�����L���O�̏���������
//======================================================
void InitRank(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bg001.jpg",		//�w�i�̃e�N�X�`��
		&g_pTextureRank[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/rank000.png",		//�����L���O���ʕ\���̃e�N�X�`��
		&g_pTextureRank[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/rank001.png",		//�����L���O���S�̃e�N�X�`��
		&g_pTextureRank[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/You000.png",		//You�̃e�N�X�`��
		&g_pTextureRank[3]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number000.png",	//�X�R�A�\���̃e�N�X�`��
		&g_pTextureRank2);

	//���_�o�b�t�@�̐���
	for (int nCnt = 0; nCnt < MAX_RANK_IMAGE; nCnt++)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffRank[nCnt],
			NULL);
	}
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUM * MAX_RANK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRank2,
		NULL);

	//�e�N�X�`���\���ʒu�̏�����
	g_posRankNum = D3DXVECTOR3(SCREEN_WIDTH / 3.2f, SCREEN_HEIGHT / 2.05f, 0.0f);
	g_posRankRogo = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 50.0f, 0.0f);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank[0]->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffRank[0]->Unlock();


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank[1]->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_posRankNum.x - 70.0f, g_posRankNum.y - 250.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posRankNum.x + 70.0f, g_posRankNum.y - 250.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posRankNum.x - 70.0f, g_posRankNum.y + 250.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posRankNum.x + 70.0f, g_posRankNum.y + 250.0f, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffRank[1]->Unlock();


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank[2]->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_posRankRogo.x - 300.0f, g_posRankRogo.y - 40.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posRankRogo.x + 300.0f, g_posRankRogo.y - 40.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posRankRogo.x - 300.0f, g_posRankRogo.y + 40.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posRankRogo.x + 300.0f, g_posRankRogo.y + 40.0f, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffRank[2]->Unlock();


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank2->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt1 = 0; nCnt1 < MAX_RANK; nCnt1++)
	{
		for (int nCnt2 = 0; nCnt2 < MAX_NUM; nCnt2++)
		{
			g_aRankScore[nCnt1].pos = D3DXVECTOR3(SCREEN_WIDTH + 150.0f, SCREEN_HEIGHT / 4.5f, 0.0f); //�X�R�A�̏ꏊ��������

			 //���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aRankScore[nCnt1].pos.x + (35.0f * nCnt2) - 15.0f, g_aRankScore[nCnt1].pos.y + (90.0f * nCnt1) - 45.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aRankScore[nCnt1].pos.x + (35.0f * nCnt2) + 15.0f, g_aRankScore[nCnt1].pos.y + (90.0f * nCnt1) - 45.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aRankScore[nCnt1].pos.x + (35.0f * nCnt2) - 15.0f, g_aRankScore[nCnt1].pos.y + (90.0f * nCnt1) + 45.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aRankScore[nCnt1].pos.x + (35.0f * nCnt2) + 15.0f, g_aRankScore[nCnt1].pos.y + (90.0f * nCnt1) + 45.0f, 0.0f);

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

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;  //���_�f�[�^�̃|�C���^���S���i�߂�
		}
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffRank2->Unlock();

	//�Q�[���I�����o�h�~�̏�����
	g_bGameEndPush = false;

	//�����L���O�̐��l�����[�h
	LoadScore();

	//�v���C���[�̃X�R�A�Q��
	g_aRankScore[5].nScore = GetScore();

	//�����L���O�̍X�V
	int nData = 0;
	for (int nCnt1 = 0; nCnt1 < MAX_RANK - 1; nCnt1++)
	{
		for (int nCnt2 = (nCnt1 + 1); nCnt2 < MAX_RANK; nCnt2++)
		{
			if (g_aRankScore[nCnt1].nScore <= g_aRankScore[nCnt2].nScore)
			{
				nData = g_aRankScore[nCnt2].nScore;
				g_aRankScore[nCnt2].nScore = g_aRankScore[nCnt1].nScore;
				g_aRankScore[nCnt1].nScore = nData;
			}
		}
	}

	//�ߋ������L���O�̃X�R�A�Q��
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		SetRank(g_aRankScore[nCnt].nScore, nCnt);
	}

	//�����L���O�̃Z�[�u
	SaveScore();

	//�v���C���[�̃X�R�A���ʂ̃`�F�b�N
	g_nPlayerRank = 0;

	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		if (g_aRankScore[nCnt].nScore == GetScore())
		{
			g_nPlayerRank = nCnt;
		}
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank[3]->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aRankScore[g_nPlayerRank].pos.x - 80.0f - 260.0f, g_aRankScore[g_nPlayerRank].pos.y + (91.0f * g_nPlayerRank) - 40.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aRankScore[g_nPlayerRank].pos.x + 80.0f - 260.0f, g_aRankScore[g_nPlayerRank].pos.y + (91.0f * g_nPlayerRank) - 40.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aRankScore[g_nPlayerRank].pos.x - 80.0f - 260.0f, g_aRankScore[g_nPlayerRank].pos.y + (91.0f * g_nPlayerRank) + 40.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aRankScore[g_nPlayerRank].pos.x + 80.0f - 260.0f, g_aRankScore[g_nPlayerRank].pos.y + (91.0f * g_nPlayerRank) + 40.0f, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffRank[3]->Unlock();

	//�T�E���h�Đ�
	PlaySound(SOUND_LABEL_BGM002);
}

//======================================================
//	�����L���O�̏I������
//======================================================
void UninitRank(void)
{
	//�T�E���h��~
	StopSound();

	for (int nCnt = 0; nCnt < MAX_RANK_IMAGE; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureRank[nCnt] != NULL)
		{
			g_pTextureRank[nCnt]->Release();
			g_pTextureRank[nCnt] = NULL;
		}

		//���_�o�b�t�@�̔j��
		if (g_pVtxBuffRank[nCnt] != NULL)
		{
			g_pVtxBuffRank[nCnt]->Release();
			g_pVtxBuffRank[nCnt] = NULL;
		}
	}
	//�e�N�X�`���̔j��
	if (g_pTextureRank2 != NULL)
	{
		g_pTextureRank2->Release();
		g_pTextureRank2 = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffRank2 != NULL)
	{
		g_pVtxBuffRank2->Release();
		g_pVtxBuffRank2 = NULL;
	}
}

//======================================================
//	�����L���O�̍X�V����
//======================================================
void UpdateRank(void)
{
	if (g_bGameEndPush == false)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//�G���^�[�L�[���������ꍇ
			if (g_aRankScore[0].pos.x <= SCREEN_WIDTH / 2.5f)
			{
				//���[�h�ڍs
				SetFade(MODE_TITLE);

				//���U���g���o�̌J��Ԃ��h�~
				g_bGameEndPush = true;
			}
			if (g_aRankScore[0].pos.x > SCREEN_WIDTH / 2.5f)
			{
				for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
				{
					g_aRankScore[nCnt].pos.x = SCREEN_WIDTH / 2.5f;
				}
			}
		}
	}

	//�����L���O�X�R�A�̈ړ�����
	if (g_aRankScore[0].pos.x > SCREEN_WIDTH / 2.5f)
	{
		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			g_aRankScore[nCnt].pos.x -= 10.0f;
		}
	}

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank[3]->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aRankScore[g_nPlayerRank].pos.x - 80.0f - 260.0f, g_aRankScore[g_nPlayerRank].pos.y + (100.0f * g_nPlayerRank) - 40.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aRankScore[g_nPlayerRank].pos.x + 80.0f - 260.0f, g_aRankScore[g_nPlayerRank].pos.y + (100.0f * g_nPlayerRank) - 40.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aRankScore[g_nPlayerRank].pos.x - 80.0f - 260.0f, g_aRankScore[g_nPlayerRank].pos.y + (100.0f * g_nPlayerRank) + 40.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aRankScore[g_nPlayerRank].pos.x + 80.0f - 260.0f, g_aRankScore[g_nPlayerRank].pos.y + (100.0f * g_nPlayerRank) + 40.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffRank[3]->Unlock();


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank2->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt1 = 0; nCnt1 < MAX_RANK; nCnt1++)
	{
		for (int nCnt2 = 0; nCnt2 < MAX_NUM; nCnt2++)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aRankScore[nCnt1].pos.x + (40.0f * nCnt2) - 20.0f, g_aRankScore[nCnt1].pos.y + (100.0f * nCnt1) - 50.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aRankScore[nCnt1].pos.x + (40.0f * nCnt2) + 20.0f, g_aRankScore[nCnt1].pos.y + (100.0f * nCnt1) - 50.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aRankScore[nCnt1].pos.x + (40.0f * nCnt2) - 20.0f, g_aRankScore[nCnt1].pos.y + (100.0f * nCnt1) + 50.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aRankScore[nCnt1].pos.x + (40.0f * nCnt2) + 20.0f, g_aRankScore[nCnt1].pos.y + (100.0f * nCnt1) + 50.0f, 0.0f);

			pVtx += 4;  //���_�f�[�^�̃|�C���^���S���i�߂�
		}
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffRank2->Unlock();
}

//======================================================
//	�����L���O�̕`�揈��
//======================================================
void DrawRank(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_RANK_IMAGE; nCnt++)
	{
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffRank[nCnt], 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureRank[nCnt]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			0,		//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);		//�v���~�e�B�u�i�|���S���j��
	}


	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRank2, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureRank2);

	for (int nCnt = 0; nCnt < MAX_RANK * MAX_NUM; nCnt++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			4 * nCnt,		//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);				//�v���~�e�B�u�i�|���S���j��
	}
}

//======================================================
//	�����L���O�̐ݒ菈��
//======================================================
void SetRank(int nScore, int nCntScore)
{
	int aPosTexU[MAX_NUM];	//�e�ʂ̐������i�[

	aPosTexU[0] = nScore % 100000000 / 10000000;
	aPosTexU[1] = nScore % 10000000 / 1000000;
	aPosTexU[2] = nScore % 1000000 / 100000;
	aPosTexU[3] = nScore % 100000 / 10000;
	aPosTexU[4] = nScore % 10000 / 1000;
	aPosTexU[5] = nScore % 1000 / 100;
	aPosTexU[6] = nScore % 100 / 10;
	aPosTexU[7] = nScore % 10 / 1;

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank2->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * MAX_NUM * nCntScore;  //���_�f�[�^�̃|�C���^���S���i�߂�

	for (int nCnt = 0; nCnt < MAX_NUM; nCnt++)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCnt], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCnt] + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCnt], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCnt] + 1), 1.0f);

		pVtx += 4;  //���_�f�[�^�̃|�C���^���S���i�߂�
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffRank2->Unlock();
}

//======================================================
//	�����L���O�̃��[�h����
//======================================================
void LoadScore(void)
{
	FILE *pFile;			//�t�@�C���|�C���^�[�錾

	//�t�@�C�����J��
	pFile = fopen("Rank.txt", "r");

	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < 5; nCnt++)
		{
			fscanf(pFile, "%d", &g_aRankScore[nCnt].nScore);
		}
	}
	else
	{
		printf("�t�@�C�����J���܂���ł���\n");
	}

	//�t�@�C�������
	fclose(pFile);
}

//======================================================
//	�����L���O�̃Z�[�u����
//======================================================
void SaveScore(void)
{
	FILE *pFile;			//�t�@�C���|�C���^�[�錾

	//�t�@�C�����J��
	pFile = fopen("Rank.txt", "w");

	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < 5; nCnt++)
		{
			fprintf(pFile, "%d\n", g_aRankScore[nCnt].nScore);
		}
	}
	else
	{
		printf("�t�@�C�����J���܂���ł���\n");
	}

	//�t�@�C�������
	fclose(pFile);
}