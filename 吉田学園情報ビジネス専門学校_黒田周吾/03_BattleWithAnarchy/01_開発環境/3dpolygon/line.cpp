//===================================================================
//
//		���C������[line.cpp]
//		Author:���c ����
//
//===================================================================

//======================================================
//	�C���N���[�h
//======================================================
#include "main.h"
#include "model.h"
#include "line.h"

//======================================================
//	�O���[�o���錾
//======================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLine = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Line g_aLine[MAX_LINE];		//���f���̏��

//======================================================
//	���C���̏���������
//======================================================
void InitLine(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_LINE) * 2 * MAX_LINE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_LINE,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLine,
		NULL);

	//�\���̗̂v�f��������
	ZeroMemory(&g_aLine[0], sizeof(Line));

	VERTEX_LINE *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLine->Lock(0, 0, (void**)&pVtx, 0);

	//�e���̏�����
	for (int nCnt = 0; nCnt < MAX_LINE; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx += 2;
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffLine->Unlock();

	//�ǔz�u
	SetLine(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetLine(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetLine(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetLine(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//======================================================
//	���C���̏I������
//======================================================
void UninitLine(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffLine != NULL)
	{
		g_pVtxBuffLine->Release();
		g_pVtxBuffLine = NULL;
	}
}

//======================================================
//	���C���̍X�V����
//======================================================
void UpdateLine(void)
{
}

//======================================================
//	���C���̕`�揈��
//======================================================
void DrawLine(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	for (int nCnt = 0; nCnt < MAX_LINE; nCnt++)
	{
		if (g_aLine[nCnt].bUse == true)
		{//�e���g�p����Ă���ꍇ
		 //���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aLine[nCnt].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aLine[nCnt].rot.y, g_aLine[nCnt].rot.x, g_aLine[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aLine[nCnt].mtxWorld, &g_aLine[nCnt].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aLine[nCnt].pos.x, g_aLine[nCnt].pos.y, g_aLine[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aLine[nCnt].mtxWorld, &g_aLine[nCnt].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aLine[nCnt].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffLine, 0, sizeof(VERTEX_LINE));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_LINE);

			//�v���C���[�̕`��
			pDevice->DrawPrimitive(D3DPT_LINELIST,		//�v���~�e�B�u�̎��
				nCnt * 2,	//�`�悷��ŏ��̒��_�C���f�b�N�X
				1);			//�v���~�e�B�u�i�|���S���j��
		}
	}
}

//======================================================
//	���C���̐ݒ菈��
//======================================================
void SetLine(D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin)
{
	for (int nCnt = 0; nCnt < MAX_LINE; nCnt++)
	{
		if (g_aLine[nCnt].bUse == false)
		{//�e���g�p����Ă��Ȃ��ꍇ
			g_aLine[nCnt].pos = VtxMax;
			g_aLine[nCnt].rot = VtxMin;
			g_aLine[nCnt].bUse = true;
			break;
		}
	}
}

//======================================================
//	���C���̏����擾
//======================================================
Line *GetLine(void)
{
	return &g_aLine[0];
}