#include "main.h"
#include "game_tutorial.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureGameTutorial[MAX_GAME_TUTORIAL] = { NULL };	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameTutorial = NULL;				//���_�o�b�t�@�ւ̃|�C���^
GAME_TUTORIAL g_aGameTutorial[MAX_GAME_TUTORIAL];						//�Q�[�����̃`���[�g���A���摜�̏��

//======================================================
//		�Q�[�����̃`���[�g���A���摜����������
//======================================================
void InitGameTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/tutorial000.png",
		&g_pTextureGameTutorial[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/tutorial001.png",
		&g_pTextureGameTutorial[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/tutorial002.png",
		&g_pTextureGameTutorial[2]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GAME_TUTORIAL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameTutorial,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGameTutorial->Lock(0, 0, (void**)&pVtx, 0);

	//�e���ڂ̏�����
	for (int nCnt = 0; nCnt < MAX_GAME_TUTORIAL; nCnt++)
	{
		g_aGameTutorial[nCnt].pos = D3DXVECTOR3(SCREEN_WIDTH + 400.0f, SCREEN_HEIGHT / 2, 0.0f);
		g_aGameTutorial[nCnt].nType = 0;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aGameTutorial[nCnt].pos.x - TUTORIAL_SIZE_X, g_aGameTutorial[nCnt].pos.y - TUTORIAL_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aGameTutorial[nCnt].pos.x + TUTORIAL_SIZE_X, g_aGameTutorial[nCnt].pos.y - TUTORIAL_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aGameTutorial[nCnt].pos.x - TUTORIAL_SIZE_X, g_aGameTutorial[nCnt].pos.y + TUTORIAL_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aGameTutorial[nCnt].pos.x + TUTORIAL_SIZE_X, g_aGameTutorial[nCnt].pos.y + TUTORIAL_SIZE_Y, 0.0f);

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

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffGameTutorial->Unlock();
}

//======================================================
//		�Q�[�����̃`���[�g���A���摜�̏I������
//======================================================
void UninitGameTutorial(void)
{
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < MAX_GAME_TUTORIAL; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureGameTutorial[nCnt] != NULL)
		{
			g_pTextureGameTutorial[nCnt]->Release();
			g_pTextureGameTutorial[nCnt] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffGameTutorial != NULL)
	{
		g_pVtxBuffGameTutorial->Release();
		g_pVtxBuffGameTutorial = NULL;
	}
}

//======================================================
//		�Q�[�����̃`���[�g���A���摜�̍X�V����
//======================================================
void UpdateGameTutorial(void)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGameTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_GAME_TUTORIAL; nCnt++)
	{
		if (g_aGameTutorial[nCnt].bUse == true)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aGameTutorial[nCnt].pos.x - TUTORIAL_SIZE_X, g_aGameTutorial[nCnt].pos.y - TUTORIAL_SIZE_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aGameTutorial[nCnt].pos.x + TUTORIAL_SIZE_X, g_aGameTutorial[nCnt].pos.y - TUTORIAL_SIZE_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aGameTutorial[nCnt].pos.x - TUTORIAL_SIZE_X, g_aGameTutorial[nCnt].pos.y + TUTORIAL_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aGameTutorial[nCnt].pos.x + TUTORIAL_SIZE_X, g_aGameTutorial[nCnt].pos.y + TUTORIAL_SIZE_Y, 0.0f);
		}
		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffGameTutorial->Unlock();
}

//======================================================
//		�Q�[�����̃`���[�g���A���摜�̕`�揈��
//======================================================
void DrawGameTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGameTutorial, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < MAX_GAME_TUTORIAL; nCnt++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureGameTutorial[g_aGameTutorial[nCnt].nType]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			nCnt * 4,		//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);								//�v���~�e�B�u�i�|���S���j��
	}
}

//======================================================
//		�Q�[�����̃`���[�g���A���摜�̐ݒ菈��
//======================================================
void SetGameTutorial(D3DXVECTOR3 pos, int nType)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGameTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_GAME_TUTORIAL; nCnt++)
	{
		if (g_aGameTutorial[nCnt].bUse == false)
		{
			g_aGameTutorial[nCnt].pos = pos;
			g_aGameTutorial[nCnt].nType = nType;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aGameTutorial[nCnt].pos.x - TUTORIAL_SIZE_X, g_aGameTutorial[nCnt].pos.y - TUTORIAL_SIZE_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aGameTutorial[nCnt].pos.x + TUTORIAL_SIZE_X, g_aGameTutorial[nCnt].pos.y - TUTORIAL_SIZE_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aGameTutorial[nCnt].pos.x - TUTORIAL_SIZE_X, g_aGameTutorial[nCnt].pos.y + TUTORIAL_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aGameTutorial[nCnt].pos.x + TUTORIAL_SIZE_X, g_aGameTutorial[nCnt].pos.y + TUTORIAL_SIZE_Y, 0.0f);

			g_aGameTutorial[nCnt].bUse = true;

			break;
		}
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffGameTutorial->Unlock();
}