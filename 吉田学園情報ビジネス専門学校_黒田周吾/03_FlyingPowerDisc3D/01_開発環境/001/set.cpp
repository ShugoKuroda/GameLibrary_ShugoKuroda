//========================================================
//
//					���_����Set�J�E���g����
//					AUTHOR:���c����
//
//========================================================
#include "player.h"
#include "set.h"
//#include "game.h"
//#include "result.h"
//#include "fade.h"

//�}�N����`
#define NUM_SET		(2)			//���_�̌�

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureSet = NULL;			//���_�̃e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSet = NULL;		//���_�̒��_���ւ̃|�C���^
Set g_aSet[MAX_PLAYER];							//���_�̏����i�[
bool g_bUseSet;				//���_�̕\���E��\�����Ǘ�
int g_nCntSet;				//�Z�b�g�J�E���^�[�̕\������
	
//========================================================
//					���_�̏���������
//========================================================
void InitSet(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureSet);

	//�O���[�o���ϐ��̏�����
	g_bUseSet = false;						//�\�����Ă��Ȃ���Ԃɂ���
	g_nCntSet = 0;
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_aSet[nCntPlayer].nScore = 0;							//�X�R�A�̏�����
		g_aSet[nCntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu�̏�����
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (NUM_SET * MAX_PLAYER),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSet,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSet->Lock(0, 0, (void**)&pVtx, 0);

	//�v���C���[[1]�̓��_
	for (int nCntScore = 0; nCntScore < NUM_SET; nCntScore++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(320.0f + (80.0f * nCntScore), 450.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(400.0f + (80.0f * nCntScore), 450.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(320.0f + (80.0f * nCntScore), 630.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(400.0f + (80.0f * nCntScore), 630.0f, 0.0f);

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

		//�e�N�X�`���̍��W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	//�v���C���[[2]�̓��_
	for (int nCntScore = 0; nCntScore < NUM_SET; nCntScore++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(800.0f + (80.0f * nCntScore), 450.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(880.0f + (80.0f * nCntScore), 450.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(800.0f + (80.0f * nCntScore), 630.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(880.0f + (80.0f * nCntScore), 630.0f, 0.0f);

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

		//�e�N�X�`���̍��W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	//���_���W���A�����b�N
	g_pVtxBuffSet->Unlock();
}

//========================================================
//					���_�̏I������
//========================================================
void UninitSet(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureSet != NULL)
	{
		g_pTextureSet->Release();
		g_pTextureSet = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffSet != NULL)
	{
		g_pVtxBuffSet->Release();
		g_pVtxBuffSet = NULL;
	}
}

//========================================================
//					���_�̍X�V����
//========================================================
void UpdateSet(void)
{
	if (g_bUseSet)
	{
		g_nCntSet++;
		if (g_nCntSet >= 150)
		{
			g_nCntSet = 0;
			g_bUseSet = false;
		}
	}
}

//========================================================
//					���_�̕`�揈��
//========================================================
void DrawSet(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffSet, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntScore = 0; nCntScore < NUM_SET * MAX_PLAYER; nCntScore++)
	{
		if (g_bUseSet == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureSet);

			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,								//�v���~�e�B�u�̎��
				4 * nCntScore,
				2);													//�v���~�e�B�u�̐�	
		}
	}
}

//========================================================
//				Set���_�̐ݒ菈��
//========================================================
void SetCounter(int nIdxPlayer)
{
	VERTEX_2D *pVtx;
	float aPosTexU[MAX_PLAYER][NUM_SET];							//�e���̐��l���i�[

	g_bUseSet = true;		//�X�R�A���g�p��Ԃɂ���

	if (nIdxPlayer == 0)
	{
		g_aSet[PLAYER_1].nScore++;
	}
	else if (nIdxPlayer == 1)
	{
		g_aSet[PLAYER_2].nScore++;
	}

	//�v���C���[[1]
	aPosTexU[0][0] = ((g_aSet[0].nScore % 100) / 10) * 0.1f;
	aPosTexU[0][1] = (g_aSet[0].nScore % 10) * 0.1f;

	//�v���C���[[2]
	aPosTexU[1][0] = ((g_aSet[1].nScore % 100) / 10) * 0.1f;
	aPosTexU[1][1] = (g_aSet[1].nScore % 10) * 0.1f;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSet->Lock(0, 0, (void**)&pVtx, 0);

	//�v���C���[[1]�̓��_
	for (int nCntScore = 0; nCntScore < NUM_SET; nCntScore++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + aPosTexU[0][nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aPosTexU[0][nCntScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aPosTexU[0][nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aPosTexU[0][nCntScore], 1.0f);

		pVtx += 4;
	}

	//�v���C���[[2]�̓��_
	for (int nCntScore = 0; nCntScore < NUM_SET; nCntScore++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + aPosTexU[1][nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aPosTexU[1][nCntScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aPosTexU[1][nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aPosTexU[1][nCntScore], 1.0f);

		pVtx += 4;
	}

	//���_���W���A�����b�N
	g_pVtxBuffSet->Unlock();
}

//========================================================
//					���U���g�^�C���̕\������
//========================================================
//void DrawResultTime(int nResultTime)
//{
//	VERTEX_2D *pVtx;
//	float aPosTexU[NUM_SCORE];									//�e���̐��l�����[
//
//	//�e���̐��l���v�Z
//	aPosTexU[0] = ((nResultTime % 1000) / 100) * 0.1f;
//	aPosTexU[1] = ((nResultTime % 100) / 10) * 0.1f;
//	aPosTexU[2] = (nResultTime % 10) * 0.1f;
//
//	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
//	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (int nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
//	{
//		//���_���W�̐ݒ�
//		pVtx[0].pos = D3DXVECTOR3(570.0f + (60.0f * nCntScore), 220.0f, 0.0f);
//		pVtx[1].pos = D3DXVECTOR3(630.0f + (60.0f * nCntScore), 220.0f, 0.0f);
//		pVtx[2].pos = D3DXVECTOR3(570.0f + (60.0f * nCntScore), 300.0f, 0.0f);
//		pVtx[3].pos = D3DXVECTOR3(630.0f + (60.0f * nCntScore), 300.0f, 0.0f);
//
//		//���_�J���[�̐ݒ�
//		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//		//�e�N�X�`���̐ݒ�
//		pVtx[0].tex = D3DXVECTOR2(0.0f + aPosTexU[nCntScore], 0.0f);
//		pVtx[1].tex = D3DXVECTOR2(0.1f + aPosTexU[nCntScore], 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(0.0f + aPosTexU[nCntScore], 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(0.1f + aPosTexU[nCntScore], 1.0f);
//
//		pVtx += 4;
//	}
//
//	//���_���W���A�����b�N
//	g_pVtxBuffScore->Unlock();
//
//	DrawScore();
//}