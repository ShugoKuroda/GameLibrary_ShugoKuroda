//=============================================================================
//
//	�Q�[����ʂ�UI���� [ui.cpp]
//	Author : ���c ����
//
//=============================================================================

//======================================================
//	�C���N���[�h
//======================================================
#include "main.h"
#include "ui.h"

//======================================================
//	�}�N����`
//======================================================
#define MAX_UI		(10)			//UI��ʂŕK�v�ȉ摜��

//======================================================
//	�O���[�o���ϐ�
//======================================================
LPDIRECT3DTEXTURE9 g_pTextureUI[MAX_UI] = { NULL };				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUI[MAX_UI] = { NULL };		//���_�o�b�t�@�ւ̃|�C���^
UI g_aUI[MAX_UI];		//UI���
int g_nCntUI;			//UI���o���܂ł̃J�E���^�[

//======================================================
//	UI��ʂ̏���������
//======================================================
void InitUI(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,		//1P
		"data/TEXTURE/1P000.png",
		&g_pTextureUI[0]);
	D3DXCreateTextureFromFile(pDevice,		//2P
		"data/TEXTURE/2P000.png",
		&g_pTextureUI[1]);
	D3DXCreateTextureFromFile(pDevice,		//Set1
		"data/TEXTURE/set001.png",
		&g_pTextureUI[2]);
	D3DXCreateTextureFromFile(pDevice,		//Set2
		"data/TEXTURE/set002.png",
		&g_pTextureUI[3]);
	D3DXCreateTextureFromFile(pDevice,		//SetFinal
		"data/TEXTURE/setFinal.png",
		&g_pTextureUI[4]);
	D3DXCreateTextureFromFile(pDevice,		//win
		"data/TEXTURE/win000.png",
		&g_pTextureUI[5]);
	D3DXCreateTextureFromFile(pDevice,		//lose
		"data/TEXTURE/lose000.png",
		&g_pTextureUI[6]);
	D3DXCreateTextureFromFile(pDevice,		//SetCount�\�L
		"data/TEXTURE/setcount000.png",
		&g_pTextureUI[7]);
	D3DXCreateTextureFromFile(pDevice,		//1Set15point�\�L
		"data/TEXTURE/1set15points000.png",
		&g_pTextureUI[8]);

	//���_�o�b�t�@�̐���
	for (int nCnt = 0; nCnt < MAX_UI; nCnt++)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffUI[nCnt],
			NULL);
	}

	//UI���̏�����
	ZeroMemory(&g_aUI, sizeof(g_aUI));
	g_nCntUI = 0;

	SetUI(D3DXVECTOR3(100.0f, 25.0f, 0.0f), D3DXVECTOR2(200.0f, 50.0f),-1,0);		//1P
	SetUI(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, 25.0f, 0.0f), D3DXVECTOR2(200.0f, 50.0f), -1,1);		//2P
	SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, 25.0f, 0.0f), D3DXVECTOR2(200.0f, 50.0f), -1, 8);			//�X�R�A�\�L

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	for (int nCnt = 0; nCnt < MAX_UI; nCnt++)
	{
		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffUI[nCnt]->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aUI[nCnt].pos.x - (g_aUI[nCnt].size.x / 2), g_aUI[nCnt].pos.y - (g_aUI[nCnt].size.y / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aUI[nCnt].pos.x + (g_aUI[nCnt].size.x / 2), g_aUI[nCnt].pos.y - (g_aUI[nCnt].size.y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aUI[nCnt].pos.x - (g_aUI[nCnt].size.x / 2), g_aUI[nCnt].pos.y + (g_aUI[nCnt].size.y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aUI[nCnt].pos.x + (g_aUI[nCnt].size.x / 2), g_aUI[nCnt].pos.y + (g_aUI[nCnt].size.y / 2), 0.0f);

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
		g_pVtxBuffUI[nCnt]->Unlock();
	}
}

//======================================================
//	UI��ʂ̏I������
//======================================================
void UninitUI(void)
{
	for (int nCnt = 0; nCnt < MAX_UI; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureUI[nCnt] != NULL)
		{
			g_pTextureUI[nCnt]->Release();
			g_pTextureUI[nCnt] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < MAX_UI; nCnt++)
	{
		//���_�o�b�t�@�̔j��
		if (g_pVtxBuffUI[nCnt] != NULL)
		{
			g_pVtxBuffUI[nCnt]->Release();
			g_pVtxBuffUI[nCnt] = NULL;
		}
	}
}

//======================================================
//	UI��ʂ̍X�V����
//======================================================
void UpdateUI(void)
{
	for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
	{
		if (g_aUI[nCntUI].nCntUse != -1)
		{//�\���J�E���^�[��-1�̏ꍇ�A��ɕ\������

			if (g_aUI[nCntUI].nCntUse <= 0)
			{//�J�E���^�[0�Ŕ�\��	
				g_aUI[nCntUI].bUse = false;
			}
			else
			{
				g_aUI[nCntUI].nCntUse--;		//�\���J�E���^�[���Z
			}
		}
	}
}

//======================================================
//	UI��ʂ̕`�揈��
//======================================================
void DrawUI(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_UI; nCnt++)
	{
		if (g_aUI[nCnt].bUse)
		{
			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffUI[nCnt], 0, sizeof(VERTEX_2D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureUI[nCnt]);

			//�v���C���[�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
				0,		//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);		//�v���~�e�B�u�i�|���S���j��
		}
	}
}

//======================================================
//	UI�̐ݒ菈��
//======================================================
void SetUI(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nCntUse, int nCntUI)
{
	g_aUI[nCntUI].pos = pos;			//�ʒu
	g_aUI[nCntUI].size = size;			//�T�C�Y
	g_aUI[nCntUI].nCntUse = nCntUse;	//�\������
	g_aUI[nCntUI].bUse = true;			//�\����Ԃɂ���

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUI[nCntUI]->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].size.x / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].size.x / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].size.x / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].size.x / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].size.y / 2), 0.0f);

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
	g_pVtxBuffUI[nCntUI]->Unlock();
}