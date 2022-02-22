#include "main.h"
#include "back.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureBack = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBack = NULL;		//���_�o�b�t�@�ւ̃|�C���^
int g_nCounterAnimBack;								//�w�i�̃A�j���[�V�����J�E���^�[
int g_nPatternAnimBack;								//�w�i�̃A�j���[�V�����p�^�[��

//======================================================
//		�w�i�̏���������
//======================================================
void InitBack(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/back001.png", &g_pTextureBack);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBack,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBack->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = 0.0f;
	pVtx[0].pos.y = 0.0f;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = SCREEN_WIDTH;
	pVtx[1].pos.y = 0.0f;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = 0.0f;
	pVtx[2].pos.y = SCREEN_HEIGHT;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = SCREEN_WIDTH;
	pVtx[3].pos.y = SCREEN_HEIGHT;
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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBack->Unlock();
}

//======================================================
//		�w�i�̏I������
//======================================================
void UninitBack(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBack != NULL)
	{
		g_pTextureBack->Release();
		g_pTextureBack = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBack != NULL)
	{
		g_pVtxBuffBack->Release();
		g_pVtxBuffBack = NULL;
	}
}

//======================================================
//		�w�i�̍X�V����
//======================================================
void UpdateBack(void)
{
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^
	g_nCounterAnimBack++;		//�J�E���^�[�����Z

	if (g_nCounterAnimBack % 6 == 0)
	{
		g_nCounterAnimBack = 0;
		g_nPatternAnimBack = (g_nPatternAnimBack - 1) % 10;		//�p�^�[��No.���X�V

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffBack->Lock(0, 0, (void**)&pVtx, 0);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.1f * (g_nPatternAnimBack % 10), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * ((g_nPatternAnimBack % 10) - 10), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * (g_nPatternAnimBack % 10), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * ((g_nPatternAnimBack % 10) - 10), 1.0f);

		//���_�o�b�t�@���A�����b�N
		g_pVtxBuffBack->Unlock();
	}
}

//======================================================
//		�w�i�̕`�揈��
//======================================================
void DrawBack(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBack, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBack);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,											//�`�悷��ŏ��̒��_�C���f�b�N�X
		2);											//�v���~�e�B�u�i�|���S���j��
}