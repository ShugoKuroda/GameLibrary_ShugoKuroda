//===================================================================
//
//	�Q�[�����̎w������[logo.cpp]
//	Author:���c ����
//
//===================================================================

//======================================================
//	�C���N���[�h
//======================================================
#include "main.h"
#include "logo.h"
#include "fade.h"

//======================================================
//	�\����
//======================================================
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	int nCntDraw;			//�o���J�E���^�[
	int nType;				//���
	bool bUse;				//�g�p����Ă��邩�ǂ���
}Logo;

//======================================================
//	�O���[�o���ϐ�
//======================================================
LPDIRECT3DTEXTURE9 g_pTexturelogo[MAX_LOGO] = { NULL };	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufflogo = NULL;			//���_�o�b�t�@�ւ̃|�C���^
Logo g_Logo[MAX_LOGO];			//���S���

//======================================================
//	�Q�[�����̃��S����������
//======================================================
void InitLogo(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���S���̏�����
	ZeroMemory(&g_Logo, sizeof(g_Logo));

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Ready000.png",
		&g_pTexturelogo[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Start000.png",
		&g_pTexturelogo[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Victory000.png",
		&g_pTexturelogo[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Lose000.png",
		&g_pTexturelogo[3]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBufflogo,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBufflogo->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̏�����
	for (int nCnt = 0; nCnt < MAX_LOGO; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Logo[nCnt].pos.x - LOGO_SIZE_X, g_Logo[nCnt].pos.y - LOGO_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Logo[nCnt].pos.x + LOGO_SIZE_X, g_Logo[nCnt].pos.y - LOGO_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Logo[nCnt].pos.x - LOGO_SIZE_X, g_Logo[nCnt].pos.y + LOGO_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Logo[nCnt].pos.x + LOGO_SIZE_X, g_Logo[nCnt].pos.y + LOGO_SIZE_Y, 0.0f);

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
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBufflogo->Unlock();
}

//======================================================
//		�Q�[�����̃��S�̏I������
//======================================================
void UninitLogo(void)
{
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < MAX_LOGO; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_pTexturelogo[nCnt] != NULL)
		{
			g_pTexturelogo[nCnt]->Release();
			g_pTexturelogo[nCnt] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBufflogo != NULL)
	{
		g_pVtxBufflogo->Release();
		g_pVtxBufflogo = NULL;
	}
}

//======================================================
//		�Q�[�����̃��S�̍X�V����
//======================================================
void UpdateLogo(void)
{
	for (int nCnt = 0; nCnt < MAX_LOGO; nCnt++)
	{
		if (g_Logo[nCnt].bUse)
		{
			g_Logo[nCnt].nCntDraw++;		//�J�E���^�[�̉��Z

			if (g_Logo[nCnt].nType == 3 || g_Logo[nCnt].nType == 2)
			{
				if (g_Logo[nCnt].nCntDraw >= 180)
				{//�J�E���^�[����萔
					//���[�h�ڍs
					SetFade(MODE_RANK);
					g_Logo[nCnt].nCntDraw = 0;		//�J�E���^�[���Z�b�g
				}
			}
			else
			{
				if (g_Logo[nCnt].nCntDraw >= 90)
				{//�J�E���^�[����萔
					g_Logo[nCnt].bUse = false;		//���S������
					g_Logo[nCnt].nCntDraw = 0;		//�J�E���^�[���Z�b�g
				}
			}
		}
	}
}

//======================================================
//		�Q�[�����̃��S�̕`�揈��
//======================================================
void DrawLogo(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_LOGO; nCnt++)
	{
		if (g_Logo[nCnt].bUse)
		{
			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBufflogo, 0, sizeof(VERTEX_2D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexturelogo[g_Logo[nCnt].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
				0,				//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);				//�v���~�e�B�u�i�|���S���j��
		}
	}
}

//======================================================
//		�Q�[�����̃��S�̐ݒ菈��
//======================================================
void SetLogo(int nType)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	for (int nCnt = 0; nCnt < MAX_LOGO; nCnt++)
	{
		if (!g_Logo[nCnt].bUse)
		{
			g_Logo[nCnt].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
			g_Logo[nCnt].nType = nType;
			g_Logo[nCnt].bUse = true;

			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBufflogo->Lock(0, 0, (void**)&pVtx, 0);

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_Logo[nCnt].pos.x - LOGO_SIZE_X, g_Logo[nCnt].pos.y - LOGO_SIZE_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Logo[nCnt].pos.x + LOGO_SIZE_X, g_Logo[nCnt].pos.y - LOGO_SIZE_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Logo[nCnt].pos.x - LOGO_SIZE_X, g_Logo[nCnt].pos.y + LOGO_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Logo[nCnt].pos.x + LOGO_SIZE_X, g_Logo[nCnt].pos.y + LOGO_SIZE_Y, 0.0f);

			//���_�o�b�t�@���A�����b�N
			g_pVtxBufflogo->Unlock();

			break;
		}
	}
}