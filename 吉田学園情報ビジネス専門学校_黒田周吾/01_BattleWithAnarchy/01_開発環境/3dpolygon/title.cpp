//=============================================================================
//
//	�^�C�g������ [title.cpp]
//	Author : ���c ����
//
//=============================================================================

//======================================================
//======================================================
#include "main.h"
#include "title.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "player.h"

//======================================================
//	�O���[�o���ϐ�
//======================================================
LPDIRECT3DTEXTURE9 g_pTextureTitle[MAX_TITLE] = { NULL };			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle[MAX_TITLE] = { NULL };		//���_�o�b�t�@�ւ̃|�C���^
bool g_GameStartPush;			//�X�^�[�g���o�̌J��Ԃ��h�~
Title g_aTitle[2];				//�^�C�g�����
int g_nCounterAnimBg;
int g_nPatternAnimBg;

//======================================================
//	�^�C�g����ʂ̏���������
//======================================================
void InitTitle(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/titlebg000.jpg",
		&g_pTextureTitle[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/titlename000.png",
		&g_pTextureTitle[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/PressEnter000.png",
		&g_pTextureTitle[2]);

	//���_�o�b�t�@�̐���
	for (int nCnt = 0; nCnt < MAX_TITLE; nCnt++)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffTitle[nCnt],
			NULL);
	}

	//�^�C�g�����̏�����
	ZeroMemory(&g_aTitle, sizeof(g_aTitle));
	g_aTitle[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3.0f, 0.0f);
	g_aTitle[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.2f, 0.0f);
	g_nCounterAnimBg = 0;
	g_nPatternAnimBg = 0;

	g_GameStartPush = false;		//�X�^�[�g���o�J��Ԃ��h�~�̏�����
	
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle[0]->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitle[0]->Unlock();

	for (int nCnt = 1; nCnt < MAX_TITLE; nCnt++)
	{
		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffTitle[nCnt]->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aTitle[nCnt].pos.x - (TITLE_SIZE_X / 2), g_aTitle[nCnt].pos.y - (TITLE_SIZE_Y / nCnt), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTitle[nCnt].pos.x + (TITLE_SIZE_X / 2), g_aTitle[nCnt].pos.y - (TITLE_SIZE_Y / nCnt), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTitle[nCnt].pos.x - (TITLE_SIZE_X / 2), g_aTitle[nCnt].pos.y + (TITLE_SIZE_Y / nCnt), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTitle[nCnt].pos.x + (TITLE_SIZE_X / 2), g_aTitle[nCnt].pos.y + (TITLE_SIZE_Y / nCnt), 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//���_�o�b�t�@���A�����b�N
		g_pVtxBuffTitle[nCnt]->Unlock();
	}

	//�T�E���h�Đ�
	PlaySound(SOUND_LABEL_BGM000);
}

//======================================================
//	�^�C�g����ʂ̏I������
//======================================================
void UninitTitle(void)
{
	//�T�E���h��~
	StopSound();

	for (int nCnt = 0; nCnt < MAX_TITLE; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureTitle[nCnt] != NULL)
		{
			g_pTextureTitle[nCnt]->Release();
			g_pTextureTitle[nCnt] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < MAX_TITLE; nCnt++)
	{
		//���_�o�b�t�@�̔j��
		if (g_pVtxBuffTitle[nCnt] != NULL)
		{
			g_pVtxBuffTitle[nCnt]->Release();
			g_pVtxBuffTitle[nCnt] = NULL;
		}
	}
}

//======================================================
//	�^�C�g����ʂ̍X�V����
//======================================================
void UpdateTitle(void)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	if (g_GameStartPush == false)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//�G���^�[�L�[�������ꂽ��
			//���[�h�ڍs
			SetFade(MODE_GAME);

			//�X�^�[�g���o�̌J��Ԃ��h�~
			g_GameStartPush = true;

			//�T�E���h�Đ�
			PlaySound(SOUND_LABEL_SE_SELECT2);
		}
	}

	g_nCounterAnimBg++;		//�J�E���^�[�����Z

	if (g_nCounterAnimBg % 2 == 0)
	{
		g_nCounterAnimBg = 0;
		g_nPatternAnimBg = (g_nPatternAnimBg + 1) % 1000;		//�p�^�[��No.���X�V

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffTitle[0]->Lock(0, 0, (void**)&pVtx, 0);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.002f * (g_nPatternAnimBg % 1000), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.002f * ((g_nPatternAnimBg % 1000) + 500), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.002f * (g_nPatternAnimBg % 1000), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.002f * ((g_nPatternAnimBg % 1000) + 500), 1.0f);

		//���_�o�b�t�@���A�����b�N
		g_pVtxBuffTitle[0]->Unlock();
	}
}

//======================================================
//	�^�C�g����ʂ̕`�揈��
//======================================================
void DrawTitle(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_TITLE; nCnt++)
	{
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffTitle[nCnt], 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTitle[nCnt]);

		//�v���C���[�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			0,		//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);		//�v���~�e�B�u�i�|���S���j��
	}
}