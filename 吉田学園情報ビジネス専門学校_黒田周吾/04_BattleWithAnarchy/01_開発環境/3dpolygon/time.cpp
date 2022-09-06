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
#include "player.h"
#include "input.h"
#include "enemy.h"
#include "bullet.h"
#include "title.h"
#include "game.h"
#include "score.h"
#include "time.h"
#include "logo.h"

//----------------------------------------
//	�}�N����`
//----------------------------------------
#define MAX_TIME		(3)			//�ő�^�C����
#define TIME_SIZE_X		(20.0f)		//�^�C�}�[�\�L�̃T�C�Y(x)
#define TIME_SIZE_Y		(40.0f)		//�^�C�}�[�\�L�̃T�C�Y(y)

//----------------------------------------
//	�O���[�o���ϐ�
//----------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;		//���_�o�b�t�@�ւ̃|�C���^	
D3DXVECTOR3 g_posTime;		//�^�C���̈ʒu
int g_Time;					//�^�C���̒l
int g_nCntTime;
bool bTimeUp;		//���Ԑ؂ꂩ�ǂ���

//----------------------------------------
//	�^�C�}�[�̏���������
//----------------------------------------
void InitTime(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,"data/TEXTURE/number000.png",&g_pTextureTime);

	//�e�^�C�}�[���̏�����
	g_posTime = D3DXVECTOR3(490.0f, 40.0f, 0.0f);
	g_Time = 0;
	g_nCntTime = 0;
	bool bTimeUp = false;

	//���_�o�b�t�@�̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);

	VERTEX_2D *pVtx;		//���_���փ|�C���^

	//���_�o�b�t�@�����b�N���A���_���փ|�C���^�擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posTime.x + TIME_SIZE_X * nCntTime, 10.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posTime.x + TIME_SIZE_X * (nCntTime + 1), 10.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posTime.x + TIME_SIZE_X * nCntTime, TIME_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posTime.x + TIME_SIZE_X * (nCntTime + 1), TIME_SIZE_Y, 0.0f);

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
	g_pVtxBuffTime->Unlock();

	//�^�C�}�[�̏����ݒ�
	AddTime(120);
}

//----------------------------------------
//	�^�C�}�[�̏I������
//----------------------------------------
void UninitTime(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTime != NULL) 
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

//----------------------------------------
//	�^�C�}�[�̍X�V����
//----------------------------------------
void UpdateTime(void)
{
	//���S���̎擾
	LogoCount *pLogoCount = GetLogoCount();

	if (pLogoCount->bNot)
	{
		g_nCntTime++;
		if (g_nCntTime % 60 == 0)
		{
			AddTime(-1);
		}

		if (g_Time <= 0)
		{
			//�������S�̕\��
			SetLogo(2);
		}
	}
}

//----------------------------------------
//	�^�C�}�[�̕`�揈��
//----------------------------------------
void DrawTime(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTime);

	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			nCntTime * 4,
			2);											//�v���~�e�B�u�i�|���S���j��
	}
}

//----------------------------------------
//	�^�C�}�[�̐ݒ菈��
//----------------------------------------
void SetTime(int nTime)
{
	int aPosTexU[MAX_TIME];	//�e���̐������i�[

	g_Time = nTime;

	aPosTexU[0] = g_Time % 1000 / 100;
	aPosTexU[1] = g_Time % 100 / 10;
	aPosTexU[2] = g_Time % 10 / 1;

	VERTEX_2D*pVtx;		//���_���փ|�C���^

	//���_�o�b�t�@�����b�N���A���_���փ|�C���^�擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntTime], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCntTime] + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntTime], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCntTime] + 1), 1.0f);

		pVtx += 4;//�|�C���^���S���i�߂�
	}

	//���_�o�b�t�@�A�����b�N
	g_pVtxBuffTime->Unlock();
}

//----------------------------------------
//	�^�C�}�[�̉��Z����
//----------------------------------------
void AddTime(int nValue)
{
	int aPosTexU[MAX_TIME];	//�e���̐������i�[

	g_Time += nValue;

	aPosTexU[0] = g_Time % 1000 / 100;
	aPosTexU[1] = g_Time % 100 / 10;
	aPosTexU[2] = g_Time % 10 / 1;

	VERTEX_2D *pVtx;		//���_���փ|�C���^

	//���_�o�b�t�@�����b�N���A���_���փ|�C���^�擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntTime], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCntTime] + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntTime], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCntTime] + 1), 1.0f);

		pVtx += 4;//�|�C���^���S���i�߂�
	}

	//���_�o�b�t�@�A�����b�N
	g_pVtxBuffTime->Unlock();
}