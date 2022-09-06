#include "main.h"
#include "game_logo.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTexturelogo[2] = { NULL };	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufflogo = NULL;		//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_poslogo;		//�Q�[�����̃��S�ʒu
int g_Typelogo;				//�Q�[�����̃��S���
bool g_blogo;				//�Q�[�����Ƀ��S�����邩�ǂ���

//======================================================
//		�Q�[�����̃��S����������
//======================================================
void InitLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���S�̂��낢�돉����
	g_poslogo = D3DXVECTOR3(SCREEN_WIDTH + 400.0f, SCREEN_HEIGHT / 2, 0.0f);
	g_Typelogo = 0;
	g_blogo = false;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/start000.png",
		&g_pTexturelogo[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/warning000.png",
		&g_pTexturelogo[1]);

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

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_poslogo.x - LOGO_SIZE_X, g_poslogo.y - LOGO_SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_poslogo.x + LOGO_SIZE_X, g_poslogo.y - LOGO_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_poslogo.x - LOGO_SIZE_X, g_poslogo.y + LOGO_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_poslogo.x + LOGO_SIZE_X, g_poslogo.y + LOGO_SIZE_Y, 0.0f);

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

		//���_�o�b�t�@�̔j��
		if (g_pVtxBufflogo != NULL)
		{
			g_pVtxBufflogo->Release();
			g_pVtxBufflogo = NULL;
		}
	}
}

//======================================================
//		�Q�[�����̃��S�̍X�V����
//======================================================
void UpdateLogo(void)
{
	if (g_blogo == true)
	{
		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBufflogo->Lock(0, 0, (void**)&pVtx, 0);

		g_poslogo += D3DXVECTOR3(-10.0f, 0.0f, 0.0f);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_poslogo.x - LOGO_SIZE_X, g_poslogo.y - LOGO_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_poslogo.x + LOGO_SIZE_X, g_poslogo.y - LOGO_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_poslogo.x - LOGO_SIZE_X, g_poslogo.y + LOGO_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_poslogo.x + LOGO_SIZE_X, g_poslogo.y + LOGO_SIZE_Y, 0.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//���_�o�b�t�@���A�����b�N
		g_pVtxBufflogo->Unlock();

		if (g_poslogo.x < -500.0f || g_poslogo.x > SCREEN_WIDTH + 500.0f)
		{
			g_blogo = false;
		}
	}
}

//======================================================
//		�Q�[�����̃��S�̕`�揈��
//======================================================
void DrawLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBufflogo, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturelogo[g_Typelogo]);

	if (g_blogo == true)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			0,				//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);				//�v���~�e�B�u�i�|���S���j��
	}
}

//======================================================
//		�Q�[�����̃��S�̐ݒ菈��
//======================================================
void Setlogo(int nType)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	g_poslogo = D3DXVECTOR3(SCREEN_WIDTH + 400.0f, SCREEN_HEIGHT / 2, 0.0f);
	g_Typelogo = nType;
	g_blogo = true;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBufflogo->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_poslogo.x - LOGO_SIZE_X, g_poslogo.y - LOGO_SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_poslogo.x + LOGO_SIZE_X, g_poslogo.y - LOGO_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_poslogo.x - LOGO_SIZE_X, g_poslogo.y + LOGO_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_poslogo.x + LOGO_SIZE_X, g_poslogo.y + LOGO_SIZE_Y, 0.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBufflogo->Unlock();
}