//========================================================================================================
//
//	�^�C�}�[����[time.cpp]
//	Author:SHUGO KURODA
//
//========================================================================================================

//----------------------------------------
//	�C���N���[�h
//----------------------------------------
#include "main.h"
#include "life.h"
#include "player.h"

//----------------------------------------
//	�}�N����`
//----------------------------------------
#define MAX_LIFE		(3)			//�ő�^�C����
#define LIFE_SIZE_X		(20.0f)		//���C�t�\�L�̃T�C�Y(x)
#define LIFE_SIZE_Y		(690.0f)	//���C�t�\�L�̃T�C�Y(y)
#define PLAYER_LIFE		(100)		//�v���C���[�̏������C�t

//----------------------------------------
//	�O���[�o���ϐ�
//----------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureLife = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;		//���_�o�b�t�@�ւ̃|�C���^	
D3DXVECTOR3 g_posLife;		//���C�t�̈ʒu
int g_Life;					//���C�t�̒l
int g_PlayerLife;			//�v���C���[�̃��C�t�l

//----------------------------------------
//	���C�t�̏���������
//----------------------------------------
void InitLife(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureLife);

	g_posLife = D3DXVECTOR3(120.0f, 0.0f, 0.0f);	//�ʒu������������
	g_Life = 0;		//�l������������
	g_PlayerLife = PLAYER_LIFE;			//�v���C���[�̗̑͒l�̏�����

	//���_�o�b�t�@�̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LIFE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife,
		NULL);

	VERTEX_2D *pVtx;		//���_���փ|�C���^

	//���_�o�b�t�@�����b�N���A���_���փ|�C���^�擾
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < MAX_LIFE; nCntTime++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posLife.x + LIFE_SIZE_X * nCntTime, 650.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posLife.x + LIFE_SIZE_X * (nCntTime + 1), 650.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posLife.x + LIFE_SIZE_X * nCntTime, LIFE_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posLife.x + LIFE_SIZE_X * (nCntTime + 1), LIFE_SIZE_Y, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̕`��
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;//�|�C���^���S���i�߂�
	}
	//���_�o�b�t�@�A�����b�N
	g_pVtxBuffLife->Unlock();

	//���C�t�̏����ݒ�
	SetLife(PLAYER_LIFE);
}

//----------------------------------------
//	���C�t�̏I������
//----------------------------------------
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

//----------------------------------------
//	���C�t�̍X�V����
//----------------------------------------
void UpdateLife(void)
{
	//�v���C���[���̎擾
	Player *pPlayer = GetPlayer();

	if (g_PlayerLife <= 0)
	{//�v���C���[�̗̑͂�0
		pPlayer->bIsDie = true;		//�v���C���[�����S��Ԃɂ���
	}
}

//----------------------------------------
//	���C�t�̕`�揈��
//----------------------------------------
void DrawLife(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffLife, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureLife);

	for (int nCntTime = 0; nCntTime < MAX_LIFE; nCntTime++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			nCntTime * 4,
			2);											//�v���~�e�B�u�i�|���S���j��
	}
}

//----------------------------------------
//	���C�t�̐ݒ菈��
//----------------------------------------
void SetLife(int nLife)
{
	int aPosTexU[MAX_LIFE];	//�e���̐������i�[

	g_Life = nLife;

	aPosTexU[0] = g_Life % 1000 / 100;
	aPosTexU[1] = g_Life % 100 / 10;
	aPosTexU[2] = g_Life % 10 / 1;

	VERTEX_2D*pVtx;		//���_���փ|�C���^

	//���_�o�b�t�@�����b�N���A���_���փ|�C���^�擾
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < MAX_LIFE; nCntTime++)
	{
		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntTime], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCntTime] + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntTime], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCntTime] + 1), 1.0f);

		pVtx += 4;//�|�C���^���S���i�߂�
	}

	//���_�o�b�t�@�A�����b�N
	g_pVtxBuffLife->Unlock();
}

//----------------------------------------
//	���C�t�̉��Z����
//----------------------------------------
void AddLife(int nValue)
{
	int aPosTexU[MAX_LIFE];	//�e���̐������i�[

	g_PlayerLife += nValue;
	g_Life += nValue;

	aPosTexU[0] = g_Life % 1000 / 100;
	aPosTexU[1] = g_Life % 100 / 10;
	aPosTexU[2] = g_Life % 10 / 1;

	VERTEX_2D *pVtx;		//���_���փ|�C���^

	//���_�o�b�t�@�����b�N���A���_���փ|�C���^�擾
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < MAX_LIFE; nCntTime++)
	{
		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntTime], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCntTime] + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntTime], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCntTime] + 1), 1.0f);

		pVtx += 4;//�|�C���^���S���i�߂�
	}

	//���_�o�b�t�@�A�����b�N
	g_pVtxBuffLife->Unlock();
}