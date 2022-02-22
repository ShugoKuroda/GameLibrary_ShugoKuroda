#include "main.h"
#include "tutorial.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureTutorial[MAX_TUTORIAL] = { NULL };			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial[MAX_TUTORIAL] = { NULL };	//���_�o�b�t�@�ւ̃|�C���^
bool g_GameStartPush2;				//�X�^�[�g���o�̌J��Ԃ��h�~

//======================================================
//		�`���[�g���A����ʂ̏���������
//======================================================
void InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Tutorial000.png",
		&g_pTextureTutorial[0]);

	//���_�o�b�t�@�̐���
	for (int nCnt = 0; nCnt < MAX_TUTORIAL; nCnt++)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffTutorial[nCnt],
			NULL);
	}

	g_GameStartPush2 = false;		//�X�^�[�g���o�J��Ԃ��h�~�̏�����

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorial[0]->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTutorial[0]->Unlock();
}

//======================================================
//		�`���[�g���A����ʂ̏I������
//======================================================
void UninitTutorial(void)
{
	//�T�E���h��~
	StopSound();

	for (int nCnt = 0; nCnt < MAX_TUTORIAL; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureTutorial[nCnt] != NULL)
		{
			g_pTextureTutorial[nCnt]->Release();
			g_pTextureTutorial[nCnt] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < MAX_TUTORIAL; nCnt++)
	{
		//���_�o�b�t�@�̔j��
		if (g_pVtxBuffTutorial[nCnt] != NULL)
		{
			g_pVtxBuffTutorial[nCnt]->Release();
			g_pVtxBuffTutorial[nCnt] = NULL;
		}
	}
}

//======================================================
//		�`���[�g���A����ʂ̍X�V����
//======================================================
void UpdateTutorial(void)
{
	if (g_GameStartPush2 == false)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//�G���^�[�L�[�������ꂽ��
			//���[�h�ڍs
			SetFade(MODE_GAME);

			//�X�^�[�g���o�̌J��Ԃ��h�~
			g_GameStartPush2 = true;
		}
	}
}

//======================================================
//		�`���[�g���A����ʂ̕`�揈��
//======================================================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_TUTORIAL; nCnt++)
	{
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffTutorial[nCnt], 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTutorial[nCnt]);

		//�v���C���[�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			0,		//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);		//�v���~�e�B�u�i�|���S���j��
	}
}