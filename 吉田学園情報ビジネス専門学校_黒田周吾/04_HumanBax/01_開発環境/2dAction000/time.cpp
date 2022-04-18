#include "main.h"
#include "time.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Time g_Time;

//===============================================
//		�X�R�A�̏���������
//===============================================
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	pDevice = GetDevice();			//�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureTime);

	g_Time.pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f - 40.0f, 0.0f, 0.0f);		//�ʒu�̏�����
	g_Time.nData = 0;			//�l�̏�����

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_TIME; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Time.pos.x + TIME_SIZE_X * nCnt, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Time.pos.x + TIME_SIZE_X * (nCnt + 1), 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Time.pos.x + TIME_SIZE_X * nCnt, TIME_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Time.pos.x + TIME_SIZE_X * (nCnt + 1), TIME_SIZE_Y, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;  //���_�f�[�^�̃|�C���^���S���i�߂�
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTime->Unlock();
}

//======================================================
//		�X�R�A�̏I������
//======================================================
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

//======================================================
//		�X�R�A�̍X�V����
//======================================================
void UpdateTime(void)
{
	/*�����ɍX�V����������*/
}

//======================================================
//		�X�R�A�̕`�揈��
//======================================================
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTime);

	for (int nCnt = 0; nCnt < MAX_TIME; nCnt++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			4 * nCnt,		//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);				//�v���~�e�B�u�i�|���S���j��
	}
}

//======================================================
//		�X�R�A�̐ݒ菈��
//======================================================
void SetTime(int nScore)
{
	int aPosTexU[MAX_TIME];		//�e���̐������i�[

	g_Time.nData = nScore;

	aPosTexU[0] = g_Time.nData % 100000000 / 10000000;
	aPosTexU[1] = g_Time.nData % 10000000 / 1000000;
	aPosTexU[2] = g_Time.nData % 1000000 / 100000;
	aPosTexU[3] = g_Time.nData % 100000 / 10000;
	aPosTexU[4] = g_Time.nData % 10000 / 1000;
	aPosTexU[5] = g_Time.nData % 1000 / 100;
	aPosTexU[6] = g_Time.nData % 100 / 10;
	aPosTexU[7] = g_Time.nData % 10 / 1;

	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_TIME; nCnt++)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCnt], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCnt] + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCnt], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * (aPosTexU[nCnt] + 1), 1.0f);

		pVtx += 4;  //���_�f�[�^�̃|�C���^���S���i�߂�
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTime->Unlock();
}

//======================================================
//		�X�R�A�̉��Z����
//======================================================
void AddTime(int nValue)
{
	/*�����ɉ��Z����������*/
}

//======================================================
//		�X�R�A�����擾
//======================================================
int GetTime(void)
{
	return g_Time.nData;
}