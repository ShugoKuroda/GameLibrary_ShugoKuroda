#include "main.h"
#include "life.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureLife = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;		//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posLife;		//���C�t�̈ʒu
int g_nLife;				//���݂̃��C�t

//======================================================
//		�v���C���[�c�@�̏���������
//======================================================
void InitLife(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���C�t�̈ʒu�̏�����
	g_posLife = D3DXVECTOR3(10.0f, 10.0f, 0.0f);

	//���݂̃��C�t�̏�����
	g_nLife = MAX_LIFE;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Player001.png",
		&g_pTextureLife);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LIFE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_LIFE; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posLife.x + LIFE_SIZE_X * nCnt, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posLife.x + LIFE_SIZE_X * (nCnt + 1), 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posLife.x + LIFE_SIZE_X * nCnt, LIFE_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posLife.x + LIFE_SIZE_X * (nCnt + 1), LIFE_SIZE_Y, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

		pVtx += 4;  //���_�f�[�^�̃|�C���^���S���i�߂�
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffLife->Unlock();
}

//======================================================
//		�v���C���[�c�@�̏I������
//======================================================
void UninitLife(void)
{
		//�e�N�X�`���̔j��
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}

		//���_�o�b�t�@�̔j��
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}
}

//======================================================
//		�v���C���[�c�@�̍X�V����
//======================================================
void UpdateLife(void)
{

}

//======================================================
//		�v���C���[�c�@�̕`�揈��
//======================================================
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffLife, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureLife);

	for (int nCnt = 0; nCnt < g_nLife; nCnt++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			4 * nCnt,		//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);				//�v���~�e�B�u�i�|���S���j��
	}
}

//======================================================
//		�v���C���[�c�@�̌�������
//======================================================
void DownLifePlayer(void)
{
	g_nLife--;
}